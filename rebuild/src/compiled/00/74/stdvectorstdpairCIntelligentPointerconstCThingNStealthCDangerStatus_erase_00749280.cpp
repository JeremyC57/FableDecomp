// std::vector<std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>>::erase(iterator first, iterator last) -- STLport range erase: assign the tail [last, finish) down onto first through the out-of-line __copy, destroy each vacated element, set finish to the copy's end, return first
// Leaf: ?erase@?$vector@U?$pair@V?$CIntelligentPointer@$$CBVCThing@@@@VCDangerStatus@NStealth@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749280, 76 bytes, thiscall, `ret 8`, returns first.
// Identity (proven): only caller CTCStealth::UpdateDangerInfo 0x00747C70 erases [remove_if result, end) of the vector at this+0x38 = Ego_r.pdb CTCStealth::DangerThings (vector<pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> >, PDB +56); FableWin.pdb names the same element in ?_Move / _Remove_if<CDeadDangerThing> COMDATs; stride 0x10 and the pair's implicit destructor = the intelligent pointer's base destructor.
// Callees: std::__copy 0x00748AF0 (landed as the 16-byte {intelligent thing pointer, 8-byte record} __copy), CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the pair's implicit destructor).
// Shape (family K020, representative 0x00749280): STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first,
//   _TrivialAss()); _Destroy(__i, _M_finish); _M_finish = __i; return __first;` -- __copy is the /Gr `__fastcall` out-of-line
//   template (last in ecx, finish in edx; first, the tag reference in a dead argument slot and `(ptrdiff_t*)0` on the stack),
//   then the __destroy_aux loop of direct destructor calls (0x10-byte stride). The landed H009 erase 0x00AD7C20 model.
#include <stddef.h>
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThing;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                           // +0x00
    _T2 second;
};
}
namespace NStealth {
enum EDangerStatus { };
class CDangerStatus {                                    // Ego_r.pdb, 8 bytes
public:
    EDangerStatus DangerStatus;                          // +0x00
    long Timer;                                          // +0x04
};
}
typedef std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> _Elem;   // 16 bytes

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>                  // 0x00748AF0 (same-TU body)
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
template vector<_Elem>::iterator vector<_Elem>::erase(vector<_Elem>::iterator, vector<_Elem>::iterator);
} // namespace std