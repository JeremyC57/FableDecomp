// libvorbis::inspect_error(int x0, int x1, int y0, int y1, const float* mask, const float* mdct, vorbis_info_floor1* info) -- libvorbis 1.0.1 floor1.c: walk the Bresenham line y0..y1 over mask[x0..x1), rejecting (1) when a used bin's quantised mask leaves the line's maxover/maxunder window, accepting (0) when the window itself cannot beat maxerr, else rejecting when the mean squared error exceeds maxerr
// Leaf: _inspect_error@28
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA2480, 542 bytes: libvorbis 1.0.1 ("Xiph.Org libVorbis I 20030909" in the image)
//   lib/floor1.c `static int inspect_error(...)`, VC7.1 private convention: y1 in ecx, x0 / x1 / y0 / mask / mdct / info on the
//   stack, callee-pop `ret 18h`.
// Identity (two sources): (1) the 1.0.1 floor1.c body verbatim against the bytes (dy/adx/abs(dy)/base/sy, the Bresenham err
//   step, vorbis_dBquant of mask+x, `mdct[x]+info->twofitatten>=mask[x]` gating the maxover/maxunder window tests, the
//   maxover^2/n and maxunder^2/n > maxerr early accepts, mse/n > maxerr), with twofitatten / maxover / maxunder / maxerr at
//   +0x458 / +0x448 / +0x44c / +0x450 of the 1.0.1 vorbis_info_floor1; (2) its callee 0x00CA1E50 is the landed floor1.c
//   vorbis_dBquant (x in eax) and its sole caller 0x00CA26C0 is floor1_fit (next to the landed floor1.c statics accumulate_fit
//   0x00CA20A0, fit_line 0x00CA2280, post_Y 0x00CA26A0 in upstream order). Catalog "CurveInterpolation_ValidateSplineQuality"
//   is a descriptive stand-in.
// Callees: vorbis_dBquant 0x00CA1E50 (x in eax), the intrinsic abs.
// Shape: the floor1.c text compiled as C linkage; the statics as `static __declspec(noinline)` with a same-TU caller (GOTCHAS
//   chunks 586/599: VC7.1 then picks retail's private registers itself), inspect_error as `__stdcall` (the lane's
//   libvorbis-static convention, as the landed accumulate_fit 0x00CA20A0). The scaffold caller is not graded.
extern "C" int __cdecl abs(int);
#pragma intrinsic(abs)
extern "C" {
#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef struct{
  int   partitions;                /* 0 to 31 */
  int   partitionclass[VIF_PARTS]; /* 0 to 15 */

  int   class_dim[VIF_CLASS];        /* 1 to 8 */
  int   class_subs[VIF_CLASS];       /* 0,1,2,3 (bits: 1<<n poss) */
  int   class_book[VIF_CLASS];       /* subs ^ dim entries */
  int   class_subbook[VIF_CLASS][8]; /* [VIF_CLASS][subs] */


  int   mult;                      /* 1 2 3 or 4 */
  int   postlist[VIF_POSIT+2];    /* first two implicit */


  /* encode side analysis parameters */
  float maxover;
  float maxunder;
  float maxerr;

  float twofitweight;
  float twofitatten;

  int   n;

} vorbis_info_floor1;

static __declspec(noinline) int vorbis_dBquant(const float *x){
  int i= *x*7.3142857f+1023.5f;
  if(i>1023)return(1023);
  if(i<0)return(0);
  return i;
}

static __declspec(noinline) int __stdcall inspect_error(int x0,int x1,int y0,int y1,const float *mask,
			 const float *mdct,
			 vorbis_info_floor1 *info){
  int dy=y1-y0;
  int adx=x1-x0;
  int ady=abs(dy);
  int base=dy/adx;
  int sy=(dy<0?base-1:base+1);
  int x=x0;
  int y=y0;
  int err=0;
  int val=vorbis_dBquant(mask+x);
  int mse=0;
  int n=0;

  ady-=abs(base*adx);

  mse=(y-val);
  mse*=mse;
  n++;
  if(mdct[x]+info->twofitatten>=mask[x]){
    if(y+info->maxover<val)return(1);
    if(y-info->maxunder>val)return(1);
  }

  while(++x<x1){
    err=err+ady;
    if(err>=adx){
      err-=adx;
      y+=sy;
    }else{
      y+=base;
    }

    val=vorbis_dBquant(mask+x);
    mse+=((y-val)*(y-val));
    n++;
    if(mdct[x]+info->twofitatten>=mask[x]){
      if(val){
	if(y+info->maxover<val)return(1);
	if(y-info->maxunder>val)return(1);
      }
    }
  }

  if(info->maxover*info->maxover/n>info->maxerr)return(0);
  if(info->maxunder*info->maxunder/n>info->maxerr)return(0);
  if(mse/n>info->maxerr)return(1);
  return(0);
}

/* Same-TU caller (floor1.c floor1_fit's inspect_error call, 0x00CA28DE), present so the static gets retail's private
   convention. Not graded. */
int __cdecl vorbis_floor1_inspect_caller(const float *logmdct, const float *logmask, int lx, int hx, int ly, int hy,
                                         vorbis_info_floor1 *info){
  if(inspect_error(lx,hx,ly,hy,logmask,logmdct,info))return 1;
  return 0;
}
}