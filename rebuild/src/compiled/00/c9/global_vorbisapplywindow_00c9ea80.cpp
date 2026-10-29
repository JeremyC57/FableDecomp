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
