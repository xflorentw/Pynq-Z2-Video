/******************************************************************************
*  @file video_out_pynq.c
*  Author: Florent Werbrouck
*  Description: Application for Video out design on PYNQ-z2
******************************************************************************/
/******************************************************************************
*
* Copyright (c) 2021, Florent Werbrouck
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/

#include <stdio.h>
#include "xil_printf.h"
#include "xparameters.h"

#include "video_out_pynq.h"


int main()
{
	
	const XVidC_VideoTimingMode* VmPtr ;
	XVidC_VideoStream VidStream;

    XV_tpg TpgInst;
    XVtc VtcInst;
	XGpio XGpioInst;
	TpgConfig_t TpgConfig;
	int resId = 0, resId_prev = 0;

    print("Video Out application on PYNQ-Z2\n\r");
	
	// System Initialization
	system_init(&TpgInst, &VtcInst, &XGpioInst);
	
	//De-assert the reset for the video path
	XGpio_DiscreteWrite(&XGpioInst, GPIO_RESET_CHANNEL, GPIO_RESET_OFF);
	usleep(200);
	
	//Set Stream In
	VmPtr=XVidC_GetVideoModeData(XVIDC_VM_1280x720_60_P);
	TpgConfig.colorFormat = XVIDC_CSF_RGB;
	TpgConfig.bckgndId = XTPG_BKGND_COLOR_BARS;
	TpgConfig.width = VmPtr->Timing.HActive;
	TpgConfig.height = VmPtr->Timing.VActive;

	//Configure the tpg
	tpg_configuration(&TpgInst, &TpgConfig);

    //Start the VTC generator with default resolution (1280x720)
    XVtc_EnableGenerator(&VtcInst);
	
	while(1){
		resId_prev = resId;

		resId = main_menu();

		if(resId != resId_prev)
		{
			//Set the output clock for selected resolution
			ClkWiz_Set_Output_Clock(XPAR_XCLK_WIZ_0_BASEADDR, resId);
			
			//Toogle the reset for the video path
			XGpio_DiscreteWrite(&XGpioInst, GPIO_RESET_CHANNEL, GPIO_RESET_ON);
			usleep(200);
			XGpio_DiscreteWrite(&XGpioInst, GPIO_RESET_CHANNEL, GPIO_RESET_OFF);
			usleep(200);

			//Reconfigure the TPG
			VmPtr=XVidC_GetVideoModeData(get_video_mode_id(resId));
			TpgConfig.width = VmPtr->Timing.HActive;
			TpgConfig.height = VmPtr->Timing.VActive;
			xil_printf("TPG resolution: %d x %d\r\n",VmPtr->Timing.HActive,VmPtr->Timing.VActive);
			tpg_configuration(&TpgInst, &TpgConfig);

			//Reconfigure VTC generator
			XVidC_SetVideoStream(&VidStream, get_video_mode_id(resId), XVIDC_CSF_RGB,
							XVIDC_BPC_8,XVIDC_PPC_1);
			xil_printf("VTG resolution: %d x %d\r\n",VidStream.Timing.HActive,VidStream.Timing.VActive);;
			configure_vtc_gen(&VtcInst, &VidStream);
		}


	}

    return 0;
}

int system_init(XV_tpg* TpgInstPtr, XVtc* VtcInstPtr, XGpio* XGpioInstPtr)
{
	int Status;
	
    // Initialise the TPG
    Status = XV_tpg_Initialize(TpgInstPtr, 0);
    if(Status!= XST_SUCCESS)
    {
    	xdbg_printf(XDBG_DEBUG_GENERAL,"TPG configuration failed\r\n");
        return(XST_FAILURE);
    }

    // Initialise the VTC
    XVtc_Config *VtcConfig = XVtc_LookupConfig(0);
    XVtc_CfgInitialize(VtcInstPtr, VtcConfig, VtcConfig->BaseAddress);
    if(Status!= XST_SUCCESS)
    {
    	xdbg_printf(XDBG_DEBUG_GENERAL,"VTC configuration failed\r\n");
        return(XST_FAILURE);
    }
	
	/* Initialize the GPIO driver */
	Status = XGpio_Initialize(XGpioInstPtr, 0);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	
	return Status;
};

void tpg_configuration(XV_tpg *InstPtr, TpgConfig_t *ConfigPtr)
{
	// Set Resolution
    XV_tpg_Set_height(InstPtr, ConfigPtr->height);
    XV_tpg_Set_width(InstPtr, ConfigPtr->width);

    // Set Color Space
    XV_tpg_Set_colorFormat(InstPtr, ConfigPtr->colorFormat);

    //Set background
    XV_tpg_Set_bckgndId(InstPtr, ConfigPtr->bckgndId);

    XV_tpg_EnableAutoRestart(InstPtr);
    XV_tpg_Start(InstPtr);
};

/*****************************************************************************/
/**
*
* This function configures the VTC Generator core.
* @param
* @param
*
*
******************************************************************************/
void configure_vtc_gen(XVtc *InstancePtr, const XVidC_VideoStream *Strm)
{
	XVtc_Reset(InstancePtr);
	XVtc_DisableGenerator(InstancePtr);
	XVtc_Disable(InstancePtr);

	XVtc_Timing XVtc_Timingconf;

	XVtc_Timingconf.HActiveVideo 	= Strm->Timing.HActive;
	XVtc_Timingconf.HBackPorch		= Strm->Timing.HBackPorch;
	XVtc_Timingconf.HFrontPorch		= Strm->Timing.HFrontPorch;
	XVtc_Timingconf.HSyncPolarity	= Strm->Timing.HSyncPolarity;
	XVtc_Timingconf.HSyncWidth		= Strm->Timing.HSyncWidth;
	XVtc_Timingconf.Interlaced		= Strm->IsInterlaced;
	XVtc_Timingconf.V0BackPorch		= Strm->Timing.F0PVBackPorch;
	XVtc_Timingconf.V0FrontPorch	= Strm->Timing.F0PVFrontPorch;
	XVtc_Timingconf.V0SyncWidth		= Strm->Timing.F0PVSyncWidth;
	XVtc_Timingconf.V1BackPorch		= Strm->Timing.F1VBackPorch;
	XVtc_Timingconf.V1FrontPorch	= Strm->Timing.F1VFrontPorch;
	XVtc_Timingconf.V1SyncWidth		= Strm->Timing.F1VSyncWidth;
	XVtc_Timingconf.VActiveVideo	= Strm->Timing.VActive;
	XVtc_Timingconf.VSyncPolarity	= Strm->Timing.VSyncPolarity;

	//Configure the VTC
	XVtc_SetGeneratorTiming(InstancePtr, &XVtc_Timingconf);
	XVtc_RegUpdate(InstancePtr);

	//Start the VTC generator
	XVtc_Enable(InstancePtr);
	XVtc_EnableGenerator(InstancePtr);
}