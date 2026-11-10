#ifndef FABLE_ENGINE_CMESSAGEEVENT_H
#define FABLE_ENGINE_CMESSAGEEVENT_H
// New (chunk 1016, row 0x00934E50 CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<...IsNot...In2DRange...> >): the AI
// message event, Ego_r.pdb CMessageEvent (88 bytes; struct_layouts_egor.tsv): a CBaseObject (vptr +0, IntelligentPointer +4) with
// Type +0x08, Pos +0x0C, SightRadius +0x18, HearingRadius +0x1C, OriginatorUID +0x20, TargetUID +0x28, PTargetFaction +0x30,
// TimeStamp +0x34, Lifespan +0x38, ExtraData +0x3C, MessageID +0x40, ReactionSpeechType +0x44, POriginator +0x48, PTarget +0x50 (the
// landed filter bodies read Type +0x08 (0x006AF1C4) and MessageID +0x40 (0x006AF280); the row reads POriginator +0x48).
// FableWin.pdb ??$GetPOriginator@VCThingAICreature@@@CMessageEvent@@QBEPAVCThingAICreature@@XZ (also <CThing>, <CThingCreatureBase>):
// inline in retail -- the originator's GetPItem (0x00A01B50) null test, its type-flag test against T::StatGetTypeFlags(), then
// GetPItem again for the result (0x00934E88..0x00934EA5). Needs FABLE_RETAIL_BASE_OBJECT / FABLE_RETAIL_CINTELLIGENTPOINTER and
// complete T for a GetPOriginator<T> instantiation. Opt-in: FABLE_RETAIL_CMESSAGEEVENT (the default path is a forward declaration).
#ifdef FABLE_RETAIL_CMESSAGEEVENT
#include "rebuild_abi.h"
#include "engine/CBaseObject.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
class CFaction;
enum EMessageEventType;                                                  // MS forward enum
enum EReactionSpeechType;                                                // MS forward enum
class CMessageEvent : public CBaseObject {
public:
#ifdef FABLE_RETAIL_CMESSAGEEVENT_TARGET_ORIGINAL
    // Original FableWin02112190 and module821 original locals.
    template<class T> T* GetPTarget() const {
        T* ret=0;
        if(PTarget!=0){
            unsigned long things_flags=PTarget->GetTypeFlags();
            unsigned long cast_to_flags=T::StatGetTypeFlags();
            unsigned long xored=things_flags^cast_to_flags;
            if(!(xored&cast_to_flags))ret=GFDebugPDynamicCast(PTarget.Get(),(T*)0);
        }
        return ret;
    }
#endif
    template<class T> T* GetPOriginator() const {
        if (POriginator.GetPItem() && (~static_cast<CThing*>(POriginator.GetPItem())->GetTypeFlags() & T::StatGetTypeFlags()) == 0)
            return static_cast<T*>(static_cast<CThing*>(POriginator.GetPItem()));
        return 0;
    }
    EMessageEventType Type;                                              // +0x08
    C3DVector Pos;                                                       // +0x0c
    float SightRadius;                                                   // +0x18
    float HearingRadius;                                                 // +0x1c
    unsigned __int64 OriginatorUID;                                      // +0x20
    unsigned __int64 TargetUID;                                          // +0x28
    CFaction* PTargetFaction;                                            // +0x30
    long TimeStamp;                                                      // +0x34
    unsigned long Lifespan;                                              // +0x38
    long ExtraData;                                                      // +0x3c
    unsigned long MessageID;                                             // +0x40
    EReactionSpeechType ReactionSpeechType;                              // +0x44
    CIntelligentPointer<CThing> POriginator;                             // +0x48
    CIntelligentPointer<CThing> PTarget;                                 // +0x50
};
FABLE_STATIC_ASSERT(sizeof(CMessageEvent) == 0x58);
#else
class CMessageEvent;
#endif
#endif // FABLE_ENGINE_CMESSAGEEVENT_H
