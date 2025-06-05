/*
 * Copyright 2025 Gergely Gati
 *
 * gati.gergely@yahoo.com
 * github.com/gega
 *
 * BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS _AS IS_
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef RPPM_H
#define RPPM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define RPPM_RED(p32)   (uint8_t)(((uint32_t)p32)>>24)
#define RPPM_GREEN(p32) (uint8_t)((((uint32_t)p32)>>16)&0xff)
#define RPPM_BLUE(p32)  (uint8_t)((((uint32_t)p32)>>8)&0xff)

#define RPPM_PACKRGB(r,g,b) (uint32_t)(((uint32_t)r)<<24|((uint32_t)g)<<16|((uint32_t)b)<<8)
#define RPPM_GET_PIXEL(rppm,x,y) (uint32_t)((rppm)->pixels[(x)*(rppm)->width+(y)])

struct rppm
{
  int width,height;
  uint32_t *pixels;
};

#endif

#ifdef RPPM_IMPLEMENTATION

#ifndef RPPM_MALLOC
#define RPPM_MALLOC(m) malloc(m)
#endif

#ifndef RPPM_FREE
#define RPPM_FREE(m) free(m)
#endif

int rppm_load(struct rppm *rppm, const char *filename)
{
  static char *pattern[2]=
  {
    " %d %d\n%d\n",
    "%*[^\n] %d %d\n%d\n"
  };
  char magic[3]={0};
  uint8_t pixel[3];
  FILE *f=NULL;
  int maxcol,i,pat=0,c;

  if(!rppm) goto error;
  if(!(f=fopen(filename,"rb"))) goto error;
  if(0==fread(&magic,3,1,f)) goto error;
  if(magic[0]!='P'||magic[1]!='6'||magic[2]!='\n') goto error;
  if((c=fgetc(f))=='#') pat=1;
  else ungetc(c,f);
  if(EOF==(fscanf(f,pattern[pat], &rppm->width, &rppm->height, &maxcol))) goto error;
  if(maxcol<=0||maxcol>65536||rppm->width<=0||rppm->height<=0) goto error;
  if(NULL==(rppm->pixels=RPPM_MALLOC(rppm->width*rppm->height*sizeof(uint32_t)))) goto error;
  for(i=0;fread(&pixel,sizeof(pixel),1,f)==1;i++) rppm->pixels[i]=RPPM_PACKRGB(pixel[0],pixel[1],pixel[2]);
  if(i!=rppm->width*rppm->height) goto error;
  fclose(f);

  return(0);

error:
  if(f) fclose(f);
  return(-1);
}

void rppm_free(struct rppm *rppm)
{
  if(NULL!=rppm&&NULL!=rppm->pixels) RPPM_FREE(rppm->pixels);
}

#endif
