// CDisplayManager::SetDisplayMode(const C2DExtentsI& size, unsigned long bitDepth, ESurfaceMultisampleType multisample, unsigned long refreshRate) -- return 2 (succeeded, nothing to do) when the requested mode is the current one, else reset the device with the last creation parameters changed to the new size (at least 32x32), depth, refresh rate and multisampling
// Leaf: ?SetDisplayMode@CDisplayManager@@QAE?AW4ECreateDeviceSuccess@1@ABVC2DExtentsI@@KW4ESurfaceMultisampleType@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BFF40, length 222 bytes, `ret 10h`.
// Callees: CPixelFormat::GetColourDepth() @ 0x009E3820, CDisplayManager::ResetDevice(const CCreateDeviceParameters&) (private)
// @ 0x009BFB30.
// Layout: PDB CDisplayManager: BackBufferDimensions +0x18c, MultisampleType +0x1a4, BackBufferFormat +0x1a8, RefreshRate
// +0x1c8; LastDeviceCreationParameters at retail +0x268 (PDB +0x270: the DeviceResetCallbacks list before it is a 4-byte
// STLport list). CCreateDeviceParameters (PDB, 0x2c bytes): WinHandle +0, BackBufferDimensions +4, BackBufferBitDepth
// +0xc, ZBufferBitDepth +0x10, BackBufferCount +0x14, RefreshRate +0x18, ExclusiveMode +0x1c, PresentationInterval
// +0x20 (8 bytes), MultisampleType +0x28.
// Shape: the unchanged test is one `&&` chain returning 2; the parameter block is a copy of the last parameters through
// the header's implicit memberwise copy constructor, expanded (VC drops the stores the following assignments overwrite
// and schedules the kept members first), then the size (clamped up to 32 per axis), depth,
// refresh rate and multisampling are assigned and the block is handed to ResetDevice.
// NF145 (q11808): on engine/CDisplayManager.h (opt-ins _CREATEDEVICE for ECreateDeviceSuccess, new _RESETDEVICE for SetDisplayMode /
//   ResetDevice, CPixelFormat _COLOURDEPTH; the 0x009C11A0 opt-in set for the member types). The local models of CDisplayManager,
//   C2DExtentsI, CPixelFormat and CPresentationIntervalDesc are gone: the parameter copy is the header's implicit
//   CCreateDeviceParameters copy constructor (owner 0x009C11A0, pick-any), which VC7.1 expands here as retail does.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#define FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
#define FABLE_RETAIL_CDISPLAYMANAGER_RESETDEVICE
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"

CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetDisplayMode(const C2DExtentsI& size, unsigned long bit_depth, ESurfaceMultisampleType multisample_type, unsigned long refresh_rate)
{
    if (size.X == BackBufferDimensions.X && size.Y == BackBufferDimensions.Y && bit_depth == BackBufferFormat.GetColourDepth()
        && refresh_rate == RefreshRate && multisample_type == MultisampleType)
        return CREATE_DEVICE_SUCCESS;
    CCreateDeviceParameters parameters = LastDeviceCreationParameters;
    parameters.BackBufferDimensions = size;
    if (parameters.BackBufferDimensions.X < 32)
        parameters.BackBufferDimensions.X = 32;
    if (parameters.BackBufferDimensions.Y < 32)
        parameters.BackBufferDimensions.Y = 32;
    parameters.BackBufferBitDepth = bit_depth;
    parameters.RefreshRate = refresh_rate;
    parameters.MultisampleType = multisample_type;
    return ResetDevice(parameters);
}
