// NUISystem::CManager::CreateTickSlider @ 0x00427216 -- builds one "tick slider" bar of the PC inventory screens (the stats,
// experience and abilities pages): a PC_TICK_SLIDER component whose back sprites are stretched to the requested width, a name
// and a level text, one tick sprite per level up to the maximum (white = earned, black = not yet, "LO" variants and a middle
// marker at tick 7 for the morality bar), a "maxed out" overlay when every level is earned, a fill sprite showing the levels the
// hero could buy with the experience he holds (or, for the morality bar, the good / evil fill around the middle marker with the
// very-good / very-bad / neutral morality icons), and a ghost sprite plus an optional cap sprite on the normal bar.
// Leaf: ?CreateTickSlider@CManager@NUISystem@@QAEPAVCComponent@2@ABVCWideString@@ABVC2DCoordI@@JJABVCRGBFloatColour@@MM_N32M@Z
// Retail: 2995 bytes, `ret 2Ch`, plain frame (`push ebp; mov ebp,esp; sub esp,50h`), ebx = 0 throughout, esi = the slider.
// Callees: NUISystem::CManager::CreateComponent(const CCharString&, bool) 0x0041DB1D, CCharString::CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString::CWideString(const wchar_t*) 0x0099B6B0 / ~CWideString 0x0099B510,
//   GFIntToCharString 0x0099F830 (__fastcall, hidden CCharString result), std::map<unsigned long, NUISystem::CUIState>::operator[]
//   0x0042D5B1 (38 calls), CCountedPointer<NUISystem::CComponent>::CCountedPointer(CComponent*) 0x00429C15 (called COMDAT),
//   ::Reset 0x00429C6F, ::~CCountedPointer 0x004291DE, __ftol2 0x00BFEA70 (19 calls), CComponent slot 3 FindChild (+0x0C), slot 59
//   AddChild (+0xEC), slot 82 GetStates (+0x148), CText slot 144 SetValue(const CWideString&, bool, bool) (+0x240) and slot 145
//   SetValue(const CCharString&, bool, bool) (+0x244).
// Float pool: 0.125f 0x0123095C, 144.0f 0x01230898, 32.0f 0x01230894, 140.0f 0x01230890, 4.0f 0x0123088C, 136.0f 0x01230888,
//   12.0f 0x01230884, 3.0f (immediate 0x40400000), 14.0f (immediate 0x41600000), 20.0f 0x0122F160, 31.0f 0x012307B0, 139.0f 0x01230790,
//   0.25f 0x0123078C, 1/7 (0.142857f) 0x01230788, 255.0f 0x01230014 (the byte scale of every CRGBFloatColour -> CRGBColour
//   conversion), 0.0f 0x0122DEDC, -1.0f 0x0122DEE0, L"" 0x0122D70C.
// Layout: [ebp-4] this, [ebp-8] startTick then the tick loop counter, [ebp-0xC] levelText, [ebp-0x1C] (float)maxValue, [ebp-0x28]
//   sliderStates, [ebp-0x30] back3 then middleX, [ebp-0x48] rightPart, [ebp-0x50] the per-tick counted pointer, [ebp-0x3C] the
//   hoisted [0] key of the tick loop, [ebp-0x2C]/[ebp-0x38]/[ebp-0x24] veryBad / veryGood / neutral, [ebp-0x40] cap; the string
//   temporaries and the later floats live in the dead parameter slots (name, position, value, width, the two flags).
// Shape (RELOCATION_MATCH at grade v8): CreateComponent takes its name through the implicit const char* -> CCharString conversion
//   (an explicit CCharString(...) prvalue is constructed at its push position, after the bool); the two-way choices (right part name,
//   level text "x" / number) are if/else statements whose common head and tail VC merges (the shared `lea ecx; push -1` before the
//   branch and the single destructor after it; a ?: of two CCharString prvalues makes two temporaries with lifetime flags); the
//   byte colours come from CRGBFloatColour's conversion operator (engine/CRGBFloatColour.h FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR),
//   the ghost's through a named copy whose alpha is replaced (the copy is the default byte-wise constructor: with the Initialise-based
//   FABLE_RETAIL_CRGBCOLOUR copy VC calls it out of line and keeps the dead alpha conversion); the morality fill width is
//   `float fillLevels = possibleValue; if (possibleValue < 0.0f) fillLevels = possibleValue * -1.0f;` (a ?: fuses the compare into
//   the multiply), and fabs(morality) is written twice (a named local moves the key store of the first alpha past the fsub).
// Names: the parameter names are this row's (the PDB keeps only the mangled signature); the two flags are read as "morality bar"
//   (the UI_MORALITY_* sprites and the LO tick sprites are only built with it) and "show the cap sprite".
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_TO_CRGBCOLOUR
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#include <math.h>
#include "engine/C2DCoordI.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CRGBColour.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CText.h"
#include "engine/CCountedPointer.h"
#pragma intrinsic(fabs)

CCharString FABLE_FASTCALL GFIntToCharString(long value);                         // 0x0099F830

namespace NUISystem {

CComponent* CManager::CreateTickSlider(const CWideString& name, const C2DCoordI& position, long value, long maxValue,
                                       const CRGBFloatColour& colour, float possibleValue, float width, bool moralityBar,
                                       bool showCap, const CRGBFloatColour& capColour, float labelWidth)
{
    // the morality bar leaves room for eight ticks of "evil" left of the middle marker
    long startTick = 0;
    if (moralityBar)
        startTick = 8;
    // the hero can never buy past the last tick
    if (possibleValue > (float)maxValue)
        possibleValue = (float)maxValue;

    CComponent* slider = CreateComponent("PC_TICK_SLIDER", false);
    std::map<unsigned long, CUIState>& sliderStates = slider->GetStates();
    sliderStates[0].Position.X = (float)position.X;
    sliderStates[0].Position.Y = (float)position.Y;

    // the three back sprites: the first is stretched by the label width (eight pixels per zoom unit), the others move right
    CComponent* back1 = slider->FindChild(CCharString("UI_TICK_SLIDER_BACK_SPRITE1"));
    back1->GetStates()[0].Zoom.X += labelWidth * 0.125f;
    CComponent* back2 = slider->FindChild(CCharString("UI_TICK_SLIDER_BACK_SPRITE2"));
    back2->GetStates()[0].Position.X += labelWidth;
    CComponent* back3 = slider->FindChild(CCharString("UI_TICK_SLIDER_BACK_SPRITE3"));
    back3->GetStates()[0].Position.X += labelWidth;

    CCountedPointer<CComponent> rightPart;
    if (moralityBar)
        rightPart.Reset(CreateComponent("UI_TICK_SLIDER_BACK_RIGHT_PART1", false));
    else
        rightPart.Reset(CreateComponent("UI_TICK_SLIDER_BACK_RIGHT_PART", false));
    CComponent* back = slider->FindChild(CCharString("UI_TICK_SLIDER_BACK"));
    if (back)
        back->AddChild(rightPart);

    CComponent* text = slider->FindChild(CCharString("UI_TICK_SLIDER_TEXT"));
    if (text)
        ((CText*)text)->SetValue(name, true, true);
    CComponent* levelText = slider->FindChild(CCharString("UI_TICK_SLIDER_LEVEL_TEXT"));
    if (levelText) {
        // no level yet: an "x" instead of the number
        if (value == -1)
            ((CText*)levelText)->SetValue(CCharString("x"), true, true);
        else
            ((CText*)levelText)->SetValue(GFIntToCharString(value), true, true);
    }

    // the morality bar's middle marker pushes the right part out by fourteen pixels
    float middleGap = 0.0f;
    if (moralityBar)
        middleGap = 14.0f;
    if (rightPart.Data != 0) {
        std::map<unsigned long, CUIState>& back3States = back3->GetStates();
        float startOffset = (float)startTick;
        back3States[0].Zoom.X = (width - 144.0f - 32.0f + startOffset - labelWidth + middleGap) * 0.125f;
        back3States[0].Position.X = labelWidth + 144.0f;
        rightPart->GetStates()[0].Position.X = width - 32.0f + startOffset + middleGap;
    }

    // one tick per level: the spacing shares the bar between the label, the caps and the maximum level
    float tickSpacing = (width - 140.0f - 32.0f + 4.0f - labelWidth) / (float)maxValue;
    float tickX = labelWidth + 136.0f;
    long middleX = 0;
    for (long i = 1; i < maxValue; ++i) {
        tickX += tickSpacing;
        CComponent* tick;
        if (moralityBar && i == 7) {
            // the neutral marker in the middle of the morality bar, three ticks wide
            tickX += 4.0f;
            middleX = (long)(tickX + 12.0f);
            tick = CreateComponent("UI_TICK_SLIDER_LEVEL_MIDDLE_SPRITE", false);
            tick->GetStates()[0].Zoom.X = 3.0f;
        } else if (i <= value) {
            // an earned level: white tick
            if (moralityBar)
                tick = CreateComponent("UI_TICK_SLIDER_LEVEL_TICK_SPRITEWLO", false);
            else
                tick = CreateComponent("UI_TICK_SLIDER_LEVEL_TICK_SPRITEW", false);
        } else {
            // a level still to earn: black tick
            if (moralityBar)
                tick = CreateComponent("UI_TICK_SLIDER_LEVEL_TICK_SPRITEBLO", false);
            else
                tick = CreateComponent("UI_TICK_SLIDER_LEVEL_TICK_SPRITEB", false);
        }
        tick->GetStates()[0].Position.X = tickX;
        CCountedPointer<CComponent> tickPointer;
        tickPointer.Reset(tick);
        slider->AddChild(tickPointer);
        if (moralityBar && i == 7)
            tickX += 20.0f;
    }

    // every level earned on a normal bar: clear the level number and show the "maxed out" overlay in the bar's colour
    if (value == maxValue && !moralityBar) {
        ((CText*)levelText)->SetValue(CWideString(L""), true, true);
        CComponent* maxedOut = slider->FindChild(CCharString("UI_TICK_SLIDER_MAXED_OUT"));
        if (maxedOut) {
            maxedOut->GetStates()[0].Position.X = width - 31.0f;
            maxedOut->GetStates()[0].Colour = colour;
        }
    }

    std::map<unsigned long, CUIState>& fillStates = slider->FindChild(CCharString("UI_TICK_SLIDER_FILL_SPRITE"))->GetStates();
    if (!moralityBar) {
        // the fill covers the earned levels (four pixels per zoom unit)
        fillStates[0].Position.X = labelWidth + 139.0f;
        fillStates[0].Zoom.X = (float)value * tickSpacing * 0.25f;
    } else {
        // the morality bar: the alignment runs -7..7 levels either side of the middle marker; the icons fade with it
        float morality = possibleValue * 0.142857f;
        CCountedPointer<CComponent> veryBad(CreateComponent("UI_MORALITY_VERY_BAD_SPRITE", false));
        CCountedPointer<CComponent> veryGood(CreateComponent("UI_MORALITY_VERY_GOOD_SPRITE", false));
        CCountedPointer<CComponent> neutral(CreateComponent("UI_MORALITY_NEUTRAL_SPRITE", false));
        neutral->GetStates()[1].Colour.A = (unsigned char)((1.0f - (float)fabs(morality)) * 255.0f);
        veryGood->GetStates()[1].Colour.A = 0;
        veryBad->GetStates()[1].Colour.A = 0;
        if (morality < 0.0f) {
            veryBad->GetStates()[1].Colour.A = (unsigned char)((float)fabs(morality) * 255.0f);
            veryGood->GetStates()[1].Colour.A = 0;
        }
        if (morality > 0.0f) {
            veryGood->GetStates()[1].Colour.A = (unsigned char)(morality * 255.0f);
            veryBad->GetStates()[1].Colour.A = 0;
        }
        // the three icons sit over the middle marker
        float iconX = (float)(middleX - 12);
        neutral->GetStates()[0].Position.X = iconX;
        neutral->GetStates()[0].Position.Y = 0.0f;
        veryBad->GetStates()[0].Position.X = iconX;
        veryBad->GetStates()[0].Position.Y = 0.0f;
        veryGood->GetStates()[0].Position.X = iconX;
        veryGood->GetStates()[0].Position.Y = 0.0f;
        slider->AddChild(veryBad);
        slider->AddChild(veryGood);
        slider->AddChild(neutral);
        // the fill grows from the middle marker towards the good or the evil end
        float fillLevels = possibleValue;
        if (possibleValue < 0.0f)
            fillLevels = possibleValue * -1.0f;
        fillStates[0].Zoom.X = fillLevels * tickSpacing * 0.25f;
        if (possibleValue < 0.0f)
            fillStates[0].Position.X = tickSpacing * possibleValue + iconX;
        else
            fillStates[0].Position.X = (float)(middleX + 12);
    }
    fillStates[0].Colour = colour;

    if (!moralityBar) {
        // the ghost sprite shows the levels the hero could buy now, in the bar's colour at the sprite's own alpha
        CComponent* ghost = slider->FindChild(CCharString("UI_TICK_SLIDER_GHOST_SPRITE"));
        ghost->GetStates()[0].Position.X = labelWidth + 139.0f;
        ghost->GetStates()[0].Zoom.X = tickSpacing * possibleValue * 0.25f;
        CRGBColour ghostColour = colour;
        ghostColour.A = ghost->GetStates()[0].Colour.A;
        ghost->GetStates()[0].Colour = ghostColour;
        if (showCap) {
            // the cap closes the bar after the last tick
            CCountedPointer<CComponent> cap(CreateComponent("UI_TICK_SLIDER_CAP_SPRITE", false));
            cap->GetStates()[0].Position.X = (float)(maxValue - 1) * tickSpacing + labelWidth + 139.0f;
            cap->GetStates()[0].Zoom.X = tickSpacing * 0.25f;
            cap->GetStates()[0].Colour = capColour;
            slider->AddChild(cap);
        }
    }
    return slider;
}

}