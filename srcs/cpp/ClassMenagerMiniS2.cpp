#include "ClassMenagerMiniS2.hpp"
/*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::ClassMenagerMiniS2(int Handle, const std::vector<int> &pin)
: _lgpio(Handle), _pin(pin), _claimPin(false), _maskBit(0)
{
    this->_pinVector = vector<int>(_pin.begin(), _pin.end());
    	
     int h = lgGpiochipOpen(_lgpio);
     if (h < 0)
    {
        throw noOpen();
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
: _lgpio(src._lgpio), _pin(src._pin), _claimPin(false), _maskBit(0)
{
    this->_pinVector = vector<int>(_pin.begin(), _pin.end());
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
void ClassMenagerMiniS2::sequenceChase()
{
    if (!_claimPin) return;

    for (size_t i = 0; i < _pinVector.size(); i++)
    {
        uint64_t mask = 1ULL << i;

        // Accendi il pin i
        lgGroupWrite(_lgpio, _pinVector[0], mask, mask);
        usleep(200000); // 200ms

        // Spegni il pin i
        lgGroupWrite(_lgpio, _pinVector[0], 0, mask);
        usleep(100000); // 100ms
    }
}
/*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2 & ClassMenagerMiniS2::operator=(const ClassMenagerMiniS2 &src)
{
    if (this != &src)
    {
        _lgpio = src._lgpio;
        _pin = src._pin;
        // TODO capire la gestone del claim per i copy e operatore = magari bisogna rilasciarli 
        _claimPin = false;
        this->_pinVector = vector<int>(_pin.begin(), _pin.end());
    }
    return *this;
}
