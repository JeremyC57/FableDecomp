// Behaviour test for 0x00BE0400 CEngineStaticMapFileBlockManager::ReleaseRequest (links the REAL row object). NF172 (q4395): rewritten
// for the shared-header row (the old test carried a host copy of the CRetailBlockSet stand-in). The manager is raw storage (Blocks at
// +8); the member is reached through a linker alias (thiscall = fastcall with a spare edx), and the STLport tree find<long> 0x00BE08E0
// is a recording stub aliased the same way (hidden iterator result first, then the key by reference). Node value +0x10 is the key,
// CFileBlock::RefCount +0x14.
#include <stdio.h>
#include <string.h>

extern "C" void __fastcall CallReleaseRequest(void* self, int spare, long block);
#pragma comment(linker, "/alternatename:@CallReleaseRequest@12=?ReleaseRequest@CEngineStaticMapFileBlockManager@@QAEXJ@Z")
#pragma comment(linker, "/alternatename:??$find@J@?$_Rb_tree@JU?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@U?$_Select1st@U?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@@2@@1@ABJ@Z=@StubFind@16")

static unsigned char gManager[0x14];
static unsigned char gNode[0x20];
static const void* gTree;
static long gKey;
static int gCalls;

extern "C" void* __fastcall StubFind(void* self, int, void** result, const long* key)
{
    gTree = self;
    gKey = *key;
    ++gCalls;
    *result = gNode;
    return result;
}

int main()
{
    long refCount = 3;
    memset(gManager, 0, sizeof(gManager));
    memcpy(gNode + 0x14, &refCount, sizeof(refCount));
    CallReleaseRequest(gManager, 0, 5);
    memcpy(&refCount, gNode + 0x14, sizeof(refCount));
    if (gCalls != 1 || gKey != 5 || gTree != gManager + 8 || refCount != 2)
    {
        printf("00be0400_TEST FAIL\n");
        return 1;
    }
    printf("00be0400_TEST PASS\n");
    return 0;
}
