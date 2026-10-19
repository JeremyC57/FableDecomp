# Claude continuation — FullDecomp, 2026-10-18

**Historical wrap-up snapshot at89320160f. The user subsequently resumed work with one Astra/medium fresh worker and one Sol6.1/medium parked worker; current counts and operating state are in `docs/HANDOFF.md` and the latest lane checkpoints. The archive described here remains the frozen pre-resume snapshot.**

Read this snapshot for baseline context, then `docs/HANDOFF.md` for the current state. The owner requested that Codex finish the current agent rows, stop, and package all work for Claude. **All workers are stopped. Every submitted ready match has been independently reviewed and landed. No new authoring or compiler run is pending.**

## Final state and progress

- Repository: FableDecomp; exact branch spelling **`FullDecomp`**. Live checkout was `/workspace/FableDecomp`.
- Claude's checkpoint: **`fbfaf9f90`**, **34,098 / 49,568 functions = 68.7903%** (68.8% in Claude's rounded reporting).
- Final accepted catalog: **34,246 / 49,568 = 69.0889%** (69.1% rounded), **148 additional functions**. Use this FUNCTION metric when reporting progress to the owner.
- The separate genuine-C++ audit is **33,662 functions / 1,568,258 bytes**. It excludes historical catalog entries identified by the earlier audit; do not substitute it for the owner's requested catalog percentage or claim that all historical catalog entries meet the current authoring standard.
- Last code landing: `4fa12a46d` (retry45). Final checkpoint/documentation commits follow it; the archive's `HANDOFF_STATE.json` gives the exact final branch tip and full baseline SHA.
- Local commits contain all accepted changes. **Do not assume a remote push occurred.** Earlier automatic approval review rejected pushing to the unverified configured remote `https://github.com/JeremyC57/FableDecomp.git`; older project documentation names `BuffJesus/FableDecomp`. No push was performed during this wrap-up. The bundle and patch transfer the work without relying on the remote.

The live count is obtained from `Address = '<8 lower-case hex digits>'` entries in `rebuild/build_candidates.ps1`. Generated dashboards may lag until the canonical Windows refresh. Do not manually edit generated coverage files to make them agree.

## What the handoff archive contains

1. `START_HERE.md`: this guide.
2. `HANDOFF_STATE.json`, `COMMITS.txt`, `CHANGED_FILES.txt`, and `LANDED_SINCE_CLAUDE.tsv`: exact tip, baseline, commit history, changed paths, and the 148 added function addresses.
3. `FullDecomp.bundle`: all Git commits since the Claude checkpoint, including sources, tests, canonical oracles/catalog, Wine fixes, pipeline fixes, documentation and committed candidate checkpoints. It requires the baseline Git history.
4. `FullDecomp-since-Claude.patch`: a full binary-capable patch from that same baseline as an alternative transfer mechanism. Do not apply both transfer mechanisms.
5. `scratch/`: portable text work under the original repository-relative paths, including sources, tests, probes, disassembly/grade logs, precise attempt ledgers, payloads/oracles, saved Claude prompts, coordinator scripts and lane tools. This preserves unfinished work that was intentionally outside canonical source control.
6. `SHA256SUMS`: hashes for every other archive member.

Retail game binaries, compiler binaries, Wine prefixes, generated objects/executables and system libraries are not packaged. Their installation/recovery is documented below. The archive is a continuation package for the existing repository, not a standalone game build.

## Import without overwriting newer work

Start from the existing repository containing `fbfaf9f90`; inspect `git status` and preserve any unrelated local work. Set `HANDOFF_DIR` to the extracted archive directory. To inspect/import the bundle into a new branch:

```bash
git bundle verify "$HANDOFF_DIR/FullDecomp.bundle"
git fetch "$HANDOFF_DIR/FullDecomp.bundle" FullDecomp:refs/heads/codex-fulldecomp-handoff
git switch codex-fulldecomp-handoff
```

The bundle verification lists its required baseline commit. If this checkout already contains the final tip, importing is unnecessary. To advance the intended branch after checking that it has not diverged:

```bash
git switch FullDecomp
git merge --ff-only codex-fulldecomp-handoff
```

If there is divergence, inspect and integrate it normally; do not reset or force-push. The patch is an alternative for a clean baseline checkout: first `git apply --check "$HANDOFF_DIR/FullDecomp-since-Claude.patch"`, then apply it only after the check succeeds.

Copy the extracted `scratch/work/` into the repository's `work/`, preserving any newer local files first. Scratch paths in the source files and logs refer to the old `/workspace/FableDecomp` checkout; update the environment if using a different path. Authoritative canonical sources always take precedence over scratch snapshots.

## What Codex completed

- Restored a working VC7.1 compiler/test lane on this host using Wine 11 WoW64, a digest-guarded Wine host-library patch, a fresh WoW64 prefix, complete VC7.1 headers/libraries, and the existing compiler binaries unchanged.
- Verified the compiler against a pre-existing catalog match and a multithreaded C++ exception smoke test, then continued real compiler-in-the-loop reconstruction.
- Landed 148 additional functions from resumed chunks, new chunks and careful parked/skipped retries. `LANDED_SINCE_CLAUDE.tsv` is the complete address list. Each new landing has independent native parity and executable behavior evidence in its batch directory.
- Preserved incomplete candidates and their historical budgets instead of treating skipped placeholders as attempts or resetting counters between workers.
- Improved failed/partial landing preservation and retained the pipeline regression checks described in the journal. Embedded jump-table verification uses the raw-COFF verifier.
- Corrected misleading propagated owner labels, native versus donor layouts, virtual destructor models, calling conventions, shared-header omissions and fixture behavior during main review. A relocation-only match was not accepted without checking those details.

See `docs/journal/2026-10/CODEX_RESUME_2026-10-17.md` for the full session record. It intentionally retains its original filename/date heading although later batches/checkpoints are dated October 18. Canonical landing evidence is under `rebuild/candidates/manual/batches/2026-10-17_linux_*` and `2026-10-18_linux_parked_retry*` through **retry45**. The final durable candidate checkpoint is `rebuild/candidates/manual/pending/2026-10-18_linux_claude_final/`.

## Owner instructions for a continuation

- Work on FullDecomp, read the READMEs and saved Claude per-chunk `PROMPT.md`, and continue the actual reconstruction work. The original prompt's model name and old four-attempt/early-parking instructions are superseded.
- **Ten total source-change or compiler-flag grading attempts per function, including historical agents and main-review integrations.** An unchanged environment rerun is not a new source attempt. A compile failure after a source change does count. Preserve exact ledgers and any uncertainty.
- Parking and skipping are last resorts. Investigate signatures, types/layouts, real callees, ownership/lifetimes and compiler output. A placeholder is not an authored DIFFER. Do not keep regrading a capped row without a new user budget instruction.
- Latest running team preference before wrap-up was **one GPT-6.1 Sol at medium effort for parked/skipped rows, one GPT-6 Astra at medium effort for fresh rows**, with main-chat review before landing. Older four/six-worker and high-effort instructions are obsolete. All agents are now stopped; a continuation should follow the owner's current authorization and available tools.
- During an authorized ongoing run, replace finishing workers within the current cap, move into the next chunk, provide occasional total FUNCTION percentage updates against Claude's baseline, and do not arbitrarily stop. This session stopped because the user explicitly requested the handoff.
- Workers author only in assigned scratch directories, do not run git or change canonical files/tools/docs, do not spawn extra agents, and do not kill shared Wine processes. Main chat owns review/landing/commits.

`work/codex-lane/w22/SESSION_RULES.md` records the final stop and prior operating preferences. Saved prompts and older checkpoints contain historical instructions; this guide and the user's latest messages take precedence.

## Resume points and remaining budgets

| Lane | Authoritative checkpoint | Next work |
|---|---|---|
| Fresh 229, then 228 | `work/codex-lane/w22/agent229/CHECKPOINT.md`, `ATTEMPT_TOTALS_FINAL.tsv` | **00972490**, researched only, zero authored grades; 12 untouched rows remain in 229. Then consult 228's checkpoint; 00a92740 is its noted next untouched row. |
| Parked/skipped 211–225 | `work/codex-lane/w22/retry/followup/CHECKPOINT.md`, `REMAINING_AFTER_WRAP.tsv` | Suggested 0054abe0 (read-only investigation, audit says original SKIP), then other eligible inventory. The 528-row inventory preserves old annotations and is **not** a fresh attempt allowance. |
| Retired parked 196–210 | `work/codex-lane/w22/retry196_200/CHECKPOINT.md` | **00bcc700 paused at 5/10**, five remain. `best/00bcc700.cpp` is complete memberwise-copy source, DIFFER190v192, no fixture claimed. Selective-memcpy variants are unsupported diagnostics, not accepted source. |
| Retired fresh 230, then 231 | `work/codex-lane/w22/agent230/CHECKPOINT.md`, `MEDIUM_ATTEMPTS.tsv`; `agent231/CHECKPOINT.md` | 230: 7 exact, 21 capped, 12 untouched. Suggested 0093c2f0, using the landed stealth sibling. 231: 10 exact, 9 capped, 21 untouched. Reconcile catalog before working. |
| Main's chunk189 retry | `work/codex-lane/w22/coordinator_retry189/*_CHECKPOINT.md` and `*_attempts.tsv` | **00a361a0 paused at 8/10**, two remain. Best actual-header/memberwise-copy source `00a361a0_a6.cpp`, DIFFER189v193, no fixture. 00a57960 and009713e0 are capped at10. |

All ready rows from the final workers are now canonical, including **0094feb0, 006380c0 and00b20fd0**. Their checkpoint text was written before the final main landing and may still say pending. Do not land them twice. Likewise use root's corrected canonical00555390; the parked worker retains an older pre-root-header snapshot.

Budget hazards that must survive handoff:

- 00956b00 has documented **11 historical** grades and00bd27f0 **12**; do not reset them or add attempts.
- 00ed76e0 is already landed. Its historical total is **uncertain**: reservation5 + agent2 + root1 = reserved8, not a proven historical count.
- 006ffe10 is held at10 with shared-header integration unresolved; a q match alone did not make it accepted coverage.
- Newly capped in the final parked lane: **00680a40 and00b21090**, both10. The latter had7 prior grades and3 new, not a fresh ten.
- 00a5f2f0 has7 historical grades in `retry211_215`, hence only3 remain.
- Root00922600,0095a7d0,009612f0,009713e0 and00a57960 are capped10. Earlier coordinator ledgers and preserved probes explain each remaining mismatch.
- Inspect older `agent189..231`, `retry*` and coordinator ledgers before extending any row. Some scratch `src/` directories inherited from interrupted older workers contain DIFFERs; only q evidence plus independent canonical verification establishes acceptance.

## Compiler setup and daily commands

Read `docs/BUILDING.md` (Wine 11 WoW64 section) and the journal. Live environment:

```bash
cd /workspace/FableDecomp
source work/codex-lane/wine/env.sh
export S="$PWD/work/codex-lane"
```

It points to `work/wine-repair/prefix-wow64`, `work/wine-repair/vc71` and `work/wine-repair/wine-11.0-amd64-wow64`. If those binaries/prefix are absent on the new host, follow the documented setup rather than assuming the copied env file installs them.

`tools/decomp_pipeline/linux_lane/patch_wine11_wow64.py` adapts the verified Wine11 host library's LDT setup to x86-64 `modify_ldt` and preserves host TLS. It checks the expected digest and retains a backup. Compiler binaries are unchanged. Use a fresh prefix with `WINEARCH=wow64`; `win64` here was a 64-bit-only prefix and failed to load 32-bit kernel32. Wine requires local Unix sockets, so this cloud shell needed approved execution outside its restrictive sandbox. Do not kill shared Wine services when a compilation pauses.

```bash
timeout 200 python3 "$S/tools/q.py" ADDRESS SOURCE.cpp LEAF
python3 "$S/tools/xdis.py" ADDRESS 0xHEX_LENGTH
python3 "$S/tools/xref.py" ADDRESS
```

The old prompt's `$S/w23/xref.py` path is stale; use `$S/tools/xref.py`. The game executable remains an external dependency at `$S/dl/game/Fable The Lost Chapters/Fable.exe`. The archive contains the text oracles/disassemblies used for current candidates but not the game executable.

Main verification/landing:

```bash
LANDVERIFY_WORK="$S/wine/UNIQUE_NAME" timeout 200 python3 -u tools/decomp_pipeline/verify_and_land.py payload.json oracle.tsv
# Only after the identical payload and oracle independently pass:
LANDVERIFY_WORK="$S/wine/UNIQUE_LAND_NAME" timeout 200 python3 -u tools/decomp_pipeline/verify_and_land.py payload.json oracle.tsv --land
```

For embedded switch tables use `tools/decomp_pipeline/verify_land_jumptable.py`, which reads raw COFF so table bytes are retained. Existing catalog entries need the recheck path, not duplicate landing. Payload rows contain address/module/name/leaf/source_cpp/test_cpp/pass_pattern and optional `flags` arrays (e.g. `/EHsc`). Avoid casually rerunning old finalize scripts: some contain batch-specific expected counts or historical team prose.

## Review and verification rules learned here

- Require exact source embedded verbatim in a standalone behavior fixture, with meaningful callback/ownership/state checks on both host g++ and native VC7.1. A parallel simplified implementation is insufficient.
- Include actual available shared target headers; use narrow documented native views when donor layout differs. Host-width adapters belong only in non-MSVC branches. Do not copy expanded headers into target fixtures.
- Audit call addresses, RTTI owners and real virtual destructor slots: relocation masking can hide incorrect external identity/vtable models. Unused interface slots in methods-only views are different from constructed polymorphic object lifetimes.
- Preserve full-expression temporary destruction, AddRef/Release, hidden return storage, argument evaluation order, reloaded pointers after callbacks, and native unchecked sentinel/precondition behavior. Several apparent scheduling-only failures were fixed by these source details.
- C3DVector/array donor widths often differ from retail; document evidence rather than inventing padding. Selective memcpy or volatile inserted only to steer instruction scheduling is not a proven native source reconstruction.
- q trims padding; the canonical oracle does not silently trim it. Exclude only proven trailing int3 bytes after the return boundary, retain original boundary evidence, and keep switch tables.
- Host flags used where needed: `-fpermissive` for the VC temporary-address extension, `-fcheck-new` for nullable allocation behavior, `-fno-elide-constructors` to exercise counted-pointer copy ownership. These do not replace native target checks.
- Native allocator00cac9d0 needs `/EHsc`; its COM/WinAPI calling conventions and virtual lifetime were audited. UI00555390 is SetDeletion(CDeletion method2), not event unregistration. UI0054e640 is CViewport, while0041a8d0 is CSprite. Shader-labelled006380c0 is actually CThingMapManager::Clear. Ice-troll009627c0 constructs the **Into** action.

The host Wine lane was validated for the compiler and fixtures. No full game runtime/playability or renderer validation is claimed. Continue from the final checkpoints; preserve both the owner's percentage baseline and the accumulated work on difficult rows.
