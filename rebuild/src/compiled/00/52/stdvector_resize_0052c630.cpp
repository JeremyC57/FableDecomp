// std::vector::resize(size_type new_size, const value_type& x) -- (vector<NThingManager::CEntityRuntimePersistence::CEntityScriptInfo >) STLport resize: shrink by erasing [begin() + new_size, end()) (copy down, destroy the stale tail), or grow by inserting new_size - size() copies of x at the end
// Leaf: ?resize@?$vector@VCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@V?$allocator@VCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@@std@@@std@@QAEXIABVCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052C630, 105 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): std::vector<NThingManager::CEntityRuntimePersistence::CEntityScriptInfo >::resize(unsigned int, const NThingManager::CEntityRuntimePersistence::CEntityScriptInfo&); evidence: its only caller is resize(n) 0x0052C6A0 (binding ?resize@CEntityScriptArray@CEntityRuntimePersistence@NThingManager@@QAEXI@Z -- an invented container name for CArray<CEntityScriptInfo>), run by CEntityRuntimePersistence::SetGamePersistentSaveData 0x00523860 and its neighbours on LevelPersistentEntityScripts / GamePersistentEntityScripts (Ego_r.pdb CArray<NThingManager::CEntityRuntimePersistence::CEntityScriptInfo>); the destroy callee 0x0052BD70 is bound over CEntityScriptInfo, the retail element is 24 bytes (Script, SaveData CArray<unsigned char>, UncompressedSize, Valid), and FableWin.pdb holds ?resize@?$vector@VCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@...@std@@QAEXI@Z.
// Label: the manifest's "resize" is right in kind but propagated from a byte-identical body elsewhere (family J087: the
//   105-byte resize(n, x) of 20/24/40-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: __copy<T*, T*, int>(first, last, result, const random_access_iterator_tag&, int*) 0x0052BD10 (erase's __copy_ptrs,
//   __fastcall, `ret 0Ch`); __destroy_aux<T*>(first, last, const __false_type&) 0x0052BD70 (its bindings ?DestroyRange@@ / ??$destroy@U__false_type@X_0052bff0@@ / ?Destroy_0052bd70@NThingManager@@ are invented spellings) (erase's _Destroy, __fastcall,
//   `ret 4`); _M_fill_insert(pos, n, x) 0x0052C300 (insert(end(), n, x), thiscall, `ret 0Ch`).
// Shape: STLport 4.6 _vector.h `resize(n, x)` with erase(first, last) inline; the tag temporaries live in the dead new_size slot.
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
protected:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);   // out of line
};
} // namespace std
class CCharString {                                        // 4 bytes (TLC)
public:
    void* PStringData;                                     // +0x00
};
template <class T> class CArray : public std::vector<T> {  // 12 bytes in retail (16 in Ego_r.pdb)
};
namespace NThingManager {
class CEntityRuntimePersistence {                          // partial view: only the nested type is used here
public:
    class CEntityScriptInfo {                              // 24 bytes in retail (28 in Ego_r.pdb)
    public:
        CCharString Script;                                // +0x00
        CArray<unsigned char> SaveData;                    // +0x04
        long UncompressedSize;                             // +0x10
        bool Valid;                                        // +0x14
    };
};
}
typedef NThingManager::CEntityRuntimePersistence::CEntityScriptInfo TElement_0052c630;
template void std::vector<TElement_0052c630 >::resize(unsigned int, const TElement_0052c630&);