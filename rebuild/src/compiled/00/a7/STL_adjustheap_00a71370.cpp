// std::__adjust_heap(pair<CCharString,C3DMeshVolume>* first, int holeIndex, int len, pair<CCharString,C3DMeshVolume> val, CKeyPairCompareLess<CCharString,C3DMeshVolume> comp) -- STLport heap sift-down of the (name, named convex mesh volume) table by name, then __push_heap val back up
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A71370, 377 bytes (+ int3 padding), `ret 18h` (__fastcall: first in ecx,
//   holeIndex in edx; len, the 16-byte value and the empty comparator on the stack: 4 + 0x10 + 4 = 0x18).
// Element (16 bytes, `shl eax,4`): CCharString key +0, C3DMeshVolume +4 (Ego_r.pdb, 12 bytes: CCharString Name +0,
//   CCountedPointer<CContainmentVolume> ConvexVolume +4 = {Data, Info}).
// Callees: CBasicString<char>::operator< @ 0x00429950 (payload compare of the inline CCharString::operator<: same
//   payload -> false, null this -> true, null other -> false), CCharString::operator= @ 0x0099EFB0, operator delete
//   @ 0x00BFE9BC (the inline CCountedPointer release), C3DMeshVolume::operator= @ 0x00A32960 (the second, tail
//   element move: VC7.1 inlines the implicit operator= in the loop and calls its COMDAT copy at the tail),
//   CCharString(const CCharString&) @ 0x0099EC30 x2 (memberwise pair copy built in __push_heap's argument slot),
//   __push_heap @ 0x00A708E0 (same instantiation), ~CCharString @ 0x0099EAE0 x2 (val: ConvexVolume released and
//   nulled first, then Name, then the key).
// Shape: STLport 4.6 `__adjust_heap`; first and topIndex homed in the `sub esp,8` slots.
extern void __cdecl operator delete(void* block);                                    // 0x00BFE9BC

struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

class CCharString {
public:
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

#include "engine/CCountedPointer.h"


class CContainmentVolume;
class C3DMeshVolume {                                   // Ego_r.pdb, 12 bytes
public:
    CCharString Name;                                   // +0x0
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x4
};

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
typedef std::pair<CCharString, C3DMeshVolume> CMeshVolumePair;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00A708E0

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

template void __fastcall __adjust_heap(CMeshVolumePair*, int, int, CMeshVolumePair, CKeyPairCompareLess<CCharString, C3DMeshVolume>);
}