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
  static bool toJson(VariantRef variant, const T& value) {
    return convertToJson(variant, value);  // find by ADL
  }

  static T fromJson(VariantConstRef variant) {
    T value;
    convertFromJson(value, variant);  // find by ADL
    return value;
  }

  static bool checkJson(VariantConstRef variant) {
    T& dummy = *static_cast<T*>(0);
    return canConvertFromJson(dummy, variant);
  }
};

template <typename T>
struct JsonConverter<
    T, typename enable_if<is_integral<T>::value && !is_same<bool, T>::value &&
                          !is_same<char, T>::value>::type> {
  static bool toJson(VariantRef variant, T value) {
    return variantSetInteger<T>(variant._data, value);
  }

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
  static bool toJson(VariantRef variant, T value) {
    return variantSetInteger(variant._data, static_cast<Integer>(value));
  }

  static T fromJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data ? static_cast<T>(data->asIntegral<int>()) : T();
  }

  static bool checkJson(VariantConstRef variant) {
    const VariantData* data = variant._data;
    return data && data->isInteger<int>();
  }
};

template <>
struct JsonConverter<bool> {
  static bool toJson(VariantRef variant, bool value) {
    return variantSetBoolean(variant._data, value);
  }

  static bool fromJson(VariantConstRef variant) {
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
  static bool toJson(VariantRef variant, T value) {
    return variantSetFloat(variant._data, static_cast<Float>(value));
  }

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
  static bool toJson(VariantRef variant, const char* value) {
    return variantSetString(variant._data, adaptString(value), variant._pool);
  }

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
struct JsonConverter<T, typename enable_if<IsString<T>::value>::type> {
  static bool toJson(VariantRef variant, const T& value) {
    return variantSetString(variant._data, adaptString(value), variant._pool);
  }

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
struct JsonConverter<SerializedValue<const char*> > {
  static bool toJson(VariantRef variant, SerializedValue<const char*> value) {
    return variantSetLinkedRaw(variant._data, value);
  }
};

// SerializedValue<std::string>
// SerializedValue<String>
// SerializedValue<const __FlashStringHelper*>
template <typename T>
struct JsonConverter<
    SerializedValue<T>,
    typename enable_if<!is_same<const char*, T>::value>::type> {
  static bool toJson(VariantRef variant, SerializedValue<T> value) {
    return variantSetOwnedRaw(variant._data, value, variant._pool);
  }
};

#if ARDUINOJSON_HAS_NULLPTR

template <>
struct JsonConverter<decltype(nullptr)> {
  static bool toJson(VariantRef variant, decltype(nullptr)) {
    variantSetNull(variant._data);
    return true;
  }
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
