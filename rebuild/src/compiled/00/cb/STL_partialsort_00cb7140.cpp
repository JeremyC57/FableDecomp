// std::__partial_sort(CScriptInfoPair* first, CScriptInfoPair* middle, CScriptInfoPair* last, CScriptInfoPair*, CScriptInfoLess comp) -- partial sort of the script registry's (name, script info) pairs by name: heapify [first, middle), pop every smaller later pair into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB7140, 162 bytes (`ret 0Ch`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __partial_sort over (name, script info) = std::pair<CCharString, CScriptInfo> (0x18 bytes, CKeyPairCompareLess -> CCharString::operator<);
//   called 7x from the script-info introsort/partial_sort wrappers (0x00CB728C, 0x00CB7668..0x00CB776A). The manifest
//   label "ScriptInfo_Heapsort" is descriptive.
// Callees: __make_heap 0x00CB6BD0, __pop_heap 0x00CB6B00, sort_heap 0x00CB6F10, CBasicString<char>::operator<
//   0x00429950 (inside the inline CCharString::operator<), CCharString copy constructor 0x0099EC30.
// Shape: make_heap is the inline STLport wrapper (it only forwards to __make_heap with the value/distance tags);
//   the `_Tp(*__i)` temporary for __pop_heap is copy-built straight into its argument slot (`sub esp,18h`).
template <class T> class CBasicString {
public:
    T* m_pData;
    bool operator<(const CBasicString& other) const;      // 0x00429950
};
struct CStringData {                                       // shared string payload: the CBasicString at +0
    CBasicString<char> Data;
};
class CCharString {
public:
    CStringData* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return PStringData->Data < other.PStringData->Data;
    }
};
class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptInfo {                                        // Ego_r.pdb, 0x14 bytes
public:
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                                 // +0x08
    long             ID;                                   // +0x0c
    bool             MasterScript;                         // +0x10
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<CCharString, CScriptInfo> CScriptInfoPair;   // 0x18 bytes
typedef CKeyPairCompareLess<CCharString, CScriptInfo> CScriptInfoLess;
namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00CB6BD0
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*);   // 0x00CB6B00
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00CB6F10

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CScriptInfoPair*)0, (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<CScriptInfoPair*, CScriptInfoPair, CScriptInfoLess>(CScriptInfoPair*, CScriptInfoPair*, CScriptInfoPair*, CScriptInfoPair*, CScriptInfoLess);
}