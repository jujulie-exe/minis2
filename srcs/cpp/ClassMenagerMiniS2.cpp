#include "ClassMenagerMiniS2.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <unistd.h>
#include <vector>
/*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::ClassMenagerMiniS2(const json &data, Camera *ptrCamera)
    : _lgpio(-1), _pinVector(data[ConfigKeys::SYSTEM][ConfigKeys::System::GPIO].get<std::vector<int>>()),
     _maskBit(0), _claimPin(false), _systemData(data[ConfigKeys::SYSTEM]), _outputData(data[ConfigKeys::OUTPUT])
     {
  _lgpio = lgGpiochipOpen(_systemData[ConfigKeys::System::CHIPSET]);
  if (_lgpio < 0) {
    throw noOpen();
  }
  if (_pinVector.empty()) {
    throw errorVector();
  }
  _parserVector();
  if (ptrCamera) {
    this->_camera = ptrCamera;
  }
  Logger::log(Logger::INFO, "Chiamata costruttore");
}

ClassMenagerMiniS2::ClassMenagerMiniS2(int ChipSet, const std::vector<int> &pin,
                                       Camera *ptrCamera)
    : _lgpio(-1), _pinVector(pin), _maskBit(0), _claimPin(false) {
  _initSystemData();
  _initOutputData();
  _lgpio = lgGpiochipOpen(ChipSet);
  if (_lgpio < 0) {
    throw noOpen();
  }
  if (_pinVector.empty()) {
    throw errorVector();
  }
  _parserVector();
  if (ptrCamera) {
    this->_camera = ptrCamera;
  }
  Logger::log(Logger::INFO, "Chiamata costruttore");
}
/*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::~ClassMenagerMiniS2() {
  Logger::log(Logger::INFO, "Chiamata decostruttore");
  if (lgGpiochipClose(_lgpio) < 0) {
    Logger::log(Logger::ERROR, "Error: Failed to close GPIO chip");
    // Destructors should not throw exceptions
  }
}
void ClassMenagerMiniS2::_initSystemData(){
  this->_systemData = json::parse(R"({"chipSet":0,"gpio":[],"relay_active_low":true,"settle_time_ms":7000000})");
  // this->_systemData = json::parse("{\"chipSet\": 0, \"gpio\": [], \"relay_active_low\": true, \"settle_time_ms\": 200000}");
  //this->_systemData = json::parse(R"( {"chipSet": 0,"gpio": [], "relay_active_low": true, "settle_time_ms": 200000 }")");
}
void ClassMenagerMiniS2::_initOutputData(){
  this->_outputData = json::parse(R"({
      "save_locally": {
          "enabled": true,
          "directory": "./scans/",
          "prefix": "scan_session_"
      },
      "remote_server": {
          "enabled": false,
          "ip": "192.168.1.1",
          "port": 8080,
          "endpoint": "/upload"
      }
  })");
}
/* ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
 * Initializes and claims the group pins as output.
 * The pins are initialized to HIGH because the relay operates on active-low
 logic
 * (LOW = ON, HIGH = OFF), ensuring devices start in the OFF state.
 ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
*/
int ClassMenagerMiniS2::intClaimPin() {
  //std::vector<const int> v(0, 0);
  int fillValue = _systemData[ConfigKeys::System::RELAY_ACTIVE_LOW] ? HIGH : LOW;
  const std::vector<int> v(_pinVector.size(), fillValue);

  if (lgGroupClaimOutput(_lgpio, 0, _pinVector.size(), _pinVector.data(),
                         v.data()) < 0) {
    return ERROR_NO_CLAIM;
  }
  _claimPin = true;
  _maskBitPin();
  Logger::log(Logger::INFO, "Cliam riuscito!");
  return OK;
}
void ClassMenagerMiniS2::_parserVector() {
  std::sort(_pinVector.begin(), _pinVector.end());
  // maybe check unique??
}

void ClassMenagerMiniS2::_maskBitPin() {
  if (_claimPin) {
    for (size_t i = 0; i < _pinVector.size(); i++) {
      _maskBit |= (1ULL << i);
    }
  }
}

bool ClassMenagerMiniS2::_allPinOn() {
  if (lgGroupWrite(_lgpio, _pinVector[0], _maskBit, _maskBit) < 0) {
    return false;
  }
  return true;
}

bool ClassMenagerMiniS2::_allPinOff() {
  if (lgGroupWrite(_lgpio, _pinVector[0], 0, _maskBit) < 0) {
    return false;
  }
  return true;
}

int ClassMenagerMiniS2::_handelPhotoOrSleep() {
  Logger::log(Logger::INFO, "HandlerPhot");
  if (_camera && (_outputData[ConfigKeys::Output::SAVE_LOCALLY][ConfigKeys::Output::SaveLocally::ENABLED] 
              || _outputData[ConfigKeys::Output::REMOTE_SERVER][ConfigKeys::Output::RemoteServer::ENABLED])) {
    Logger::log(Logger::INFO, "Call->takeAFrame");
    //int ret = this->_camera->takeAFrame(salvataggio, nomeDellaLavorazione, NomeCartella);
    //int ret = this->_camera->takeAFrame(salvataggio, _outputData[ConfigKeys::Output::SAVE_LOCALLY][ConfigKeys::Output::SaveLocally::PREFIX], _outputData[ConfigKeys::Output::SAVE_LOCALLY][ConfigKeys::Output::SaveLocally::DIRECTORY]);
    int ret = this->_camera->takeAFrame();
    if (ret != OK) {
      return (ret);
    }
  } else {
    Logger::log(Logger::INFO, "Sleep");
    usleep(20000); // 200ms
  }
  return (OK);
}
/* ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
 * Executes the chase sequence:
 * - Activates each pin (Logic LOW) sequentially.
 * - Waits 7s, captures a frame, then deactivates (Logic HIGH).
 * - Adds a 2s delay between iterations.
 ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
*/
int ClassMenagerMiniS2::_LOWReleSequentially(){
  for (size_t i = 0; i < _pinVector.size(); i++) {
    uint64_t mask = 1ULL << i;

    if (lgGroupWrite(_lgpio, this->_pinVector[0], LOW, mask) < 0) {
      return (ERROR_NO_WRITE_GROUP);
    }
    {
      std::string msg = "LED " + std::to_string(i) + " ACCESO";
      Logger::log(Logger::INFO, msg.c_str());
    }

    usleep(this->_systemData[ConfigKeys::System::SETTLE_TIME_MS]);
    int ret = _handelPhotoOrSleep();
    if (ret != OK) {
      return (ret);
    }
    // 200ms
    if (lgGroupWrite(_lgpio, this->_pinVector[0], mask, mask) < 0) {
      return (ERROR_NO_WRITE_GROUP);
    }
    {
      std::string msg = "LED " + std::to_string(i) + " SPENTO";
      Logger::log(Logger::INFO, msg.c_str());
    }
  }
  Logger::log(Logger::INFO, "Fine sequenza di acqusizione");
  return (OK);
}
int ClassMenagerMiniS2::_HIGHReleSequentially(){
  for (size_t i = 0; i < _pinVector.size(); i++) {
    uint64_t mask = 1ULL << i;

    if (lgGroupWrite(_lgpio, this->_pinVector[0], mask, mask) < 0) {
      return (ERROR_NO_WRITE_GROUP);
    }
    {
      std::string msg = "LED " + std::to_string(i) + " ACCESO";
      Logger::log(Logger::INFO, msg.c_str());
    }

    usleep(this->_systemData[ConfigKeys::System::SETTLE_TIME_MS]);
    int ret = _handelPhotoOrSleep();
    if (ret != OK) {
      return (ret);
    }
    // 200ms
    if (lgGroupWrite(_lgpio, this->_pinVector[0], LOW, mask) < 0) {
      return (ERROR_NO_WRITE_GROUP);
    }
    {
      std::string msg = "LED " + std::to_string(i) + " SPENTO";
      Logger::log(Logger::INFO, msg.c_str());
    }
  }
  Logger::log(Logger::INFO, "Fine sequenza di acqusizione");
  return (OK);
}

int ClassMenagerMiniS2::sequenceChase() {
  if (!_claimPin || this->_maskBit == 0) {
    return (ERROR_NO_CLAIM);
  }
  Logger::log(Logger::INFO, "Inizio sequenza di acqusizione");
  if (_systemData[ConfigKeys::System::RELAY_ACTIVE_LOW]){
    return _LOWReleSequentially();
  }
  else{
    return _HIGHReleSequentially();
  }
  return OK;
}

static bool checkTipeDataCamera(json dataCamera) {
  if (!dataCamera.contains(ConfigKeys::Camera::PATH)) {
    Logger::log(Logger::ERROR,
                "Error: Missing 'path' key inside camera!");
    return (false);
  }
  if (!dataCamera.contains(ConfigKeys::Camera::WIDTH)) {
    Logger::log(Logger::ERROR,
                "Error: Missing 'width' key inside camera!");
    return (false);
  }
  if (!dataCamera.contains(ConfigKeys::Camera::HEIGHT)) {
    Logger::log(Logger::ERROR,
                "Error: Missing 'height' key inside camera!");
    return (false);
  }
  if (dataCamera[ConfigKeys::Camera::PATH].is_string() &&
      dataCamera[ConfigKeys::Camera::WIDTH].get<int>() >= 0 &&
      dataCamera[ConfigKeys::Camera::HEIGHT].get<int>() >= 0) {
    Logger::log(Logger::INFO, "Camera data type valid!");
    return (true);
  }
  Logger::log(Logger::ERROR, "Invalid camera data type: width/height must be "
                             "integers and path must be string!\n");
  return (false);
}

static bool checkTipeDataSystem(json dataSystem) {
  if (!dataSystem.contains(ConfigKeys::System::CHIPSET)) {
    Logger::log(Logger::ERROR,
                "Error: Missing 'chipSet' key inside System!");
    return (false);
  }
  if (!dataSystem.contains(ConfigKeys::System::GPIO)) {
    Logger::log(Logger::ERROR,
                "Error: Missing 'gpio' key inside System!");
    return (false);
  }
  if (!dataSystem[ConfigKeys::System::GPIO].is_array()) {
    Logger::log(Logger::ERROR, "Error: gpio must be an array type!");
    return (false);
  }
  return (true);
}

int ClassMenagerMiniS2::parserData(json data) {

  if (!data.contains(ConfigKeys::SYSTEM) ||
      !data.contains(ConfigKeys::CAMERA) ||
      !data[ConfigKeys::CAMERA].is_object() ||
      !data[ConfigKeys::SYSTEM].is_object()) {
    Logger::log(Logger::ERROR,
                "Error: 'camera' or 'System' must be a JSON object!  and "
                "contain System: {GPIO: []} and camera: {path: "
                ", width: 0, height: 0}!");
    return 1;
  }

  if (!checkTipeDataSystem(data[ConfigKeys::SYSTEM])) {
    return (1);
  }
  if (!checkTipeDataCamera(data[ConfigKeys::CAMERA])) {
    return (1);
  }

  return 0;
}
