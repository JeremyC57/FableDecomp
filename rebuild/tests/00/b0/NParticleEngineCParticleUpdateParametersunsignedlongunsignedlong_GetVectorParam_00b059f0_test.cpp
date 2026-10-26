#include <cstdio>

// Model of CParticleUpdateParameters::GetVectorParam: sorted (key, vector) array, lower-bound lookup, no store on miss.
class C3DVector { public: float X, Y, Z; };
struct Entry {
    unsigned long first;
    C3DVector second;
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
    bool GetVectorParam(C3DVector& v, const unsigned long& key) const
    {
        if (key) {
            const Entry* it = Find(key);
            if (it != end()) { v = it->second; return true; }
        }
        return false;
    }
};

int main()
{
    CParams p; p.count = 3;
    for (int i = 0; i < 3; ++i) {
        p.entries[i].first = (unsigned long)(i + 1) * 100;
        p.entries[i].second.X = (float)i; p.entries[i].second.Y = (float)(i * 2); p.entries[i].second.Z = (float)(i * 3);
    }
    C3DVector v; v.X = v.Y = v.Z = -1.0f;
    unsigned long k = 300;
    if (!p.GetVectorParam(v, k) || v.X != 2.0f || v.Y != 4.0f || v.Z != 6.0f) { std::printf("00b059f0_TEST FAIL\n"); return 1; }
    v.X = 9.0f; k = 150;
    if (p.GetVectorParam(v, k) || v.X != 9.0f) { std::printf("00b059f0_TEST FAIL\n"); return 1; }
    k = 0;
    if (p.GetVectorParam(v, k) || v.X != 9.0f) { std::printf("00b059f0_TEST FAIL\n"); return 1; }
    k = 100;
    if (!p.GetVectorParam(v, k) || v.X != 0.0f) { std::printf("00b059f0_TEST FAIL\n"); return 1; }
    std::printf("00b059f0_TEST PASS\n");
    return 0;
}