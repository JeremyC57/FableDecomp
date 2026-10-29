#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#include <stdlib.h>
#define malloc test_malloc
#endif
// 00c9bcd0 mdct_init
// Leaf: @mdct_init@8
// Flags: /Gr
// Original libvorbis mdct.c/mdct.h and os.h numeric macros; BSD-style terms in reference COPYING.
#ifdef _MSC_VER
#pragma intrinsic(log,sin,cos)
#else
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
extern "C" double __cdecl log(double);
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
extern "C" __declspec(dllimport) double __cdecl floor(double);
extern "C" __declspec(dllimport) void* __cdecl malloc(unsigned int);
#define _ogg_malloc malloc
#define rint(x) (floor((x)+0.5f))
#define M_PI 3.1415926536f
#define DATA_TYPE float
#define FLOAT_CONV(x) (x)
typedef struct {
  int n;
  int log2n;
  
  DATA_TYPE *trig;
  int       *bitrev;

  DATA_TYPE scale;
} mdct_lookup;
extern "C" void __fastcall mdct_init(mdct_lookup *lookup,int n){
  int   *bitrev=(int*)_ogg_malloc(sizeof(*bitrev)*(n/4));
  DATA_TYPE *T=(DATA_TYPE*)_ogg_malloc(sizeof(*T)*(n+n/4));
  
  int i;
  int n2=n>>1;
  int log2n=lookup->log2n=rint(log((float)n)/log(2.f));
  lookup->n=n;
  lookup->trig=T;
  lookup->bitrev=bitrev;

/* trig lookups... */

  for(i=0;i<n/4;i++){
    T[i*2]=FLOAT_CONV(cos((M_PI/n)*(4*i)));
    T[i*2+1]=FLOAT_CONV(-sin((M_PI/n)*(4*i)));
    T[n2+i*2]=FLOAT_CONV(cos((M_PI/(2*n))*(2*i+1)));
    T[n2+i*2+1]=FLOAT_CONV(sin((M_PI/(2*n))*(2*i+1)));
  }
  for(i=0;i<n/8;i++){
    T[n+i*2]=FLOAT_CONV(cos((M_PI/n)*(4*i+2))*.5);
    T[n+i*2+1]=FLOAT_CONV(-sin((M_PI/n)*(4*i+2))*.5);
  }

  /* bitreverse lookup... */

  {
    int mask=(1<<(log2n-1))-1,i,j;
    int msb=1<<(log2n-2);
    for(i=0;i<n/8;i++){
      int acc=0;
      for(j=0;msb>>j;j++)
	if((msb>>j)&i)acc|=1<<j;
      bitrev[i*2]=((~acc)&mask)-1;
      bitrev[i*2+1]=acc;

    }
  }
  lookup->scale=FLOAT_CONV(4.f/n);
}

#ifndef _MSC_VER
#undef malloc
#endif
extern "C" void __cdecl abort();
static unsigned cases,used,allocations,currentN;static void*pointers[2];static unsigned sizes[2];
static union{double alignment;unsigned char bytes[65536];}arena;
static void check(bool p){if(!p){fprintf(stderr,"MDCT init case %u failed\n",cases);abort();}}
extern "C" void* __cdecl test_malloc(unsigned bytes){check(allocations<2&&bytes==(allocations?5*currentN:currentN));unsigned begin=(used+15)&~7U;check(begin+bytes+8<sizeof arena.bytes);pointers[allocations]=arena.bytes+begin;sizes[allocations++]=bytes;used=begin+bytes+8;return arena.bytes+begin;}
#ifdef _MSC_VER
static double __cdecl test_floor(double value){check(value>=0&&value<32);return (double)(int)value;}
extern "C" {void* (__cdecl* _imp__malloc)(unsigned)=test_malloc;double (__cdecl* _imp__floor)(double)=test_floor;}
#endif
static double absolute(double x){return x<0?-x:x;}
static void closeValue(float actual,double expected){check(actual==actual&&absolute(actual-expected)<1.0/1048576.0);}
int main(){for(int exponent=6;exponent<=13;++exponent)for(int pattern=0;pattern<3;++pattern)for(int offset=0;offset<4;++offset){int n=1<<exponent;currentN=n;used=offset*8;allocations=0;memset(arena.bytes,0xc7,sizeof arena.bytes);mdct_lookup lookup;memset(&lookup,pattern*73,sizeof lookup);mdct_init(&lookup,n);check(lookup.n==n&&lookup.log2n==exponent&&lookup.scale==4.0f/n&&allocations==2&&lookup.bitrev==pointers[0]&&lookup.trig==pointers[1]);double pi=(double)M_PI;
for(int i=0;i<n/4;++i){double first=4.0*i*pi/n,second=(2.0*i+1.0)*pi/(2.0*n);closeValue(lookup.trig[2*i],cos(first));closeValue(lookup.trig[2*i+1],-sin(first));closeValue(lookup.trig[n/2+2*i],cos(second));closeValue(lookup.trig[n/2+2*i+1],sin(second));}
for(int i=0;i<n/8;++i){double angle=(4.0*i+2.0)*pi/n;closeValue(lookup.trig[n+2*i],cos(angle)*0.5);closeValue(lookup.trig[n+2*i+1],-sin(angle)*0.5);unsigned value=i,reversed=0;for(int k=0;k<exponent-1;++k){reversed=(reversed<<1)|(value&1);value>>=1;}check(lookup.bitrev[2*i+1]==(int)reversed&&lookup.bitrev[2*i]+lookup.bitrev[2*i+1]==n/2-2);}
for(unsigned k=0;k<2;++k){unsigned char*p=(unsigned char*)pointers[k];for(int g=1;g<=8;++g)check(p[-g]==0xc7&&p[sizes[k]+g-1]==0xc7);}++cases;}check(cases==96);puts("00c9bcd0_TEST PASS");return 0;}