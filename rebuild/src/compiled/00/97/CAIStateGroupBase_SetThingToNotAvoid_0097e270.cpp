// CAIStateGroupBase::SetThingToNotAvoid(const CThing* thing) -- hand the thing to the creature's navigation component (interface 7) as the thing its route need not avoid
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097E270, 92 bytes (ret 4).
// Name: Ego_r.pdb ?SetThingToNotAvoid@CAIStateGroupBase@@IAEXPBVCThing@@@Z (the PDB order GetPBestTargetToFaceForCreature,
//   SetThingToNotAvoid, SetProximityForDirectApproach, SetPushableByHero matches retail 0x0097E140/E270/E2D0/E330); the
//   catalog label "AssignCreatureToTask_Helper" is propagated.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCCreatureNavigation::SetThingToIgnore @
//   0x006CBE30 (PDB name SetPThingToNotAvoid; the landed tree spells it SetThingToIgnore, kept for one symbol per address).
// Layout: engine/CAIStateGroupBase.h PBrain +0x04; CAIBrain creature +0x18; creature interface bitset +0x20, interface map
//   +0x44 (local models: the header struct cannot carry the member declaration).
// Shape: the UNCHECKED out-parameter GetInterface (GOTCHAS chunk 129): the result is ignored, so the call is duplicated into
//   both arms and the absent arm reads the never-written out pointer (VC homes it on the argument slot); the presence test
//   on type 7 is the masked byte test `test cl,cl ; jns`.
// Leaf: ?SetThingToNotAvoid@CAIStateGroupBase@@
class CTCBase;
class CThing;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CTCCreatureNavigation { public:                  // interface 7
    void SetThingToIgnore(const CThing* thing); // 0x006CBE30
};
class CThingCreatureBase { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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
class CAIBrain { public:
    void*               __vftable;              // +0x00
    unsigned char       _pad04[0x14];           // +0x04..+0x18
    CThingCreatureBase* Creature;               // +0x18
};
class CAIStateGroupBase {
public:
    void*     __vftable;                        // +0x00
    CAIBrain* PBrain;                           // +0x04
protected:
    void SetThingToNotAvoid(const CThing* thing);
};

void CAIStateGroupBase::SetThingToNotAvoid(const CThing* thing)
{
    CTCCreatureNavigation* navigation;
    PBrain->Creature->GetInterface(&navigation, 7);
    navigation->SetThingToIgnore(thing);
}