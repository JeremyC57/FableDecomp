#pragma optimize("s",on)
#include "engine/CHasNameDef.h"

class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                 // 0x00431020 Transfer(unsigned long&): PDB-typed ulong members
};

void __fastcall CHasNameDef_Transfer(CHasNameDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->Transfer(*(unsigned long*)((int*)&def->DefaultNameTag));
    ctx->Transfer(*(unsigned long*)((int*)&def->Home));
    ctx->Transfer(*(unsigned long*)((int*)&def->Occupation));
}
