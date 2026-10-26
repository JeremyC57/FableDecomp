// CBlendedEnvironmentTheme::BlendTextureSets(const CIDrawEnvironmentTextureSet& from, const CIDrawEnvironmentTextureSet& to, CIDrawEnvironmentTextureSet& result, float blend) -- lay both sets' textures side by side in the result (to's after from's), weight to's blends by `blend` and from's by 1 - blend, then compact the result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00887830, 498 bytes (512 with padding), `ret 10h`.
// Callees: CArray<long>::resize(n, const long&) 0x004428F0 and CArray<float>::resize(n, const float&) 0x00442770
//   (catalog "vector<unsigned int>::resize", as the landed COpinionSet::TransferBinaryIn), CompactTextureSet
//   0x00886E80 (this, set, count).
// Layout: Ego_r.pdb CIDrawEnvironmentTextureSet {CArray<long> Textures, CArray<float> Blends}; retail CArrays are 12-byte
//   STLport vectors (Blends at +0xC, not the PDB's +0x10).
// Shape: the loop is VC7.1's 4x unroll of the plain per-index body; 1 - blend is hoisted.
template <class T> class CArray { public:             // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long n) { return _M_start[n]; }
    const T& operator[](long n) const { return _M_start[n]; }
    void resize(unsigned long n, const T& value);   // 0x004428F0 (long) / 0x00442770 (float)
};
struct CIDrawEnvironmentTextureSet {
    CArray<long>  Textures;                    // +0x00
    CArray<float> Blends;                      // +0x0c
};
struct CBlendedEnvironmentTheme {
    void CompactTextureSet(CIDrawEnvironmentTextureSet& set, long count);   // 0x00886E80
    void BlendTextureSets(const CIDrawEnvironmentTextureSet& from, const CIDrawEnvironmentTextureSet& to,
                          CIDrawEnvironmentTextureSet& result, float blend);
};

void CBlendedEnvironmentTheme::BlendTextureSets(const CIDrawEnvironmentTextureSet& from, const CIDrawEnvironmentTextureSet& to,
                                                CIDrawEnvironmentTextureSet& result, float blend)
{
    long count = from.Textures.size();
    result.Textures.resize(count * 2, 0);
    result.Blends.resize(count * 2, 0.0f);
    for (long i = 0; i < count; ++i) {
        result.Textures[count + i] = to.Textures[i];
        result.Textures[i] = from.Textures[i];
        result.Blends[count + i] = to.Blends[i] * blend;
        result.Blends[i] = from.Blends[i] * (1.0f - blend);
    }
    CompactTextureSet(result, count);
}