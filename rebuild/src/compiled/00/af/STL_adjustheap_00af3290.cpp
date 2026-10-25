// std::__adjust_heap(pair<CCharString,NParticleEngine::CRegisteredPSCInfo>* first, int holeIndex, int len, pair<CCharString,NParticleEngine::CRegisteredPSCInfo> val, CKeyPairCompareLess comp) -- STLport heap sift-down of the registered particle-component table by name, then __push_heap val back up
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@UCRegisteredPSCInfo@NParticleEngine@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF3290, 280 bytes (+ int3 padding), `ret 18h` (__fastcall: first in ecx,
//   holeIndex in edx; len, the 16-byte value and the empty comparator on the stack: 4 + 0x10 + 4 = 0x18).
// Element (16 bytes, `shl eax,4`): CCharString key +0, NParticleEngine::CRegisteredPSCInfo +4 (PDB: AllocFunc +0,
//   EPSCType ComponentType +4, CCharString GUIName +8 -> element +0xC).
// Callees: CBasicString<char>::operator< @ 0x00429950 (the payload compare of the inline CCharString::operator<:
//   same payload -> false, null this -> true, null other -> false), CCharString::operator= @ 0x0099EFB0 (the
//   implicit element assignment: key, two words, GUIName), CCharString(const CCharString&) @ 0x0099EC30 (the
//   memberwise pair copy built in __push_heap's argument slot), __push_heap @ 0x00AF2F10 (same instantiation),
//   ~CCharString @ 0x0099EAE0 (val: GUIName, then the key).
// Callers: __pop_heap 0x00AF3430 and the make_heap / sort family of the same table.
// Shape: STLport 4.6 `__adjust_heap`; topIndex homed in the `push ecx` slot; `comp(first[second], first[second-1])`.
class CPSCBase;

struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return *PStringData < *other.PStringData;
    }
private:
    CBasicStringChar* PStringData;
};

enum EPSCType { PSC_TYPE_NONE = 0 };

namespace NParticleEngine {
struct CRegisteredPSCInfo {
    CPSCBase* (__cdecl* AllocFunc)();           // +0x0
    EPSCType ComponentType;                     // +0x4
    CCharString GUIName;                        // +0x8
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {     // empty (Ego_r.pdb: 1 byte)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> CPSCPair;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00AF2F10

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap(CPSCPair*, int, int, CPSCPair,
                                       CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo>);
}