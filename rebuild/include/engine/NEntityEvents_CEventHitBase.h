#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBASE_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBASE_H
// NEntityEvents::CEventHitBase (new, chunk 767): RTTI .?AVCEventHitBase@NEntityEvents@@, vtable 0x01272344 (slot 3 GetLifeTime
// 0x00810E50, landed). Ego_r.pdb struct_layouts_egor.tsv: CEventBase { vptr +0, WorldFrame +4 } (8 bytes), CEventHitBase adds
// CCountedPointer<CHitParameters> PHitParams at +8 (16 bytes). Only what rows use is modelled; no virtual overrides beyond the dtor.
// Name set (Ego_r.pdb, donor order IsFlourish .. GetHitParams .. GetPHitBy / GetPWeaponUsed = retail 0x00810FC0 / 0x00810FD0):
//   ?GetPHitBy@CEventHitBase@NEntityEvents@@QBEPAVCThing@@XZ, ?GetPWeaponUsed@CEventHitBase@NEntityEvents@@QBEPAVCThing@@XZ.
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
#include "engine/CHitParameters.h"
class CThing;
#ifdef FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS
class CTCEntityEvents;
#endif
namespace NEntityEvents {
#ifdef FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
// agentFAM05 (2026-11-11): the event type GetType returns (mangled W4EEventType@NEntityEvents@@; the enumerators are not in the name sets, so the
// rows write the retail values as casts).
enum EEventType { };
#endif // FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
class CEventBase {
public:
    virtual ~CEventBase();                               // slot 0
    long WorldFrame;                                     // +0x04
#ifdef FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS
    // agentFAM03 fix14 (2026-11-11): retail virtuals of CEventBase, declaration only (no layout change; the vtable exists). Slots
    // from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings from the Ego_r / FableWin name sets.
    virtual void OnAdd(const ::CTCEntityEvents&);        // slot 2, 0x0057A4FC
    virtual bool IsReadyToDie() const;                   // slot 4, 0x0057A4FF
#endif
#ifdef FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
// agentFAM05 (2026-11-11): retail virtuals of CEventBase the declarations above lack, declaration only (no layout change; the vtable exists). Slots
// from the TLC vtable 0x0124F2C4 (agentFAM03 proof, re-verified by agentFAM05): slot 1 is _purecall (each event's GetType),
// slot 3 0x00810E60; spellings from the Ego_r / FableWin name sets.
    virtual EEventType GetType() const = 0;              // slot 1, _purecall
    virtual long GetLifeTime() const;                    // slot 3, 0x00810E60
#endif // FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
};
class CEventHitBase : public CEventBase {
public:
    virtual ~CEventHitBase();                            // slot 0
    CThing* GetPHitBy() const;                           // 0x00810FC0 PHitParams->PThingHitting
    CThing* GetPWeaponUsed() const;                      // 0x00810FD0 PHitParams->PWeapon
#ifdef FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
// agentFAM05 (2026-11-11): the retail virtual of CEventHitBase this declaration lacks, declaration only (no layout change). Slot 5 of the TLC vtable
// 0x01272344 (agentFAM03 proof, re-verified by agentFAM05); spelling from the Ego_r / FableWin name sets.
    virtual bool IsFlourish() const;                     // slot 5, 0x00810EF0
#endif // FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
    CCountedPointer<CHitParameters> PHitParams;          // +0x08
};
} // namespace NEntityEvents
#endif // FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBASE_H
