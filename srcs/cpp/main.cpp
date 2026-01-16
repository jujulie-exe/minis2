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
            static_cast<int>(pin::pin_17),
            static_cast<int>(pin::pin_27),
            static_cast<int>(pin::pin_22),
            static_cast<int>(pin::pin_23)
        }, NULL);
        int ret = menager->intClaimPin();
        if (handelError(ret) != OK)
        {
            return ret;
        }
        ret = menager->sequenceChase();
        if (handelError(ret) != OK)
        {
            return ret;
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    delete menager;
    return 0;
}
