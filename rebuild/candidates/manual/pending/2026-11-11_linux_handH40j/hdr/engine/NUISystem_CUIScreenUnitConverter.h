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
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR
    // Retail opt-in (chunk 840, row 0x0052EA30 CComponent::ChangePositionDelta): the two out-of-line vector converters, static
    // __fastcall with a by-value C2DVector argument (user copy ctor, built in its stack slot) and a hidden C2DVector result in ecx.
    // ConvertVector 0x0052E580 (Ego_r.pdb / FableWin.pdb ?ConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z,
    // bound by the landed 0x0052EEC0): divides by the 640x480 screen size and multiplies by the back-buffer size (20 callers).
    // UnConvertVector 0x0052E530 (FableWin.pdb ?UnConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z): the
    // inverse (divides by the back-buffer size, multiplies by the screen size); callers 0x0052EA6B / 0x0052EB2B
    // (ChangePositionDelta / ChangeTargetPositionDelta). The row includes engine/C2DVector.h before this header.
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE
    // Nested opt-in (chunk 882, row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC): ConvertVector is an inline header function
    // in retail (0x0052E580 is its out-of-line copy, the landed body): the row expands it at its first site (0x0061EA22..0x0061EA8F:
    // the flag test, then x / 640 * back-buffer width and y / 480 * back-buffer height) and calls 0x0052E580 at its second
    // (0x00620F0A) once the inline budget is spent. The landed 0x0052E580 body; needs FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE.
    static C2DVector __fastcall ConvertVector(C2DVector v)         // 0x0052E580 (out-of-line copy)
    {
        C2DVector result = v;
        if (IsUsingRelativeCoords) {
            result.X = v.X / ScreenWidth;
            result.X *= BackBufferWidth;
            result.Y = v.Y / ScreenHeight;
            result.Y *= BackBufferHeight;
        }
        return result;
    }
#else
    static C2DVector __fastcall ConvertVector(C2DVector v);        // 0x0052E580
#endif
    static C2DVector __fastcall UnConvertVector(C2DVector v);      // 0x0052E530
#endif
};
}
#endif
