// CTCDRegionEntrance::CreateAdditionalTCs() -- this driver adds no thing components (empty override, `ret`)
// Leaf: ?CreateAdditionalTCs@CTCDRegionEntrance@@UAEXXZ
// Identity: slot 29 of CTCDRegionEntrance's vtable (CTCDriverBase slot 29 CreateAdditionalTCs, engine/CTCDriverBase.h, landed base body
//   0x004D3F2B); Ego_r/FableWin name set: ?CreateAdditionalTCs@CTCDRegionEntrance@@UAEXXZ; the body is a bare `ret`.
// Model: no engine/ header exists for CTCDRegionEntrance; declared over the shared engine/CTCDriverBase.h with just this override.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCDriverBase.h"
class CTCDRegionEntrance : public CTCDriverBase {
public:
    virtual void CreateAdditionalTCs();
};
void CTCDRegionEntrance::CreateAdditionalTCs()
{
}
