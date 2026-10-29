#include <stdio.h>
#include <stdlib.h>
// 00c9ea80 _vorbis_apply_window
// Leaf: @_vorbis_apply_window@24
// Flags: /Gr
// Original libvorbis window.c; BSD-style terms in reference libvorbis COPYING.
#ifndef _MSC_VER
#define __fastcall
#endif
extern "C" float *vwin[8];
extern "C" void __fastcall _vorbis_apply_window(float *d,int *winno,long *blocksizes,
			  int lW,int W,int nW){
  lW=(W?lW:0);
  nW=(W?nW:0);
  
  {
    float *windowLW=vwin[winno[lW]];
    float *windowNW=vwin[winno[nW]];

    long n=blocksizes[W];
    long ln=blocksizes[lW];
    long rn=blocksizes[nW];
    
    long leftbegin=n/4-ln/4;
    long leftend=leftbegin+ln/2;
    
    long rightbegin=n/2+n/4-rn/4;
    long rightend=rightbegin+rn/2;
    
    int i,p;
    
    for(i=0;i<leftbegin;i++)
      d[i]=0.f;
    
    for(p=0;i<leftend;i++,p++)
      d[i]*=windowLW[p];
    
    for(i=rightbegin,p=rn/2-1;i<rightend;i++,p--)
      d[i]*=windowNW[p];
    
    for(;i<n;i++)
      d[i]=0.f;
  }
}


extern "C" float *vwin[8];
float *vwin[8];
static float table[8][4096];
static float data[8194],snapshot[8194];
static unsigned cases;
static void check(bool value){if(!value){fprintf(stderr,"window case %u failed\n",cases);abort();}}
int main(){
 for(int k=0;k<8;++k){vwin[k]=table[k];for(int i=0;i<4096;++i)table[k][i]=(i+1)*(k+1)*(1.0f/32768.0f);}
 for(int a=0;a<8;++a)for(int b=a;b<8;++b)for(int previous=0;previous<2;++previous)for(int current=0;current<2;++current)for(int next=0;next<2;++next)for(int seed=0;seed<3;++seed){
 int winno[2]={a,b};long blocksizes[2]={64L<<a,64L<<b};int n=blocksizes[current];int left=current?previous:0;int right=current?next:0;
 for(int i=0;i<8194;++i)data[i]=snapshot[i]=((i*7+seed*11)%17-8)*0.125f;
 _vorbis_apply_window(data+1,winno,blocksizes,previous,current,next);
 int leftStart=(n-blocksizes[left])/4,leftStop=leftStart+blocksizes[left]/2;
 int rightStart=(3*n-blocksizes[right])/4,rightStop=rightStart+blocksizes[right]/2;
 for(int i=0;i<n;++i){float expected=snapshot[i+1];if(i<leftStart||i>=rightStop)expected=0.0f;else if(i<leftStop)expected*=table[winno[left]][i-leftStart];else if(i>=rightStart)expected*=table[winno[right]][rightStop-i-1];check(data[i+1]==expected);}
 check(data[0]==snapshot[0]);for(int i=n+1;i<8194;++i)check(data[i]==snapshot[i]);check(winno[0]==a&&winno[1]==b);check(blocksizes[0]==(64L<<a)&&blocksizes[1]==(64L<<b));++cases;
 }
 check(cases==864);puts("00c9ea80_TEST PASS");return 0;
}