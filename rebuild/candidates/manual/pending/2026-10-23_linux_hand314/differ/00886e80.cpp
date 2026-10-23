// CBlendedEnvironmentTheme::CompactTextureSet(CIDrawEnvironmentTextureSet& set, long count) -- merge duplicate textures (summing their blends), bubble-sort by blend (largest first), keep the first `count`: subtract the first dropped blend, renormalise (uniform 1/count when the total is below 1/256), and truncate both arrays
// Grade: DIFFER(1174v1190) -- 66 blocks: register allocation -- retail keeps the texture count in edi and stores 0.0f as an immediate, ours keeps the count in ebx and materialises zero in edi (also SIB base/index order flips); loops, unrolling, sort, renormalise and the inline STLport resize/erase line up
// Leaf: CompactTextureSet
// Retail 0x00886E80, 1191 bytes, `ret 8`.
// Callees: memmove 0x00BFEAE6 (inline STLport erase), vector<long>::_M_fill_insert 0x00440F00, vector<float>::_M_fill_insert
//   0x00441020 (the out-of-line half of resize).
// Layout: Ego_r.pdb CIDrawEnvironmentTextureSet {CArray<long> Textures, CArray<float> Blends}; retail CArrays are 12-byte
//   STLport vectors (Blends at +0xC), as the landed BlendTextureSets 0x00887830.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);
template <class T> struct CArray {             // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long n) { return _M_start[n]; }
    void _M_fill_insert(T* position, unsigned long n, const T& x);   // 0x00440F00 (long) / 0x00441020 (float)
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
    void resize(unsigned long n, const T& x = T())
    {
        if (n < (unsigned long)size())
            erase(_M_start + n, _M_finish);
        else
            _M_fill_insert(_M_finish, n - size(), x);
    }
};
template <class T> inline void Swap(T& a, T& b) { T t = a; a = b; b = t; }
struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                    // +0x00
    CArray<float> Blends;                      // +0x0c
};
struct CBlendedEnvironmentTheme {
    void CompactTextureSet(CIDrawEnvironmentTextureSet& set, long count);
};

void CBlendedEnvironmentTheme::CompactTextureSet(CIDrawEnvironmentTextureSet& set, long count)
{
    long n = set.Textures.size();
    for (long i = 0; i < n; ++i) {
        for (long j = i + 1; j < n; ++j) {
            if (set.Textures[j] == set.Textures[i]) {
                set.Blends[i] += set.Blends[j];
                set.Blends[j] = 0.0f;
            }
        }
    }
    bool swapped;
    do {
        swapped = false;
        for (long i = 0; i < n - 1; ++i) {
            if (set.Blends[i] < set.Blends[i + 1]) {
                Swap(set.Blends[i], set.Blends[i + 1]);
                Swap(set.Textures[i], set.Textures[i + 1]);
                swapped = true;
            }
        }
    } while (swapped);
    for (long i = 0; i < count; ++i)
        set.Blends[i] -= set.Blends[count];
    float total = 0.0f;
    for (long i = 0; i < count; ++i)
        total += set.Blends[i];
    if (total < 0.00390625f) {
        for (long i = 0; i < count; ++i)
            set.Blends[i] = 1.0f / count;
    } else {
        float scale = 1.0f / total;
        for (long i = 0; i < count; ++i)
            set.Blends[i] *= scale;
    }
    set.Textures.resize(count);
    set.Blends.resize(count);
}
