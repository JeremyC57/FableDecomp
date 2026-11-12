#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTKICKED_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTKICKED_H
// New shared header (chunk 2053, row 0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction): NEntityEvents::CEventKicked, the
// 8-byte event the kick posts on the target's CTCEntityEvents (`push 8 ; call operator new` then `mov [eax+4],0 ; mov [eax],01277B5Ch`,
// 0x0087F48C..0x0087F49D). RTTI: vtable 0x01277B5C is .?AVCEventKicked@NEntityEvents@@ (rtti-vtables.tsv: slot 0 the scalar deleting
// destructor 0x0087FEA0, slot 1 GetType 0x0087E760). FableWin.pdb ??0CEventKicked@NEntityEvents@@QAE@XZ, ??1CEventKicked@..@UAE@XZ,
// ?GetType@CEventKicked@..@UBE?AW4EEventType@2@XZ and ??0CEventBase@NEntityEvents@@QAE@XZ (WorldFrame = 0: the +4 store; the base vptr
// store is dead). Not combinable with engine/NEntityEvents_CEventHitBase.h (which declares CEventBase's dtor out of line).
namespace NEntityEvents {
enum EEventType;
class CEventBase {
public:
    CEventBase() : WorldFrame(0) {}
    virtual ~CEventBase() {}
    virtual EEventType GetType() const = 0;
    long WorldFrame;                                                     // +0x04
};
class CEventKicked : public CEventBase {
public:
    CEventKicked() {}
    virtual EEventType GetType() const;                                  // 0x0087E760
};
}
#endif
