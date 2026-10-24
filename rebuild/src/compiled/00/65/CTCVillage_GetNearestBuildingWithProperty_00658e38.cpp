// CTCVillage::GetNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property) -- run the nearest-building functor over the village's buildings and return the building it kept
// Leaf: GetNearestBuildingWithProperty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00658E38, 59 bytes, thiscall `leave ; ret 8`. Name from FableWin.pdb / Ego_r.pdb
// (?GetNearestBuildingWithProperty@CTCVillage@@QAEPAVCThingBuilding@@ABVC3DVector@@W4ESimBuildingProperty@@@Z, used as a
// name set); callers are AI state groups (0x0090FB34, 0x0094794D, 0x00949D37, 0x0094B90E, 0x0096D2BB).
// Callees: CNearestBuildingWithProperty(const C3DVector&, ESimBuildingProperty) @ 0x0065846C (built straight in its
// by-value argument slot), std::for_each<list<CIntelligentPointer<CThingBuilding>>::iterator,
// CNearestBuildingWithProperty> @ 0x0065F5D2 (`__fastcall`, both iterators on the stack, functor result through the
// hidden pointer [ebp-18h]; landed).
// Label correction: the catalog "AIStateGroup_FindNearestBuildingWithProperty" is descriptive.
// Shape: `return for_each(VillageBuildings.begin(), VillageBuildings.end(), CNearestBuildingWithProperty(pos,
// property)).NearestBuilding;` -- right-to-left argument order gives the functor, then end() (the sentinel), then
// begin() (sentinel->next), each iterator copy-built in its slot (`push ecx ; mov eax,esp ; mov [eax],ecx`).
// Layout: local model of CTCVillage: engine/CTCVillage.h carries VillageBuildings at +0x28 as an opaque 0xC-byte PDB
// list, but the retail STLport list is the 4-byte sentinel pointer and the body needs begin()/end(); only +0x28 is used.
// Opt: size TU (`optimize("s", on)`; the frame comes from the functor result local).
#pragma optimize("s", on)

class C3DVector {
public:
    float X, Y, Z;
};
class CThing;
class CThingBuilding;
enum ESimBuildingProperty { SIM_BUILDING_PROPERTY_NONE = 0 };

class CBaseIntelligentPointer {                   // 8 bytes: vptr + item
public:
    virtual void v00();
    CThing* m_pItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

class CNearestBuildingWithProperty {             // Ego_r.pdb, 0x18 bytes
public:
    ESimBuildingProperty BuildingProperty;        // +0x00
    C3DVector            Pos;                     // +0x04
    CThingBuilding*      NearestBuilding;         // +0x10
    float                BestDistanceSquared;     // +0x14
    CNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property);   // 0x0065846C
    CNearestBuildingWithProperty(const CNearestBuildingWithProperty& other);             // 0x0065D750
    void operator()(const CIntelligentPointer<CThingBuilding>& building);               // 0x0065F056
};

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                     // +0x00
    _List_node_base* _M_prev;                     // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                  // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _Node* _M_node;                               // retail STLport list: the sentinel pointer only (4 bytes)
    iterator begin() { return iterator((_Node*)(_M_node->_M_next)); }
    iterator end() { return iterator(_M_node); }
};
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);   // 0x0065F5D2
}

class CTCVillage {
public:
    unsigned char _pad00[0x28];
    std::list<CIntelligentPointer<CThingBuilding> > VillageBuildings;   // +0x28
    CThingBuilding* GetNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property);
};

CThingBuilding* CTCVillage::GetNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property)
{
    return std::for_each(VillageBuildings.begin(), VillageBuildings.end(), CNearestBuildingWithProperty(pos, property)).NearestBuilding;
}