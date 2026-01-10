#include <lgpio.h> 
#include <stdexcept>
#include <iostream>
#include <set>
#include <vector>
#include "Camera.hpp"
enum error{
	ERROR_NO_CLAIM,
	ERROR_NO_WRITE_GROUP,
	ERROR_FAILURE_CLAIM_GROUP,
	ERROR_NO_PHOTO_TAKEN,
	OK = 0,
};
enum class pin : int {
    PIN_0 = 0,
    PIN_1 = 1,
    PIN_2 = 2,
    PIN_3 = 3,
    PIN_4 = 4,
    PIN_5 = 5,
    PIN_6 = 6,
    PIN_7 = 7,
    PIN_8 = 8,
    PIN_9 = 9,
    PIN_10 = 10,
    PIN_11 = 11,
    PIN_12 = 12,
    PIN_13 = 13,
    PIN_14 = 14,
    PIN_15 = 15,
    PIN_16 = 16,
    PIN_17 = 17,
    PIN_18 = 18,
    PIN_19 = 19,
    PIN_20 = 20,
    PIN_21 = 21,
    PIN_22 = 22,
    PIN_23 = 23,
    PIN_24 = 24,
    PIN_25 = 25,
    PIN_26 = 26,
    PIN_27 = 27,
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
