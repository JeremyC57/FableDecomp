// CTCPerceiveThing::GetSightRadius() -- SightRadius scaled by the def's day-time / night-time visibility multiplier (their average at dusk and dawn)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007448F0, 81 bytes (catalogued as GetExtendedSightRadius; it scales SightRadius +0x40 -- the
//   0x00744950 twin scales ExtendedSightRadius +0x44).
// Callees: 0x00686A80 (this-ignoring member returning g_pMainGameComponent->+0x24, the world; manifest "CViewBase::GetDisplayEngine",
//   propagated), CGameTimeManager::IsDay @ 0x006BB2C0 and ::IsNightTime @ 0x006BB210 on the world's +0x1c. 0x0122F59C = 0.5f.
// Layout: engine/CTCPerceiveThing.h (SightRadius +0x40, PDef_Object +0x34) and engine/CPerceivedThingDef.h
//   (DayTimeVisibilityMultiplier +0x48, NightTimeVisibilityMultiplier +0x4c).
// Shape: the inline visibility multiplier (landed standalone at 0x00744580) times the radius, multiplier loaded first; each
//   arm returns on its own (`fmul [esi+0x40] ; pop esi ; ret` three times).
// Leaf: GetSightRadius
#define CTCPerceiveThing CTCPerceiveThingLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
class CWorld;

class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCPerceiveThing : public CTCBase, public CTCPerceiveThingLayout
{
};
#include "engine/CPerceivedThingDef.h"

class CGameTimeManager { public:
    bool IsDay() const;                     // 0x006BB2C0
    bool IsNightTime() const;               // 0x006BB210
};

class CWorld { public:
    unsigned char     _pad00[0x1c];
    CGameTimeManager* GameTimeManager;      // +0x1c
};

struct CTCPerceiveThing_Methods : CTCPerceiveThing {
    const CPerceivedThingDef* GetDef() const { return reinterpret_cast<const CPerceivedThingDef*>(PDef_Object); }
    float GetVisibilityMultiplier() const
    {
        if (PeekWorld().GameTimeManager->IsDay())
            return GetDef()->DayTimeVisibilityMultiplier;
        if (PeekWorld().GameTimeManager->IsNightTime())
            return GetDef()->NightTimeVisibilityMultiplier;
        return (GetDef()->NightTimeVisibilityMultiplier + GetDef()->DayTimeVisibilityMultiplier) * 0.5f;
    }
    float GetSightRadius() const;
};

float CTCPerceiveThing_Methods::GetSightRadius() const
{
    return GetVisibilityMultiplier() * SightRadius;
}