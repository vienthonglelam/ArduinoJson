// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/IsWriteableString.hpp>
#include <ArduinoJson/Variant/VariantFunctions.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <>
struct JsonConverter<ArrayConstRef, void> {
  typedef ArrayConstRef type;
  static ArrayConstRef get(const VariantData* _data) {
    return ArrayConstRef(variantAsArray(_data));
  }
};

template <>
struct JsonConverter<ObjectConstRef, void> {
  typedef ObjectConstRef type;
  static ObjectConstRef get(const VariantData* _data) {
    return ObjectConstRef(variantAsObject(_data));
  }
};

template <>
struct JsonConverter<VariantConstRef, void> {
  typedef VariantConstRef type;
  static VariantConstRef get(const VariantData* _data) {
    return VariantConstRef(_data);
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<IsWriteableString<T>::value>::type> {
  typedef T type;

  static T get(const VariantData* _data) {
    const char* cstr = _data != 0 ? _data->asString() : 0;
    if (cstr)
      return T(cstr);
    T s;
    serializeJson(VariantConstRef(_data), s);
    return s;
  }
};

template <>
inline ArrayRef variantAs<ArrayRef>(VariantData* data, MemoryPool* pool) {
  return ArrayRef(pool, data != 0 ? data->asArray() : 0);
}

template <>
inline ObjectRef variantAs<ObjectRef>(VariantData* data, MemoryPool* pool) {
  return ObjectRef(pool, data != 0 ? data->asObject() : 0);
}

template <>
inline VariantRef variantAs<VariantRef>(VariantData* data, MemoryPool* pool) {
  return VariantRef(pool, data);
}

}  // namespace ARDUINOJSON_NAMESPACE
