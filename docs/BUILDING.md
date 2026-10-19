# Toolchain & exact commands

*Everything needed to reproduce the RE environment. Adapted from Fable2RE's TOOLCHAIN.md, minus
the Xbox/recomp parts (not applicable to a native x86 exe).*

## Target binary
- `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe`
- **PE32, x86-32, ImageBase `0x400000`**, 6 sections (`.text` 14.8 MB, `.rdata`, `.data`, `.idata`,
  `_PDATA`, `.rsrc`). **No packer / no SteamStub** — verified this session. Ghidra loads it directly.
- Origin: PC build of the Xbox codebase (`_PDATA` section + `titleid 4D530937` in `Fable.exe.cfg`).
- Era compiler: ships `msvcr71.dll` / `msvcp71.dll` → **MSVC 2003 (VC7.1)**. Use this when compiling
  oracle/test functions to match codegen (Phase 4).

## Ghidra
- Ghidra **12.1 PUBLIC** — installed at **`D:\Subuwu\tools\ghidra-public`** (verified: this is the
  real path; the `D:\ghidra_12.1.2_PUBLIC` in Fable2RE's docs is stale). Java 21 (Temurin). The
  REPlugins were built for 12.1.2 → their `extension.properties` version is edited to `12.1` to load here.
- **Extensions:** install `REPlugins/GhidraMCP` (from `D:\Documents\Fable2RE\REPlugins\GhidraMCP`) —
  HTTP MCP server on **port 8089**, ~249 RE-automation tools for AI-driven decompile/rename/xref.
  **Do NOT install XEXLoaderWV** — that's Xbox-only; TLC is a native PE Ghidra reads natively.
- **Headless import** (verified command, run 2026-07-17):
  ```
  D:\Subuwu\tools\ghidra-public\support\analyzeHeadless.bat D:\Documents\FableTLC\ghidra_proj FableTLC ^
    -import "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe" ^
    -max-cpu 12
  ```
  Log → `D:\Documents\FableTLC\ghidra_out\analyze.log`. Ghidra auto-detects `x86:LE:32:default` +
  Windows compiler spec. Full auto-analysis of the 14.8 MB `.text` takes a while (not "minutes").
- **Enable the RTTI analyzer** (MSVC RTTI is intact → recovers many C++ class names/vtables).
- **Seed real names/types:** `python tools\fse_import\fse_manifest_to_ghidra.py` → in the GUI,
  `File > Parse C Source` on `tools\fse_import\fse_api.h`, then *Analysis > Apply Function Datatypes*.
- **Verify GhidraMCP:** `curl http://127.0.0.1:8089/check_connection` (expect ghidra_version 12.1.2).

## Ghidra scripting suite (`tools/ghidra_scripts/`, copied from Fable2RE)
Point Ghidra's Script Manager at this dir (Script Directories → add). Key scripts:
- `LabelApply.java` / `LabelDump.java` — TSV-driven mass (re)naming; `labels_*.tsv` in `ghidra_out/`
  is the reproducible source of truth for the DB (git-track the TSV, not the `.rep`).
- `FindStrXref.java` / `FindStrRaw.java` / `FindRefs.java` — string→function pinning (primary anchor).
- `FindLuaNatives{,2,3,4,5}.java` / `FindLuaLibs.java` — locate the game's own Lua binding tables.
- `DecompFuncs.java` / `DisasmRange.java` / `DumpFuncsInRange.java` — batch decompile/disasm to files.
- `FindAccessors.java` / `FindAllCodePointers.java` / `FindCodeAddrTaken.java` — vtable/accessor discovery.
- `ParsePdataInBlobs.java` / `DumpAllPdata.java` — exploit the `_PDATA` section for function bounds.
- Probes: `WhatIs.java`, `NameAt.java`, `ReadStr.java`, `ReadMem.java`, `ListSyms.java`, `FindValueInData.java`.

## Linux verification lane (Wine + DDK 3790.1830 compiler) — added 2026-10-13
The parity oracle can run on a Linux host that has the retail `Fable.exe`: the VC7.1-family
compiler build 13.10.4035 ships inside the Windows Server 2003 SP1 DDK (community mirror
`github.com/9176324/WinDDK`, tree `3790.1830`), and `docs/pipeline/QFE4035_COMPILER_GATE.md`
records that its code generation equals the RTM 13.10.3077 used for the landed catalog. Proven on
2026-10-13: eight catalog `MATCH` functions re-graded `MATCH`/`PASS` through this lane.
```
apt-get install wine32:i386 wine binutils          # objdump must list pe-i386 (`objdump -i`)
bash tools/decomp_pipeline/linux_lane/setup_vc71_wine.sh /path/to/lane   # ~700 MB sparse checkout
source /path/to/lane/env.sh                        # FABLE_ROOT/FABLE_VC71/FABLE_OBJDUMP/FABLE_WINE, LINK=
FABLE_EXE=/path/to/Fable.exe python tools/decomp_pipeline/crawl/next_smallest.py 16 batch
python tools/decomp_pipeline/verify_and_land.py payload.json oracle.tsv [--land] [--recheck-landed]
S=<scratch> FAMFILE=$S/batch/families512.json python tools/decomp_pipeline/families/fam.py . <family> <batch> - <template>   # shape families
S=<scratch> python tools/decomp_pipeline/families/funclets.py . <batch>   # EH unwind funclets of landed /EHsc parents
```
Notes: the DDK's `<cstdio>`-style headers lack the `namespace std` using-declarations, so the
setup overlays thin shims (`write_std_shims.py`); the DDK CRT libraries need
`bufferoverflowu.lib` (set via the linker's `LINK` variable); Wine path spelling (`Z:\...`) is
handled inside `verify_and_land.py` when `FABLE_WINE` is set. The toolset lives only in the lane
directory and must never be committed.

## Live analysis (native — no cdb guest math needed)
- **x64dbg** (32-bit `x32dbg`) or WinDbg to breakpoint engine functions and confirm signatures live.
- **FSE Lua probes** — fastest dynamic check: write a small quest/entity Lua (via FSE or FQT) that
  calls a suspected API and observe `FSE/FableScriptExtender.log`. The FSE runtime log already prints
  `g_fableBase` (`0x400000`) and vtable addresses.

## FSE source & manifest
- Source: `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\` (read, don't rebuild
  unless extending). Hook site `0xCDB355`; `g_fableBase`-relative pointers = engine call targets.
- Manifest → header/roadmap: `python tools\fse_import\fse_manifest_to_ghidra.py` (see §Ghidra above).

## auto-re-agent (Phase 4, optional)
- `D:\Documents\Fable2RE\auto-re-agent` — AI+Ghidra parity-checked function reversing (produces
  compileable C). See its `docs/getting-started.md` / `configuration.md`.

## FSE address extraction
- `powershell -ExecutionPolicy Bypass -File tools\ExtractFseAddresses.ps1` reads the local FSE source's
  literal `ASLR<...>(0x...)` engine addresses and writes:
  - `refs\fse_engine_addresses.tsv` (all extracted function/data/vtable addresses)
  - `ghidra_out\labels_fse_addresses.tsv` (conservative function labels for later `LabelApply.java`)

## Lua tooling (`tools/lua_mod/`, copied from Fable2RE)
- `luadis.py` / `luadis51.py` — Lua 5.0/5.1 bytecode disassembler (confirm TLC's Lua version first).
- `script_index.py`, `bnk_repack.py`, `apply_mod.py` — script catalog + archive repack patterns.

## Background automation (know it exists)

Two hidden Windows Scheduled Tasks rewrite files in this tree without you asking:

| Task | Installed by | Cadence | Touches |
|---|---|---|---|
| `FableTLC Rebuild Refresh` | `tools/InstallRebuildRefreshTask.ps1` | every 15 min | `rebuild/manifest/*`, `rebuild/COVERAGE.*`, `README.md` metrics block, `ghidra_out/fable_engine.h`, root scratch sweep |
| `FableTLC Local Parity Queue` | `tools/InstallLocalParityTask.ps1` | queue-driven | `rebuild/build_candidates.ps1`, landed sources (auto-landing) |

If a file changed under you, check `rebuild/refresh.log` first. Disable while doing manual
catalog/manifest surgery:

    schtasks /Change /TN "FableTLC Rebuild Refresh" /DISABLE
    schtasks /Change /TN "FableTLC Local Parity Queue" /DISABLE

and `/ENABLE` afterwards. Both defer while another pipeline PID file is live (see
`rebuild/README.md`, "Regenerate").

## Relocation-target identity check (added 2026-10-19)

`verify_and_land.py` now runs `reloc_identity.check` on every MATCH/RELOCATION_MATCH before the
behaviour test. It recovers each relocation of the graded function as (object symbol, retail
target address) from the object file plus the retail bytes and rejects the row (`RELOC_IDENTITY`)
when one symbol is bound to two retail targets, one target to two symbols, or a bound import thunk
(`MSVCR71.DLL::malloc`, `operator_new[]`, ...) is not what the symbol names. Disagreements with a
Ghidra label (`RELOC_NAME`) or with a binding already recorded by a landed row
(`RELOC_CONFLICT`, table `rebuild/oracles/reloc_bindings.tsv`) are warnings; set
`LANDVERIFY_STRICT_RELOCS=1` to reject them too. Funclet slices are not checked.

Catalog-wide audit (resumable, hours): `python3 tools/decomp_pipeline/reloc_audit.py
[--addrs file] [--out rebuild/oracles/reloc_audit.tsv] [--limit N]` recompiles each landed row
with its recorded flags and writes the findings per row; `grep RELOC_IDENTITY` on the output is
the list of landed rows that name the wrong callee, global or float pool.
