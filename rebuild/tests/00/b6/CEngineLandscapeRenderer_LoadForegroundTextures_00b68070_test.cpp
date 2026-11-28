#define FABLE_PORTABLE
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include "rebuild_abi.h"
#define FABLE_RETAIL_MESH_MIPMAP_PASS30
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFGetTopBitInByte.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

unsigned char GTopBitInByteTable[256];

struct TestFrame
{
    long Width;
    long Height;
    bool IsLoading;
    long LastMip;
    unsigned long LastFlags;
};

struct TestLandscape
{
    float MaxPixelsPerTexel;
    float CameraScale;
    float WindowHeight;
    float NearZ;
    TestFrame Frames[3];
    unsigned long TexturesUsedThisFrame[3];
    float TextureIDToMinPatchDistanceSquared[3];
    unsigned char CurrentTextureLODLevels[3];
    unsigned int UsedCount;

    bool LoadForegroundTextures(bool loading_new_scene)
    {
        float base_texture_lod_factor = MaxPixelsPerTexel / 8.0f / CameraScale / WindowHeight;
        bool loading = false;
        unsigned long get_flags = loading_new_scene ? 16 : 0;
        for (unsigned int n = 0; n < UsedCount; ++n) {
            long texture = TexturesUsedThisFrame[n];
            float distance = (float)sqrt(TextureIDToMinPatchDistanceSquared[texture]);
            float texture_lod_factor = 0;
            if (distance > NearZ)
                texture_lod_factor = base_texture_lod_factor * distance;
            TestFrame& frame = Frames[texture];
            long texture_size = frame.Width > frame.Height ? frame.Width : frame.Height;
            long ratio = GFFloatToLongFloor(texture_size * texture_lod_factor) > 255
                ? 255 : GFFloatToLongFloor(texture_size * texture_lod_factor);
            long lod = ratio > 0 ? GFGetTopBitInByte((unsigned char)ratio) : 0;
            frame.LastMip = lod;
            frame.LastFlags = get_flags;
            if (loading_new_scene && frame.IsLoading)
                loading = true;
            CurrentTextureLODLevels[texture] = (unsigned char)(lod < 0 ? 0 : lod);
            TextureIDToMinPatchDistanceSquared[texture] = -1.0f;
        }
        UsedCount = 0;
        return loading;
    }
};

int main()
{
    for (unsigned int i = 1; i < 256; ++i) {
        unsigned int value = i;
        unsigned char top = 0;
        while ((value >>= 1) != 0)
            ++top;
        GTopBitInByteTable[i] = top;
    }

    TestLandscape fixture = TestLandscape();
    fixture.MaxPixelsPerTexel = 8.0f;
    fixture.CameraScale = 1.0f;
    fixture.WindowHeight = 4.0f;
    fixture.NearZ = 2.0f;
    fixture.Frames[0].Width = 2;
    fixture.Frames[0].Height = 20;
    fixture.Frames[1].Width = 16;
    fixture.Frames[1].Height = 2;
    fixture.Frames[2].Width = 256;
    fixture.Frames[2].Height = 512;
    fixture.Frames[1].IsLoading = true;
    fixture.Frames[2].IsLoading = true;
    fixture.TexturesUsedThisFrame[0] = 2;
    fixture.TexturesUsedThisFrame[1] = 0;
    fixture.TextureIDToMinPatchDistanceSquared[0] = 4.0f;
    fixture.TextureIDToMinPatchDistanceSquared[1] = 25.0f;
    fixture.TextureIDToMinPatchDistanceSquared[2] = 100.0f;
    assert(!fixture.LoadForegroundTextures(true)); // empty
    fixture.UsedCount = 2;
    assert(!fixture.LoadForegroundTextures(false)); // pending frames excluded from per-frame return
    assert(fixture.UsedCount == 0 && fixture.Frames[2].LastFlags == 0);
    assert(fixture.Frames[0].LastMip == 0 && fixture.Frames[2].LastMip == 7); // near boundary / clamp
    assert(fixture.CurrentTextureLODLevels[0] == 0 && fixture.CurrentTextureLODLevels[2] == 7);
    assert(fixture.TextureIDToMinPatchDistanceSquared[0] == -1.0f);
    assert(fixture.TextureIDToMinPatchDistanceSquared[2] == -1.0f);
    assert(fixture.TextureIDToMinPatchDistanceSquared[1] == 25.0f); // untouched texture

    fixture.TexturesUsedThisFrame[0] = 1;
    fixture.UsedCount = 1;
    assert(fixture.LoadForegroundTextures(true)); // pending scene load
    assert(fixture.Frames[1].LastMip == 4 && fixture.Frames[1].LastFlags == 16);
    assert(fixture.CurrentTextureLODLevels[1] == 4 && fixture.UsedCount == 0);

    fixture.Frames[1].IsLoading = false;
    fixture.TextureIDToMinPatchDistanceSquared[1] = 9.0f;
    fixture.UsedCount = 1;
    assert(!fixture.LoadForegroundTextures(true)); // finished scene load
    assert(fixture.Frames[1].LastMip == 3);
    puts("00b68070_TEST PASS");
}