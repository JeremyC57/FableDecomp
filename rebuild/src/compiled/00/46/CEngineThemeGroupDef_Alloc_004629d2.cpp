// CEngineThemeGroupDef::Alloc() -- the def factory: new CEngineThemeGroupDef (operator new 0x00BFEA1A), the inline default constructor (the out-of-line CParentDefClassBase() 0x00430370, then the CEngineThemeGroupDef vtable) when the block is not null
// Leaf: ?Alloc@CEngineThemeGroupDef@@SIPAVCDefClassBase@@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x004629D2. Identity: the vtable it stores, 0x0123512C, is RTTI .?AVCEngineThemeGroupDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb ?Alloc@CEngineThemeGroupDef@@SIPAVCDefClassBase@@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CEngineThemeGroupDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CEngineThemeGroupDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CPersistContext;

class CEngineThemeGroupDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();                  // 0x004629D2
    CEngineThemeGroupDef();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    bool Dummy;
};

inline CEngineThemeGroupDef::CEngineThemeGroupDef()
{
}

CDefClassBase* FABLE_FASTCALL CEngineThemeGroupDef::Alloc()
{
    return new CEngineThemeGroupDef;
}
