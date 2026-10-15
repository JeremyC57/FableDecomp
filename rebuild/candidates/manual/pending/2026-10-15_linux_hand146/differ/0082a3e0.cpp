// CMarkerInfo::IsReversedOnMap() -- whether the marked thing's map icon is drawn mirrored: false for a missing or dead thing; a thing with the cached driver component (presence bit 0x64) answers from it (+0x28), otherwise the thing's map-marker component (kind 0xC2, looked up in the component map) answers from +0x4c; false when it has neither
// Grade: DIFFER(118v118) -- scheduling only: before the const LowerBound call retail emits `lea esi,[eax+44h] ; lea eax,[esp+4] ; push eax`, ours `lea ecx,[esp+4] ; lea esi,[eax+44h] ; push ecx` (the key temporary's address in ecx instead of eax)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A3E0, length 118 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (four times, PThing at +0), the const component-map LowerBound @ 0x004365B0.
// Layout (PDB CMarkerInfo, 0x10): PThing (CIntelligentPointer<CThing const>) +0, GraphicIndex +8, AutoMarker +0xc. The thing: component
//   presence bitset +0x20 (kind 0x64 -> byte +0x2c bit 0x10, kind 0xC2 -> byte +0x38 bit 4), component map +0x44, cached driver
//   component +0x5c (as in the landed 0x00750DD0), dead bit +0x91.
// Tried: the driver read through a fresh GetPItem (retail reuses the third call's result: a named local), the marker read through the
//   pointer-returning Find (`mov edx,[eax+4]`), the out-parameter GetTCOut (fixes the read, leaves the lea order), a `const long&` key (worse).
struct CTCBase_Retail;
struct STCEntry_Retail {
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
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
    CTCMap_Retail  m_tcs;
    unsigned char  _pad50[0xc];
    CTCDriverCached* m_pDriver;
    unsigned char  _pad60[0x31];
    unsigned char  m_dead : 1;
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
template <class T> __forceinline bool GetTCOut(const CThing* thing, long kind, const T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (const T*)thing->m_tcs.Find(kind);
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
