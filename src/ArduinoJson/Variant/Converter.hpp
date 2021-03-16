// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2021
// MIT License

#pragma once

#include <ArduinoJson/Strings/IsWriteableString.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename T, typename Enable = void>
struct Converter;

// ---

// TODO: move
inline bool variantIsNull(const VariantData *var) {
  return var == 0 || var->isNull();
}

}  // namespace ARDUINOJSON_NAMESPACE
