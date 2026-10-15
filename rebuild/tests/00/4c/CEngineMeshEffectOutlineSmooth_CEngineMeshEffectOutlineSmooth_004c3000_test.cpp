// Behaviour test for CEngineMeshEffectOutlineSmooth::CEngineMeshEffectOutlineSmooth @ 0x004C3000.
#include <cstdio>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : B(b), G(g), R(r), A(a) { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); }
};
struct CEngineMeshEffectOutlineBase {
    long m_refCount; void* m_pOwner; float m_width, m_fade; CRGBColour m_colour; unsigned char m_optionA, m_optionB, m_optionC;
    virtual int Kind() { return 0; }
    virtual ~CEngineMeshEffectOutlineBase() { }
    CEngineMeshEffectOutlineBase(float w, float f, const CRGBColour& c, unsigned char a, unsigned char b, unsigned char cc)
        : m_refCount(1), m_pOwner(0), m_width(w), m_fade(f), m_colour(c), m_optionA(a), m_optionB(b), m_optionC(cc) { }
};
struct CEngineMeshEffectOutlineSmooth : CEngineMeshEffectOutlineBase {
    virtual int Kind() { return 1; }
    CEngineMeshEffectOutlineSmooth(float w, float f, const CRGBColour& c, unsigned char a, unsigned char b, unsigned char cc)
        : CEngineMeshEffectOutlineBase(w, f, c, a, b, cc) { }
};
int main()
{
    CRGBColour c(10, 20, 30, 40);
    CEngineMeshEffectOutlineSmooth o(1.5f, 2.5f, c, 7, 8, 9);
    int ok = o.Kind() == 1 && o.m_refCount == 1 && o.m_pOwner == 0 && o.m_width == 1.5f && o.m_fade == 2.5f
          && o.m_colour.R == 10 && o.m_colour.G == 20 && o.m_colour.B == 30 && o.m_colour.A == 40
          && o.m_optionA == 7 && o.m_optionB == 8 && o.m_optionC == 9;
    if (!ok) { std::printf("004c3000_TEST FAIL\n"); return 1; }
    std::printf("004c3000_TEST PASS\n");
    return 0;
}