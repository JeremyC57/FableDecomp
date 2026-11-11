#ifndef FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONFOLLOWCREATURE_H
#define FABLE_ENGINE_NTCSCRIPTEDCONTROL_CACTIONFOLLOWCREATURE_H
// New header (chunk Q15, row 0x00713D50). NTCScriptedControl::CActionFollowCreature : CTCScriptedControl::CActionBase, Ego_r.pdb
// (sizeof 0x28): PTargetThing (CScriptThing) +0x0C, PTCOldPhysics +0x18, Sneaking +0x1C, FollowRange +0x20, AddedStealthTC +0x24,
// AvoidDynamicObstacles +0x25, PreviousPushableByHero +0x26, Running +0x27 (no containers: retail = PDB). Members from the Ego_r.pdb
// name set: ctor (CThing&, float, bool) 0x0092BC10 (Ego_r), Update 0x00713EE0, OnActivate 0x00715710, OnCleanup, GetActionName,
// private SetAsSneaking 0x00713D50. Includer contract: FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE.
#include "engine/CTCScriptedControl.h"
#include "engine/CScriptThing.h"
namespace NTCScriptedControl {
class CActionFollowCreature : public CTCScriptedControl::CActionBase {
public:
    CActionFollowCreature(CThing& target, float follow_range, bool avoid_dynamic_obstacles);
    virtual void Update();                                               // 0x00713EE0
    virtual void OnActivate();                                           // 0x00715710
    virtual CCharString GetActionName() const;
    virtual void OnCleanup();
private:
    void SetAsSneaking(bool sneaking);                                   // 0x00713D50
    CScriptThing PTargetThing;                                           // +0x0c
    CTCBase*     PTCOldPhysics;                                          // +0x18
    bool         Sneaking;                                               // +0x1c
    float        FollowRange;                                            // +0x20
    bool         AddedStealthTC;                                         // +0x24
    bool         AvoidDynamicObstacles;                                  // +0x25
    bool         PreviousPushableByHero;                                 // +0x26
    bool         Running;                                                // +0x27
};
}
FABLE_STATIC_ASSERT(sizeof(NTCScriptedControl::CActionFollowCreature) == 0x28);
#endif
