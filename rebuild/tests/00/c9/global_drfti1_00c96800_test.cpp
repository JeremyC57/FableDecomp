#include <stdio.h>
#include <stdlib.h>
// 00c96800 drfti1
// Leaf: _drfti1@12
// Flags:
// Original libvorbis smallft.c; BSD-style terms in reference COPYING.
#ifdef _MSC_VER
#pragma intrinsic(cos,sin)
#else
#define __stdcall
#define __cdecl
#endif
extern "C" double __cdecl cos(double);
extern "C" double __cdecl sin(double);
extern "C" void __stdcall drfti1(int n, float *wa, int *ifac){
  static int ntryh[4] = { 4,2,3,5 };
  static float tpi = 6.28318530717958648f;
  float arg,argh,argld,fi;
  int ntry=0,i,j=-1;
  int k1, l1, l2, ib;
  int ld, ii, ip, is, nq, nr;
  int ido, ipm, nfm1;
  int nl=n;
  int nf=0;

 L101:
  j++;
  if (j < 4)
    ntry=ntryh[j];
  else
    ntry+=2;

 L104:
  nq=nl/ntry;
  nr=nl-ntry*nq;
  if (nr!=0) goto L101;

  nf++;
  ifac[nf+1]=ntry;
  nl=nq;
  if(ntry!=2)goto L107;
  if(nf==1)goto L107;

  for (i=1;i<nf;i++){
    ib=nf-i+1;
    ifac[ib+1]=ifac[ib];
  }
  ifac[2] = 2;

 L107:
  if(nl!=1)goto L104;
  ifac[0]=n;
  ifac[1]=nf;
  argh=tpi/n;
  is=0;
  nfm1=nf-1;
  l1=1;

  if(nfm1==0)return;

  for (k1=0;k1<nfm1;k1++){
    ip=ifac[k1+2];
    ld=0;
    l2=l1*ip;
    ido=n/l2;
    ipm=ip-1;

    for (j=0;j<ipm;j++){
      ld+=l1;
      i=is;
      argld=(float)ld*argh;
      fi=0.f;
      for (ii=2;ii<ido;ii+=2){
	fi+=1.f;
	arg=fi*argld;
	wa[i++]=cos(arg);
	wa[i++]=sin(arg);
      }
      is+=ido;
    }
    l1=l2;
  }
}

static unsigned cases;
static void check(bool p){if(!p){fprintf(stderr,"FFT setup case %u failed\n",cases);abort();}}
static double absolute(double x){return x<0?-x:x;}
static int factorize(int n,int*out){int count=0,power=0;while(n%2==0){++power;n/=2;}if(power&1)out[count++]=2;for(int k=0;k<power/2;++k)out[count++]=4;for(int p=3;p<=n;p+=2)while(n%p==0){out[count++]=p;n/=p;}return count;}
static void run(int n,int pattern){float data[2060],expected[2060];bool changed[2060];int factors[32],wanted[32];float sentinel=(pattern-1)*16.0f;for(int i=0;i<2060;++i){data[i]=expected[i]=sentinel;changed[i]=false;}for(int i=0;i<32;++i)factors[i]=-71;int count=factorize(n,wanted);drfti1(n,data+1,factors+1);check(factors[0]==-71&&factors[1]==n&&factors[2]==count);for(int i=0;i<count;++i)check(factors[i+3]==wanted[i]);for(int i=count+3;i<32;++i)check(factors[i]==-71);
int prefix=1,offset=0;double twoPi=(double)6.28318530717958648f;for(int stage=0;stage<count-1;++stage){int factor=wanted[stage],group=prefix*factor,width=n/group;for(int frequency=1;frequency<factor;++frequency){for(int k=0;k<(width-1)/2;++k){double angle=twoPi*prefix*frequency*(k+1)/n;int at=1+offset+(frequency-1)*width+2*k;expected[at]=(float)cos(angle);expected[at+1]=(float)sin(angle);changed[at]=changed[at+1]=true;}}offset+=(factor-1)*width;prefix=group;}
for(int i=0;i<2060;++i){if(changed[i])check(data[i]==data[i]&&absolute((double)data[i]-expected[i])<1.0/32768.0);else check(data[i]==sentinel);}++cases;}
int main(){for(int pattern=0;pattern<3;++pattern){for(int n=2;n<=128;++n)run(n,pattern);run(256,pattern);run(257,pattern);run(512,pattern);run(1024,pattern);}check(cases==393);puts("00c96800_TEST PASS");return 0;}