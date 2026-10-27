// std::vector::push_back(const CIntelligentPointer<const CThing>& __x) -- (vector<CIntelligentPointer<const CThing> >) append, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@V?$CIntelligentPointer@$$CBVCThing@@@@V?$allocator@V?$CIntelligentPointer@$$CBVCThing@@@@@std@@@std@@QAEXABV?$CIntelligentPointer@$$CBVCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007398B0, 82 bytes, thiscall, `ret 4`.
// Identity (proven): the inline copy stores vtable 0x01237F1C, RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@ (the element); the overflow path calls
//   vector<CIntelligentPointer<const CThing> >::_M_insert_overflow 0x00739550; callers include CTCCombat::AddThingToAlwaysBlock 0x007352D0, CTCSolidToThing::AddThingNotSolidTo 0x00770FA0, CTCGatherPointBase::AddAsUser 0x007FFEB0.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90,
//   _M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x00739550.
// Shape (family M077, representative 0x004AD520): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the intelligent pointer's
//   copy constructor inline (`SetPItem(other.GetPItem())` over the out-of-line base constructor); the tag homed in the x slot.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    void* PData;                                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C for const CThing
public:
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
namespace std {
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00739550
};
template void vector<CIntelligentPointer<const CThing> >::push_back(const CIntelligentPointer<const CThing>& __x);
}