#include <math.h>
#include <stdio.h>
#include <functional>
#include <vector>

struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X, (long)Y); }
};
struct C2DBoxI {
    long Left, Bottom, Right, Top;
    C2DBoxI(const C2DCoordI& centre, long radius)
        : Left(centre.X - radius), Bottom(centre.Y - radius), Right(centre.X + radius), Top(centre.Y + radius) {}
    bool Contains(const C3DVector& pos) const
    {
        return pos.X >= Left && pos.X <= Right && pos.Y >= Bottom && pos.Y <= Top;
    }
};
struct CTCPerceiveThing {
    float SightRadius;
    explicit CTCPerceiveThing(float radius) : SightRadius(radius) {}
    float GetSightRadius() const { return SightRadius; }
};
enum ECreatureInteractionType { CREATURE_INTERACTION_CONVERSATION = 1, CREATURE_INTERACTION_TAG = 2 };
struct CThing {
    C3DVector Pos;
    CTCPerceiveThing Perception;
    bool Incapacitated;
    CThing(float x, float y, float z, float radius)
        : Pos(x,y,z), Perception(radius), Incapacitated(false) {}
    const C3DVector& GetPos() const { return Pos; }
    void PeekTC(const CTCPerceiveThing** out) const { *out = &Perception; }
};
struct CThingCreatureBase : CThing {
    CThingCreatureBase(float x, float y, float z, float radius) : CThing(x,y,z,radius) {}
};
struct CThingAICreature : CThingCreatureBase {
    unsigned Interactions;
    const CThingAICreature* AwareSource;
    CThingAICreature(float x, float y, float z, float radius)
        : CThingCreatureBase(x,y,z,radius), Interactions(0), AwareSource(0) {}
};

struct CThingFilter_IsAvailableForCreatureInteraction {
    const CThingAICreature& Source;
    ECreatureInteractionType Type;
    CThingFilter_IsAvailableForCreatureInteraction(const CThingAICreature& source, ECreatureInteractionType type)
        : Source(source), Type(type) {}
    bool operator()(const CThingAICreature& item) const { return &item != &Source && (item.Interactions & Type) != 0; }
};
struct CThingFilter_IsAICreatureAwareOf {
    const CThingAICreature* Source;
    explicit CThingFilter_IsAICreatureAwareOf(const CThingAICreature* source) : Source(source) {}
    bool operator()(const CThingAICreature& item) const { return item.AwareSource == Source; }
};
struct CThingFilter_IsIncapacitated : std::unary_function<CThing, bool> {
    bool operator()(const CThing& item) const { return item.Incapacitated; }
};
template<class A, class B, class C> struct CFilter_Bind3 {
    A First; B Second; C Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(a), Second(b), Third(c) {}
    bool operator()(const CThingAICreature& item) const { return First(item) && Second(item) && Third(item); }
};
template<class A, class B, class C>
CFilter_Bind3<A,B,C> GFFilter_Bind3(const A& a, const B& b, const C& c)
{
    return CFilter_Bind3<A,B,C>(a,b,c);
}
struct CThingCompare_Nearest {
    C3DVector Origin;
    explicit CThingCompare_Nearest(const C3DVector& pos) : Origin(pos) {}
    float Distance(const CThing& item) const
    {
        float x = item.Pos.X - Origin.X;
        float y = item.Pos.Y - Origin.Y;
        float z = item.Pos.Z - Origin.Z;
        return x*x + y*y + z*z;
    }
};
struct CThingSearchTools {
    std::vector<CThingAICreature*> Candidates;
    template<class Filter>
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const Filter& filter,
        const CThingCompare_Nearest& compare, CThingAICreature** out) const
    {
        for (unsigned i = 0; i != Candidates.size(); ++i) {
            CThingAICreature* item = Candidates[i];
            if (area.Contains(item->GetPos()) && filter(*item) &&
                (!*out || compare.Distance(*item) < compare.Distance(**out)))
                *out = item;
        }
        return *out != 0;
    }
};
struct CTCTargetingAI {
    const CThing& Parent;
    CThingSearchTools Search;
    explicit CTCTargetingAI(const CThing& parent) : Parent(parent) {}
    const CThing& GetParentThing() const { return Parent; }
    const CThingSearchTools& PeekThingSearchTools() const { return Search; }
    virtual CThingCreatureBase* GetPBestCreatureForInteractionType(ECreatureInteractionType) const;
};

CThingCreatureBase* CTCTargetingAI::GetPBestCreatureForInteractionType(ECreatureInteractionType interaction_type) const
{
    const CThingAICreature& parent_creature = static_cast<const CThingAICreature&>(GetParentThing());
    const C3DVector& pos = parent_creature.GetPos();
    const CTCPerceiveThing* ptc_perceive;
    parent_creature.PeekTC(&ptc_perceive);
    CThingAICreature* thing_creature = 0;
    C2DBoxI area(pos.ToC2DCoordI(), (long)ptc_perceive->GetSightRadius());
    PeekThingSearchTools().GetPBestThingInRoughArea(area,
        GFFilter_Bind3(CThingFilter_IsAvailableForCreatureInteraction(parent_creature, interaction_type),
                      CThingFilter_IsAICreatureAwareOf(&parent_creature), std::not1(CThingFilter_IsIncapacitated())),
        CThingCompare_Nearest(pos), &thing_creature);
    return thing_creature;
}

int main()
{
    CThingAICreature owner(5.75f,-4.5f,0,5.9f);
    CTCTargetingAI targeting(owner);
    if (targeting.GetPBestCreatureForInteractionType(CREATURE_INTERACTION_CONVERSATION) != 0)
        return 1;
    CThingAICreature closest(6,-4,0,0);
    CThingAICreature eligible(10,-4,1,0);
    CThingAICreature farther(5,-8,4,0);
    CThingAICreature outside(11,-4,0,0);
    CThingAICreature unaware(7,-4,0,0);
    closest.Incapacitated = true;
    closest.Interactions = eligible.Interactions = farther.Interactions = outside.Interactions = unaware.Interactions =
        CREATURE_INTERACTION_CONVERSATION;
    closest.AwareSource = eligible.AwareSource = farther.AwareSource = outside.AwareSource = &owner;
    targeting.Search.Candidates.push_back(&farther);
    targeting.Search.Candidates.push_back(&closest);
    targeting.Search.Candidates.push_back(&unaware);
    targeting.Search.Candidates.push_back(&outside);
    targeting.Search.Candidates.push_back(&eligible);
    if (targeting.GetPBestCreatureForInteractionType(CREATURE_INTERACTION_CONVERSATION) != &eligible)
        return 2;
    if (targeting.GetPBestCreatureForInteractionType(CREATURE_INTERACTION_TAG) != 0)
        return 3;
    eligible.Interactions = CREATURE_INTERACTION_TAG;
    if (targeting.GetPBestCreatureForInteractionType(CREATURE_INTERACTION_TAG) != &eligible)
        return 4;
    if (targeting.GetPBestCreatureForInteractionType(CREATURE_INTERACTION_CONVERSATION) != &farther)
        return 5;
    printf("007f68b0_TEST PASS\n");
    return 0;
}