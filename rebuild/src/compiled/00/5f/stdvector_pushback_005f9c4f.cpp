// std::vector::push_back(const std::pair<long, EHeroAbility>& __x) -- (vector<std::pair<long, EHeroAbility> >) append a copy, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@U?$pair@JW4EHeroAbility@@@std@@V?$allocator@U?$pair@JW4EHeroAbility@@@std@@@2@@std@@QAEXABU?$pair@JW4EHeroAbility@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9C4F, 57 bytes, thiscall, `ret 4`, ebp frame.
// Identity (probable): called from the landed CTCInventoryAbilities::SetAbilityToButton 0x005F6423 on AbilityButtons (Ego_r.pdb CVectorMap<long, EHeroAbility, ...>, a vector of pairs) and from the CVectorMap Add 0x005F9E36; its overflow is M147 0x005F957E.
// Callees: vector::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x005F957E.
// Shape (family M148, representative 0x005947B6): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the pair's explicit copy
//   constructor (two dword moves, null-checked placement); `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
enum EHeroAbility { HERO_ABILITY_NULL = 0 };
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x005F957E
};
template void vector<std::pair<long, EHeroAbility> >::push_back(const std::pair<long, EHeroAbility>& __x);
}