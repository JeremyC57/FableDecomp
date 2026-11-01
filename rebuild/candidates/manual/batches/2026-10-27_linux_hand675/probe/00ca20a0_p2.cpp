// libvorbis::accumulate_fit(const float* flr, const float* mdct, int x0, int x1, lsfit_acc* a, int n, vorbis_info_floor1* info) -- libvorbis 1.0.1 floor1.c: accumulate the least-squares sums of one floor-1 segment, weighted toward the bins the mdct actually uses
// Leaf: _accumulate_fit
#include <string.h>
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

typedef struct lsfit_acc{
  long x0;
  long x1;

  long xa;
  long ya;
  long x2a;
  long y2a;
  long xya;
  long an;
} lsfit_acc;

static __declspec(noinline) int vorbis_dBquant(const float *x){
  int i= *x*7.3142857f+1023.5f;
  if(i>1023)return(1023);
  if(i<0)return(0);
  return i;
}

static __declspec(noinline) int __stdcall accumulate_fit(const float *flr,const float *mdct,
			  int x0, int x1,lsfit_acc *a,
			  int n,vorbis_info_floor1 *info){
  long i;
  int quantized=vorbis_dBquant(flr+x0);

  long xa=0,ya=0,x2a=0,y2a=0,xya=0,na=0, xb=0,yb=0,x2b=0,y2b=0,xyb=0,nb=0;

  memset(a,0,sizeof(*a));
  a->x0=x0;
  a->x1=x1;
  if(x1>=n)x1=n-1;

  for(i=x0;i<=x1;i++){
    int quantized=vorbis_dBquant(flr+i);
    if(quantized){
      if(mdct[i]+info->twofitatten>=flr[i]){
	xa  += i;
	ya  += quantized;
	x2a += i*i;
	y2a += quantized*quantized;
	xya += i*quantized;
	na++;
      }else{
	xb  += i;
	yb  += quantized;
	x2b += i*i;
	y2b += quantized*quantized;
	xyb += i*quantized;
	nb++;
      }
    }
  }

  xb+=xa;
  yb+=ya;
  x2b+=x2a;
  y2b+=y2a;
  xyb+=xya;
  nb+=na;

  /* weight toward the actually used frequencies if we meet the threshhold */
  {
    int weight=nb*info->twofitweight/(na+1);

    a->xa=xa*weight+xb;
    a->ya=ya*weight+yb;
    a->x2a=x2a*weight+x2b;
    a->y2a=y2a*weight+y2b;
    a->xya=xya*weight+xyb;
    a->an=na*weight+nb;
  }

  return(na);
}

/* Same-TU caller (floor1.c floor1_fit's segment loop, 0x00CA276D / 0x00CA27B4), present so the static gets retail's
   private convention. Not graded. */
int __cdecl vorbis_floor1_accumulate_caller(const float *logmdct, const float *logmask, lsfit_acc *fits,
                                            int posts, int n, vorbis_info_floor1 *info){
  int i,nonzero=0;
  for(i=0;i<posts-1;i++)
    nonzero+=accumulate_fit(logmask,logmdct,info->postlist[i],info->postlist[i+1],fits+i,n,info);
  return nonzero;
}
}
