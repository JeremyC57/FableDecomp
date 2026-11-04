// CArenaWaveDef::GetSizeofClass() -- slot 20: sizeof(CArenaWaveDef) = 0x3c
// Leaf: ?GetSizeofClass@CArenaWaveDef@@UAEKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F25CA0, 6 bytes: b83c000000c3.
// Identity (naming wave 35, family def): Anniversary wave 2026-11-04 (high): vtable 0x012FB2F0 slot 20; Anniversary RTTI CArenaWaveDef (string-literal match, validated 257/258); PDB name set has the GetSizeofClass member of CArenaWaveDef; slot layout from named TLC siblings; function referenced by this vtable only; body=mov eax,imm;ret.
//   ?GetSizeofClass@CArenaWaveDef@@UAEKXZ in FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB also: ??_ECArenaWaveDef@@UAEPAXI@Z, ??1CArenaWaveDef@@UAE@XZ.
// Was: Constant-return member: returns 0x3c. __fastcall this=ecx. (catalog stand-in _gapscan).
// Class: the class model of the landed operator= row 0x00F25F60 (rebuild/src/compiled/00/f2/CArenaWaveDef_operator_00f25f60.cpp); only this member is graded.

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CArenaCreatureDefVector {                                          // retail std::vector<CArenaCreatureDef> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CArenaCreatureDefVector& operator=(const CArenaCreatureDefVector& other);   // 0x00F25F90
};

// def vtable 0x012FB2F0: slot 0 0x00F25CB0 (implicit inline dtor); real overrides from the def vtable scan
class CArenaWaveDef : public CDefClassBase { public:                                    // vtable 0x012FB2F0 (no RTTI)
    long                    NumWaveCreatures;                             // +0x28
    CArenaCreatureDefVector Creatures;                                    // +0x2c
    bool                    ShortWave;                                    // +0x38
    virtual void v00();
    CArenaWaveDef& operator=(const CArenaWaveDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00F261F0
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00F25F50
    virtual unsigned long GetSizeofClass();                  // slot 20 0x00F25CA0
};

CArenaWaveDef& CArenaWaveDef::operator=(const CArenaWaveDef& other)
{
    CDefClassBase::operator=(other);
    NumWaveCreatures = other.NumWaveCreatures;
    Creatures = other.Creatures;
    ShortWave = other.ShortWave;
    return *this;
}
unsigned long CArenaWaveDef::GetSizeofClass()
{
    return sizeof(CArenaWaveDef);
}
