#ifndef FABLE_ENGINE_CXMVPLAYER_H
#define FABLE_ENGINE_CXMVPLAYER_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). Only the two static setters the front end uses: Ego_r.pdb
// ?SetClearColour@CXMVPlayer@@SIXVCRGBColour@@@Z (uncalled inline copy 0x0042DB80: `mov eax,[esp+4] ; mov [13961E0h],eax ; ret 4`)
// and ?SetSkipOnAnyButton@CXMVPlayer@@SIX_N@Z (uncalled copy 0x0042DB90: `mov [13CA7E9h],cl ; ret`; of the three CXMVPlayer static
// bool setters, SetLeftAlignText / SetNoHangulWordWrap are the called copies 0x009BC890 / 0x009BC8A0 fed from the start-up config
// bytes 0x013B861B / 0x013B861C, this one is set only around the front-end movies). The two statics have no PDB data symbol:
// descriptive names.
#include "engine/CRGBColour.h"
class CXMVPlayer {
public:
    static void __fastcall SetClearColour(CRGBColour colour) { ClearColour = colour; }
    static void __fastcall SetSkipOnAnyButton(bool skip) { SkipOnAnyButton = skip; }
private:
    static CRGBColour ClearColour;                                           // 0x013961E0 (descriptive)
    static bool SkipOnAnyButton;                                             // 0x013CA7E9 (descriptive)
};
#endif
