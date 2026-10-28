// std::__linear_insert(pair<CCharString,CScriptInfo>* __first, pair<CCharString,CScriptInfo>* __last, pair<CCharString,CScriptInfo> __val, CKeyPairCompareLess __comp) -- STLport insertion-sort step: a new minimum shifts [first, last) up one with copy_backward, else the unguarded linear insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB6CE0, 202 bytes. __fastcall: __first ecx, __last edx, the 0x18-byte
// __val and the comparator word on the stack (`ret 1Ch`); its only caller is __insertion_sort 0x00CB6EA0. The manifest
// label "ScriptInfo_InsertElement" is descriptive.
// Callees: CBasicString<char>::operator< 0x00429950, __copy_backward 0x00CB62F0 (first, last, result, const
// random_access_iterator_tag&, ptrdiff_t*), CCharString::operator= 0x0099EFB0, CCharString copy constructor
// 0x0099EC30, __unguarded_linear_insert 0x00CB6420, ~CCharString 0x0099EAE0.
// Shape: the STLport body; copy_backward(__first, __last, __last + 1) reaches the out-of-line random-access
// __copy_backward with the iterator-category temporary homed in the dead __comp slot and a null distance pointer;
// the else arm copy-constructs __val into __unguarded_linear_insert's argument area.
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
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x00CB62F0

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CB6420

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first,
                                _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template void __fastcall __linear_insert<SScriptInfoPair*, SScriptInfoPair, SScriptInfoLess>(
    SScriptInfoPair* __first, SScriptInfoPair* __last, SScriptInfoPair __val, SScriptInfoLess __comp);
}