#pragma optimize("s",on)
#include "engine/CHasNameDef.h"

struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferULong(int* value);                 // 0x00431020 Transfer(unsigned long&): PDB-typed ulong members
};

void __fastcall CHasNameDef_Transfer(CHasNameDef* def, void* /*edx*/, CPersistContext* ctx)
{
    ctx->TransferULong((int*)&def->DefaultNameTag);
    ctx->TransferULong((int*)&def->Home);
    ctx->TransferULong((int*)&def->Occupation);
}
