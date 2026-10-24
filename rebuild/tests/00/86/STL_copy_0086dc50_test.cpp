// Behaviour test for 0x0086DC50: STLport __copy assigns [first, last) onto result element by element (front to back)
// and returns result + (last - first); an empty range returns result.
#include <cstdio>
#include <vector>

struct CTextureMorph { unsigned int NewTexture; unsigned int GroupID; };
struct STextureMorphMap { std::vector<CTextureMorph> Vector; bool Dirty; };
struct CBodyPartMesh { int MeshIndex; int Priority; STextureMorphMap TextureMorphs; };

static CBodyPartMesh* copy_model(CBodyPartMesh* first, CBodyPartMesh* last, CBodyPartMesh* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}

int main()
{
    int fails = 0;
    CBodyPartMesh a[4];
    for (int i = 0; i < 4; ++i)
    {
        a[i].MeshIndex = i; a[i].Priority = 10 * i; a[i].TextureMorphs.Dirty = (i & 1) != 0;
        for (int j = 0; j < i; ++j) { CTextureMorph t = { (unsigned int)j, (unsigned int)i }; a[i].TextureMorphs.Vector.push_back(t); }
    }
    CBodyPartMesh b[4];
    for (int i = 0; i < 4; ++i) { b[i].MeshIndex = -1; b[i].Priority = -1; b[i].TextureMorphs.Dirty = false; }
    if (copy_model(a + 1, a + 4, b) != b + 3) ++fails;
    for (int i = 0; i < 3; ++i)
        if (b[i].MeshIndex != i + 1 || b[i].Priority != 10 * (i + 1) || b[i].TextureMorphs.Vector.size() != (unsigned int)(i + 1) || b[i].TextureMorphs.Dirty != (((i + 1) & 1) != 0)) ++fails;
    if (b[3].MeshIndex != -1) ++fails;
    if (copy_model(a, a, b) != b) ++fails;
    // overlapping shift left (the erase use): front-to-back assignment is safe
    copy_model(a + 1, a + 4, a);
    if (a[0].MeshIndex != 1 || a[2].MeshIndex != 3 || a[2].TextureMorphs.Vector.size() != 3u) ++fails;
    if (fails) { std::printf("0086dc50_TEST FAIL\n"); return 1; }
    std::printf("0086dc50_TEST PASS\n");
    return 0;
}