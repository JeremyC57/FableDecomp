// CTCBoastingArea::FrameUpdate() -- track the living hero against the area's anticipation and boasting radii and fire the enter/leave/approach notifications on each state change
// Leaf: ?FrameUpdate@CTCBoastingArea@@QAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail 0x007828E0, 432 bytes (plain `ret`). Callees: CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer
// @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CThing::GetPos @ 0x004C73D0, CTCBoastingArea::OnHeroEnteringArea
// @ 0x007827D0, OnHeroLeavingArea @ 0x00782860, CVectorMap LowerBound @ 0x0040F020 (inlined GetInterface 0x73),
// CTCBoastingCrowdControl::OnHeroApproachPodiumArea @ 0x00783BC0.
// Layout = Ego_r.pdb CTCBoastingArea: HeroWithinRadiusState +0x0c, Radius +0x10, AnticipationRadius +0x14 (CTCBase +4 parent).
// Shape: each radius test is an inline InRange(offset, radius) over `thing - hero` (the radius is a by-value float loaded
// before the squares, `X*X + (Y*Y + Z*Z)` for retail's Z,Y,X order); the anticipation result lives in a frame byte,
// the boasting one in cl; the state switch returns from every arm. The parent thing goes through an inline GetThing()
// accessor so it is read into ebx BEFORE the hero position call (a direct `Thing->` loads it after and keeps the bool in bl).
class CTCBase;
#include "engine/C3DVector.h"
__forceinline bool InRange(const C3DVector& d, float radius)
{
    return d.X * d.X + (d.Y * d.Y + d.Z * d.Z) < radius * radius;
}
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                      // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCBase { protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40 (Ego_r.pdb CTCBase, protected)
};
class CTCBoastingCrowdControl { public:
    void OnHeroApproachPodiumArea();                // 0x00783BC0
};

enum EHeroWithinRadiusState { HERO_OUTSIDE = 0, HERO_IN_ANTICIPATION = 1, HERO_IN_RADIUS = 2 };
struct CTCBoastingArea : public CTCBase {
    void*   __vftable;                              // +0x00
    CThing* Thing;                                  // +0x04 CTCBase parent thing
    unsigned char _pad08[4];
    EHeroWithinRadiusState HeroWithinRadiusState;   // +0x0c
    float   Radius;                                 // +0x10
    float   AnticipationRadius;                     // +0x14
    void OnHeroEnteringArea(CThing* hero);          // 0x007827D0
    void OnHeroLeavingArea(CThing* hero);           // 0x00782860
    void FrameUpdate();
    CThing* GetThing() const { return Thing; }
};

void CTCBoastingArea::FrameUpdate()
{
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead()) {
        HeroWithinRadiusState = HERO_OUTSIDE;
        return;
    }
    bool inAnticipation = InRange(GetThing()->GetPos() - hero->GetPos(), AnticipationRadius);
    bool inRadius = InRange(GetThing()->GetPos() - hero->GetPos(), Radius);
    switch (HeroWithinRadiusState) {
    case HERO_OUTSIDE:
        if (!inAnticipation)
            return;
        if (inRadius) {
            HeroWithinRadiusState = HERO_IN_RADIUS;
            OnHeroEnteringArea(hero);
            return;
        }
        HeroWithinRadiusState = HERO_IN_ANTICIPATION;
        {
            CTCBoastingCrowdControl* crowd;
            if (hero->GetTC(TCI_BOASTING_CROWD_CONTROL, &crowd))
                crowd->OnHeroApproachPodiumArea();
        }
        return;
    case HERO_IN_ANTICIPATION:
        if (!inAnticipation) {
            HeroWithinRadiusState = HERO_OUTSIDE;
            return;
        }
        if (inRadius) {
            HeroWithinRadiusState = HERO_IN_RADIUS;
            OnHeroEnteringArea(hero);
        }
        return;
    case HERO_IN_RADIUS:
        if (inRadius)
            return;
        if (inAnticipation) {
            HeroWithinRadiusState = HERO_IN_ANTICIPATION;
            OnHeroLeavingArea(hero);
        } else {
            HeroWithinRadiusState = HERO_OUTSIDE;
            OnHeroLeavingArea(hero);
        }
        return;
    }
}