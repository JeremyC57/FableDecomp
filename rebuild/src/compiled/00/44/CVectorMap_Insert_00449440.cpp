// CVectorMap::Insert(const K& key, const V& value) -- (CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase>, CKeyPairCompareLess<...> >) append a {key, counted pointer} entry to the map's vector (unsorted) and mark the map dirty
// Leaf: ?Insert@?$CVectorMap@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@@QAEXABW4EGameAction@@ABV?$CCountedPointer@$$CBVCInputTypeBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449440, 133 bytes, thiscall `ret 8`; no direct callers (an emitted COMDAT).
// Identity (proven): CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase>, CKeyPairCompareLess<EGameAction, CCountedPointer<const CInputTypeBase> > >::Insert(const EGameAction&, const CCountedPointer<const CInputTypeBase>&); evidence: its _M_insert_overflow 0x00449130 is also called at 0x004472BA by the function that appends a 12-byte entry to CGamePlayerInterface::Controls (+0x7A8), sets Dirty and sorts it through 0x004495D0 (SetControlScheme, H047); Ego_r.pdb / FableWin.pdb name ?Insert@?$CVectorMap@ for exactly the six H047 pair types (plus CCharString / CEntityScriptBindingBase and unsigned __int64 / CActiveEntityScriptBase, other shapes).
// Label: the manifest's "Vector_InsertElement" is propagated (family H051: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one map type).
// Callees: std::vector<pair<EGameAction, CCountedPointer<const CInputTypeBase> > >::_M_insert_overflow 0x00449130 and operator delete
//   0x00BFE9BC (the temporary's counted pointer released).
// Shape: landed CVectorMap<long, CSoundBankRecord>::Insert 0x009937D0 model: `Data.push_back(value_type(key, value));
//   Dirty = true;` -- the pair temporary in the frame (`inc [Info]`), push_back inlined (placement copy under its null
//   test; overflow with the __false_type tag homed on the dead key slot), the temporary destroyed and Dirty (+0xD) set.
//   Retail keeps key / Data / Info in ecx / edx / esi across the `inc [Info]` although the temporary's address goes to
//   _M_insert_overflow: that needs the overflow's STLport body (and the uninitialized-copy / fill / destroy helpers it
//   calls) defined in this TU, as retail emits them here (0x00449130); with the overflow only declared VC7.1 treats the
//   escaped temporary as aliased and reloads it after every reference-count write (166 v 133).
#include <new>
#include <stddef.h>
#include <stdlib.h>
struct CCPPointerInfo {                                    // CCountedPointer's shared block
    long  RefCount;                                        // +0x0
    void  (__fastcall* DeleteFunc)(void* data);            // +0x4
    void* Data;                                            // +0x8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x0
    CCPPointerInfo* Info;                                  // +0x4
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        T* const              data = o.Data;
        CCPPointerInfo* const info = o.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
enum EGameAction { EGameAction_Force32 = 0x7fffffff };   // enumerators not needed
class CInputTypeBase;

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __p) { __p->~_Tp(); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(__cur, __x);
    return __cur;
}
template <class _ForwardIter>
void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x0
    _Tp* _M_finish;                                        // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false);         // 0x00449130
    size_t size() const { return (size_t)(this->_M_finish - this->_M_start); }
    _Tp* _M_allocate(size_t __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void _M_clear()
    {
        __destroy_aux(this->_M_start, this->_M_finish, __false_type());
        if (this->_M_start) free(this->_M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e) { this->_M_start = __s; this->_M_finish = __f; this->_M_end_of_storage._M_data = __e; }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                                            bool __atend)   // STLport 4.6 _vector.c
{
    const size_t __old_size = size();
    const size_t __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    _Tp* __new_start = _M_allocate(__len);
    _Tp* __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
} // namespace std
template <class K, class V> class CKeyPairCompareLess {    // Ego_r.pdb: orders vector-map entries by key
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                          // +0x0
    C    Compare;                                          // +0xc (empty)
    bool Dirty;                                            // +0xd
    void Insert(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
};
typedef EGameAction _Key;
typedef CCountedPointer<const CInputTypeBase> _Value;
template void CVectorMap<_Key, _Value>::Insert(const _Key&, const _Value&);