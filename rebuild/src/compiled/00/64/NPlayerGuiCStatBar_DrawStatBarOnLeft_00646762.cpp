// NPlayerGui::CStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& graphic_bank, const C2DVector& bar_pos, const C2DVector& text_offset, const CRGBColour& colour, unsigned char alpha, bool mirror) -- place the bar at its faded-in position (floored, anchored bottom-left), pick the effect (the gold gain / loss arrow from the first change text, or the stat-update effect for the renown / morality graphics once fully faded in, none while faded out), build the value text (signed for morality and gains), draw the stat box, draw the value text (mirrored to the right edge when asked) and the pending change texts interpolated between their offsets
// Leaf: ?DrawStatBarOnLeft@CStatBar@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCGraphicDataBank@@ABVC2DVector@@2ABVCRGBColour@@E_N@Z
// Flags: none
// Retail-asm: 0x00646798 `fsub` + 0x0064679B `fistp dword ptr [ebp-4]` -- the engine's inline-assembly floor helper GFFloatToLongFloor
//   (engine/GFFloatToLongFloor.h; its half / one constants in the frame at [ebp-0Ch] / [ebp-8], 0x0064677D / 0x00646792). This row
//   carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00646762, 740 bytes, `ret 1Ch` (size-optimised NPlayerGui TU, ebp frame 0x40).
// Identity: Ego_r.pdb / Ego_d.pdb ?DrawStatBarOnLeft@CStatBar@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCGraphicDataBank@@ABVC2DVector@@2ABVCRGBColour@@E_N@Z
//   (virtual; the landed CCrimeStatBar override is 0x00646010). Parameters and locals exactly as Ego_d.pdb (debug length 1541): info,
//   graphic_bank, bar_pos, text_offset, colour, alpha, mirror; effect_index, vtext_pos, value_text, tl_pos, text_pos; { change_text };
//   { interpolate { it { change_text_offset, v_text_pos, change_text } } }. No `start` local: the fade-in lerp is one call.
// Shape: Ego_d.pdb's C13 line table (ref/00646762/egod_lines.tsv, lines 7377..7471; Ego_d is a VC10 build, so `this->` lines run a
//   few bytes over VC7.1 /Od and the STL `for` line is not comparable): `EffectIndex = 0;` (10 bytes) before `effect_index = 0;` (4),
//   the fully-faded-in test on the member (16), the change-text offset built by ONE two-argument constructor line of 79 bytes = two
//   GFGetInterpolatedValue calls (VC7.1 /Od: 79), the mirror line 62 (GetVStringWidth call form: 56 + the `this` reloads).
// Callees: GFGetInterpolatedValue (inline, engine/GFGetInterpolatedValue.h, FableWin's named-delta text), GFFloatToLongFloor (inline),
//   NPlayerGui::GFAnchorToBottomLeft 0x00643CDA, GFIntToWideString 0x00405F00, __ftol2 0x00BFEA70, operator+(const wchar_t*, const
//   CWideString&) 0x0099BFF0, CWideString::operator= 0x0099B7D0, ~CWideString 0x0099B510, DrawStatBox (virtual slot 2),
//   GFACoordToVCoord 0x009E21D0, CFontBank::GetVStringWidth (inline here) -> GetVStringExtents 0x00437210, CDisplayManager::
//   GetRenderTargetDimensions 0x009BEDC0 on GPDisplayManager 0x013B8390, GFAXToVX 0x009E1DA0 (?GFAXToVX@@YIMM@Z: actual to virtual X;
//   the first draft's GFVXToAX was wrong), CPlayerGui::Draw2DText 0x00438010, DrawChangeText 0x00645F2E.
//   Data: 1.0f 0x0122DED8, 0.0f 0x0122DEDC, DELTA_NOTIONAL_ZERO 0x0129BA3C (declared by engine/C2DVector.h), L"+" 0x01231760 (both sign prefixes).
// Inline-budget: two header / TU devices, no flag --
//   (1) CFontBank::GetVStringWidth is expanded here (new overlay opt-in FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH_INLINE, the text of its landed
//       out-of-line copy 0x0043E4D0): retail calls GetVStringExtents FIRST and copies its X into a float temporary in the dead `mirror`
//       word (0x0064695F..0x0064696C `call ; mov eax,[eax] ; ... ; mov [ebp+20h],eax`) before the dimensions call -- an inline boundary;
//       no operand order of `... - GetVStringExtents(value_text).X` gives that (probes probe/6762 m1..m4). GetVStringExtents' own inline
//       body is visible as well; VC7.1 leaves it out of line by itself (retail's call 0x00437210).
//   (2) GFAnchorToBottomLeft's body is in this TU, ahead of the row (see the Fidelity line): with a declaration only, the anchor / extents
//       result temporaries share the later loop-sum slot (frame 0x38); with the body visible they get retail's own word at [ebp-40h]
//       (frame 0x40; probes probe/6762 t8 v t9).
// Bracket: the first constructor argument `(GFGetInterpolatedValue(.. X ..))` is byte-required: it fixes 0x006469F6 `fadd st(1)` +
//   0x006469F8 `fstp st(1)` (without it VC7.1 emits one `faddp st(1)`, 738 v 740 bytes). Both arguments bracketed also match; the
//   second argument alone, or a bracket on OldOffset.X / Offset.X / interpolate inside the call, do not (probes probe/6762 w1..w11).
// Overlays (today's headers + opt-ins): engine/NPlayerGui_CStatBar.h FABLE_RETAIL_NPLAYERGUI_CSTATBAR_DRAWCHANGETEXT (declaration),
//   engine/CFontBank.h FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH_INLINE.
#pragma optimize("s", on)
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGE_TEXT_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CMULTISTATBAR_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CDRAWEXPERIENCE_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_EXPERIENCE_PASS23
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_MAINFONT
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_CGRAPHICDATABANK_FRAMESIZE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_BOX_PASS23
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#define FABLE_RETAIL_NPLAYERGUI_CSTATBAR_DRAWCHANGETEXT
#define FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH_INLINE
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_NAMED
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CIEngine.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CFontBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CMultiStatBar.h"
#include "engine/NPlayerGui_CDrawExperience.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/NPlayerGui_Graphics.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFIntToWideString.h"
#include "engine/GFGetInterpolatedValue.h"
#include <math.h>
extern CDisplayManager* GPDisplayManager;                                       // 0x013B8390 ?GPDisplayManager@@3PAVCDisplayManager@@A
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector& acoord, C2DVector& vcoord); // 0x009E21D0 ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z


// Fidelity: same-TU strong definition of 0x00643CDA (retail TU-mate)
C2DVector FABLE_FASTCALL NPlayerGui::GFAnchorToBottomLeft(const C2DVector& pos)
{
    return C2DVector(pos.X, (float)GPDisplayManager->GetRenderTargetDimensions().Y + pos.Y - 480.0f);
}

#pragma optimize("y", off)
void NPlayerGui::CStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& graphic_bank,
                                             const C2DVector& bar_pos, const C2DVector& text_offset, const CRGBColour& colour,
                                             unsigned char alpha, bool mirror)
{
    C2DVector tl_pos((float)GFFloatToLongFloor(GFGetInterpolatedValue(-GuiDef.MoneyWidth, bar_pos.X, FadeInTimer)), bar_pos.Y);
    tl_pos = GFAnchorToBottomLeft(tl_pos);
    C2DVector text_pos = tl_pos + text_offset;
    CWideString value_text = GFIntToWideString((long)OldValue);
    long effect_index = EffectIndex;
    if (effect_index == GuiDef.StatBoxGoldGainEffect)
    {
        effect_index = 0;
        if (!ChangeTexts.empty())
        {
            const CChangeText& change_text = ChangeTexts.front();
            if (change_text.Value > 0)
                effect_index = GuiDef.StatBoxGoldGainEffect;
            else if (change_text.Value < 0)
                effect_index = GuiDef.StatBoxGoldLossEffect;
        }
    }
    else
    {
        if (FadeInTimer >= 1.0f)
        {
            if (EffectIndex == 0)
            {
                if (GraphicIndex == GuiDef.StatUpdateGraphics[STAT_UPDATE_RENOWN])
                    EffectIndex = GuiDef.StatUpdateEffects[STAT_UPDATE_RENOWN];
                else if (GraphicIndex == GuiDef.StatUpdateMoralityGoodGraphic)
                    EffectIndex = GuiDef.StatUpdateMoralityGoodEffect;
                else if (GraphicIndex == GuiDef.StatUpdateMoralityEvilGraphic)
                    EffectIndex = GuiDef.StatUpdateMoralityEvilEffect;
                effect_index = EffectIndex;
            }
        }
        else if (FadeInTimer < DELTA_NOTIONAL_ZERO)
        {
            EffectIndex = 0;
            effect_index = 0;
        }
        if (GraphicIndex == GuiDef.StatUpdateGraphics[STAT_UPDATE_MORALITY])
            value_text = L"+" + GFIntToWideString((long)fabs(OldValue));
        if (OldValue > 0.0f)
            value_text = L"+" + value_text;
    }
    DrawStatBox(info, graphic_bank, tl_pos, GraphicIndex, BorderGraphicIndex, effect_index, value_text, alpha, mirror, false);
    C2DVector vtext_pos;
    GFACoordToVCoord(text_pos, vtext_pos);
    if (mirror)
        vtext_pos.X = GFAXToVX((float)GPDisplayManager->GetRenderTargetDimensions().X) - vtext_pos.X - FontBank.GetVStringWidth(value_text);
    CPlayerGui::Get().Draw2DText(vtext_pos, value_text, FontBank, CRGBColour(colour.R, colour.G, colour.B, alpha), 1.0f, ENGINE_2D_LAYER_0, false);
    if (GuiMode == 0 || GuiMode == 3)
    {
        float interpolate = info.Interpolate.Current.WFInterpolate;
        for (std::list<CChangeText>::const_iterator it = ChangeTexts.begin(); it != ChangeTexts.end(); ++it)
        {
            const CChangeText& change_text = *it;
            C2DVector change_text_offset((GFGetInterpolatedValue(change_text.OldOffset.X, change_text.Offset.X, interpolate)),
                                         GFGetInterpolatedValue(change_text.OldOffset.Y, change_text.Offset.Y, interpolate));
            C2DVector v_text_pos;
            GFACoordToVCoord(change_text_offset + tl_pos, v_text_pos);
            DrawChangeText(v_text_pos, change_text.Text, change_text.Colour);
        }
    }
}