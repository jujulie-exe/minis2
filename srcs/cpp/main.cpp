#include "ClassMenagerMiniS2.hpp"
#include "Camera.hpp"
#include "Logger.hpp"
int handelError(int ret)
{
    if (ret != OK)
    { 
        if (ret == ERROR_NO_CLAIM)
        {
        	std::cerr << "ERROR: " << "ERROR_NO_CLAIM" << std::endl;
			Logger::log(Logger::ERROR, "ERROR_NO_CLAIM" );

        }
        if (ret == ERROR_NO_WRITE_GROUP)
        {
			Logger::log(Logger::ERROR, "ERROR_NO_WRITE_GROUP" );
        }
        if (ret == ERROR_FAILURE_CLAIM_GROUP)
        {
			Logger::log(Logger::ERROR, "ERROR_FAILURE_CLAIM_GROUP" );
        }
		if (ret == ERROR_NO_PHOTO_TAKEN)
		{
			Logger::log(Logger::ERROR, "ERROR_NO_PHOTO_TAKEN" );
		}
    }
    return ret;
}
Camera* CallCammObj(){     
	
	Camera *cam = NULL;
    try {
        cam = new Camera("/dev/video0", 640, 480);
        Logger::log(Logger::INFO, "camera initialized" );
        
        if (cam->initV4L2() == OK) {
            Logger::log(Logger::INFO, "V4L2 initialized" );
        } else {
			Logger::log(Logger::ERROR, "V4L2 initialized" );
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
		
		return NULL;
    }
    return cam;
}

int main(){
    std::srand(std::time(NULL));
	Camera *CameraV4L2 = CallCammObj();
	if(CameraV4L2 == NULL){
		Logger::log(Logger::ERROR, "Failed initialized camera" );
		return 1;
	}
    //ClassMenagerMiniS2(int ChipSet, const std::vector<int> &pin, Camera *ptrCamera)
    ClassMenagerMiniS2 *menager = NULL;
    try {
        menager = new ClassMenagerMiniS2(0, {
            static_cast<int>(pin::GPIO_5),
            static_cast<int>(pin::GPIO_13),
            static_cast<int>(pin::GPIO_26),
            static_cast<int>(pin::GPIO_17),
            static_cast<int>(pin::GPIO_22),
            static_cast<int>(pin::GPIO_27),
            static_cast<int>(pin::GPIO_23),
            static_cast<int>(pin::GPIO_24),
            static_cast<int>(pin::GPIO_25),
        }, CameraV4L2);
        int ret = menager->intClaimPin();
        if (handelError(ret) != OK)
        {
            delete menager;
			delete CameraV4L2;
            return 1;
        }
        ret = menager->sequenceChase();
        if (handelError(ret) != OK)
        {
            delete menager;
			delete CameraV4L2;
            return 1;
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
		delete CameraV4L2;
        return 1;
    }
	delete CameraV4L2;
    delete menager;
    return 0;
}
