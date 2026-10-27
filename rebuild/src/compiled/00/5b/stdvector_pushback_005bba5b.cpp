// std::vector::push_back(const std::pair<EClothingSuitPart, NUISystem::CComponent*>& __x) -- (vector<std::pair<EClothingSuitPart, NUISystem::CComponent*> >) append a copy, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@U?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@std@@V?$allocator@U?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@std@@@2@@std@@QAEXABU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BBA5B, 57 bytes, thiscall, `ret 4`, ebp frame.
// Identity (proven): its only caller is the landed CTCInventoryClothing::AddClothingMenuEntry 0x005B4771 on ClothingMenuEntries (Ego_r.pdb vector<pair<EClothingSuitPart, NUISystem::CComponent*> >); its overflow is M147 0x005BB409.
// Callees: vector::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x005BB409.
// Shape (family M148, representative 0x005947B6): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the pair's explicit copy
//   constructor (two dword moves, null-checked placement); `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE = 0 };
namespace NUISystem { class CComponent; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) { }   // STLport's explicit copy
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x005BB409
};
template void vector<std::pair<EClothingSuitPart, NUISystem::CComponent*> >::push_back(const std::pair<EClothingSuitPart, NUISystem::CComponent*>& __x);
}