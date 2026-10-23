// CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes(const CArray<CBlendedEnvironmentTheme>& themes, const CArray<float>& weights) -- this = sum of weights[i] * themes[i] for the 19 colours and 30 scalers; each of the three texture sets collects every theme's textures with weighted blends and is compacted to the first theme's texture count
// Grade: DIFFER(1172v1131) -- 56 blocks: with push_back/_M_insert_overflow inlined (as retail; plain inline leaves the overflow out of line, 910 bytes) the texture-set loop hoists different addresses (source Blends pointer, set base at +0x1b0 v +0x1bc) and allocates ebx/ebp/esi differently; colour/scaler loops, clears, the overflow path and CompactTextureSet call line up
// Leaf: SetAsWeightedBlendOfThemes
// Retail 0x008873C0, 1131 bytes, `ret 8`.
// Callees: CompactTextureSet 0x00886E80; malloc 0x00BFEA0E, free 0x00BFEA14, memmove 0x00BFEAE6 (inline STLport clear and
//   push_back with its overflow path).
// Layout: Ego_r.pdb CBlendedEnvironmentTheme {float TimeOfDay; CRGBFloatColour Colours[19]; float Scalers[30];
//   CIDrawEnvironmentTextureSet TextureSets[3]} with retail 12-byte STLport CArrays (0x1f4 bytes, texture sets at +0x1ac),
//   as the landed BlendTextureSets 0x00887830 and Blend 0x00887B70.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float s) const { return CRGBFloatColour(R * s, G * s, B * s, A * s); }
    CRGBFloatColour& operator+=(const CRGBFloatColour& o) { R += o.R; G += o.G; B += o.B; A += o.A; return *this; }
};
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
template <class T> struct CArray {             // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned long n) { return _M_start[n]; }
    const T& operator[](unsigned long n) const { return _M_start[n]; }
    static T* __copy_trivial(const T* first, const T* last, T* result)
    {
        return (last == first) ? result
                               : (T*)((char*)memmove(result, first, (const char*)last - (const char*)first) +
                                      ((const char*)last - (const char*)first));
    }
    T* erase(T* first, T* last)
    {
        T* i = __copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(_M_start, _M_finish); }
    __forceinline void _M_insert_overflow(T* position, const T& x, unsigned long fill_len)
    {
        const unsigned long old_size = size();
        const unsigned long len = old_size + Max(old_size, fill_len);
        T* new_start = len != 0 ? (T*)malloc(len * sizeof(T)) : 0;
        T* new_finish = __copy_trivial(_M_start, position, new_start);
        *new_finish = x;
        ++new_finish;
        if (_M_start)
            free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
    __forceinline void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, 1UL);
    }
};
struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                    // +0x00
    CArray<float> Blends;                      // +0x0c
};
class CBlendedEnvironmentTheme {
public:
    float                       TimeOfDay;     // +0x000
    CRGBFloatColour             Colours[19];   // +0x004
    float                       Scalers[30];   // +0x134
    CIDrawEnvironmentTextureSet TextureSets[3];   // +0x1ac
    void CompactTextureSet(CIDrawEnvironmentTextureSet& set, long count);   // 0x00886E80
    void SetAsWeightedBlendOfThemes(const CArray<CBlendedEnvironmentTheme>& themes, const CArray<float>& weights);
};

void CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes(const CArray<CBlendedEnvironmentTheme>& themes,
                                                          const CArray<float>& weights)
{
    if (weights.empty() || themes.empty())
        return;
    for (long c = 0; c < 19; ++c) {
        Colours[c].R = 0.0f;
        Colours[c].G = 0.0f;
        Colours[c].B = 0.0f;
        Colours[c].A = 0.0f;
        for (unsigned long i = 0; i < themes.size(); ++i)
            Colours[c] += themes[i].Colours[c] * weights[i];
    }
    for (long s = 0; s < 30; ++s) {
        Scalers[s] = 0.0f;
        for (unsigned long i = 0; i < themes.size(); ++i)
            Scalers[s] += weights[i] * themes[i].Scalers[s];
    }
    for (long t = 0; t < 3; ++t) {
        CIDrawEnvironmentTextureSet& set = TextureSets[t];
        set.Textures.clear();
        set.Blends.clear();
        for (unsigned long i = 0; i < themes.size(); ++i) {
            const CIDrawEnvironmentTextureSet& source = themes[i].TextureSets[t];
            for (unsigned long j = 0; j < source.Textures.size(); ++j) {
                set.Textures.push_back(source.Textures[j]);
                set.Blends.push_back(source.Blends[j] * weights[i]);
            }
        }
        CompactTextureSet(set, themes[0].TextureSets[t].Textures.size());
    }
}
