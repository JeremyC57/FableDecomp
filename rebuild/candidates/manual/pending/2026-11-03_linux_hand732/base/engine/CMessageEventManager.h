#ifndef FABLE_ENGINE_CMESSAGEEVENTMANAGER_H
#define FABLE_ENGINE_CMESSAGEEVENTMANAGER_H
// CMessageEventManager (chunk 728 round 2, row 0x006D0830 CTCWife::GlobalUpdate): CWorld::PMessageEventManager (+0x60). Members
// GlobalUpdate calls, Ego_r.pdb / FableWin.pdb spellings:
//   ??$FindMostRecentMessage@VCMessageEventFilter_IsOfType@@@CMessageEventManager@@QBEPBVCMessageEvent@@ABVCMessageEventFilter_IsOfType@@@Z
//     (member template, retail instance 0x004AB8B0), ?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z 0x00493B5B,
//   ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z 0x00687540.
// CMessageEventFilter_IsOfType: the one-member filter GlobalUpdate builds on the stack (`mov [esp+N],32h ; push &it`), Ego_r.pdb
// FableWin.pdb ??0CMessageEventFilter_IsOfType@@QAE@W4EMessageEventType@@@Z. EMessageEventType has no enumerator names on disk
// (MS forward enum; the callers name the values they use).
#include "rebuild_abi.h"
class C3DVector;
class CThing;
class CMessageEvent;
enum EMessageEventType;
class CMessageEventFilter_IsOfType {
public:
    CMessageEventFilter_IsOfType(EMessageEventType type) : Type(type) { }
private:
    EMessageEventType Type;                                              // +0x00
};
class CMessageEventManager {
public:
    template<class Filter> const CMessageEvent* FindMostRecentMessage(const Filter& filter) const;
    void RemoveMessage(const CMessageEvent* message);                    // 0x00493B5B
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long lifespan, const C3DVector& pos, CThing* originator, CThing* target,
                              float sightRadius, float hearingRadius, long extraData);   // 0x00687540
};
#endif
