# nf355 u1: ~CThingNoiseDef (0x0045C5F6), its deleting destructor's call (0x0045C5DA) and CTCKraken::EndAttackBeam (0x007FC800), one unit

Agent NF355 (Account A naming queue; hand-back `pending/2026-11-24_nf355_handback/`). Before: 0x0045C5F6 was a local CTCKraken stand-in
that strongly defined `?EndAttackBeam@CTCKraken@@QAEXXZ`, which is 0x007FC800's name; 0x007FC800 was a free fastcall stand-in. After:
- **0x0045C5F6** is `??1CThingNoiseDef@@UAE@XZ` (Ego_r.pdb + FableWin.pdb; slot 0 ??_G 0x0045C5DA calls it), the compiler-generated
  destructor, emitted the way retail's def TU emits it: the row is `CThingNoiseDef::Alloc` (retail 0x0045C5C1, Ego_r definition_manager.obj)
  under `#pragma optimize("s")`, graded on the `??1` COMDAT it emits (GOTCHAS agentFAM13; landed precedent 0x004317D0
  CUIMiscThingsDef::Alloc graded on its ??1). File renamed CTCKraken_EndAttackBeam_0045c5f6 -> CThingNoiseDef_CThingNoiseDef_0045c5f6.
- **0x0045C5DA** calls `??1CThingNoiseDef@@UAE@XZ` (its own self name stays queued: T3 nf355 0x0045C5C1).
- **0x007FC800** is `?EndAttackBeam@CTCKraken@@QAEXXZ` and calls `?KillBeamEffect@CTCKraken@@AAEXXZ` / `?KillTentacles@CTCKraken@@AAEXXZ`.

Headers: new `engine/CThingNoiseDef.h` (promoted from `_quarantine`: the quarantine was donor 0xA8 v retail 0xA4, and the header asserts the
retail 0xA4; class model under FABLE_RETAIL_CTHINGNOISEDEF_VIRTUAL_MODEL; INDEX HAND_RETAIL), new `engine/CThingComponentSet.h` (the repo
generator's text, byte-identical, plus one declaration opt-in), opt-ins in `engine/CThingBaseDef.h` (_COMPONENTS, _CTOR_DECL) and
`engine/CTCNoiseDef.h` (_IMPLICIT_DTOR). Every replaced line survives verbatim in the default branch. Coordinator whole-catalog
equiv_check (logs/equiv_coordinator_*.txt): 57,155 rows, 0 differing; a plant in all four headers is seen in 526 rows.

Rulings (advisor decision, owner delegated, 2026-11-24), each checked on A's own objects:
- **R1 (strong Alloc in the row): yes.** Alloc's bytes and relocation targets equal retail 0x0045C5C1 (operator new 0x00BFEA1A,
  ctor 0x00455EF0); 0x0045C5C1 has no catalog row, claim, reservation or lock, so nothing landed is duplicated. Not the 0x0070D940 case.
- **R2 (new LNK2005 on ??0CThingNoiseDef): yes, nf184 pattern.** 0x00455EF0's strong copy (selection 1) and this object's pick-any copy
  (selection 2) are byte-identical; the one renamed relocation (??0CMemberBase_0044c0c0 v ??0CSubDefClassBase) binds 0x0044C0C0 under both
  names. The vftables: 0x00455EF0 emits one-slot ??_7CThingNoiseDef / ??_7CTCNoiseDef (selection 2, 4 bytes) and this object the full ones
  (selection 2, 100 bytes); the linker keeps the first it meets (0x00455EF0's, HEAD debt). Ports 0x00455EF0 / 0x0044C1BB queued PRIORITY.
- **R3 (engine/CDefPointeeBase.h included above the size pragma): yes, as scaffolding** (`// Pick-any:` note in the row). Owner 0x0042D9A0
  has no optimize pragma and no CompilerFlags entry; the copy equals retail 0x0042D9A0 directly (logs/retcmp_repo_object.txt). GOTCHAS:
  the pick-any checkers do not follow vftables.
- **R4 (headers): yes** on the closure check above.

Verification from the repo after landing (logs/recheck_repo_with_binders.log): the three rows and the kraken binders 0x00923F60,
0x009243B0, 0x00924520, 0x00924930 grade RELOCATION_MATCH PASS; both checkers with `--leaf` show no NEQ copy in any of the seven; each binder
references exactly `?EndAttackBeam@CTCKraken@@QAEXXZ`, which only 0x007FC800 defines. Every function the 0x0045C5F6 object emits that has a
retail home (Alloc, ??1 / ??0 / ??_G CThingNoiseDef, ??1CThingBaseDef, ??0 / ??_G CTCNoiseDef, ??_GCDefPointeeBase) equals retail in bytes and
relocation targets (logs/retcmp_repo_object.txt; the checkers read 4 of them EQ and the rest NOADDR for want of table lines). The four
11-byte base destructors it also emits have no retail address and nothing references them. The table carries one name each for
0x0045C5F6 and 0x007FC800. T3: nf348 0x007FC800 and nf302 0x007FB1C0 / 0x007FB870 closed; nf334 0x0045C5F6 restored with what stays open.
NAME_FIXES: 7 lines closed (NAME_FIXES_CLOSED.tsv), NF355's 6 queued.
