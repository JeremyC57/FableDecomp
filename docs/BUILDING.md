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

## The RTM compiler on the Linux lane (cl 13.10.3077)

Retail `Fable.exe` was compiled by cl 13.10.3077 (1,369 C++ and 51 C objects) plus five objects (4 C, 1 C++) by the 4035
QFE and linked by link.exe 7.10.3077 (Rich header, FINDINGS_LOG 2026-10-20; prodid labels corrected 2026-10-30 in
`tools/decomp_pipeline/rich_header.py`: 0x5A/3077 is the linker, 0x5E/3052 is cvtres 7.10.3052 for the `.res` object, and
the six 0x5D/4035 stamps are import descriptors of 4035-built import libraries, not compiled objects). The Linux lane originally had only the DDK 4035 build; the free
"Visual C++ Toolkit 2003" turned out to be a pre-RTM 13.10.3052 (tested and discarded). The real 3077 comes from the
VS .NET 2003 disc (archive.org item `visual-studio-2003-unpacked`, `VisualStudio2003.zip`, SHA-1
`298398e607a8e6033abe261987b94cab8fb57c24`, already unpacked): copy `Vc7/bin` to `$S/wine/vc71rtm/bin`, add
`mspdb71.dll` and `msobj71.dll` from `Common7/IDE`, symlink `include` and `lib` to the DDK tree, keep
`LINK=bufferoverflowu.lib`. **Since 2026-10-20 it is the lane default** (`FABLE_VC71=$S/wine/vc71rtm` in
`$S/wine/env.sh` / `env.sh.template`); the 4035 build stays reachable as `--qfe` (`VC71_QFE=$S/wine/vc71`), matching
the Windows layout. The switch was made after the full parked-row sweep (1,079 rows) and a 3,400-row landed-catalog
re-grade under 3077 showed no compiler-caused difference anywhere except the `/GS` prologue. What 3077 changed: the
parked `/GS` rows whose stack-cookie prologue 4035 could not reproduce now match (24 landed,
`batches/2026-10-20_linux_gs_rtm3077` and `_b`); the inventory frame-slot
and zero-register rows, the inline-once destructor band and the biased /GS frame (00581b33) do not change, so those are
source-formulation questions, not compiler-build ones.

**Row compiles define `_USE_ANSI_CPP` (owner, 2026-11-26).** `verify_and_land.parity_of` (every lane row compile: vland, recheck,
nf.compile_one, pk, name_gate ...) and `build_candidates.ps1`'s row compile add `/D_USE_ANSI_CPP`. Under the lane's /ML, VC7.1's
`<new>` -> `<exception>` -> `<xstddef>` -> `yvals.h` -> `use_ansi.h` writes `/DEFAULTLIB:"libcp"` into every row object that reaches it;
retail was /MD, so that directive is a lane artefact. NF468 compiled all 57,160 landed rows both ways: 12,441 differ only by that
`.drectve` token, 44,713 are identical, no grade changes (`pending/2026-11-26_nf461_handback`, NF468 evidence in the coordination log).
Behaviour-test compiles do **not** get it (a test calling `new(std::nothrow)` needs libcp at link), and a `/MD` / `/MT` row keeps the
directive the header picks for it.

## VC10 SP1 on the Linux lane (evidence only: cl 16.00.40219.01)

Ego_d.pdb and FableWin.pdb were built with VC10 SP1 (compiland records `16.00.40219`; EGOD_FINDINGS.md), so their line tables are
only comparable byte for byte with a VC10 compile. `S=$S bash tools/decomp_pipeline/linux_lane/fetch_vc10sp1.sh` fetches Microsoft's
"Visual C++ 2010 SP1 Compiler Update for the Windows SDK 7.1" (KB2519277, `VC-Compiler-KB2519277.exe`, 126,890,440 bytes, sha256
`46497a249dac03f8a6d6b4f4a737546246752ca751ae16c34127a1d784232645`) and copies the x86-native cl / c1 / c1xx / c2 / clui plus
mspdb100 / msobj100 / msvcr100 / msvcp100 into `$S/wine/vc10sp1/bin` (no Windows SDK ISO needed), and VC10's include tree (msiextract,
apt `msitools`) into `$S/wine/vc10sp1/include` plus a declaration-only `stubinc/ammintrin.h` (missing from the package). Then
`python3 tools/decomp_pipeline/linux_lane/lane_tools/odall10.py [--lib fablelib|bbb|engine|particle|ego] [--stl] <src.cpp> <proc substr>...`
compiles with the compile line of the Ego_d library the function belongs to (S_ENVBLOCK; they differ: FableLib / BBBLibrary /
FableEngine / ParticleSystem use `-GS- -GR -Gr`, `-W4` or `-W3`, their own defines; only the game project's few objects use
`-GS -GR- -Gd -D_STLP_DEBUG`; default `fablelib`) and prints the bytes per source line. Without `--stl` it uses the lane headers (VC7.1 CRT
+ rebuild/include), so a text on the repo's engine headers compiles and its non-STL statements compare exactly. Ego_d's library STL is
VC10 Dinkumware (checked iterators), which the repo's STLport model cannot stand in for: measure STL statements with `--stl` (VC10 headers
first) in a standalone text. Calibration (2026-11-21): AUTH2's 0x00A4EFC0 helpers all equal Ego_d (SetScaling 34, GetMaximumAbsoluteComponent
259, GetBoneName 49, C3DVectorWord::operator== 86, SetupIdentityFlag 97; VC7.1 /Od gives SetScaling 30, GetMaximumAbsoluteComponent 227, GetBoneName 31); `--stl --lib bbb` reproduces
all 13 `std::vector<unsigned long>` members of Ego_d's `BBBLibrary\Debug\lib_bank_file.obj` byte for byte (ctors 106 / 94,
operator= 33, _Assign_rv 334, insert<_Vector_iterator> 205, _Insert 911, swap 222, ...). **Never build, grade or land a row with it**:
rows are VC7.1 3077. Never commit its binaries or the package.

## Third-party source trees

`bash tools/decomp_pipeline/libsig/fetch_lzo108.sh` (LZO 1.08, public) puts the vendor .asm that `lib_verify.py` proves 0x00C069D0 from under `$S/thirdparty/lzo/lzo-1.08`; `FABLE_THIRDPARTY` overrides the directory. `docs/pipeline/LIBRARY_SIGNATURES.md`.

Every vendor tree the `// Vendor-library:` / `// Vendor-proof:` rows are proven from (zlib 1.2.1, LZO 1.08, libogg
1.1.1, libvorbis 1.0.1, and the DirectShow base classes of the DirectX 9.0c SDK Summer 2004) is listed with its URL,
sha256 and flags in `tools/decomp_pipeline/libsig/vendor_libs.tsv` and fetched by
```
python3 -I tools/decomp_pipeline/libsig/fetch_vendor_libs.py --dest $S/thirdparty --dl $S/sdk/dl   # needs 7z for the DX SDK installer
export FABLE_THIRDPARTY=$S/thirdparty
export FABLE_WINDDK_INC=$S/wine/winddk/3790.1830/inc/wnet   # windows.h set for strmbase (this is the default)
```
Objects are cached under `$FABLE_THIRDPARTY/obj` (`FABLE_VENDOR_OBJ`). Never commit a tree, an installer or an object.

### GameCODA 2.10 audio SDK (Sensaura, 2004)
Fable's sound layer (CSal* / CCoda* / reverb / filter code around 0x00C3xxxx..0x00C8xxxx) is Sensaura **GameCODA 2.10**. Codex pass 23 used it without
recording where it came from. It was found again on 2026-11-19 on the Internet Archive, item
**[`game-coda-2.10-pc`](https://archive.org/details/game-coda-2.10-pc)** ("GameCODA 2.10 PC", Sensaura Ltd, 2004, uploaded 2026-08-10; it does not show
up in ordinary web search, only in archive.org's own search). That item holds an installer zip and an unpacked 7z:

```
mkdir -p $S/sdk/gamecoda/dl && cd $S/sdk/gamecoda/dl
curl -L -o gc210.7z "https://archive.org/download/game-coda-2.10-pc/GameCODA%202.10%20PC.7z"
#   44,321,413 bytes  md5 42cd21b0cb8c2cc2a0a53f02f39b4c32
#   sha256 f5014ed0becfc4efc01866adc989c1837583fbd210ab3aed28999463c3e6c2d9
mkdir -p ../tree && cd ../tree && 7z x ../dl/gc210.7z -xr'!Media'      # -> $S/sdk/gamecoda/tree/CODA (~74 MB)
```

**Contents (CODA/):**
- `lib/Win32/CODAApi.lib` (release) and `CODAApiDbg.lib` (debug, with CodeView names, locals and scopes). These carry the objects pass 23 cited:
  ReverbFns_SSE.obj, ReverbDspModule.obj, FloatFilter.obj, SalPlatform.obj, ...
- CAGE and Toolkit libraries, plus their C++ source (`CAGE/src`, `Toolkit/src`).
- OpenAL32 / ALut / Network libraries; `include/`, `doc/`, `License.pdf`.

**Handling:**
- Read-only reference, kept outside the repo. Never commit its files: the license has not been reviewed for redistribution.
- How rows use it, and when a row byte-identical to it may take the library grade: EXTRA_RULES 2026-11-19 and `docs/pipeline/LIBRARY_SIGNATURES.md`
  "GameCODA 2.10".

## Era SDKs (fetched, not yet the lane default)

`docs/pipeline/SDK_NEEDS.md` identifies the SDKs retail was built against. `fetch_sdks.py` fetches Microsoft's own
files from archive.org, checks every hash (SHA-1 per file for VS 2003 `Vc7/include`, `vs2003_include.sha1`; SHA-1 +
SHA-256 for the DX SDK installers; the item SHA-1 for the 13 PSDK cabs) and unpacks them under `$FABLE_SDK`:
```
python3 -I tools/decomp_pipeline/libsig/fetch_sdks.py              # all four (~750 MB of downloads; needs 7z)
python3 -I tools/decomp_pipeline/libsig/fetch_sdks.py --env        # the export lines below
export FABLE_SDK=$S/sdk
export FABLE_VS2003_INCLUDE=$FABLE_SDK/vs2003/include   # VS .NET 2003 RTM Vc7/include (Dinkumware 313, ivec/fvec/dvec, comdef)
export FABLE_DXSDK_SUM2004=$FABLE_SDK/dxsdk_sum2004     # DX 9.0c Summer 2004: Include, Lib (strmiids.lib), DirectShow BaseClasses
export FABLE_DXSDK_APR2005=$FABLE_SDK/dxsdk_apr2005     # DX April 2005: Include (d3d9/d3dx9/dinput/dsound), Lib/x86
export FABLE_PSDK=$FABLE_SDK/psdk_feb2003               # Platform SDK Feb 2003: include/, lib/ (x86 import libraries)
```
Nothing reads these yet: `$FABLE_VC71/include` stays the WinDDK `inc/crt` set until a full landed-catalog re-grade
under `FABLE_VS2003_INCLUDE` (a 50-row sample was grade-neutral) lets the coordinator switch it. The Platform SDK
cabs store files under MSI keys (`User32_Lib.95CE7DF8_...`); the tool renames them by key stem and keeps i386
libraries only (conflicts in `psdk_feb2003/RENAME_CONFLICTS.txt`).
