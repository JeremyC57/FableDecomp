#ifndef FABLE_ENGINE_CTCREGIONDISPLAY_H
#define FABLE_ENGINE_CTCREGIONDISPLAY_H
// New header (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad). Retail opt-in only: FABLE_RETAIL_CTCREGIONDISPLAY.
// Class: Ego_r.pdb CTCRegionDisplay : CTCBase; interface TCI_REGION_DISPLAY (142). Method: Ego_r.pdb
// ?SetCurrentAreaName@CTCRegionDisplay@@QAEXABVCWideString@@M_N11@Z / ..ABVCCharString@@M_N11@Z (out of line). Members are not modelled.
#ifdef FABLE_RETAIL_CTCREGIONDISPLAY
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
class CCharString;
class CWideString;
class CTCRegionDisplay : public CTCBase {
public:
#if defined(FABLE_RETAIL_CTCREGIONDISPLAY_ISDRAWING_PASS26) && !defined(FABLE_RETAIL_REGION_DRAW_PASS29)
    // Retail opt-in (wfE9, row 0x00648820 NPlayerGui::CDrawMiniMap::Draw): Ego_r.pdb ?IsDrawing@CTCRegionDisplay@@QBE_NXZ, out of line at
    // 0x00639B70 (landed row CTCRegionDisplay_IsDrawing_00639b70.cpp). Declaration only.
    bool IsDrawing() const;                                             // 0x00639B70
#endif
#ifdef FABLE_RETAIL_REGION_DRAW_PASS29
    void GlobalDraw(const class CInterpolationInfo& interp) const;
    bool IsDrawing() const { return CurrentAlpha > 0 && CurrentAreaName != L""; }
private:
    CWideString CurrentAreaName;
    CWideString CurrentAreaNameExtra;
    long FrameNameChanged, CurrentAlpha, LastAlpha;
    float DisplayTime;
    bool TimeOut, InfoDisplay, UpdateWhilePaused;
public:
#endif
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_REGION_DISPLAY; }
#ifdef FABLE_RETAIL_TARGETING_PLAYER_TEST_PASS27
    CTCRegionDisplay(CThing& thing);
#endif
    void SetCurrentAreaName(const CCharString&, float, bool, bool, bool);
    void SetCurrentAreaName(const CWideString&, float, bool, bool, bool);
};
#endif
#endif // FABLE_ENGINE_CTCREGIONDISPLAY_H
