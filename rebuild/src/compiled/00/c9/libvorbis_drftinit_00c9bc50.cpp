// libvorbis::drft_init(drft_lookup* l, int n) -- smallft.c: record n, allocate the 3n-float trig cache and the 32-int split cache, then set them up through fdrffti
// Shape (retry1): parked as an "LTCG eax-argument convention" gap; this is libvorbis 1.0.1 smallft.c's static fdrffti. Inside one
//   smallft.c TU (C, /Gr like the landed drft_clear) with the static drfti1 0x00C96800 and the caller drft_init 0x00C9BC50, VC7.1 /O2
//   gives the static retail's private convention itself (n in eax, wsave in edx, ifac in ecx).
// Leaf: @drft_init@8
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9BC50, 53 bytes, `__fastcall` (l in ecx, n in edx), calloc import [0x01440058] hoisted into
//   ebx, tail `jmp` to fdrffti 0x00C969B0 with n/wsave/ifac left in eax/edx/ecx. Callers 0x00C8F2D7 / 0x00C8F2E2.
// Callee: drfti1 @ 0x00C96800 (static, `__stdcall`-style `ret 0Ch`; landed as `_drfti1@12`).
// Label correction: the catalog's `_Cons_val<allocator<CCharString>,...>` is propagated; the parked FFT_InitFactorsUnlessTrivial_00c969b0
//   is libvorbis fdrffti (smallft.c: `if (n == 1) return; drfti1(n, wsave+n, ifac);`).
// Original libvorbis smallft.c; BSD-style terms in reference COPYING.
#pragma intrinsic(cos,sin)
extern "C" {
double __cdecl cos(double);
double __cdecl sin(double);
__declspec(dllimport) void* __cdecl calloc(unsigned int count, unsigned int size);   // [0x01440058]

typedef struct {
  int n;
  float *trigcache;
  int *splitcache;
} drft_lookup;

static __declspec(noinline) void __stdcall drfti1(int n, float *wa, int *ifac){
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

static __declspec(noinline) void fdrffti(int n, float *wsave, int *ifac){

  if (n == 1) return;
  drfti1(n, wsave+n, ifac);
}

void drft_init(drft_lookup *l,int n){
  l->n=n;
  l->trigcache=(float*)calloc(3*n,sizeof(*l->trigcache));
  l->splitcache=(int*)calloc(32,sizeof(*l->splitcache));
  fdrffti(n, l->trigcache, l->splitcache);
}
}