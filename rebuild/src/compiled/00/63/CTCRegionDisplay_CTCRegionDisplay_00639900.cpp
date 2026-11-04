// CTCRegionDisplay::~CTCRegionDisplay() -- region-name display component dtor: leave the global-update TC list, then ~CurrentAreaNameExtra, ~CurrentAreaName and ~CTCBase
// Leaf: ??1CTCRegionDisplay@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00639900, virtual destructor (slot 0 target of the scalar deleting dtor).
// Identity: Ego_r.pdb ??1CTCRegionDisplay@@UAE@XZ; RTTI vtable of CTCRegionDisplay : CTCBase; members CurrentAreaName +0xc,
//   CurrentAreaNameExtra +0x10 (CWideString, ~CWideString 0x0099B510).
// Callees: CTCBase::RemoveFromGlobalTCs, ~CWideString x2, CTCBase::~CTCBase (engine/CTCBase.h retail branch).
// Shape: user body `RemoveFromGlobalTCs();`, then members in reverse order and the base.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "fable_string.h"

class CTCRegionDisplay : public CTCBase {
public:
    virtual ~CTCRegionDisplay();
    CWideString   CurrentAreaName;                       // +0x0c
    CWideString   CurrentAreaNameExtra;                  // +0x10
    long          FrameNameChanged;                      // +0x14
    long          CurrentAlpha;                          // +0x18
    long          LastAlpha;                             // +0x1c
    float         DisplayTime;                           // +0x20
    bool          TimeOut;                               // +0x24
    bool          InfoDisplay;                           // +0x25
    bool          UpdateWhilePaused;                     // +0x26
};

CTCRegionDisplay::~CTCRegionDisplay()
{
    RemoveFromGlobalTCs();
}