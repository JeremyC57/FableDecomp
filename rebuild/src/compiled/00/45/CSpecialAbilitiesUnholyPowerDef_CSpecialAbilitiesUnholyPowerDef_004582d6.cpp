// CSpecialAbilitiesUnholyPowerDef::~CSpecialAbilitiesUnholyPowerDef() -- release fourteen definition-owned pointers in reverse field order and tail-call the base destructor; retail @ 0x004582D6, 231 bytes.
// Retail accesses fourteen definition-owned container pointers at the offsets modeled below and tail-calls the base destructor at 0x00430300. The quarantined PDB header places 16-byte vectors at different offsets, so the local retail model follows disassembly instead.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* memory); // 0x00BFEA14
struct CDefinitionBaseModel {
    unsigned char data[0x3c];
    ~CDefinitionBaseModel(); // 0x00430300
};
struct CSpecialAbilitiesUnholyPowerDef : CDefinitionBaseModel {
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