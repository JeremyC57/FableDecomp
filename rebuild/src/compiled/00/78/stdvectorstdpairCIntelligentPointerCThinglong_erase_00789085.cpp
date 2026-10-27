// std::vector<std::pair<CIntelligentPointer<CThing>, long>>::erase(std::pair<CIntelligentPointer<CThing>, long>* __position) -- remove one element: shift the tail down, destroy the old last slot
// Leaf: ?erase@?$vector@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@V?$allocator@U?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@@2@@std@@QAEPAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@2@PAU32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00789085, 52 bytes, thiscall, `ret 4` (frame-pointer module).
// Identity (proven): its __copy 0x00788F76 is bound ??$__copy@PAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@... (12-byte stride); the destructor is the pair's intelligent pointer's base ~CBaseIntelligentPointer 0x00A01C10; caller CTCTrapBase::FrameUpdate 0x007888B4.
// Callees: std::__copy<T*, T*, int> 0x00788F76 (`__fastcall`), the element destructor 0x00A01C10.
// Shape (family M123, representative 0x0053CD05): STLport 4.6 `if (__position + 1 != end()) __copy_ptrs(__position + 1,
//   _M_finish, __position, _TrivialAss()); --_M_finish; _Destroy(_M_finish); return __position;` -- __copy out of line with
//   the destructor called directly (the qualified `_Tp::~_Tp()`: under /Os VC7.1 routes an unqualified `p->~T()`
//   through the class's `??_G` with flags 0, which retail does not do here); __copy's empty tag homed in the top byte of the position slot (`optimize("y", off)` + `optimize("s", on)`) (`lea eax,[ebp+0Bh]`) and a null distance pointer.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}
class CBaseObjectPointer;
class CBaseIntelligentPointer {                           // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObjectPointer* PData;
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
namespace std {
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->_Tp::~_Tp(); }   // qualified: a direct destructor call
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    typedef __false_type _TrivialAss;
    iterator end() { return _M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, _TrivialAss());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template vector<std::pair<CIntelligentPointer<CThing>, long> >::iterator vector<std::pair<CIntelligentPointer<CThing>, long> >::erase(vector<std::pair<CIntelligentPointer<CThing>, long> >::iterator);
}