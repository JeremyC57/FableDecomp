// Behaviour test for 0x00B3AFF0 CEngineResourceManager::OnRestoreDefaultPoolResources (links the REAL row object). NF172 (q5133):
// rewritten for the shared-header row (the old test modelled the generated CBaseIntelligentPointer stand-in). The manager is raw
// storage (GPUScratchBufferTextureManager at +0x3190); the private override is reached through a linker alias (thiscall = fastcall
// with a spare edx), and CGPUScratchBufferTextureManager::EndShutdown 0x00F36D30 is a recording stub aliased the same way.
#include <stdio.h>
#include <string.h>

extern "C" bool __fastcall CallOnRestore(void* self, int spare);
#pragma comment(linker, "/alternatename:@CallOnRestore@8=?OnRestoreDefaultPoolResources@CEngineResourceManager@@EAE_NXZ")
#pragma comment(linker, "/alternatename:?EndShutdown@CGPUScratchBufferTextureManager@@QAE_NXZ=@StubEndShutdown@8")

static const void* gObserved;
static bool gResult;
static int gCalls;

extern "C" bool __fastcall StubEndShutdown(void* self, int)
{
    gObserved = self;
    ++gCalls;
    return gResult;
}

static unsigned char gManager[0x3200];

int main()
{
    memset(gManager, 0, sizeof(gManager));
    gResult = true;
    if (!CallOnRestore(gManager, 0) || gCalls != 1 || gObserved != gManager + 0x3190)
    {
        printf("00b3aff0_TEST FAIL\n");
        return 1;
    }
    gResult = false;
    if (CallOnRestore(gManager, 0) || gCalls != 2)
    {
        printf("00b3aff0_TEST FAIL\n");
        return 2;
    }
    printf("00b3aff0_TEST PASS\n");
    return 0;
}
