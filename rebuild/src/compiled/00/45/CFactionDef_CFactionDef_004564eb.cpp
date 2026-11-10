// CFactionDef::CFactionDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CFactionDef vtable, then the members' constructors
// Leaf: ??0CFactionDef@@QAE@XZ
// Retail: Fable.exe @ 0x004564EB. Identity: the vtable it stores, 0x01233B14, is RTTI .?AVCFactionDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CFactionDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CFactionDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CFactionDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CPersistContext;

class CFactionDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CFactionDef();                                   // 0x004564EB
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<CCharString> EnemyFactions;
    std::vector<CCharString> AlliedFactions;
};

CFactionDef::CFactionDef()
{
}
