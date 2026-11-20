// Behaviour test for CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc&) (0x009BFE70). NF145: on engine/CDisplayManager.h with the row's opt-ins; the row body is repeated below
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
// The row body (0x009BFE70).
void CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc& interval)
{
    CCreateDeviceParameters params(LastDeviceCreationParameters);
    if (params.PresentationInterval.PresentImmediate != interval.PresentImmediate
        || params.PresentationInterval.VSyncCount != interval.VSyncCount) {
        params.PresentationInterval = interval;
        ResetDevice(params);
    }
}

int main()
{
    CDisplayManager& m = Manager();
    m.LastDeviceCreationParameters.PresentationInterval.PresentImmediate = false;
    m.LastDeviceCreationParameters.PresentationInterval.PresentImmediateIfSyncMissed = false;
    m.LastDeviceCreationParameters.PresentationInterval.VSyncCount = 1;
    m.LastDeviceCreationParameters.RefreshRate = 60;
    int ok = 1;
    CDisplayManager::CPresentationIntervalDesc same = { false, true, 1 };   // differs only in the ignored field
    m.ChangePresentationInterval(same);
    if (gResets != 0) ok = 0;
    CDisplayManager::CPresentationIntervalDesc other = { true, true, 2 };
    m.ChangePresentationInterval(other);
    if (gResets != 1 || !gSeen.PresentationInterval.PresentImmediate || !gSeen.PresentationInterval.PresentImmediateIfSyncMissed
        || gSeen.PresentationInterval.VSyncCount != 2 || gSeen.RefreshRate != 60) ok = 0;
    if (m.LastDeviceCreationParameters.PresentationInterval.VSyncCount != 1) ok = 0;
    if (!ok) { printf("009bfe70_TEST FAIL\n"); return 1; }
    printf("009bfe70_TEST PASS\n");
    return 0;
}
