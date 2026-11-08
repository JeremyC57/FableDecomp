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
namespace NPlayerGui {
// EHighlightingCursorType: Ego_r.pdb names the type (the CursorType member) but no PDB on disk lists its enumerators; the values are
// the row's (0 none, 1 use, 2 attack, 3 talk, 4 talk-with-interest). PLACEHOLDER spellings, flagged for the naming queue.
enum EHighlightingCursorType {
    HIGHLIGHTING_CURSOR_NONE = 0,
    HIGHLIGHTING_CURSOR_USE = 1,
    HIGHLIGHTING_CURSOR_ATTACK = 2,
    HIGHLIGHTING_CURSOR_TALK = 3,
    HIGHLIGHTING_CURSOR_INTEREST = 4
};
class CDrawHighlightingTargetingCursor : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x0064BFBA ?Draw@CDrawHighlightingTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
private:
    CIntelligentPointer<CThing> PTargetedThing;                          // +0x28
    CRGBColour                  CurrentColour;                           // +0x30
    float                       CurrentWidthScreenSpace;                 // +0x34
    float                       CurrentWidthWorldSpace;                  // +0x38
    long                        CurrentFadeStep;                         // +0x3c
    long                        Width;                                   // +0x40
    EHighlightingCursorType     CursorType;                              // +0x44
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawHighlightingTargetingCursor) == 0x48);
#endif
