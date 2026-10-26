// Behaviour test for 0x006B6010 CIDrawEnvironmentParams::CThemeChannel implicit copy constructor (model copy).
#include <cstdio>
#include <cstdlib>
#include <cstring>

template <class T> struct Vec {                       // STLport vector<T> of POD: three pointers
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    Vec() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    Vec(const Vec& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        if (x._M_finish != x._M_start) {
            std::memmove(_M_start, x._M_start, (char*)x._M_finish - (char*)x._M_start);
            _M_finish = _M_start + n;
        }
    }
    ~Vec() { if (_M_start) std::free(_M_start); }
    void assign(const T* p, unsigned int n)
    {
        _M_start = (T*)std::malloc(n * sizeof(T)); std::memcpy(_M_start, p, n * sizeof(T));
        _M_finish = _M_end_of_storage = _M_start + n;
    }
};
class CRGBFloatColour { public: float R, G, B, A; };
struct CIDrawEnvironmentTextureSet { Vec<long> Textures; Vec<float> Blends; };
struct CThemeChannel {
    CRGBFloatColour Colours[19];
    float Scalers[30];
    CIDrawEnvironmentTextureSet TextureSets[3];
};

int main()
{
    CThemeChannel* a = new CThemeChannel;
    for (int i = 0; i < 19; ++i) { a->Colours[i].R = (float)i; a->Colours[i].A = 0.5f * i; }
    for (int i = 0; i < 30; ++i) a->Scalers[i] = 2.0f * i;
    long tex[3] = { 7, 8, 9 };
    float bl[2] = { 0.25f, 0.75f };
    a->TextureSets[0].Textures.assign(tex, 3);
    a->TextureSets[2].Blends.assign(bl, 2);
    CThemeChannel* b = new CThemeChannel(*a);
    int ok = 1;
    for (int i = 0; i < 19; ++i) if (b->Colours[i].R != (float)i || b->Colours[i].A != 0.5f * i) ok = 0;
    for (int i = 0; i < 30; ++i) if (b->Scalers[i] != 2.0f * i) ok = 0;
    if (b->TextureSets[0].Textures._M_finish - b->TextureSets[0].Textures._M_start != 3) ok = 0;
    if (b->TextureSets[0].Textures._M_start == a->TextureSets[0].Textures._M_start) ok = 0;   // deep copy
    if (b->TextureSets[0].Textures._M_start[2] != 9) ok = 0;
    if (b->TextureSets[1].Textures._M_start != 0 || b->TextureSets[1].Blends._M_start != 0) ok = 0;
    if (b->TextureSets[2].Blends._M_end_of_storage - b->TextureSets[2].Blends._M_start != 2) ok = 0;
    if (b->TextureSets[2].Blends._M_start[1] != 0.75f) ok = 0;
    delete b; delete a;
    if (!ok) { std::printf("006b6010_TEST FAIL\n"); return 1; }
    std::printf("006b6010_TEST PASS\n");
    return 0;
}