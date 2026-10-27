// std::vector::push_back(const std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo>& __x) -- (vector<std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo>>) append, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@U?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@std@@V?$allocator@U?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@std@@@2@@std@@QAEXABU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743B30, 58 bytes, thiscall, `ret 4`.
// Identity (proven): the overflow insert 0x00743270 is bound ?_M_insert_overflow@?$vector@U?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@std@@...; callers are the eight mode registrations of Init_RegisterGameComponents 0x0073B130 into the CVectorMap<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> (Ego_r.pdb) whose base is this vector.
// Callees: vector<std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo>>::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x00743270.
// Shape (family M027, representative 0x004428B0): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the tag temporary is homed in
//   the x argument slot only when __false_type has a user default constructor (else a zero-filled frame byte).
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CModeBase;
class CThing;
class CTCCreatureModeManager {
public:
    class CRegisteredModeInfo {                           // Ego_r.pdb: 4 bytes
    public:
        CModeBase* (__cdecl* AllocFunc)(CThing*);
    };
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
namespace std {
struct __true_type { };
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);
};
template void vector<std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> >::push_back(const std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo>& __x);
}