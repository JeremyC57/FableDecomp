#include <cstdio>
#include <cstring>
// Behaviour test for 0x00A26C50 CMeshDataBank::Close (links the row object): CloseMeshes runs first, then the CBankFileAsync
// base Close, both on the same object.
// NF176 (2026-11-20, q3886): rewritten on engine/CMeshDataBank.h, the header the row now uses (the old test was a standalone copy
//   over a local CMeshDataBank model and the h1 / h2 stand-ins). CloseMeshes and CBankFileAsync::Close are the test's fixtures;
//   the qualified call keeps the test off the (unset) vptr. PassPattern unchanged.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK_CTOR
#define FABLE_RETAIL_CMESHDATABANK_CLOSE
#include "fable_string.h"
#include "engine/CMeshDataBank.h"

static int g_seq = 0;
static const void* g_this[2] = { 0, 0 };
void CMeshDataBank::CloseMeshes() { g_seq = g_seq * 10 + 1; g_this[0] = this; }
void CBankFileAsync::Close() { g_seq = g_seq * 10 + 2; g_this[1] = this; }

int main()
{
    static double storage[(sizeof(CMeshDataBank) + 7) / 8];
    std::memset(storage, 0, sizeof storage);
    CMeshDataBank* bank = (CMeshDataBank*)storage;
    bank->CMeshDataBank::Close();
    if (g_seq != 12 || g_this[0] != (void*)storage || g_this[1] != (void*)storage) { std::printf("00a26c50_TEST FAIL\n"); return 1; }
    std::printf("00a26c50_TEST PASS\n");
    return 0;
}
