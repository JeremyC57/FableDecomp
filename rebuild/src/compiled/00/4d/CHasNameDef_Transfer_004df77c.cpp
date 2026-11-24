// Leaf: ?Transfer@CHasNameDef@@UAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_CHASNAMEDEF_RTTI_DECLS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CHASNAMEDEF
#pragma optimize("s",on)
#include "engine/CHasNameDef.h"

class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                 // 0x00431020 Transfer(unsigned long&): PDB-typed ulong members
};

void CHasNameDef::Transfer(class CPersistContext& ctx)
{
    CHasNameDef* def = this;
    ctx.Transfer(*(unsigned long*)((int*)&def->DefaultNameTag));
    ctx.Transfer(*(unsigned long*)((int*)&def->Home));
    ctx.Transfer(*(unsigned long*)((int*)&def->Occupation));
}
// Flatten route (flatten_pilot v4, NF324): CHasNameDef_Transfer -> CHasNameDef::Transfer, the RTTI-placed PDB member; class model: engine/CHasNameDef.h opt-in FABLE_RETAIL_CHASNAMEDEF, FABLE_RETAIL_CHASNAMEDEF_RTTI_DECLS (preamble of CTCHasName_GetNPCShortName_007f4990.cpp).
