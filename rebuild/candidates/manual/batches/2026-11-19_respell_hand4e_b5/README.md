# Respell hand4e b5 (2026-11-19): two corrections to rows landed from hand4e b1-b3

Recheck: 0x006379D0, 0x00AAC5E0 and the caller 0x00638540 are all MATCH / RELOCATION_MATCH + PASS (`logs/recheck_good.txt`). Diffs are in this folder; hand4e's
notes are in CORRECTIONS.md.

1. **0x006379D0 NThingMap::InitLayerInfo is __fastcall**, as both PDBs spell it (?InitLayerInfo@NThingMap@@YIXXZ).
   - The no-argument body cannot show the convention, so the landed row had used cdecl.
   - FABLE_FASTCALL was added to the opt-in declaration (NThingMap_CThingMap.h, FABLE_RETAIL_CTHINGMAP_INITLAYERINFO; only this row defines it) and to the
     definition.
   - The caller 0x00638540 still binds its local-model spelling ?InitLayerInfo@CThingMapManager@NThingMap@@SAXXZ. That is naming debt, recorded in hand4e's
     NAME_FIXES.
2. **0x00AAC5E0 CSoundEmitterLH::GetActiveSampleInfo** moved from a local CSoundManagerLH model onto the shared engine/CSoundManagerLH.h (PManager->SoundSystem).
   - It gains an `// Inline-budget:` line for STLP_VECTOR_PUSHBACK_OUTOFLINE (push_back expands without it, DIFFER(267v136)).
   - The landed CSoundEmitterLH siblings carry the same local model; porting them is open.

Not changed: the MOVESTRINGPOS_EGOD opt-in keeps its inherited FABLE_FORCEINLINE. Plain inline also matches.
