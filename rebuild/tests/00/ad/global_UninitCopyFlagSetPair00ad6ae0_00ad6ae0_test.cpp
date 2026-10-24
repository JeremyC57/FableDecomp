// Behaviour test for 0x00AD6AE0 UninitCopy_FlagSetPair_00ad6ae0: placement copy of {flag, set, set} records.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
static int g_setCopies = 0;
struct STreeNode { int m_key; };
struct SRetailSet {
    STreeNode*    Head;
    unsigned int  Count;
    unsigned char Compare;
    SRetailSet() : Head(0), Count(0), Compare(0) { }
    SRetailSet(const SRetailSet& other) : Head(other.Head), Count(other.Count), Compare(other.Compare) { ++g_setCopies; }
};
struct SFlagSetPair { unsigned char Flag; SRetailSet A; SRetailSet B; };

static SFlagSetPair* UninitCopy(const SFlagSetPair* first, const SFlagSetPair* last, SFlagSetPair* result, const __false_type&)
{
    SFlagSetPair* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SFlagSetPair(*first);
    return cur;
}

int main()
{
    static STreeNode nodes[6];
    SFlagSetPair src[3];
    for (int i = 0; i < 3; ++i) { src[i].Flag = (unsigned char)(0x80 | i); src[i].A.Head = &nodes[2 * i]; src[i].A.Count = i; src[i].B.Head = &nodes[2 * i + 1]; src[i].B.Count = 10u + i; }
    SFlagSetPair dst[3];
    __false_type tag;
    SFlagSetPair* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_setCopies == 6;
    for (int k = 0; k < 3; ++k)
        if (dst[k].Flag != (unsigned char)(0x80 | k) || dst[k].A.Head != &nodes[2 * k] || dst[k].A.Count != (unsigned int)k
            || dst[k].B.Head != &nodes[2 * k + 1] || dst[k].B.Count != 10u + k) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst || g_setCopies != 6) ok = false;
    if (!ok) { std::printf("00ad6ae0_TEST FAIL\n"); return 1; }
    std::printf("00ad6ae0_TEST PASS\n");
    return 0;
}