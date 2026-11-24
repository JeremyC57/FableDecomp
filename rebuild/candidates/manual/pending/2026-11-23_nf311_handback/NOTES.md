# NF311 notes (2026-11-23): T1 "other" class / namespace / form, continuation of NF308, hand-back 1

Base: the repo working tree as of today; all 10 orig/ copies (6 rows + the 4 held entry-18 files) are cmp-equal to HEAD at hand-back.
Nothing was written under the repo; no git command of any kind was run. "Entry n" = the n-th data line of ENTRIES.tsv (file line n+1).

## Summary (STATUS.tsv: 164 verdicts; 48 census entries left unreached, listed below)
- **6 rows changed, landing-ready** (rows.txt), 4 entries:
  - entry 20, owner self + binder (LAND TOGETHER): 0x006D5050 (RemoveEmoteIcon `protected:`, full Leaf) + 0x006D5180 (one label);
  - entry 119: 0x00701990 (local CReactionMatch base: TransferBinaryIn `virtual`, `__vftable` member -> implicit vptr; own override QAE -> UAE, full Leaf);
  - entry 138: 0x00835AC0 (local CThingPhysical: DrawPreparePrimitivesForRendering `virtual`, pad 0x64 -> 0x60; own override `protected: virtual` = PDB MAE, full Leaf);
  - entry 85: 0x00709B10 + 0x00708A30 (local C3DAnimationInterpolator member model: `virtual` dtor, pad 0x14 -> 0x10).
- **Proof** (evidence/proof_e20 / proof_b2 / proof_b3, cmpfn_*, link_e20 / link_b2 / link_b3, recheck_final.log): HEAD and new
  RELOCATION_MATCH; objsec ALL_EQ under exactly the listed renames (.drectve included); COMDAT sets SAME; no ??_7 / ??_R symbol before or
  after (the only emitted tables, 0x006D5180's two, unchanged); links HEAD v new: every reference HEAD resolved stays resolved, the moved
  ones resolve (0x00835AC0's base call stays unresolved only on the PDB name: 0x006A64D0 has no landed row), 0 duplicates; vland
  --recheck-landed 6/6 RELOCATION_MATCH + test PASS. New-name census (evidence/namecensus.txt): only the owner rows define the names.
- **Table** (TABLE_CHANGES.tsv, 9 lines): 3 deletes that go with the rows (entries 20 / 119 / 138); 3 stale-witness deletes
  (entries 74 / 116 / 118: the witness's HEAD object binds the PDB form); 2 rewitnesses (entry 85 QAE line -> 0x00BD9250, entry 35
  CCharStringVector -> 0x005F05E5); 1 replace (entry 136, a stale stand-in spelling -> the stand-in the witness binds today). Each delete
  / replace was checked against every exe call site of its target (tools/xsites.py, all landed callers' sources; dtorcensus.py) and
  every absolute reference (absrefs.py; the one virtual target, 0x006A64D0, has two vftable slots whose table rows carry ??_7 lines only).
- **Held, prepared: entry 18 (NF308 entry 179, 0x0097E330 IAE owner fix).** The owner + the three NF308 respells, re-derived against
  today's rows (the same one-line labels), are in evidence/held_18/ (NOT rows/, NOT rows.txt): RELOCATION_MATCH, objsec ALL_EQ, COMDATs
  SAME, link with the IAE binder 0x009692C0, vland 4/4 PASS (evidence/recheck_held18.log). It cannot land: the fourth QAE binder
  0x009689C0 (LeanOnBar) is locked by NF310 (its HasEmptyJugSlot signature entry, NF310 rows/009689c0.cpp). Q1.
- **Rider (NF308 Q3), 8 U?$char_traits lines** (evidence/rider_U.tsv, rider_objdump.txt): 0 respelled.
  - 3 `__imp_` lines (witnesses 0x00C13840 / 0x00C10940) are MSVCP71.dll's own export names (Dinkumware U form, `objdump -p` of the exe):
    correct as they are.
  - the witnesses 0x007748E0 / 0x00841790 / 0x0099CD60 were compiled: their objects bind the U form themselves (local `struct char_traits`
    basic_string models), so the lines carry what their witnesses bind; the real fix is a struct -> class port of those rows (Q2, Q8).
  - 0x0083EBD0 (witness of the 0x00777DB0 reserve line) is an NF310 target: not compiled.
- Counts: 2 done (119, 138), 5 done-partial (20 and 85 with rows; 74 / 116 / 118 table-only), 4 skipped-held (3, 17, 18 prepared, 23),
  153 skipped-port (incl. the vtable stops 108 / 128 and entry 136 with its table replace), 0 rejected; 48 unreached (below).

## Order (E2 recorded)
Entries 1-23 (NF308's held ones) first, then 18 (owner unit) and 20 (same shape), then the rider, then 24-212 in order through the census;
the hand-opened shapes (QUAL / TAG / SCOPE from tools/mdiff2.py) 85, 99, 102, 108, 116, 119, 121-123, 125, 128, 136, 138 were pulled
ahead inside that pass.

## How the skipped verdicts were reached
- **Entries 1-23:** each NF308 hold re-read against today's tables and off-limits set (tools/offl.py now lists NF309 / NF310 entries,
  witnesses and locks). 17 had been held only for NF306 / NF307 but were ports anyway (stand-in class / parameter / free function) or
  wait on a T3_PORT_QUEUE row: now skipped-port. Still held: 3 (0x004BFFC0 identity research), 17 (only binder 0x009A6610 is an NF309
  witness; the in-place NVectorMath namespace shape), 23 (in-place const shape, but the binder 0x007BDB66 is an NF310 witness).
- **Entries 24-212: census (NF299 Q4), tools/census311.py.** IMPORTANT: the ENTRIES `spellings` column is CUT at 160 characters, so the
  census reads every stand-in's FULL spelling from rebuild/oracles/reloc_bindings.tsv (target match). A cut spelling made 0x009804A0
  (entry 91) look like an access-only fix; the full line takes a `UCIntelligentPointerConstThing@@` stand-in parameter.
  - tokens of the demangled stand-in found in neither PDB name set = MADEUP; another owning class = WRONGCLASS; free function / C stub =
    FREE; ?A0x hash = HASH; differing parameter / return types = SIG. 162 entries were all-port that way (48 of them only through SIG lines: left unreached, see below), 27 OPEN were read by hand.
  - stale screen (tools/stale311.py, full spellings): 387 OK, 6 MISS; the MISSes were compiled: entries 35 / 74 / 123 / 136 have stale
    table witnesses (table lines above; 123 is Q4), 109 is still a signature stand-in, 80 is an anonymous-namespace hash.
  - NF299 Q4 HDRFLAG check (tools/hdrflag311.py: the binder #includes engine/<PDB class>.h that names the leaf): 6 entries (87, 106, 114,
    118, 134, 139), all opened: 118 was a stale witness (compiled: binds the PDB form; table delete); the rest bind made-up classes or
    signatures. The identifier stale screen cannot see access / const / pointer-v-reference differences, which is why 116 / 118 needed
    the compile.
  - 1-in-10 sample of the census-only verdicts (entries 30, 40, 50, 60, 70, 80, 110, 130, 150, 160, 180, 200, 210): declaration lines
    read (evidence/sample311.txt); all still the stand-in.
  - **48 entries UNREACHED (no STATUS line):** 31 32 33 37 65 66 84 86 88 90 92 93 94 95 96 100 101 103 104 106 111 113 115 120 124 126
    127 129 131 140 142 146 154 155 158 162 170 171 172 176 188 190 191 195 202 205 207 208 (work/unreached.txt). Each has a stand-in
    line that keeps the PDB class and differs only in parameter / return types (census tag SIG), which NF299 Q4 does not cover, and I
    did not open them. Shapes seen in the census lines: pointer v reference, other element types on folded STLport bodies, `long` for a
    PDB enum (NF300 Q1 / NF304 Q2 candidates, e.g. 84 / 92 / 100 / 101 / 111 / 131), struct v class tags (31 `U?$allocator`, 33 / 188
    `PAV?$pair` for the STLport struct pair: rider-like), member-template instances bound by non-templates. Next batch: open them.
- **Vtable stops (brief: hand back as decisions):** entry 108 (0x00BACD50 emits the Group vftable; a virtual base dtor adds a base slot)
  and entry 128 (0x00A44A40 emits the vftable; the PDB protected dtor renames its own ??_G UAE -> MAE and the slot relocation). Q3.
- Entries 99 / 136 (0x007042A0) / 121-123: a virtual dtor would make the binder's own derived class polymorphic or change emitted vtable
  layouts: ports, not compiled.

## LAND TOGETHER
- Includers by path: none of the 6 rows or the 4 held files is #included by another landed source or test (evidence/includers.txt).
- 0x006D5050 + 0x006D5180 (the owner defined the public QAE stand-in; the binder resolved against it) + the entry-20 delete line.
- 0x00709B10 + 0x00708A30 + the entry-85 rewitness (the QAE line must not lose its last binder's witness).
- Each of 0x00701990 / 0x00835AC0 with its delete line.

## Notes on changed rows
- 0x00708A30's self is still the stand-in ??1CAnimationInterpolatorOwner_00708a30@@QAE@XZ (no Leaf; vland STANDIN_LEAF warn, same at
  HEAD): not this batch's entry; a catalog / self port (T3), mentioned here only.
- 0x00709B10's vland RELOC_NAME warn (??1CThemeChannelVector@@QAE@XZ -> 0x007099E0 labelled Destroy_CRetailVector) and 0x00967B40's two
  (FillTankardFromJug / PutDownJug ctors at 0x00842A80 / 0x00842AF0 labelled Start / StopFillingTankard) are pre-existing callee labels.

## Rule notes
- Locks: claimed with lock.sh (NF311) before any compile, after the off-limits refresh; every landed test prechecked BEFORE its lock
  (evidence/pre_tests1..8.log), every compiled source (HEAD and new) prechecked after its lock in an earlier call (pre_srcH1..7,
  pre_rows1, pre_srcH2 rows). 0x00945060 and 0x00CDFF50 / 0x00D408B0 were not compiled (tests fail clang only on VC7.1 headers / _assert
  and they were not needed), so never locked. Held now: the 6 changed rows; 19 released (evidence/locks.log).
- Background: every grade / link / vland ran under nohup with `timeout 900` on its own launch line (tools/gr.sh, lk.sh, rf.sh) and was
  checked with kill -0 in a later call. No sleep, no rm, no header / test / flag / catalog change, no __asm / naked / _emit.
- No retail or donor bytes in hand-back files (hex-run grep clean); oracles stay in work/.
- Reading: the prefix, the standing brief and all of GOTCHAS.md in full; NF308's landing README, NOTES.md and LESSONS.md in full, its
  STATUS.tsv held lines, and (at hand-back, to match the formats) the heads of its APPLIED / TABLE_CHANGES / RENAMES / NAME_FIXES; the
  first `cat` of its TSVs was truncated by the tool, so I did not read every line of them. CLAUDE.md as given. `apply_table.py --dry` on
  TABLE_CHANGES.tsv: 6 delete / 2 rewitness / 1 replace, 0 misses, no new multi-target symbol (evidence/apply_table_dry.txt; the held
  entry-18 line separately, evidence/apply_table_dry_held18.txt).

## Questions
1. **Entry 18:** the unit needs 0x009689C0 (HEAD) line 73 `    void SetPushableByHero(bool pushable);      // 0x0097E330` to become
   `    protected: void SetPushableByHero(bool pushable);      // 0x0097E330` (line 74 is `protected:` already; re-read against NF310's
   row first). Should NF310 add that label to its 0x009689C0 row, or a continuation land the unit after NF310's row lands? The prepared
   files are in evidence/held_18/ (the table delete for the QAE line goes with them).
2. **Rider:** no U line could be respelled (every compiled witness binds U itself). Should the three local `struct char_traits` rows
   (0x007748E0 / 0x00841790 / 0x0099CD60) be queued as one T3 port (their own definitions rename)?
3. **Entries 108 / 128 (vtable stops):** confirm both are ports (the fix changes an emitted vftable's content / names).
4. **Entry 123:** the ??1ScriptMethods@@UAE@XZ witness 0x008976F0 is stale (compiled: binds ??1CScriptThing@@UAE@XZ) but 0x00CDFF50 /
   0x00D408B0 still declare `virtual ~ScriptMethods();` (source read; their tests fail clang only on _assert, so I did not lock them).
   Rewitness to one of them after a landing-side compile?
5. **Entry 85 stretches NF299 Q3:** the local C3DAnimationInterpolator is a MEMBER model destroyed implicitly by the owner's dtor, not a
   base called qualified; same facts (no vftable / RTTI emitted, offsets kept, objsec ALL_EQ). If that is not allowed, drop rows
   0x00709B10 + 0x00708A30 and the entry-85 rewitness line as one unit.
6. **Entry 138 adds an access label on top of NF303 Q1** (the override QAE -> MAE, the PDB protected virtual, both PDBs). If only the
   UAE flip is allowed, drop row 0x00835AC0 and its delete line as one unit (nothing else depends on it).
7. **The entry-85 rewitness points at 0x00BD9250, which I could not compile** (T3_PORT_QUEUE); the source read shows the non-virtual
   declaration. Compile it at landing, or drop the rewitness and keep 0x00709B10 as witness until the T3 port (the row then no longer
   binds it).
8. **Rider names (moved here from NAME_FIXES, one source each):** the 0x00777DB0 reserve line (witness 0x0083EBD0, NF310 target, not
   compiled) and the U lines of 0x005693B0 / 0x00777F80 / 0x00567120 / 0x0099CB00 (witnesses bind U themselves) should carry the
   STLport `V?$char_traits` form (stlport/fable_string.h line 68 `class char_traits`) once their rows are ported; queue them?
