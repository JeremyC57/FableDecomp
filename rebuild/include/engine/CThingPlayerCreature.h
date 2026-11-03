#ifndef FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#define FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#include "engine/CThingCreatureBase.h"
// Original sole base CThingCreatureBase; native Create006ac91c allocates208.
// Complete original tail ControlPos through MovementBandType is unused here.
class CThingPlayerCreature:public CThingCreatureBase {
private: unsigned char _pad_0x160[0xa8];
public: CThingPlayerCreature();
#ifdef FABLE_RETAIL_CTHINGPLAYERCREATURE
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingPlayerCreature@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature)==0x208);
#if defined(FABLE_RETAIL_PLAYER) && defined(FABLE_RETAIL_CINTELLIGENTPOINTER) && defined(FABLE_ENGINE_CPLAYER_H)
// Retail opt-in (chunk 728): the inline FableWin-only CPlayer::GetPPlayerCharacter declared in engine/CPlayer.h (PlayerCharacter +0x34).
inline CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return PlayerCharacter.Get(); }
#endif
#endif
