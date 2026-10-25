// Test for 0x006CA950 CEngineTextureMorphSet::AddTextureMorph: the source object finds or creates the texture's morph
// record (adding an opaque white base layer of type 3), clears the blends on a base-type blend, then appends the blend.
// AddEngineMorph / push_back / erase are test stubs over static storage.
#include <stdio.h>
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& o) : B(o.B), G(o.G), R(o.R), A(o.A) {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CCompositeBlendDesc {
public:
    unsigned long TextureIndex; long BlendType; CRGBColour DiffuseColour; CRGBColour SpecularColour;
    CCompositeBlendDesc(const CCompositeBlendDesc& o)
        : TextureIndex(o.TextureIndex), BlendType(o.BlendType), DiffuseColour(o.DiffuseColour), SpecularColour(o.SpecularColour) {}
    CCompositeBlendDesc(unsigned long texture, long type)
        : TextureIndex(texture), BlendType(type), DiffuseColour(255, 255, 255, 255), SpecularColour(0, 0, 0, 0) {}
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start; _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    void push_back(const _Tp& __x);
    iterator erase(iterator __first, iterator __last);
};
}
template <class T> class CArray : public std::vector<T> { };
class CEngineTextureMorph {
public:
    long OriginalTextureIndex; long MorphID; std::vector<CCompositeBlendDesc> Blends;
};
class CEngineTextureMorphSet {
public:
    void AddTextureMorph(long texture_index, const CCompositeBlendDesc& blend);
    CArray<CEngineTextureMorph> Morphs;
protected:
    CEngineTextureMorph* AddEngineMorph(long texture_index);           // protected: IAE, as in the PDB
};
static unsigned char g_blendpool[4][8][sizeof(CCompositeBlendDesc)];
static unsigned char g_morphpool[4][sizeof(CEngineTextureMorph)];
static int g_adds;
CEngineTextureMorph* CEngineTextureMorphSet::AddEngineMorph(long texture_index)
{
    ++g_adds;
    CEngineTextureMorph* m = Morphs._M_finish++;
    m->OriginalTextureIndex = texture_index; m->MorphID = 0;
    m->Blends._M_start = m->Blends._M_finish = (CCompositeBlendDesc*)g_blendpool[m - Morphs._M_start];
    m->Blends._M_end_of_storage._M_data = m->Blends._M_start + 8;
    return m;
}
namespace std {
template <> void vector<CCompositeBlendDesc>::push_back(const CCompositeBlendDesc& __x)
{
    unsigned char* d = (unsigned char*)_M_finish; const unsigned char* s = (const unsigned char*)&__x;
    for (unsigned k = 0; k < sizeof(CCompositeBlendDesc); ++k) d[k] = s[k];
    ++_M_finish;
}
template <> vector<CCompositeBlendDesc>::iterator vector<CCompositeBlendDesc>::erase(iterator __first, iterator __last)
{
    if (__last == _M_finish) _M_finish = __first;
    return __first;
}
}
int main()
{
    CEngineTextureMorphSet set;
    set.Morphs._M_start = set.Morphs._M_finish = (CEngineTextureMorph*)g_morphpool;
    set.Morphs._M_end_of_storage._M_data = set.Morphs._M_start + 4;
    CCompositeBlendDesc b1(5, 1), b2(5, 2), b3(5, 3), b4(7, 1);
    b1.DiffuseColour.R = 1; b3.DiffuseColour.R = 9;
    set.AddTextureMorph(5, b1);
    CEngineTextureMorph* m0 = set.Morphs._M_start;
    bool ok = g_adds == 1 && set.Morphs._M_finish - set.Morphs._M_start == 1 && m0->Blends._M_finish - m0->Blends._M_start == 2
        && m0->Blends._M_start[0].BlendType == 3 && m0->Blends._M_start[0].TextureIndex == 5
        && m0->Blends._M_start[0].DiffuseColour.R == 255 && m0->Blends._M_start[0].DiffuseColour.A == 255
        && m0->Blends._M_start[0].SpecularColour.A == 0 && m0->Blends._M_start[1].DiffuseColour.R == 1;
    set.AddTextureMorph(5, b2);
    ok = ok && g_adds == 1 && m0->Blends._M_finish - m0->Blends._M_start == 3;
    set.AddTextureMorph(5, b3);
    ok = ok && m0->Blends._M_finish - m0->Blends._M_start == 1 && m0->Blends._M_start[0].DiffuseColour.R == 9;
    set.AddTextureMorph(7, b4);
    ok = ok && g_adds == 2 && set.Morphs._M_finish - set.Morphs._M_start == 2 && set.Morphs._M_start[1].OriginalTextureIndex == 7
        && set.Morphs._M_start[1].Blends._M_finish - set.Morphs._M_start[1].Blends._M_start == 2;
    if (!ok) { puts("006ca950_TEST FAIL"); return 1; }
    puts("006ca950_TEST PASS");
    return 0;
}
