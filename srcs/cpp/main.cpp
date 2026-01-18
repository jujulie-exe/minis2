#include "ClassMenagerMiniS2.hpp"
int handelError(int ret)
{
    if (ret != OK)
    { 
        if (ret == ERROR_NO_CLAIM)
        {
        	std::cerr << "ERROR: " << "ERROR_NO_CLAIM" << std::endl;
        }
        if (ret == ERROR_NO_WRITE_GROUP)
        {
            std::cerr << "ERROR: " << "ERROR_NO_WRITE_GROUP" << std::endl;
        }
        if (ret == ERROR_FAILURE_CLAIM_GROUP)
        {
            std::cerr << "ERROR: " << "ERROR_FAILURE_CLAIM_GROUP" << std::endl;
        }
    }
    return ret;
}
int main()
{
    //ClassMenagerMiniS2(int ChipSet, const std::vector<int> &pin, Camera *ptrCamera)
    ClassMenagerMiniS2 *menager = NULL;
    try {
        menager = new ClassMenagerMiniS2(0, {
            static_cast<int>(pin::GPIO_5),
            static_cast<int>(pin::GPIO_13),
            static_cast<int>(pin::GPIO_26),
            static_cast<int>(pin::GPIO_17)
        }, NULL);
        int ret = menager->intClaimPin();
        if (handelError(ret) != OK)
        {
            delete menager;
            return 1;
        }
        ret = menager->sequenceChase();
        if (handelError(ret) != OK)
        {
            delete menager;
            return 1;
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    delete menager;
    return 0;
}
