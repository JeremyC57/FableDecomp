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