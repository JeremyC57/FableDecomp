# h26_tbvm.py: naming wave 26, family tbvm -- 0x00744580 is CTCPerceiveThing::GetTimeBasedVisibilityMultiplier (DECISIONS_W25_RESOLVED D5).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
he.put('00744580', 'src', '''// CTCPerceiveThing::GetTimeBasedVisibilityMultiplier() const -- the perceived-thing def's day-time visibility multiplier by day, its
//   night-time one by night, their average at dusk and dawn
// Leaf: ?GetTimeBasedVisibilityMultiplier@CTCPerceiveThing@@ABEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00744580, 72 bytes, thiscall, `ret`; 0x0122F59C = 0.5f.
// Identity (naming wave 26, DECISIONS_W25_RESOLVED D5): Ego_r.pdb ?GetTimeBasedVisibilityMultiplier@CTCPerceiveThing@@ABEMXZ (private,
//   const, float) and FableWin.pdb the same symbol; the body reads CPerceivedThingDef::DayTimeVisibilityMultiplier +0x48 /
//   NightTimeVisibilityMultiplier +0x4c through PDef +0x34 (Ego_r.pdb layouts), and sits in the PDB's CTCPerceiveThing order after the
//   Set*Radius setters 0x00744470..0x00744540 and before the ctor 0x007445E0. GetSightRadius 0x007448F0 / GetExtendedSightRadius
//   0x00744950 expand it inline. Was landed as a free CTCPerceiveThing_GetExtendedSightRadius over GetCtx / CheckA / CheckB stand-ins.
// Callees: ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 0x00686A80 (DECISIONS_W24_RESOLVED item 7), ?IsDay@CGameTimeManager@@QBE_NXZ 0x006BB2C0,
//   ?IsNightTime@CGameTimeManager@@QBE_NXZ 0x006BB210 (Ego_r.pdb / FableWin.pdb) on CWorld +0x1c scoped_ptr<CGameTimeManager> PGameTimeManager.
#define CTCPerceiveThing CTCPerceiveThingLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing
#include "engine/CPerceivedThingDef.h"

class CGameTimeManager { public:
    bool IsNightTime() const;                // 0x006BB210
    bool IsDay() const;                      // 0x006BB2C0
};
class CWorld { public:
    unsigned char     _pad00[0x1c];
    CGameTimeManager* PGameTimeManager;      // +0x1c scoped_ptr<CGameTimeManager> (its one pointer)
};
class CTCBase {
protected:
    const CWorld& PeekWorld() const;         // 0x00686A80
};
class CTCPerceiveThing : public CTCBase, public CTCPerceiveThingLayout {
    float GetTimeBasedVisibilityMultiplier() const;
};

float CTCPerceiveThing::GetTimeBasedVisibilityMultiplier() const
{
    if (PeekWorld().PGameTimeManager->IsDay())
        return reinterpret_cast<const CPerceivedThingDef*>(PDef_Object)->DayTimeVisibilityMultiplier;
    if (PeekWorld().PGameTimeManager->IsNightTime())
        return reinterpret_cast<const CPerceivedThingDef*>(PDef_Object)->NightTimeVisibilityMultiplier;
    const CPerceivedThingDef* def = reinterpret_cast<const CPerceivedThingDef*>(PDef_Object);
    return (def->NightTimeVisibilityMultiplier + def->DayTimeVisibilityMultiplier) * 0.5f;
}
''')
