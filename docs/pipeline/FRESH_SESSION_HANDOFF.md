# Fresh-session handoff (Linux cloud lane), 2026-10-26

Paste everything below the line into a new Claude Code session on branch `FullDecomp`. The owner gives the
`Fable.exe` download link in that session; it is deliberately not in this file.

---

You are the coordinator for **FableDecomp**: a byte-exact VC7.1 reconstruction of retail PC `Fable.exe` (Fable: The
Lost Chapters). Repo `JeremyC57/FableDecomp`, branch **FullDecomp**. Read `CLAUDE.md` and `docs/HANDOFF.md` first.

**State (2026-10-26):** the catalog holds **47,413 of 49,568 functions (95.7%)**
(`rebuild/oracles/auto-re-candidates.tsv`). No agents are running. Chunk 484 was paused by the owner (its workspace was
scratch and is gone: re-cut it if wanted). See `docs/HANDOFF.md` for what landed on 2026-10-25/26 and the open queue.

## 1. Standing rules (owner)
- **Git:** push only to `FullDecomp` with `git push -u origin FullDecomp` (retry 2/4/8/16 s on network errors). Never
  open a PR, never force-push. Commit subjects start `land:|docs:|tools:|fix:|hygiene:` + a human sentence; end with the
  attribution lines your session reminder gives; no model names in commits. The stop hook wants a clean, pushed tree.
- **Landing:** only through `verify_and_land.py` (wrapper `lane_tools/vland.py`). Require MATCH or RELOCATION_MATCH,
  the test printing exactly `<addr>_TEST PASS`, and no RELOC_IDENTITY. Cross-row RELOC_CONFLICT/NAME/ALIAS are warnings.
- **Byte purity:** no `__asm`, `naked`, `_emit`, `volatile` (comments included). Exception: grade `retail_asm` (an
  instruction VC7.1 cannot emit from C++, with a `// Retail-asm: <evidence>` line; `crawl/purity.py`).
- **Retail fidelity (owner 2026-10-25/26, all applied catalog-wide; keep new work consistent):**
  - STLport 4.6 code lives in namespace **`std`**, never `_STL` (retail mangles `@std@@`).
  - **Class key = retail's** (`ghidra_out/pdb_class_keys.tsv`): `V` -> `class X { public: ... };`, `U` -> `struct`.
  - **Container sizes = STLport 4.6** (engine headers are re-laid out): list 4; vector/map/set/string/CArray 0xC;
    deque 0x28; CVectorMap 0x10; vector<bool> 0xC; bitset<N> 4*ceil(N/32). Bases from RTTI (`ghidra_out/rtti_bases.tsv`).
  - **No invented stand-in types** for library code (no new CRetailTree*/CRetailList/CRetailVector): real STLport
    templates with proven types, else per-address `CUnproven*_<addr>` placeholders marked "descriptive".
  - **One retail address = one name:** look the callee up in `rebuild/oracles/reloc_bindings.tsv` + PDB name sets;
    reach inherited members through real inheritance / the D34 layout-rename include pattern / base-dtor chaining,
    never a local alias on a derived stand-in. A destructor's identity is the vtable it stores first.
  - Names from evidence only (PDB, RTTI, callers, strings); propagated catalog labels are often wrong (no ICF in retail).
- **Shared headers:** `#include "engine/<Class>.h"`, never a local padded struct when a header exists.
- **Never hand-edit coverage:** run `tools/write_decomp_dashboard.py`, then `tools/update_readme_progress.py`.
- **Never commit** `work/`, scratch, retail bytes, PDBs, compilers, or the game.
- **Agents:** no git, never write under the repo (or work in a private `git worktree` you create for them and merge
  yourself), kill only explicit PIDs they started (never pkill/killall/wineserver -k), wait on jobs by PID (never
  `pgrep -f`). Files are the channel; the coordinator lands. Give landed/total and the percentage after each landing.

## 2. Build the Linux lane (about 20 minutes)
Use your scratchpad as `S`. Never put the toolchain in the repo.
```
export S=<your scratchpad>; cd /home/user/FableDecomp
sudo dpkg --add-architecture i386 && sudo apt-get update && sudo apt-get install -y wine wine32:i386 binutils unrar p7zip-full llvm
objdump -i | grep -q pe-i386 && echo objdump-ok          # objdump must handle pe-i386
which llvm-undname || ls /usr/lib/llvm-*/bin/llvm-undname # used by tools/decomp_pipeline/pdb_class_keys.py (FABLE_UNDNAME)
# (a) DDK 3790.1830 (cl 13.10.4035, the --qfe compiler) + CRT headers/libs: sparse clone from GitHub
bash tools/decomp_pipeline/linux_lane/setup_vc71_wine.sh $S/wine      # clones https://github.com/9176324/winddk
# (b) RTM cl 13.10.3077 = the retail compiler (1,369 of 1,380 objects) and the lane default
#     https://archive.org/details/visual-studio-2003-unpacked  file VisualStudio2003.zip
#     SHA-1 298398e607a8e6033abe261987b94cab8fb57c24 (verify it)
mkdir -p $S/vs2003 && cd $S/vs2003 && curl -L -o VisualStudio2003.zip \
  https://archive.org/download/visual-studio-2003-unpacked/VisualStudio2003.zip && sha1sum VisualStudio2003.zip
unzip -q VisualStudio2003.zip                       # locate the Vc7/bin and Common7/IDE folders inside
mkdir -p $S/wine/vc71rtm && cp -r <unpacked>/Vc7/bin $S/wine/vc71rtm/bin
cp <unpacked>/Common7/IDE/mspdb71.dll <unpacked>/Common7/IDE/msobj71.dll $S/wine/vc71rtm/bin/
ln -s $S/wine/vc71/include $S/wine/vc71rtm/include; ln -s $S/wine/vc71/lib $S/wine/vc71rtm/lib
rm -rf $S/vs2003                                    # ~disk: keep only the copied bin
cd /home/user/FableDecomp
```
Make `$S/wine/env.sh` (setup wrote one; template `tools/decomp_pipeline/linux_lane/lane_tools/env.sh.template`) read:
```
export WINEPREFIX=$S/wine/prefix WINEARCH=win32 WINEDEBUG=-all WINEDLLOVERRIDES="mscoree,mshtml=" DISPLAY=
export FABLE_ROOT=/home/user/FableDecomp FABLE_OBJDUMP=objdump FABLE_WINE=wine
export FABLE_VC71=$S/wine/vc71rtm VC71_QFE=$S/wine/vc71
export FABLE_EXE="$S/dl/game/Fable The Lost Chapters/Fable.exe"
export LANDVERIFY_WORK=$S/wine/landverify
export LINK=bufferoverflowu.lib     # keep: the DDK CRT is /GS
wineserver -p 2>/dev/null || true   # one persistent wineserver; without it every cl.exe boots a server (~5 s)
```
Get the retail game from the owner's link into `$S/dl/` (a `.rar`; `unrar x`). Check `sha1sum Fable.exe` =
**39fd416ba23a91a89713ec80c789f57ddb22e2c8**. ImageBase 0x400000; a `.text` file offset = VA - 0x400000.

**Prime and prove the lane** (every Bash call: `export S=...; source $S/wine/env.sh; cd /home/user/FableDecomp`):
1. `wine $FABLE_VC71/bin/cl.exe` prints the 13.10.3077 banner (the first Wine start takes a minute).
2. `python3 tools/decomp_pipeline/linux_lane/lane_tools/recheck.py $S/rc 00406e30 004110d0 009b08c0 00adba40 00853830`,
   then `python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/rc/payload.json $S/rc/oracle.tsv --recheck-landed`.
   Expect 5x MATCH/RELOCATION_MATCH PASS. If one fails, fix the lane before anything else.
3. Disk is a fixed allowance: delete `LANDVERIFY_WORK` dirs after big regrades.

## 3. Landing and pass workflow
- **Payload** `{"result":{"authored":[{address,module,name,source_cpp,test_cpp,pass_pattern,leaf,flags?}]}}`; `flags`
  = extras beyond `/O2 /Oy /W3`; `leaf` = the exact object symbol (check with `objdump -t`; a leaf naming no symbol
  silently grades the largest function -- `lane_tools/naming/leafscan.py <lvw> [addrs]` finds those).
- **Oracle** TSV `address name length bytes` cut from `$FABLE_EXE`; exact lengths (trim trailing 0xCC, record in
  `rebuild/oracles/boundary-overrides-pending.tsv`).
- **Agent chunks/family passes:** `lane_tools/chunk_pool/` (`mkchunk.py`, `mkprompt25.py`, `EXTRA_RULES.md` = the
  current agent rules, `landone.sh N` / `land5.sh N` land `$S/w22/agentN` -> batch `2026-10-24_linux_handN`;
  `land5.sh` converts `_STL`->`std` and class keys (`lane_tools/naming/conv.py`, `keypayload.py`) before grading).
  Family passes: build reps-only chunk from `famscan.py` families (see `docs/journal/2026-10/` family-pass notes).
- **Catalog-wide passes** (the 2026-10-26 pattern): give the agent a private `git worktree add --detach $S/<pass>/repo
  HEAD` with `FABLE_ROOT` pointing at it; it edits there and regrades every touched row (`recheck.py` +
  `vland.py --recheck-landed`, batches of ~400, baseline regrade for failures). The coordinator commits the worktree on
  a temp branch, `git cherry-pick -n` into `FullDecomp` (resolve `reloc_bindings.tsv` by set-difference), runs an
  independent spot regrade, then commits, pushes and removes the worktree.
- **After landing:** GOTCHAS one-liners (`docs/pipeline/GOTCHAS.md`), names with evidence in
  `ghidra_out/labels_<batch>_<date>.tsv`, FINDINGS_LOG paragraph, catalog count in `docs/HANDOFF.md`, dashboard +
  readme scripts, commit, push.

## 4. Open work (pick by owner direction; details in `docs/HANDOFF.md` and the 2026-10-26 pass notes)
1. **Family pass 5+:** ~735 families / ~2,540 rows remain in `families1024` after pass 4 (rebuild the family file
   with `lane_tools/famscan.py` in a new scratch).
2. **One-address-one-name remainder** (`docs/journal/2026-10/2026-10-26_one_address_one_name_pass.md`, REMAINING):
   CBaseIntelligentPointer::GetPItem return type (~340 rows), free/static stand-in callers (GetHeroThing, GetFrame,
   GetMainPlayer; ~3,490 candidates), 59 CRT free-mock tests, LowerBound const overloads, CDataInputStream (227).
3. **CRetailTree remainder:** 882 kept rows (`docs/journal/2026-10/treepass/KEPT.tsv`) and ~180 misnamed /Os tree
   functions (`OS_FAMILY.tsv`).
4. **Recheck sweep:** 35 rows failing before the class-key pass, 93 rows whose Leaf names no symbol, ~484 stale
   bindings, 318 unknown-key headers, 389 headers without size evidence, CEngineResourceManager size drift.
5. **Parked rows** (`rebuild/candidates/manual/pending/<batch>/differ/`), retail_asm candidates (0x774610, 0x779EF0,
   0x81EA50, 0xB17BD0, 0xB1BC80, 0xA145C0), volatile candidates (0x9919C0, 0x991C10).
