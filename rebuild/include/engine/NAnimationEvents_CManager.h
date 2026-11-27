#ifndef FABLE_ENGINE_NANIMATIONEVENTS_CMANAGER_H
#define FABLE_ENGINE_NANIMATIONEVENTS_CMANAGER_H
// NEW shared header (AUTH22, 2026-11-22; first user row 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents).
// NAnimationEvents::CManager, Ego_d.pdb TPI: a polymorphic class (vfptr +0x00) with the private members MeshDataBank +0x04 and
// EventSymbolMap +0x08 (CSymbolMap), then AnimToExtraEventsMap, DifferentToSavedCopy, UseEmbeddedEvents (not modelled: their retail
// offsets are unproven; the debug class is 88 bytes with a Dinkumware map). Only what the first user needs:
//  - ?GetEventSymbolMap@CManager@NAnimationEvents@@QBEABVCSymbolMap@@XZ (FableWin.pdb), inline in retail (0x0066B73D `add ecx,8`);
//  - ?GetAnimEventsBetweenTimes@CManager@NAnimationEvents@@QBE_NKAAV?$CSmallVector@VCEvent@NAnimationEvents@@$0CA@@@MM@Z
//    (Ego_r.pdb / FableWin.pdb), out of line in retail at 0x006F99B0;
//  - the free ?GetSoundEventManager@NAnimationEvents@@YIAAVCManager@1@XZ (Ego_r 0x0071E740), retail 0x006F90C0.
// The vfptr is modelled as a data word (no virtual is called through this header yet). Needs FABLE_RETAIL_CSYMBOLMAP for the member's type.
#include "rebuild_abi.h"
#include "engine/CSymbolMap.h"
#include "engine/CSmallVector.h"
#include "engine/NAnimationEvents_CEvent.h"
#ifdef FABLE_RETAIL_ANIMATION_EVENTS_LOAD_PASS29
#include "engine/CVectorMap.h"
#endif
class CMeshDataBank;
namespace NAnimationEvents {
class CManager {
public:
#ifdef FABLE_RETAIL_UPDATE_ACTION_ANIMATION_PASS29
    bool AnimHasEvent(unsigned long,const CCharString&) const;
    float GetEventTimeInSeconds(unsigned long,const CCharString&) const;
    void GetEventNames(unsigned long,std::vector<CCharString>&) const;
#endif
    const CSymbolMap& GetEventSymbolMap() const { return EventSymbolMap; }
#ifdef FABLE_RETAIL_ANIMATION_EVENTS_LOAD_PASS29
    bool Load(const CWideString& animation_events_save_path);
    void AddEventToAnim(unsigned long,const CEvent&);
    bool EventExists(long,const CEvent&) const;
#endif
    bool GetAnimEventsBetweenTimes(unsigned long anim, CSmallVector<CEvent, 32>& events, float start_time, float end_time) const;   // 0x006F99B0
private:
    void*                __vfptr;                                        // +0x00
    const CMeshDataBank* MeshDataBank;                                   // +0x04
    CSymbolMap           EventSymbolMap;                                 // +0x08
#ifdef FABLE_RETAIL_ANIMATION_EVENTS_LOAD_PASS29
    CVectorMap<unsigned long,CSmallVector<CEvent,32> > AnimToExtraEventsMap;
    bool DifferentToSavedCopy;
    bool UseEmbeddedEvents;
    void LoadBinary(const CWideString&);
    void SaveBinary(const CWideString&);
#endif
};
#ifdef FABLE_RETAIL_UPDATE_ACTION_ANIMATION_PASS29
CManager& FABLE_FASTCALL GetGameEventManager();
#endif
CManager& FABLE_FASTCALL GetSoundEventManager();                         // 0x006F90C0
}
#endif // FABLE_ENGINE_NANIMATIONEVENTS_CMANAGER_H
