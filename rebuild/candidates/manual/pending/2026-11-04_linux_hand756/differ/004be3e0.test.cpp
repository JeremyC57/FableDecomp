// Behaviour test for CTCGraphicAppearance::IsResourcesLoaded (0x004BE3E0): a model of the row -- true only when every mesh the
// components use is loaded; with beginLoading the missing ones are started (and the answer is still false).
#include <stdio.h>
#include <vector>
struct Mesh { bool Loaded, Loading; };
static bool IsResourcesLoaded(std::vector<Mesh*>& meshes, bool beginLoading)
{
    bool all = true;
    for (size_t i = 0; i < meshes.size(); ++i)
        if (!meshes[i]->Loaded) { all = false; if (beginLoading) meshes[i]->Loading = true; else break; }
    return all;
}
int main()
{
    Mesh a = { true, false }, b = { false, false }, c = { false, false };
    std::vector<Mesh*> v; v.push_back(&a); v.push_back(&b); v.push_back(&c);
    int ok = !IsResourcesLoaded(v, false) && !b.Loading && !IsResourcesLoaded(v, true) && b.Loading && c.Loading;
    b.Loaded = c.Loaded = true; ok = ok && IsResourcesLoaded(v, false);
    printf(ok ? "004be3e0_TEST PASS\n" : "004be3e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
