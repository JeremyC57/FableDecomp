#ifndef FABLE_ENGINE_CTHINGAICREATUREINIT_H
#define FABLE_ENGINE_CTHINGAICREATUREINIT_H
// CThingAICreatureInit (new shared header, chunk 1025, row 0x007C0AD0 NThingShot::CreateShot): the Ego_r.pdb record
// (struct_layouts_egor.tsv, 28 bytes) with the PDB constructor ??0CThingAICreatureInit@@QAE@PAVCThingPhysical@@0_NABVCCharString@@21@Z,
// out of line in retail at 0x004FACD0 (landed). The landed CTCSummonSpell::CreateMinion 0x0079ACC0 keeps a local model with the
// body (that TU expands it); a TU that defines FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY gets the same body in the class.
// The implicit destructor destroys OverridingBrainName then ScriptName (0x007C0CAA / 0x007C0CB5).
#include "rebuild_abi.h"
#include "fable_string.h"
class CThingPhysical;
class CThingBuilding;
class CThingAICreatureInit {
public:
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString ScriptName;                             // +0x08
    CCharString OverridingBrainName;                    // +0x0c
    bool PlayerCharacter;                               // +0x10
    bool Drawable;                                      // +0x11
    bool PlaceAtEndOfList;                              // +0x12
    bool EnableCreatureAutoPlacing;                     // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
#ifdef FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
    CThingAICreatureInit(CThingPhysical* original = 0, CThingPhysical* previous = 0, bool playerCharacter = false,
                         const CCharString& scriptName = "", const CCharString& overridingBrainName = "", bool drawable = true)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName), OverridingBrainName(overridingBrainName),
          PlayerCharacter(playerCharacter), Drawable(drawable), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
#else
    CThingAICreatureInit(CThingPhysical* original = 0, CThingPhysical* previous = 0, bool playerCharacter = false,
                         const CCharString& scriptName = "", const CCharString& overridingBrainName = "", bool drawable = true);   // 0x004FACD0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingAICreatureInit) == 0x1c);
#endif // FABLE_ENGINE_CTHINGAICREATUREINIT_H
