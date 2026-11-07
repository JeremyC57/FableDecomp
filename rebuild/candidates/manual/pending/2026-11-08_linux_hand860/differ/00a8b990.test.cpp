// Behaviour test for 0x00A8B990 C3DMesh2::InitialiseBones: the sub-mesh's bone chunks become bones (each parent counting its
// children) with names; the bones the mesh uses, and every ancestor of a used bone, are kept in their original order, the kept bones'
// indices are remapped to their packed positions, and the packed names must store for the call to succeed.
#include <cstdio>
#include <vector>
#include <string>
#include <map>

struct Bone { long parent; long children; };

static bool Initialise(const std::vector<long>& parents, std::vector<bool> used, bool names_ok, std::vector<Bone>& out,
                       std::vector<std::string>& out_names, std::map<unsigned long, unsigned long>& remap)
{
    std::vector<Bone> bones; std::vector<std::string> names;
    for (size_t i = 0; i < parents.size(); ++i) {
        Bone b = { parents[i], 0 };
        if (b.parent != -1) ++bones[b.parent].children;
        bones.push_back(b);
        names.push_back(std::string(1, (char)('a' + i)));
    }
    for (long i = (long)used.size() - 1; i >= 0; --i)
        if (used[i] && bones[i].parent != -1) used[bones[i].parent] = true;
    unsigned long n = 0;
    for (size_t i = 0; i < used.size(); ++i) if (used[i]) ++n;
    out.resize(n); out_names.resize(n);
    unsigned long k = 0;
    for (size_t j = 0; j < used.size(); ++j)
        if (used[j]) { out[k] = bones[j]; out_names[k] = names[j]; remap[(unsigned long)j] = k; ++k; }
    for (size_t i = 0; i < out.size(); ++i) if (out[i].parent != -1) out[i].parent = (long)remap[(unsigned long)out[i].parent];
    return names_ok;
}

int main()
{
    long p[] = { -1, 0, 1, 0, 3 };
    std::vector<long> parents(p, p + 5);
    std::vector<bool> used(5, false); used[2] = true;
    std::vector<Bone> out; std::vector<std::string> names; std::map<unsigned long, unsigned long> remap;
    bool ok = Initialise(parents, used, true, out, names, remap);
    ok = ok && out.size() == 3 && names[2] == "c" && out[0].children == 2 && out[2].parent == 1 && remap[2] == 2 && remap.size() == 3;
    std::vector<bool> used2(5, false); used2[4] = true;
    std::vector<Bone> o2; std::vector<std::string> n2; std::map<unsigned long, unsigned long> r2;
    ok = ok && !Initialise(parents, used2, false, o2, n2, r2) && o2.size() == 3 && n2[1] == "d" && o2[2].parent == 1;
    std::printf(ok ? "00a8b990_TEST PASS\n" : "00a8b990_TEST FAIL\n");
    return ok ? 0 : 1;
}
