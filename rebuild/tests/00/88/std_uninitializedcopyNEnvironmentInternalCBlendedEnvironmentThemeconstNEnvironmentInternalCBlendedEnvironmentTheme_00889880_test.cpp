// Behaviour test for 00889880: placement-copy 0x1F4-byte CElement1F4 records through the element's out-of-line copy constructor, return the advanced result
#include <cstdio>
#include <new>

static int g_copies = 0;

struct CElement1F4 {
    unsigned char Body[0x1F4];
    CElement1F4() { for (int i = 0; i < 0x1F4; ++i) Body[i] = 0; }
    CElement1F4(const CElement1F4& other)                                  // retail 0x008894A0 (stub: byte copy)
    {
        for (int i = 0; i < 0x1F4; ++i) Body[i] = other.Body[i];
        ++g_copies;
    }
};

struct FalseType { FalseType() { } };

template <class T1, class T2>
inline void Construct(T1* p, const T2& value) { new (p) T1(value); }

CElement1F4* UninitCopy(const CElement1F4* first, const CElement1F4* last, CElement1F4* result, const FalseType&)
{
    CElement1F4* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return cur;
}

int main()
{
    static CElement1F4 src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].Body[0] = (unsigned char)(i + 1);
        src[i].Body[0x1F4 - 1] = (unsigned char)(0x80 + i);
    }
    void* raw = ::operator new(3 * sizeof(CElement1F4));
    CElement1F4* dst = static_cast<CElement1F4*>(raw);
    CElement1F4* end = UninitCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && g_copies == 3 && sizeof(CElement1F4) == 0x1F4;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].Body[0] == i + 1 && dst[i].Body[0x1F4 - 1] == 0x80 + i;
    ok = ok && UninitCopy(src, src, dst, FalseType()) == dst && g_copies == 3;
    ::operator delete(raw);
    if (!ok) { std::printf("00889880_TEST FAIL\n"); return 1; }
    std::printf("00889880_TEST PASS\n");
    return 0;
}