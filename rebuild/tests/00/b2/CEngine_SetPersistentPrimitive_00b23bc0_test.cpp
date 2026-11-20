// Behaviour test for 0x00B23BC0 CEngine::SetPersistentPrimitive (links the REAL row object). NF172 (q11904 / q11905): rewritten for the
// shared-header row (the old test carried a host copy of the free CEngine_SetPersistentPrimitive stand-in); the catalog PassPattern is kept.
// The engine and the primitive renderer are raw storage; the override is reached through a linker alias (thiscall = fastcall with a spare
// edx), CEnginePrimitiveRenderer::SetPersistentPrimitive 0x00B324A0 is a recording stub aliased the same way, and the component pointer
// NEngineInternal::GPPrimitiveRenderer 0x01436E80 is aliased to a test variable.
#include <stdio.h>

extern "C" void __fastcall CallSetPersistentPrimitive(void* self, int spare, void* handle, const void* primitive, unsigned long flags);
#pragma comment(linker, "/alternatename:@CallSetPersistentPrimitive@20=?SetPersistentPrimitive@CEngine@@UAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z")
#pragma comment(linker, "/alternatename:?SetPersistentPrimitive@CEnginePrimitiveRenderer@@QAEXAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K_N@Z=@StubSetPersistentPrimitive@24")
#pragma comment(linker, "/alternatename:?GPPrimitiveRenderer@NEngineInternal@@3PAVCEnginePrimitiveRenderer@@A=_gRendererPointer")

static unsigned char gRenderer[0x40];
extern "C" { void* gRendererPointer = gRenderer; }
static void* gSelf; static void* gHandle; static const void* gPrimitive; static unsigned long gFlags; static int gNonPersistent = -1;
static int gCalls;

extern "C" void __fastcall StubSetPersistentPrimitive(void* self, int, void* handle, const void* primitive, unsigned long flags, bool nonPersistent)
{
    gSelf = self; gHandle = handle; gPrimitive = primitive; gFlags = flags; gNonPersistent = nonPersistent ? 1 : 0;
    ++gCalls;
}

int main()
{
    static unsigned char engine[0x100];
    int handle = 7, primitive = 9;
    CallSetPersistentPrimitive(engine, 0, &handle, &primitive, 12345);
    if (gCalls == 1 && gSelf == gRenderer && gHandle == &handle && gPrimitive == &primitive && gFlags == 12345 && gNonPersistent == 0)
        printf("PARITY_OK\n");
    return 0;
}
