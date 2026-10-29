// CTCBoastingArea::FrameUpdate() -- track the living hero against the area's anticipation and boasting radii and fire the enter/leave/approach notifications on each state change
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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
__forceinline bool InRange(const C3DVector& d, float radius)
{
    return d.X * d.X + (d.Y * d.Y + d.Z * d.Z) < radius * radius;
}
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                         // +0x91 bit 0
    const C3DVector& GetPos() const;           // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                      // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
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
    if (!hero || hero->Dead) {
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
            if (hero->GetInterface(&crowd, 0x73))
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