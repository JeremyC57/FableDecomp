// Behaviour test for CDisplayManager::SetDisplayMode (0x009BFF40). NF145: on engine/CDisplayManager.h with the row's opt-ins; the row body is repeated below
// and ResetDevice is a recording stand-in.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#define FABLE_RETAIL_CDISPLAYMANAGER_CREATEDEVICE
#define FABLE_RETAIL_CDISPLAYMANAGER_RESETDEVICE
#define private public
#define protected public
#include "engine/C2DExtentsI.h"
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"
#undef private
#undef protected
// The manager is never constructed: the methods under test only read and copy plain members, so a zeroed block stands in.
static double gStorage[sizeof(CDisplayManager) / sizeof(double) + 1];
static CDisplayManager& Manager() { return *(CDisplayManager*)gStorage; }
static int gResets = 0;
static CDisplayManager::CCreateDeviceParameters gSeen;
static CDisplayManager::ECreateDeviceSuccess gResult = CDisplayManager::CREATE_DEVICE_FAILED;
CDisplayManager::ECreateDeviceSuccess CDisplayManager::ResetDevice(const CCreateDeviceParameters& parameters)   // stand-in for 0x009BFB30
{
    ++gResets;
    memcpy(&gSeen, &parameters, sizeof(gSeen));
    return gResult;
}
static unsigned long gColourDepth = 32;
unsigned long CPixelFormat::GetColourDepth() const { return gColourDepth; }   // stand-in for 0x009E3820
// The row body (0x009BFF40).
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

int main()
{
    CDisplayManager& d = Manager();
    HWND window = (HWND)&d;
    d.BackBufferDimensions.X = 640; d.BackBufferDimensions.Y = 480; d.RefreshRate = 60;
    d.MultisampleType = (ESurfaceMultisampleType)0;
    d.LastDeviceCreationParameters.WinHandle = window; d.LastDeviceCreationParameters.ZBufferBitDepth = 24;
    d.LastDeviceCreationParameters.BackBufferCount = 2; d.LastDeviceCreationParameters.ExclusiveMode = true;
    d.LastDeviceCreationParameters.PresentationInterval.PresentImmediate = true; d.LastDeviceCreationParameters.PresentationInterval.VSyncCount = 7;
    d.LastDeviceCreationParameters.RefreshRate = 75; d.LastDeviceCreationParameters.BackBufferBitDepth = 16;
    gResult = CDisplayManager::CREATE_DEVICE_FAILED;
    int bad = 0;
    C2DExtentsI same; same.X = 640; same.Y = 480;
    if (d.SetDisplayMode(same, 32, (ESurfaceMultisampleType)0, 60) != CDisplayManager::CREATE_DEVICE_SUCCESS || gResets) bad = 1;
    C2DExtentsI tiny; tiny.X = 8; tiny.Y = 600;
    if (d.SetDisplayMode(tiny, 16, (ESurfaceMultisampleType)2, 85) != CDisplayManager::CREATE_DEVICE_FAILED || gResets != 1) bad = 2;
    const CDisplayManager::CCreateDeviceParameters& p = gSeen;
    if (p.BackBufferDimensions.X != 32 || p.BackBufferDimensions.Y != 600 || p.BackBufferBitDepth != 16 || p.RefreshRate != 85
        || p.MultisampleType != 2) bad = 3;
    if (p.WinHandle != window || p.ZBufferBitDepth != 24 || p.BackBufferCount != 2 || !p.ExclusiveMode
        || !p.PresentationInterval.PresentImmediate || p.PresentationInterval.VSyncCount != 7) bad = 4;
    if (bad) { printf("009bff40_TEST FAIL\n"); return 1; }
    printf("009bff40_TEST PASS\n");
    return 0;
}
