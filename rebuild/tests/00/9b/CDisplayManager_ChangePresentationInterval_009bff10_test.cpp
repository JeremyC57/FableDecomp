// Behaviour test for CDisplayManager::ChangePresentationInterval(bool, long, bool) (0x009BFF10). NF145: on engine/CDisplayManager.h with the row's opt-ins; the row body is repeated below
// and ChangePresentationInterval(const CPresentationIntervalDesc&) is a recording stand-in.
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
static int gCalls = 0;
static CDisplayManager::CPresentationIntervalDesc gDesc;
void CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc& interval)   // stand-in for 0x009BFE70
{
    ++gCalls;
    gDesc = interval;
}
// The row body (0x009BFF10).
void CDisplayManager::ChangePresentationInterval(bool present_immediate, long vsync_count, bool present_immediate_if_sync_missed)
{
    CPresentationIntervalDesc desc = { present_immediate, present_immediate_if_sync_missed, vsync_count };
    ChangePresentationInterval(desc);
}

int main()
{
    CDisplayManager& m = Manager();
    int ok = 1;
    m.ChangePresentationInterval(true, 3, false);
    if (gCalls != 1 || !gDesc.PresentImmediate || gDesc.PresentImmediateIfSyncMissed || gDesc.VSyncCount != 3) ok = 0;
    m.ChangePresentationInterval(false, 1, true);
    if (gCalls != 2 || gDesc.PresentImmediate || !gDesc.PresentImmediateIfSyncMissed || gDesc.VSyncCount != 1) ok = 0;
    if (!ok) { printf("009bff10_TEST FAIL\n"); return 1; }
    printf("009bff10_TEST PASS\n");
    return 0;
}
