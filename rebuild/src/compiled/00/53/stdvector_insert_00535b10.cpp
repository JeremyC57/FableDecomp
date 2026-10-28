// std::vector::insert(iterator position, const value_type& x) -- (vector<CCountedPointer<NUISystem::CComponent> >) STLport single-element insert: with room left, append at the end or shift the tail up one slot (copy-construct the last element, copy_backward the rest, assign a saved copy of x into the hole); otherwise grow through _M_insert_overflow; returns begin() + the position's index
// Leaf: ?insert@?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@QAEPAV?$CCountedPointer@VCComponent@NUISystem@@@@PAV3@ABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00535B10, 247 bytes, thiscall, `ret 8`.
// Identity (proven): STLport 4.6 _vector.h `iterator insert(iterator __position, const _Tp& __x)` of
//   vector<CCountedPointer<NUISystem::CComponent> >: its callers are NUISystem::CComponent::AddChildFront 0x00533BA0
//   (Children.insert(Children.begin(), child), Children at +0xB0; Ego_r.pdb AddChildFront(CCountedPointer<CComponent>&))
//   and 0x00546012; its callees are this element's landed COMDATs __copy_backward 0x00534F80 (STLport spelling, same
//   element) and _M_insert_overflow 0x005359D0 (same element, `push 0 ; push 1` = fill_len 1, atend false).
// Label: the manifest's "std::vector_insert_CCountedPointer" is behavioural; the landed callers bind it under the
//   invented CRetailVector / CComponentPointerVector stand-ins (NAME_FIXES.tsv).
// Callees: _M_insert_overflow(pos, x, const __false_type&, 1, false) 0x005359D0, __copy_backward(first, last, result,
//   const random_access_iterator_tag&, int*) 0x00534F80 (__fastcall), CCountedPointer<CComponent>::operator= 0x00429C58
//   (out of line, the /Os COMDAT the linker kept), operator delete 0x00BFE9BC (the x copy's last release).
// Shape: __copy_backward_ptrs (the __false_type overload) forwards to __copy_backward with a random_access_iterator_tag
//   temporary homed in the dead `x` argument slot (a same-TU body of the real __copy_backward makes VC home it there),
//   the saved x copy lives in the two frame words, its destructor is the inline ReleaseData (Data/Info zeroing dead).
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0
    void (__fastcall* DeleteFunc)(void* data);              // +4, Data in ecx
    void* Data;                                             // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00429C58 for CComponent (same-TU body below)
    void ShareData(T* data, CCPPointerInfo* info);              // 0x0042920A for CComponent
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};
// The retail operator= COMDAT (0x00429C58, `push [other+4] ; push [other] ; call ShareData ; mov eax,esi`): a same-TU
// body that only reads `other` lets VC keep the saved copy's Info in a register across the call.
#pragma auto_inline(off)
template <class T> __declspec(noinline) CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer& other)
{
    ShareData(other.Data, other.Info);
    return *this;
}
#pragma auto_inline(on)
namespace NUISystem { class CComponent; }

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _Tp>
inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

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
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);   // 0x005359D0
public:
    iterator insert(iterator __position, const _Tp& __x)
    {
        size_type __n = __position - begin();
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            if (__position == end()) {
                _Construct(this->_M_finish, __x);
                ++this->_M_finish;
            }
            else {
                _Construct(this->_M_finish, *(this->_M_finish - 1));
                ++this->_M_finish;
                _Tp __x_copy = __x;
                __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, _TrivialAss());
                *__position = __x_copy;
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), 1UL);
        return begin() + __n;
    }
};

template class vector<CCountedPointer<NUISystem::CComponent> >;
}   // namespace std