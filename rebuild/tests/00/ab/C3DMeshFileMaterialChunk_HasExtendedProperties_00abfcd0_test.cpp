// Behaviour test for 0x00ABFCD0 C3DMeshFileMaterialChunk::HasExtendedProperties: looks up "MTLE" (retail 0x0129EDCC) and returns lookup != 0.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include "engine/C3DMeshFileMaterialChunk.h"
#include "rebuild_abi.h"
#include <cstdio>
#include <cstring>
struct C3DMeshFileMaterialChunk_Methods : C3DMeshFileMaterialChunk {
    bool HasExtendedProperties();
};
static const char* g_name = 0; static int g_calls = 0; static int g_result = 0;
int FABLE_STDCALL FableHasExtHelper_abfcd0(const char *name) { g_name = name; ++g_calls; return g_result; }
static char g_obj[0x200];
int main()
{
    C3DMeshFileMaterialChunk_Methods* c = (C3DMeshFileMaterialChunk_Methods*)g_obj;
    g_result = 0;
    if (c->HasExtendedProperties() || g_calls != 1 || g_name == 0 || std::strcmp(g_name, "MTLE") != 0) { std::printf("BAD0\n"); return 1; }
    g_result = 0x1234;
    if (!c->HasExtendedProperties() || g_calls != 2 || std::strcmp(g_name, "MTLE") != 0) { std::printf("BAD1\n"); return 1; }
    std::printf("HASEXT_OK\n"); return 0;
}
