# NF453 (2026-11-25): census-verified stale binding lines (speed-up A3)

Repo state: the census started at HEAD dfaf7c8908. While it ran, these landed:
- NF451 and NF452 (ea19acdf65);
- NF454's best-guess registry (e08afb26ee);
- NF455 (e689398abc);
- NF456, 60 rows respelt as units (cd4b0b36ce).

The final state is **HEAD cd4b0b36ce**; `work/base/` holds the reloc_bindings / self_symbols / descriptive_names copied at that HEAD.
- **Drift:** every row whose source sha1 changed after it was compiled was recompiled for the census (12 + 29 + 3 + 56 rows,
  `work/cache_9*.tsv`) and for the evidence (`work/evid_zz_r5.tsv`). The only changed headers were the six CSalRender*.h files.
- **Last check at cd4b0b36ce:** 0 source drift.
  - Every TABLE_CHANGES line is still in the table with the same single witness.
  - No object has UND/DEF of a deleted spelling.
  - The dry runs and tracker runs were redone on this snapshot. The baseline and batch shadows were built back to back, because the
    shadow's symlinked inputs (ghidra_out etc.) move with the repo.
- **No git:** I ran no git. HEAD was read from `.git/refs`. naming_status ran in a shadow tree with a `git` shim that runs nothing (`shim/git`).
- **Disclosure:**
  - census453 / evid453 import repo modules (`verify_and_land`, `reloc_identity`) and call the repo's `clean.py` without `-I`. All
    run from my work dir; the paths are explicit.
  - The repo `rebuild/candidates/manual/naming/status/` files that changed meanwhile carry `Commit: e08afb26ee`. They came from the
    coordinator. My runs print an empty commit and write only under `work/ns_*`.

## Method
1. **Lines** (`tools/prep453.py`):
   - The 1,054 lines of `stale_candidates_v2.tsv` were re-read against the current table; 1,052 still have the same single witness.
   - The two that left the table (NF452) are GONE.
   - Lines are ordered strong, weak_nohdr, weak_hdr.
2. **Full compile census** (`tools/census453.py`, 3 shards):
   - Covers every landed row (57,160), each compiled with VC7.1 from its current repo source with its catalog flags (nf.compile_one, as NF448).
   - Per row it keeps the external DEF/UND symbols and the leaf's (retail target, symbol) relocation pairs.
     For dir32, the object's addend is subtracted.
   - Grades: 50,524 MATCH/RELOCATION_MATCH LEAF; 4,759 BIG (leaf name not found, largest function graded); 1,424 DIFFER;
     451 NOTEXT (toolchain-library stubs); 2 SRC_FAIL.
   - The two SRC_FAIL rows are 0x005DCC00 CAnimationSet::GetRandomAnim and 0x00A7A5E0 CNavQuadTree::InitialiseLines. They reference
     no target of this task and witness no line; their sources name none of the deleted spellings' key identifiers.
3. **Calibration:**
   - On the table lines outside the list, the witness's census pairs reproduce the line in 93% of LEAF cases.
   - The misses are explained by three things:
     - anonymous-namespace hashes (`?A0x<hash>` depends on the build path, so every comparison normalises it);
     - genuinely stale lines not in the list (`batches/NEXT_CANDIDATES.tsv`);
     - data lines with an addend.
   - Neither addend nor anonymous-namespace lines can produce a false delete. The holder rule (below) holds any spelling that any
     object still lists, and holder matching normalises the hash.
4. **Classify** (`tools/classify453.py`) per line (s, t, witness w):
   - **NOT_STALE:** w's leaf still binds s at t.
   - **REWITNESS:** another row with a matching grade binds s at t in its leaf.
   - **DELETE:** all of the following hold:
     - no row binds s at t;
     - no object among all 57,160 has UND or DEF of s (NF448's keep rule, now over the whole catalog);
     - every row whose retail bytes reference t, plus the target row itself, has an object;
     - w binds some other symbol at t.
   - **HOLD_*:** anything else.
   - **SKIP:** a line whose target, witness or new witness is locked by another agent (`released` counts as free), or that is named in a
     pending hand-back (NF450 `STALE_TABLE_00a0d3d0.tsv`; the NF442 file for `??0CDefClassBase`).
   - Retail references are rel32 E8/E9/0F8x or abs32 equal to t; for a non-catalog target, abs32 in [t, t+0x80).
5. **Final evidence** (`tools/evid453.py`, `tools/evcheck453.py`):
   - Each witness and new witness of a DELETE / REWITNESS line was recompiled with `verify_and_land.parity_of`.
   - Its pairs were re-derived with `reloc_identity.check`, the code that builds the table.
   - A DELETE is written only if that re-derivation shows w binding another symbol at t and not s. A REWITNESS is written only if the
     new witness binds s at t with MATCH / RELOCATION_MATCH and no hard error.
   - All 434 cited rows passed: 432 RELOCATION_MATCH; the 2 DIFFER are old witnesses of REWITNESS lines.
   - **Closing check (`batches/nf453_all/EVCHECK.tsv`):** every row whose retail bytes reference a TABLE_CHANGES target (13,337
     rows, any grade) was re-derived the same way. Result: 515/515 deletes have no binder, and 23/23 rewitnesses pass (the new
     witness binds, the old one does not).
   - This check removed **two spurious rewitnesses**:
     - `__TI2?AVCGenericException@@` / 013692f8: the witness 00501d30 is DIFFER(1087v1086), the int3-after-noreturn shape, and still
       binds the spelling;
     - `__v_writestring` / 00c8db80: the census picked the wrong section in 00c8e6e0, a leaf-name substring.
   - It also **held one delete**, `??1CString_00c0e360@@QAE@XZ` / 00c0ba40. Row 01210fb1 names that class, and its leaf pairs could
     not be re-derived.
   - **Residual caveat:** 38 lines have at least one referencing row with no re-derivable pairs (RELOC_UNCHECKED). For those, only the
     external-symbol rule covers them, which cannot see a static symbol. A text check found no other such row naming the key identifier.
6. **Vanish hold** (`tools/vanish453.py`):
   - 16 deletes, at 8 vtable targets 0x012B1E84..0x012B8364, would remove the address from the tracker. Both of its lines are stale,
     and the spelling the witness binds today is not in the table.
   - They are held in `batches/HELD_VANISH.tsv` as proposed `replace` lines (old -> current spelling). They need a ruling.

## Counts (1,054 lines)
| Verdict | strong (404) | weak_nohdr (113) | weak_hdr (537) | all |
|---|---:|---:|---:|---:|
| DELETE in TABLE_CHANGES | 350 | 80 | 85 | 515 |
| REWITNESS in TABLE_CHANGES | 0 | 19 | 4 | 23 |
| held: would vanish (HELD_VANISH) | 14 | 2 | 0 | 16 |
| NOT_STALE (false positive) | 32 | 1 | 430 | 463 |
| HOLD_HELD_ELSEWHERE (UND/DEF in some object, no leaf binds it at t) | 0 | 7 | 10 | 17 |
| HOLD_WITNESS_NO_PAIR_AT_T / HOLD_WITNESS_GRADE / HOLD_UNCHECKED_REF | 1 / 0 / 1 | 0 | 1 / 1 / 0 | 2 / 1 / 1 |
| SKIP_PENDING / SKIP_LOCKED / GONE | 3 / 1 / 2 | 4 / 0 / 0 | 5 / 1 / 0 | 12 / 2 / 2 |

The held and skipped lines are in `batches/HELD_LINES.tsv`; the false positives are in `batches/FALSE_POSITIVES.tsv`.

## False positives
- **weak_hdr is mostly live:** 430 of 537 lines. The witness gets the spelling from a header, as with `??0CDefClassBase`.
- **31 C-symbol / CRT lines are live,** e.g. `__onexit` at 0x00401296, which witness 0x004012BC still binds. The tokeniser strips the
  leading underscores (`onexit` is not the identifier `_onexit`), and CRT spellings come from VC7.1 system headers outside the repo.
- **Libpk / Vorbis `@name@N` fastcall lines are live,** e.g. `@_v_readstring@12`.
- **The full census changed 7 weak_hdr verdicts from DELETE to HOLD.** The partial census (18k rows) did not show them: pick-any
  COMDAT copies in rows that do not reference t. A partial census is not enough.

## Sandbox results (`batches/nf453_*`; shadow trees under work/, base snapshot at cd4b0b36ce)
In LINES.tsv, column `final` marks the 16 vanish lines HELD_VANISH and the three review changes in place.
- **apply_table --dry:** 0 misses in each batch and in the combined `nf453_all`. Combined: `{'delete': 515, 'rewitness': 23}`; multi-target 1 -> 1, new [].
- **descriptive_check:** 2016 lines, 0 FAIL, in every shadow.
- **Tracker** (naming_status on the shadow, HEAD snapshot v batch, full record diff; `TRACKER_EFFECT.tsv` per batch).
  Combined batch:
  - +39 split_fixable -> agree_pdb and +49 split_unknown -> agree_std: **+88 done**.
  - 107 split_unknown -> agree_unverified; 69 split_unknown -> standin (one stand-in spelling left).
  - 117 split_unknown and 18 split_fixable keep their status with fewer spellings; 1 split_pdb_multi has fewer spellings.
  - 3 moves that look worse:
    - 0x004ADFF0 and 0x00C5F51C go agree_unverified -> standin, "shared name (2 addresses)". With the stale spelling gone, the
      same name now agrees at two addresses: an existing duplicate binding comes to light.
    - 0x012B2F60 goes split_fixable -> standin. The stale `??_7CGuiVarTransferFloat@@6B@` line goes, and witness 0x00C467D0
      no longer binds it.
  - Per batch:
    - strong: 21 -> agree_pdb, 33 -> agree_std.
    - weak_nohdr: 5 -> agree_pdb, 9 -> agree_std.
    - weak_hdr: 12 -> agree_pdb, 6 -> agree_std.
- **Rewitness note:** some REWITNESS lines keep a wrong-but-live spelling. For example, `?g_pScriptTimerManager@@3PAUCTimerManager@@A` at
  0x0143E8F8 is bound by 0x00D9E9E0, while the PDB name there is pDSTGame. They are live bindings; fixing them is a respell job.

## Finding for a ruling
- **Many deletes leave no record of the current spelling.** In 208 of the 515 final deletes, the spelling the witness binds today
  at t is in the table nowhere; in 2 more it is bound only at another target. This is an observation; I did not verify the cause.
  Deleting the stale line is correct, but the table then lacks the live binding.
  - Option: turn those deletes into `replace old -> current`. That needs a ruling and its own tracker run.
  - The 16 HELD_VANISH lines are the cases where this matters for the tracker.

## Resume point
- **Next batch:** `batches/NEXT_CANDIDATES.tsv` holds 323 delete candidates and 167 rewitness candidates among table lines
  OUTSIDE the A3 list, from the same full census.
  - They are not evid-confirmed. Run evid453 on their witnesses, then classify and finalize.
- **Census reuse:** `work/cache_*.tsv` is reusable for any table line while sources do not drift (sha1 per row). Recompile rows whose
  sha changed (see the drift step above).

## Locks held (NF453)
455 addresses (`work/locks_held.txt`): every target and new witness of a DELETE / REWITNESS line, including the 8 HELD_VANISH
targets. None released.

## Tools (tools/)
- From NF448, repointed: `nf.py`.
- New: `prep453`, `census453` + `run_census.sh`, `classify453`, `calib453`, `evid453` + `run_evid.sh`, `final453`, `vanish453`,
  `mkshadow.sh` (shadow copies from `work/base`), `track453.sh`, `recdiff453`.

## r453 rebuild (coordinator rulings r453a-e, 2026-11-26; HEAD cd4b0b36ce, table b1924e9f5f)
The v1 batches are kept in `batches/superseded_v1/`. `tools/rebuild453.py` rebuilt the level batches and `nf453_all` from them and
from HELD_VANISH.

- **r453b / r453a:** each delete is decided by the one live spelling its witness binds at t (reloc_identity.check):
  - the live spelling is already in the table at t: **plain delete**;
  - the live spelling is in no table line: **replace stale -> live** (same target, same witness);
  - a second stale line at a target that a replace already covers: **delete**.

  Result: **292 deletes** (280, plus 12 that a replace already covers), **204 replaces** (6 of them from vanish lines), **17 rewitnesses**.
  The 2 "bound only elsewhere" cases (`??1CDspGain@@UAE@XZ` live at 00c69680, bound in the table at 00c5f51c) were already held by
  r453d and are in IDENTITY_FIXES.tsv.
- **r453d:** 41 lines (35 deletes, 6 rewitnesses) whose referencing rows could not all be re-derived are held in HELD_LINES.tsv
  (HOLD_r453d_UNCHECKED_REF). The v1 holds stay held.
- **Checks on the sandbox (shadow built from the cd4b0b36ce table):**
  - apply_table --dry: `{'delete': 292, 'replace': 204, 'rewitness': 17}`, 0 misses; multi-target 1 -> 1, new [].
    Each level batch also has 0 misses.
  - descriptive_check: 2016 lines, 0 FAIL. REGISTRY_REMOVE.tsv is empty.
  - Closing check `nf453_all/EVCHECK.tsv`: 513/513 lines pass, every referencing row re-derived.
- **TRACKER_EFFECT (nf453_all v the HEAD table):**
  - **+72 done:** 29 split_fixable -> agree_pdb, 43 split_unknown -> agree_std.
  - 92 split_unknown -> agree_unverified; 32 split_unknown -> standin; 4 split_unknown -> split_fixable.
  - 75 split_unknown, 16 split_fixable and 1 split_pdb_multi keep their status with fewer spellings.
  - 1 split_fixable -> split_unknown (0x012B2F60, below). No agree -> standin moves remain.
- **r453c:**
  - **0x004ADFF0:** the stale line masked a real exposure. `??1CRetailTree_erase004ad690@@QAE@XZ` is the self symbol of two landed
    rows, 0x004ADFF0 and 0x004AE200 (self_symbols.tsv). Listed in IDENTITY_FIXES.
  - **0x00C5F51C:** this is an exposure too. The table binds `??1CDspGain@@UAE@XZ` at 00c5f51c through funclet row 012165ce
    (`_global::Unwind@012165ce`, graded on its parent's block). 0x00C5F51C is inside CImaAdpcmCodec::EncodeS16, not an entry.
    Retail CDspGain::~CDspGain is the catalog row 0x00C69680, which witnesses 00c6a340 and 00c75980 bind live. The 00c5f51c line
    looks wrong (bad funclet resolution). Listed in IDENTITY_FIXES.
  - **0x012B2F60:** a wrong label. The vtable is not in rebuild/manifest/rtti-vtables.tsv, which has no CGuiVarTransferFloat
    entry at all. Its users are libpk code (0x00C1/0x00C4, no RTTI). Witness 0x00C467D0 binds `??_7CNodeAllocator@@6B@` there today
    (now a replace). Since the PDB spelling was not true, there is no NAME_FIXES line.
- **r453e:** `batches/NAME_FIXES.tsv` has 23 lines in which a live spelling written or kept by this batch differs from a PDB
  name-set spelling at the same target. Examples: `g_pScriptTimerManager` / `g_pGameScriptInterface` vs `pDSTGame` at 0x0143E8F8;
  PAV vs AAV CThing ctor parameters; missing NParticleEngine / NCreatureMode scope.
- **Non-blocking count** (`tools/missing453.py`, census MATCH/RELOCATION_MATCH LEAF rows; anonymous-namespace and literal symbols skipped):
  - 3,746 live (target, symbol) pairs from 4,077 rows are missing from the table, at 3,424 targets.
  - Status of those targets: 801 untracked, 779 split_unknown, 602 agree_pdb, 483 standin, 250 split_fixable, 231 agree_std,
    146 agree_descriptive, 123 agree_unverified, 8 split_pdb_multi, 1 rtti_identified.
  - Adding them would take about **798 targets out of an agree status**: 409 agree_pdb, 137 agree_std, 136 agree_descriptive,
    116 agree_unverified (naming_status.norm comparison). This is approximate: census leaf selection by name is not exact.
- **Locks:** the same 455, still held.
