// Behaviour test for QuadricOptimiserInternals::COptimisedVertex::RemapGroups 0x00AC97D0: when any of the vertex's group ids is in
// the remap table, every group id is remapped (unmapped ids kept) and the weights of ids that collapse together are summed;
// when no id is remapped the groups are left untouched.
#include <cstdio>
#include <map>

struct COptimisedVertex {
    std::map<unsigned long, float> Groups;
    void RemapGroups(const std::map<unsigned long, unsigned long>& remap)
    {
        bool anyRemapped = false;
        std::map<unsigned long, float>::iterator it;
        for (it = Groups.begin(); it != Groups.end(); ++it) {
            if (remap.find(it->first) != remap.end()) {
                anyRemapped = true;
                break;
            }
        }
        if (!anyRemapped)
            return;
        std::map<unsigned long, float> newGroups;
        for (it = Groups.begin(); it != Groups.end(); ++it) {
            unsigned long group = it->first;
            float weight = it->second;
            std::map<unsigned long, unsigned long>::const_iterator remapped = remap.find(group);
            if (remapped != remap.end())
                group = remapped->second;
            newGroups.insert(std::map<unsigned long, float>::value_type(group, 0.0f));
            newGroups[group] += weight;
        }
        Groups = newGroups;
    }
};

int main()
{
    int fails = 0;
    {
        COptimisedVertex v;
        v.Groups[3] = 0.25f; v.Groups[5] = 0.5f; v.Groups[9] = 0.25f;
        std::map<unsigned long, unsigned long> remap;
        remap[3] = 7; remap[9] = 7;               // 3 and 9 collapse into 7, 5 stays
        v.RemapGroups(remap);
        if (v.Groups.size() != 2) ++fails;
        if (v.Groups.count(7) != 1 || v.Groups[7] != 0.5f) ++fails;
        if (v.Groups.count(5) != 1 || v.Groups[5] != 0.5f) ++fails;
        if (v.Groups.count(3) != 0 || v.Groups.count(9) != 0) ++fails;
    }
    {
        COptimisedVertex v;
        v.Groups[1] = 1.0f; v.Groups[2] = 2.0f;
        std::map<unsigned long, unsigned long> remap;
        remap[10] = 11;                           // nothing of the vertex is remapped
        v.RemapGroups(remap);
        if (v.Groups.size() != 2 || v.Groups[1] != 1.0f || v.Groups[2] != 2.0f) ++fails;
    }
    {
        COptimisedVertex v;
        v.Groups[4] = 0.75f;
        std::map<unsigned long, unsigned long> remap;
        remap[4] = 4;                             // identity remap keeps the weight
        v.RemapGroups(remap);
        if (v.Groups.size() != 1 || v.Groups[4] != 0.75f) ++fails;
    }
    if (fails == 0) printf("00ac97d0_TEST PASS\n"); else printf("00ac97d0_TEST FAIL %d\n", fails);
    return fails != 0;
}