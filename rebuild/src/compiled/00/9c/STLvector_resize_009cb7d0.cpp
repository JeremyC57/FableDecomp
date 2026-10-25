// _STL::vector::resize(unsigned int new_size) -- STLport one-argument resize of a text entry's tag vector: resize(n, CTextTag()) with the default tag destroyed on both exits
// Leaf: ?resize@?$vector@VCTextTag@NGameText@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CB7D0, 108 bytes (+ int3 padding), `__fastcall` (this -> esi), `ret 4`.
// Identity: Ego_r.pdb `?resize@?$vector@VCTextTag@NGameText@@...@@QAEXI@Z` (NGameText::CTextEntry::Tags is
//   CArray<NGameText::CTextTag>); the one caller 0x009C8EDD is CTextEntry::ReadFromDataStream 0x009C8DE0. The 8-byte
//   element is Ego_r.pdb CTextTag { long Pos; CCharString Name; }: the implicit default constructor builds only the
//   string (Pos left uninitialised). The catalog's "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize"
//   is propagated.
// Callees: CCharString::CCharString() 0x0099E4B0, vector::erase(first, last) 0x009CACD0, vector::_M_fill_insert(pos, n, x)
//   0x009CAF60, CCharString::~CCharString 0x0099EAE0.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over the inline `resize(n, x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }`: the default element in the 8-byte frame (the string built at +4 first), its address pushed
//   to the fill-insert, and its destructor on both exits.
class CCharString {
public:
    void* m_pData;                                      // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};

namespace NGameText {
class CTextTag {                                        // Ego_r.pdb, 8 bytes
public:
    long        Pos;                                    // +0x00
    CCharString Name;                                   // +0x04
};
}

namespace _STL {
template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last);              // 0x009CACD0
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x009CAF60
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

template void vector<NGameText::CTextTag>::resize(unsigned int);
}