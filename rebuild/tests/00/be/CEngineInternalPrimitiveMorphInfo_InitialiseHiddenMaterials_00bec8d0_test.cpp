// Behaviour test for 0x00BEC8D0 CEngineInternalPrimitiveMorphInfo::InitialiseHiddenMaterials: size the hidden-flag
// array to the mesh's material count (new flags false) and set each flag to whether that material's name is in the
// hidden-name set.
#include <cstdio>
#include <cstring>

struct Material { const char* Name; };
struct Mesh { unsigned long MaterialCount; Material* Materials; };

struct NameSet {
    const char* names[8];
    unsigned int count;
    bool Contains(const char* n) const
    {
        for (unsigned int i = 0; i < count; ++i)
            if (std::strcmp(names[i], n) == 0) return true;
        return false;
    }
};

struct MorphInfo {
    NameSet HiddenMaterialNames;
    unsigned char HiddenMaterials[16];
    unsigned int  HiddenCount;
    void resize(unsigned int n, unsigned char x)
    {
        for (unsigned int i = HiddenCount; i < n; ++i) HiddenMaterials[i] = x;
        HiddenCount = n;
    }
    void InitialiseHiddenMaterials(const Mesh& mesh)
    {
        unsigned long count = mesh.MaterialCount;
        resize(count, false);
        for (unsigned long i = 0; i < count; ++i) {
            bool hidden = false;
            if (HiddenMaterialNames.Contains(mesh.Materials[i].Name))
                hidden = true;
            HiddenMaterials[i] = hidden;
        }
    }
};

int main()
{
    static MorphInfo info;
    info.HiddenMaterialNames.count = 2;
    info.HiddenMaterialNames.names[0] = "cloak";
    info.HiddenMaterialNames.names[1] = "hat";
    info.HiddenCount = 0;
    Material mats[4] = { { "body" }, { "hat" }, { "boots" }, { "cloak" } };
    Mesh mesh = { 4, mats };
    int ok = 1;
    info.InitialiseHiddenMaterials(mesh);
    if (info.HiddenCount != 4) ok = 0;
    if (info.HiddenMaterials[0] != 0 || info.HiddenMaterials[1] != 1 || info.HiddenMaterials[2] != 0 || info.HiddenMaterials[3] != 1) ok = 0;
    info.HiddenMaterialNames.count = 0;                      // nothing hidden any more: flags rewritten
    info.InitialiseHiddenMaterials(mesh);
    for (unsigned int i = 0; i < 4; ++i) if (info.HiddenMaterials[i] != 0) ok = 0;
    std::printf(ok ? "00bec8d0_TEST PASS\n" : "00bec8d0_TEST FAIL\n");
    return ok ? 0 : 1;
}