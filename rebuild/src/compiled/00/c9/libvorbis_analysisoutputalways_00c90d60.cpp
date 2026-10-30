// libvorbis::_analysis_output_always(char* base, int i, float* v, int n, int bark, int dB, ogg_int64_t off) -- libvorbis analysis.c debug dump: write n "x y" lines of the vector v to the Octave/Matlab file "<base>_<i>.m" (x = bark-scale band, time (j+off)/8000 or the index j; y = the dB value (-140 for zero) or the raw value)
// Leaf: @_analysis_output_always@32
// Flags: /Gr /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C90D60, 370 bytes, `__fastcall` (base in ecx, i in edx, five stack words, `ret 18h`),
//   ebp frame aligned to 8 (`and esp,-8`), /GS cookie (__security_cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity (proven): the libvorbis 1.0.1 analysis.c body verbatim against the binary -- "%s_%d.m" (0x012BF2D0), "w",
//   perror "failed to open data dump file", "%f " / "%f\n", toBARK((4000.f*j/n)+.25) with the double constants 1.85e-8,
//   2.24, .00074, 13.1, 1e-4, (double)(j+off)/8000. (as * 0.000125), (double)j, -140. for a zero sample else todB(v+j);
//   its wrapper _analysis_output 0x00C90EE0 tests analysis_noisy (0x0139DDB0) first, and vorbisfile's lap debugging
//   (0x00C8C52F / 0x00C8C54B) calls it directly with base in ecx / i in edx. libpk's statically linked Xiph libvorbis
//   ("Xiph.Org libVorbis I 20030909"). The catalog label Debug_DumpMatlabFile is descriptive.
// Callees: sprintf / fopen / perror / fprintf (hoisted into edi) / fclose through their MSVCR71 imports (0x01440128,
//   0x01440040, 0x01440010, 0x01440044, 0x0144014C); the scales.h static todB 0x00C90D40, defined here as a same-TU
//   `static __declspec(noinline)` so VC7.1 gives it retail's private convention (the pointer in eax, result in st0).
//   Land the leaf only.
typedef __int64 ogg_int64_t;                       // ogg/os_types.h (_WIN32)
typedef unsigned int ogg_uint32_t;
typedef struct _iobuf FILE;
extern "C" __declspec(dllimport) int __cdecl sprintf(char* buffer, const char* format, ...);   // [0x01440128]
extern "C" __declspec(dllimport) FILE* __cdecl fopen(const char* name, const char* mode);      // [0x01440040]
extern "C" __declspec(dllimport) void __cdecl perror(const char* text);                         // [0x01440010]
extern "C" __declspec(dllimport) int __cdecl fprintf(FILE* stream, const char* format, ...);   // [0x01440044]
extern "C" __declspec(dllimport) int __cdecl fclose(FILE* stream);                              // [0x0144014C]
extern "C" double __cdecl atan(double x);
#pragma intrinsic(atan)

#define toBARK(n)   (13.1f*atan(.00074f*(n))+2.24f*atan((n)*(n)*1.85e-8f)+1e-4f*(n))

extern "C" {
/* Segher was off (too high) by ~ .3 decibel.  Center the conversion correctly. */
static __declspec(noinline) float todB(const float *x){
  union {
    ogg_uint32_t i;
    float f;
  } ix;
  ix.f = *x;
  ix.i = ix.i&0x7fffffff;
  return (float)(ix.i * 7.17711438e-7f -764.6161886f);
}

/* there was no great place to put this.... */
void _analysis_output_always(char *base,int i,float *v,int n,int bark,int dB,ogg_int64_t off){
  int j;
  FILE *of;
  char buffer[80];

  /*  if(i==5870){*/
    sprintf(buffer,"%s_%d.m",base,i);
    of=fopen(buffer,"w");

    if(!of)perror("failed to open data dump file");

    for(j=0;j<n;j++){
      if(bark){
        float b=toBARK((4000.f*j/n)+.25);
        fprintf(of,"%f ",b);
      }else
        if(off!=0)
          fprintf(of,"%f ",(double)(j+off)/8000.);
        else
          fprintf(of,"%f ",(double)j);

      if(dB){
        float val;
        if(v[j]==0.)
          val=-140.;
        else
          val=todB(v+j);
        fprintf(of,"%f\n",val);
      }else{
        fprintf(of,"%f\n",v[j]);
      }
    }
    fclose(of);
    /*  } */
}
}