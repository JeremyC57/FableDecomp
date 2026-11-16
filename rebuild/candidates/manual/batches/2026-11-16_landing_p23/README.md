# 2026-11-16 landing p23: Codex pass 23 hand-back (103 rows, 55,157 bytes)

Owner: "here is codex's completed work. verify and land the matches and park the best versions for the unmatched. there will
likely be header merge conflicts again. make sure to reade the readme and start here files if they are present".

Source: `FableDecomp_pass23_handoff_2026-11-16.zip`. SHA256SUMS lists 46,396 files; all verified. Read before landing:
START_HERE_CLAUDE.md, STATUS.tsv (91 RELOCATION_MATCH, 9 PARKED), BOUNDS_FIX.tsv, `hdr/PATCHES.json`, per-row ROW.json /
NAMING_EVIDENCE.md / PROBES.md, NAME_FIXES.tsv (406) and LABELS.tsv.

## Headers (first commit, 6333e27417)
191 headers merged onto the current tree. Details in `merge/README.md`. In short:
- 80 unchanged on our side plus 69 new take Codex's text.
- 42 changed on both sides go through diff3: 33 merge clean, 9 conflict (11 hunks), resolved by `merge/resolve.py`.
- Whole-tree equivalence over the 56,461 landed rows: only the 5 known `__FILE__` rows differ.

## Rows (this commit)
All 103 are RELOCATION_MATCH (one MATCH) with their behaviour tests PASS under `LANDVERIFY_STRICT_HEADERS=1`
(`logs/land_phase1.log`, `logs/land_phase2.log`). They land in two phases (`LANDED.tsv`, column `kind`):
- **49 ROW:** the reserved rows themselves. Three of them land at their real start:
  - 0x008F92EF lands as LHNetworkLib::LSocket::Send 0x008F8F50 (970 bytes);
  - 0x00D2988B lands as 0x00D29880;
  - 0x004A1840 carries its catch block 0x004A1C41.
- **17 PARENT:** whole EH parents that Codex proved with a funclet, each landed at its real start with its own header line. The
  source is the parent file from the Codex row folder, or the funclet's own source when Codex wrote the parent there.
- **37 FUNCLET:** `Unwind<addr>` rows (module `_global`, `funclet_offset` 0, `/EHsc`) with no code of their own. Each
  `#include`s its landed parent's file, as 0x01215000 does in landing gb. Five of them belong to parents landed earlier:
  0x00C3DFD0 (Codex's copy is identical to the landed one) and 0x00C2E610.
- **Evidence:** Codex's top-level files and the landed rows' NAMING_EVIDENCE / PROBES / ROW.json / BUILD.json are in
  `pending/2026-11-15_codex_pass23/return/` (as gb did for pass 22). Scanned: no retail byte runs, only sha256 header profiles.

Tools are in `tools/`:
- `mkpayload23.py` builds the payload, oracle and plan. Its first version did not inline `#include "<addr>.cpp"`, so about 25
  rows compiled a missing file and timed out (`logs/dry_run1_uninlined.log`).
- `mkphase2.py` builds the funclet wrappers.
- `nfq.py` splits the name fixes.
- `unmatched23.py` updates UNMATCHED_ROWS.tsv.

The payloads are `payload_phase1.json` and `payload_phase2.json`; the oracle is not kept because it holds retail bytes.

### Six new function starts (`rebuild/manifest/boundary-starts.tsv`)
The manifest had no start for six of the parents, so the lander reported OUTSIDE_MANIFEST. Each has int3 padding before it, the
SEH prologue `64 A1 00000000 6A FF`, and a `ret` at the BOUNDS_FIX length followed by int3. None overlaps a landed neighbour.
`manifest_add_boundaries.py --write` added them to functions.tsv.
- **0x00CA97A0 CDynamicOutputPin::Disconnect:** vtable slot at .rdata 0x012BF774.
- **0x00CAA860 / 0x00CAA8D0 / 0x00CAA9C0 CBaseMediaFilter::SetSyncSource / GetSyncSource / Run.** Codex's BOUNDS_FIX
  shows their funclets were credited to 0x00CA9710 / 0x00CAA790.
- **0x00C319B0 PKSerialiseCParser::ValidateReader.**
- **0x00C4B160 PKCBufferedReader::PollReadCompletion.**

### DirectShow vendor code
The four strmbase parents are unmodified DX9.0c SDK Summer 2004 `BaseClasses/amfilter.cpp`.
- `lib_verify.py` proves each one MATCH from the vendor build.
- `__ehhandler$...` (the 18-byte `.text$x` block that also holds the funclet) is proven too, at 0x01216CE8.
- They land as hand-written rows with a `// Vendor-proof:` line, the same shape as the landed ~CBaseMediaFilter 0x00CAA790
  and its funclet 0x01216DE0.
- They are not body-less `Vendor-library:` stubs (the TimeDiff shape), because a stub has no translation unit for the funclet rows
  to include.
- vendor_signatures.tsv marks the four `landed` (they were `outside`).

## Not landed (best drafts in `pending/2026-11-16_codex_pass23_open/`, listed in UNMATCHED_ROWS.tsv)
- **9 PARKED** (README there): 0x007D00F0, 0x0089C910, 0x012152D0, 0x012117A5 / 0x012117AD, 0x012139DB..0x012139F6. The parents
  0x00C12E50, 0x00C2CBD0 and 0x00C425E0 now point at these drafts (closest on record).
- **0x00C29A3C, with parent 0x00C298D0 and the catch block 0x00C29A30 in its extent:** matches for Codex, but needs VC7.1's
  Dinkumware `<vector>` ahead of the repo STLport. The harness has no per-row include order (pilot: SRC_FAIL, `_Insert_n` is not
  a member).
- **0x01210E95 / 0x00C0AF90 and 0x01212472 / 0x00C1C460:** match for Codex but are naming-blocked. Their classes are
  `CUnprovenAudioChannelBank`, `CUnprovenSoundWorld_00c1c460` and similar placeholders, which CLAUDE.md bars from landed code.

## Bookkeeping
- **Reservation:** `pending/2026-11-15_codex_pass23/RESERVED.tsv` is renamed `RELEASED.tsv`.
- **UNMATCHED_ROWS.tsv:** 100 rows removed: 87 landed reservations, plus 13 rows already listed outside the reservation (11 of
  the parents and the real starts 0x008F8F50 / 0x00D29880). 16 rows changed, plus the three parents above.
- **Labels:** `ghidra_out/labels_landing_2026-11-16p23.tsv` has 66 labels, for rows and parents (funclets take none, as in gb).
- **Name fixes:** 185 queued as `land1116p23` (182 more were already queued with the same spelling). 39 that disagree with a
  queued spelling, a landed leaf or a 2026-11-16gn label are held in `NAME_FIXES_CONFLICTS_WITH_QUEUE.tsv` for the coordinator. All 39 disagree with a queued spelling only; none
  disputes a leaf landed here or a gn label.

## Audit notes
- **RELOC_NAME:** 94 warnings (69 distinct symbols). Codex's spellings differ from current labels; 80 of the 94 bind a symbol the
  name-fix queue already carries.
- **RELOC_CONFLICT (one):** 0x0064E978 binds `??0C3DAnimationBlendState@@QAE@XZ` to 0x004C2DD0, which the manifest names
  C3DAnimationBlendState. The other side is a stale reloc_bindings line from 0x00C72600; that row respelled its callee
  0x00C59BA0 to `??0CCodaVoice@@QAE@XZ` in vendor naming wave V1. The new row is the right one.
- **RELOC_ALIAS (one):** `__CIacos` reaches the MSVCR71 import thunk.
- **Placeholder scan:** the scan of the landed text finds no `Unproven`, `FUN_`, `sub_` or asm. Eight GUI tests
  (0x004A1840, 0x0054EF00, ...) override `Virtual08/11/12/13_<addr>`, which are the pure virtuals engine/CIEngine.h declares
  under those names (header debt since 2026-11-03). That debt is not in the rows.
- **Optional:** 0x00CAA790 is vendor-proved as `??1CBaseMediaFilter@@UAE@XZ`, yet it is still landed as `~CVolume_00caa790`.
  It is older naming debt, left as it is.

## Open: 25 caller-side bindings (`callers_wrong_symbol.tsv`)
A scan of every retail E8/E9 into the 66 new non-funclet addresses (`retail_calls_into_new_rows.tsv`, `tools/callers.py`) finds 67
call sites. 58 of them sit in 49 landed rows, which were compiled with `tools/xrow.sh` (relocations resolved against retail
by `tools/xref.py`). Their 56 resolved relocations into the new rows break down as follows: 31 bind the new rows' leaves.
The other two sites resolve to no relocation of the graded function. **25, in 20 older caller rows, bind another spelling.** These
are const / access / parameter-type differences such as `QAE` v `IAE`, `PAV` v `ABV`, `J` v the enum, as well as stand-in names
(`?Run_0064e978@Sub@@`, `_ForwardTarget_00590235`, `CProcessor_00c09110::Load`, `CDspGain::ApplyRamps` for CFloatFilter::ApplyFilter,
`CUnknownSystem1D8::InitSubSystemB` for CSalPlatform::CreateSw). Each caller needs its declaration respelled, the same kind of
work as landing gn follow-up 2. They are still RELOCATION_MATCH: the grade masks the symbol, so retail parity is unaffected.
0x007E2A60 (a correct binding) is the one caller the one-off q.py could not grade, because it needs its catalog flags.
