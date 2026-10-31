// libvorbis::drft_forward(drft_lookup* l, float* data) -- smallft.c: forward real FFT of data in place (no-op for n == 1) through drftf1
// Leaf: @drft_forward@8
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C9BC10, 30 bytes, `__fastcall` (l in ecx, data in edx). Sole caller 0x00C9F0FE (libvorbis).
// Name: libvorbis 1.0.1 smallft.c `drft_forward` -- (1) the body is drft_forward's exactly (n == 1 test, then
//   drftf1(n, data, trigcache, trigcache + n, splitcache)) and the whole upstream forward chain compiled in this TU
//   reproduces it; (2) its neighbours are the smallft.c tail in source order: 0x00C9BC30 drft_backward (same shape,
//   calls drftb1 0x00C9B9F0), 0x00C9BC50 drft_init and 0x00C9BC90 drft_clear (both landed as @drft_init@8 / drft_clear).
//   The catalog's DiscardAsyncData is a propagated label.
// Callees: drftf1 @ 0x00C98E80 (static, private convention: ch in eax, ifac in ecx, c in edi, n and wa pushed, `ret 8`);
//   it calls dradf2 0x00C969D0 (`ret 8`), dradf4 0x00C96D20 (`ret 10h`), dradfg 0x00C97600 (`ret 1Ch`).
// Lever (chunk 604): the parked retry1 TU had drftf1 as a same-TU static but dradf2/dradf4/dradfg extern, and VC7.1 gave
//   drftf1 c in esi / ifac in edx. With the real upstream bodies of dradf2, dradf4 and dradfg as same-TU
//   `static __declspec(noinline) __stdcall` functions (all four are callee-pop in retail), VC7.1 picks retail's private
//   convention for drftf1 itself (c in edi, ifac in ecx): RELOCATION_MATCH. drfti1 / fdrffti / drft_init stay as in the
//   landed rows (0x00C96800 / 0x00C969B0 / 0x00C9BC50).
// Original libvorbis smallft.c (libvorbis 1.0.1, lib/smallft.c, unmodified function bodies); BSD-style terms in reference COPYING.
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

static __declspec(noinline) void __stdcall dradf2(int ido,int l1,float *cc,float *ch,float *wa1){
  int i,k;
  float ti2,tr2;
  int t0,t1,t2,t3,t4,t5,t6;

  t1=0;
  t0=(t2=l1*ido);
  t3=ido<<1;
  for(k=0;k<l1;k++){
    ch[t1<<1]=cc[t1]+cc[t2];
    ch[(t1<<1)+t3-1]=cc[t1]-cc[t2];
    t1+=ido;
    t2+=ido;
  }
    
  if(ido<2)return;
  if(ido==2)goto L105;

  t1=0;
  t2=t0;
  for(k=0;k<l1;k++){
    t3=t2;
    t4=(t1<<1)+(ido<<1);
    t5=t1;
    t6=t1+t1;
    for(i=2;i<ido;i+=2){
      t3+=2;
      t4-=2;
      t5+=2;
      t6+=2;
      tr2=wa1[i-2]*cc[t3-1]+wa1[i-1]*cc[t3];
      ti2=wa1[i-2]*cc[t3]-wa1[i-1]*cc[t3-1];
      ch[t6]=cc[t5]+ti2;
      ch[t4]=ti2-cc[t5];
      ch[t6-1]=cc[t5-1]+tr2;
      ch[t4-1]=cc[t5-1]-tr2;
    }
    t1+=ido;
    t2+=ido;
  }

  if(ido%2==1)return;

 L105:
  t3=(t2=(t1=ido)-1);
  t2+=t0;
  for(k=0;k<l1;k++){
    ch[t1]=-cc[t2];
    ch[t1-1]=cc[t3];
    t1+=ido<<1;
    t2+=ido;
    t3+=ido;
  }
}

static __declspec(noinline) void __stdcall dradf4(int ido,int l1,float *cc,float *ch,float *wa1,
	    float *wa2,float *wa3){
  static float hsqt2 = .70710678118654752f;
  int i,k,t0,t1,t2,t3,t4,t5,t6;
  float ci2,ci3,ci4,cr2,cr3,cr4,ti1,ti2,ti3,ti4,tr1,tr2,tr3,tr4;
  t0=l1*ido;
  
  t1=t0;
  t4=t1<<1;
  t2=t1+(t1<<1);
  t3=0;

  for(k=0;k<l1;k++){
    tr1=cc[t1]+cc[t2];
    tr2=cc[t3]+cc[t4];

    ch[t5=t3<<2]=tr1+tr2;
    ch[(ido<<2)+t5-1]=tr2-tr1;
    ch[(t5+=(ido<<1))-1]=cc[t3]-cc[t4];
    ch[t5]=cc[t2]-cc[t1];

    t1+=ido;
    t2+=ido;
    t3+=ido;
    t4+=ido;
  }

  if(ido<2)return;
  if(ido==2)goto L105;


  t1=0;
  for(k=0;k<l1;k++){
    t2=t1;
    t4=t1<<2;
    t5=(t6=ido<<1)+t4;
    for(i=2;i<ido;i+=2){
      t3=(t2+=2);
      t4+=2;
      t5-=2;

      t3+=t0;
      cr2=wa1[i-2]*cc[t3-1]+wa1[i-1]*cc[t3];
      ci2=wa1[i-2]*cc[t3]-wa1[i-1]*cc[t3-1];
      t3+=t0;
      cr3=wa2[i-2]*cc[t3-1]+wa2[i-1]*cc[t3];
      ci3=wa2[i-2]*cc[t3]-wa2[i-1]*cc[t3-1];
      t3+=t0;
      cr4=wa3[i-2]*cc[t3-1]+wa3[i-1]*cc[t3];
      ci4=wa3[i-2]*cc[t3]-wa3[i-1]*cc[t3-1];

      tr1=cr2+cr4;
      tr4=cr4-cr2;
      ti1=ci2+ci4;
      ti4=ci2-ci4;

      ti2=cc[t2]+ci3;
      ti3=cc[t2]-ci3;
      tr2=cc[t2-1]+cr3;
      tr3=cc[t2-1]-cr3;

      ch[t4-1]=tr1+tr2;
      ch[t4]=ti1+ti2;

      ch[t5-1]=tr3-ti4;
      ch[t5]=tr4-ti3;

      ch[t4+t6-1]=ti4+tr3;
      ch[t4+t6]=tr4+ti3;

      ch[t5+t6-1]=tr2-tr1;
      ch[t5+t6]=ti1-ti2;
    }
    t1+=ido;
  }
  if(ido&1)return;

 L105:
  
  t2=(t1=t0+ido-1)+(t0<<1);
  t3=ido<<2;
  t4=ido;
  t5=ido<<1;
  t6=ido;

  for(k=0;k<l1;k++){
    ti1=-hsqt2*(cc[t1]+cc[t2]);
    tr1=hsqt2*(cc[t1]-cc[t2]);

    ch[t4-1]=tr1+cc[t6-1];
    ch[t4+t5-1]=cc[t6-1]-tr1;

    ch[t4]=ti1-cc[t1+t0];
    ch[t4+t5]=ti1+cc[t1+t0];

    t1+=ido;
    t2+=ido;
    t4+=t3;
    t6+=ido;
  }
}

static __declspec(noinline) void __stdcall dradfg(int ido,int ip,int l1,int idl1,float *cc,float *c1,
                          float *c2,float *ch,float *ch2,float *wa){

  static float tpi=6.283185307179586f;
  int idij,ipph,i,j,k,l,ic,ik,is;
  int t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10;
  float dc2,ai1,ai2,ar1,ar2,ds2;
  int nbd;
  float dcp,arg,dsp,ar1h,ar2h;
  int idp2,ipp2;
  
  arg=tpi/(float)ip;
  dcp=cos(arg);
  dsp=sin(arg);
  ipph=(ip+1)>>1;
  ipp2=ip;
  idp2=ido;
  nbd=(ido-1)>>1;
  t0=l1*ido;
  t10=ip*ido;

  if(ido==1)goto L119;
  for(ik=0;ik<idl1;ik++)ch2[ik]=c2[ik];

  t1=0;
  for(j=1;j<ip;j++){
    t1+=t0;
    t2=t1;
    for(k=0;k<l1;k++){
      ch[t2]=c1[t2];
      t2+=ido;
    }
  }

  is=-ido;
  t1=0;
  if(nbd>l1){
    for(j=1;j<ip;j++){
      t1+=t0;
      is+=ido;
      t2= -ido+t1;
      for(k=0;k<l1;k++){
        idij=is-1;
        t2+=ido;
        t3=t2;
        for(i=2;i<ido;i+=2){
          idij+=2;
          t3+=2;
          ch[t3-1]=wa[idij-1]*c1[t3-1]+wa[idij]*c1[t3];
          ch[t3]=wa[idij-1]*c1[t3]-wa[idij]*c1[t3-1];
        }
      }
    }
  }else{

    for(j=1;j<ip;j++){
      is+=ido;
      idij=is-1;
      t1+=t0;
      t2=t1;
      for(i=2;i<ido;i+=2){
        idij+=2;
        t2+=2;
        t3=t2;
        for(k=0;k<l1;k++){
          ch[t3-1]=wa[idij-1]*c1[t3-1]+wa[idij]*c1[t3];
          ch[t3]=wa[idij-1]*c1[t3]-wa[idij]*c1[t3-1];
          t3+=ido;
        }
      }
    }
  }

  t1=0;
  t2=ipp2*t0;
  if(nbd<l1){
    for(j=1;j<ipph;j++){
      t1+=t0;
      t2-=t0;
      t3=t1;
      t4=t2;
      for(i=2;i<ido;i+=2){
        t3+=2;
        t4+=2;
        t5=t3-ido;
        t6=t4-ido;
        for(k=0;k<l1;k++){
          t5+=ido;
          t6+=ido;
          c1[t5-1]=ch[t5-1]+ch[t6-1];
          c1[t6-1]=ch[t5]-ch[t6];
          c1[t5]=ch[t5]+ch[t6];
          c1[t6]=ch[t6-1]-ch[t5-1];
        }
      }
    }
  }else{
    for(j=1;j<ipph;j++){
      t1+=t0;
      t2-=t0;
      t3=t1;
      t4=t2;
      for(k=0;k<l1;k++){
        t5=t3;
        t6=t4;
        for(i=2;i<ido;i+=2){
          t5+=2;
          t6+=2;
          c1[t5-1]=ch[t5-1]+ch[t6-1];
          c1[t6-1]=ch[t5]-ch[t6];
          c1[t5]=ch[t5]+ch[t6];
          c1[t6]=ch[t6-1]-ch[t5-1];
        }
        t3+=ido;
        t4+=ido;
      }
    }
  }

L119:
  for(ik=0;ik<idl1;ik++)c2[ik]=ch2[ik];

  t1=0;
  t2=ipp2*idl1;
  for(j=1;j<ipph;j++){
    t1+=t0;
    t2-=t0;
    t3=t1-ido;
    t4=t2-ido;
    for(k=0;k<l1;k++){
      t3+=ido;
      t4+=ido;
      c1[t3]=ch[t3]+ch[t4];
      c1[t4]=ch[t4]-ch[t3];
    }
  }

  ar1=1.f;
  ai1=0.f;
  t1=0;
  t2=ipp2*idl1;
  t3=(ip-1)*idl1;
  for(l=1;l<ipph;l++){
    t1+=idl1;
    t2-=idl1;
    ar1h=dcp*ar1-dsp*ai1;
    ai1=dcp*ai1+dsp*ar1;
    ar1=ar1h;
    t4=t1;
    t5=t2;
    t6=t3;
    t7=idl1;

    for(ik=0;ik<idl1;ik++){
      ch2[t4++]=c2[ik]+ar1*c2[t7++];
      ch2[t5++]=ai1*c2[t6++];
    }

    dc2=ar1;
    ds2=ai1;
    ar2=ar1;
    ai2=ai1;

    t4=idl1;
    t5=(ipp2-1)*idl1;
    for(j=2;j<ipph;j++){
      t4+=idl1;
      t5-=idl1;

      ar2h=dc2*ar2-ds2*ai2;
      ai2=dc2*ai2+ds2*ar2;
      ar2=ar2h;

      t6=t1;
      t7=t2;
      t8=t4;
      t9=t5;
      for(ik=0;ik<idl1;ik++){
        ch2[t6++]+=ar2*c2[t8++];
        ch2[t7++]+=ai2*c2[t9++];
      }
    }
  }

  t1=0;
  for(j=1;j<ipph;j++){
    t1+=idl1;
    t2=t1;
    for(ik=0;ik<idl1;ik++)ch2[ik]+=c2[t2++];
  }

  if(ido<l1)goto L132;

  t1=0;
  t2=0;
  for(k=0;k<l1;k++){
    t3=t1;
    t4=t2;
    for(i=0;i<ido;i++)cc[t4++]=ch[t3++];
    t1+=ido;
    t2+=t10;
  }

  goto L135;

 L132:
  for(i=0;i<ido;i++){
    t1=i;
    t2=i;
    for(k=0;k<l1;k++){
      cc[t2]=ch[t1];
      t1+=ido;
      t2+=t10;
    }
  }

 L135:
  t1=0;
  t2=ido<<1;
  t3=0;
  t4=ipp2*t0;
  for(j=1;j<ipph;j++){

    t1+=t2;
    t3+=t0;
    t4-=t0;

    t5=t1;
    t6=t3;
    t7=t4;

    for(k=0;k<l1;k++){
      cc[t5-1]=ch[t6];
      cc[t5]=ch[t7];
      t5+=t10;
      t6+=ido;
      t7+=ido;
    }
  }

  if(ido==1)return;
  if(nbd<l1)goto L141;

  t1=-ido;
  t3=0;
  t4=0;
  t5=ipp2*t0;
  for(j=1;j<ipph;j++){
    t1+=t2;
    t3+=t2;
    t4+=t0;
    t5-=t0;
    t6=t1;
    t7=t3;
    t8=t4;
    t9=t5;
    for(k=0;k<l1;k++){
      for(i=2;i<ido;i+=2){
        ic=idp2-i;
        cc[i+t7-1]=ch[i+t8-1]+ch[i+t9-1];
        cc[ic+t6-1]=ch[i+t8-1]-ch[i+t9-1];
        cc[i+t7]=ch[i+t8]+ch[i+t9];
        cc[ic+t6]=ch[i+t9]-ch[i+t8];
      }
      t6+=t10;
      t7+=t10;
      t8+=ido;
      t9+=ido;
    }
  }
  return;

 L141:

  t1=-ido;
  t3=0;
  t4=0;
  t5=ipp2*t0;
  for(j=1;j<ipph;j++){
    t1+=t2;
    t3+=t2;
    t4+=t0;
    t5-=t0;
    for(i=2;i<ido;i+=2){
      t6=idp2+t1-i;
      t7=i+t3;
      t8=i+t4;
      t9=i+t5;
      for(k=0;k<l1;k++){
        cc[t7-1]=ch[t8-1]+ch[t9-1];
        cc[t6-1]=ch[t8-1]-ch[t9-1];
        cc[t7]=ch[t8]+ch[t9];
        cc[t6]=ch[t9]-ch[t8];
        t6+=t10;
        t7+=t10;
        t8+=ido;
        t9+=ido;
      }
    }
  }
}

static __declspec(noinline) void __stdcall drftf1(int n,float *c,float *ch,float *wa,int *ifac){
  int i,k1,l1,l2;
  int na,kh,nf;
  int ip,iw,ido,idl1,ix2,ix3;

  nf=ifac[1];
  na=1;
  l2=n;
  iw=n;

  for(k1=0;k1<nf;k1++){
    kh=nf-k1;
    ip=ifac[kh+1];
    l1=l2/ip;
    ido=n/l2;
    idl1=ido*l1;
    iw-=(ip-1)*ido;
    na=1-na;

    if(ip!=4)goto L102;

    ix2=iw+ido;
    ix3=ix2+ido;
    if(na!=0)
      dradf4(ido,l1,ch,c,wa+iw-1,wa+ix2-1,wa+ix3-1);
    else
      dradf4(ido,l1,c,ch,wa+iw-1,wa+ix2-1,wa+ix3-1);
    goto L110;

 L102:
    if(ip!=2)goto L104;
    if(na!=0)goto L103;

    dradf2(ido,l1,c,ch,wa+iw-1);
    goto L110;

  L103:
    dradf2(ido,l1,ch,c,wa+iw-1);
    goto L110;

  L104:
    if(ido==1)na=1-na;
    if(na!=0)goto L109;

    dradfg(ido,ip,l1,idl1,c,c,c,ch,ch,wa+iw-1);
    na=1;
    goto L110;

  L109:
    dradfg(ido,ip,l1,idl1,ch,ch,ch,c,c,wa+iw-1);
    na=0;

  L110:
    l2=l1;
  }

  if(na==1)return;

  for(i=0;i<n;i++)c[i]=ch[i];
}


void drft_forward(drft_lookup *l,float *data){
  if(l->n==1)return;
  drftf1(l->n,data,l->trigcache,l->trigcache+l->n,l->splitcache);
}

void drft_init(drft_lookup *l,int n){
  l->n=n;
  l->trigcache=(float*)calloc(3*n,sizeof(*l->trigcache));
  l->splitcache=(int*)calloc(32,sizeof(*l->splitcache));
  fdrffti(n, l->trigcache, l->splitcache);
}
}