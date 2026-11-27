// 00886e80 NEnvironmentInternal::CBlendedEnvironmentTheme::CompactTextureSet
// Leaf: ?CompactTextureSet@CBlendedEnvironmentTheme@NEnvironmentInternal@@QBEXAAUCIDrawEnvironmentTextureSet@@J@Z
// Flags: none
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22
#define FABLE_RETAIL_CMATRIX3X4_TRANSPOSE
#include "engine/CMatrix3x4.h"
#include "engine/CBlendedEnvironmentTheme.h"
#include "fable_assert.h"

void NEnvironmentInternal::CBlendedEnvironmentTheme::CompactTextureSet(CIDrawEnvironmentTextureSet& tset, long max_count) const
{
    long src_count = tset.Textures.size();
    FABLE_RETAIL_ASSERT(max_count >= 0);
    FABLE_RETAIL_ASSERT(src_count >= max_count);
    for (long n = 0; n < src_count; ++n) {
        for (long n2 = n + 1; n2 < src_count; ++n2) {
            if (tset.Textures[n2] == tset.Textures[n]) {
                tset.Blends[n] += tset.Blends[n2];
                tset.Blends[n2] = 0.0f;
            }
        }
    }
    bool sorting = true;
    while (sorting) {
        sorting = false;
        for (long n = 0; n < src_count - 1; ++n) {
            if (tset.Blends[n] < tset.Blends[n + 1]) {
                sorting = true;
                SWAP(tset.Blends[n], tset.Blends[n + 1]);
                SWAP(tset.Textures[n], tset.Textures[n + 1]);
            }
        }
    }
    for (long n = 0; n < max_count; ++n)
        tset.Blends[n] -= tset.Blends[max_count];
    float total_blends = 0.0f;
    for (long n = 0; n < max_count; ++n)
        total_blends += tset.Blends[n];
    if (total_blends < 0.00390625f) {
        for (long n = 0; n < max_count; ++n)
            tset.Blends[n] = (float)(1.0f / max_count);
    } else {
        for (long n = 0; n < max_count; ++n)
            tset.Blends[n] /= total_blends;
    }
    tset.Textures.resize(max_count);
    tset.Blends.resize(max_count);
}