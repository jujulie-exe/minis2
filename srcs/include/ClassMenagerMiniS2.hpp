
#ifndef CLASSMENAGERMINIS2_HPP
#define CLASSMENAGERMINIS2_HPP

#include <lgpio.h> 
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
enum class pin : int {
    // TODO mettere solo i pin ch usero nel progetto
    pin_17 = 17,
    pin_27 = 27,
    pin_22 = 22,
    pin_23 = 23,
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

        bool	intClaimPin();
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
