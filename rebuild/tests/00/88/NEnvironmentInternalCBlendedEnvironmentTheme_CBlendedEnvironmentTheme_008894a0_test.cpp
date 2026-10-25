// Behaviour test for 0x008894A0 NEnvironmentInternal::CBlendedEnvironmentTheme copy constructor (implicit, memberwise)
// The model mirrors the source: POD blocks copied as-is, each texture-set CArray deep-copied the STLport way
// (allocate x.size() words, __copy_trivial = memmove + n).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

template <class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CArray(const CArray& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        unsigned int bytes = (unsigned int)((const char*)x._M_finish - (const char*)x._M_start);
        _M_finish = bytes == 0 ? _M_start : (T*)((char*)std::memmove(_M_start, x._M_start, bytes) + bytes);
    }
    ~CArray() { if (_M_start) std::free(_M_start); }
    void fill(unsigned int n, T base)
    {
        _M_start = (T*)std::malloc((n ? n : 1) * sizeof(T));
        for (unsigned int i = 0; i < n; ++i) _M_start[i] = base + (T)i;
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_finish;
    }
};

struct CRGBFloatColour { float R, G, B, A; };
struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;
    CArray<float> Blends;
};
struct CBlendedEnvironmentTheme {
    float                       TimeOfDay;
    CRGBFloatColour             Colours[19];
    float                       Scalers[30];
    CIDrawEnvironmentTextureSet TextureSets[3];
    CBlendedEnvironmentTheme() { }
};

int main()
{
    int fail = 0;
    CBlendedEnvironmentTheme a;
    a.TimeOfDay = 13.5f;
    for (int i = 0; i < 19; ++i) { a.Colours[i].R = (float)i; a.Colours[i].G = i * 2.0f; a.Colours[i].B = i * 3.0f; a.Colours[i].A = 1.0f; }
    for (int i = 0; i < 30; ++i) a.Scalers[i] = i * 0.25f;
    a.TextureSets[0].Textures.fill(3, 100); a.TextureSets[0].Blends.fill(3, 0.5f);
    a.TextureSets[1].Textures.fill(0, 0);   a.TextureSets[1].Blends.fill(1, 2.0f);
    a.TextureSets[2].Textures.fill(5, 7);   a.TextureSets[2].Blends.fill(0, 0.0f);

    CBlendedEnvironmentTheme* b = new CBlendedEnvironmentTheme(a);
    if (b->TimeOfDay != 13.5f) fail |= 1;
    for (int i = 0; i < 19; ++i) if (b->Colours[i].G != i * 2.0f || b->Colours[i].A != 1.0f) fail |= 2;
    for (int i = 0; i < 30; ++i) if (b->Scalers[i] != i * 0.25f) fail |= 4;
    for (int s = 0; s < 3; ++s) {
        const CArray<long>& t = b->TextureSets[s].Textures; const CArray<long>& u = a.TextureSets[s].Textures;
        if ((t._M_finish - t._M_start) != (u._M_finish - u._M_start)) fail |= 8;
        if (t._M_end_of_storage != t._M_finish) fail |= 16;
        if (t._M_start != 0 && t._M_start == u._M_start) fail |= 32;                // deep copy, not aliasing
        for (long i = 0; i < (long)(u._M_finish - u._M_start); ++i) if (t._M_start[i] != u._M_start[i]) fail |= 64;
        const CArray<float>& f = b->TextureSets[s].Blends; const CArray<float>& g = a.TextureSets[s].Blends;
        if ((f._M_finish - f._M_start) != (g._M_finish - g._M_start)) fail |= 128;
        for (long i = 0; i < (long)(g._M_finish - g._M_start); ++i) if (f._M_start[i] != g._M_start[i]) fail |= 256;
    }
    if (b->TextureSets[1].Textures._M_start != 0 || b->TextureSets[2].Blends._M_start != 0) fail |= 512;   // empty -> null
    delete b;
    if (fail) { std::printf("008894a0_TEST FAIL\n"); return 1; }
    std::printf("008894a0_TEST PASS\n");
    return 0;
}