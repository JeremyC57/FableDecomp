#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 00c9ec40 mapping0_pack
// Leaf: @mapping0_pack@12
// Flags: /Gr
// Original libvorbis mapping0.c; BSD-style terms in reference COPYING.
#ifndef _MSC_VER
#define __fastcall
#endif
typedef void vorbis_info_mapping;
typedef struct vorbis_info{
  int version;
  int channels;
  long rate;

  /* The below bitrate declarations are *hints*.
     Combinations of the three values carry the following implications:
     
     all three set to the same value: 
       implies a fixed rate bitstream
     only nominal set: 
       implies a VBR stream that averages the nominal bitrate.  No hard 
       upper/lower limit
     upper and or lower set: 
       implies a VBR bitstream that obeys the bitrate limits. nominal 
       may also be set to give a nominal rate.
     none set:
       the coder does not care to speculate.
  */

  long bitrate_upper;
  long bitrate_nominal;
  long bitrate_lower;
  long bitrate_window;

  void *codec_setup;
} vorbis_info;
typedef struct vorbis_info_mapping0{
  int   submaps;  /* <= 16 */
  int   chmuxlist[256];   /* up to 256 channels in a Vorbis stream */
  
  int   floorsubmap[16];   /* [mux] submap to floors */
  int   residuesubmap[16]; /* [mux] submap to residue */

  int   coupling_steps;
  int   coupling_mag[256];
  int   coupling_ang[256];

} vorbis_info_mapping0;
typedef struct {
  long endbyte;
  int  endbit;

  unsigned char *buffer;
  unsigned char *ptr;
  long storage;
} oggpack_buffer;
extern "C" void __fastcall oggpack_write(oggpack_buffer*,unsigned long,int);
// Fidelity: Anonymous C++ scope preserves the original C file-local helper identity.
// The residue translation unit has a different function also named ilog.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
namespace {
int __fastcall ilog(unsigned int v){
  int ret=0;
  if(v)--v;
  while(v){
    ret++;
    v>>=1;
  }
  return(ret);
}
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
extern "C" void __fastcall mapping0_pack(vorbis_info *vi,vorbis_info_mapping *vm,
			  oggpack_buffer *opb){
  int i;
  vorbis_info_mapping0 *info=(vorbis_info_mapping0 *)vm;

  /* another 'we meant to do it this way' hack...  up to beta 4, we
     packed 4 binary zeros here to signify one submapping in use.  We
     now redefine that to mean four bitflags that indicate use of
     deeper features; bit0:submappings, bit1:coupling,
     bit2,3:reserved. This is backward compatable with all actual uses
     of the beta code. */

  if(info->submaps>1){
    oggpack_write(opb,1,1);
    oggpack_write(opb,info->submaps-1,4);
  }else
    oggpack_write(opb,0,1);

  if(info->coupling_steps>0){
    oggpack_write(opb,1,1);
    oggpack_write(opb,info->coupling_steps-1,8);
    
    for(i=0;i<info->coupling_steps;i++){
      oggpack_write(opb,info->coupling_mag[i],ilog(vi->channels));
      oggpack_write(opb,info->coupling_ang[i],ilog(vi->channels));
    }
  }else
    oggpack_write(opb,0,1);
  
  oggpack_write(opb,0,2); /* 2,3:reserved */

  /* we don't write the channel submappings if we only have one... */
  if(info->submaps>1){
    for(i=0;i<vi->channels;i++)
      oggpack_write(opb,info->chmuxlist[i],4);
  }
  for(i=0;i<info->submaps;i++){
    oggpack_write(opb,0,8); /* time submap unused */
    oggpack_write(opb,info->floorsubmap[i],8);
    oggpack_write(opb,info->residuesubmap[i],8);
  }
}

static unsigned cases,writeBit,readBit;
static unsigned char packet[4098];
static void check(bool p){if(!p){fprintf(stderr,"mapping pack case %u failed\n",cases);abort();}}
extern "C" void __fastcall oggpack_write(oggpack_buffer*b,unsigned long value,int n){check(n>=0&&n<=32&&writeBit+n<32768);for(int k=0;k<n;++k){if((value>>k)&1U)packet[1+writeBit/8]|=(unsigned char)(1U<<(writeBit%8));++writeBit;}b->endbit=writeBit%8;b->endbyte=writeBit/8;}
static unsigned take(int n){check(readBit+n<=writeBit);unsigned value=0;for(int k=0;k<n;++k){value|=((packet[1+readBit/8]>>(readBit%8))&1U)<<k;++readBit;}return value;}
int main(){const int channels[]={1,2,3,7,8,16,31,32,64,127,128,255,256};const int steps[]={0,1,2,7,31,255,256};
for(int ci=0;ci<13;++ci)for(int maps=1;maps<=16;++maps)for(int si=0;si<7;++si)for(int seed=0;seed<3;++seed){
 int ch=channels[ci],count=steps[si];if(ch==1&&count)continue;
 vorbis_info vi;vorbis_info_mapping0 info;oggpack_buffer buffer;
 memset(&vi,0,sizeof vi);memset(&info,0,sizeof info);memset(&buffer,0,sizeof buffer);
 vi.channels=ch;info.submaps=maps;info.coupling_steps=count;
 for(int c=0;c<ch;++c)info.chmuxlist[c]=(c+seed)%maps;
 for(int m=0;m<maps;++m){info.floorsubmap[m]=(m*13+seed*71)%256;info.residuesubmap[m]=(m*29+seed*101)%256;}
 for(int k=0;k<count;++k){info.coupling_mag[k]=(k+seed)%ch;info.coupling_ang[k]=(k+seed+1)%ch;}
 vorbis_info viBefore=vi;vorbis_info_mapping0 before=info;
 memset(packet,0,sizeof packet);packet[0]=0x5a;packet[4097]=0xa5;readBit=writeBit=0;
 mapping0_pack(&vi,&info,&buffer);
 unsigned submaps=take(1)?take(4)+1:1;check(submaps==(unsigned)maps);
 unsigned pairCount=take(1)?take(8)+1:0;check(pairCount==(unsigned)count);
 int width=0;while((1U<<width)<(unsigned)ch)++width;
 for(unsigned p=0;p<pairCount;++p){check(take(width)==(unsigned)info.coupling_mag[p]);check(take(width)==(unsigned)info.coupling_ang[p]);}
 check(take(2)==0);
 if(submaps>1)for(int c=0;c<ch;++c)check(take(4)==(unsigned)info.chmuxlist[c]);
 for(unsigned m=0;m<submaps;++m){check(take(8)==0);check(take(8)==(unsigned)info.floorsubmap[m]);check(take(8)==(unsigned)info.residuesubmap[m]);}
 check(readBit==writeBit&&buffer.endbyte==(long)(writeBit/8)&&buffer.endbit==(int)(writeBit%8));
 check(memcmp(&vi,&viBefore,sizeof vi)==0&&memcmp(&info,&before,sizeof info)==0);
 check(packet[0]==0x5a&&packet[4097]==0xa5);++cases;
}
check(cases==4080);puts("00c9ec40_TEST PASS");return 0;}