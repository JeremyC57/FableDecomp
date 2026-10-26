// _global::__adjust_heap(pair<CCharString,CTCBase::CRegisteredTCInfo>*, long, long, pair<CCharString,CTCBase::CRegisteredTCInfo>, CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>) -- SGI heap sift-down of the registered-TC table entries by name, then __push_heap @ 0x004DD875
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DFDBB, 167 bytes (a /Os module: unaligned entry,
// ebp frame, loop test at the bottom; the SGI algorithm TUs are compiled /Gr, so first/hole ride in
// ecx/edx).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x004DA56A pair::operator= and 0x004D788D pair
// copy ctor (out of line in /Os; manifest labels them pair<CCharString,CCharString>), 0x0099EAE0
// ~CCharString, 0x004DD875 __push_heap (same instantiation).
// Element (16 bytes): CCharString key +0, CTCBase::CRegisteredTCInfo +4 (PDB: AllocFunc +0,
// DriverType +4, Name +8 -> element +0xc).  The by-value element is copy-built in the __push_heap
// slot (`sub esp,0x10 ; mov ecx,esp ; push &value`) and destroyed here by the implicit pair
// destructor (Name at value+0xc, then the key).  `ret 0x18` = bottom, the element, the comparator.
// Shape: SGI `__adjust_heap` (`2 * (secondChild + 1)`); the key compare is CCharString::operator<
// inline (same payload -> false, null this -> true, null other -> false, else payload compare).
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
    CCharString& operator=(const CCharString& other);
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

void __fastcall __push_heap(TEntry* first, long holeIndex, long topIndex, TEntry value, TCompare comp);   // 0x004DD875

void __fastcall __adjust_heap(TEntry* first, long holeIndex, long len, TEntry value, TCompare comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value, comp);
}