// CEngineLocalDetailGeneratorDef::CEngineLocalDetailGeneratorDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CEngineLocalDetailGeneratorDef vtable, then the members' constructors
// Leaf: ??0CEngineLocalDetailGeneratorDef@@QAE@XZ
// Retail: Fable.exe @ 0x00455C8A. Identity: the vtable it stores, 0x01233674, is RTTI .?AVCEngineLocalDetailGeneratorDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CEngineLocalDetailGeneratorDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CEngineLocalDetailGeneratorDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CEngineLocalDetailGeneratorDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
#include "engine/CEngineLocalDetailLayerDef.h"
class CPersistContext;

class CEngineLocalDetailGeneratorDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CEngineLocalDetailGeneratorDef();                                   // 0x00455C8A
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<CEngineLocalDetailLayerDef> Layers;
};

CEngineLocalDetailGeneratorDef::CEngineLocalDetailGeneratorDef()
{
}
