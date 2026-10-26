// CTCObstacle::CTCObstacle(CThing& thing) -- CTCBase(thing), then this class's vtable and SteeringAgent = 0
// Leaf: ??0CTCObstacle@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D63FB, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x0123E28C.
// Identity (proven): CTCObstacle::CTCObstacle(CThing&); evidence: it stores vtable 0x0123E28C whose RTTI is .?AVCTCObstacle@@ (: CTCBase : CBaseClassNonCopyable : CBaseClass); FableWin.pdb holds
//   ??0CTCObstacle@@QAE@AAVCThing@@@Z; its only caller is the type factory 0x004D9580.
// Label: the manifest's "CCrimeStatBar" is propagated (family H073: six TC constructors that zero one member, the shape of the landed
//   family G056, e.g. CTCCreatureHive::CTCCreatureHive 0x004D45C7).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Layout: Ego_r.pdb CTCObstacle::SteeringAgent (CSteeringAgent*) at retail +0x0c; CTCBase modelled locally as the landed G056 rows do (engine/CTCBase.h
//   has no constructor), Review at landing.
#pragma optimize("s", on)
class CThing;
class CSteeringAgent;
class CTCBase {                                            // retail 0x0C bytes
public:
    CTCBase(CThing& thing);                                // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CTCObstacle : public CTCBase {
public:
    CTCObstacle(CThing& thing);
    CSteeringAgent* SteeringAgent;                    // +0x0c
};
CTCObstacle::CTCObstacle(CThing& thing)
    : CTCBase(thing), SteeringAgent(0)
{
}