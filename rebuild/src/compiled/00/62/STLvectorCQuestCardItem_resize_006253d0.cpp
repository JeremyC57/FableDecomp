// std::vector<CQuestCardItem>::resize(unsigned int new_size) -- STLport one-argument resize of the quest screen's card-item vector: resize(n, CQuestCardItem()) with the default item destroyed on both exits
// Leaf: ?resize@?$vector@VCQuestCardItem@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006253D0, 108 bytes (+ int3 padding), `__fastcall` (this -> esi), `ret 4`.
// Identity: Ego_r.pdb `?resize@?$vector@VCQuestCardItem@@...@@QAEXI@Z` (CTCInventoryQuests::QuestCardItemList is a
//   vector<CQuestCardItem>); the one caller 0x0061A731 is CTCInventoryQuests::OnCreate 0x0061A6A0. The 8-byte element is
//   Ego_r.pdb CQuestCardItem { long ItemIndex; CWideString ItemText; }: the implicit default constructor builds only the
//   string (ItemIndex left uninitialised) and the implicit destructor destroys it. The catalog's
//   "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize" is propagated.
// Callees: CWideString::CWideString() 0x0099AED0, vector::erase(first, last) 0x006249E0, vector::_M_fill_insert(pos, n, x)
//   0x00624E40, CWideString::~CWideString 0x0099B510.
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over the inline `resize(n, x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }`: the default element in the 8-byte frame (the string built at +4 first), its address pushed
//   to the fill-insert, and its destructor on both exits.
class CWideString {
public:
    void* m_pData;                                      // +0x00
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};

class CQuestCardItem {                                  // Ego_r.pdb, 8 bytes
public:
    long        ItemIndex;                              // +0x00
    CWideString ItemText;                               // +0x04
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
    _Tp* erase(_Tp* __first, _Tp* __last);              // 0x006249E0
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x00624E40
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

template void vector<CQuestCardItem>::resize(unsigned int);
}