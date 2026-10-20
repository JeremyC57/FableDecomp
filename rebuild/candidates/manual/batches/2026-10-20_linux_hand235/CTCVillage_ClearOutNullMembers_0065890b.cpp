// CTCVillage::ClearOutNullMembers() -- removes the null entries from the village's building, people, patrol-node, torch, horn, gate, crop and marker lists
// Leaf: ClearOutNullMembers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065890B, 247 bytes (plain `ret`, `leave`).
// Callees: the CIntelligentPointer<T>(T*) converting ctors, out of line in this size TU (manifest "CIntelligentPointer<CThing
//          const>" is propagated across the byte twins): <CThingBuilding> 0x0050F380, <CThingAICreature> 0x0050F3F0,
//          <CThingTrackNode> 0x0065E15D, <CThing> 0x004A96F0, <CThingObject> 0x0065E133, <CThingMarker> 0x0065E21E;
//          the list<CIntelligentPointer<T> >::remove(const T&) instantiations (manifest "SetScriptBrushThingsToMove" is
//          propagated): 0x0065DDDB, 0x0065DE48, 0x0065DF94, 0x0065DE8A (x3, CThing), 0x0065DF0F, 0x0065E09E;
//          ~CBaseIntelligentPointer 0x00A01C10.
// Layout: engine/CTCVillage.h carries Ego_r.pdb offsets (12-byte lists); in retail every list is one sentinel pointer
//          (as the landed CTCVillage rows model it), so a local model: VillageBuildings +0x28, VillagePeople +0x2c,
//          VillageTorches +0x30, VillageHorns +0x34, VillageGates +0x38, VillageCrops +0x3c, VillagePatrols +0x48,
//          VillageMarkers +0x70.
// Shape: frame-pointer size TU (`optimize("y", off)` + `("s", on)`); each statement is `list.remove(0)`
//        with the null pointer converted implicitly (the temporary re-addressed with `lea`; an explicit temporary pushes
//        the ctor's eax), the temporary in one shared frame slot and the null argument from the edi zero.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CThingBuilding;
class CThingAICreature;
class CThingTrackNode;
class CThingObject;
class CThingMarker;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item);               // out of line: 0x0050F380 / 0x0050F3F0 / 0x0065E15D / 0x004A96F0 / 0x0065E133 / 0x0065E21E
};
template<class T> struct CRetailList {          // retail std::list: one sentinel pointer
    void* m_head;
    void remove(const T& value);                // 0x0065DDDB / 0x0065DE48 / 0x0065DF94 / 0x0065DE8A / 0x0065DF0F / 0x0065E09E
};

struct CTCVillage {
    virtual void v00();
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x20];
    CRetailList< CIntelligentPointer<CThingBuilding> >   VillageBuildings;   // +0x28
    CRetailList< CIntelligentPointer<CThingAICreature> > VillagePeople;      // +0x2c
    CRetailList< CIntelligentPointer<CThing> >           VillageTorches;     // +0x30
    CRetailList< CIntelligentPointer<CThing> >           VillageHorns;       // +0x34
    CRetailList< CIntelligentPointer<CThing> >           VillageGates;       // +0x38
    CRetailList< CIntelligentPointer<CThingObject> >     VillageCrops;       // +0x3c
    CRetailList< CIntelligentPointer<CThing> >           VillageCages;       // +0x40
    CRetailList< CIntelligentPointer<CThing> >           VillageCampfires;   // +0x44
    CRetailList< CIntelligentPointer<CThingTrackNode> >  VillagePatrols;     // +0x48
    unsigned char _pad_0x4c[0x24];
    CRetailList< CIntelligentPointer<CThingMarker> >     VillageMarkers;     // +0x70
    void ClearOutNullMembers();
};

void CTCVillage::ClearOutNullMembers()
{
    VillageBuildings.remove(0);
    VillagePeople.remove(0);
    VillagePatrols.remove(0);
    VillageTorches.remove(0);
    VillageHorns.remove(0);
    VillageGates.remove(0);
    VillageCrops.remove(0);
    VillageMarkers.remove(0);
}
