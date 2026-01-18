
#ifndef CLASSMENAGERMINIS2_HPP
#define CLASSMENAGERMINIS2_HPP

#include <lgpio.h> 
//#include <rgpio.h> 
#include <linux/gpio.h>
#include <stdexcept>
#include <iostream>
#include <set>
#include <vector>
#include "Camera.hpp"
#define ERROR_NO_CLAIM -1
#define ERROR_NO_WRITE_GROUP -2
#define ERROR_FAILURE_CLAIM_GROUP -3
#define ERROR_NO_PHOTO_TAKEN -4
#define OK 0
#define LOW 0
#define HIGH 1
enum class pin : int {
    // TODO mettere solo i pin ch usero nel progetto
    GPIO_5 = 5,
    GPIO_13 = 13,
    GPIO_17 = 17,
    GPIO_22 = 22,
    GPIO_23 = 23,
    GPIO_24 = 24,
    GPIO_25 = 25,
    GPIO_26 = 26,
    GPIO_27 = 27,
};
class ClassMenagerMiniS2
{
        /*♡♡♡♡♡♡♡♡♡♡♡PRIVATE♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    private:
        int _lgpio;
        //const std::set<int> _pin;
        std::vector<int> _pinVector;
        uint64_t _maskBit;
        bool _claimPin;


        
        /*♡♡♡♡♡♡♡♡♡♡♡HEANDELPIN♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        bool _allPinOn();
        bool _allPinOff();
        void _maskBitPin();
		void _parserVector();
		int	_handelPhotoOrSleep();



		//




        /*♡♡♡♡♡♡♡♡♡♡♡PUBLIC♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    public:
         /*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2(int ChipSet, const std::vector<int> &pin, Camera *ptrCamera);
         /*♡♡♡♡♡♡♡♡♡♡♡COPY♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2(const ClassMenagerMiniS2 &src) = delete;
         /*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2 & operator=(const ClassMenagerMiniS2 &src) = delete;

        int	intClaimPin();
        int		sequenceChase();
		Camera *_camera = NULL;


         /*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        virtual ~ClassMenagerMiniS2();
    class noOpen : public std::exception
    {
        public:
            const char * what () const noexcept override { return "Failed to open GPIO chip"; }
    };
    class noClose : public std::exception
    {
        public:
            const char * what () const noexcept override { return "Failed to close GPIO chip"; }
    };
    class errorVector : public std::exception
    {
        public:
            const char * what () const noexcept override { return "Invalid pin vector"; }
    };
};
#endif // ♡♡♡CLASSMENAGERMINIS2_HPP♡♡♡
