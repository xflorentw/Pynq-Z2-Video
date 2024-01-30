/******************************************************************************
*  @file menu.c
*  Author: Florent Werbrouck
*  Description: Application selection menu
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
#include "menu.h"

int main_menu()
{
	int i;
	char userInput;

	xil_printf("\r\n-----------------------------\r\n");
	xil_printf("Select the output resolution:\r\n");

	for(i=0;i<=SUPPORTED_VIDEO_FORMATS-1;i++)
	{
		xil_printf("%d. ",i);
		//Print Resolution Name
		xil_printf("%s",XVidC_GetVideoModeStr(get_video_mode_id(i)));
		xil_printf("\r\n");
	}

	xil_printf("\r\n-----------------------------\r\n");

	userInput = inbyte();

	userInput = userInput-48;

	if((userInput>= 0)&&(userInput <= 3))
	{
		xil_printf("Selected Resolution: %s",XVidC_GetVideoModeStr(get_video_mode_id(userInput)));
		return (userInput);
	}
	else
	{
		xil_printf("Invalid Option\r\n");
		return 0;
	}
}
