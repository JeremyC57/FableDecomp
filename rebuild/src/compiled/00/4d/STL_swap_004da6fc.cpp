// _STL::swap(pair<CCharString,CTCBase::CRegisteredTCInfo>& a, pair<CCharString,CTCBase::CRegisteredTCInfo>& b) -- exchange two thing-component registry entries (key string + alloc function, driver type, name) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA6FC, 60 bytes (a /Os module: unaligned entry, ebp frame, `leave`), /Gr
//   `__fastcall` (a in ecx -> edi, b in edx -> esi), plain `ret`.
// Identity: the 16-byte element's copy constructor 0x004D788D copies a CCharString and then a 12-byte member through
//   0x004D6C66, its assignment 0x004DA56A assigns the string and then the member through 0x004D78C5, and the temporary
//   dies at +0xc and +0 (both ~CCharString): _STL::pair<CCharString, CTCBase::CRegisteredTCInfo> (Ego_r.pdb
//   CRegisteredTCInfo, 12 bytes: AllocFunc +0, DriverType +4, CCharString Name +8) -- the thing-component registry the
//   catalog's "swap<CTCBase::CRegisteredTCInfo>" names. Callers: the out-of-line iter_swap 0x004DBEDA (a bare `jmp`) and
//   the partition step 0x004DFD37.
// Callees: pair copy ctor 0x004D788D, pair::operator= 0x004DA56A (both out of line in this /Os module), ~CCharString
//   0x0099EAE0 (the temporary's Name, then its key).
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` in a size TU: the pair's copy constructor and
//   assignment stay calls, its destructor inlines to the two string destructors.
#pragma optimize("s", on)
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CThing;

class CTCBase {
public:
    class CRegisteredTCInfo {                           // Ego_r.pdb, 12 bytes
    public:
        CTCBase*      (__cdecl* AllocFunc)(CThing*);    // +0x00
        unsigned long DriverType;                       // +0x04
        CCharString   Name;                             // +0x08
    };
};

namespace _STL {
template <class _T1, class _T2>
struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

typedef pair<CCharString, CTCBase::CRegisteredTCInfo> TRegisteredTCPair;
template void __fastcall swap<TRegisteredTCPair>(TRegisteredTCPair&, TRegisteredTCPair&);
}