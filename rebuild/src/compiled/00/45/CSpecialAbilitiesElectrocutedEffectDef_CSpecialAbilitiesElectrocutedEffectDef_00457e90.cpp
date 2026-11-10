// CSpecialAbilitiesElectrocutedEffectDef::CSpecialAbilitiesElectrocutedEffectDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CSpecialAbilitiesElectrocutedEffectDef vtable, then the members' constructors
// Leaf: ??0CSpecialAbilitiesElectrocutedEffectDef@@QAE@XZ
// Retail: Fable.exe @ 0x00457E90. Identity: the vtable it stores, 0x01234744, is RTTI .?AVCSpecialAbilitiesElectrocutedEffectDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CSpecialAbilitiesElectrocutedEffectDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CSpecialAbilitiesElectrocutedEffectDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CSpecialAbilitiesElectrocutedEffectDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;

class CSpecialAbilitiesElectrocutedEffectDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CSpecialAbilitiesElectrocutedEffectDef();                                   // 0x00457E90
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    float FadeInTimeSecs;
    float FadeOutTimeStartSecs;
    float FadeOutTimeSecs;
    std::vector<long> Color;
    std::vector<long> HighlightColor;
    float HighlightWidth;
    float HeightSmall;
    float HeightMedium;
};

CSpecialAbilitiesElectrocutedEffectDef::CSpecialAbilitiesElectrocutedEffectDef()
{
}
