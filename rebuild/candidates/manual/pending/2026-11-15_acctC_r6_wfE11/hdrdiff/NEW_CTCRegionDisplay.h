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
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_REGION_DISPLAY; }
    void SetCurrentAreaName(const CCharString&, float, bool, bool, bool);
    void SetCurrentAreaName(const CWideString&, float, bool, bool, bool);
};
#endif
#endif // FABLE_ENGINE_CTCREGIONDISPLAY_H
