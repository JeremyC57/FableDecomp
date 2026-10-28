// _global::__push_heap(pair<CCharString,CTCBase::CRegisteredTCInfo>*, long, long, pair<CCharString,CTCBase::CRegisteredTCInfo>, CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>) -- SGI heap sift-up of the registered-TC table entries by name
// Leaf: __push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DD875, 138 bytes, `ret 0x18` (a /Os module: unaligned
// entry, ebp frame with `first` spilled to [ebp-4], `leave`; the SGI algorithm TUs are compiled /Gr,
// so first/hole ride in ecx/edx).
// Called by the landed __adjust_heap @ 0x004DFDBB (same instantiation; its model is reused here).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x004DA56A pair::operator= (out of line in /Os;
// manifest labels it pair<CCharString,CCharString>), 0x0099EAE0 ~CCharString.
// Element (16 bytes): CCharString key +0, CTCBase::CRegisteredTCInfo +4 (PDB: AllocFunc +0,
// DriverType +4, Name +8 -> element +0xc).  The by-value element is destroyed at the end by the
// implicit pair destructor (Name at value+0xc, then the key).  `ret 0x18` = top, the 16-byte
// element and the comparator.  The key compare is CCharString::operator< inline (same payload ->
// false, null this -> true, null other -> false, else the payload compare); under /Os the value's
// key word is compared in memory (`cmp [ebp+0Ch],ecx`) rather than loaded.
// Shape: SGI `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(*(first + parent), value))`).
#pragma optimize("s", on)

class CThing;
class CTCBase;

struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);
    __forceinline bool operator<(const CCharString& other) const
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

struct CRegisteredTCInfo {
    CTCBase* (__cdecl* AllocFunc)(CThing*);                // +0x0
    unsigned long DriverType;                              // +0x4
    CCharString Name;                                      // +0x8
};

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other);                               // 0x004D788D
    pair& operator=(const pair& other);                    // 0x004DA56A
};
typedef pair<CCharString, CRegisteredTCInfo> TEntry;

template<class K, class V> class CKeyPairCompareLess { public:
    char m_tag;
    __forceinline bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, CRegisteredTCInfo> TCompare;

void __fastcall __push_heap(TEntry* first, long holeIndex, long topIndex, TEntry value, TCompare comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}