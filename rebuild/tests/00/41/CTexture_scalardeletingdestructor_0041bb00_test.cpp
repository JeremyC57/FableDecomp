#include <stdio.h>
#include <stdlib.h>
// Behaviour test for 0x0041BB00 CTexture::`scalar deleting destructor' (the compiler-generated ??_GCTexture@@QAEPAXI@Z).
// NF142 (2026-11-20, q4296): rewritten on engine/CTexture.h, the header the row now uses (the old test only printed its pattern). A delete
//   of a CTexture runs the scalar deleting destructor (a pick-any COMDAT, the same one the row emits): the inline destructor calls
//   Uninitialise on the object, then operator delete frees it. PassPattern unchanged.
#define FABLE_RETAIL_CTEXTURE
#include "fable_compiler.h"
#include "engine/CTexture.h"

static int g_uninitialised = 0;
static int g_freed = 0;
static const void* g_last_uninitialised = 0;
static const void* g_last_freed = 0;

void CTexture::Uninitialise() { g_last_uninitialised = this; ++g_uninitialised; }

void operator delete(void* p)
{
    ++g_freed;
    g_last_freed = p;
    free(p);
}

int main(void)
{
    CTexture* texture = (CTexture*)malloc(sizeof(CTexture));
    delete texture;
    if (g_uninitialised == 1 && g_freed == 1 && g_last_uninitialised == texture && g_last_freed == texture) {
        printf("PASS_0041bb00\n");
        return 0;
    }
    printf("FAIL u=%d f=%d\n", g_uninitialised, g_freed);
    return 1;
}
