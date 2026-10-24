// Behaviour test for 0x00471F97 vector<EOpinionAttitudeType>(const vector&): allocate size() slots (none for empty), copy
// every element, finish = start + size, end of storage = start + size
#include <cstdio>
#include <cstdlib>
enum EOpinionAttitudeType { ATT_A = 1, ATT_B = 5, ATT_C = 9 };
struct Vec {
    EOpinionAttitudeType* start; EOpinionAttitudeType* finish; EOpinionAttitudeType* end;
    void Base(unsigned int n) { start = finish = end = 0; start = n ? (EOpinionAttitudeType*)std::malloc(n * sizeof(EOpinionAttitudeType)) : 0; finish = start; end = start + n; }
    void CopyConstruct(const Vec& x)
    {
        Base((unsigned int)(x.finish - x.start));
        EOpinionAttitudeType* cur = start;
        for (const EOpinionAttitudeType* p = x.start; p != x.finish; ++p, ++cur) if (cur) *cur = *p;
        finish = cur;
    }
};
int main()
{
    EOpinionAttitudeType data[3] = { ATT_C, ATT_A, ATT_B };
    Vec src = { data, data + 3, data + 3 };
    Vec v; v.CopyConstruct(src);
    bool ok = v.start != data && v.finish - v.start == 3 && v.end == v.finish && v.start[0] == ATT_C && v.start[2] == ATT_B;
    std::free(v.start);
    Vec empty = { 0, 0, 0 }, w; w.CopyConstruct(empty);
    ok = ok && w.start == 0 && w.finish == 0 && w.end == 0;
    if (!ok) { std::printf("00471f97_TEST FAIL\n"); return 1; }
    std::printf("00471f97_TEST PASS\n");
    return 0;
}