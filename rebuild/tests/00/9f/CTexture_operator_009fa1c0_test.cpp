#include <stdio.h>
#include <string.h>
// Behaviour test for 0x009FA1C0 CTexture::operator= (links the row object).
// NF139 (2026-11-20, q4659): rewritten on the shared headers the row now uses (engine/CTexture.h, engine/Direct3DTexture9.h); the
//   checks and the PassPattern are unchanged. The textures live in zeroed 8-byte blocks (no constructor or inline destructor runs);
//   the Direct3D textures are fake objects whose slots 1 / 2 (+4 / +8) are the AddRef / Release fixtures.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CTexture.h"

struct FakeTexture { void** vtable; };

static int g_AddRefCalls;
static int g_ReleaseCalls;
static FakeTexture* g_AddedTexture;
static FakeTexture* g_ReleasedTexture;

static unsigned long __stdcall AddTextureReference(FakeTexture* texture)
{
    ++g_AddRefCalls;
    g_AddedTexture = texture;
    return 2;
}

static unsigned long __stdcall ReleaseTextureReference(FakeTexture* texture)
{
    ++g_ReleaseCalls;
    g_ReleasedTexture = texture;
    return 0;
}

static unsigned int g_blocks[4][2];

static CTexture& Block(int index)
{
    memset(g_blocks[index], 0, sizeof g_blocks[index]);
    return *(CTexture*)g_blocks[index];
}

static IDirect3DTexture9* AsD3D(FakeTexture* texture) { return (IDirect3DTexture9*)texture; }

int main()
{
    void* textureVtable[3] = {0, (void*)&AddTextureReference, (void*)&ReleaseTextureReference};
    FakeTexture oldTexture = {textureVtable};
    FakeTexture newTexture = {textureVtable};

    CTexture& destination = Block(0);
    destination.PD3DTexture = AsD3D(&oldTexture);
    destination.ByteLength = 0x00123456;
    destination.AllocationSource = 0xA;

    CTexture& source = Block(1);
    source.PD3DTexture = AsD3D(&newTexture);
    source.ByteLength = 0x07654321;
    source.AllocationSource = 0x3;

    const CTexture* result = &(destination = source);
    if (result != &destination ||
        destination.PD3DTexture != AsD3D(&newTexture) ||
        destination.ByteLength != 0x07654321 ||
        destination.AllocationSource != 0x3 ||
        source.PD3DTexture != AsD3D(&newTexture) ||
        source.ByteLength != 0x07654321 ||
        source.AllocationSource != 0x3 ||
        g_ReleaseCalls != 1 ||
        g_ReleasedTexture != &oldTexture ||
        g_AddRefCalls != 1 ||
        g_AddedTexture != &newTexture)
    {
        printf("FABLETLC_TEXTURE_ASSIGNMENT_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    CTexture& emptySource = Block(2);
    emptySource.ByteLength = 0x000ABCDE;
    emptySource.AllocationSource = 0x7;
    destination = emptySource;
    if (destination.PD3DTexture != 0 ||
        destination.ByteLength != 0x000ABCDE ||
        destination.AllocationSource != 0x7 ||
        g_ReleaseCalls != 2 ||
        g_ReleasedTexture != &newTexture ||
        g_AddRefCalls != 1)
    {
        printf("FABLETLC_TEXTURE_ASSIGNMENT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    CTexture& self = Block(3);
    self.PD3DTexture = AsD3D(&oldTexture);
    self.ByteLength = 0x00013579;
    self.AllocationSource = 0xB;
    self = self;
    if (self.PD3DTexture != 0 ||
        self.ByteLength != 0x00013579 ||
        self.AllocationSource != 0 ||
        g_ReleaseCalls != 3 ||
        g_ReleasedTexture != &oldTexture ||
        g_AddRefCalls != 1)
    {
        printf("FABLETLC_TEXTURE_ASSIGNMENT_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_TEXTURE_ASSIGNMENT_BEHAVIOR PASS\n");
    return 0;
}
