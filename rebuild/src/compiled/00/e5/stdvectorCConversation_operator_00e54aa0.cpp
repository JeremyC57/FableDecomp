// std::vector<CConversation>::operator=(const vector& x) -- STLport copy assignment of a script def's conversation vector (0x5C-byte CConversation defs): reallocate and copy-construct when x outgrows the capacity, else assign over the live prefix (destroying the surplus) or assign the prefix and copy-construct the tail
// Leaf: ??4?$vector@VCConversation@@V?$allocator@VCConversation@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54AA0, 354 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): the callees are the landed CConversation members -- operator= 0x00E54C50 (??4CConversation@@QAEAAV0@ABV0@@Z)
//   in the assignment loops, the copy constructor 0x00E54CA0 (??0CConversation@@QAE@ABV0@@Z) in the construct loop -- and
//   the landed vector<CConversation>::_M_allocate_and_copy 0x00E54E40, with the 0x5C stride; Ego_r.pdb / FableWin.pdb name set
//   ??4?$vector@VCConversation@@V?$allocator@VCConversation@@@std@@@std@@QAEAAV01@ABV01@@Z. Callers 0x00E549AC, 0x00F2AF01.
//   Catalog label std_vector_CConversation_Assign; the bound ??4?$vector@UCConversation@X_00e54990@@@std@X_00e54990@@... is invented.
// Callees: _M_allocate_and_copy 0x00E54E40, _M_clear 0x00E54C10 (destroy + free; landed as the invented
//   ~CRetailVector_poly0x5c), CConversation::operator= 0x00E54C50, _Destroy 0x00CDB790 (__fastcall; landed as the invented
//   Destroy_00cdb790), CConversation::CConversation(const CConversation&) 0x00E54CA0.
// Shape: the STLport 4.6 vector::operator= of the landed vector<CLipSyncPhoneme> 0x00A4A520 / vector<CFireHeartPatternDef>
//   0x00D34D40: here __copy_ptrs and its random-access __copy are `__forceinline` (counted loops of out-of-line element
//   assignments; plain `inline` keeps __copy an out-of-line call, 341v354), __uninitialized_copy is `__forceinline` with the inline _Construct (`new (p) T(v)`: the placement null test, the
//   copy constructor out of line); _M_allocate_and_copy, _M_clear and _Destroy are declarations.
inline void* operator new(unsigned int, void* p) { return p; }
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, 0x28 bytes, vtable 0x01230BCC).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CConversation : public CDefClassBase {            // 0x5C bytes in retail (Ego_r.pdb 108: Dinkumware vectors)
public:
    virtual ~CConversation();                 // slot 0  0x00E54D60 (scalar deleting); vtable 0x012E5FF0 (no RTTI)
    virtual void Transfer(CPersistContext&);            // slot 18 0x00E579E0
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00E54D10
    virtual unsigned long GetSizeofClass();             // slot 20 0x00E54D00
    long Lines;                                         // +0x28
    unsigned char Speaker[0x0c];                        // +0x2c vector<CCharString> Speaker (opaque here)
    unsigned char Dialogue[0x0c];                       // +0x38 vector<CCharString> Dialogue
    unsigned char Animation[0x0c];                      // +0x44 vector<CCharString> Animation
    unsigned char AnimLoop[0x0c];                       // +0x50 vector<CCharString> AnimLoop
    CConversation(const CConversation& other);          // 0x00E54CA0
    CConversation& operator=(const CConversation& other);   // 0x00E54C50
};

namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);                    // 0x00CDB790
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector& operator=(const vector& __x);
protected:
    void _M_clear();                                    // 0x00E54C10
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00E54E40
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template vector<CConversation>& vector<CConversation>::operator=(const vector<CConversation>&);
}   // namespace std