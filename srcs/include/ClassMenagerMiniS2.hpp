#include <lgpio.h> 
#include <stdexcept>
#include <iostream>
#include <set>
#include <vector>
class ClassMenagerMiniS2
{
        /*♡♡♡♡♡♡♡♡♡♡♡PRIVATE♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    private:
        const int _lgpio;
        const std::set<int> _pin;
        const std::vector<int> _pinVector;
        uint64_t _maskBit;
        bool _claimPin;


        
        /*♡♡♡♡♡♡♡♡♡♡♡HEANDELPIN♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        bool _allPinOn();
        bool _allPinOff();
        bool _onePinOn(int pin);
        bool _onePinOff(int pin);
        void _maskBitPin();




        /*♡♡♡♡♡♡♡♡♡♡♡PUBLIC♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    public:
         /*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2(int Handle, std::vector<int> pin);
         /*♡♡♡♡♡♡♡♡♡♡♡COPY♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2(const ClassMenagerMiniS2 &src);
         /*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2 & operator=(const ClassMenagerMiniS2 &src);

        bool intClaimPin();
        bool sequenzaPin();


         /*♡♡♡♡♡♡♡♡♡♡♡DTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        virtual ~ClassMenagerMiniS2();
    class noOpen : public std::exception
    {
        public:
            const char * what () const throw ()
    };
    class noClose : public std::exception
    {
        public:
            const char * what () const throw ()
    };
};