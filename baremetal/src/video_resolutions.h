/******************************************************************************
*  @file video_resolutions.h
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
#include "xvidc.h"

#ifndef _VIDEO_RESOLUTIONS_H_
#define _VIDEO_RESOLUTIONS_H_

	#define SUPPORTED_VIDEO_FORMATS 4

	#define CLK_LOCK			1

	//Video clocks
	#define CLKWIZ_CLKOUT0_74_25_MHz		0
	#define CLKWIZ_CLKOUT0_65_MHz			1
	#define CLKWIZ_CLKOUT0_40_MHz			2
	#define	CLKWIZ_CLKOUT0_25_175MHz		3

	typedef struct {
			XVidC_VideoMode		VmId;
			u8					clkFreq;
	} VideoTiming_t;

	int ClkWiz_Wait_For_Lock(UINTPTR Addr);
	int ClkWiz_Set_Output_Clock(UINTPTR ClkWiz_BaseAddr, int resId);
	XVidC_VideoMode get_video_mode_id(int resID);


#endif /* _VIDEO_RESOLUTIONS_H_ */
