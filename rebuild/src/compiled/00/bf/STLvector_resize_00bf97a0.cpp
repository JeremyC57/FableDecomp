// std::vector::resize(unsigned int new_size) -- STLport one-argument resize of a texture palette's entry vector: resize(n, CPaletteEntry()) with the default entry destroyed on both exits
// Leaf: ?resize@?$vector@UCPaletteEntry@CEngineTexturePalette@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF97A0, 108 bytes (+ int3 padding), `__fastcall` (this -> esi), `ret 4`.
// Identity: element Ego_r.pdb CEngineTexturePalette::CPaletteEntry (struct { ulong Index; CCharString Name; }, 8 bytes;
//   the Palette member is CArray<CEngineTexturePalette::CPaletteEntry>, whose resize is the inherited vector one); the
//   callers 0x00BF9260, 0x00BF92FF, 0x00BF9366 are CEngineTexturePalette::GetTexturePaletteIndex 0x00BF9150 / Load
//   0x00BF9290. The implicit default constructor builds only the string (Index left uninitialised). The catalog's
//   "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize" is propagated.
// Callees: CCharString::CCharString() 0x0099E4B0, vector::erase(first, last) 0x00BF9420 (catalog
//   "CParticleMorphs::CopyAndConstruct"), vector::_M_fill_insert(pos, n, x) 0x00BF95D0, CCharString::~CCharString
//   0x0099EAE0.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over the inline `resize(n, x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }`: the default element in the 8-byte frame (the string built at +4 first), its address pushed
//   to the fill-insert, and its destructor on both exits.
class CCharString {
public:
    void* m_pData;                                      // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};

class CEngineTexturePalette {
public:
    struct CPaletteEntry {                              // Ego_r.pdb, 8 bytes
        unsigned long Index;                            // +0x00
        CCharString   Name;                             // +0x04
    };
};

namespace std {
template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last);              // 0x00BF9420
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x00BF95D0
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CEngineTexturePalette::CPaletteEntry>::resize(unsigned int);
}