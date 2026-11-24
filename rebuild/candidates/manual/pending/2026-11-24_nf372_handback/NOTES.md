# NF372 hand-back: T1 slice (19 `access` + 150 `standin_alias`), Account A naming queue, 2026-11-24

Folder `$S/w22/agentNF372/` (S = the 40effb6c scratchpad). Lock name NF372. No repo write, no git, no header or test change.

## R1 first (ruling needed)
129 of the 169 targets in `$S/t1.txt` were already reserved in T3_PORT_QUEUE.tsv when I started, in column 2 or a unit column (nf284 / nf285 /
nf286 / nf287 / nf288 / nf289 / nf299 / nf309 / nf333 ...: `evidence/t3_lines_of_targets.tsv`). OPEN.tsv does not consult the queue, and the
prefix lists these addresses as mine. So I re-read every T3 line plus a witness census (tools/tri.py, `work/tri_all.txt`). I acted only where
the line's premise had changed or measured work was already waiting:
- **batches/b1 (3 rows):** the nf284 "deferred-12" DeleteData hybrids NF309 had measured and parked (`pending/2026-11-23_nf309_handback/reverted/unit5/`,
  NF309 Q6). The HEAD sources and tests are unchanged since then; re-measured here.
- **table_only/ (1 line):** the nf285 0x004D5ECA line is stale.
If the T3 targets were not meant for this slice, drop b1 and table_only. The T3 lines that would close are listed below.
Binder rows reserved in their own right (T3 col 2 / unit, P28, held, B, other agents) were not changed or compiled, with these exceptions:
- **0x0085A840 (changed):** it has its own nf309 column-2 line, which says the row belongs to this same unit.
- **Compiled read-only for the link check:** the three deferred-12 selves 0x006174F0 / 0x0085A5A0 / 0x00AC3A10, under NF372 locks.
- **`--release-t3 nf309` needed:** binder 0x0085A840 has its own T3 line (nf309, column 2, "part of the NF284 D4 deferred-12 unit").
  land_scripted holds it without that flag. 0x00618660 and 0x00AC3A40 are not mentioned in the queue.

## Result (STATUS.tsv, 169 lines)
| status | entries |
|---|---:|
| done | 3 (67, 74 in b1; 48 table-only) |
| done-partial | 1 (72: 0x00905FD0, a T3 nf295 row, keeps the stand-in) |
| skipped-held | 148: 125 T3-reserved targets (line re-read, stands; incl. 70 / 75 nested T, NF309 Q5), 13 whose wrong-spelling witnesses are off-limits (incl. 121, 161), 8 libpk / CODA identity (6-11, 43, 108), 28 destructor family, 37 vtable |
| skipped-port | 17 (free targets, binders bind through stand-in classes / scopes / parameter types: QUEUE_PROPOSALS.tsv) |

## batches/b1 (land_scripted run dir, 3 rows, all ACCEPT)
| binder | target | old -> new |
|---|---|---|
| 0x00618660 | 0x006174F0 | ?Destroy_006174f0@@YIXPAX@Z -> ?DeleteData@?$CCountedPointer@V?$vector@_KV?$allocator@_K@std@@@std@@@@SIXPAX@Z |
| 0x0085A840 | 0x0085A5A0 | ?Destroy_0085a5a0@@YIXPAX@Z -> ?DeleteData@?$CCountedPointer@VCCombatActionBase@@@@SIXPAX@Z |
| 0x00AC3A40 | 0x00AC3A10 | ?Destroy_00ac3a10@@YIXPAX@Z -> ?DeleteData@?$CCountedPointer@V?$CArray@VC3DTriangle2@@@@@@SIXPAX@Z |

- **Form:** the landed NF282 hybrid, `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` + `engine/CCountedPointer.h` with T forward-declared. The text is NF309's unit-5 text, byte for byte.
- **Names, two sources each:**
  - the PDB name set: FableWin for vector / CCombatActionBase; Ego_r + FableWin for CArray<C3DTriangle2>;
  - the retail store of the target as the count block's DeleteFunc;
  - the self row's PDB `// Leaf:`.
- **Gates:** per row; logs and evidence are in b1/.
  - Prechecks:
    - clang landed tests before the locks;
    - HEAD + new sources after the locks: all PASS (`work/pre.log`).
  - vland --recheck-landed, HEAD and new under ONE LANDVERIFY_WORK path:
    - HEAD and new both RELOCATION_MATCH PASS;
    - the RELOC_NAME warning on the stand-in is gone in the new build.
  - objsec under the one rename (`evidence/objsec/`):
    - ALL_EQ, with `.drectve` byte-equal;
    - the DEF / UND symbol sets differ only by the rename;
    - the COMDAT sets are the same (2 = 2 text sections).
  - pk_all + pickany_vs_retail:
    - H = N (one NOADDR row-local `SCountBlock` ctor, no NEQ);
    - reachflip: 3 pairs, 0 flips.
  - /FORCE link of the 3 binders + the 3 HEAD selves (`evidence/link_u5.txt`):
    - HEAD leaves the three Destroy_ stand-ins unresolved;
    - the new set resolves DeleteData;
    - 0 duplicates.
  - Census:
    - no landed source, test or header names Destroy_006174f0 / Destroy_00ac3a10 any more;
    - only 0x00905FD0 still names Destroy_0085a5a0. That rests on its source text plus NF309's census; the row is T3 (nf295) and was
      not compiled here;
    - no other landed source defines the three DeleteData specializations;
    - no landed source, test or header `#include`s the three changed sources (folded-TU / funclet check: 0 hits).
- **TABLE_CHANGES (apply_table --dry: 0 misses, no new multi-target):** replace x2, add, plus a **rewitness** of the Destroy_0085a5a0 line onto
  0x00905FD0.
  - land_scripted keeps only lines witnessed by a landed row, so it drops that rewitness: **apply it by hand**.
  - Otherwise the old line keeps a witness that no longer binds it.
- **T3 lines that close:**
  - nf309 0x0085A840 (`--release-t3 nf309`);
  - nf284 deferred-12, for 0x006174F0 / 0x00AC3A10 (and partly 0x0085A5A0). Its column 2 is `deferred-12`, not an address, so
    land_scripted's `--release-t3` cannot close it: drop those three addresses from that line by hand.

## Coordinator hand steps
1. Run land_scripted on batches/b1 with `--release-t3 nf309`, so 0x0085A840 is not held.
2. Apply the `rewitness` line of b1/TABLE_CHANGES.tsv (Destroy_0085a5a0 -> witness 00905fd0) with apply_table.py by hand. land_scripted
   drops it because its witness is not a landed row of the run.
3. Apply table_only/TABLE_CHANGES.tsv (entry 48) with apply_table.py.
4. Edit the nf284 deferred-12 line by hand: remove 0x006174F0 / 0x00AC3A10, and 0x0085A5A0 if you treat the partial as closed. Close the
   nf285 0x004D5ECA line.

## table_only/ (entry 48)
- **The stale line:** `??0CClass_004d5eca@@QAE@XZ 004d5eca 004d92b0`.
- **What the witness binds:** 0x004D92B0's source has no CClass_004d5eca. Its HEAD object (vland RELOCATION_MATCH PASS) binds 0x004D5ECA to
  the PDB `??0CCarriedReadableDef@@QAE@XZ` (Ego_r + FableWin; `evidence/xr_004d92b0_head.txt`).
- **The fix:** one `replace` (apply_table --dry clean; no landed text names the stand-in). T3 nf285 0x004D5ECA can close.

## Skips needing a ruling or a queue line (QUEUE_PROPOSALS.tsv, 28 lines)
- **libpk / CODA identity, entries 6-11, 43, 108.**
  - **The claim:** the tracker's PDB spellings `??0C3DAnimationUtil@@QAE@XZ`, `??0CBaseIntelligentPointer@@QAE@XZ` and
    `??0CEnginePrimitiveOcclusionManager@@QAE@XZ` are propagated labels.
  - **Evidence:**
    - 10 OPEN.tsv addresses (00c3f600 00c596a0 00c3d970 00c55090 00c550b0 00c550f0 00c55110 00c56d30 00c573e0 00c72600) share the
      C3DAnimationUtil spelling, which each PDB name set holds once;
    - all ten addresses sit in the libpk / CODA block (LIBRARY_MAP 0x00C086C0..0x00C89C10: no PDB, no RTTI);
    - the binders construct audio panners and node groups.
  - **Consequence:** respelling the binders would bind one name at many addresses.
  - **Cross-flag for NF370:** its T0 slice (t02.txt) renames 00c3f600 / 00c596a0 onto that same name.
- **37 (0x009D5200):** making OpenFileReadWrite virtual in binder 0x00502170's local model grows the emitted pick-any `??_7CWADBank@@6B@`
  (NF320 Q3). That is a vtable decision. NF176 did the same for Close before NF320.
- **164 (0x00724010):** the binder needs the CTCPhysicsBase / CTCBase class-model switch. That is a model change.
- **28:** destructor family (NF334 D1).
- **121:** an identity question: SetQuestAsFailed -> Rest(1.0f).

## Locks
Claimed 7, all for compiles: 00618660 0085a840 00ac3a40 (changed), 004d92b0 (table witness), 006174f0 0085a5a0 00ac3a10 (link selves).
All are released (`evidence/locks_final.txt`). land_scripted re-locks the rows as COORD.

## Tools (tools/)
| tool | purpose |
|---|---|
| offl.py | stay-off screen: T3 col 2 / unit, P28, B claims / pending, held rows, other agents, live locks |
| tri.py | witness census: stand-in tokens in the witness code, member lines |
| st372.py | STATUS |
| runpair.sh | HEAD / new vland at one path, from a symlinked rsroot |
| mkrun372.py | builds the run dir |
| nf.py | NF366's, retargeted to NF372 |
