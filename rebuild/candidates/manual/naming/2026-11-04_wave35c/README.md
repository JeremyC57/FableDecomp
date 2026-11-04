# Naming pass wave 35c (2026-11-04): 271 landed rows respelled (spawned-func slots, particle-engine getters, queued fixes)

Input: `ROWS.tsv` (255 template and hand rows) plus the PRIORITY `land0411a..j` / `land0411k2` entries of
`naming/NAME_FIXES_QUEUE.tsv` (`QUEUE.tsv`). One agent built an overlay in scratch (`scripts/`). The coordinator applied it
with `scripts/apply_w35c.py` and re-graded the result from the working tree.

## Result
- **271 respelled**, each now defining its exact PDB symbol:
  - 175 `CSpawnedFunc<T>` / `CSpawnedFunc1<T,P>::SuspendableProcess` (slot 4).
  - 61 hand rows: OnInterrupted members, the NParticleEngine getters, TransferableComponent rows, MurderTwist nested classes, and
    6 queue definitions (`CPlayerGui::UpdateStat` 0x00437AE0, `GFLimit<float,float,float>` 0x0048DAE0, 0x00646B12,
    `CSymbolMap::FindFirstSymbolWithVal` 0x009E5090, `CPSCAttractor::DatabaseSerialise` 0x00B18110, 0x00BBC130).
  - 35 landed callers corrected in place (`CALLERS.tsv`), for example the 8 callers of `CPlayer::GetPPlayerCharacterThing`.
- **Header change:** `engine/CSpawnedFunc.h` and `CSpawnedFunc1.h`. In retail, slot 4 stores `StopRunning = true` after the call;
  the header body left that store out. This changes only the inline slot-4 body. It is not behind a retail macro, so every landed
  includer was re-graded.
- **Re-grade from the working tree** (`logs/log0..3.txt`, `vland.py --recheck-landed`, four splits): 723 rows (the 271 wave rows
  plus the 627 landed sources that include either header; the sets overlap). **723 / 723 MATCH or RELOCATION_MATCH + PASS**
  (176 MATCH, 547 RELOCATION_MATCH).
  - Warnings: 229 SHARED_HEADER_AVAILABLE, 67 RELOC_NAME, 2 RELOC_CONFLICT, 1 RELOC_ALIAS. All of them are on callees or data that
    this wave did not rename: junk catalog labels and placeholder data names. The two conflicts (004a0940 CopyHolySites, 0080fad0
    CIntelligentPointerConstThing::operator=) were already there.
  - The agent's per-family logs are in `logs/agent/`.
- **Bindings:** +20 (`BINDINGS.tsv` ADD lines marked "respelled symbol ... exact PDB symbol"). `bindings_multi_check.py` prints 0.
  - Skipped: placeholder data names (SKIP-candidate), spellings this wave did not change (adding them would freeze stand-in names),
    and the DO NOT ADD pointer spelling of `RenderSingleSpriteTrail`.
  - The 53 RETIRE candidates are left in the table: a stale binding names a symbol nothing emits, and it does no harm.
- `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Not changed (`DEFERRED.tsv`)
- 17 libvorbis/libogg rows were already exact.
- 6 `sub_` rows have no name proven by two sources; DEFERRED gives the single-source leads.
- 00ce0ef0 needs a re-model.
- Queue entries that need new parameter types or class models (binders 00aff0a0 00afe9b0 00b22b40, the 0075b8d0 group,
  0075fa30 00b846e0 00a4dea0 00b82100 00bbe090/00bc04f0) and the callers 00ae0050, 00af4530 and 0047db73 stay queued.
- `NAME_FIXES.tsv` (15) records the respellings this wave found but did not make, for example the junk labels at 0x00AF0A60,
  0x00AF0D80 and 0x00B20B60, and `CVillager` declared as a struct in 00ef49a0.
- `GetRandomSeed` 0x00AEAAD0 still reads its seed through the absolute address 0x013D2E10 until the seed's data symbol is known.
