// std::vector::clear() -- (vector<CTestQuest>) STLport erase(begin(), end()): assign the (empty) tail down over the front, destroy the leftover records, and pull _M_finish back to the start
// Leaf: ?clear@?$vector@VCTestQuest@@V?$allocator@VCTestQuest@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ADAA0, 49 bytes, thiscall `ret`, `push ecx` frame (the iterator / type tags).
// Identity: the callees are the CTestQuest instances of STLport's out-of-line helpers -- __copy<CTestQuest*, CTestQuest*, int>
//   0x004AA580 (28-byte stride, six CCharString assignments; landed as Copy_Record28_004aa580) and the landed
//   std::__destroy_aux<CTestQuest*> 0x004ABD90 (CWorld's ActiveTestQuests vector<CTestQuest>; Ego_r.pdb CTestQuest is 28
//   bytes). `copy(last = _M_finish, _M_finish, first = _M_start)` then `_Destroy(i, _M_finish)` and `_M_finish = i` is
//   erase(begin(), end()), i.e. clear(). The row has no direct caller (gap-scan COMDAT; the xref.py hit 0x004F103C decodes
//   to no instruction).
// Callees: std::__copy<CTestQuest*, CTestQuest*, int> 0x004AA580 (__fastcall: first ecx, last edx; result, the
//   random_access_iterator_tag and the null distance pointer on the stack), std::__destroy_aux<CTestQuest*> 0x004ABD90
//   (__fastcall: first ecx, last edx, the __false_type tag by address).
// Shape: STLport 4.6 `erase(first, last)` inlined into clear(); both empty tags live in the one `push ecx` frame dword
//   and have user constructors (else VC7.1 zero-stores their bytes); the copy result is kept in edi for the destroy
//   and the final store.
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);   // 0x004AA580
template <class _Tp>
inline _Tp* copy(_Tp* __first, _Tp* __last, _Tp* __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x004ABD90
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
} // namespace std

class CCharString {
public:
    void* PStringData;                          // 4-byte counted string handle
};

class CTestQuest {                              // Ego_r.pdb, 28 bytes
public:
    CCharString QuestName;                      // +0x00
    CCharString HSPName;                        // +0x04
    long        ColourIndex;                    // +0x08
    CCharString DisplayName;                    // +0x0C
    CCharString IniFile;                        // +0x10
    CCharString OutiFile;                       // +0x14
    CCharString QuestCardDefName;               // +0x18
};

template void std::vector<CTestQuest>::clear();