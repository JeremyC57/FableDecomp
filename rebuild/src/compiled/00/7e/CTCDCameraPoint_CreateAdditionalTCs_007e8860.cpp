// CTCDCameraPoint::CreateAdditionalTCs() -- this driver adds no thing components (empty override, `ret`)
// Leaf: ?CreateAdditionalTCs@CTCDCameraPoint@@UAEXXZ
// Identity: slot 29 of CTCDCameraPoint's vtable (CTCDriverBase slot 29 CreateAdditionalTCs, engine/CTCDriverBase.h, landed base body
//   0x004D3F2B); Ego_r/FableWin name set: ?CreateAdditionalTCs@CTCDCameraPoint@@UAEXXZ; the body is a bare `ret`.
// Model: no engine/ header exists for CTCDCameraPoint; declared over the shared engine/CTCDriverBase.h with just this override.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCDriverBase.h"
class CTCDCameraPoint : public CTCDriverBase {
public:
    virtual void CreateAdditionalTCs();
};
void CTCDCameraPoint::CreateAdditionalTCs()
{
}
