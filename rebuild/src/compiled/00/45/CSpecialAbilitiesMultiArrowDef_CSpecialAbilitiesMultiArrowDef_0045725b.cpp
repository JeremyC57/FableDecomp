// CSpecialAbilitiesMultiArrowDef::CSpecialAbilitiesMultiArrowDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CSpecialAbilitiesMultiArrowDef vtable, then the members' constructors
// Leaf: ??0CSpecialAbilitiesMultiArrowDef@@QAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x0045725B. Identity: the vtable it stores, 0x01234194, is RTTI .?AVCSpecialAbilitiesMultiArrowDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CSpecialAbilitiesMultiArrowDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CSpecialAbilitiesMultiArrowDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CSpecialAbilitiesMultiArrowDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDefString.h"
class CPersistContext;

class CSpecialAbilitiesMultiArrowDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CSpecialAbilitiesMultiArrowDef();                                   // 0x0045725B
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    long NumberOfShots[4];
    long NumberInVolley[4];
    float AmmoEffectCreationDelaySecs;
    CDefString CastSound;
    float BowHighlightWidth;
    std::vector<long> BowHighlightColor;
    std::vector<float> MultiArrowPerArrowDamageMultiplier;
};

CSpecialAbilitiesMultiArrowDef::CSpecialAbilitiesMultiArrowDef()
{
}
