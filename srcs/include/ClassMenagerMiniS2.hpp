
#ifndef CLASSMENAGERMINIS2_HPP
#define CLASSMENAGERMINIS2_HPP

#include "ConfigKeys.hpp"
#include "json.hpp"
#include <cstdint>
#include <lgpio.h>
// #include <rgpio.h>
#include "Camera.hpp"
#include <iostream>
#include <linux/gpio.h>
#include <set>
#include <stdexcept>
#include <vector>
using json = nlohmann::json;
#define ERROR_NO_CLAIM -10
#define ERROR_NO_WRITE_GROUP -11
#define ERROR_FAILURE_CLAIM_GROUP -12
#define ERROR_NO_PHOTO_TAKEN -13
#define OK 0
#define LOW 0
#define HIGH 1

enum class pin : int {
  // TODO mettere solo i pin ch usero nel progetto
  GPIO_6 = 6,
  GPIO_13 = 13,
  GPIO_17 = 17,
  GPIO_22 = 22,
  GPIO_23 = 23,
  GPIO_24 = 24,
  GPIO_25 = 25,
  GPIO_26 = 26,
  GPIO_27 = 27,
};
class ClassMenagerMiniS2 {
  /*♡♡♡♡♡♡♡♡♡♡♡PRIVATE♡♡♡♡♡♡♡♡♡♡♡♡♡*/
private:
  int _lgpio;
  // const std::set<int> _pin;
  std::vector<int> _pinVector;
  uint64_t _maskBit;
  bool _claimPin;
  json _systemData;
  json _outputData;
  Camera *_camera = NULL;
  

  /*♡♡♡♡♡♡♡♡♡♡♡HEANDELPIN♡♡♡♡♡♡♡♡♡♡♡♡♡*/
  bool _allPinOn();
  bool _allPinOff();
  void _maskBitPin();
  void _parserVector();
  int _handelPhotoOrSleep();
  void _initSystemData();
  void _initOutputData();
  int _LOWReleSequentially();
  int _HIGHReleSequentially();
  //

  /*♡♡♡♡♡♡♡♡♡♡♡PUBLIC♡♡♡♡♡♡♡♡♡♡♡♡♡*/
public:
  /*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
  ClassMenagerMiniS2(int ChipSet, const std::vector<int> &pin,
                     Camera *ptrCamera);
  ClassMenagerMiniS2(const json &data, Camera *ptrCamera);
  /*♡♡♡♡♡♡♡♡♡♡♡COPY♡♡♡♡♡♡♡♡♡♡♡♡♡*/
  ClassMenagerMiniS2(const ClassMenagerMiniS2 &src) = delete;
  /*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
  ClassMenagerMiniS2 &operator=(const ClassMenagerMiniS2 &src) = delete;

  static int parserData(json data);
  int intClaimPin();
  int sequenceChase();
  //TODO esporre i metodi di di cam SetParametre e Get
  

  /*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
  virtual ~ClassMenagerMiniS2();
  class noOpen : public std::exception {
  public:
    const char *what() const noexcept override {
      return "Failed to open GPIO chip";
    }
  };
  class noClose : public std::exception {
  public:
    const char *what() const noexcept override {
      return "Failed to close GPIO chip";
    }
  };
  class errorVector : public std::exception {
  public:
    const char *what() const noexcept override { return "Invalid pin vector"; }
  };
};
#endif // ♡♡♡CLASSMENAGERMINIS2_HPP♡♡♡
