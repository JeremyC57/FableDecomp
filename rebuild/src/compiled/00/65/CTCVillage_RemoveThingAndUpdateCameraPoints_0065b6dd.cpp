// CTCVillage::RemoveThingAndUpdateCameraPoints(const CThing& thing) -- drop the thing from every village task, then erase it from the first village list that holds it (people, torches, buildings, crops, patrols, markers)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065B6DD, length 375 bytes (`leave`, `ret 4`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed CTCVillage siblings.
// Callees: CTCVillage::RemoveThingFromAllTasks(CThing*) 0x00659EA9 (landed); the STLport std::find instantiations
//   0x0065F4E2 / 0x0065F50A / 0x0065F532 / 0x0065F55A / 0x0065F582 / 0x0065F5AA (`__fastcall`: hidden result in ecx, the
//   value's address in edx, both iterators by value on the stack; landed as "CVector range constructors" -- the same
//   bytes forward to __find with the input-iterator tag); list<>::erase(iterator) 0x0065DE1D / 0x004B7CA0 (the shared
//   list<CIntelligentPointer<CThing> > copy) / 0x0065DDB0 / 0x0065DEE4 / 0x0065DF69 / 0x0065E073 (manifest
//   "DoublyLinkedList_RemoveNode" / "ListNode_RemoveWithCallback" are descriptive; the returned iterator is discarded).
// Layout: retail CTCVillage lists are bare STLport head-node pointers (4 bytes; landed ~CTCVillage 0x0065B598):
//   VillageBuildings +0x28, VillagePeople +0x2c, VillageTorches +0x30, VillageCrops +0x3c, VillagePatrols +0x48,
//   VillageMarkers +0x70 (PDB order with 4-byte lists and the 12-byte PlayersCrimes set at +0x64).
// Identity: the PDB names no RemoveThingAndUpdateCameraPoints; the matching Ego_r.pdb member is
//   `?RemoveVillageMember@CTCVillage@@QAEXABVCThing@@@Z` (const CThing&, as RemoveThingFromAllTasks
//   `?RemoveThingFromAllTasks@CTCVillage@@AAEXABVCThing@@@Z`), so the find key is the rvalue `&thing`, a temporary homed
//   in the dead argument slot [ebp+8] and re-stored before each find.
// Shape: one scoped find/compare/erase per list in the order people, torches, buildings, crops, patrols, markers; each
//   hit erases and returns. The same-TU noinline find body (STLport find instantiations follow this function in retail)
//   lets VC7.1 sink the key store to just before each call, as retail.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CThingAICreature;
class CThingBuilding;
class CThingObject;
class CThingTrackNode;
class CThingMarker;

template <class T> class CIntelligentPointer { public:
    void* _vfptr;
    T*    m_pItem;
};

template <class T> struct SListNode {           // STLport _List_node
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};

template <class T> struct SListIterator {       // STLport _List_iterator (user copy constructor: hidden-pointer return)
    SListNode<T>* _M_node;
    SListIterator(SListNode<T>* node) : _M_node(node) {}
    SListIterator(const SListIterator& other) : _M_node(other._M_node) {}
    bool operator!=(const SListIterator& other) const { return _M_node != other._M_node; }
};

template <class T> struct SList {               // STLport list<T>: bare head-node pointer
    SListNode<T>* _M_node;
    SListIterator<T> begin() { return SListIterator<T>(_M_node->_M_next); }
    SListIterator<T> end() { return SListIterator<T>(_M_node); }
    SListIterator<T> erase(SListIterator<T> position);
};

const CThing* __fastcall ListItem_s8(const void* pointer);   // stand-in: CIntelligentPointer<>::GetPItem on the node's value
#pragma auto_inline(off)
template <class It, class T> __declspec(noinline) It __fastcall find(It first, It last, const T& value)
{
    while (first != last && ListItem_s8(&first._M_node->_M_data) != value)
        first._M_node = first._M_node->_M_next;
    return first;
}
#pragma auto_inline(on)

class CTCVillage { public:
    void*                                 __vftable;          // +0x00
    unsigned char                         _pad_0x04[0x24];
    SList<CIntelligentPointer<CThingBuilding> >   VillageBuildings;   // +0x28
    SList<CIntelligentPointer<CThingAICreature> > VillagePeople;      // +0x2c
    SList<CIntelligentPointer<CThing> >           VillageTorches;     // +0x30
    SList<CIntelligentPointer<CThing> >           VillageHorns;       // +0x34
    SList<CIntelligentPointer<CThing> >           VillageGates;       // +0x38
    SList<CIntelligentPointer<CThingObject> >     VillageCrops;       // +0x3c
    SList<CIntelligentPointer<CThing> >           VillageCages;       // +0x40
    SList<CIntelligentPointer<CThing> >           VillageCampfires;   // +0x44
    SList<CIntelligentPointer<CThingTrackNode> >  VillagePatrols;     // +0x48
    unsigned char                         _pad_0x4c[0x24];
    SList<CIntelligentPointer<CThingMarker> >     VillageMarkers;     // +0x70
    void RemoveThingFromAllTasks(const CThing& thing);              // 0x00659EA9
    void RemoveThingAndUpdateCameraPoints(const CThing& thing);
};

void CTCVillage::RemoveThingAndUpdateCameraPoints(const CThing& thing)
{
    RemoveThingFromAllTasks(thing);
    {
        SListIterator<CIntelligentPointer<CThingAICreature> > it = find(VillagePeople.begin(), VillagePeople.end(), &thing);
        if (it != VillagePeople.end()) {
            VillagePeople.erase(it);
            return;
        }
    }
    {
        SListIterator<CIntelligentPointer<CThing> > it = find(VillageTorches.begin(), VillageTorches.end(), &thing);
        if (it != VillageTorches.end()) {
            VillageTorches.erase(it);
            return;
        }
    }
    {
        SListIterator<CIntelligentPointer<CThingBuilding> > it = find(VillageBuildings.begin(), VillageBuildings.end(), &thing);
        if (it != VillageBuildings.end()) {
            VillageBuildings.erase(it);
            return;
        }
    }
    {
        SListIterator<CIntelligentPointer<CThingObject> > it = find(VillageCrops.begin(), VillageCrops.end(), &thing);
        if (it != VillageCrops.end()) {
            VillageCrops.erase(it);
            return;
        }
    }
    {
        SListIterator<CIntelligentPointer<CThingTrackNode> > it = find(VillagePatrols.begin(), VillagePatrols.end(), &thing);
        if (it != VillagePatrols.end()) {
            VillagePatrols.erase(it);
            return;
        }
    }
    {
        SListIterator<CIntelligentPointer<CThingMarker> > it = find(VillageMarkers.begin(), VillageMarkers.end(), &thing);
        if (it != VillageMarkers.end()) {
            VillageMarkers.erase(it);
        }
    }
}