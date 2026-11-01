// Behaviour test for 00aca5f0: QuadricOptimiserInternals::COptimisedVertex::LimitGroupCount -- keeps at most maxGroups bone
// groups, the heaviest first, renormalised to sum to one; fewer groups than the limit are left untouched
#include <cstdio>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
struct CGroup {
    unsigned long BoneIndex; float BoneWeight;
    CGroup(unsigned long index, float weight) : BoneIndex(index), BoneWeight(weight) { }
    static bool CompareWeight(const CGroup& a, const CGroup& b) { return a.BoneWeight > b.BoneWeight; }
};
struct COptimisedVertex {
    std::map<unsigned long, float> Groups;
    void LimitGroupCount(unsigned long maxGroups)
    {
        unsigned long count = Groups.size();
        unsigned long newCount = count;
        std::vector<CGroup> groups;
        groups.reserve(count);
        for (std::map<unsigned long, float>::iterator it = Groups.begin(); it != Groups.end(); ++it)
            groups.push_back(CGroup(it->first, it->second));
        CGroup* first = &groups[0];
        std::sort(first, first + groups.size(), CGroup::CompareWeight);
        if (count > maxGroups) {
            float total = 0.0f;
            for (unsigned long i = 0; i < maxGroups; ++i) total += first[i].BoneWeight;
            float scale = 1.0f / total;
            for (unsigned long i = 0; i < maxGroups; ++i) first[i].BoneWeight *= scale;
            newCount = maxGroups;
        }
        Groups.clear();
        for (unsigned long i = 0; i < newCount; ++i) Groups[first[i].BoneIndex] = first[i].BoneWeight;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    COptimisedVertex v; v.Groups[7] = 0.2f; v.Groups[3] = 0.5f; v.Groups[9] = 0.3f;
    v.LimitGroupCount(5);
    bool ok = v.Groups.size() == 3 && near(v.Groups[3], 0.5f) && near(v.Groups[9], 0.3f) && near(v.Groups[7], 0.2f);
    v.LimitGroupCount(2);
    ok = ok && v.Groups.size() == 2 && v.Groups.count(7) == 0 && near(v.Groups[3], 0.625f) && near(v.Groups[9], 0.375f);
    std::printf(ok ? "00aca5f0_TEST PASS\n" : "00aca5f0_TEST FAIL\n");
    return ok ? 0 : 1;
}