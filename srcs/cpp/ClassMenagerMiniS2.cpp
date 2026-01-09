#include "ClassMenagerMiniS2.hpp"
/*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::ClassMenagerMiniS2(int Handle, const std::vector<int> &pin, Camera *ptrCamera = NULL)
: _lgpio(Handle), _pinVector(pin), _claimPin(false), _maskBit(0)
{
    	
     int h = lgGpiochipOpen(_lgpio);
     if (h < 0)
    {
        throw noOpen();
    }
	if (_pinVector.empty())
	{
		throw errorVector();
	}
	_parserVector() 
	if (ptrCamera)
	{
		this->Camera = ptrCamera;
	}
}
/*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::~ClassMenagerMiniS2()
{
    if (lgGpiochipClose(_lgpio) < 0)
    {
        throw noClose();
    }
}
/*♡♡♡♡♡♡♡♡♡♡♡COPY♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::ClassMenagerMiniS2(const ClassMenagerMiniS2 &src)
: _lgpio(src._lgpio), _pinVector(src._pinVector), _claimPin(false), _maskBit(0)
{
    // TODO capire la gestone del claim per i copy e operatore =
}

bool ClassMenagerMiniS2::intClaimPin()
{
    if( lgGpioGroupClaimOutput(
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
	if (Camera) {
		if (this->Camera.TakePhoto() < 0){
			return (ERROR_NO_PHOTO_TAKEN);
		}
	}
	else {
		usleep(200000); // 200ms
	}
	return (OK);

}

int ClassMenagerMiniS2::sequenceChase() const
{
    if (!_claimPin || this->_maskBitPin != 0) return (ERROR_NO_CLAIM);


	if (!_allPinOn()){
		return (ERROR_NO_WRITE_GROUP)
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
			return (ERROR_NO_WRITE_GROUP)
		}

		if (_handelPhotoOrSleep() != 0){
			return (ERROR_NO_PHOTO_TAKEN);
		}

        if (lgGroupWrite(_lgpio, this->_pinVector[0], 0, mask) < 0){
			return (ERROR_NO_WRITE_GROUP)
		}
		usleep(200000); // 200ms
    }
	return (OK);
}

/*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2 & ClassMenagerMiniS2::operator=(const ClassMenagerMiniS2 &src)
{
    if (this != &src)
    {
        _lgpio = src._lgpio;
        _pinVector = src._pinVector;
        // TODO capire la gestone del claim per i copy e operatore = magari bisogna rilasciarli 
        _claimPin = false;
    }
    return *this;
}
