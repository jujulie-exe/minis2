#include "ClassMenagerMiniS2.hpp"
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
}
/*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::~ClassMenagerMiniS2()
{
    if (lgGpiochipClose(_lgpio) < 0)
    {
        // Destructors should not throw exceptions
        std::cerr << "Error: Failed to close GPIO chip" << std::endl;
    }
}


bool ClassMenagerMiniS2::intClaimPin()
{
    if( lgGroupClaimOutput(
        _lgpio,
        0,
        _pinVector.size(),
        _pinVector.data(),
        0
    ) < 0)
    {
        return false;
    }
    _claimPin = true;
    _maskBitPin();
    return true;
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
	if (_camera) {
		if (!this->_camera->takeAFrame()){
			return (ERROR_NO_PHOTO_TAKEN);
		}
	}
	else {
		usleep(200000); // 200ms
	}
	return (OK);

}

int ClassMenagerMiniS2::sequenceChase()
{
    if (!_claimPin || this->_maskBit != 0) return (ERROR_NO_CLAIM);


	if (!_allPinOn()){
		return (ERROR_NO_WRITE_GROUP);
	}
	if (_handelPhotoOrSleep() != 0){
		return (ERROR_NO_PHOTO_TAKEN);
	}
	if (!_allPinOff()){
		return (ERROR_NO_WRITE_GROUP);
	}

    for (size_t i = 0; i < _pinVector.size(); i++){
		uint64_t mask = 1ULL << i;

        if (lgGroupWrite(_lgpio, this->_pinVector[0], mask, mask) < 0){
			return (ERROR_NO_WRITE_GROUP);
		}

		if (_handelPhotoOrSleep() != 0){
			return (ERROR_NO_PHOTO_TAKEN);
		}

        if (lgGroupWrite(_lgpio, this->_pinVector[0], 0, mask) < 0){
			return (ERROR_NO_WRITE_GROUP);
		}
		usleep(200000); // 200ms
    }
	return (OK);
}


