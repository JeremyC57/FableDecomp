// Behaviour test for 0x00A35B30 C3DMeshObject::GetAllDummyObjectNames (NF217, 2026-11-21, q12368): rewritten on the shared engine/C3DMeshObject.h and
//   engine/C3DMeshStats.h (the July test modelled the stand-in ?fn_00a35b30@@YIXPAUBL@@HH@Z locally). Links the row object: the row's
//   C3DMeshObject::GetAllDummyObjectNames must forward the counted pointer's Data (Mesh, +8) as `this`, the names (only) and sort = false to the
//   C3DMeshStats query of the same name, stubbed here.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_C3DMESHOBJECT_GETNAMES
#define FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES
#include "engine/C3DMeshObject.h"
#include "engine/C3DMeshStats.h"

static const void* g_this = 0; static const void* g_names = 0; static const void* g_prefix = 0; static int g_sort = -1; static int g_calls = 0;
void C3DMeshStats::GetAllDummyObjectNames(std::vector<CCharString>& names, bool sort) const
{
    g_this = this; g_names = &names; g_sort = sort ? 1 : 0; ++g_calls;
}

int main()
{
    static double object_words[sizeof(C3DMeshObject) / sizeof(double) + 1];
    static double stats_words[4], names_words[4], prefix_words[4];
    void* stats = stats_words;
    std::memcpy(reinterpret_cast<char*>(object_words) + 8, &stats, sizeof(stats));   // Mesh.Data (+8)
    const C3DMeshObject* object = reinterpret_cast<const C3DMeshObject*>(object_words);
    std::vector<CCharString>& names = *reinterpret_cast<std::vector<CCharString>*>(names_words);
    object->GetAllDummyObjectNames(names);
    if (g_calls != 1 || g_this != stats || g_names != &names || g_sort != 0) { std::printf("00a35b30_TEST FAIL\n"); return 1; }
    std::printf("00a35b30_TEST PASS\n");
    return 0;
}
