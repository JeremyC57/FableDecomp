// CMarkerInfo::IsReversedOnMap() -- whether the marked thing's map icon is drawn mirrored: false for a missing or dead thing; a thing with the cached driver component (presence bit 0x64) answers from it (+0x28), otherwise the thing's map-marker component (kind 0xC2, looked up in the component map) answers from +0x4c; false when it has neither
// Grade: DIFFER(118v118) -- wave 2 sec3: the real const CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0 body now in the TU (same-TU noinline mod-ref) and leaf set; residue unchanged: key address in ecx computed before lea esi (retail lea esi first, then eax). Tried: same-TU LowerBound body, member GetTC const reference (worse), key copy before/after the map pointer
// Leaf: ?IsReversedOnMap@CMarkerInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A3E0, length 118 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four times, PThing at +0), the const component-map LowerBound @ 0x004365B0.
// Layout (PDB CMarkerInfo, 0x10): PThing (CIntelligentPointer<CThing const>) +0, GraphicIndex +8, AutoMarker +0xc. The thing: component
//   presence bitset +0x20 (kind 0x64 -> byte +0x2c bit 0x10, kind 0xC2 -> byte +0x38 bit 4), component map +0x44, cached driver
//   component +0x5c (as in the landed 0x00750DD0), dead bit +0x91.
// Tried: the driver read through a fresh GetPItem (retail reuses the third call's result: a named local), the marker read through the
//   pointer-returning Find (`mov edx,[eax+4]`), the out-parameter GetTCOut (fixes the read, leaves the lea order), a `const long&` key (worse).
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp* end() const { return _M_finish; }
};
}
enum ETCInterfaceType { };
class CTCBase;
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    const value_type* LowerBound(const K& key) const;       // 0x004365B0
    std::vector<value_type> Data;
    Cmp  Compare;
    bool Dirty;
};
template <class K, class V, class Cmp>
__declspec(noinline) const std::pair<K, V>* CVectorMap<K, V, Cmp>::LowerBound(const K& key) const   // 0x004365B0: the retail body, out of line (same-TU mod-ref)
{
    long count = Data._M_finish - Data._M_start;
    const value_type* first = Data._M_start;
    while (count > 0) {
        long half = count / 2;
        const value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count = count - half - 1;
        } else
            count = half;
    }
    return first;
}
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CTCDriverCached {
    unsigned char _pad[0x28];
    bool m_reversedOnMap;
};
struct CTCMapMarker {
    unsigned char _pad[0x4c];
    bool m_reversedOnMap;
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];
    unsigned char  _pad40[4];
    CInterfaceMap  m_tcs;
    unsigned char  _pad54[0x8];
    CTCDriverCached* m_pDriver;
    unsigned char  _pad60[0x31];
    unsigned char  m_dead : 1;
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline const CTCBase& GetTC(ETCInterfaceType type) const   // Ego_r ?GetTC@CThing@@QBEABVCTCBase@@W4ETCInterfaceType@@@Z (inline)
    {
        const CInterfaceMap* map = &m_tcs;
        const ETCInterfaceType key = type;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        const CInterfaceMap::value_type* end = map->Data.end();
        return *(it == end || type < it->first ? end : it)->second;
    }
};
template <class T> __forceinline bool GetTCOut(const CThing* thing, long kind, const T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        const ETCInterfaceType key = (ETCInterfaceType)kind;
        const CInterfaceMap* map = &thing->m_tcs;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        const CInterfaceMap::value_type* end = map->Data.end();
        *out = (const T*)(it == end || kind < it->first ? end : it)->second;
        return true;
    }
    return false;
}
struct CBaseIntelligentPointer {
    void*   _vptr;
    CThing* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
struct CMarkerInfo {
    CBaseIntelligentPointer PThing;
    long GraphicIndex;
    bool AutoMarker;
    bool IsReversedOnMap();
};
bool CMarkerInfo::IsReversedOnMap()
{
    if (PThing.GetPItem() && !PThing.GetPItem()->m_dead) {
        CThing* driven = PThing.GetPItem();
        if (driven->HasTC(0x64))
            return driven->m_pDriver->m_reversedOnMap;
        const CThing* thing = PThing.GetPItem();
        const CTCMapMarker* marker;
        if (GetTCOut(thing, 0xc2, &marker))
            return marker->m_reversedOnMap;
    }
    return false;
}
