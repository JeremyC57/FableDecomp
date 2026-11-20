// CDisplayManager::SetExclusiveMode(bool) -- re-create the device with the last creation parameters but a new exclusive-mode flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C0020, length 144 bytes.
// Leaf: ?SetExclusiveMode@CDisplayManager@@QAE?AW4ECreateDeviceSuccess@1@_N@Z
// Callees: CDisplayManager::ResetDevice(const CCreateDeviceParameters&) @ 0x009BFB30 (private).
// Layout: PDB CDisplayManager (748 bytes): ExclusiveMode +0x1a0 (416), LastDeviceCreationParameters at PDB +624; the
// shipped build stores the DeviceResetCallbacks list as one sentinel pointer (PDB 12 bytes), so every member after it
// sits 8 lower and the parameters live at +0x268. CCreateDeviceParameters (44 bytes, PDB) is copied memberwise
// (the bool at +0x1c as one byte, the CPresentationIntervalDesc at +0x20 as two dwords) into a 0x2c-byte frame local.
// Shape: when the flag already matches the method returns 2 (CREATE_DEVICE_SUCCESS: nothing to change); otherwise the copy of
// ExclusiveMode is dead (overwritten by the argument) so only the other members are copied, and ResetDevice's
// result is returned.
// NF145 (q11808): on engine/CDisplayManager.h (opt-ins _CREATEDEVICE for ECreateDeviceSuccess, new _RESETDEVICE for SetExclusiveMode /
//   ResetDevice; the 0x009C11A0 opt-in set for the member types). The local CDisplayManager model is gone: the parameter copy is the
//   header's implicit CCreateDeviceParameters copy constructor (owner 0x009C11A0, pick-any), which VC7.1 expands here as retail does.
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

CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetExclusiveMode(bool exclusive)
{
    if (ExclusiveMode != exclusive) {
        CCreateDeviceParameters params(LastDeviceCreationParameters);
        params.ExclusiveMode = exclusive;
        return ResetDevice(params);
    }
    return CREATE_DEVICE_SUCCESS;
}
