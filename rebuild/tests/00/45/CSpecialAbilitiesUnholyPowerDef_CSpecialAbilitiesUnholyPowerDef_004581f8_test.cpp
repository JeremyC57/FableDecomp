#include <cstdio>
// CSpecialAbilitiesUnholyPowerDef::CSpecialAbilitiesUnholyPowerDef() -- initialise inherited definition, scalar defaults, and nine empty vector members at their padded retail offsets; retail @004581f8 length 188.
// The quarantine header for UnholyPowerDef places 16-byte vectors at +0x40, but retail constructor bytes zero +0x3c..+0x74 then initialize nine 12-byte triples at +0x78, so the test embeds the retail-derived local layout.
#pragma optimize("s", on)
struct CParentDefClassBase { unsigned char base[0x3c]; CParentDefClassBase(); }; // 0x00430370
struct Vec { unsigned int first,last,end; };
struct CSpecialAbilitiesUnholyPowerDef : CParentDefClassBase { unsigned char pad0[0x3c]; Vec v0; unsigned char gap0[4]; Vec v1,v2,v3,v4,v5,v6; unsigned char gap1[12]; Vec v7; Vec v8; CSpecialAbilitiesUnholyPowerDef(); };
CSpecialAbilitiesUnholyPowerDef::CSpecialAbilitiesUnholyPowerDef() {
    *(volatile unsigned int*)this = 0x012348e4; volatile unsigned int* z=(volatile unsigned int*)this;
    z[15]=0;z[16]=0;z[17]=0;z[18]=0;z[19]=0;z[20]=0;z[21]=0;z[22]=0;z[23]=0;z[24]=0;z[25]=0;z[26]=0;z[27]=0;z[28]=0;z[29]=0;
    Vec& a=v0; a.first=0;a.last=0;a.end=0; Vec& b=v1; b.first=0;b.last=0;b.end=0;
    Vec& c=v2; c.first=0;c.last=0;c.end=0; Vec& d=v3; d.first=0;d.last=0;d.end=0;
    Vec& e=v4; e.first=0;e.last=0;e.end=0; Vec& f=v5; f.first=0;f.last=0;f.end=0;
    Vec& g=v6; g.first=0;g.last=0;g.end=0; Vec& h=v7; h.first=0;h.last=0;h.end=0;
    Vec& i=v8; i.first=0;i.last=0;i.end=0;
}

CParentDefClassBase::CParentDefClassBase() { for (int i=0;i<0x3c;++i) base[i]=0x5a; }
#include <new>
int main() {
    union Storage { unsigned int align; unsigned char bytes[sizeof(CSpecialAbilitiesUnholyPowerDef)]; } storage;
    for (unsigned int i=0;i<sizeof(storage.bytes);++i) storage.bytes[i]=0xa5;
    CSpecialAbilitiesUnholyPowerDef* value=new (storage.bytes) CSpecialAbilitiesUnholyPowerDef();
    if (*(unsigned int*)value!=0x012348e4) { std::printf("004581f8_TEST FAIL\n"); return 1; }
    for (int i=4;i<0x3c;++i) if (value->base[i]!=0x5a) { std::printf("004581f8_TEST FAIL\n"); return 1; }
    for (int i=0;i<0x3c;++i) if (value->pad0[i]!=0) { std::printf("004581f8_TEST FAIL\n"); return 1; }
    for (int i=0;i<4;++i) if (value->gap0[i]!=0xa5) { std::printf("004581f8_TEST FAIL\n"); return 1; }
    for (int i=0;i<12;++i) if (value->gap1[i]!=0xa5) { std::printf("004581f8_TEST FAIL\n"); return 1; }
    Vec* vectors[9]={&value->v0,&value->v1,&value->v2,&value->v3,&value->v4,&value->v5,&value->v6,&value->v7,&value->v8};
    for (int i=0;i<9;++i) if (vectors[i]->first || vectors[i]->last || vectors[i]->end) { std::printf("004581f8_TEST FAIL\n"); return 1; }
    value->~CSpecialAbilitiesUnholyPowerDef();
    std::printf("004581f8_TEST PASS\n"); return 0;
}