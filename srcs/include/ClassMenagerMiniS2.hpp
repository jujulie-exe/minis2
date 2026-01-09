#include <lgpio.h> 
#include <stdexcept>
#include <iostream>
#include <set>
#include <vector>
enum error{
	ERROR_NO_CLAIM,
	ERROR_NO_WRITE_GROUP,
	ERROR_FAILURE_CLAIM_GROUP,
	OK = 0,
};
class ClassMenagerMiniS2
{
        /*♡♡♡♡♡♡♡♡♡♡♡PRIVATE♡♡♡♡♡♡♡♡♡♡♡♡♡*/
    private:
        const int _lgpio;
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
        ClassMenagerMiniS2(int Handle, std::vector<int> pin);
         /*♡♡♡♡♡♡♡♡♡♡♡COPY♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2(const ClassMenagerMiniS2 &src);
         /*♡♡♡♡♡♡♡♡♡♡♡OPERATOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2 & operator=(const ClassMenagerMiniS2 &src);

        bool	intClaimPin();
        int		sequenceChase() const;
		Camera *Camera = NULL;


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
    class errorVector : public std::exception
    {
        public:
            const char * what () const throw ()
    };
};
