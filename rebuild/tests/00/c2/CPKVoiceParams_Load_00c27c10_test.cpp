#include <assert.h>
#include <stdio.h>
#include <string.h>
// BEGIN exact source snapshot SHA256 9bdb7e6665c88cc8c02a56fecbe471e06921964e690a208212f703950c1897e7
// 00c27c10 SerializedState_00c27c10::Load
// Flags: /O2 /Oy
// Leaf: Load
// Layout: retail receiver fields +1c..44; manifest CBrainOutput disagrees with PDB fields. Packed input offsets established directly from retail loads.
#pragma pack(push,1)
struct Input {unsigned a,b,c,d;unsigned char e,pad11[3];unsigned f,g,h,i;unsigned char j,pad25,flags0,flags1;unsigned short k,l,m,pad2e;unsigned values[9];bool flag54;unsigned char pad55[3];unsigned tail;};
struct CPKVoiceParams {
 char prefix[28];unsigned a,b,c,d,e,f,g,h;unsigned char j,pad3d[3];unsigned i;unsigned short k;
 void Set0_00c203c0(unsigned);void Set1_00c203e0(unsigned);void Set2_00c20480(unsigned);void Set3_00c204a0(unsigned);void Set4_00c204c0(unsigned);void SetFilter_00c20440(long);void Set6_00c20560(unsigned);void Set7_00c20460(bool);void Set8_00c20400(unsigned);void Set9_00c20420(unsigned);void SetPair0_00c27ba0(unsigned short,unsigned short);void SetPair1_00c27bf0(unsigned short,unsigned short);
 void Load(const Input*);
};
#pragma pack(pop)
void CPKVoiceParams::Load(const Input* s){
 a=s->a;b=s->b;c=s->c;d=s->d;e=s->e;f=s->f;g=s->g;h=s->h;j=s->j;i=s->i;k=s->k;
 if(s->flags0&1)Set0_00c203c0(s->values[0]);if(s->flags0&2)Set1_00c203e0(s->values[1]);if(s->flags0&4)Set2_00c20480(s->values[3]);if(s->flags0&8)Set3_00c204a0(s->values[4]);if(s->flags0&16)Set4_00c204c0(s->values[5]);if(s->flags0&32)SetFilter_00c20440((long)s->values[6]);if(s->flags0&64)Set6_00c20560(s->values[8]);if(s->flags0&128)Set7_00c20460(s->flag54);if(s->flags1&1)Set8_00c20400(s->values[2]);if(s->flags1&2)Set9_00c20420(s->tail);
 SetPair0_00c27ba0(s->l,*(const unsigned short*)((const char*)s+0x4c));SetPair1_00c27bf0(s->m,*(const unsigned short*)((const char*)s+0x4e));
}

// END exact source snapshot
static unsigned seen[10],mask;
void CPKVoiceParams::Set0_00c203c0(unsigned n){seen[0]=n;mask|=1U<<0;}
void CPKVoiceParams::Set1_00c203e0(unsigned n){seen[1]=n;mask|=1U<<1;}
void CPKVoiceParams::Set2_00c20480(unsigned n){seen[2]=n;mask|=1U<<2;}
void CPKVoiceParams::Set3_00c204a0(unsigned n){seen[3]=n;mask|=1U<<3;}
void CPKVoiceParams::Set4_00c204c0(unsigned n){seen[4]=n;mask|=1U<<4;}
void CPKVoiceParams::SetFilter_00c20440(long n){seen[5]=(unsigned)n;mask|=1U<<5;}
void CPKVoiceParams::Set6_00c20560(unsigned n){seen[6]=n;mask|=1U<<6;}
void CPKVoiceParams::Set7_00c20460(bool n){seen[7]=n;mask|=1U<<7;}
void CPKVoiceParams::Set8_00c20400(unsigned n){seen[8]=n;mask|=1U<<8;}
void CPKVoiceParams::Set9_00c20420(unsigned n){seen[9]=n;mask|=1U<<9;}
static unsigned pair0,pair1;void CPKVoiceParams::SetPair0_00c27ba0(unsigned short a,unsigned short b){pair0=(unsigned(a)<<16)|b;}void CPKVoiceParams::SetPair1_00c27bf0(unsigned short a,unsigned short b){pair1=(unsigned(a)<<16)|b;}
int main(){Input input;memset(&input,0,sizeof(input));CPKVoiceParams s;memset(&s,0,sizeof(s));input.a=17;input.e=255;input.j=9;input.l=3;input.m=4;s.Load(&input);assert(s.a==17 && s.e==255 && s.j==9 && mask==0 && pair0==0x30000 && pair1==0x40000);input.flags0=255;input.flags1=3;for(int i=0;i<9;++i)input.values[i]=10+i;input.flag54=true;input.tail=99;s.Load(&input);assert(mask==1023 && seen[0]==10 && seen[2]==13 && seen[7]==1 && seen[9]==99);puts("00c27c10_TEST PASS");return 0;}