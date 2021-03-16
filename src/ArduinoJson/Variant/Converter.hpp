// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/IsWriteableString.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

// TODO: remove
class VariantRef;
class VariantConstRef;

template <typename T, typename Enable = void>
struct Converter;

// ---

// TODO: inline
template <typename T>
inline T variantAs(VariantConstRef variant);

template <typename T>
T variantAs(VariantRef variant);

template <typename T>
inline bool variantIs(VariantConstRef variant);

template <typename T>
bool variantIs(VariantRef variant);

inline bool variantIsNull(const VariantData *var) {
  return var == 0 || var->isNull();
}

}  // namespace ARDUINOJSON_NAMESPACE
