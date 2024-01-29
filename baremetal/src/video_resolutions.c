/******************************************************************************
*  @file video_resolutions.c
*  Author: Florent Werbrouck
*  Description: Utilities to configure the resolution for Video out design on
*  PYNQ-z2
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
#include "video_resolutions.h"
#include "sleep.h"

VideoTiming_t VideoTimingClk[SUPPORTED_VIDEO_FORMATS]=
{
		{XVIDC_VM_1280x720_60_P, 	CLKWIZ_CLKOUT0_74_25_MHz},
		{XVIDC_VM_1024x768_60_P,	CLKWIZ_CLKOUT0_65_MHz},
		{XVIDC_VM_800x600_60_P,		CLKWIZ_CLKOUT0_40_MHz},
		{XVIDC_VM_640x480_60_P,		CLKWIZ_CLKOUT0_25_175MHz}
};

/****************************************************************************/
/**
*
*	This function wait for some time and check if the clocking wizard is locked
*
*
* @param	outClockFreq frequency to be set.
*
* @return	status.
*
* @note		None.
*
******************************************************************************/
int ClkWiz_Wait_For_Lock(UINTPTR Addr)
{
	usleep(800);
	if(Xil_In32(Addr + 0x04)& CLK_LOCK)
	{
		return XST_SUCCESS;
	}
	else
	{
		return XST_FAILURE;
	}
}


/*****************************************************************************/
/**
*
* This function configure the clock output of the clocking wizard
* @param
* @param
* @param
*
*
******************************************************************************/
int ClkWiz_Set_Output_Clock(UINTPTR ClkWiz_BaseAddr, int resId)
{
	int status;

	u8 outClockFreq = VideoTimingClk[resId].clkFreq;

	//Soft Reset is required from 2018.3
	Xil_Out32(ClkWiz_BaseAddr, 0xA);
	usleep(800);

	switch(outClockFreq)
	{
		case CLKWIZ_CLKOUT0_74_25_MHz:
			//Set clock to 74.25 MHz
			// [7:0] = DIVCLK_DIVIDE = 4 (0x4)
			// [15:8] = CLKFBOUT_MULT = 37 (0x25)
			// [25:16] = CLKFBOUT_FRAC = 125 (0x7D)
			Xil_Out32(ClkWiz_BaseAddr + 0x200, 0x7D2504);
			// DIV = 12.500 (0x0C.0x1FA)
			Xil_Out32(ClkWiz_BaseAddr + 0x208, 0x1FA0C);
			break;

		case CLKWIZ_CLKOUT0_65_MHz:
			//Set clock to 65 MHz
			// [7:0] = DIVCLK_DIVIDE = 10 (0x05)
			// [15:8] = CLKFBOUT_MULT = 50 (0x32)
			// [25:16] = CLKFBOUT_FRAC = 375 (0x177)
			Xil_Out32(ClkWiz_BaseAddr + 0x200, 0x1773205);
			// DIV = 15.500 (0x0F.0x1F4)
			Xil_Out32(ClkWiz_BaseAddr + 0x208, 0x1F40F);
			break;

		case CLKWIZ_CLKOUT0_40_MHz:
			//Set clock to 40 MHz
			// [7:0] = DIVCLK_DIVIDE = 1 (0x01)
			// [15:8] = CLKFBOUT_MULT = 5 (0x0A)
			// [25:16] = CLKFBOUT_FRAC = 0 (0x0)
			Xil_Out32(ClkWiz_BaseAddr + 0x200, 0x000A01);
			// DIV = 25.000 (0x19.0x00)
			Xil_Out32(ClkWiz_BaseAddr + 0x208, 0x19);
			break;

		case CLKWIZ_CLKOUT0_25_175MHz:
			//Set clock to 25.175 MHz
			// [7:0] = DIVCLK_DIVIDE = 4 (0x04)
			// [15:8] = CLKFBOUT_MULT = 36 (0x24)
			// [25:16] = CLKFBOUT_FRAC = 375 (0x177)
			Xil_Out32(ClkWiz_BaseAddr + 0x200, 0x1772404);
			// DIV = 36.125 (0x24.0x07D)
			Xil_Out32(ClkWiz_BaseAddr + 0x208, 0x07D24);
			break;
		default:
			return XST_FAILURE;
			break;
	}

	// Update the clocking wizard
	Xil_Out32(ClkWiz_BaseAddr + 0x25C, 0x3);

	status = ClkWiz_Wait_For_Lock(ClkWiz_BaseAddr);
	return status;

}

/*****************************************************************************/
/**
*
* This function get the resolution name
* @param
* @param
*
*
******************************************************************************/
XVidC_VideoMode get_video_mode_id(int resID)
{
	XVidC_VideoMode VmId = VideoTimingClk[resID].VmId;
	return VmId;
}
