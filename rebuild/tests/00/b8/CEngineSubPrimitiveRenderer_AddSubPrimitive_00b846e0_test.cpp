// Behaviour test for 0x00B846E0 CEngineSubPrimitiveRenderer::AddSubPrimitive (five arguments; links the REAL row object). NF172 (q4824):
// rewritten for the shared-header row (the old test carried a host copy of the void* / SSortKey stand-in). The renderer is raw storage;
// the member is reached through a linker alias (thiscall = fastcall with a spare edx), and the six-argument overload 0x00B84580 is a
// recording stub aliased the same way (it must receive the five arguments unchanged and an empty CEngineSubPrimitiveChain by reference).
#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CallAdd5(void* self, int spare, void* sub, float distance, unsigned char flags, unsigned long key, void* manager);
#pragma comment(linker, "/alternatename:@CallAdd5@28=?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@@Z")
#pragma comment(linker, "/alternatename:?AddSubPrimitive@CEngineSubPrimitiveRenderer@@QAEXPAVCEngineSubPrimitiveBase@@MEKPAVCEnginePrimitiveManager@@AAVCEngineSubPrimitiveChain@@@Z=@StubAdd6@32")

static void* gSelf; static void* gSub; static float gDistance; static unsigned char gFlags; static unsigned long gKey; static void* gManager;
static void* gChain[2]; static int gCalls;

extern "C" void __fastcall StubAdd6(void* self, int, void* sub, float distance, unsigned char flags, unsigned long key, void* manager, void** chain)
{
    gSelf = self; gSub = sub; gDistance = distance; gFlags = flags; gKey = key; gManager = manager;
    gChain[0] = chain[0]; gChain[1] = chain[1];
    ++gCalls;
}

static unsigned char gRenderer[0x40];

int main()
{
    int sub, manager;
    gChain[0] = gChain[1] = &sub;
    CallAdd5(gRenderer, 0, &sub, 12.5f, 3, 0x1234, &manager);
    if (gCalls != 1 || gSelf != gRenderer || gSub != &sub || gDistance != 12.5f || gFlags != 3 || gKey != 0x1234 || gManager != &manager ||
        gChain[0] != 0 || gChain[1] != 0)
    {
        printf("00b846e0_TEST FAIL\n");
        return 1;
    }
    printf("00b846e0_TEST PASS\n");
    return 0;
}
