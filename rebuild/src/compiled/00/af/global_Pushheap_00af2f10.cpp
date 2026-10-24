// _global::_Push_heap(pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*, long, long, pair<CCharString,NParticleEngine::CRegisteredPSCInfo>, CKeyPairCompareLess<CCharString,NParticleEngine::CRegisteredPSCInfo>) -- heap sift-up of the particle-system component registry
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2F10, 193 bytes, `ret 0x18`.
// Callees: 0x00429950 CBasicString<char>::operator<, 0x0099EFB0 CCharString::operator= (x2 per
// move), 0x0099EAE0 ~CCharString (x2 at the end).  Link-time __fastcall (/Gr algorithm TU): first in
// ecx, hole in edx, then top, the 16-byte pair by value and the empty comparator (4 + 0x10 + 4 = 0x18).
// NParticleEngine::CRegisteredPSCInfo (PDB, 12 bytes): AllocFunc +0, EPSCType ComponentType +4,
// CCharString GUIName +8; the implicit pair assignment is the key call, two dwords and the GUIName
// call.  TU neighbour of the landed CVectorMap<..CRegisteredPSCInfo..>::LowerBound @ 0x00AF3000.
// CCharString::operator< (inline, the landed 0x009B5C70 / 0x00CB68C0 shape): same payload -> false,
// null this -> true, null other -> false, else CBasicString<char>::operator< @ 0x00429950 on the payloads.
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
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
};

class CPSCBase;
enum EPSCType { PSC_TYPE_NONE };

namespace NParticleEngine {
struct CRegisteredPSCInfo {
    CPSCBase* (__cdecl* AllocFunc)();                      // +0x00
    EPSCType ComponentType;                                // +0x04
    CCharString GUIName;                                   // +0x08
};
}

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<CCharString, NParticleEngine::CRegisteredPSCInfo> TEntry;

template<class K, class V> struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, NParticleEngine::CRegisteredPSCInfo> TCompare;

void __fastcall _Push_heap(TEntry* first, long hole, long top, TEntry value, TCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}