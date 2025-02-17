// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//
// static conat char rcsid[] = "$Id: m_bbox.c,v 1.1 1997/02/03 22:45:10 b1 Exp $";
//-----------------------------------------------------------------------------




#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_timer.h>

#include "include/doomdef.h"
#include "include/m_misc.h"
#include "include/i_video.h"

#include "include/l_sound_sdl.h"

#include "include/d_net.h"
#include "include/g_game.h"

#include "include/i_system.h"




int	mb_used = 6;

SDL_Joystick *joystick;

int I_strncasecmp(char *str1, char *str2, int len)
{
	char c1, c2;

	while ( *str1 && *str2 && len-- ) {
		c1 = *str1++;
		c2 = *str2++;
		if ( toupper(c1) != toupper(c2) )
			return(1);
	}
	return(0);
}

void
I_Tactile
( int	on,
  int	off,
  int	total )
{
  // UNUSED.
  on = off = total = 0;
}

ticcmd_t	emptycmd;
ticcmd_t*	I_BaseTiccmd(void)
{
    return &emptycmd;
}


int  I_GetHeapSize (void)
{
    return mb_used*1024*1024;
}

byte* I_ZoneBase (int*	size)
{
    *size = mb_used*1024*1024;
    return (byte *) malloc (*size);
}



//
// I_GetTime
// returns time in 1/35 second tics
//
int  I_GetTime (void)
{
    return (SDL_GetTicks()*TICRATE)/1000;
}



//
// I_Init
//
void I_Init (void)
{
        if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0 )
        {

                I_InitSound();
                //  I_InitGraphics();

	            //SDL_Joystick *joystick;

	            joystick = SDL_JoystickOpen(0);
        }
        
        else
        {
            I_Error("Could not initialize SDL: %s", SDL_GetError());
        }
}

//
// I_Quit
//
void I_Quit (void)
{
    // Just a simple back to browser, from uLauncELF4.13 sources (TBD : check for latest code) - cosmito
	__asm__ __volatile__("	li $3, 0x04;" "	syscall;" "	nop;" );

    D_QuitNetGame ();
    I_ShutdownSound();
    I_ShutdownMusic();
    M_SaveDefaults ();
    I_ShutdownGraphics();
    exit(0);
}

void I_WaitVBL(int count)
{
    SDL_Delay((count*1000)/70);
}

byte*	I_AllocLow(int length)
{
    byte*	mem;
        
    mem = (byte *)malloc (length);
    memset (mem,0,length);
    return mem;
}


//
// I_Error
//
extern boolean demorecording;

void I_Error (char *error, ...)
{
    va_list	argptr;

    // Message first.
    va_start (argptr,error);
    fprintf (stderr, "Error: ");
    vfprintf (stderr,error,argptr);
    fprintf (stderr, "\n");
    va_end (argptr);

    fflush( stderr );

    // Shutdown. Here might be other errors.
    if (demorecording)
	G_CheckDemoStatus();

    D_QuitNetGame();
    I_ShutdownGraphics();
    
    exit(-1);
}
