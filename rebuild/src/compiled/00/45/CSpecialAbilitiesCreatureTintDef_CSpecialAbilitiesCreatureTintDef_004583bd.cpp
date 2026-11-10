// CSpecialAbilitiesCreatureTintDef::CSpecialAbilitiesCreatureTintDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CSpecialAbilitiesCreatureTintDef vtable, then the members' constructors
// Leaf: ??0CSpecialAbilitiesCreatureTintDef@@QAE@XZ
// Retail: Fable.exe @ 0x004583BD. Identity: the vtable it stores, 0x0123494C, is RTTI .?AVCSpecialAbilitiesCreatureTintDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CSpecialAbilitiesCreatureTintDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CSpecialAbilitiesCreatureTintDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CSpecialAbilitiesCreatureTintDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;

class CSpecialAbilitiesCreatureTintDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CSpecialAbilitiesCreatureTintDef();                                   // 0x004583BD
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    float FadeInTimeSecs;
    float FadeOutTimeSecs;
    std::vector<long> Color;
};

CSpecialAbilitiesCreatureTintDef::CSpecialAbilitiesCreatureTintDef()
{
}
