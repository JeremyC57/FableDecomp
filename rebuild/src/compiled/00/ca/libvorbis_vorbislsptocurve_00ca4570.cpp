// libvorbis::vorbis_lsp_to_curve(float* curve, int* map, int n, int ln, float* lsp, int m, float amp, float ampoffset) -- libvorbis lsp.c: LSP to filter curve with the FLOAT_LOOKUP tables (cos/invsq/invsq2exp/fromdB), side effect: lsp[] becomes cos(lsp)
// Leaf: @vorbis_lsp_to_curve@32
// Flags: /O2 /Oy /QIfist
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA4570, 398 bytes. libvorbis 1.0.1 lib/lsp.c (libpk's statically linked Xiph decoder,
//   "Xiph.Org libVorbis I 20030909" @0x012BF240), the FLOAT_LOOKUP build of vorbis_lsp_to_curve.
// Identity (proven): the one caller is 0x00CA39FF inside the landed floor0_inverse2 0x00CA39C0 (floor0.c, which declares it
//   `__fastcall` at this address); the body calls the four landed lookup.c helpers (vorbis_coslook 0x00CA3E50 at 0x00CA45A0 and
//   0x00CA4603, vorbis_invsq2explook 0x00CA3EF0 at 0x00CA46A4 with qexp+m in ecx, vorbis_invsqlook 0x00CA3EA0 at 0x00CA46B2,
//   vorbis_fromdBlook 0x00CA3F00 at 0x00CA46C7), the MSVCR71 frexp import (IAT 0x01440018, qword argument + &qexp) and the
//   os.h M_PI float (0x0128F604, `fdivr`) for wdel = M_PI/ln; the .7071067812f seeds are the 0x3F3504F3 stores; the do/while
//   pair loop (`dec ecx ; jne`, ftmp[0]-w / ftmp[1]-w), the m&1 odd-order arm (q*=q, p*=p*(1-w*w)) and the
//   `do{curve[i++]*=q;}while(map[i]==k)` tail (`cmp ecx,edi ; je`) are the lsp.c text verbatim.
// Convention: `__fastcall` (curve in ecx, map in edx, six stack arguments, `ret 18h`), as the landed lookup.c siblings and
//   vorbis_lpc_to_lsp 0x00CA4160 in the same TU. vorbis_fpu_setround / vorbis_fpu_restore are empty inlines for _MSC_VER (os.h).
// Label: the manifest "Math_ApplyDampeningFilter" is a descriptive placeholder; the real name is libvorbis vorbis_lsp_to_curve.
extern "C" __declspec(dllimport) double __cdecl frexp(double, int*);   // <math.h>, MSVCR71.dll import (IAT 0x01440018: `call dword ptr`)

#define M_PI (3.1415926536f)                                              // os.h (_MSC_VER)
typedef short vorbis_fpu_control;                                         // os.h: ogg_int16_t
static __inline void vorbis_fpu_setround(vorbis_fpu_control *fpu){ }
static __inline void vorbis_fpu_restore(vorbis_fpu_control fpu){ }

/* os.h (_MSC_VER): vorbis_ftoi is the out-of-line 0x00CA3B10 (landed retail_asm: `fld f ; fistp i`, inline asm VC7.1 keeps
   out of line). It is in this TU, so VC7.1 knows it touches only eax: the lsp_to_curve loop index stays in edx across the
   vorbis_coslook call. Here it is the same conversion in C++ under /QIfist (fistp in the current rounding mode). */
extern "C" __declspec(noinline) long __stdcall vorbis_ftoi(double f){
	int i;
	i=(int)f;
	return i;
}

/* ---- lookup.c, #included by lsp.c under FLOAT_LOOKUP (the same TU: VC7.1 knows the callees' register use) ---- */
#define COS_LOOKUP_SZ 128
#define INVSQ_LOOKUP_SZ 32
#define INVSQ2EXP_LOOKUP_MIN (-32)
#define FROMdB_LOOKUP_SZ 35
#define FROMdB2_LOOKUP_SZ 32
#define FROMdB_SHIFT 5
#define FROMdB2_SHIFT 3
#define FROMdB2_MASK 31
extern "C" float COS_LOOKUP[COS_LOOKUP_SZ + 1];        // 0x013AC260 (lookup_data.h)
extern "C" float INVSQ_LOOKUP[INVSQ_LOOKUP_SZ + 1];    // 0x013AC468
extern "C" float INVSQ2EXP_LOOKUP[65];                 // 0x013AC4F0
extern "C" float FROMdB_LOOKUP[FROMdB_LOOKUP_SZ];      // 0x013AC5F8
extern "C" float FROMdB2_LOOKUP[FROMdB2_LOOKUP_SZ];    // 0x013AC688

extern "C" {
/* interpolated lookup based cos function, domain 0 to PI only */
__declspec(noinline) float __fastcall vorbis_coslook(float a){                                   // 0x00CA3E50
  double d=a*(.31830989*(float)COS_LOOKUP_SZ);
  int i=vorbis_ftoi(d-.5);

  return COS_LOOKUP[i]+ (d-i)*(COS_LOOKUP[i+1]-COS_LOOKUP[i]);
}

/* interpolated 1./sqrt(p) where .5 <= p < 1. */
__declspec(noinline) float __fastcall vorbis_invsqlook(float a){                                 // 0x00CA3EA0
  double d=a*(2.f*INVSQ_LOOKUP_SZ)-INVSQ_LOOKUP_SZ;
  int i=vorbis_ftoi(d-.5f);
  return INVSQ_LOOKUP[i]+ (d-i)*(INVSQ_LOOKUP[i+1]-INVSQ_LOOKUP[i]);
}

/* interpolated 1./sqrt(p) where .5 <= p < 1. */
__declspec(noinline) float __fastcall vorbis_invsq2explook(int a){                               // 0x00CA3EF0
  return INVSQ2EXP_LOOKUP[a-INVSQ2EXP_LOOKUP_MIN];
}

/* interpolated pow(10,-.1*x) */
__declspec(noinline) float __fastcall vorbis_fromdBlook(float a){                                // 0x00CA3F00
  int i=vorbis_ftoi(a*((float)(-(1<<FROMdB2_SHIFT)))-.5f);
  return (i<0)?1.f:
    ((i>=(FROMdB_LOOKUP_SZ<<FROMdB_SHIFT))?0.f:
     FROMdB_LOOKUP[i>>FROMdB_SHIFT]*FROMdB2_LOOKUP[i&FROMdB2_MASK]);
}

/* side effect: changes *lsp to cosines of lsp */
void __fastcall vorbis_lsp_to_curve(float *curve,int *map,int n,int ln,float *lsp,int m,
			    float amp,float ampoffset){
  int i;
  float wdel=M_PI/ln;
  vorbis_fpu_control fpu;
  
  vorbis_fpu_setround(&fpu);
  for(i=0;i<m;i++)lsp[i]=vorbis_coslook(lsp[i]);

  i=0;
  while(i<n){
    int k=map[i];
    int qexp;
    float p=.7071067812f;
    float q=.7071067812f;
    float w=vorbis_coslook(wdel*k);
    float *ftmp=lsp;
    int c=m>>1;

    do{
      q*=ftmp[0]-w;
      p*=ftmp[1]-w;
      ftmp+=2;
    }while(--c);

    if(m&1){
      /* odd order filter; slightly assymetric */
      /* the last coefficient */
      q*=ftmp[0]-w;
      q*=q;
      p*=p*(1.f-w*w);
    }else{
      q*=q*(1.f+w);
      p*=p*(1.f-w);
    }

    q=frexp(p+q,&qexp);
    q=vorbis_fromdBlook(amp*             
			vorbis_invsqlook(q)*
			vorbis_invsq2explook(qexp+m)- 
			ampoffset);

    do{
      curve[i++]*=q;
    }while(map[i]==k);
  }
  vorbis_fpu_restore(fpu);

}
}