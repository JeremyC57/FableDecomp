#include <stdio.h>
#include <list>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/ThingAlive.h"
#include "fable_assert.h"

template<class T>
class CIntelligentPointer {
public:
    CIntelligentPointer(T* item) : Item(item) {}
    T* Get() const { return Item; }
    T* operator->() const { return Item; }
private:
    T* Item;
};

template<class T>
bool operator!=(const CIntelligentPointer<T>& pointer, int zero)
{
    return zero == 0 && pointer.Get() != 0;
}

class CThing {
public:
    CThing(unsigned long type, bool dead, const C3DVector& pos)
        : Type(type), Dead(dead), Pos(pos) {}
    unsigned long GetType() const { return Type; }
    bool IsDead() const { return Dead; }
    const C3DVector& GetPos() const { return Pos; }
private:
    unsigned long Type;
    bool Dead;
    C3DVector Pos;
};

class CTCSingleUserActionPoints {
public:
    explicit CTCSingleUserActionPoints(bool free) : Free(free) {}
    bool IsActionPointFree() const { return Free; }
private:
    bool Free;
};

class CThingObject : public CThing {
public:
    CThingObject(bool dead, bool space, const C3DVector& pos, bool campfire = true,
                 bool hasPoints = true)
        : CThing(StatGetType(), dead, pos), Properties(campfire ? 0x4000UL : 0),
          HasPoints(hasPoints), Points(space) {}
    static long StatGetType() { return 5; }
    bool HasProperty(unsigned long property) const { return (Properties & property) != 0; }
    bool PeekTC(const CTCSingleUserActionPoints** points) const
    {
        if (!HasPoints)
            return false;
        *points = &Points;
        return true;
    }
private:
    unsigned long Properties;
    bool HasPoints;
    CTCSingleUserActionPoints Points;
};

template<class T, class U>
T* GFDebugPDynamicCast(U* item, T*) { return static_cast<T*>(item); }

class CTCVillage {
public:
    std::list<CIntelligentPointer<CThing> > Fires;
    const std::list<CIntelligentPointer<CThing> >& PeekVillageCampfires() const { return Fires; }
};

class CTCVillageMember {
public:
    explicit CTCVillageMember(CTCVillage* village) : Village(village) {}
    CTCVillage* GetPVillage() const { return Village; }
private:
    CTCVillage* Village;
};

class CThingAICreature {
public:
    CThingAICreature(const CTCVillageMember& member, const C3DVector& pos)
        : Member(member), Pos(pos) {}
    bool PeekTC(const CTCVillageMember** member) const { *member = &Member; return true; }
    const C3DVector& GetPos() const { return Pos; }
private:
    const CTCVillageMember& Member;
    C3DVector Pos;
};

class CIsCampfireWithSpace {
public:
    bool operator()(const CThingObject& campfire) const;
};

inline bool CIsCampfireWithSpace::operator()(const CThingObject& campfire) const
{
    const CTCSingleUserActionPoints* ptc_action_points;
    if (campfire.HasProperty(0x4000UL) && campfire.PeekTC(&ptc_action_points))
        return ptc_action_points->IsActionPointFree();
    return false;
}

class CIsNearCampfireWithSpace {
public:
    CIsNearCampfireWithSpace(float distance, const C3DVector& from_pos) : FromPos(from_pos)
    {
        DistanceSq = GFSqr(distance);
    }
    bool operator()(const CThingObject& campfire) const;
    float GetDistanceSq() const { return DistanceSq; }
    const C3DVector& GetFromPos() const { return FromPos; }
private:
    float DistanceSq;
    C3DVector FromPos;
};

class CThingCompare_Nearest {
public:
    explicit CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
    template<class T> T* operator()(T* first, T* second) const;
    const C3DVector& GetPos() const { return Pos; }
private:
    C3DVector Pos;
};

// FableWin 02B7A940, matching the predicate called at native 0093B698.
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline bool CIsNearCampfireWithSpace::operator()(const CThingObject& campfire) const
{
    if (campfire.HasProperty(0x4000UL) && (campfire.GetPos() - FromPos).GetSquaredMagnitude() <= DistanceSq) {
        const CTCSingleUserActionPoints* ptc_action_points;
        if (campfire.PeekTC(&ptc_action_points))
            return ptc_action_points->IsActionPointFree();
    }
    return false;
}

// FableWin 02B6CED0, matching the candidate type at native 0093B6A7.
template<> inline
#ifdef _MSC_VER
__declspec(noinline)
#endif
CThingObject* CThingCompare_Nearest::operator()(CThingObject* first, CThingObject* second) const
{
    if (!first)
        return second;
    if (!second)
        return first;
    return (first->GetPos() - Pos).GetSquaredMagnitude() <
           (second->GetPos() - Pos).GetSquaredMagnitude() ? first : second;
}

class CThingSearchTools {
public:
    explicit CThingSearchTools(CThingObject* result)
        : Calls(0), CorrectArguments(false), Result(result) {}
    bool GetPBestThing(const CIsNearCampfireWithSpace& filter, const CThingCompare_Nearest& compare,
                       CThingObject** fire) const
    {
        ++Calls;
        const C3DVector& filter_origin = filter.GetFromPos();
        const C3DVector& compare_origin = compare.GetPos();
        CorrectArguments = filter.GetDistanceSq() == 484.0f && filter_origin.X == 1.0f &&
                           filter_origin.Y == 2.0f && filter_origin.Z == 3.0f &&
                           compare_origin.X == 1.0f && compare_origin.Y == 2.0f &&
                           compare_origin.Z == 3.0f;
        *fire = Result && filter(*Result) ? compare((CThingObject*)0, Result) : 0;
        return *fire != 0;
    }
    mutable unsigned Calls;
    mutable bool CorrectArguments;
private:
    CThingObject* Result;
};

class CAIStateGroup_BanditSitRoundCampfire {
public:
    CAIStateGroup_BanditSitRoundCampfire(const CThingAICreature& creature, const CThingSearchTools& search)
        : Creature(creature), Search(search) {}
    CThingObject* GetPNearCampfire() const;
private:
    const CThingAICreature& GetCreature() const { return Creature; }
    const CThingSearchTools& PeekThingSearchTools() const { return Search; }
    const CThingAICreature& Creature;
    const CThingSearchTools& Search;
};


CThingObject* CAIStateGroup_BanditSitRoundCampfire::GetPNearCampfire() const
{
    CThingObject* pcampfire = 0;
    const CTCVillageMember* ptc_vill_mem;
    if (!GetCreature().PeekTC(&ptc_vill_mem))
        FABLE_RETAIL_ASSERT_ARM(false);

    const CTCVillage* ptc_village = ptc_vill_mem->GetPVillage();
    if (ptc_village) {
        std::list<CIntelligentPointer<CThing> > village_campfires(ptc_village->PeekVillageCampfires());
        CIsCampfireWithSpace is_campfire_with_space;
        float best_sq_dist = 0.0f;
        {
            for (std::list<CIntelligentPointer<CThing> >::const_iterator it = village_campfires.begin(),
                 end_it = village_campfires.end(); it != end_it; ++it) {
                if (IS_THING_ALIVE(*it) && (*it)->GetType() == CThingObject::StatGetType()) {
                    CThingObject* pthis_campfire = GFDebugPDynamicCast((*it).Get(), (CThingObject*)0);
                    if (is_campfire_with_space(*pthis_campfire)) {
                        float this_sq_dist = (pthis_campfire->GetPos() - GetCreature().GetPos()).GetSquaredMagnitude();
                        if (!pcampfire || this_sq_dist < best_sq_dist) {
                            pcampfire = pthis_campfire;
                            best_sq_dist = this_sq_dist;
                        }
                    }
                }
            }
        }
    }
    if (!pcampfire) {
        PeekThingSearchTools().GetPBestThing(CIsNearCampfireWithSpace(22.0f, GetCreature().GetPos()),
                                            CThingCompare_Nearest(GetCreature().GetPos()), &pcampfire);
    }
    return pcampfire;
}



static bool Check(CTCVillage* village, CThingObject* fallback, CThingObject* expected, bool usesSearch)
{
    CTCVillageMember member(village);
    CThingAICreature creature(member, C3DVector(1.0f, 2.0f, 3.0f));
    CThingSearchTools search(fallback);
    CAIStateGroup_BanditSitRoundCampfire group(creature, search);
    return group.GetPNearCampfire() == expected &&
           search.Calls == (usesSearch ? 1U : 0U) &&
           (!usesSearch || search.CorrectArguments);
}

int main()
{
    CThingObject fallback(false, true, C3DVector(6.0f, 5.0f, 4.0f));
    CTCVillage village;
    if (!Check(0, 0, 0, true) || !Check(0, &fallback, &fallback, true) ||
        !Check(&village, &fallback, &fallback, true))
        return 1;

    CThing wrongType(4, false, C3DVector(1.0f, 2.0f, 3.0f));
    CThingObject dead(true, true, C3DVector(1.0f, 2.0f, 3.0f));
    CThingObject full(false, false, C3DVector(1.0f, 2.0f, 3.0f));
    CThingObject noPoints(false, true, C3DVector(1.0f, 2.0f, 3.0f), true, false);
    CThingObject notCampfire(false, true, C3DVector(1.0f, 2.0f, 3.0f), false);
    village.Fires.push_back(&notCampfire);
    village.Fires.push_back(&noPoints);
    village.Fires.push_back(0);
    village.Fires.push_back(&dead);
    village.Fires.push_back(&wrongType);
    village.Fires.push_back(&full);
    if (!Check(&village, &fallback, &fallback, true))
        return 2;

    CThingObject far(false, true, C3DVector(1.0f, 2.0f, 6.0f));
    CThingObject near(false, true, C3DVector(2.0f, 2.0f, 3.0f));
    CThingObject tied(false, true, C3DVector(1.0f, 3.0f, 3.0f));
    village.Fires.push_back(&far);
    if (!Check(&village, &fallback, &far, false))
        return 3;

    village.Fires.push_back(&near);
    village.Fires.push_back(&far);
    village.Fires.push_back(&tied);
    if (!Check(&village, &fallback, &near, false))
        return 4;
    const C3DVector origin(1.0f, 2.0f, 3.0f);
    const CIsNearCampfireWithSpace in_range(22.0f, origin);
    CThingObject at_limit(false, true, C3DVector(23.0f, 2.0f, 3.0f));
    CThingObject beyond_limit(false, true, C3DVector(24.0f, 2.0f, 3.0f));
    if (!in_range(near) || !in_range(at_limit) || in_range(beyond_limit) ||
        in_range(full) || in_range(noPoints) || in_range(notCampfire))
        return 5;
    if (!Check(0, &at_limit, &at_limit, true) || !Check(0, &beyond_limit, 0, true))
        return 6;

    const CThingCompare_Nearest compare(origin);
    if (compare((CThingObject*)0, &near) != &near || compare(&near, (CThingObject*)0) != &near ||
        compare(&near, &far) != &near || compare(&far, &near) != &near ||
        compare(&near, &tied) != &tied)
        return 7;
    puts("0093ab70_TEST PASS");
}