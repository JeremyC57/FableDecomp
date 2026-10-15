// CTCTavernBar::FreeBarPoint(long) -- hand a bar point index back to the parent thing's interface-0x2E component
// Leaf: FreeBarPoint
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BA4F0, length 88 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined interface lookup,
// type 0x2E = bit 14 of the bitset dword at +0x24, tested with `mov ecx,[eax+24h] ; test ch,40h`), then
// the component's 0x007FEF40 (it appends the long to the vector at its +0x0c: `if (last != end) *last++ =
// v; else insert` -- the manifest's "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" label is
// propagated).
// Identity: the manifest label "CTCActionUseAcceptBoast::OnActionUse" is propagated over the byte-identical
// GetInterface shape. This copy sits directly before CTCTavernBar::OnInitialActivate (0x007BA550); it is
// the interface-0xB9 component's method the landed CAIStateGroup_GoToPub::FreeBarPoint (0x00949320) calls
// with its bar-point index (also CTCTavernBar::HasEmptyJugSlot 0x007BA8C0 and two
// CAIStateGroup_TavernKeeper::FreeBarPoint copies); byte-identical to CTCTavernTable::FreePoint 0x007B9090.
//
// Shape: the component pointer is an UNINITIALISED local filled by the inlined GetInterface (the landed
// 0x00949320 template): hit path -> the lookup result in eax; miss path -> retail reads the never-written
// local, which VC7.1 homed in the argument's own stack slot (`mov eax,[esp+8]` beside the argument load).
// The frame is one `push ecx` (the key temporary). No engine header models the component table, so the
// thing is local.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCFreePointList {                       // interface type 0x2E component
    void AddFreePoint(long index);              // 0x007FEF40
};

struct CTCTavernBar {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    void FreeBarPoint(long index);
};

void CTCTavernBar::FreeBarPoint(long index)
{
    CTCFreePointList* points;
    ParentThing->GetInterface(&points, 0x2E);
    points->AddFreePoint(index);
}
