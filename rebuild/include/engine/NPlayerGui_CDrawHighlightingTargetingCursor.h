#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWHIGHLIGHTINGTARGETINGCURSOR_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWHIGHLIGHTINGTARGETINGCURSOR_H
// NPlayerGui::CDrawHighlightingTargetingCursor (new, chunk 867, row 0x0064BFBA Draw): the player-GUI illustrator that glows the
// targeted thing's mesh (CTCGraphicAppearance::SetAsHighlighted) in the colour / width of the cursor type the target deserves, fading
// from the gui def's start values to its end values over HighlightFadeSteps frames, and draws the two targeting-border sprites while
// the hero Z-targets an attackable thing.
// RTTI .?AVCDrawHighlightingTargetingCursor@NPlayerGui@@ : CDrawBase (ghidra_out/rtti_bases.tsv). Ego_r.pdb members
// (struct_layouts_egor.tsv, 76 bytes there with the 12-byte CIntelligentPointer and 16-byte vector; retail 0x48): PTargetedThing +0x28,
// CurrentColour +0x30, CurrentWidthScreenSpace +0x34, CurrentWidthWorldSpace +0x38, CurrentFadeStep +0x3C, Width +0x40, CursorType +0x44
// (the row reads / writes every one of them at those offsets off `this`).
// Needs FABLE_RETAIL_CINTELLIGENTPOINTER (the 8-byte retail pointer).
#include "rebuild_abi.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRGBColour.h"
class CThing; class CEnginePrimitivePrepareInfo;
#ifdef FABLE_RETAIL_NPLAYERGUI_HIGHLIGHT_ORIGINAL_NAMES
// Retail opt-in (Codex pass22): both complete TPI enum records, actual Draw stores and table indices.
enum EHighlightingCursorType { CURSOR_TYPE_NONE=0, CURSOR_TYPE_USABLE=1, CURSOR_TYPE_ENEMY=2, CURSOR_TYPE_ALLY=3, CURSOR_TYPE_HAS_INFORMATION=4, MAX_NO_CURSOR_TYPES=5 };
#endif
namespace NPlayerGui {
// EHighlightingCursorType: Ego_r.pdb names the type (the CursorType member) but no PDB on disk lists its enumerators; the values are
// the row's (0 none, 1 use, 2 attack, 3 talk, 4 talk-with-interest). PLACEHOLDER spellings, flagged for the naming queue.
 #ifndef FABLE_RETAIL_NPLAYERGUI_HIGHLIGHT_ORIGINAL_NAMES
enum EHighlightingCursorType {
    HIGHLIGHTING_CURSOR_NONE = 0,
    HIGHLIGHTING_CURSOR_USE = 1,
    HIGHLIGHTING_CURSOR_ATTACK = 2,
    HIGHLIGHTING_CURSOR_TALK = 3,
    HIGHLIGHTING_CURSOR_INTEREST = 4
};
#endif
#ifdef FABLE_RETAIL_NPLAYERGUI_CURSOR_ORIGINAL_DECLARATIONS_PASS22
class CDrawHighlightingTargetingCursor : public CDrawBase {
public:
    CDrawHighlightingTargetingCursor(CPlayerGui&,std::vector<CDrawBase*>&);
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x0064BFBA ?Draw@CDrawHighlightingTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
    void Clear();
    CThing* GetPTargetedThing() const;
private:
    CIntelligentPointer<CThing> PTargetedThing;                          // +0x28
    CRGBColour                  CurrentColour;                           // +0x30
    float                       CurrentWidthScreenSpace;                 // +0x34
    float                       CurrentWidthWorldSpace;                  // +0x38
    long                        CurrentFadeStep;                         // +0x3c
    long                        Width;                                   // +0x40
    EHighlightingCursorType     CursorType;                              // +0x44
};
#else
class CDrawHighlightingTargetingCursor : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x0064BFBA ?Draw@CDrawHighlightingTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
#ifdef FABLE_RETAIL_NPLAYERGUI_CDRAWHIGHLIGHTINGTARGETINGCURSOR_CLEAR
    // Retail opt-in (wfA2, row 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine): Ego_r.pdb / FableWin.pdb
    // ?Clear@CDrawHighlightingTargetingCursor@NPlayerGui@@QAEXXZ (out of line in retail at 0x00646709) and FableWin.pdb
    // ?GetPTargetedThing@CDrawHighlightingTargetingCursor@NPlayerGui@@QBEPAVCThing@@XZ, inline in retail (the caller reads PTargetedThing
    // +0x28 in place: `lea ecx,[eax+28h] ; call CBaseIntelligentPointer::GetPItem`, 0x00439803). No layout change.
    void Clear();                                                        // 0x00646709
    CThing* GetPTargetedThing() const { return PTargetedThing.Get(); }
#endif
private:
    CIntelligentPointer<CThing> PTargetedThing;                          // +0x28
    CRGBColour                  CurrentColour;                           // +0x30
    float                       CurrentWidthScreenSpace;                 // +0x34
    float                       CurrentWidthWorldSpace;                  // +0x38
    long                        CurrentFadeStep;                         // +0x3c
    long                        Width;                                   // +0x40
    EHighlightingCursorType     CursorType;                              // +0x44
};
#endif

}
FABLE_STATIC_ASSERT_AT(40, sizeof(NPlayerGui::CDrawHighlightingTargetingCursor) == 0x48);
#endif
