#ifndef FABLE_ENGINE_CXMVPLAYER_H
#define FABLE_ENGINE_CXMVPLAYER_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). Only the two static setters the front end uses: Ego_r.pdb
// ?SetClearColour@CXMVPlayer@@SIXVCRGBColour@@@Z (uncalled inline copy 0x0042DB80: `mov eax,[esp+4] ; mov [13961E0h],eax ; ret 4`)
// and ?SetSkipOnAnyButton@CXMVPlayer@@SIX_N@Z (uncalled copy 0x0042DB90: `mov [13CA7E9h],cl ; ret`; of the three CXMVPlayer static
// bool setters, SetLeftAlignText / SetNoHangulWordWrap are the called copies 0x009BC890 / 0x009BC8A0 fed from the start-up config
// bytes 0x013B861B / 0x013B861C, this one is set only around the front-end movies). The two statics have no PDB data symbol:
// descriptive names.
#include "engine/CRGBColour.h"
#ifdef FABLE_RETAIL_CXMVPLAYER_GETCHARWIDTHW
#include "engine/CCountedPointer.h"
class CFontBank;
#endif
class CXMVPlayer {
public:
    static void __fastcall SetClearColour(CRGBColour colour) { ClearColour = colour; }
    static void __fastcall SetSkipOnAnyButton(bool skip) { SkipOnAnyButton = skip; }
#ifdef FABLE_RETAIL_CXMVPLAYER_TEXTLAYOUT_SETTERS
    // b24c4 opt-in (row 0x00402510 GFMain): Ego_r.pdb ?SetLeftAlignText@CXMVPlayer@@SIX_N@Z /
    // ?SetNoHangulWordWrap@CXMVPlayer@@SIX_N@Z, out of line in retail (0x009BC890 / 0x009BC8A0). Declarations only.
    static void __fastcall SetLeftAlignText(bool left_align);          // 0x009BC890
    static void __fastcall SetNoHangulWordWrap(bool no_wrap);          // 0x009BC8A0
#endif
private:
    static CRGBColour ClearColour;                                           // 0x013961E0 (descriptive)
    static bool SkipOnAnyButton;                                             // 0x013CA7E9 (descriptive)
#ifdef FABLE_RETAIL_CXMVPLAYER_GETCHARWIDTHW
    // Opt-in (w23 e15, row 0x009BCE70): Ego_r.pdb ?GetCharWidthW@CXMVPlayer@@SII_W@Z (public static __fastcall; retail VC7.1 wide char =
    // unsigned short -> SIIG) and Ego_d.pdb's private static CCountedPointer<CFontBank> PlayerFont (LF_STMEMBER; `dynamic initializer
    // for 'CXMVPlayer::PlayerFont'`), 0x013CA7F0 (read only by 0x009BCE70 and its atexit destructor 0x0122A6C0 (initializer 0x01224070)). Needs engine/CCountedPointer.h.
public:
    static unsigned int __fastcall GetCharWidthW(unsigned short c);                 // 0x009BCE70
private:
    static CCountedPointer<CFontBank> PlayerFont;                            // 0x013CA7F0
#endif
};
#endif
