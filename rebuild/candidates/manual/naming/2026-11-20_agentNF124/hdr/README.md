# NF124 header overlay (round 10). Every change is an `#ifdef` opt-in; with no opt-in defined each header is token-identical to its base.
Bases: orig/<path> = the repo working-tree copy taken at the start of this round (2026-11-20).

## engine/CTCEnemy.h (base: rebuild/include/engine/CTCEnemy.h)
- FABLE_RETAIL_CTCENEMY_ADDALLYTHING: public `void AddAllyThing(const CThing&);` (0x0076D270; Ego_r.pdb 0x0077B360 + FableWin.pdb 0x02606170
  ?AddAllyThing@CTCEnemy@@QAEXABVCThing@@@Z). Declaration only.
- FABLE_RETAIL_CTCENEMY_REMOVEALLYTHING: public `void RemoveAllyThing(const CThing&);` (0x0076C850; Ego_r.pdb 0x0077B3E0 + FableWin.pdb
  0x02606200 ?RemoveAllyThing@CTCEnemy@@QAEXABVCThing@@@Z). Declaration only.
- Both sit after `SetFaction` (public section), before `protected:`. No layout or vtable change.
- pin_asserts.py pinned 1: `FABLE_STATIC_ASSERT(sizeof(CTCEnemy)==28)` -> `FABLE_STATIC_ASSERT_AT(59, ...)` (its base line).
- Used by: rows/0089c100.cpp. Landed includers of engine/CTCEnemy.h (27 files in rebuild/src/compiled) define neither macro: unchanged.
- Neither NF122 nor NF123 overlays this header.

## engine/CTCCreatureModeManager.h (base: rebuild/include/engine/CTCCreatureModeManager.h, the working tree with NF120's REMOVEALLMODES)
- FABLE_RETAIL_CMODEBASE_GETNOFRAMESBEFOREPLAYINGCOMBINATIONIDLEANIMS: in CModeBase, `protected:` + `long GetNoFramesBeforePlayingCombinationIdleAnims(
  NCreatureMode::EMode);` (0x0073A6F0; Ego_r.pdb 0x00751090 + FableWin.pdb 0x024C3200, IAE; the landed self row defines this spelling). Needs
  engine/NCreatureMode.h (EMode) included first. Placed just before the existing `protected:` member block. Declaration only.
- pin_asserts.py pinned 0 (the one assert was already pinned in the base). Landed includers (159 files in rebuild/src/compiled) do not
  define the macro: unchanged.

## engine/NCreatureMode_CModeSitFloor.h (base: rebuild/include/engine/NCreatureMode_CModeSitFloor.h)
- FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ONACTIVATE: in NCreatureMode::CModeSitBase, `virtual void OnActivate();` (slot-5 override,
  0x008D6580; Ego_r.pdb 0x008178B0 + FableWin.pdb 0x02841C90 ?OnActivate@CModeSitBase@NCreatureMode@@UAEXXZ). Needs
  FABLE_RETAIL_CMODEBASE_VIRTUALS. Placed before the REMOVEMODEWITHACTION block (public). Declaration only: no slot, no layout change.
- pin_asserts.py pinned 0 (already pinned in the base). Landed includers (16 files) do not define the macro: unchanged.

Used by rows/: 0089c100.cpp (CTCEnemy opt-ins), 008d6580.cpp (both CModeBase / CModeSitBase opt-ins), 008d8ea0.cpp (no new opt-in).
NF122 / NF123 overlays (re-listed 2026-11-20 03:28: NF122 CGameTimeManager / CTCDParticleEmitter / CTCRegionFollower / NScreenEffect_CScreenEffectPriorityStack, NF123 LHHttp2 / LHHttpHeaderStatus / LHNetworkTypes / LSocket): neither overlays these three headers. orig/ is byte-equal to the repo working tree for all three.
