// _STL::vector<pair<CCharString,unsigned long> >::resize(unsigned int new_size) -- STLport one-argument resize of the particle program's export-name map storage: resize(n, pair()) with the default pair destroyed on both exits
// Leaf: ?resize@?$vector@U?$pair@VCCharString@@K@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7D10, 116 bytes (+ int3 padding), `__fastcall` (this -> esi), `ret 4`.
// Identity: Ego_r.pdb `?resize@?$vector@U?$pair@VCCharString@@K@std@@...@@QAEXI@Z` (CParticleProgram::ExportParticles is
//   a CVectorMap<CCharString, unsigned long>, a vector of pairs); the one caller 0x00AE4419 is CParticleProgram::ReadFromFile
//   0x00AE3C30. STLport `pair() : first(_T1()), second(_T2())` builds the string at +0 and zeroes the count (the zero store
//   scheduled after the compare); the implicit ~pair destroys the string. The catalog's
//   "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize" is propagated.
// Callees: CCharString::CCharString() 0x0099E4B0, vector::erase(first, last) 0x00AE6A20, vector::_M_fill_insert(pos, n, x)
//   0x00AE7340, CCharString::~CCharString 0x0099EAE0.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over the inline `resize(n, x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }`: the default element in the 8-byte frame (string at +0, then the zero second word after the compare), its address pushed
//   to the fill-insert, and its destructor on both exits.
class CCharString {
public:
    void* m_pData;                                      // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair() : first(_T1()), second(_T2()) { }
};

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last);              // 0x00AE6A20
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x00AE7340
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

template void vector<pair<CCharString, unsigned long> >::resize(unsigned int);
}