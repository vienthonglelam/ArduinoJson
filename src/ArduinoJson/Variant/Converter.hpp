// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/IsWriteableString.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

class ArrayRef;
class ArrayConstRef;
class ObjectRef;
class ObjectConstRef;
class VariantRef;
class VariantConstRef;

template <typename T, typename Enable = void>
struct JsonConverter;

template <typename T>
struct JsonConverter<
    T, typename enable_if<is_integral<T>::value && !is_same<bool, T>::value &&
                          !is_same<char, T>::value>::type> {
  static T fromJson(const VariantData* data) {
    ARDUINOJSON_ASSERT_INTEGER_TYPE_IS_SUPPORTED(T);
    return data->asIntegral<T>();
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_enum<T>::value>::type> {
  static T fromJson(const VariantData* data) {
    return static_cast<T>(data->asIntegral<int>());
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_same<T, bool>::value>::type> {
  static T fromJson(const VariantData* data) {
    return data->asBoolean();
  }
};

template <typename T>
struct JsonConverter<T, typename enable_if<is_floating_point<T>::value>::type> {
  static T fromJson(const VariantData* data) {
    return data->asFloat<T>();
  }
};

template <>
struct JsonConverter<const char*> {
  static const char* fromJson(const VariantData* data) {
    return data->asString();
  }
};

template <typename T>
struct VariantConstAs {
  typedef typename JsonConverter<T>::type type;
};

template <>
struct VariantConstAs<VariantRef> {
  typedef VariantConstRef type;
};

template <>
struct VariantConstAs<ObjectRef> {
  typedef ObjectConstRef type;
};

template <>
struct VariantConstAs<ArrayRef> {
  typedef ArrayConstRef type;
};

// ---

template <>
struct JsonConverter<ArrayConstRef>;

template <>
struct JsonConverter<ObjectConstRef>;

template <>
struct JsonConverter<VariantConstRef>;

template <typename T>
struct JsonConverter<T, typename enable_if<IsWriteableString<T>::value>::type>;

template <typename T>
inline T variantAs(const VariantData* data) {
  return data != 0 ? JsonConverter<T>::fromJson(data) : T();
}

template <typename T>
T variantAs(VariantData* data, MemoryPool*) {
  // By default use the read-only conversion.
  // There are specializations for
  // - ArrayRef
  return variantAs<T>(data);
}

}  // namespace ARDUINOJSON_NAMESPACE
