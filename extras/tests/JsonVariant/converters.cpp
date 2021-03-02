// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#include <ArduinoJson.h>
#include <stdint.h>
#include <catch.hpp>

namespace {
struct Date {
  int day;
  int month;
  int year;
};

template <>
struct JsonConverter<Date> {
  static Date fromJson(JsonVariantConst variant) {
    return {
      variant["day"], variant["month"], variant["year"],
    }
  }
};

}  // namespace

TEST_CASE("Custom converters") {
  DynamicJsonDocument doc(4096);

  SECTION("convert JsonVariant to Date") {
    doc["date"]["day"] = 2;
    doc["date"]["month"] = 3;
    doc["date"]["year"] = 2021;

    Date date = doc["date"];

    REQUIRE(date.day, 2)
    REQUIRE(date.month, 3)
    REQUIRE(date.year, 2021)
  }

  SECTION("isNull() return true") {
    var.add("hello");
    var.clear();

    REQUIRE(var.isNull() == true);
  }
}
