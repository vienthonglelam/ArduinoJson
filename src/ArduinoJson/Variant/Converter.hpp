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
  return JsonConverter<T>::fromJson(VariantConstRef(data));
}

template <typename T>
T variantAs(VariantData* data, MemoryPool* pool) {
  return JsonConverter<T>::fromJson(VariantRef(pool, data));
}

}  // namespace ARDUINOJSON_NAMESPACE
