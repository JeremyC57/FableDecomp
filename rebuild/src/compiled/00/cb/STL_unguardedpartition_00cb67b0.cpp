// std::__unguarded_partition(pair<CCharString,CScriptInfo>* __first, pair<CCharString,CScriptInfo>* __last, pair<CCharString,CScriptInfo> __pivot, CKeyPairCompareLess __comp) -- STLport quicksort partition step around a by-value pivot; returns the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB67B0, 119 bytes. __fastcall: __first ecx, __last edx, the 0x18-byte
// pivot and the comparator word on the stack (`ret 1Ch`), the split iterator in eax. The manifest label
// "MusicEntry_Partition" is propagated.
// Callees: CBasicString<char>::operator< 0x00429950, swap 0x00CB6230 (through the inline STLport iter_swap; `__fastcall` *i1/*i2; manifest
// "swap<CMusicManager::CRegisteredMusicEntry>" is propagated), ~CCharString 0x0099EAE0.
// Shape: the STLport body verbatim; the pivot key's handle is re-read each outer pass (`mov esi,[esp+10h]`), the
// `--__last` before and inside the second scan share one loop head. The handle stays in esi only when the
// string compare's body is in the TU (an extern compare reloads it after every call: 128v119).
// Model: STLport 4.6 algorithms (namespace std, the /Gr TU makes the templates __fastcall) over the script registry's
// 0x18-byte std::pair<CCharString, CScriptInfo> (CScriptInfoManager::Scripts, a CVectorMap sorted with
// CKeyPairCompareLess). Ego_r.pdb CScriptInfo (0x14): AllocFunc +0, MasterScriptDataAlloc +4, CCharString Name +8,
// long ID +0xc, bool MasterScript +0x10 (implicit copy and assignment) -- the model of the landed __insertion_sort
// 0x00CB6EA0, __pop_heap 0x00CB6B00 and __copy_backward 0x00CB62F0. STLport's pair has a user copy constructor;
// CCharString::operator< is the inline handle compare in front of CBasicString<char>::operator< 0x00429950.
// Same-TU stand-in of CBasicString<char>::operator< 0x00429950 (the landed body, kept out of line): with it VC7.1
// knows the compare writes no memory and keeps the pivot key's handle in esi across the scans.
#pragma auto_inline(off)
template <class T> class CBasicString {
public:
    T* m_pData;
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }   // 0x00429950
};
#pragma auto_inline(on)

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
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b);                                   // 0x00CB6230 (out of line)

// STLport 4.6 iter_swap, inline: the partition calls swap(*__i1, *__i2) directly; the out-of-line iter_swap
// copy 0x00CB63F0 is a bare `jmp` to swap.
template <class _ForwardIter1, class _ForwardIter2>
inline void __fastcall iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2) { swap(*__i1, *__i2); }

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first,
                                                   _RandomAccessIter __last,
                                                   _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template SScriptInfoPair* __fastcall __unguarded_partition<SScriptInfoPair*, SScriptInfoPair, SScriptInfoLess>(
    SScriptInfoPair* __first, SScriptInfoPair* __last, SScriptInfoPair __pivot, SScriptInfoLess __comp);
}