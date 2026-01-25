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
        // Destructors should not throw exceptions
        std::cerr << "Error: Failed to close GPIO chip" << std::endl;
    }
}


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


