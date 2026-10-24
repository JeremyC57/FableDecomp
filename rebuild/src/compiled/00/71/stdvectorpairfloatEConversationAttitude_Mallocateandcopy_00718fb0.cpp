// std::vector<pair<float,EConversationAttitude>>::_M_allocate_and_copy(size_type n, const pair* first, const pair* last) -- allocate n 8-byte (time, attitude) pairs and copy-construct [first, last) into them
// Leaf: ??$_M_allocate_and_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00718FB0, length 79 bytes. __fastcall (`this` unused), `ret 0Ch`, the block in eax.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E.
// Identity: the manifest label "Vector_AllocAndCopy_8byte" is descriptive. Single caller: vector<pair<float,
//   EConversationAttitude> >::operator= 0x00719470 (its manifest label "CSubtitledMessage::Assign_Pairs"; the
//   CSubtitledMessage::AttitudeTags CVectorMap<float, EConversationAttitude> storage).
// Shape: STLport `_M_allocate_and_copy` member template: `allocate(n)` (`n != 0 ? malloc(n * 8) : 0`), then the inline
//   `__uninitialized_copy(first, last, result, __false_type)` placement loop (null test, strength-reduced to the
//   result - first offset in esi). STLport's pair carries an explicit copy constructor (`pair(const pair<_T1,_T2>&)`
//   next to the converting template): with the implicit one VC7.1 keeps the tested pointer for the second word
//   (`mov ebx,[ecx+4] ; mov [edx+4],ebx`) instead of retail's `mov edx,[ecx+4] ; mov [esi+ecx+4],edx`.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)
struct __false_type { __false_type() {} };

enum EConversationAttitude { CONVERSATION_ATTITUDE_DUMMY };

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

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template pair<float, EConversationAttitude>* vector<pair<float, EConversationAttitude> >::_M_allocate_and_copy<const pair<float, EConversationAttitude>*>(
    unsigned int, const pair<float, EConversationAttitude>*, const pair<float, EConversationAttitude>*);
}