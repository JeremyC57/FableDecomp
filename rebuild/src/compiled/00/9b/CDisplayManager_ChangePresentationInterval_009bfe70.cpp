// CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc&) -- re-create the device when the requested present interval differs from the last one
// Leaf: ?ChangePresentationInterval@CDisplayManager@@QAEXABVCPresentationIntervalDesc@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BFE70, length 160 bytes.
// Callees: CDisplayManager::ResetDevice(const CCreateDeviceParameters&) @ 0x009BFB30 (private).
// Layout: as SetExclusiveMode (0x009C0020): LastDeviceCreationParameters at +0x268 (PDB +624, the shipped
// DeviceResetCallbacks list is 8 bytes narrower), copied memberwise into a 0x2c-byte frame local.
// Shape: the local copy is taken first (its VSyncCount word stays in esi, never stored, because the reset path
// overwrites it); the interval equality compares PresentImmediate and VSyncCount only (written out: neither PDB has an
// operator==; PresentImmediateIfSyncMissed is ignored). On a change the new interval is copied in as two dwords and the device
// is reset; the reset result is discarded (void method).
// NF145 (q11808): on engine/CDisplayManager.h (opt-ins _CREATEDEVICE, new _RESETDEVICE; the 0x009C11A0 opt-in set). The parameter is the
//   nested class CDisplayManager::CPresentationIntervalDesc, as Ego_r.pdb 0x00B24F00 / FableWin.pdb 0x0301CC40 spell the leaf
//   (was the global struct ...ABUCPresentationIntervalDesc@@@Z); the only landed referencer 0x009BFF10 moves with it. The class has no
//   operator== in either PDB, so the two-field test is written out. The copy is the header's implicit CCreateDeviceParameters copy.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#define FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
#define FABLE_RETAIL_CDISPLAYMANAGER_RESETDEVICE
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"

void CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc& interval)
{
    CCreateDeviceParameters params(LastDeviceCreationParameters);
    if (params.PresentationInterval.PresentImmediate != interval.PresentImmediate
        || params.PresentationInterval.VSyncCount != interval.VSyncCount) {
        params.PresentationInterval = interval;
        ResetDevice(params);
    }
}
