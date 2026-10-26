// CTCActionUseAcceptBoast::vfunc_28(CThingCreatureBase*) -- run the base slot, then tell the parent thing's boasting-position component (interface 0x71) that the creature accepted
// Leaf: vfunc_28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EBBC0, length 96 bytes. Slot 28 of the
// CTCActionUseAcceptBoast vtable 0x0123F674 (rtti-vtables.tsv).
// Callees: CTCActionUseBase's slot 28 @ 0x007EAB90 (an empty `ret 4`, called non-virtually; manifest "$E2"),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0x71
// = bit 17 of the bitset dword at +0x2c), CTCBoastingPosition::OnBoastAccepted(CThingCreatureBase*) @
// 0x00782390.
//
// Shape: the creature argument is held in edi across the base call; then the inlined GetInterface
// template (landed 0x00949320) with an UNINITIALISED component local -- the key temporary and the
// never-written local both live in the argument's own (dead) stack slot (`lea eax,[esp+0Ch]` / the miss
// path's `mov eax,[esp+0Ch]`), so there is no frame of its own.
// Header note: engine/CTCBoastingPosition.h holds the layout only; the callee is modelled as
// CTCBoastingPosition_Calls.
class CTCBase;
class CThingCreatureBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
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

struct CTCBoastingPosition_Calls {                    // interface type 0x71 component
    void OnBoastAccepted(CThingCreatureBase* creature);   // 0x00782390
};

class CTCActionUseBase { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    void vfunc_28(CThingCreatureBase* creature);  // 0x007EAB90 (empty)
};

struct CTCActionUseAcceptBoast : CTCActionUseBase {
    void vfunc_28(CThingCreatureBase* creature);
};

void CTCActionUseAcceptBoast::vfunc_28(CThingCreatureBase* creature)
{
    CTCActionUseBase::vfunc_28(creature);
    CTCBoastingPosition_Calls* position;
    ParentThing->GetInterface(&position, 0x71);
    position->OnBoastAccepted(creature);
}