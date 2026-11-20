#include <stdio.h>
#include <string.h>
// Behaviour test for 0x009F9F70 CTexture::Uninitialise (links the row object).
// NF139 (2026-11-20, q4296): rewritten on the shared headers the row now uses (engine/CTexture.h, engine/Direct3DTexture9.h); the
//   checks and the PassPattern are unchanged. The texture is an 8-byte block with PD3DTexture (+0) and the +4 bitfield word written in
//   place; the Direct3D texture is a fake object whose slot 2 (+8) is the Release fixture.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CTexture.h"

struct FakeTexture { void** vtable; };

static int g_ReleaseCalls = 0;
static FakeTexture* g_ReleasedTexture = 0;

static unsigned long __stdcall ReleaseTexture(FakeTexture* texture)
{
    ++g_ReleaseCalls;
    g_ReleasedTexture = texture;
    return 0;
}

static unsigned int g_storage[2];

int main()
{
    void* vtable[3] = {0, 0, (void*)&ReleaseTexture};
    FakeTexture texture = {vtable};
    FakeTexture* texturePointer = &texture;
    unsigned int flags = 0xF1234567u;
    memcpy(&g_storage[0], &texturePointer, 4);
    memcpy(&g_storage[1], &flags, 4);
    CTexture& owner = *(CTexture*)g_storage;

    owner.Uninitialise();
    if (
        g_storage[0] != 0 ||
        g_storage[1] != 0x01234567u ||
        g_ReleaseCalls != 1 ||
        g_ReleasedTexture != &texture)
    {
        return 1;
    }

    owner.Uninitialise();
    if (g_ReleaseCalls != 1)
    {
        return 1;
    }

    printf("FABLETLC_TEXTURE_UNINITIALISE_BEHAVIOR PASS\n");
    return 0;
}
