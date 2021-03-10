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
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_enum<T>::value>::type> {
  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? static_cast<T>(data->asIntegral<int>()) : T();
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_same<T, bool>::value>::type> {
  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? data->asBoolean() : false;
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_floating_point<T>::value>::type> {
  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? data->asFloat<T>() : false;
  }
};

template <>
struct JsonConverter<const char*> {
  static const char* fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? data->asString() : 0;
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<IsWriteableString<T>::value>::type> {
  typedef T type;

  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    const char* cstr = data != 0 ? data->asString() : 0;
    if (cstr)
      return T(cstr);
    T s;
    serializeJson(variant, s);
    return s;
  }
};

template <>
struct JsonConverter<ArrayConstRef> {
  typedef ArrayConstRef type;
  static ArrayConstRef fromJson(VariantConstRef variant) {
    return ArrayConstRef(variantAsArray(variant._data));
  }
};

template <>
struct JsonConverter<ObjectConstRef> {
  typedef ObjectConstRef type;
  static ObjectConstRef fromJson(VariantConstRef variant) {
    return ObjectConstRef(variantAsObject(variant._data));
  }
};

template <>
struct JsonConverter<VariantConstRef> {
  typedef VariantConstRef type;
  static VariantConstRef fromJson(VariantConstRef variant) {
    return VariantConstRef(variant._data);
  }
};

template <>
struct JsonConverter<ArrayRef> {
  static ArrayRef fromJson(VariantRef variant) {
    VariantData* data = variant._data;
    MemoryPool* pool = variant._pool;
    return ArrayRef(pool, data != 0 ? data->asArray() : 0);
  }
};

template <>
struct JsonConverter<ObjectRef> {
  static ObjectRef fromJson(VariantRef variant) {
    VariantData* data = variant._data;
    MemoryPool* pool = variant._pool;
    return ObjectRef(pool, data != 0 ? data->asObject() : 0);
  }
};

template <>
struct JsonConverter<VariantRef> {
  static VariantRef fromJson(VariantRef variant) {
    return variant;
  }
};

template <typename T>
inline T variantAs(VariantConstRef variant) {
  return JsonConverter<T>::fromJson(variant);
}

template <typename T>
T variantAs(VariantRef variant) {
  return JsonConverter<T>::fromJson(variant);
}

}  // namespace ARDUINOJSON_NAMESPACE
