#include "ClassMenagerMiniS2.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <unistd.h>
/*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::ClassMenagerMiniS2(int ChipSet, const std::vector<int> &pin, Camera *ptrCamera)
: _lgpio(-1), _pinVector(pin), _maskBit(0), _claimPin(false)
{
    	
     _lgpio = lgGpiochipOpen(ChipSet);
     if (_lgpio < 0)
    {
        throw noOpen();
    }
	if (_pinVector.empty())
	{
		throw errorVector();
	}
	_parserVector(); 
	if (ptrCamera)
	{
		this->_camera = ptrCamera;
	}
	Logger::log(Logger::INFO, "Chiamata costruttore♡♡♡♡♡♡♡♡♡♡♡");
}
/*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::~ClassMenagerMiniS2()
{
	Logger::log(Logger::INFO, "Chiamata decostruttore♡♡♡♡♡♡♡♡♡♡♡");
    if (lgGpiochipClose(_lgpio) < 0)
    {
		Logger::log(Logger::ERROR, "Error: Failed to close GPIO chip♡♡♡♡");
        // Destructors should not throw exceptions
    }
}

    /* ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
     * Initializes and claims the group pins as output.
     * The pins are initialized to HIGH because the relay operates on active-low logic
     * (LOW = ON, HIGH = OFF), ensuring devices start in the OFF state.
     ♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡♡
   */
int ClassMenagerMiniS2::intClaimPin()
{
	//std::vector<const int> v(8, 1);
	std::vector<int> v(_pinVector.size(), HIGH);
    if( lgGroupClaimOutput(
        _lgpio,
        0,
        _pinVector.size(),
        _pinVector.data(),
		v.data()
    ) < 0)
    {
        return ERROR_NO_CLAIM;
    }
    _claimPin = true;
    _maskBitPin();
	Logger::log(Logger::INFO, "Cliam riuscito! ♡♡♡♡♡♡♡♡♡♡♡");
    return OK;
}
void ClassMenagerMiniS2::_parserVector(){
	std::sort(_pinVector.begin(), _pinVector.end());
	// maybe check unique?? 
}

void ClassMenagerMiniS2::_maskBitPin()
{
    if (_claimPin)
    {
        for (size_t i = 0; i < _pinVector.size(); i++)
        {
            _maskBit |= (1ULL << i);
        }
    }
}

bool ClassMenagerMiniS2::_allPinOn()
{
    if (lgGroupWrite(_lgpio, _pinVector[0], _maskBit, _maskBit)  < 0)
    {
        return false;
    }
    return true;
}

bool ClassMenagerMiniS2::_allPinOff()
{
    if (lgGroupWrite(_lgpio, _pinVector[0], 0, _maskBit)  < 0)
    {
        return false;
    }
    return true;
}

int	ClassMenagerMiniS2::_handelPhotoOrSleep()
{
	Logger::log(Logger::INFO, "HandlerPhot ♡♡♡♡♡♡♡♡♡♡♡");
	if (_camera) {
		Logger::log(Logger::INFO, "Call->takeAFrame ♡♡♡♡♡♡♡♡♡♡♡");
		int ret = this->_camera->takeAFrame();
		if (ret != OK){
			return (ret);
		}
	}
	else {
		Logger::log(Logger::INFO, "Sleep ♡♡♡♡♡♡♡♡♡♡♡");
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
int ClassMenagerMiniS2::sequenceChase()
{
    if (!_claimPin || this->_maskBit == 0){
	    return (ERROR_NO_CLAIM);
    }
 	Logger::log(Logger::INFO, "Inizio sequenza di acqusizione ♡♡♡♡♡♡♡♡♡♡♡");

	/*if (!_allPinOn()){
		return (ERROR_NO_WRITE_GROUP);
	}
	if (_handelPhotoOrSleep() != 0){
		return (ERROR_NO_PHOTO_TAKEN);
	}
	if (!_allPinOff()){
		return (ERROR_NO_WRITE_GROUP);
	} */
	// setto il focus della camere per avere tutte le immagini uguali
	(_camera->setParameters(V4L2_CID_FOCUS_AUTO, DISABLE));
	/* for (size_t i = 0; i <= 16; ++i)
	{
		_handelPhotoOrSleep();
	}*/
	(_camera->setParameters(V4L2_CID_FOCUS_AUTO, ENABLE));
    for (size_t i = 0; i < _pinVector.size(); i++){
		uint64_t mask = 1ULL << i;

        if (lgGroupWrite(_lgpio, this->_pinVector[0], LOW, mask) < 0){
			return (ERROR_NO_WRITE_GROUP);
		}
		{
		std::string msg = "LED " + std::to_string(i) + " ACCESO (Logic LOW) ♡♡♡";
		Logger::log(Logger::INFO, msg.c_str());
		}
		
		usleep(7000000);
		int ret = _handelPhotoOrSleep();
		if (ret != OK){
			return (ret);
		}
		 // 200ms
        if (lgGroupWrite(_lgpio, this->_pinVector[0], mask, mask) < 0){
			return (ERROR_NO_WRITE_GROUP);
		}
		{
		std::string msg = "LED " + std::to_string(i) + " SPENTO (Logic HIGH) ♡♡♡";
		Logger::log(Logger::INFO, msg.c_str());
		}
		
		usleep(2000000); // 200ms
    }
	Logger::log(Logger::INFO, "Fine sequenza di acqusizione ♡♡♡♡♡♡♡♡♡♡♡");
	return (OK);
}
static bool checkTipeDataCamera(json dataCamera){
	if(!dataCamera.contains("path")){
		Logger::log(Logger::ERROR, "Error: Missing 'path' key inside camera! ♡♡♡♡♡♡♡");
		return (false);
	}
	if(!dataCamera.contains("width")){
		Logger::log(Logger::ERROR, "Error: Missing 'width' key inside camera! ♡♡♡♡♡♡♡");
		return (false);
	}
	if(!dataCamera.contains("height")){
		Logger::log(Logger::ERROR, "Error: Missing 'height' key inside camera! ♡♡♡♡♡♡♡");
		return (false);
	}
	if (dataCamera["path"].is_string() &&
		dataCamera["width"].is_number() &&
		dataCamera["height"].is_number()){
			Logger::log(Logger::INFO, "Camera data type valid!");
			return (true);
		}
		Logger::log(Logger::ERROR, "Invalid camera data type: width/height must be integers and path must be string!\n");
		return (false);
}

static bool checkTipeDataSystem(json dataSystem){
	if(!dataSystem.contains("chipSet")){
		Logger::log(Logger::ERROR, "Error: Missing 'chipSet' key inside system! ♡♡♡♡♡♡♡");
		return (false);
	}
	if(!dataSystem.contains("GPIO")){
		Logger::log(Logger::ERROR, "Error: Missing 'GPIO' key inside system! ♡♡♡♡♡♡♡");
		return (false);
	}
	if(!dataSystem["GPIO"].is_array()){
		Logger::log(Logger::ERROR, "Error: GPIO must be an array type! ♡♡♡♡♡♡♡");
		return (false);
	}
	return (true);
}
int ClassMenagerMiniS2::parserData(json data){

    if (!data.contains("camera") || !data.contains("system") || !data["camera"].is_object() || !data["system"].is_object()) {
        Logger::log(Logger::ERROR, "Error: 'camera' or 'system' must be a JSON object!  and contain system: {GPIO: []} and camera: {path: "", width: 0, height: 0}! ♡♡♡♡♡♡♡");
        return 1;
    }

    if (!checkTipeDataSystem(data["system"])){
        return (1);
    }
	if (!checkTipeDataCamera(data["camera"])){
		return (1);
	}

	return 0; 
}

