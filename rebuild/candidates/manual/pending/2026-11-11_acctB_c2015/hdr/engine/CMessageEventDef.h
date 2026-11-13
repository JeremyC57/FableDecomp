#ifndef FABLE_ENGINE_CMESSAGEEVENTDEF_H
#define FABLE_ENGINE_CMESSAGEEVENTDEF_H
// CMessageEventDef (new, chunk 2015, row 0x0097FEA0). RTTI: the def class derives CParentDefClassBase (landed ctor 0x004556B8, vtable
// 0x012334D4). Members: Ego_r.pdb struct_layouts_egor.tsv MaxSeenRadius / MaxHeardRadius / Lifespan / ReactionSpeechType / Type (all
// 4 bytes, Ego_r +0x40..+0x50); the retail CParentDefClassBase is 0x39 bytes packed, so the first member sits at +0x3C: retail
// CMessageEventManager::AddMessage(const CMessageEventDef&, ...) 0x00687730 reads +0x40 and +0x3C as radii (fild) and copies +0x44 as
// the lifespan, and AIAction_FindAGuardAndYellForHelp scales +0x44 by 0.8 (0x0097FFB9). Needs FABLE_RETAIL_DEF_CLASS_BASE.
// StatGetTypeName is the FableWin.pdb static spelling every def class carries (only named here for the GetDef body).
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
enum EReactionSpeechType;                                                // MS forward enum
enum EMessageEventType;
class CMessageEventDef : public CParentDefClassBase {
public:
    long                MaxSeenRadius;                                   // +0x3c
    long                MaxHeardRadius;                                  // +0x40
    long                Lifespan;                                        // +0x44
    EReactionSpeechType ReactionSpeechType;                              // +0x48
    EMessageEventType   Type;                                            // +0x4c
};
#endif
