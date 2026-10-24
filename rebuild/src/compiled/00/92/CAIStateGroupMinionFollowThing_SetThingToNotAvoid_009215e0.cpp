// CAIStateGroup_MinionFollowThing::SetThingToNotAvoid(const CThing* thing) -- tell the minion's navigation component (interface 7) to ignore the given thing when avoiding obstacles
// Leaf: ?SetThingToNotAvoid@CAIStateGroup_MinionFollowThing@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009215E0, 72 bytes (80 with padding), __fastcall `ret 4`.
// Identity: Ego_r.pdb ?SetThingToNotAvoid@CAIStateGroup_MinionFollowThing@@IAEXPBVCThing@@@Z (protected); the only caller
//   is this class's Update (0x009219C9). The catalog label "CAIStateGroup_MinionFollowThing_AssignTask" is an autoname.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (non-const copy),
//   CTCCreatureNavigation::SetThingToIgnore(const CThing*) 0x006CBE30 (manifest "CVillageTaskCarryCrate::AssignCreatureToTask"
//   is propagated; the landed Cleanup 0x00921870 names it).
// Layout: PBrain +0x04 -> PCreature +0x18; the thing's interface bitset at +0x20 (interface 7 = the sign bit of byte +0x20,
//   `test cl,cl ; jns`), the sorted {type, component} vector map at +0x44.
// Shape: the bool out-parameter GetInterface inside the `if` -- the component is stored to the local (`mov eax,[eax+4]`)
//   before the argument push and moved to ecx at the call; the entry-returning FindEntry form folds it straight into ecx
//   (`mov ecx,[eax+4]`). Model shared with the landed CAIStateGroup_MinionFollowThing::Cleanup 0x00921870 (the engine
//   header flattens the base, so the class is modelled locally).
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CTCCreatureNavigation {                  // interface 7
    void SetThingToIgnore(const CThing* thing); // 0x006CBE30
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
};

struct CAIStateGroup_MinionFollowThing : CAIStateGroupBase {
    void SetThingToNotAvoid(const CThing* thing);
};

void CAIStateGroup_MinionFollowThing::SetThingToNotAvoid(const CThing* thing)
{
    CTCCreatureNavigation* navigation;
    if (PBrain->PCreature->GetInterface(&navigation, 7))
        navigation->SetThingToIgnore(thing);
}