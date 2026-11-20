# Landing nf158 (Account A, 2026-11-20): particle emitter manager and counted-pointer names (21 rows)

Agent NF158 (round 10, finished in a resume session) handed back 31 entries (6 done, 5 done-already, 13 blocked, 1 rejected, 6 open,
not started); its files are in `naming/2026-11-20_agentNF158/`.

- 21 rows. Every caller of NParticleEngine::GetParticleEmitterManager (0x00AEAAF0) now binds the PDB spelling (7 spellings shrink
  to 1); Create2DEmitter and CreateInterpolatedEmitter (0x00AEB790 / 0x00AEB850 / 0x00AEB8C0 / 0x00AEB6D0) move into
  NParticleEngine as one family; the CCountedPointer<NInventory::CPerItemData> destructor (0x005C04BF) is bound from the shared
  header (5 spellings shrink to 2); 0x00AF1860 is the explicit-instantiation Reset of CCountedPointer<CParticleEmitter>.
  0x00B0CA90 and 0x00B0F990 land together: the header inline ~CPSCUpdateNormal is pick-any in both, so neither defines it strongly.
- Self renames (4): 0x0088F4A0 CGameScriptInterface::EnableDecals, 0x00AEB850 / 0x00AEB8C0 CreateInterpolatedEmitter,
  0x00AF1860 Reset.
- Headers (add-only opt-ins, `orig` equals HEAD): engine/CParticleEmitterManager.h (INTERPOLATED_ONLY, SETENABLEDECALS),
  CPSCUpdateNormal.h (DTOR_INLINE), CPSCBase.h (two IMPLICIT_DTOR), CTCInventoryWeapons.h (REMOVEALLCONFISCATEDITEMS).
- Advisor decision (owner delegated): 0x006E11F0 reaches the header CreateInterpolatedEmitter through casts of its stand-in vector /
  set and of the emitter result, marked `// Fidelity: cast, not retail`; the port is queued. Its catalog CompilerFlags now carry the
  row's `// Flags: /O2 /Oy /W3`.
- Recheck (`logs/recheck.log`): 21 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 104 good, 0 bad. tablecheck: 0x0048DA62 and 0x004C9C10 show a second spelling. Both come from row text unchanged
  since HEAD (0x00645CE9, 0x006E13E0) whose binding was missing from the HEAD table. Both are queued.
- Link (`logs/link_landing.log`): 1 LNK2005, ??0CRightHandedSet, the same at HEAD (`logs/link_control_head.log`).
- Queued: the 0x006E11F0 port; an engine/CInstance.h from the PDB layout (q5181-q5186); the CCountedPointer.h Reset header item
  (q5208); one canonical CSoundEvent header (q4894, q5053); the two exposed splits.
