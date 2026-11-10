// COpinionReactionMaskDef::COpinionReactionMaskDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the COpinionReactionMaskDef vtable, then the members' constructors
// Leaf: ??0COpinionReactionMaskDef@@QAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x00456CE1. Identity: the vtable it stores, 0x01233D84, is RTTI .?AVCOpinionReactionMaskDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0COpinionReactionMaskDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb COpinionReactionMaskDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models COpinionReactionMaskDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CPersistContext;
enum EOpinionReactionType { };                                   // enumerators not needed here (no engine header)

class COpinionReactionMaskDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    COpinionReactionMaskDef();                                   // 0x00456CE1
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::map<EOpinionReactionType, bool> ReactionEnabled;
};

COpinionReactionMaskDef::COpinionReactionMaskDef()
{
}
