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

template <typename T>
inline T variantAs(VariantConstRef variant);

template <typename T>
T variantAs(VariantRef variant);

}  // namespace ARDUINOJSON_NAMESPACE
