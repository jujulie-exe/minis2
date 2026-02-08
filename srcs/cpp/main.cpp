#include "Camera.hpp"
#include "ClassMenagerMiniS2.hpp"
#include "Logger.hpp"

using json = nlohmann::json;

int handelError(int ret) {
  if (ret != OK) {
    // ERROR GPIO
    if (ret == ERROR_NO_CLAIM) {
      Logger::log(Logger::ERROR,
                  "♡ ERROR_NO_CLAIM: GPIO initialization failed. Check if pins "
                  "are available/exported. ♡");
    }
    if (ret == ERROR_NO_WRITE_GROUP) {
      Logger::log(Logger::ERROR, "♡ ERROR_NO_WRITE_GROUP: Writing to GPIO "
                                 "group failed. Check hardware/logic. ♡");
    }
    if (ret == ERROR_FAILURE_CLAIM_GROUP) {
      Logger::log(Logger::ERROR, "♡ ERROR_FAILURE_CLAIM_GROUP: Claiming GPIO "
                                 "group failed. Driver configuration error? ♡");
    }
    // ERROR CAMERA
    if (ret == ERROR_NO_PHOTO_TAKEN) {
      Logger::log(Logger::ERROR, "♡ ERROR_NO_PHOTO_TAKEN: No photo captured. "
                                 "Camera stream might be unresponsive. ♡");
    }
    if (ret == ERROR_SET_FMT) {
      Logger::log(Logger::ERROR, "♡ ERROR_SET_FMT: Camera format setup failed. "
                                 "Check supported resolution/pixel format. ♡");
    }
    if (ret == ERROR_REQ_BUFFER) {
      Logger::log(Logger::ERROR, "♡ ERROR_REQ_BUFFER: Memory buffer request "
                                 "failed. Video device busy or no memory. ♡");
    }
    if (ret == ERROR_QBUF) {
      Logger::log(Logger::ERROR, "♡ ERROR_QBUF: Enqueue buffer failed. Video "
                                 "stream might be interrupted. ♡");
    }
    if (ret == ERROR_DQBUF) {
      Logger::log(Logger::ERROR, "♡ ERROR_DQBUF: Dequeue buffer failed. Cannot "
                                 "retrieve frame from camera. ♡");
    }
    if (ret == ERROR_EPOLL_CREATE) {
      Logger::log(Logger::ERROR,
                  "♡ ERROR_EPOLL_CREATE: System call epoll_create failed. "
                  "Check system resources. ♡");
    }
    if (ret == ERROR_CTL_EPOLL) {
      Logger::log(Logger::ERROR, "♡ ERROR_CTL_EPOLL: Epoll control failed. "
                                 "Camera file descriptor might be closed. ♡");
    }
    if (ret == ERROR_OFSTREM_NON_OPEN) {
      Logger::log(Logger::ERROR, "♡ ERROR_OFSTREM_NON_OPEN: Cannot open output "
                                 "file. Check disk space/permissions. ♡");
    }
    if (ret == TIME_OUT) {
      Logger::log(Logger::ERROR, "♡ TIME_OUT: Camera frame timeout. No data "
                                 "received within limit. Stuck? ♡");
    }
    if (ret == ERROR_NO_SET_CTRL) {
      Logger::log(Logger::WARNING,
                  "♡ ERROR_NO_SET_CTRL: Check if control is available. ♡");
    }
  }
  return ret;
}
Camera *CallCammObj(const json &data) {
  /*
  ♥ User Controls KYO Razer
  ♥ brightness 0x00980900 (int) : min=0 max=255 step=1 default=128 value=128
  ♥ contrast 0x00980901 (int) : min=0 max=255 step=1 default=128 value=128
  ♥ saturation 0x00980902 (int) : min=0 max=255 step=1 default=128 value=128
  ♥ white_balance_automatic 0x0098090c (bool) : default=1 value=1
  ♥ gain 0x00980913 (int) : min=0 max=255 step=1 default=0 value=0
  ♥ power_line_frequency 0x00980918 (menu) : min=0 max=2 default=2 value=1 (50
  Hz)
  ♥ white_balance_temperature 0x0098091a (int) : min=2000 max=7500 step=10
  default=4000 value=4380 flags=inactive
  ♥ sharpness 0x0098091b (int) : min=0  max=255 step=1 default=128 value=117
  ♥ backlight_compensation 0x0098091c (int):  min=0 max=1 step=1 default=0
  value=0 ♥
  ♥ Camera Controls
  ♥ auto_exposure x009a0901 (menu) : min=0 max=3
  default=3 value=3 (Aperture Priority Mode)
  ♥ exposure_time_absolute 0x009a0902 (int) : min=3 max=2047 step=1 default=3
  value=156 flags=inactive ♥ exposure_dynamic_framerate 0x009a0903 (bool) :
  default=0 value=0 ♥ pan_absolute 0x009a0908 (int) : min=-36000 max=36000
  step=3600 default=0 value=0 ♥ tilt_absolute 0x009a0909 (int) : min=-36000
  max=36000 step=3600 default=0 value=0 ♥ focus_absolute 0x009a090a (int) :
  min=0 max=255 step=1 default=0 value=255 flags=inactive ♥
  focus_automatic_continuous 0x009a090c (bool) : default=1 value=1 ♥
  zoom_absolute 0x009a090d (int) : min=100 max=140 step=10 default=100 value=100
  */

  Camera *cam = NULL;
  try {
    cam = new Camera(data["camera"]["path"], data["camera"]["width"],
                     data["camera"]["height"]);
    // ♥♥ Abilita autofocus per la messa a fuoco iniziale
    handelError(cam->setParameters(V4L2_CID_FOCUS_AUTO, ENABLE));
    handelError(
        cam->setParameters(0x009a090c, 0)); // focus_automatic_continuous

    // ♥♥ Imposta frequenza rete elettrica (50Hz)
    handelError(cam->setParameters(V4L2_CID_POWER_LINE_FREQUENCY, 1));

    // ♥♥ Disabilita esposizione automatica e imposta esposizione assoluta
    // (2047)
    handelError(cam->setParameters(V4L2_CID_EXPOSURE_AUTO, DISABLE));
    handelError(cam->setParameters(V4L2_CID_EXPOSURE_ABSOLUTE, 2047));

    // ♥♥ Disabilita bilanciamento bianco automatico e imposta temperatura a
    // 5000K (luce ambiente)
    handelError(cam->setParameters(V4L2_CID_AUTO_WHITE_BALANCE, DISABLE));
    handelError(cam->setParameters(V4L2_CID_WHITE_BALANCE_TEMPERATURE, 5000));

    // ♥♥ Disabilita compensazione retroilluminazione
    handelError(cam->setParameters(V4L2_CID_BACKLIGHT_COMPENSATION, 0));

    usleep(500000); // 500ms per permettere stabilizzazione camera e focus

    handelError(cam->setParameters(
        V4L2_CID_FOCUS_AUTO,
        0)); // ♥♥ Disabilita autofocus per bloccare la messa a fuoco

    Logger::log(Logger::INFO, "Configurazione parametri camera completata♡♡♡♡");
    int ret = 0;
    if ((ret = cam->initV4L2()) == OK) {
      Logger::log(Logger::INFO,
                  "Sottosistema V4L2 inizializzato con successo♡♡♡♡");
    } else {
      handelError(ret);
      delete cam;
      return NULL;
    }
  } catch (const std::exception &e) {
        Logger::log(Logger::ERROR, "Exception: " << e.what() << std::endl;
		
		return NULL;
  }
  return cam;
}

int main() {
  std::srand(std::time(NULL));
  std::ifstream f(".confExampleMiniS2.json");
  if (!f.is_open()) {
    Logger::log(Logger::ERROR, "Failed to open file .confExampleMiniS2.json");
    return 1;
  }
  json data;
  try {
    data = json::parse(f, nullptr, true);
    if (ClassMenagerMiniS2::parserData(data) != 0) {
      return 1;
    }
  } catch (const std::exception &e) {
    Logger::log(Logger::ERROR, "Exception: " << e.what() << std::endl);
    return 1;
  }

  Camera *CameraV4L2 = CallCammObj(data["camera"]);
  if (CameraV4L2 == NULL) {
    Logger::log(Logger::ERROR, "Failed initialized camera♡♡♡♡");
    return 1;
  }
  /* ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
  ClassMenagerMiniS2(int ChipSet, const std::vector<int> &pin, Camera
  *ptrCamera) ♡♡♡♡
  ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
*/
  ClassMenagerMiniS2 *menager = NULL;
  try {
    menager = new ClassMenagerMiniS2(
        data["system"]["chipSet"],
        data["system"]["GPIO"].get<std::vector<int>>(), CameraV4L2);
    int ret = menager->intClaimPin();
    if (handelError(ret) != OK) {
      delete menager;
      delete CameraV4L2;
      return 1;
    }
    ret = menager->sequenceChase();
    if (handelError(ret) != OK) {
      delete menager;
      delete CameraV4L2;
      return 1;
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    delete CameraV4L2;
    return 1;
  }
  delete CameraV4L2;
  delete menager;
  return 0;
}
