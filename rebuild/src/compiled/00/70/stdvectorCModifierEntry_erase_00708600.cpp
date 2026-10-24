// std::vector<CModifierEntry>::erase(iterator position) -- STLport single-element erase of an 8-byte CTCHeroAttachableAppearanceModifiers::CModifierEntry: copy the tail down one slot, drop the last element, return the position
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00708600, length 63 bytes. __fastcall (this in ecx), `ret 4`.
// Callees: none (the copy and the trivial destroy are inline).
// Identity: the manifest label "AppearanceModifier_VectorErase" is descriptive. Single caller is
//           CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifier (0x00706370, landed), which calls it as
//           `entries.erase(it)` on AppearanceModifiers[type] (a vector<CModifierEntry>).
// Shape: STLport 4.x `vector::erase(iterator)` verbatim: `if (pos + 1 != end()) __copy_ptrs(pos + 1, _M_finish, pos,
//        __false_type())` -> the counted random-access `__copy` (`n = last - first; n > 0; --n`, hence `sar 3 ;
//        test ; jle` and the dec/jne loop), then `--_M_finish` (re-read after the copy) and a trivial `_Destroy`.
//        The 8-byte element is copied as two dwords (implicit POD assignment of {long, bool} copies the padding too).
struct CModifierEntry {                             // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                  // +0x00 global def index
    bool Rendered;                                  // +0x04
};

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> inline void _Destroy(_Tp*) {}

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* end() { return _M_finish; }
    T* erase(T* __position);
};

template <class T> T* CRetailVector<T>::erase(T* __position)
{
    if (__position + 1 != end())
        __copy_ptrs(__position + 1, _M_finish, __position, __false_type());
    --_M_finish;
    _Destroy(_M_finish);
    return __position;
}

template struct CRetailVector<CModifierEntry>;