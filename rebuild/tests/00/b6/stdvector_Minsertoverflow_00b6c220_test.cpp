// behaviour test for 0x00B6C220: growing through the overflow path keeps every record and inserts fill copies in place
#include <cstdio>
#include <vector>
struct R { long w[14]; };
int main()
{
    std::vector<R> v;
    for (long i = 0; i < 5; ++i) { R r; for (int k = 0; k < 14; ++k) r.w[k] = i * 100 + k; v.push_back(r); }
    R f; for (int k = 0; k < 14; ++k) f.w[k] = -1;
    v.insert(v.begin() + 2, 3, f);
    if (v.size() != 8 || v[2].w[0] != -1 || v[4].w[14 - 1] != -1 || v[5].w[0] != 200 || v[7].w[1] != 401) { std::printf("00b6c220_TEST FAIL\n"); return 1; }
    std::printf("00b6c220_TEST PASS\n");
    return 0;
}