#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() : B(0), G(0), R(0), A(0) { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
struct SColouredRecord18 { unsigned long m_key; long m_word4, m_word8, m_wordC; CRGBColour m_colour0, m_colour1; SColouredRecord18() : m_key(0), m_word4(0), m_word8(0), m_wordC(0) { } };
typedef SColouredRecord18 TElem;
static void Fill(TElem& e, int i)
{
    e.m_key = 11u * i; e.m_word4 = i; e.m_word8 = -i; e.m_wordC = 1000 + i; e.m_colour0.Set((unsigned char)(i + 4), (unsigned char)(i + 5), (unsigned char)(i + 6), (unsigned char)(i + 7)); e.m_colour1.Set(1, (unsigned char)(2 * i), 3, (unsigned char)(4 * i));
}
static bool Check(const TElem& e, int i)
{
    return e.m_key == 11u * i && e.m_word4 == i && e.m_word8 == -i && e.m_wordC == 1000 + i && e.m_colour0.R == i + 4 && e.m_colour0.G == i + 5 && e.m_colour0.B == i + 6 && e.m_colour0.A == i + 7 && e.m_colour1.R == 1 && e.m_colour1.G == 2 * i && e.m_colour1.B == 3 && e.m_colour1.A == 4 * i;
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    
    if (!ok) { std::printf("00720270_TEST FAIL\n"); return 1; }
    std::printf("00720270_TEST PASS\n");
    return 0;
}