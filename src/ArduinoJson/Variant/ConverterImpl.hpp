// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/IsWriteableString.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename Enable>
struct JsonConverter {
  static T fromJson(VariantConstRef variant) {
    T value;
    convertFromJson(value, variant);  // find by ADL
    return value;
  }
};

template <typename T>
struct JsonConverter<
    T, typename enable_if<is_integral<T>::value && !is_same<bool, T>::value &&
                          !is_same<char, T>::value>::type> {
  static T fromJson(VariantConstRef variant) {
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    const VariantData* data = variant._data;
    return data ? data->asIntegral<T>() : T();
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isInteger<T>();
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_enum<T>::value>::type> {
  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? static_cast<T>(data->asIntegral<int>()) : T();
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isInteger<int>();
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_same<T, bool>::value>::type> {
  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? data->asBoolean() : false;
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isBoolean();
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_floating_point<T>::value>::type> {
  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? data->asFloat<T>() : false;
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isFloat();
  }
};

template <>
struct JsonConverter<const char*> {
  static const char* fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? data->asString() : 0;
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isString();
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<IsWriteableString<T>::value>::type> {
  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    const char* cstr = data != 0 ? data->asString() : 0;
    if (cstr)
      return T(cstr);
    T s;
    serializeJson(variant, s);
    return s;
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isString();
  }
};

template <>
struct JsonConverter<ArrayConstRef> {
  static ArrayConstRef fromJson(VariantConstRef variant) {
    return ArrayConstRef(variantAsArray(variant._data));
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isArray();
  }
};

template <>
struct JsonConverter<ObjectConstRef> {
  static ObjectConstRef fromJson(VariantConstRef variant) {
    return ObjectConstRef(variantAsObject(variant._data));
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isObject();
  }
};

template <>
struct JsonConverter<VariantConstRef> {
  static VariantConstRef fromJson(VariantConstRef variant) {
    return VariantConstRef(variant._data);
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data;
  }
};

template <>
struct JsonConverter<ArrayRef> {
  static ArrayRef fromJson(VariantRef variant) {
    VariantData* data = variant._data;
    MemoryPool* pool = variant._pool;
    return ArrayRef(pool, data != 0 ? data->asArray() : 0);
  }

  static bool checkJson(VariantConstRef) {
    return false;
  }

  static bool checkJson(VariantRef variant) {
    VariantData* data = variant._data;
    return data && data->isArray();
  }
};

template <>
struct JsonConverter<ObjectRef> {
  static ObjectRef fromJson(VariantRef variant) {
    VariantData* data = variant._data;
    MemoryPool* pool = variant._pool;
    return ObjectRef(pool, data != 0 ? data->asObject() : 0);
  }

  static bool checkJson(VariantConstRef) {
    return false;
  }

  static bool checkJson(VariantRef variant) {
    VariantData* data = variant._data;
    return data && data->isObject();
  }
};

template <>
struct JsonConverter<VariantRef> {
  static VariantRef fromJson(VariantRef variant) {
    return variant;
  }
  static bool checkJson(VariantRef variant) {
    VariantData* data = variant._data;
    return data;
  }
  static bool checkJson(VariantConstRef) {
    return false;
  }
};

#if ARDUINOJSON_HAS_NULLPTR

template <>
struct JsonConverter<decltype(nullptr)> {
  static decltype(nullptr) fromJson(VariantConstRef) {
    return nullptr;
  }
  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data == 0 || data->isNull();
  }
};

#endif

template <typename T>
T variantAs(VariantConstRef variant) {
  return JsonConverter<T>::fromJson(variant);
}

template <typename T>
T variantAs(VariantRef variant) {
  return JsonConverter<T>::fromJson(variant);
}

template <typename T>
bool variantIs(VariantConstRef variant) {
  return JsonConverter<T>::checkJson(variant);
}

template <typename T>
bool variantIs(VariantRef variant) {
  return JsonConverter<T>::checkJson(variant);
}

}  // namespace ARDUINOJSON_NAMESPACE
