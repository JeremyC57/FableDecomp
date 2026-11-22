// Test for 0x00742790 NCreatureMode::CModeSitBench::Alloc(CThing&) on the shared header (NF238): links the row object.
// The out-of-line constructor 0x00740040 is stubbed through a test-only /alternatename onto a __fastcall stub (thiscall
// ecx = this, one stack argument, ret 4 = __fastcall ecx, edx, one stack argument). operator new is replaced so the null
// path (allocation failure) is a case of its own.
#include <stdio.h>
#include <stdlib.h>
#include <new>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_ALLOC
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/NCreatureMode_CModeSitFloor.h"

static int g_ctorCalls = 0;
static void* g_ctorThis = 0;
static void* g_ctorOwner = 0;
static int g_failNew = 0;
static size_t g_lastSize = 0;
static unsigned char g_block[0x40];

void* __cdecl operator new(size_t size)
{
    g_lastSize = size;
    if (g_failNew) return 0;
    return g_block;
}
void __cdecl operator delete(void*) { }

extern "C" void* __fastcall StubCModeSitBenchCtor(void* self, int, void* owner)
{
    ++g_ctorCalls; g_ctorThis = self; g_ctorOwner = owner;
    return self;
}
#pragma comment(linker, "/alternatename:??0CModeSitBench@NCreatureMode@@QAE@AAVCThing@@@Z=@StubCModeSitBenchCtor@12")

static int Fail(int n) { printf("00742790_TEST FAIL case %d\n", n); return 1; }

int main()
{
    unsigned char ownerBuf[0x200];
    CThing& owner = *reinterpret_cast<CThing*>(ownerBuf);

    // Case 1: allocation succeeds -> 0x38 bytes, constructor runs once on the block with the owner, the block is returned.
    CTCCreatureModeManager::CModeBase* mode = NCreatureMode::CModeSitBench::Alloc(owner);
    if (g_lastSize != 0x38) return Fail(1);
    if (g_ctorCalls != 1 || g_ctorThis != g_block || g_ctorOwner != ownerBuf) return Fail(2);
    if (reinterpret_cast<void*>(mode) != g_block) return Fail(3);

    // Case 2: allocation fails -> null, the constructor is not called.
    g_failNew = 1; g_ctorCalls = 0;
    mode = NCreatureMode::CModeSitBench::Alloc(owner);
    if (mode != 0 || g_ctorCalls != 0 || g_lastSize != 0x38) return Fail(4);

    printf("00742790_TEST PASS\n");
    return 0;
}
