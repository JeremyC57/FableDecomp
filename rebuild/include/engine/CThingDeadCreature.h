#ifndef FABLE_ENGINE_CTHINGDEADCREATURE_H
#define FABLE_ENGINE_CTHINGDEADCREATURE_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// PAnimFilter, PKiller, BodyDieTimer, BodyReorienter, StartedDecay; native allocation527300.
class CThingDeadCreature:public CThingGameObject {
protected:
    unsigned char PAnimFilter[8]; // +d0 CCountedPointer<CAnimFilterBase>, ctor835c55/835c5b
    unsigned char PKiller[8]; // +d8 CIntelligentPointer<const CThingCreatureBase>, ctor835c4d/835c66
    long BodyDieTimer; // +e0, original member between the two known composites
    unsigned char BodyReorienter[4]; // +e4 boost::scoped_ptr<CThingBodyReorienter>, ctor835c6c
    bool StartedDecay; // +e8, ctor835c73
    unsigned char _pad_0xe9[7];
public: static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGDEADCREATURE_PEEKDEFINITION
    // Retail opt-in (agentNF9, row 0x00835B90): Ego_r.pdb ?PeekDefinition@CThingDeadCreature@@IBEABVCThingCreatureDef@@XZ (protected
    // const, returns a reference), out of line in retail at 0x00835B90 (called by OnCreate 0x00835FD0 and WorldUpdate 0x00836260).
    // Declaration only (CThingCreatureDef forward-declared).
protected:
    const class CThingCreatureDef& PeekDefinition() const;               // 0x00835B90
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingDeadCreature)==0xf0);
#endif
