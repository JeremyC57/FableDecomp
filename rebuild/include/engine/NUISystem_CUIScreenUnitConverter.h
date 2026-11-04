#ifndef FABLE_ENGINE_NUISYSTEM_CUISCREENUNITCONVERTER_H
#define FABLE_ENGINE_NUISYSTEM_CUISCREENUNITCONVERTER_H
// NUISystem::CUIScreenUnitConverter (chunk 735, row 0x00577AC0): FableWin.pdb ?GetBackBufferWidth@CUIScreenUnitConverter@
// NUISystem@@SIMXZ and ?GetBackBufferHeight@CUIScreenUnitConverter@NUISystem@@SIMXZ (static __fastcall, float, no
// arguments). Retail 0x0041CC14 / 0x0041CC2B are the two 23-byte static float getters (a byte flag, then one of two float
// globals) next to CManager::GetUIScale 0x0041CF47; the row scales the first with GetUIScale().X (centre x) and the second
// with GetUIScale().Y (centre y). Both landed under the propagated label CTCLook::GetHeadAngularVelocityMaxXY.
namespace NUISystem {
class CUIScreenUnitConverter {
public:
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
    // Chunk 749 (rows 0x0054EF00 CText::Draw / 0x005502E0 CMesh::Draw): the two getters are inline header functions -- the
    // Draw rows expand them in place (`cmp byte [13B8768h],0 ; je ; fld [13B876Ch] / fld [1375CD4h]`), the out-of-line
    // COMDAT copies are 0x0041CC14 / 0x0041CC2B. Data symbols: the established spellings of the landed rows 0x004299A8 /
    // 0x0052F5C0 (no PDB data-symbol dump): IsUsingRelativeCoords 0x013B8768, BackBufferWidth / Height 0x013B876C /
    // 0x013B8770, ScreenWidth / Height (640 / 480) 0x01375CD4 / 0x01375CD8.
    static bool  IsUsingRelativeCoords;                // 0x013B8768
    static float BackBufferWidth;                      // 0x013B876C
    static float BackBufferHeight;                     // 0x013B8770
    static float ScreenWidth;                          // 0x01375CD4
    static float ScreenHeight;                         // 0x01375CD8
    static float __fastcall GetBackBufferWidth() { if (IsUsingRelativeCoords) return BackBufferWidth; return ScreenWidth; }
    static float __fastcall GetBackBufferHeight() { if (IsUsingRelativeCoords) return BackBufferHeight; return ScreenHeight; }
#else
    static float __fastcall GetBackBufferWidth();      // 0x0041CC14
    static float __fastcall GetBackBufferHeight();     // 0x0041CC2B
#endif
};
}
#endif
