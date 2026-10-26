// CAIStateGroupBase::SetProximityForDirectApproach(float proximity) -- set the creature navigation component's (interface 7) direct-approach proximity
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097E2D0, 82 bytes (ret 4).
// Name: Ego_r.pdb ?SetProximityForDirectApproach@CAIStateGroupBase@@IAEXM@Z (PDB order GetPBestTargetToFaceForCreature,
//   SetThingToNotAvoid, SetProximityForDirectApproach, SetPushableByHero = retail 0x0097E140/E270/E2D0/E330); the catalog
//   label "SetTaskProperty_AtOffset" is propagated.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020; CTCCreatureNavigation::SetProximityForDirectApproach
//   (PDB QAEXM@Z) is inline: one float store to +0x58.
// Layout: engine/CAIStateGroupBase.h PBrain +0x04; CAIBrain creature +0x18; creature interface bitset +0x20, interface map
//   +0x44; navigation proximity +0x58 (local models: the header struct cannot carry the member declaration).
// Shape: the UNCHECKED out-parameter GetInterface (sibling 0x0097E270): the inline setter is duplicated into both arms; the
//   hit arm copies the float as a dword, the absent arm reads the never-written out pointer from the argument slot and
//   copies through the FPU.
// Leaf: ?SetProximityForDirectApproach@CAIStateGroupBase@@
class CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CTCCreatureNavigation { public:                  // interface 7
    unsigned char _pad0[0x58];
    float ProximityForDirectApproach;           // +0x58
    void SetProximityForDirectApproach(float proximity) { ProximityForDirectApproach = proximity; }
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
    void SetProximityForDirectApproach(float proximity);
};

void CAIStateGroupBase::SetProximityForDirectApproach(float proximity)
{
    CTCCreatureNavigation* navigation;
    PBrain->Creature->GetInterface(&navigation, 7);
    navigation->SetProximityForDirectApproach(proximity);
}