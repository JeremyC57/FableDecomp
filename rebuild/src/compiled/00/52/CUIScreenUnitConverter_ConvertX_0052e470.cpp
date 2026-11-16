// NUISystem::CUIScreenUnitConverter::ConvertX (retail 0x0052E470): FableWin.pdb / Ego_r.pdb
// ?ConvertX@CUIScreenUnitConverter@NUISystem@@SIMM@Z (static __fastcall, one float on the stack, `ret 4`); the landed caller
// 0x005379F5 binds this symbol. Static data through the shared header: IsUsingRelativeCoords 0x013B8768, DefaultBackBufferWidth 0x01375CD4,
// BackBufferWidth 0x013B876C: the private statics both PDBs give (FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS; Codex pass 22/23 two-PDB GDATA evidence, landed 0x0054EF00).
// Leaf: ?ConvertX@CUIScreenUnitConverter@NUISystem@@SIMM@Z
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_CONVERTXY
#include "engine/NUISystem_CUIScreenUnitConverter.h"

float __fastcall NUISystem::CUIScreenUnitConverter::ConvertX(float x)
{
    if (IsUsingRelativeCoords)
    {
        float t = x / DefaultBackBufferWidth;
        return t * BackBufferWidth;
    }
    return x;
}
