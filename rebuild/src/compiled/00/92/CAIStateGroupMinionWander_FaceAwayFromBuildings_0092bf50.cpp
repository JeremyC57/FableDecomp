// CAIStateGroup_MinionWander::FaceAwayFromBuildings() -- find the nearest building leaf thing within 10 units (2D) of the creature; when it is alive and in the creature's field of view, look at the point mirrored away from it (creature + (creature - building)); otherwise return 2 (done)
// Leaf: ?FaceAwayFromBuildings@CAIStateGroup_MinionWander@@AAE?AW4EAIActionReturn@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092BF50, 287 bytes, plain `ret` (private, Ego_r.pdb AAE?AW4EAIActionReturn@@XZ).
// Callees: CThingPhysical::PeekPos 0x006A4D00 (x5), CAIStateGroupBase::PeekThingSearchTools 0x008FD000 (protected const),
//   CThingSearchTools::GetPBestLeafThing<CThingFilter_In2DRange, CThingCompare_Nearest, CThingBuilding> 0x0092C260,
//   CThingCreatureBase::IsThingInFOV(const CThing&, bool) const 0x00665190, CAIStateGroupBase::AIAction_LookAtPos 0x0097D580
//   (protected; C3DVector by value). Floats: 10.0f (filter range), 0.0027777f (0x3B360A13, the look speed).
// Same TU in retail: CThingCompare_Nearest::operator()<CThingBuilding> 0x0092BE70 and GetPBestLeafThing 0x0092C260 (both landed);
//   their template bodies are reproduced here (both still byte-match in this TU). Seeing them, VC7.1 knows the search does not keep
//   &building (the found thing stays in edi) and schedules the compare's position copy as retail does.
// Layout: Ego_r.pdb CThingFilter_In2DRange {C3DVector Pos; float Range} (16), CThingCompare_Nearest {C3DVector Pos} (12);
//   PBrain +0x04 -> Creature +0x18; CThing state byte +0x91 (Dead bit 0).
// Shape: the look target is written `creature + (creature - building)`: the difference (z, y, x) is spilled across the third
//   PeekPos call and the sum is stored straight into the argument slot.
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
enum EAIActionReturn { AI_ACTION_RETURN_DONE = 2 };
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 1 };
class CClass;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct _List_node {
    _List_node<_Tp>* _M_next;
    _List_node<_Tp>* _M_prev;
    _Tp _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    _List_node<_Tp>* _M_node;
};
}
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char StateBits;                        // +0x91 Dead 0x01 / IncludeInThingScans 0x04 / Activated 0x08 / InLimbo 0x20
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
    bool IsDead() const { return (StateBits & 0x01) != 0; }
    const C3DVector& GetPos() const;                // 0x004C73D0
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;               // 0x006A4D00
};
class CThingBuilding : public CThingPhysical { public: enum { ThingListType = 3 }; };
class CThingAICreature : public CThingPhysical { public:
    bool IsThingInFOV(const CThing& thing, bool full) const;   // 0x00665190 (CThingCreatureBase)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                     // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }
};
class CThingFilter_In2DRange { public:
    C3DVector Pos;                                  // +0x00
    float     Range;                                // +0x0c
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
    bool operator()(const CThing& thing) const;     // 0x006A76B0
};
class CThingCompare_Nearest { public:
    C3DVector Pos;                                  // +0x00
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
    template <class T> T* operator()(T* a, T* b) const;   // 0x0092BE70 for CThingBuilding
};
// Same TU in retail: CThingCompare_Nearest::operator()<CThingBuilding> 0x0092BE70 (landed body) precedes this row.
template <class T> T* CThingCompare_Nearest::operator()(T* a, T* b) const
{
    if (!a)
        return b;
    if (!b)
        return a;
    C3DVector deltaA = a->GetPos() - Pos;
    C3DVector deltaB = b->GetPos() - Pos;
    return (deltaA.GetSquaredMagnitude() < deltaB.GetSquaredMagnitude()) ? a : b;
}
class CThingSearchTools {
protected:
    const std::list<CClass*>& PeekTypeList(long type) const;   // 0x0049C780
public:
    template <class TFilter, class TCompare, class TThing> bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out) const;
};
// Same TU in retail: the GetPBestLeafThing instantiation 0x0092C260 (landed body) follows this row.
template <class TFilter, class TCompare, class TThing>
bool CThingSearchTools::GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out) const
{
    TThing* best = 0;
    const std::list<CClass*>& things = PeekTypeList(TThing::ThingListType);
    for (std::_List_node<CClass*>* it = things._M_node->_M_next; it != things._M_node; it = it->_M_next)
    {
        CThing* thing = (CThing*)it->_M_data;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            best = compare(best, (TThing*)thing);
    }
    *out = best;
    return best != 0;
}
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
protected:
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long frames, float speed);   // 0x0097D580
};
class CAIStateGroup_MinionWander : public CAIStateGroupBase {
private:
    EAIActionReturn FaceAwayFromBuildings();
};

EAIActionReturn CAIStateGroup_MinionWander::FaceAwayFromBuildings()
{
    CThingBuilding* building = 0;
    PeekThingSearchTools().GetPBestLeafThing(CThingFilter_In2DRange(GetCreature().PeekPos(), 10.0f), CThingCompare_Nearest(GetCreature().PeekPos()), &building);
    if (building && !building->IsDead() && GetCreature().IsThingInFOV(*building, true))
        return AIAction_LookAtPos(AI_ACTION_REPEAT_ONCE, GetCreature().PeekPos() + (GetCreature().PeekPos() - building->PeekPos()), 0x11, 0.0027777f);
    return AI_ACTION_RETURN_DONE;
}