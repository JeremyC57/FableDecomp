// CTCDExplosiveTrail::AddThingToIgnore(CThing& thing) -- append the thing to the trail's ThingsToIgnore list
// Leaf: ?AddThingToIgnore@CTCDExplosiveTrail@@QAEXAAVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810970, 66 bytes, thiscall, `ret 4`.
// Identity (proven, agentFAM07): Ego_r.pdb ?AddThingToIgnore@CTCDExplosiveTrail@@QAEXAAVCThing@@@Z, and the body is exactly that:
//   push_back(CIntelligentPointer<CThing>(&thing)) into the vector at +0x20, which is CTCDExplosiveTrail::ThingsToIgnore (Ego_r
//   +40 behind the Dinkumware 12-byte list ComponentTraps; the retail list is 4 bytes, so +0x20 -- the layout the landed
//   CTCDExplosiveTrail::OnInitialActivate 0x008100A0 in this TU uses). It sits in the CTCDExplosiveTrail TU (0x008100A0 ..
//   0x00810D40). Its only caller, CCreatureAction_IceTrollIcicleAttack::PerformAction 0x008C6DB0, calls it on the driver
//   (CThing::PTCDriver, +0x5C) of the thing CTCDExplosiveTrail::Create 0x008100D0 returns, passing the troll.
//   Was the placeholder CUnprovenDriverOwner_00810970::AppendTrackedThing.
// Callees: ??0CBaseIntelligentPointer@@QAE@XZ 0x00A01B10, ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z 0x00A01B90,
//   ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (the temporary's dtor), vector<CIntelligentPointer<CThing> >::push_back 0x004AD520
//   (out of line, FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE); the temporary's vptr is 0x01238C6C.
// Layout: no engine header for CTCDExplosiveTrail: a local class over the shared CTCBase / CIntelligentPointer / STLport headers,
//   Ego_r.pdb member names at the retail offsets (as 0x008100A0).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCBase.h"
#include "stlport/fable_containers.h"

class CExplosiveTrailDef;

class CTCDExplosiveTrail : public CTCBase {
public:
    void AddThingToIgnore(CThing& thing);
    CIntelligentPointer<const CThing> PTarget;                                           // +0x0C
    long Timer;                                                                         // +0x14
    bool Terminated;                                                                    // +0x18
    std::list<std::pair<CIntelligentPointer<CThing>, long> > ComponentTraps;            // +0x1C
    std::vector<CIntelligentPointer<CThing> > ThingsToIgnore;                           // +0x20
    CDefPointer<const CExplosiveTrailDef> PDef;                                         // +0x2C
};

void CTCDExplosiveTrail::AddThingToIgnore(CThing& thing)
{
    ThingsToIgnore.push_back(CIntelligentPointer<CThing>(&thing));
}
