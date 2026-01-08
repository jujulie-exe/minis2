#include "ClassMenagerMiniS2.hpp"
/*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
ClassMenagerMiniS2::ClassMenagerMiniS2(int Handle)
: _lgpio(Handle)
{
    	
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
