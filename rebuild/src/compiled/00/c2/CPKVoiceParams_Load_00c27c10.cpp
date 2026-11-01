// 00c27c10 SerializedState_00c27c10::Load
// Flags: /O2 /Oy
// Leaf: Load
// Naming (naming wave 21): the receiver is the libpk CPKVoiceParams record (ctor 0x00C10A30; SetMinDistance 0x00C20480 / SetMaxDistance 0x00C204A0 / SetScale 0x00C20420 take floats: the stored words are passed as their bit patterns); the other setters keep their address names.
// Layout: retail receiver fields +1c..44; manifest CBrainOutput disagrees with PDB fields. Packed input offsets established directly from retail loads.
#pragma pack(push,1)
struct Input {unsigned a,b,c,d;unsigned char e,pad11[3];unsigned f,g,h,i;unsigned char j,pad25,flags0,flags1;unsigned short k,l,m,pad2e;unsigned values[9];bool flag54;unsigned char pad55[3];unsigned tail;};
struct CPKVoiceParams {
 char prefix[28];unsigned a,b,c,d,e,f,g,h;unsigned char j,pad3d[3];unsigned i;unsigned short k;
 void Set0_00c203c0(unsigned);void Set1_00c203e0(unsigned);void SetMinDistance(float);void SetMaxDistance(float);void Set4_00c204c0(unsigned);void SetFilter_00c20440(long);void Set6_00c20560(unsigned);void Set7_00c20460(bool);void Set8_00c20400(unsigned);void SetScale(float);void SetPair0_00c27ba0(unsigned short,unsigned short);void SetPair1_00c27bf0(unsigned short,unsigned short);
 void Load(const Input*);
};
#pragma pack(pop)
void CPKVoiceParams::Load(const Input* s){
 a=s->a;b=s->b;c=s->c;d=s->d;e=s->e;f=s->f;g=s->g;h=s->h;j=s->j;i=s->i;k=s->k;
 if(s->flags0&1)Set0_00c203c0(s->values[0]);if(s->flags0&2)Set1_00c203e0(s->values[1]);if(s->flags0&4)SetMinDistance(*(const float*)&s->values[3]);if(s->flags0&8)SetMaxDistance(*(const float*)&s->values[4]);if(s->flags0&16)Set4_00c204c0(s->values[5]);if(s->flags0&32)SetFilter_00c20440((long)s->values[6]);if(s->flags0&64)Set6_00c20560(s->values[8]);if(s->flags0&128)Set7_00c20460(s->flag54);if(s->flags1&1)Set8_00c20400(s->values[2]);if(s->flags1&2)SetScale(*(const float*)&s->tail);
 SetPair0_00c27ba0(s->l,*(const unsigned short*)((const char*)s+0x4c));SetPair1_00c27bf0(s->m,*(const unsigned short*)((const char*)s+0x4e));
}