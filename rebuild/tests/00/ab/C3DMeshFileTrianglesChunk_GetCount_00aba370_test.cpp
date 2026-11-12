// Behaviour test for 0x00ABA370 C3DMeshFileTrianglesChunk::GetCount() const: the triangle count is (end - begin) / 6 over the
// TriangleList CArray<C3DTriangle2> at +0x2C / +0x30 (name-fix 2026-11-12: was the stand-in CWorldMap_DrawGetNoMaps). The object
// is a raw 0x38-byte block (GetCount is non-virtual; the out-of-line constructor is not linked).
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK_ACCESSORS
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "engine/C3DTriangle2.h"
#include "engine/C3DMeshFileTrianglesChunk.h"

static unsigned long Count(char* block, char* begin, char* end)
{
    std::memcpy(block + 0x2C, &begin, 4);
    std::memcpy(block + 0x30, &end, 4);
    return reinterpret_cast<const C3DMeshFileTrianglesChunk*>(block)->GetCount();
}

int main()
{
    static char block[0x38];
    static char tris[10 * 6];
    if (Count(block, tris, tris + 7 * 6) != 7) { std::printf("00aba370_TEST FAIL 1\n"); return 1; }
    if (Count(block, tris, tris) != 0) { std::printf("00aba370_TEST FAIL 2\n"); return 1; }
    if (Count(block, tris, tris + 10 * 6) != 10) { std::printf("00aba370_TEST FAIL 3\n"); return 1; }
    std::printf("00aba370_TEST PASS\n");
    return 0;
}
