// floor1_fit(vorbis_block*, vorbis_look_floor1*, const float*, const float*) -- fit Vorbis floor-1 posts
// Leaf: @floor1_fit@16
// Layout: original libvorbis 1.0.1 floor1.c/backends.h vendor structures; native posts +0x504, vi +0x510.
#include <stddef.h>
#include <string.h>
extern "C" __declspec(dllimport) __declspec(noreturn) void __cdecl exit(int);
extern "C" int __cdecl abs(int);
extern "C" __declspec(dllimport) double __cdecl floor(double);
#pragma intrinsic(abs,memset)
#define rint(x) (floor((x)+0.5f))
#define VIF_POSIT 63
#define VIF_CLASS 16
#define VIF_PARTS 31
typedef struct vorbis_block vorbis_block;
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
typedef struct {
  int sorted_index[VIF_POSIT+2];
  int forward_index[VIF_POSIT+2];
  int reverse_index[VIF_POSIT+2];
  
  int hineighbor[VIF_POSIT];
  int loneighbor[VIF_POSIT];
  int posts;

  int n;
  int quant_q;
  vorbis_info_floor1 *vi;

  long phrasebits;
  long postbits;
  long frames;
} vorbis_look_floor1;

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


extern "C" {
void* __fastcall _vorbis_block_alloc(vorbis_block*,long);
static __declspec(noinline) int __stdcall render_point(int x0,int x1,int y0,int y1,int x){
  y0&=0x7fff; /* mask off flag */
  y1&=0x7fff;
    
  {
    int dy=y1-y0;
    int adx=x1-x0;
    int ady=abs(dy);
    int err=ady*(x-x0);
    
    int off=err/adx;
    if(dy<0)return(y0-off);
    return(y0+off);
  }
}
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
static __declspec(noinline) void __stdcall fit_line(lsfit_acc *a,int fits,int *y0,int *y1){
  long x=0,y=0,x2=0,y2=0,xy=0,an=0,i;
  long x0=a[0].x0;
  long x1=a[fits-1].x1;

  for(i=0;i<fits;i++){
    x+=a[i].xa;
    y+=a[i].ya;
    x2+=a[i].x2a;
    y2+=a[i].y2a;
    xy+=a[i].xya;
    an+=a[i].an;
  }

  if(*y0>=0){
    x+=   x0;
    y+=  *y0;
    x2+=  x0 *  x0;
    y2+= *y0 * *y0;
    xy+= *y0 *  x0;
    an++;
  }

  if(*y1>=0){
    x+=   x1;
    y+=  *y1;
    x2+=  x1 *  x1;
    y2+= *y1 * *y1;
    xy+= *y1 *  x1;
    an++;
  }
  
  if(an){
    /* need 64 bit multiplies, which C doesn't give portably as int */
    double fx=x;
    double fy=y;
    double fx2=x2;
    double fxy=xy;
    double denom=1./(an*fx2-fx*fx);
    double a=(fy*fx2-fxy*fx)*denom;
    double b=(an*fxy-fx*fy)*denom;
    *y0=rint(a+b*x0);
    *y1=rint(a+b*x1);
    
    /* limit to our range! */
    if(*y0>1023)*y0=1023;
    if(*y1>1023)*y1=1023;
    if(*y0<0)*y0=0;
    if(*y1<0)*y1=0;
    
  }else{
    *y0=0;
    *y1=0;
  }
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
static __declspec(noinline) int post_Y(int *A,int *B,int pos){
  if(A[pos]<0)
    return B[pos];
  if(B[pos]<0)
    return A[pos];

  return (A[pos]+B[pos])>>1;
}
int* __fastcall floor1_fit(vorbis_block *vb,vorbis_look_floor1 *look,
			  const float *logmdct,   /* in */
			  const float *logmask){
  long i,j;
  vorbis_info_floor1 *info=look->vi;
  long n=look->n;
  long posts=look->posts;
  long nonzero=0;
  lsfit_acc fits[VIF_POSIT+1];
  int fit_valueA[VIF_POSIT+2]; /* index by range list position */
  int fit_valueB[VIF_POSIT+2]; /* index by range list position */

  int loneighbor[VIF_POSIT+2]; /* sorted index of range list position (+2) */
  int hineighbor[VIF_POSIT+2]; 
  int *output=NULL;
  int memo[VIF_POSIT+2];

  for(i=0;i<posts;i++)fit_valueA[i]=-200; /* mark all unused */
  for(i=0;i<posts;i++)fit_valueB[i]=-200; /* mark all unused */
  for(i=0;i<posts;i++)loneighbor[i]=0; /* 0 for the implicit 0 post */
  for(i=0;i<posts;i++)hineighbor[i]=1; /* 1 for the implicit post at n */
  for(i=0;i<posts;i++)memo[i]=-1;      /* no neighbor yet */

  /* quantize the relevant floor points and collect them into line fit
     structures (one per minimal division) at the same time */
  if(posts==0){
    nonzero+=accumulate_fit(logmask,logmdct,0,n,fits,n,info);
  }else{
    for(i=0;i<posts-1;i++)
      nonzero+=accumulate_fit(logmask,logmdct,look->sorted_index[i],
			      look->sorted_index[i+1],fits+i,
			      n,info);
  }
  
  if(nonzero){
    /* start by fitting the implicit base case.... */
    int y0=-200;
    int y1=-200;
    fit_line(fits,posts-1,&y0,&y1);

    fit_valueA[0]=y0;
    fit_valueB[0]=y0;
    fit_valueB[1]=y1;
    fit_valueA[1]=y1;

    /* Non degenerate case */
    /* start progressive splitting.  This is a greedy, non-optimal
       algorithm, but simple and close enough to the best
       answer. */
    for(i=2;i<posts;i++){
      int sortpos=look->reverse_index[i];
      int ln=loneighbor[sortpos];
      int hn=hineighbor[sortpos];
      
      /* eliminate repeat searches of a particular range with a memo */
      if(memo[ln]!=hn){
	/* haven't performed this error search yet */
	int lsortpos=look->reverse_index[ln];
	int hsortpos=look->reverse_index[hn];
	memo[ln]=hn;
		
	{
	  /* A note: we want to bound/minimize *local*, not global, error */
	  int lx=info->postlist[ln];
	  int hx=info->postlist[hn];	  
	  int ly=post_Y(fit_valueA,fit_valueB,ln);
	  int hy=post_Y(fit_valueA,fit_valueB,hn);
	  
	  if(ly==-1 || hy==-1){
	    exit(1);
	  }

	  if(inspect_error(lx,hx,ly,hy,logmask,logmdct,info)){
	    /* outside error bounds/begin search area.  Split it. */
	    int ly0=-200;
	    int ly1=-200;
	    int hy0=-200;
	    int hy1=-200;
	    fit_line(fits+lsortpos,sortpos-lsortpos,&ly0,&ly1);
	    fit_line(fits+sortpos,hsortpos-sortpos,&hy0,&hy1);
	    
	    /* store new edge values */
	    fit_valueB[ln]=ly0;
	    if(ln==0)fit_valueA[ln]=ly0;
	    fit_valueA[i]=ly1;
	    fit_valueB[i]=hy0;
	    fit_valueA[hn]=hy1;
	    if(hn==1)fit_valueB[hn]=hy1;
	    
	    if(ly1>=0 || hy0>=0){
	      /* store new neighbor values */
	      for(j=sortpos-1;j>=0;j--)
		if(hineighbor[j]==hn)
		  hineighbor[j]=i;
		else
		  break;
	      for(j=sortpos+1;j<posts;j++)
		if(loneighbor[j]==ln)
		  loneighbor[j]=i;
		else
		  break;
	      
	    }
	  }else{
	    
	    fit_valueA[i]=-200;
	    fit_valueB[i]=-200;
	  }
	}
      }
    }
  
    output=(int*)_vorbis_block_alloc(vb,sizeof(*output)*posts);
    
    output[0]=post_Y(fit_valueA,fit_valueB,0);
    output[1]=post_Y(fit_valueA,fit_valueB,1);
    
    /* fill in posts marked as not using a fit; we will zero
       back out to 'unused' when encoding them so long as curve
       interpolation doesn't force them into use */
    for(i=2;i<posts;i++){
      int ln=look->loneighbor[i-2];
      int hn=look->hineighbor[i-2];
      int x0=info->postlist[ln];
      int x1=info->postlist[hn];
      int y0=output[ln];
      int y1=output[hn];
      
      int predicted=render_point(x0,x1,y0,y1,info->postlist[i]);
      int vx=post_Y(fit_valueA,fit_valueB,i);
      
      if(vx>=0 && predicted!=vx){ 
	output[i]=vx;
      }else{
	output[i]= predicted|0x8000;
      }
    }
  }

  return(output);
  
}

}