// CCreatureGenerationFamilyDef::CCreatureGenerationFamilyDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CCreatureGenerationFamilyDef vtable, then the members' constructors
// Leaf: ??0CCreatureGenerationFamilyDef@@QAE@XZ
// Retail: Fable.exe @ 0x00455FCA. Identity: the vtable it stores, 0x0123390C, is RTTI .?AVCCreatureGenerationFamilyDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CCreatureGenerationFamilyDef@@QAE@XZ.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ), the shared
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
    CCreatureGenerationFamilyDef();                                   // 0x00455FCA
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    std::vector<long> Creatures;
    long DifficultyLevel;
    long GeneratorObject;
    std::vector<ECreatureGeneratorGenerateType> GenerationTypes;
};

CCreatureGenerationFamilyDef::CCreatureGenerationFamilyDef()
{
}
