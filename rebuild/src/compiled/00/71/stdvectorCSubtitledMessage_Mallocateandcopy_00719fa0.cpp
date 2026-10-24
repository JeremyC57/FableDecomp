// std::vector<CSubtitledMessage>::_M_allocate_and_copy(size_type n, const CSubtitledMessage* first, const CSubtitledMessage* last) -- allocate n subtitled messages and copy-construct [first, last) into them
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00719FA0, length 77 bytes. __fastcall (`this` unused), `ret 0Ch`, the block in eax.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E, CSubtitledMessage::CSubtitledMessage(const CSubtitledMessage&) @ 0x004D1FE0.
// Identity: the manifest label "Vector_AllocAndConstruct_CSubtitledMessage" is descriptive. Single caller 0x0071A1B4 (the
//   vector<CSubtitledMessage> assignment); retail CSubtitledMessage is 0x44 bytes (`imul eax,eax,44h`: Ego_r.pdb 0x50
//   minus 4 for each of the three CVectorMap vectors).
// Shape: STLport `_M_allocate_and_copy` member template: `allocate(n)`, then the inline `__uninitialized_copy(first,
//   last, result, __false_type)` placement loop calling the out-of-line copy constructor.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)
struct __false_type { __false_type() {} };

class CSubtitledMessage {                               // retail 0x44 bytes (Ego_r.pdb 0x50 minus 4 per CVectorMap vector)
public:
    unsigned char _body[0x44];
    CSubtitledMessage(const CSubtitledMessage& other);  // 0x004D1FE0
};

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {   // the end-of-storage pointer
public:
    _Value _M_data;
};

template <class _Tp> class vector {                     // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
};

template CSubtitledMessage* vector<CSubtitledMessage>::_M_allocate_and_copy<const CSubtitledMessage*>(unsigned int,
    const CSubtitledMessage*, const CSubtitledMessage*);
}