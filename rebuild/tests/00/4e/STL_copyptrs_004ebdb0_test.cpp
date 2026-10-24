#include <cstdio>
// Behaviour copy of STLport __copy_ptrs<CCardPositionsDef>: counted forward assignment (base part, then the Offset vector
// part), returning result + (last - first).
static int g_baseAssigns = 0, g_vecAssigns = 0;
struct SBase { long name; SBase& operator=(const SBase& o) { name = o.name; ++g_baseAssigns; return *this; } };
struct SVec { long count; SVec& operator=(const SVec& o) { count = o.count; ++g_vecAssigns; return *this; } };
struct SDef : SBase {
    SVec Offset;
    SDef& operator=(const SDef& o) { SBase::operator=(o); Offset = o.Offset; return *this; }
};
static SDef* CopyPtrs(const SDef* first, const SDef* last, SDef* result)
{
    for (int n = int(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    bool ok = true;
    SDef src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].name = 10 + i; src[i].Offset.count = 20 + i; }
    for (int i = 0; i < 4; ++i) { dst[i].name = -1; dst[i].Offset.count = -1; }
    SDef* end = CopyPtrs(src, src + 3, dst);
    if (end != dst + 3 || g_baseAssigns != 3 || g_vecAssigns != 3) ok = false;
    for (int i = 0; ok && i < 3; ++i) if (dst[i].name != 10 + i || dst[i].Offset.count != 20 + i) ok = false;
    if (dst[3].name != -1) ok = false;
    if (CopyPtrs(src, src, dst) != dst || g_baseAssigns != 3) ok = false;   // empty range
    if (!ok) { std::printf("004ebdb0_TEST FAIL\n"); return 1; }
    std::printf("004ebdb0_TEST PASS\n");
    return 0;
}