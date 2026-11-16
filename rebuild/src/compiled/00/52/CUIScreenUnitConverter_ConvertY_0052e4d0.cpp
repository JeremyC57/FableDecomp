// NUISystem::CUIScreenUnitConverter::ConvertY (retail 0x0052E4D0): FableWin.pdb / Ego_r.pdb
// ?ConvertY@CUIScreenUnitConverter@NUISystem@@SIMM@Z (static __fastcall, one float on the stack, `ret 4`); the landed caller
// 0x005379F5 binds this symbol. Static data through the shared header: IsUsingRelativeCoords 0x013B8768, DefaultBackBufferHeight 0x01375CD8,
// BackBufferHeight 0x013B8770: the private statics both PDBs give (FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS; Codex pass 22/23 two-PDB GDATA evidence, landed 0x0054EF00).
// Leaf: ?ConvertY@CUIScreenUnitConverter@NUISystem@@SIMM@Z
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_CONVERTXY
#include "engine/NUISystem_CUIScreenUnitConverter.h"

float __fastcall NUISystem::CUIScreenUnitConverter::ConvertY(float y)
{
    if (IsUsingRelativeCoords)
    {
        float t = y / DefaultBackBufferHeight;
        return t * BackBufferHeight;
    }
    return y;
}
