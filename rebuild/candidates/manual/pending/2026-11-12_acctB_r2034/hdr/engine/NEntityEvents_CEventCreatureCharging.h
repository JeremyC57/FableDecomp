#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTCREATURECHARGING_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTCREATURECHARGING_H
// NEntityEvents::CEventCreatureCharging (new, chunk 2034, row 0x0097BDA0 CAIStateGroup_ScorpionKing::UpdateCombat): RTTI
// .?AVCEventCreatureCharging@NEntityEvents@@ (vtable 0x012989D4) : NEntityEvents::CEventBase, no members of its own (Ego_r.pdb /
// FableWin.pdb list none; retail `push 8 ; call operator new`, 0x0097C2F6). Names: Ego_r.pdb / FableWin.pdb
// ??0CEventCreatureCharging@NEntityEvents@@QAE@XZ (inline: retail expands it after the null test, `mov [eax+4],0 ; mov [eax],012989D4h`,
// 0x0097C304, the CEventBase vtable store dead), ??1CEventCreatureCharging@NEntityEvents@@UAE@XZ and
// ?GetType@CEventCreatureCharging@NEntityEvents@@UBE?AW4EEventType@2@XZ. CEventBase (Ego_r.pdb: vptr +0, WorldFrame +4) with the inline
// ??0CEventBase@NEntityEvents@@QAE@XZ (FableWin.pdb) clearing WorldFrame (the same `mov [esi+4],0` in the out-of-line
// CEventTriggered ctor 0x0083CF29); slots 0..4 as engine/NEntityEvents_CEventExpressionPerformedTo.h (chunk 2015) declares them.
#include "rebuild_abi.h"
class CTCEntityEvents;
namespace NEntityEvents {
enum EEventType;                                                         // MS forward enum
#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTBASE_DEFINED
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTBASE_DEFINED
class CEventBase {
public:
    CEventBase() : WorldFrame(0) { }
    virtual ~CEventBase();                                               // slot 0
    virtual EEventType GetType() const;                                  // slot 1
    virtual void OnAdd(const CTCEntityEvents& events);                   // slot 2
    virtual long GetLifeTime() const;                                    // slot 3
    virtual bool IsReadyToDie() const;                                   // slot 4
    long WorldFrame;                                                     // +0x04
};
#endif
class CEventCreatureCharging : public CEventBase {
public:
    CEventCreatureCharging() { }
    virtual ~CEventCreatureCharging();
    virtual EEventType GetType() const;
};
} // namespace NEntityEvents
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventCreatureCharging) == 8);
#endif // FABLE_ENGINE_NENTITYEVENTS_CEVENTCREATURECHARGING_H
