#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWNPCNAME_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWNPCNAME_H
// NPlayerGui::CDrawNPCName (new, chunk 765): RTTI .?AVCDrawNPCName@NPlayerGui@@, single base NPlayerGui::CDrawBase at +0 (rtti_bases.tsv).
// Ego_r.pdb members (52 bytes): Name (CWideString) +0x2C, ScreenShotMessageCounter +0x30, 4 lower in retail on the 0x28-byte CDrawBase:
// Name +0x28, ScreenShotMessageCounter +0x2C; sizeof 0x30. Ego_r donor order SetName 0x00699420, IsShowingScreenshotMessage 0x00699430,
// ShowScreenshotMessage 0x00699480, Draw 0x006994A0 = retail 0x00657230, 0x00657240, 0x00657280, 0x006572A0. CPlayerGui holds it as the
// counted pointer +0x248 (engine/CPlayerGui.h).
#include "engine/NPlayerGui_CDrawBase.h"
#include "fable_string.h"
namespace NPlayerGui {
class CDrawNPCName : public CDrawBase {
public:
    void SetName(const CWideString& name);                               // 0x00657230
protected:
    CWideString Name;                                                    // +0x28
    long ScreenShotMessageCounter;                                       // +0x2c
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawNPCName) == 0x30);
#endif
