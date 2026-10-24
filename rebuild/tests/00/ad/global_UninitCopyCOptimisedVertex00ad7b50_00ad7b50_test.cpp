// Behaviour test for 00ad7b50: placement-copy 0xB8-byte COptimisedVertex records through the element's out-of-line copy constructor, return the advanced result
#include <cstdio>
#include <new>

static int g_copies = 0;

struct COptimisedVertex {
    unsigned char Body[0xB8];
    COptimisedVertex() { for (int i = 0; i < 0xB8; ++i) Body[i] = 0; }
    COptimisedVertex(const COptimisedVertex& other)                                  // retail 0x00AD77F0 (stub: byte copy)
    {
        for (int i = 0; i < 0xB8; ++i) Body[i] = other.Body[i];
        ++g_copies;
    }
};

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

COptimisedVertex* UninitCopy(const COptimisedVertex* first, const COptimisedVertex* last, COptimisedVertex* result, const FalseType&)
{
    COptimisedVertex* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    static COptimisedVertex src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Body[0] = (unsigned char)(i + 1);
        src[i].Body[0xB8 - 1] = (unsigned char)(0x80 + i);
    }
    void* raw = ::operator new(3 * sizeof(COptimisedVertex));
    COptimisedVertex* dst = static_cast<COptimisedVertex*>(raw);
    COptimisedVertex* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && g_copies == 3 && sizeof(COptimisedVertex) == 0xB8;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].Body[0] == i + 1 && dst[i].Body[0xB8 - 1] == 0x80 + i;
    ok = ok && UninitCopy(src, src, dst, FalseType()) == dst && g_copies == 3;
    ::operator delete(raw);
    if (!ok) { std::printf("00ad7b50_TEST FAIL\n"); return 1; }
    std::printf("00ad7b50_TEST PASS\n");
    return 0;
}