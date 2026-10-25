// Behaviour test for __copy_ptrs over CFireHeartPatternDef (0x00D34FD0): assign last - first elements in order (base
// part, then the PadActive vector) and return the destination end; an empty range assigns nothing.
#include <cstdio>

static int g_baseAssigns = 0;
static int g_vectorAssigns = 0;
class CDefClassBase {
public:
    int defIndex;
    CDefClassBase& operator=(const CDefClassBase& o) { defIndex = o.defIndex; ++g_baseAssigns; return *this; }
};
struct CBoolVector {
    int pattern;
    CBoolVector& operator=(const CBoolVector& o) { pattern = o.pattern; ++g_vectorAssigns; return *this; }
};
class CFireHeartPatternDef : public CDefClassBase {
public:
    CBoolVector PadActive;
};
CFireHeartPatternDef* CopyPtrs(const CFireHeartPatternDef* first, const CFireHeartPatternDef* last, CFireHeartPatternDef* result)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    CFireHeartPatternDef src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].defIndex = i + 1; src[i].PadActive.pattern = 10 * (i + 1); }
    for (int k = 0; k < 4; ++k) { dst[k].defIndex = 0; dst[k].PadActive.pattern = 0; }
    CFireHeartPatternDef* end = CopyPtrs(src, src + 3, dst);
    CFireHeartPatternDef* none = CopyPtrs(src, src, dst + 3);
    if (end != dst + 3 || none != dst + 3 || dst[0].defIndex != 1 || dst[2].defIndex != 3 || dst[1].PadActive.pattern != 20
        || dst[3].defIndex != 0 || g_baseAssigns != 3 || g_vectorAssigns != 3) {
        std::printf("00d34fd0_TEST FAIL\n");
        return 1;
    }
    std::printf("00d34fd0_TEST PASS\n");
    return 0;
}