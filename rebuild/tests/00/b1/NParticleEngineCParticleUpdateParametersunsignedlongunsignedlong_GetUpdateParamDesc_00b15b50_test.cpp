#include <cstdio>

// Model of CParticleUpdateParameters::GetUpdateParamDesc: sorted (key, descriptor) array, lower-bound lookup.
struct CCallback { int id; };
struct CParamDesc {
    CCallback* Callback;
    unsigned long Data;
};
struct Entry {
    unsigned long first;
    CParamDesc second;
};
struct CParams {
    Entry entries[4];
    int count;
    const Entry* LowerBound(const unsigned long& key) const
    {
        const Entry* first = entries;
        int len = count;
        while (len > 0) {
            int half = len / 2;
            const Entry* middle = first + half;
            if (middle->first < key) { first = middle + 1; len = len - half - 1; }
            else len = half;
        }
        return first;
    }
    const Entry* end() const { return entries + count; }
    const Entry* Find(const unsigned long& key) const
    {
        const Entry* it = LowerBound(key);
        if (it == end() || key < it->first) return end();
        return it;
    }
    bool GetUpdateParamDesc(CParamDesc& desc, const unsigned long& key) const
    {
        if (key) {
            const Entry* it = Find(key);
            if (it != end()) { desc = it->second; return true; }
        }
        desc.Callback = 0;
        return false;
    }
};

int main()
{
    CCallback a, b; a.id = 1; b.id = 2;
    CParams p; p.count = 2;
    p.entries[0].first = 10; p.entries[0].second.Callback = &a; p.entries[0].second.Data = 5;
    p.entries[1].first = 20; p.entries[1].second.Callback = &b; p.entries[1].second.Data = 6;
    CParamDesc d; d.Callback = &a; d.Data = 99;
    unsigned long k = 20;
    if (!p.GetUpdateParamDesc(d, k) || d.Callback != &b || d.Data != 6) { std::printf("00b15b50_TEST FAIL\n"); return 1; }
    k = 15; d.Data = 77;
    if (p.GetUpdateParamDesc(d, k) || d.Callback != 0 || d.Data != 77) { std::printf("00b15b50_TEST FAIL\n"); return 1; }
    k = 0; d.Callback = &a;
    if (p.GetUpdateParamDesc(d, k) || d.Callback != 0) { std::printf("00b15b50_TEST FAIL\n"); return 1; }
    k = 30; d.Callback = &a;
    if (p.GetUpdateParamDesc(d, k) || d.Callback != 0) { std::printf("00b15b50_TEST FAIL\n"); return 1; }
    std::printf("00b15b50_TEST PASS\n");
    return 0;
}