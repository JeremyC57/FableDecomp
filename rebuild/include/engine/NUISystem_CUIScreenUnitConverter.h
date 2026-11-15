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
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_RESETDIMENSIONS
    // Opt-in (chunk 2051, row 0x0042EC7C CNewFrontendGameComponent::Run): Ego_r.pdb ?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ, retail 0x0040F0E0 (landed). Declaration only.
    static void __fastcall ResetDimensions();                // 0x0040F0E0
#endif
#if defined(FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS)
    // Both original complete types, exact private PUBLICs and GDATA32 declarations.
private:
    static bool IsUsingRelativeCoords;
    static float BackBufferWidth, BackBufferHeight;
    static float DefaultBackBufferWidth, DefaultBackBufferHeight;
public:
    static float __fastcall GetBackBufferWidth() { if (IsUsingRelativeCoords) return BackBufferWidth; return DefaultBackBufferWidth; }
    static float __fastcall GetBackBufferHeight() { if (IsUsingRelativeCoords) return BackBufferHeight; return DefaultBackBufferHeight; }
#elif defined(FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE)
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
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_CONVERTXY
    // Retail opt-in (agentC43, row 0x005379F5 NUISystem::CList::ComputeChildrenVisibility): FableWin.pdb ?ConvertX@CUIScreenUnitConverter@NUISystem@@SIMM@Z /
    // ?ConvertY@CUIScreenUnitConverter@NUISystem@@SIMM@Z, out of line at 0x0052E470 / 0x0052E4D0 (static __fastcall, one float on the
    // stack; x / ScreenWidth * BackBufferWidth when IsUsingRelativeCoords). Declarations only: the row TU defines them ahead of the row
    // (same TU in retail), needs FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE for the static data members.
    static float __fastcall ConvertX(float x);         // 0x0052E470
    static float __fastcall ConvertY(float y);         // 0x0052E4D0
#endif
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_CONVERTXY_INLINE
    // Retail opt-in (wfB6, row 0x005379F5 NUISystem::CList::ComputeChildrenVisibility; use INSTEAD of
    // FABLE_RETAIL_CUISCREENUNITCONVERTER_CONVERTXY, which only declares them): FableWin.pdb ?ConvertX@CUIScreenUnitConverter@NUISystem@@SIMM@Z /
    // ?ConvertY@CUIScreenUnitConverter@NUISystem@@SIMM@Z as inline header functions like GetBackBufferWidth / ConvertVector above
    // (out-of-line copies 0x0052E470 / 0x0052E4D0). The row calls them (not expanded under optimize("s")) but VC7.1 has read the
    // bodies: it knows they write no memory, so the def fetches of the next argument are loaded after the call (0x00537B79..0x00537BDB).
    // With declarations alone the row is 18 bytes longer. Needs FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE (static data).
    // Bodies: the real texts -- the landed rows' bodies (named quotient, then the multiply: `fld x; fdiv ScreenWidth; fmul
    // BackBufferWidth`); compiled out of line (default flags, probe address-of) they grade RELOCATION_MATCH against 0x0052E470 /
    // 0x0052E4D0 (wfB6 fix-up 2026-11-14; the earlier one-expression text `x / ScreenWidth * BackBufferWidth` was reassociated by
    // VC7.1 into `fld BackBufferWidth; fdiv ScreenWidth; fmul x`, a different result: DIFFER(35v35)).
    static float __fastcall ConvertX(float x)     // 0x0052E470 (out-of-line copy; landed body global_PhysicsScaleFloatByGameSpeed_0052e470.cpp)
    {
        if (IsUsingRelativeCoords)
        {
            float t = x / ScreenWidth;
            return t * BackBufferWidth;
        }
        return x;
    }
    static float __fastcall ConvertY(float y)     // 0x0052E4D0 (out-of-line copy; landed body global_LengthScaleConversion_0052e4d0.cpp)
    {
        if (IsUsingRelativeCoords)
        {
            float t = y / ScreenHeight;
            return t * BackBufferHeight;
        }
        return y;
    }
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
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE_CONVERTXY
    // Nested opt-in (agentC28, row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC): the same inline ConvertVector written through
    // the class's own per-axis converters, Ego_r.pdb / FableWin.pdb ?ConvertX@CUIScreenUnitConverter@NUISystem@@SIMM@Z and ?ConvertY@..@@SIMM@Z
    // (Ego_r 0x0055ECC6 / 0x0055ECF0, FableWin 0x01E1E0F6 / 0x01E1E12E; static __fastcall float(float)). Evidence: (1) FableWin's debug
    // ConvertVector is 28 instructions / 81 bytes: this text is 27i / 77b at plain /Od, the direct-global text of the #else branch 29i / 95b
    // (four absolute float operands FableWin does not have); (2) the out-of-line copy 0x0052E580 compiles to the same retail bytes from
    // this text; (3) the row: with this text 0x0061E6D0 is RELOCATION_MATCH, with the #else text its last residue is the SIB operand
    // order at 0x0061FB58 / 0x0061FB5E (c2 symbol-record ids: the two expanded per-axis helpers add folded parameter records with no byte).
    // ConvertX / ConvertY bodies: Inferred (FableWin 15i / 45b each; this text 13i / 42b at /Od -- the flag test and the two-step divide /
    // multiply are what the retail copy 0x0052E580 shows; the exact debug text is not pinned).
    static float __fastcall ConvertX(float x) { if (IsUsingRelativeCoords) { x /= ScreenWidth; x *= BackBufferWidth; } return x; }
    static float __fastcall ConvertY(float y) { if (IsUsingRelativeCoords) { y /= ScreenHeight; y *= BackBufferHeight; } return y; }
    static C2DVector __fastcall ConvertVector(C2DVector v)         // 0x0052E580 (out-of-line copy)
    {
        C2DVector result = v;
        if (IsUsingRelativeCoords) {
            result.X = ConvertX(v.X);
            result.Y = ConvertY(v.Y);
        }
        return result;
    }
#else
    static C2DVector __fastcall ConvertVector(C2DVector v)         // 0x0052E580 (out-of-line copy)
    {
        C2DVector result = v;
        if (IsUsingRelativeCoords) {
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
            result.X = v.X / DefaultBackBufferWidth;
#else
            result.X = v.X / ScreenWidth;
#endif
            result.X *= BackBufferWidth;
#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
            result.Y = v.Y / DefaultBackBufferHeight;
#else
            result.Y = v.Y / ScreenHeight;
#endif
            result.Y *= BackBufferHeight;
        }
        return result;
    }
#endif
#else
    static C2DVector __fastcall ConvertVector(C2DVector v);        // 0x0052E580
#endif
    static C2DVector __fastcall UnConvertVector(C2DVector v);      // 0x0052E530
#endif
};
}
#endif
