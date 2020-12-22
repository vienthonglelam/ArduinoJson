// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <malloc.h>
#include <setjmp.h>
#include <catch.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4611)  // interaction between '_setjmp' and C++ object
                                 // destruction is non-portable
#endif

size_t STACK = 1000;

class IncrementalDeserializer {
  struct Reader {
    IncrementalDeserializer* _parent;

   public:
    Reader(IncrementalDeserializer* parent) : _parent(parent) {}

    Reader(Reader& other) : _parent(other._parent) {}

    int read() {
      // std::cout << "read()" << std::endl;
      if (_parent->_size == 0)
        pause();
      // std::cout << "avail = " << _parent->_size << std::endl;
      // std::cout << "avail = " << _parent->_data << std::endl;
      int result = _parent->_data[0];
      consume(1);
      // std::cout << "read() -> " << result << std::endl;
      return result;
    }

    size_t readBytes(char* buffer, size_t length) {
      // std::cout << "readBytes()" << std::endl;
      if (_parent->_size == 0)
        pause();
      if (length > _parent->_size)
        length = _parent->_size;
      memcpy(buffer, _parent->_data, length);
      consume(length);
      return length;
    }

   private:
    void pause() {
      // std::cout << "pause" << std::endl;
      if (setjmp(_parent->_back_to_reader) == 0) {
        // std::cout << "paused" << std::endl;
        longjmp(_parent->_back_to_program, 1);
      }
      // std::cout << "resumed" << std::endl;
    }

    void consume(size_t n) {
      _parent->_data += n;
      _parent->_size -= n;
    }
  };

  jmp_buf _back_to_program;
  jmp_buf _back_to_reader;
  JsonDocument* _doc;
  const char* _data;
  size_t _size;
  DeserializationError _result;

 public:
  IncrementalDeserializer(JsonDocument& doc) : _doc(&doc) {}

  void start() {
    // std::cout << "start()" << std::endl;
    _data = 0;
    _size = 0;
    _result = DeserializationError::IncompleteInput;
    if (setjmp(_back_to_program) == 0) {
      char space[STACK];     // Reserve enough space for main to run
      space[STACK - 1] = 1;  // Do not optimize array out of existence
      (void)space;
      Reader reader(this);
      _result = deserializeJson(*_doc, reader);
      longjmp(_back_to_program, 2);
    }
  }

  bool feed(const char* data, size_t size) {
    // std::cout << "feed()" << std::endl;
    _data = data;
    _size = size;
    switch (setjmp(_back_to_program)) {
      case 0:
        longjmp(_back_to_reader, 1);
      case 1:
        // std::cout << "feed() -> false" << std::endl;
        return false;  // need more data
      default:
        // std::cout << "feed() -> true" << std::endl;
        return true;  // done
    }
  }

  DeserializationError result() const {
    return _result;
  }
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

TEST_CASE("prout") {
  StaticJsonDocument<1024> doc;
  IncrementalDeserializer deserializer(doc);

  SECTION("basics") {
    deserializer.start();
    REQUIRE(deserializer.feed("{\"answ", 6) == false);
    REQUIRE(deserializer.feed("er\":42}", 7) == true);
    REQUIRE(doc["answer"] == 42);
  }
}
