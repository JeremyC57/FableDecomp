#include "rebuild_abi.h"
#include <cstdio>

class CPatchTesselationEdgeStrip { public: char _pad[0x6c + 4]; };
void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* thisptr);

static int g_deletes = 0;
static int g_base = 0;

// spy on the MSVCR71 free thunk (0x00BFEA14) the source calls; it never releases (test objects live on the stack)
static int g_armed = 0;   // armed only around the call under test (CRT start-up/exit frees are ignored)
extern "C" void __cdecl free(void* p_) { void* p = (void*)p_; if (!g_armed) return; if (p) g_deletes++; }
void __fastcall CPatchTesselationEdgeStrip_base_dtor(CPatchTesselationEdgeStrip*) { g_base++; }

int main()
{
    CPatchTesselationEdgeStrip obj;
    char* b = (char*)&obj;
    for (unsigned i = 0; i < sizeof(obj); i++) b[i] = 0;

    int v = 1;
    *(void**)(b + 0x6c) = &v;
    *(void**)(b + 0x60) = 0;
    *(void**)(b + 0x54) = &v;
    *(void**)(b + 0x48) = 0;
    *(void**)(b + 0x3c) = &v;

    g_armed = 1; CPatchTesselationEdgeStrip_dtor(&obj); g_armed = 0;

    if (g_deletes == 3 && g_base == 1)
        printf("OK_458476\n");
    else
        printf("FAIL %d %d\n", g_deletes, g_base);
    return 0;
}