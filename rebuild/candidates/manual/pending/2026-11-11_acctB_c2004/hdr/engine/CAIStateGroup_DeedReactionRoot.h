#ifndef FABLE_ENGINE_CAISTATEGROUP_DEEDREACTIONROOT_H
#define FABLE_ENGINE_CAISTATEGROUP_DEEDREACTIONROOT_H
// New header (chunk 2004, row 0x009609B0 CAIStateGroup_DeedReactionRoot::IsRunnable). Retail opt-in only:
// FABLE_RETAIL_CAISTATEGROUP_DEEDREACTIONROOT (needs FABLE_RETAIL_CAISTATEGROUPBASE and FABLE_RETAIL_CINTELLIGENTPOINTER).
// Class: Fable.exe RTTI .?AVCAIStateGroup_DeedReactionRoot@@ : CAIStateGroupBase (class key V). Members: Ego_r.pdb TPI
// (struct_layouts_egor.tsv, 56 bytes; no STL member, so the PDB offsets are retail's): ThingReactingTo +0x14 (the row's
// `lea ecx,[esi+14h]` SetPItem, 0x00960BAE), CurrentDeedType +0x1C, CurrentDeedHandle +0x20, DeedIsVirtual +0x24 (0x00960BBE),
// LastThingOpinionSourceUID +0x28, LastOpinionSourceVirtualDeedID +0x30 (0x009609D5..0x009609E3), DeliveredReaction +0x34,
// DeliveredSpeech +0x35. Methods: the Ego_r.pdb / FableWin.pdb overrides (all public virtual, UAE / UBE):
// ?InitStateGroup@..@@UAEXABVCWorld@@AAVCThingAICreature@@@Z, ?IsRunnable@..@@UAE_NXZ (0x009609B0), ?IsStillValid@..@@UBE_NXZ,
// ?OnActivate@..@@UAEXXZ, ?Update@..@@UAEXXZ, ?OnReplace@..@@UAEXXZ.
#ifdef FABLE_RETAIL_CAISTATEGROUP_DEEDREACTIONROOT
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/EOpinionDeedType.h"
class CThing;
class CWorld;
class CThingAICreature;
class CAIStateGroup_DeedReactionRoot : public CAIStateGroupBase {
public:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
    virtual bool IsRunnable();                                           // 0x009609B0
    virtual bool IsStillValid() const;                                   // 0x0095FED0
    virtual void OnActivate();                                           // 0x00960070
    virtual void Update();                                               // 0x00960120
    virtual void OnReplace();                                            // 0x00960760
protected:
    CIntelligentPointer<const CThing> ThingReactingTo;                   // +0x14
    EOpinionDeedType CurrentDeedType;                                    // +0x1c
    long CurrentDeedHandle;                                              // +0x20
    bool DeedIsVirtual;                                                  // +0x24
    unsigned __int64 LastThingOpinionSourceUID;                          // +0x28
    long LastOpinionSourceVirtualDeedID;                                 // +0x30
    bool DeliveredReaction;                                              // +0x34
    bool DeliveredSpeech;                                                // +0x35
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_DeedReactionRoot) == 0x38);
#endif
#endif // FABLE_ENGINE_CAISTATEGROUP_DEEDREACTIONROOT_H
