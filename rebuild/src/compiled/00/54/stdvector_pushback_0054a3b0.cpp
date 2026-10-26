// std::vector::push_back(const CCountedPointer<NUISystem::CAction>& x) -- (vector<CCountedPointer<NUISystem::CAction> >) copy-construct x at the end when there is room (adding a reference), else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@V?$CCountedPointer@VCAction@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCAction@NUISystem@@@@@std@@@std@@QAEXABV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054A3B0, 64 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): std::vector<CCountedPointer<NUISystem::CAction> >::push_back(const CCountedPointer<NUISystem::CAction>&); evidence: its only callee is agent950's landed _M_insert_overflow 0x0054A2A0 of vector<CCountedPointer<NUISystem::CAction> > (NUISystem::CTextSlider::Initialize 0x00549B20's vector); it has no direct caller (xr's one absolute reference, 0x0049FB8C, is the rel32 of a call)
// Label: the manifest's "sub_0054a3b0" is propagated/invented (family J054: five byte-identical push_backs, one per element type).
// Callees: std::vector<CCountedPointer<NUISystem::CAction> >::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x0054A2A0 (protected; agent950 landed it).
// Body: STLport 4.6 _vector.h `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, __x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the placement new's null test and the counted pointer's add-ref inline.
// Model: agent950's landed vector<CCountedPointer<T> > / CCountedPointer model (0x00449050 etc.).
inline void* operator new(unsigned int, void* p) { return p; }
class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void* Data;                                             // +0x08
};
template <class T> class CCountedPointer {
public:
    T* Data;                                                // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer();
};
namespace NUISystem { class CAction; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);   // 0x0054A2A0
};
template class vector<CCountedPointer<NUISystem::CAction> >;
}   // namespace std