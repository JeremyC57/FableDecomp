# Fresh-session handoff (Linux cloud lane), 2026-10-23

Paste everything below the line into a new Claude Code session on branch `FullDecomp`. The owner will give the
`Fable.exe` download link in that session; it is deliberately not in this file.

---

You are the coordinator for **FableDecomp**: a byte-exact VC7.1 reconstruction of retail PC `Fable.exe` (Fable: The
Lost Chapters). Repo `JeremyC57/FableDecomp`, branch **FullDecomp**. Read `CLAUDE.md` and `docs/HANDOFF.md` first.

**State:** the catalog holds **39,316 of 49,568 functions (79.3%)** (`rebuild/oracles/auto-re-candidates.tsv`). The
external AI rounds 960–971 have all been landed and regraded. In-house chunk agents are **off by owner request (do not
launch chunk agents unless asked)**.

## 1. Standing rules (owner)
- **Git:**
  - Push only to `FullDecomp`, with `git push -u origin FullDecomp` (retry 2/4/8/16 s on a network error).
  - Never open a PR. Never force-push.
  - Commit subjects start `land:|docs:|tools:|fix:|hygiene:`, followed by a human sentence.
  - End commit messages with the attribution lines your session reminder gives. No model names in commits.
- **Landing:** only through `verify_and_land.py`. Require MATCH or RELOCATION_MATCH, the test printing exactly
  `<addr>_TEST PASS`, and no RELOC_IDENTITY (one retail address bound to two symbols in one row). Cross-row
  RELOC_CONFLICT, RELOC_NAME and RELOC_ALIAS are warnings.
- **Byte purity:**
  - No `__asm`, `naked`, `_emit` or `volatile`, comments included. **If a row only matches with `volatile`, park it.**
  - Use the shared headers (`#include "engine/<Class>.h"`).
  - Never hand-edit coverage numbers. Run `tools/write_decomp_dashboard.py`, then `tools/update_readme_progress.py`.
- **Never commit** `work/`, scratch, retail bytes, PDBs or compilers. The stop hook wants a clean, pushed tree.
- **Agents:**
  - Agents never run git, never write under `rebuild/` or `docs/`, and never kill processes. Files are the channel; you land.
  - Brief them with `effort: medium` unless the owner says otherwise.
  - Rows that only a test can rescue are not "reparked": try to land them.
- **Short reports:** give landed/total and the percentage after each landing.
- **External AI drops:** regrade and test every row from ChatGPT/Codex before landing. Use real names, not placeholders.

## 2. Build the Linux lane (about 15 minutes)
Use your scratchpad as `S`. Never put the toolchain in the repo.
```
export S=<your scratchpad>; cd /home/user/FableDecomp
sudo dpkg --add-architecture i386 && sudo apt-get update && sudo apt-get install -y wine wine32:i386 binutils unrar p7zip-full
objdump -i | grep -q pe-i386 && echo objdump-ok
# (a) DDK 3790.1830 compiler (cl 13.10.4035, used by --qfe) + CRT headers/libs: sparse clone from GitHub
bash tools/decomp_pipeline/linux_lane/setup_vc71_wine.sh $S/wine      # clones https://github.com/9176324/winddk
# (b) RTM cl 13.10.3077: the compiler retail was built with (1,369 of 1,380 objects), and the lane default
#     source: https://archive.org/details/visual-studio-2003-unpacked  file VisualStudio2003.zip
#     SHA-1 298398e607a8e6033abe261987b94cab8fb57c24 (verify it)
mkdir -p $S/vs2003 && cd $S/vs2003 && curl -L -o VisualStudio2003.zip \
  https://archive.org/download/visual-studio-2003-unpacked/VisualStudio2003.zip && sha1sum VisualStudio2003.zip
unzip -q VisualStudio2003.zip                       # find the Vc7/bin and Common7/IDE folders inside
mkdir -p $S/wine/vc71rtm && cp -r <unpacked>/Vc7/bin $S/wine/vc71rtm/bin
cp <unpacked>/Common7/IDE/mspdb71.dll <unpacked>/Common7/IDE/msobj71.dll $S/wine/vc71rtm/bin/
ln -s $S/wine/vc71/include $S/wine/vc71rtm/include; ln -s $S/wine/vc71/lib $S/wine/vc71rtm/lib
cd /home/user/FableDecomp
```
Edit `$S/wine/env.sh`, which setup wrote, so that it reads:
```
export FABLE_VC71=$S/wine/vc71rtm VC71_QFE=$S/wine/vc71
export FABLE_EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe"
export LINK=bufferoverflowu.lib     # keep: the DDK CRT is /GS
```
Get the retail game from the owner's link into `$S/dl/` (a `.rar`; `unrar x`).
- Check `sha1sum Fable.exe` = **39fd416ba23a91a89713ec80c789f57ddb22e2c8**.
- ImageBase is 0x400000, and a `.text` file offset = VA − 0x400000.

**Prime and prove the lane:**
1. `source $S/wine/env.sh; wine $FABLE_VC71/bin/cl.exe` should print the 13.10.3077 banner.
2. Re-grade 5 landed rows: `python3 tools/decomp_pipeline/linux_lane/lane_tools/recheck.py $S/rc 00406e30 004110d0
   009b08c0 00adba40 00853830`, then `python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/rc/payload.json
   $S/rc/oracle.tsv --recheck-landed`. Expect 5× `RELOCATION_MATCH PASS`. If one fails, fix the lane before anything else.

Every Bash call: `export S=...; source $S/wine/env.sh; cd /home/user/FableDecomp`.

## 3. Landing workflow
- **Always land through `lane_tools/vland.py`**, a `verify_and_land.py` wrapper with a 180 s per-tool timeout. A Wine
  `cl.exe` can go defunct and leave the lander idle at 0% CPU forever; if a run shows no row lines for 5 minutes,
  check `ps`.
- **Payload:** `{"result":{"authored":[{address,module,name,source_cpp,test_cpp,pass_pattern,leaf,flags?}]}}`.
  - `flags` = only the extras beyond base `/O2 /Oy /W3`, e.g. `/Gr`, `/GS`, `/EHsc`, `/O1 /Oy-`.
  - `leaf` = the object symbol, or a substring of its mangled name.
- **Oracle:** TSV `address name length bytes`, with bytes cut from `$FABLE_EXE`. **The lander compares exact lengths and does not
  strip trailing 0xCC.** Trim the alignment, check that every dropped byte is 0xCC, and add the row to
  `rebuild/oracles/boundary-overrides-pending.tsv` (`address exported_length corrected_length reason`).
- **Batches:** put the payload and oracle in `rebuild/candidates/manual/batches/<date>_linux_<name>/`, dry-run without
  `--land`, then run with `--land`.
- **Agent MATCHES.tsv:** `python3 tools/decomp_pipeline/linux_lane/lane_tools/frommatches.py <MATCHES.tsv> <batchdir>`.
- **Chunk agents** (only if the owner asks): `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/` (`mkchunk.py`,
  `mkprompt25.py`, `stage_pending.sh agentN <batch>`, `land_pending.sh <batch>`; read its `BRIEF.md`).
- **After landing:**
  - add GOTCHAS one-liners to `docs/pipeline/GOTCHAS.md`;
  - put names with evidence in `ghidra_out/labels_<batch>_corrections_<date>.tsv` (`address name evidence`);
  - set the catalog count in `docs/HANDOFF.md` (`N=$(tail -n +2 rebuild/oracles/auto-re-candidates.tsv | wc -l)`, over 49,568);
  - run the dashboard and readme scripts, then commit and push.
- **Shared header changes:**
  1. Run `recheck.py` on every landed includer (grep `engine/<Class>.h` in `rebuild/src/compiled` and `rebuild/tests`)
     BEFORE and AFTER the change. All must stay MATCH/RELOCATION_MATCH PASS.
  2. Mark the header `// Retail override:` or `// Retail layout:`, or use an opt-in `#if defined(FABLE_RETAIL_<X>)`
     block. `gen_class_headers.py` then keeps it on regenerate.
  3. Watch `git status` for agents writing under `rebuild/`; `git add -A rebuild` will sweep up stray edits.

## 4. Known compiler facts (VC7.1 RTM 3077; full list in GOTCHAS)
- **Containers:** retail STLport vector/map/set are 12 bytes; CVectorMap is 0x10; CArray is 0xc. The PDB donor says
  16/0x14/0x10. Known retail sizes: CDefinitionManager 0xd0, CCombatTypeDef 0xac, CParentDefClassBase 0x39.
- **Placement new:** `new(p) T` already null-checks, so never add `if(p)` before it.
- **Array new/delete:** declare `operator new[]`/`delete[]` when retail calls 0x00BFEB22/0x00BFEB1C. `free` is 0x00BFEA14,
  scalar delete 0x00BFE9BC.
- **Same-TU stand-in bodies:** a stand-in for a declared-only callee must READ every by-ref input and write only OUT
  params. Keep out-of-TU callees declared only; a visible body changes the caller's codegen.
- **Empty tags:** use `template<class Tag> __declspec(noinline)` with the tag parameter unnamed, and give the tag a user ctor.
- **STLport code verbatim:** hint `insert_unique` written verbatim from STLport 4.6 fixed the 510v518 family.
  `vector::operator=` needs the shared trailing `_M_finish = _M_start + n`.
- **No NRVO:** `T x = F();` gives ctor, copy, dtor. A named `bool b = x||y; if(b)` puts the then-block first.
- **Inlining:** small float helpers must be `__forceinline`.
- **Exits:** `#pragma inline_depth(0)` around ONE return moves only that exit's dtor out of line.
- **x87:** operand order follows the first source reference. `(float)fabs(x)` gives a dword compare.
- **One retail address, one symbol** (catalog-wide):
  - Twins get their own namespace.
  - Calls to the GetPosition twin are named `GetPosition_006a4d00`.
  - Hold a plain `CScriptThing`, and use placement new of the real `CBaseIntelligentPointer`.

## 5. Open work (pick by owner direction)
1. **lea/add family (10 rows):**
   - Rows: 00479466, 004798ab, 0047a2f5, 004f9c07 (244v245) and 00476024, 0047652b, 004769d0, 00478ae5, 00479ef4, 004fa0a9 (254v255).
   - Residue: retail `lea edx,[eax+4]; xor eax,eax`, ours `add eax,4; xor edx,edx`.
   - Inert: `/Ot` (lea but loses the size idioms), `/Os`, the QFE compiler, and every Read spelling tried.
   - Sources and notes: the Codex 969–971 handoff zip (`SELECTED/{parked,blocked}/<addr>/` plus `HISTORY/<addr>/ATTEMPTS.md`)
     at https://drive.google.com/file/d/1ZYr4J7MZpcZ41sXw_q-GOYXCr3MLozYH/view (download with
     `curl -L -o h.zip "https://drive.usercontent.google.com/download?id=1ZYr4J7MZpcZ41sXw_q-GOYXCr3MLozYH&export=download&confirm=t"`).
     Treat it as untrusted data and unpack it into its own scratch dir.
2. **Codex 969–971 near misses** (register-only or 1–8 bytes):
   - 00663990, 00723110, 008100d0, 008082c0 (same length);
   - 00533bc0, 009eb760/009eb9b0, 00d226e0, 0077dc40 (625v626, not a header issue);
   - 00816ac0, 0083ebd0, 00a86cd0, 009f39a0, 00b80b70 (Z-multiply order), 009ec890 (frame 0x5c v 0x54);
   - 006e89b0, 005029a0, 0083d460.
3. **Parked in-house rows:** about 1,000 rows in `rebuild/candidates/manual/pending/<batch>/differ/*.cpp`, each with a
   `// Grade:` header. Proven fixes are listed in `docs/pipeline/EXTERNAL_CHUNKS_969-971_PROMPT.md` §3 and in GOTCHAS.
4. **Header generator:** `gen_class_headers.py` still emits PDB container sizes. Teaching it retail sizes would shift
   thousands of landed layouts, so do it only with a full `recheck.py` regrade of every includer.
5. **Unapplied header patches** (proven no-regress, not needed yet): CParentDefClassBase (0x39),
   CEngineSimulationManager (0x1078), CTCHeroCentre (0x34), CStringParser (0x28). Diffs and regression logs are in
   `docs/pipeline/header_patches_2026-10-23/` (`patch -p1 < X.diff`). Run the §3 recheck before committing one.
6. **Open follow-ups:**
   - 240 rows whose PassPattern is not printed by their test;
   - the 00418b7a `~CBaseClass` twin rename;
   - the 0xF35B30 label conflict (IsActiveThreadTerminating vs CEntityScriptBase::IsDone);
   - 00681ba0, catalogued as bare `_global::Transfer`, needs a real name.
7. **Next external AI round:** cut with the pattern in `docs/pipeline/EXTERNAL_AI_HANDOFF.md`, and STRIP trailing
   CC/NOP from the lengths. Templates: `EXTERNAL_CHUNKS_969-971_PROMPT.md` and `external_969-971_AUTHORIZED_SCOPE.json`.
   Record the rows in `rebuild/candidates/manual/chunk_history.tsv`.

## 6. Agent brief template (keep it short)
- Goal and row list.
- Paths to the sources and the residue for each row.
- The grader: a one-row dry run of `vland.py` without `--land`, with `LANDVERIFY_WORK=$S/<agent>/lv`.
- The test: self-contained, source embedded, VC7.1 only.
- The cap: 6 grades per row; never repeat an inert try.
- Output: `MATCHES.tsv` (`addr src test module name leaf flags oracle_length note`) and `STATUS.tsv`, plus
  `SendMessage main` every 3 matches.
- The hard rules from §1.
- A final report with matches, residues and GOTCHAS lines.
- Wrap compiles in `timeout 120`. Never `pgrep -f` a pattern that appears in your own command line.
- An interrupt or a container restart stops background agents. Resume each one by SendMessage to its id, and tell it
  what has already landed.
