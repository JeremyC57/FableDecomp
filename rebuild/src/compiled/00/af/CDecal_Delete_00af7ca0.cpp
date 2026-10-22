
#include "engine/CDecal.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall CDecal_Dtor(CDecal* decal);
void __fastcall CDecal_OpDelete(CDecal* decal);

void __fastcall CDecal_Delete(CDecal* decal)
{
    if (decal)
    {
        CDecal_Dtor(decal);
        CDecal_OpDelete(decal);
    }
}