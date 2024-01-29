/******************************************************************************
*  @file video_out_pynq.h
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

#ifndef _VID_OUT_PYNQ_H_
#define _VID_OUT_PYNQ_H_

	#include "xdebug.h"
	#include "xv_tpg.h"
	#include "xvtc.h"
	#include "xvidc.h"
	#include "menu.h"
	#include "sleep.h"
	#include "xgpio.h"

	#define GPIO_RESET_ON		0
	#define GPIO_RESET_OFF	 	1

	#define GPIO_RESET_CHANNEL 	1

	typedef struct {
		 u32 	colorFormat;
		 u32 	bckgndId;
		 u32	width;
		 u32	height;
	} TpgConfig_t;


	int system_init(XV_tpg* TpgInstPtr, XVtc* VtcInstPtr, XGpio* XGpioInstPtr);
	void tpg_configuration(XV_tpg *InstPtr, TpgConfig_t *ConfigPtr);
	void configure_vtc_gen(XVtc *InstancePtr, const XVidC_VideoStream *Strm);

#endif /* _VID_OUT_PYNQ_H_ */
