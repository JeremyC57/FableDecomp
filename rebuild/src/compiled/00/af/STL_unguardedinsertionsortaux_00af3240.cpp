// std::__unguarded_insertion_sort_aux(pair<CCharString, NParticleEngine::CRegisteredPSCInfo>* first, pair<CCharString, NParticleEngine::CRegisteredPSCInfo>* last, pair<CCharString, NParticleEngine::CRegisteredPSCInfo>*, CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo> comp) -- unguarded insertion sort of [first, last): each element, copied by value, goes through the unguarded linear insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3240, 75 bytes, __fastcall (first ecx, last edx; the null value-type pointer and
//   the comparator on the stack, `ret 8`).
// Callers: 0x00AF3427, 0x00AF38FC and the landed sort 0x00AF3B70 (its inlined __final_insertion_sort, 0x00AF3BC9).
// Callees: __unguarded_linear_insert(last, val, comp) 0x00AF2E70, CCharString copy constructor 0x0099EC30.
// Element: STLport pair<CCharString, NParticleEngine::CRegisteredPSCInfo> (16 bytes; Ego_r.pdb CRegisteredPSCInfo = AllocFunc +0,
//   EPSCType ComponentType +4, CCharString GUIName +8) -- the particle-component registry's CVectorMap entries (the
//   catalog names the TU's heap helper _Adjust_heap<pair<CCharString, NParticleEngine::CRegisteredPSCInfo>*, ...,
//   CKeyPairCompareLess<...> >); the pair copy is memberwise (key string, AllocFunc, ComponentType, GUIName string).
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

namespace NParticleEngine {
class CPSCBase;
enum EPSCType { PSC_TYPE_NULL };
class CRegisteredPSCInfo {                      // Ego_r.pdb, 12 bytes
public:
    CPSCBase* (__cdecl* AllocFunc)();           // +0x0
    EPSCType    ComponentType;                  // +0x4
    CCharString GUIName;                        // +0x8
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCInfoEntry;   // 16 bytes
typedef CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCInfoLess;

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00AF2E70

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template void __fastcall __unguarded_insertion_sort_aux(CPSCInfoEntry*, CPSCInfoEntry*, CPSCInfoEntry*, CPSCInfoLess);
}