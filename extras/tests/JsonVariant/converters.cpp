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

void convertFromJson(Date& date, JsonVariantConst variant) {
  date.day = variant["day"];
  date.month = variant["month"];
  date.year = variant["year"];
}
}  // namespace

TEST_CASE("Custom converters") {
  DynamicJsonDocument doc(4096);

  SECTION("convert JsonVariant to Date") {
    doc["date"]["day"] = 2;
    doc["date"]["month"] = 3;
    doc["date"]["year"] = 2021;

    Date date = doc["date"];

    REQUIRE(date.day == 2);
    REQUIRE(date.month == 3);
    REQUIRE(date.year == 2021);
  }
}
