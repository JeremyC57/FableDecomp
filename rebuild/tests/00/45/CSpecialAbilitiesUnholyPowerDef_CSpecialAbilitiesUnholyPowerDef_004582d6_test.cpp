#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#endif
extern "C" void __cdecl tracked_free(void*);
#define free tracked_free
// CSpecialAbilitiesUnholyPowerDef::~CSpecialAbilitiesUnholyPowerDef() -- release fourteen definition-owned pointers in reverse field order and tail-call the base destructor; retail @ 0x004582D6, 231 bytes.
// Retail accesses fourteen definition-owned container pointers at the offsets modeled below and tail-calls the base destructor at 0x00430300. The quarantined PDB header places 16-byte vectors at different offsets, so the local retail model follows disassembly instead.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* memory); // 0x00BFEA14
struct CDefinitionBaseModel {
    unsigned char data[0x3c];
    ~CDefinitionBaseModel(); // 0x00430300
};
class CSpecialAbilitiesUnholyPowerDef : public CDefinitionBaseModel { public:
    void* p3c; unsigned char gap40[8];
    void* p48; unsigned char gap4c[8];
    void* p54; unsigned char gap58[8];
    void* p60; unsigned char gap64[8];
    void* p6c; unsigned char gap70[8];
    void* p78; unsigned char gap7c[0xc];
    void* p88; unsigned char gap8c[8];
    void* p94; unsigned char gap98[8];
    void* pA0; unsigned char gapA4[8];
    void* pAC; unsigned char gapB0[8];
    void* pB8; unsigned char gapBC[8];
    void* pC4; unsigned char gapC8[0x14];
    void* pDC; unsigned char gapE0[8];
    void* pE8;
    ~CSpecialAbilitiesUnholyPowerDef();
};
CSpecialAbilitiesUnholyPowerDef::~CSpecialAbilitiesUnholyPowerDef() {
    if (pE8) free(pE8);
    if (pDC) free(pDC);
    if (pC4) free(pC4);
    if (pB8) free(pB8);
    if (pAC) free(pAC);
    if (pA0) free(pA0);
    if (p94) free(p94);
    if (p88) free(p88);
    if (p78) free(p78);
    if (p6c) free(p6c);
    if (p60) free(p60);
    if (p54) free(p54);
    if (p48) free(p48);
    if (p3c) free(p3c);
}

#undef free
static int events[20], count;
static void* blocks[14];
static void event(int n) { events[count++]=n; }
extern "C" void __cdecl tracked_free(void* p) {
    int i; for(i=0;i<14;++i) if(p==blocks[i]) break;
    event(i<14 ? i+1 : 99); std::free(p);
}
CDefinitionBaseModel::~CDefinitionBaseModel() { event(15); }
static void* block() { return std::malloc(16); }
static int ordered(const int* expected,int n) { int i; if(count!=n) return 0; for(i=0;i<n;++i) if(events[i]!=expected[i]) return 0; return 1; }
int main() {
    union Storage { void* align; char bytes[sizeof(CSpecialAbilitiesUnholyPowerDef)]; } storage;
    CSpecialAbilitiesUnholyPowerDef* p=new(storage.bytes) CSpecialAbilitiesUnholyPowerDef;
    blocks[0]=p->p3c=block();
    blocks[1]=p->p48=block();
    blocks[2]=p->p54=block();
    blocks[3]=p->p60=block();
    blocks[4]=p->p6c=block();
    blocks[5]=p->p78=block();
    blocks[6]=p->p88=block();
    blocks[7]=p->p94=block();
    blocks[8]=p->pA0=block();
    blocks[9]=p->pAC=block();
    blocks[10]=p->pB8=block();
    blocks[11]=p->pC4=block();
    blocks[12]=p->pDC=block();
    blocks[13]=p->pE8=block();
    for(int i=0;i<14;++i) if(!blocks[i]) { std::printf("004582d6_TEST FAIL\n"); return 1; }
    p->~CSpecialAbilitiesUnholyPowerDef();
    { const int expected[15]={14,13,12,11,10,9,8,7,6,5,4,3,2,1,15}; if(!ordered(expected,15)) { std::printf("004582d6_TEST FAIL\n"); return 1; } }
    count=0; union Storage emptyStorage;
    for(unsigned int j=0;j<sizeof(emptyStorage.bytes);++j) emptyStorage.bytes[j]=0;
    CSpecialAbilitiesUnholyPowerDef* empty=new(emptyStorage.bytes) CSpecialAbilitiesUnholyPowerDef();
    empty->~CSpecialAbilitiesUnholyPowerDef();
    { const int expected[1]={15}; if(!ordered(expected,1)) { std::printf("004582d6_TEST FAIL\n"); return 1; } }
    std::printf("004582d6_TEST PASS\n"); return 0;
}