// CCreatureGenerationFamilyDef::~CCreatureGenerationFamilyDef() -- the compiler-generated destructor: the members' destructors in reverse order (STLport vectors free their storage), then the out-of-line ~CParentDefClassBase 0x00430300 as the tail jump; no vtable store
// Leaf: ??1CCreatureGenerationFamilyDef@@UAE@XZ
#pragma optimize("s",on)
// Retail: Fable.exe @ 0x00456010. Identity: its only caller is the scalar deleting destructor in slot 0 of vtable 0x0123390C,
//   RTTI .?AVCCreatureGenerationFamilyDef@@ : CParentDefClassBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??1CCreatureGenerationFamilyDef@@UAE@XZ.
// Callees: free 0x00BFEA14 (the STLport malloc allocator), CParentDefClassBase::~CParentDefClassBase 0x00430300 (Ego_r.pdb / FableWin.pdb ??1CParentDefClassBase@@UAE@XZ), the shared
//   engine/CParentDefClassBase.h retail model.
// Layout: Ego_r.pdb CCreatureGenerationFamilyDef members, at the retail offsets (the retail CParentDefClassBase is 0x39 bytes and STLport containers
//   are smaller than the Ego_r Dinkumware ones). No engine header models CCreatureGenerationFamilyDef, so it is declared here.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "stlport/fable_containers.h"
class CPersistContext;
enum ECreatureGeneratorGenerateType { };                                   // enumerators not needed here (no engine header)

class CCreatureGenerationFamilyDef : public CParentDefClassBase {
public:
    static CDefClassBase* FABLE_FASTCALL Alloc();
    CCreatureGenerationFamilyDef();                                   // 0x00456010
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<long> Creatures;
    long DifficultyLevel;
    long GeneratorObject;
    std::vector<ECreatureGeneratorGenerateType> GenerationTypes;
};

// Emission harness (not a retail function): the class declares no destructor (retail's stores no vtable); a qualified
// call makes VC7.1 emit the implicit destructor's COMDAT, as the deleting destructor's call does in retail.
void DestroyCCreatureGenerationFamilyDef(CCreatureGenerationFamilyDef* def)
{
    def->CCreatureGenerationFamilyDef::~CCreatureGenerationFamilyDef();
}
