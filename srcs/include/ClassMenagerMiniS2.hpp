#include <lgpio.h> 
#include <stdexcept>
#include <iostream>
class ClassMenagerMiniS2
{
    private:
        int _lgpio;
    public:
         /*♡♡♡♡♡♡♡♡♡♡♡CTOR♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2(int Handle);
        ClassMenagerMiniS2(const ClassMenagerMiniS2 &src);
         /*♡♡♡♡♡♡♡♡♡♡♡COPY♡♡♡♡♡♡♡♡♡♡♡♡♡*/
        ClassMenagerMiniS2 & operator=(const ClassMenagerMiniS2 &src);
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