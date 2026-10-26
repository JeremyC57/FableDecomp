// std::__make_heap(pair<CCharString,CScriptInfo>* __first, pair<CCharString,CScriptInfo>* __last, CKeyPairCompareLess __comp, pair<CCharString,CScriptInfo>*, int*) -- STLport make_heap core: sift every parent from (len-2)/2 down to the root with __adjust_heap
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB6BD0, 134 bytes. __fastcall: __first ecx, __last edx, the comparator
// word and the two type-tag pointers on the stack (`ret 0Ch`). The manifest label "ScriptInfo_BuildHeapify" is
// descriptive.
// Callees: __adjust_heap 0x00CB68C0 (__fastcall: first ecx, parent edx, len, the by-value element, comp), CCharString
// copy constructor 0x0099EC30.
// Shape: the STLport body verbatim; `__last - __first < 2` and `__len` share one divide (`imul 2AAAAAABh`), the
// `_Tp(*(__first + __parent))` temporary is copy-constructed straight into __adjust_heap's argument area (the pair copy
// constructor, then CScriptInfo's implicit one), __first lives in the `push ecx` frame word.
// Model: STLport 4.6 algorithms (namespace std, the /Gr TU makes the templates __fastcall) over the script registry's
// 0x18-byte std::pair<CCharString, CScriptInfo> (CScriptInfoManager::Scripts, a CVectorMap sorted with
// CKeyPairCompareLess). Ego_r.pdb CScriptInfo (0x14): AllocFunc +0, MasterScriptDataAlloc +4, CCharString Name +8,
// long ID +0xc, bool MasterScript +0x10 (implicit copy and assignment) -- the model of the landed __insertion_sort
// 0x00CB6EA0, __pop_heap 0x00CB6B00 and __copy_backward 0x00CB62F0. STLport's pair has a user copy constructor;
// CCharString::operator< is the inline handle compare in front of CBasicString<char>::operator< 0x00429950.
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;    // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;                          // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo {                                     // Ego_r.pdb, 0x14 bytes
public:
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                              // +0x08
    long             ID;                                // +0x0c
    bool             MasterScript;                      // +0x10
};

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V>
class CKeyPairCompareLess { public:                            // empty, passed by value (one stack word)
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<CCharString, CScriptInfo>   SScriptInfoPair;
typedef CKeyPairCompareLess<CCharString, CScriptInfo> SScriptInfoLess;

namespace std {
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex,
                              _Distance __len, _Tp __val, _Compare __comp);    // 0x00CB68C0

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first,
                            _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

template void __fastcall __make_heap<SScriptInfoPair*, SScriptInfoLess, SScriptInfoPair, int>(
    SScriptInfoPair* __first, SScriptInfoPair* __last, SScriptInfoLess __comp, SScriptInfoPair*, int*);
}