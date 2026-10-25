// _STL::__unguarded_insertion_sort_aux(pair<CCharString, CScriptInfo>* first, pair<CCharString, CScriptInfo>* last, pair<CCharString, CScriptInfo>*, CKeyPairCompareLess<CCharString, CScriptInfo> comp) -- unguarded insertion sort of [first, last): each element, copied by value, goes through the unguarded linear insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB6860, 94 bytes, __fastcall (first ecx, last edx; the null value-type pointer and
//   the comparator on the stack, `ret 8`).
// Callers: 0x00CB6AF7 and 0x00CB7116 (the script registry's sort; the TU's landed __adjust_heap 0x00CB68C0 is the same
//   pair and comparator).
// Callees: __unguarded_linear_insert(last, val, comp) 0x00CB6420, CCharString copy constructor 0x0099EC30.
// Element: STLport pair<CCharString, CScriptInfo> (24 bytes; Ego_r.pdb CScriptInfo = AllocFunc +0, MasterScriptDataAlloc +4,
//   CCharString Name +8, long ID +0xC, bool MasterScript +0x10) -- memberwise copy: key string, the two function
//   pointers, Name, ID, MasterScript.
// Label: catalog "vector<...>::_Destroy" is propagated; the body is STLport's sort helper.
// Shape: STLport 4.6 `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the
//   by-value element is copy-constructed in its outgoing argument slot (the copy expanded inline); the comparator's
//   operator() stays declaration-only (never called here).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;               // +0x0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptInfo {                             // Ego_r.pdb, 20 bytes
public:
    CScriptBase* (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                              // +0x04
    CCharString Name;                           // +0x08
    long        ID;                             // +0x0C
    bool        MasterScript;                   // +0x10
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const;
};

typedef _STL::pair<CCharString, CScriptInfo> CScriptInfoEntry;            // 24 bytes
typedef CKeyPairCompareLess<CCharString, CScriptInfo> CScriptInfoLess;

namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CB6420

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(CScriptInfoEntry*, CScriptInfoEntry*, CScriptInfoEntry*, CScriptInfoLess);
}