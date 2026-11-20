// Behaviour test for CDisplayManager::SetExclusiveMode (0x009C0020). NF145: on engine/CDisplayManager.h with the row's opt-ins; the row body is repeated below
// and ResetDevice is a recording stand-in.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
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
// The row body (0x009C0020).
CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetExclusiveMode(bool exclusive)
{
    if (ExclusiveMode != exclusive) {
        CCreateDeviceParameters params(LastDeviceCreationParameters);
        params.ExclusiveMode = exclusive;
        return ResetDevice(params);
    }
    return CREATE_DEVICE_SUCCESS;
}

int main()
{
    CDisplayManager& m = Manager();
    m.ExclusiveMode = true;
    m.LastDeviceCreationParameters.BackBufferDimensions.X = 640; m.LastDeviceCreationParameters.BackBufferDimensions.Y = 480;
    m.LastDeviceCreationParameters.ExclusiveMode = true; m.LastDeviceCreationParameters.MultisampleType = (ESurfaceMultisampleType)3;
    gResult = CDisplayManager::CREATE_DEVICE_DEVICE_LOST;
    int ok = 1;
    if (m.SetExclusiveMode(true) != CDisplayManager::CREATE_DEVICE_SUCCESS || gResets != 0) ok = 0;
    if (m.SetExclusiveMode(false) != CDisplayManager::CREATE_DEVICE_DEVICE_LOST || gResets != 1) ok = 0;
    if (gSeen.ExclusiveMode != false || gSeen.BackBufferDimensions.X != 640 || gSeen.BackBufferDimensions.Y != 480 || gSeen.MultisampleType != 3) ok = 0;
    if (m.LastDeviceCreationParameters.ExclusiveMode != true) ok = 0;
    if (!ok) { printf("009c0020_TEST FAIL\n"); return 1; }
    printf("009c0020_TEST PASS\n");
    return 0;
}
