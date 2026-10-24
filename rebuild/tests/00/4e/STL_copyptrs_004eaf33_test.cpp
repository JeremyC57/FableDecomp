#include <cstdio>
// Behaviour copy of STLport __copy_ptrs<CShoveHaPennyRegionDef>: counted forward assignment (base part, Start, Score),
// returning result + (last - first).
static int g_baseAssigns = 0;
struct SBase { long index; SBase& operator=(const SBase& o) { index = o.index; ++g_baseAssigns; return *this; } };
struct SRegion : SBase {
    float Start; float Score;
    SRegion& operator=(const SRegion& o) { SBase::operator=(o); Start = o.Start; Score = o.Score; return *this; }
};
static SRegion* CopyPtrs(const SRegion* first, const SRegion* last, SRegion* result)
{
    for (int n = int(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    bool ok = true;
    SRegion src[2], dst[3];
    src[0].index = 1; src[0].Start = 0.5f; src[0].Score = 10.0f;
    src[1].index = 2; src[1].Start = 0.75f; src[1].Score = 20.0f;
    for (int i = 0; i < 3; ++i) { dst[i].index = -1; dst[i].Start = -1.0f; dst[i].Score = -1.0f; }
    SRegion* end = CopyPtrs(src, src + 2, dst);
    if (end != dst + 2 || g_baseAssigns != 2) ok = false;
    if (dst[0].index != 1 || dst[0].Start != 0.5f || dst[0].Score != 10.0f) ok = false;
    if (dst[1].index != 2 || dst[1].Start != 0.75f || dst[1].Score != 20.0f) ok = false;
    if (dst[2].index != -1) ok = false;
    if (CopyPtrs(src, src, dst) != dst || g_baseAssigns != 2) ok = false;
    if (!ok) { std::printf("004eaf33_TEST FAIL\n"); return 1; }
    std::printf("004eaf33_TEST PASS\n");
    return 0;
}