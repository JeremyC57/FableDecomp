// std::vector<CFireHeartPatternDef>::operator=(const vector& x) -- STLport copy assignment of the Fire Heart def's pattern vector (0x34-byte defs): reallocate and copy-construct when x outgrows the capacity, else assign over the live prefix (destroying the surplus) or assign the prefix and copy-construct the tail
// Leaf: ??4?$vector@VCFireHeartPatternDef@@V?$allocator@VCFireHeartPatternDef@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D34D40, 315 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): the callees are the landed vector<CFireHeartPatternDef> helpers -- _M_allocate_and_copy 0x00D35020
//   (??$_M_allocate_and_copy@PBVCFireHeartPatternDef@@@?$vector@VCFireHeartPatternDef@@...@IAE...), __copy_ptrs 0x00D34FD0
//   (the CFireHeartPatternDef range assignment) and _Construct 0x00D34EC0 (??$_Construct@VCFireHeartPatternDef@@V1@@std@@...)
//   -- with the 0x34 stride; Ego_r.pdb / FableWin.pdb name set
//   ??4?$vector@VCFireHeartPatternDef@@V?$allocator@VCFireHeartPatternDef@@@std@@@std@@QAEAAV01@ABV01@@Z (the CScriptDef
//   FHPatterns member). Callers 0x00D348C2, 0x00F2C3A1. Catalog label std_vector_Conversation_Assign names the wrong
//   element (CConversation is 0x5C: its operator= is 0x00E54AA0); the binding ??4CFireHeartPatternVector@@QAEAAU0@ABU0@@Z
//   is invented.
// Callees: _M_allocate_and_copy 0x00D35020, _M_clear 0x00D34E80 (destroy + free; landed as the invented
//   ~CRetailVector_poly0x34), __copy_ptrs 0x00D34FD0 (__fastcall), _Destroy 0x00CDB7C0 (__fastcall; landed as the invented
//   Destroy_00cdb7c0), _Construct 0x00D34EC0 (__fastcall).
// Shape: the STLport 4.6 vector::operator= of the landed vector<CLipSyncPhoneme> 0x00A4A520 with this instance's out-of-line
//   helpers: _M_allocate_and_copy, _M_clear, __copy_ptrs, _Destroy and _Construct are declarations; __uninitialized_copy is
//   `__forceinline` (plain `inline` leaves it an out-of-line call, 290v315). __false_type has no user constructor: retail
//   zero-stores the __copy_ptrs tag in the dead argument slot (the two __copy_ptrs calls share their pushes).
// Layout: the retail polymorphic def-class chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase, 0x28 bytes, vtable 0x01230BCC).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CFireHeartPatternDef : public CDefClassBase {     // 0x34 bytes in retail (Ego_r.pdb 60: Dinkumware vector<bool>)
public:
    virtual ~CFireHeartPatternDef();          // slot 0  0x00D34F60 (scalar deleting); vtable 0x012CCC50 (no RTTI)
    virtual void Transfer(CPersistContext&);            // slot 18 0x00D35070
    virtual void Copy(const CDefClassBase*);            // slot 19 0x00D34FB0
    virtual unsigned long GetSizeofClass();             // slot 20 0x00D34EF0
    unsigned char PadActive[0x0c];                      // +0x28 vector<bool> PadActive (opaque here)
};

namespace std {
struct __false_type { };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&);   // 0x00D34FD0
template <class _T1, class _T2>
void __fastcall _Construct(_T1* __p, const _T2& __val);                                         // 0x00D34EC0
template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);                    // 0x00CDB7C0
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
    void _M_clear();                                    // 0x00D34E80
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last);   // 0x00D35020
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

template vector<CFireHeartPatternDef>& vector<CFireHeartPatternDef>::operator=(const vector<CFireHeartPatternDef>&);
}   // namespace std