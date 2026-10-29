#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 00ca2280 fit_line
// Leaf: _fit_line@16
// Flags:
// Original libvorbis floor1.c; BSD-style terms in reference COPYING.
#ifndef _MSC_VER
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
extern "C" __declspec(dllimport) double __cdecl floor(double);
#define rint(x) (floor((x)+0.5f))
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
extern "C" void __stdcall fit_line(lsfit_acc *a,int fits,int *y0,int *y1){
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

static unsigned cases;
static void check(bool p){if(!p){fprintf(stderr,"line fit case %u failed\n",cases);abort();}}
#ifdef _MSC_VER
static double __cdecl test_floor(double x){check(x==x&&x>-8192&&x<8192);int n=(int)x;if((double)n>x)--n;return (double)n;}
extern "C" {double(__cdecl* _imp__floor)(double)=test_floor;}
#endif
static int clamp(int x){return x<0?0:(x>1023?1023:x);}
static void add(lsfit_acc*a,int x,int y){a->xa+=x;a->ya+=y;a->x2a+=x*x;a->y2a+=y*y;a->xya+=x*y;++a->an;}
static void verify(lsfit_acc*a,int fits,int first,int last,int expectedFirst,int expectedLast){lsfit_acc before[7];memcpy(before,a,sizeof before);int out[6]={0x556677,first,0x778899,last,0x112233,0x224466};fit_line(a,fits,out+1,out+3);check(out[1]==expectedFirst&&out[3]==expectedLast&&out[0]==0x556677&&out[2]==0x778899&&out[4]==0x112233&&out[5]==0x224466);check(memcmp(a,before,sizeof before)==0);++cases;}
int main(){const int slopes[]={-32,-3,0,2,32};const int intercepts[]={-100,0,17,500,1200};for(int fits=1;fits<=5;++fits)for(int base=0;base<=7;base+=7)for(int si=0;si<5;++si)for(int bi=0;bi<5;++bi)for(int noise=-2;noise<=2;++noise)for(int anchors=0;anchors<4;++anchors){lsfit_acc a[7];memset(a,0,sizeof a);int slope=slopes[si],intercept=intercepts[bi];for(int f=0;f<fits;++f){a[f].x0=base+f*3;a[f].x1=base+f*3+4;for(int k=0;k<3;++k){int x=base+f*3+k+1;int residual=k==1?-2*noise:noise;add(a+f,x,intercept+slope*x+residual);}}int first=intercept+slope*a[0].x0,last=intercept+slope*a[fits-1].x1;verify(a,fits,(anchors&1)&&first>=0?first:-1,(anchors&2)&&last>=0?last:-1,clamp(first),clamp(last));}
for(int fits=1;fits<=5;++fits){lsfit_acc a[7];memset(a,0,sizeof a);a[0].x0=2;a[fits-1].x1=19;verify(a,fits,-1,-1,0,0);}
const int samples[5][3]={{0,0,1},{0,1,0},{0,0,2},{5,7,7},{1023,1023,1024}};const int wanted[5][2]={{0,1},{0,0},{0,2},{5,7},{1023,1023}};for(int i=0;i<5;++i){lsfit_acc a[7];memset(a,0,sizeof a);a[0].x0=0;a[0].x1=2;for(int k=0;k<3;++k)add(a,k,samples[i][k]);verify(a,1,-1,-1,wanted[i][0],wanted[i][1]);}check(cases==5010);puts("00ca2280_TEST PASS");return 0;}