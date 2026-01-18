#include "ClassMenagerMiniS2.hpp"
#include "Camera.hpp"
#include "Logger.hpp"
int handelError(int ret)
{
    if (ret != OK)
    { 
        // ERROR GPIO
        if (ret == ERROR_NO_CLAIM)
        {
        	Logger::log(Logger::ERROR, "♡ ERROR_NO_CLAIM: GPIO initialization failed. Check if pins are available/exported. ♡" );

        }
        if (ret == ERROR_NO_WRITE_GROUP)
        {
			Logger::log(Logger::ERROR, "♡ ERROR_NO_WRITE_GROUP: Writing to GPIO group failed. Check hardware/logic. ♡" );
        }
        if (ret == ERROR_FAILURE_CLAIM_GROUP)
        {
			Logger::log(Logger::ERROR, "♡ ERROR_FAILURE_CLAIM_GROUP: Claiming GPIO group failed. Driver configuration error? ♡" );
        }
        // ERROR CAMERA
		if (ret == ERROR_NO_PHOTO_TAKEN)
		{
			Logger::log(Logger::ERROR, "♡ ERROR_NO_PHOTO_TAKEN: No photo captured. Camera stream might be unresponsive. ♡" );
		}
        if (ret == ERROR_SET_FMT)
        {
            Logger::log(Logger::ERROR, "♡ ERROR_SET_FMT: Camera format setup failed. Check supported resolution/pixel format. ♡" );
        }
        if (ret == ERROR_REQ_BUFFER)
        {
            Logger::log(Logger::ERROR, "♡ ERROR_REQ_BUFFER: Memory buffer request failed. Video device busy or no memory. ♡" );
        }
        if (ret == ERROR_QBUF)
        {
            Logger::log(Logger::ERROR, "♡ ERROR_QBUF: Enqueue buffer failed. Video stream might be interrupted. ♡" );
        }
        if (ret == ERROR_DQBUF)
        {
            Logger::log(Logger::ERROR, "♡ ERROR_DQBUF: Dequeue buffer failed. Cannot retrieve frame from camera. ♡" );
        }
        if (ret == ERROR_EPOLL_CREATE)
        {
            Logger::log(Logger::ERROR, "♡ ERROR_EPOLL_CREATE: System call epoll_create failed. Check system resources. ♡" );
        }
        if (ret == ERROR_CTL_EPOLL)
        {
            Logger::log(Logger::ERROR, "♡ ERROR_CTL_EPOLL: Epoll control failed. Camera file descriptor might be closed. ♡" );
        }
        if (ret == ERROR_OFSTREM_NON_OPEN)
        {
            Logger::log(Logger::ERROR, "♡ ERROR_OFSTREM_NON_OPEN: Cannot open output file. Check disk space/permissions. ♡" );
        }
        if (ret == TIME_OUT)
        {
            Logger::log(Logger::ERROR, "♡ TIME_OUT: Camera frame timeout. No data received within limit. Stuck? ♡" );
        }
        if (ret == ERROR_NO_SET_CTRL)
        {
            Logger::log(Logger::WARNING, "♡ ERROR_NO_SET_CTRL: Check if control is available. ♡" );
        }
    }
    return ret;
}
Camera* CallCammObj(){     
	
	Camera *cam = NULL;
    try {
        cam = new Camera("/dev/video0", 2688, 1520);
        
       handelError(cam->setParameters(V4L2_CID_FOCUS_AUTO, ENABLE));
       handelError(cam->setParameters(V4L2_CID_POWER_LINE_FREQUENCY, 1));
       handelError(cam->setParameters(V4L2_CID_EXPOSURE_AUTO  , DISABLE)); 
       handelError(cam->setParameters(V4L2_CID_EXPOSURE_ABSOLUTE, 2047));
       handelError(cam->setParameters(0x009a090c , 0)); // focus_automatic_continuous
         
        Logger::log(Logger::INFO, "camera initialized" );
        int ret = 0;
        if ( (ret = cam->initV4L2()) == OK) {
            Logger::log(Logger::INFO, "V4L2 initialized" );
        } else {
			handelError(ret);
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
