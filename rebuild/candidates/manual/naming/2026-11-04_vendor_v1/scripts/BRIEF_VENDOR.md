# Naming pass: vendor code (libpk, GameCODA, DirectShow strmbase), wave V1, 2026-11-04

Owner request 2026-11-04: "start a naming pass for vendor / other code like libpk". In scope:
- **libpk**, Lionhead's audio middleware (`d:\dev\audio\ver_fable_1.5_b\libpk\`), at 0xC08AE0..0xC57C90 and 0xC5E180..0xC89000.
- **GameCODA** (Sensaura) at 0xC57C90..0xC5E180.
- **DirectShow strmbase** (BaseClasses) at 0xCA4000..0xCB2000.

**Out of scope:** Xiph Ogg/Vorbis 0xC89000..0xCA4000, Miles, Bink and zlib (true third-party; owner rule), and Unreal code.

You build the evidence and decisions, then an OVERLAY of respelled landed sources, and you grade every row read-only. You do NOT
edit the repo or run git (not even read-only). The only places you write are `W=$S/naming/vendor/w1`, and inside it `$W/vw_*` for
vland work dirs. Other agents (chunks 768/769/770) use the lane at the same time. Never kill a process you did not start (kill your
own hung cl.exe by explicit PID only; never pkill/killall/wineserver -k).

## Environment (every Bash call)
    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
    source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
    W=$S/naming/vendor/w1; : "${W:?}"

## Read first
1. `CLAUDE.md` and `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`. These set the rules: names right the
   first time; two sources; exact spelling; one retail address = one name.
2. `$S/BRIEF_W35.md` for the respell / overlay / grading / deliverable mechanics. Follow it exactly, except that your rows come from
   this pass.
3. The model scripts: `rebuild/candidates/manual/naming/2026-11-04_wave35c/scripts/`, `2026-11-04_wave35d/`, `2026-11-03_wave33/`
   (mkpayload, mkbind, recheck). Reuse them.
4. libpk facts in `docs/pipeline/GOTCHAS.md` (grep libpk, GameCODA): no PDB, no RTTI, Dinkumware STL, LTCG register helpers. The
   earlier libpk decisions are in the naming wave dirs (grep -il libpk `rebuild/candidates/manual/naming/*/README.md`
   `*/DECISIONS*.tsv`), e.g. wave 9/31 D5 and wave 589. Do not reopen a decided row without new evidence.

## Inventory (built by the coordinator)
`$W/INVENTORY.tsv` has one row per address in 0xC08AE0..0xCB8000:
- range, landed, manifest module/name;
- `tu_strings`: the `__FILE__` strings the body references, such as `PKCBufferedReader.cpp` or `PKCMailbox.h`;
- `n_spellings` / `spellings`: every symbol that landed callers bind to this address in `rebuild/oracles/reloc_bindings.tsv`
  (symbol, target = callee address, address = caller row).

Rebuild it with `python3 $S/naming/vendor/inventory.py <out>` if you need to.

Counts:
- 285 libpk targets carry more than one spelling (0x00C167C0 has eight).
- About 920 landed libpk rows still use placeholder names (CUnproven*, CObject_<addr>, CVtable_<addr>, Method_<addr>, _global
  autonames).
- The strmbase block has misattributed module labels that `label_trust` propagation put there: CScriptThing, CTCObstacle,
  CEditControlCentre, CThingFilter_IsValid, CVideoPlayer_00ca9a90, CLockedAudioObject00cafd90.

## Evidence sources (two independent sources for any name you apply)
- **libpk `__FILE__` strings:** in libpk each `PKCFoo.cpp` defines class PKCFoo, and an inline in `PKCFoo.h` is a member of PKCFoo
  (or of the template PKCFoo<T>).
  - That is one source for the CLASS. The second must be structural: the ctor stores that class's vtable, the `this` layout matches
    the landed sibling, or a caller passes it where the landed model expects it.
  - A `__FILE__` string never proves a METHOD name.
- **libpk copies of Lionhead engine code** (e.g. CHandleBasedMemoryPool::Free 0x00C28690, the libpk copy of 0x00A02B30):
  - A body that is byte-identical or structurally identical to an engine function with an Ego_r/FableWin PDB name
    (`ghidra_out/egor_pdb_names.tsv`, `fablewin_pdb_names.tsv`) gets that name.
  - The second source is the same callee/field pattern.
  - Mind ICF: two retail copies of one symbol cannot both take one mangled name. Follow the wave-9 decision for the libpk copy (it is
    DEFERRED there) and report the conflict.
- **libpk's embedded MFC-6 CString** (assert texts `GetData()->nRefs`, `IsValidString(lpsz)`..., CString::AllocCopy 0x00C0BF90):
  - Use the MFC 4.2/6.0 `strcore.cpp`/`strex.cpp` method names, matched by assert line order and body shape.
  - `$S/dl/vs2003/.../mfc71.pdb` is MFC 7.1's CStringT. It is not the same class, so do not use it for CString names.
- **strmbase:** the DirectShow BaseClasses source (Windows SDK `Samples\Multimedia\DirectShow\BaseClasses`, or
  github.com/microsoft/Windows-classic-samples `Samples/Win7Samples/multimedia/directshow/baseclasses`).
  - Try to fetch it. If the network blocks it, say so and use only the vtable-slot evidence plus the names already landed.
  - Vtable slot order against the class declaration is the second source. COM methods are `__stdcall` (`UAGJ...`).
- **GameCODA:** strings only, since no SDK is public. Mostly placeholder consolidation.

## The work, in this order (write DECISIONS_V1.tsv incrementally: address, decided_symbol, grade proven|placeholder|ICF|deferred, evidence)
1. **One address, one name** for every multi-spelled target in scope:
   - If the address has a proven name (two sources), that wins.
   - Otherwise the spelling the landed DEFINING row emits (its `// Leaf:`) wins.
   - Otherwise take the most-bound existing spelling. Never invent a new descriptive spelling.
   - **ICF exception:** distinct real symbols of byte-identical bodies (template instances folded by the linker) are genuine retail
     behaviour. Mark them ICF and leave them.
   - Respell the losing callers (CALLERS.tsv) and add the ADD/RETIRE bindings.
2. **strmbase:** prove names from the BaseClasses source + vtables. Fix the misattributed module labels (LABELS.tsv). Respell landed
   rows whose leaf is wrong.
3. **libpk classes from `__FILE__` + structure:** rename placeholder classes where two sources agree (e.g. `CUnprovenX_00c1....` ->
   `PKCFoo`). A method name without a second source stays a placeholder, queued as PROPOSAL in NAME_FIXES.tsv.
4. **libpk engine-code copies** (PDB-named twins) and the MFC CString members.

## Limits and pacing
- Overlay respells go in batches of at most 150 rows. Do the rows of steps 1-2 first.
- Respelling a defining row changes its symbol, so every landed caller must follow. Recheck each caller in the same batch.
- A row whose respell changes bytes, or whose evidence you doubt, goes to DEFER with a reason. Never guess.
- Stop at about 300 respelled rows. The decisions table may cover the whole range; put what you did not respell in QUEUE_V2.tsv for
  the next wave.

## Deliverables in $W
- **Overlay and respells:** `overlay/...`, `RENAMES.tsv`, `PASS.tsv`, `DEFERRED.tsv`, `CALLERS.tsv` and `BINDINGS.tsv`, exactly as
  BRIEF_W35 specifies.
- **Decisions and names:** `DECISIONS_V1.tsv`, `LABELS.tsv` (address, name, evidence: catalog-form `Class::Method`, two sources
  only), `NAME_FIXES.tsv` (PROPOSAL rows for one-source names), `QUEUE_V2.tsv`.
- **Notes and logs:** `NOTES.txt` (method, counts, GOTCHAS one-liners) and `logs/`.

## Final reply (short)
- counts: decided by grade, respelled + passing, deferred by reason;
- callers respelled;
- bindings ADD/RETIRE;
- strmbase source fetched or not;
- anything the coordinator or owner must decide.
