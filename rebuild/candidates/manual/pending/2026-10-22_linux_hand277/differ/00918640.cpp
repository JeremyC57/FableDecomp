// Grade: DIFFER(581v570) -- early 'return 0' exits keep the vector free test (retail folds it), register allocation (this in esi v ebx), the random's zero-count test folded
// CAIStateGroup_Gaze::FindFreeParticularGazePos(long, bool) -- collect the free, living gaze positions of a given type (in the building, optionally inside it; else around the creature's region) and pick one at random
// Retail: Fable.exe (TLC, VC7.1) @ 0x00918640, 576 bytes (ret 8).
// Callees: the state group's virtual slot 14 (+0x38, current building), CVectorMap::LowerBound 0x0040F020 (same-TU
//   stand-in; inlined GetInterface 0x21 / 0x23), CBaseIntelligentPointer::GetPItem 0x00A01B50, the free-position test
//   0x006A8AF0 (catalog "CTCInterface_FindInterface", propagated), CThing::GetPosition 0x004C73D0,
//   CThingBuilding::IsPointInside 0x0082D000, vector<CThing*>::push_back 0x00436C80, the region accessor 0x007EE540,
//   CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890, free 0x00BFEA14.
// Layout: retail CThing interface bitset +0x20 / map +0x44, dead bit +0x91; gaze type word at +0x8c; the gaze
// list component's STLport list at +0x0c; the region thing's list at +0x70.
#include <stdlib.h>
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CPointerList;
struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short GazeType;                    // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& pos);   // 0x0082D000
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CPointerListNode {                       // STLport list node of CIntelligentPointer<CThing>
    CPointerListNode* _M_next;
    CPointerListNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CPointerList {
    CPointerListNode* _M_node;
};
struct CRegionThing {
    unsigned char _pad00[0x70];
    CPointerList m_gazeList;                    // +0x70
};
struct CTCGazePositions {                       // interface 0x21
    unsigned char _pad00[0x0c];
    CPointerList Positions;                     // +0x0c
};
struct CRegionThing;
struct CTCRegionLink {                          // interface 0x23
    CRegionThing* GetPRegion();                 // 0x007EE540
};
struct CGazePosFree {
    bool operator()(CThing* thing) const;       // 0x006A8AF0
};
struct CThingVector {                           // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    void push_back(CThing* const& thing);       // 0x00436C80
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThing* Creature;                           // +0x18
};
struct CAIStateGroupBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34();
    virtual CThingBuilding* GetPBuilding();     // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};
__forceinline unsigned long GetRandomIndex(CAIStateGroupBase* group, unsigned long n)
{
    unsigned long* seed = group->GetPWorldSeed();
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return n ? *seed % n : 0;
}
struct CAIStateGroup_Gaze : CAIStateGroupBase {
    CThing* FindFreeParticularGazePos(long gazeType, bool mustBeInside);
};

CThing* CAIStateGroup_Gaze::FindFreeParticularGazePos(long gazeType, bool mustBeInside)
{
    CThingVector candidates;
    CThingBuilding* building = GetPBuilding();
    if (building) {
        if (!building->HasTC(0x21))
            return 0;
        CTCGazePositions* positions = (CTCGazePositions*)building->GetTC(0x21);
        CGazePosFree isFree;
        for (CPointerListNode* node = positions->Positions._M_node->_M_next; node != positions->Positions._M_node;
             node = node->_M_next) {
            CBaseIntelligentPointer& pos = node->_M_data;
            if (IS_THING_ALIVE(pos) && pos.GetPItem()->GazeType == gazeType && isFree(pos.GetPItem())
                && (!mustBeInside || building->IsPointInside(pos.GetPItem()->GetPosition())))
                candidates.push_back(pos.GetPItem());
        }
    } else {
        CThing* creature = GetCreature();
        if (!creature->HasTC(0x23))
            return 0;
        CTCRegionLink* link = (CTCRegionLink*)creature->GetTC(0x23);
        if (!link->GetPRegion())
            return 0;
        CPointerList* list = &link->GetPRegion()->m_gazeList;
        CGazePosFree isFree;
        for (CPointerListNode* node = list->_M_node->_M_next; node != list->_M_node; node = node->_M_next) {
            CBaseIntelligentPointer& pos = node->_M_data;
            if (IS_THING_ALIVE(pos) && pos.GetPItem()->GazeType == gazeType && isFree(pos.GetPItem()))
                candidates.push_back(pos.GetPItem());
        }
    }
    unsigned long count = candidates.size();
    if (count > 0)
        return candidates._M_start[GetRandomIndex(this, count)];
    return 0;
}
