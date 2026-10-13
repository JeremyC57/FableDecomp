# Gotchas (living list)

One line per solved problem. Moved out of `CLAUDE.md` on 2026-09-07 so the agent guide stays
short; **append here** when you solve something real. Newest at the bottom of each section.

## Ghidra / symbols
- Ghidra `X86FunctionPurgeAnalyzer` on huge binaries (165k fns) goes log-silent for hours in a
  quadratic progress-reporting loop — it's working, not hung; verify with jstack + CPU sampling,
  never kill (headless saves only after analysis completes).
- PDB names can contain whitespace ("dynamic initializer for 'x'") which Ghidra symbols reject —
  `ApplyNames.java` sanitizes `\s+`→`_` before `setName`.
- Preserve MSVC decorated names on the first PDB/BSim `setName` attempt so `DemangleAll.java` still
  works; only fall back to `SymbolUtilities.replaceInvalidChars(...)` after Ghidra rejects a name.
- RTTI vtable-slot ports beat low-confidence BSim guesses when slot counts align; preserve the
  compare TSV, then use `LabelApplyForce.java` and demangle.
- `analyzeHeadless.bat` script args: cmd.exe splits on `=`, so `name=0xaddr` arrives as TWO args —
  pass alternating `name addr` pairs instead.
- Ghidra DB after RTTI force pass: 49,082 functions, 40,187 named, 8,895 default-named. Bulk RTTI
  port source is `ghidra_out/labels_rtti_port.tsv`; conflict audit is `ghidra_out/rtti_port_compare.tsv`.
- Quest logic is compiled C++ (161-entry name→allocator table @ 0x00CD52D0, no quest VM) — see
  `docs/engine/QUEST_SCRIPTS.md#quest-binding-compiled-c-classes`. Trust FSE ASLR addresses over BSim names when they clash (0x00CB8110 is
  the CScriptBase ctor, not "CHeroMorphDef"; 0x00CBFAB8 is SetScriptActiveStatus).
- Manifest module labels are NOT trustworthy per file: Ghidra propagates one name over every
  byte-identical body (2,977 "CLandscapeBackgroundPatch::vector_deleting_destructor" rows are dtors of
  many classes). Use `tools/decomp_pipeline/label_trust.py` (address-unique `(module, leaf)` pairs)
  before attributing a function to a class.

## Decomp harness (`tools/decomp_pipeline`)
- `verify_and_land.py` decodes `&lt; &gt; &quot; &apos; &amp;` in payloads, rewrites `__thiscall`→
  `__fastcall`, and strips every line containing lowercase `static_assert`. Never write the literal
  `__thiscall` keyword (VC7.1 C4234); use `FABLE_STATIC_ASSERT` from `rebuild_abi.h` for size checks.
- Workflow `args` arrive in the script as a JSON **string**, not a parsed value — guard with
  `const items = typeof args === 'string' ? JSON.parse(args) : args`.
- EMBEDDED JUMP TABLES (switch >~4 dense cases) cannot be verified by `verify_and_land.py` (objdump
  splits the body at `$Lxxx` labels and elides the zeroed table). Use
  `tools/decomp_pipeline/verify_land_jumptable.py <land.json> <oracle.tsv> [--land]` (raw-COFF
  extraction). Proven on `CKeyRedefiner::GetSubTypeForAction` 0x557CA0 and `AreAllowedToCoexist` 0x5578A0.
- Manifest boundary OVER-CAPTURE: an oracle span `[addr, next_manifest_addr)` swallows int3 padding +
  the next function's head when the next fn isn't in the manifest. Fix with
  `tools/decomp_pipeline/trim_overcapture.py 0x<addr>` / `--oracle <tsv>`; then author the single fn.
  A VOID member forwarder gets tail-call-optimised to `jmp` by VC7.1 while retail kept `push;call;ret`
  (same-length DIFFER, not recoverable); value-returning and cdecl-cleanup forwarders recover byte-exact.
  Backlog: `rebuild/backlog/overcapture-recovery-worklist.tsv`.
- Shared engine headers (`rebuild/include/engine/<Class>.h`, generated from the PDB layouts by
  `gen_class_headers.py`) are the required way to model `this`; `retype_landed.py` converts landed files
  and re-proves parity. A `char`-vs-`bool` field type change DOES change bytes when the store comes from
  a differently typed parameter (0088e920 went MATCH→DIFFER) — the parity gate decides, never assume.
- Heredoc-fed Python that contains `\b`/`\w` regexes gets its backslashes eaten twice in this shell and
  writes real backspace bytes into the file. Write regex-bearing source with the Write/Edit tools.
- A file named `CON` (or any reserved device name) at the repo root makes every `git add`/`status`
  hang forever on Windows. Delete with `del "\\?\D:\Documents\FableTLC\CON"`; a killed git leaves a
  stale `.git/index.lock` to remove.

## Build / frontend
- WinLibs mingw64 g++ builds can die at startup with 0xC0000139 (entrypoint not found) from runtime
  DLL mismatches on PATH — link `-static -static-libgcc -static-libstdc++` (done in FableForge).
- Adding a D3D9 frontend texture (`visual_boot_d3d9.cpp`) requires registering it in BOTH the
  `FableInitialiseVisualD3D9` upload chain AND the `VisualRender2DAdapter`
  `RENDER2D_ADAPTER_ATTACH_TEXTURE` chain; miss the attach chain and the quads draw FLAT WHITE.
  Free frontend resource ids: 101-120 taken; 116-119 are WAVE; About=120, spooky=121/122.
- Frontend visual QA runs headlessly: `rebuild/build_bootstrap.ps1 -RetailFrontendBank <frontend.big>`,
  launch the VisualCheckpoint exe, synth-click via SetCursorPos+mouse_event, screenshot with
  Graphics.CopyFromScreen. `$PID` is read-only in PS. Recipe: `docs/pipeline/VISUAL_PARITY_STATUS.md`.
- Retail Fable.exe must be CLOSED while building: WinMain's real single-instance mutex fails the
  `FABLETLC_WINMAIN_BEHAVIOR` fixture (code=2).
- Profile-name font doubling (`AppendProfileNameText`, ENG_ARIAL): fixed by a half-texel UV inset in
  commit d0becd0 (2026-08-10, merged to main 2026-09-07). Re-verify visually after any Render2D change.

## Formats / data (see `docs/formats/`)
- Fable name hash = **crc0** = reflected CRC-32 poly 0xEDB88320, seed 0, NO final inversion
  (`CCharString::ComputeCRC32` 0x00404310). NOT `0xFFFFFFFF-crc32`, NOT zlib. A game.bin def APPEND
  resolves only if the names.bin CRC is crc0 AND the payload's self global-entry-index back-refs are
  retargeted (component sub-defs are SHARED). See `docs/formats/DEF_LOAD_CONTRACT.md`.
- 3DAF anim payload = `u32 decompSize` + ONE raw LZO1X stream; chunks are `[fourcc][u32 size]`, one
  XSEQ per bone track. EgoCore (`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`) is the
  whole-format answer key — check it BEFORE byte-RE; do NOT copy its `CDefStringTable::GetCRC` or
  `classIndex=0`. `docs/formats/BIG_ANIM_FORMAT.md` §9, `docs/engine/DEMON_DOOR_FACE.md`.
- Texture payloads: only MIP 0 is chunked-LZO; mips 1..n-1 are RAW. Info+24 (MipSize0) = mip-0 region
  size, 0 = all-raw. DXT3 Info tail is `02 08`. Writer `tools/texture_build.py`; `docs/formats/TEXTURE_WRITER.md`.
- Compiled-mesh `LODSizes[]` are byte SIZES; every retail 1-LOD type-1 entry appends an UNCOUNTED ghost
  LOD; material lists end with a `DegenerateTriangles` sentinel (STATIC meshes only). Skinned weights sum
  exactly 255, max 3 influences. `docs/formats/MESH_COMPOSE.md`.
- Save edits: any SAVED_ENTITIES cell edit must patch the 36-byte cell descriptor AND sectionLen AND
  chunk1_ulen, then re-sign. `tools/save_edit.py`; grammar in `docs/formats/SAVE_ENTITY_GRAPH.md` §9.5.
- Dialogue join: `data\Defs\<bank>snds.bin` = sorted {crc0("SND_"+entryName), soundID}; soundID == .lut
  clip Index == dialogue.big LIPSYNC id. `tools/dialogue_pipeline.py`, `docs/formats/DIALOGUE_PIPELINE.md`.
- Chest facts: `CChestDef::Transfer` is retail 0x004DE204; `OpenerObject` +0x34, `OpenersRequired`
  +0x38; rewards in `CContainerRewardHeroDef::ObjectFamilies` +0x28.
- New-map registration REQUIRES a FinalAlbion_RT.stb common-header chunk (unchecked map lookup in
  OpenRetailStaticMap 0xB41E50 → CTD 0xA2428A). WER Application-log fault offsets +0x400000 = Ghidra VA.
- Region vector is hard-capped at 142 (owner must be ≤141); the ForgeTest crash at 0x7dd1d3 is
  `CEngineLandscapeMap::OpenStaticMap` zero-filling `malloc(field_04)` with no null check.
- Custom terrain textures: foreground triples are GBANK_MAIN_PC entry IDs in textures.big (156
  `UNASSIGNED_*` 512² slots); pipeline `texture_build.py replace` + `forge stb settex`.

## Modding toolchain (lives in FableForge / ForgeFSE now — `docs/modding/README.md`)
- Active build target since 2026-07-18: FableForge (`D:\Code\FableForge`); FQT is a donor, not the target.
- WLD/BWD authoring is `forge wld compile` / `forge world add-level|install-level|attach-map` /
  `forge stb settex`; `forge validate` cross-checks BWD↔WLD↔STB.
- Do not activate a quest during ForgeTest terrain teleports (quest-region abandon modal blocks
  validation). Wasp Menace renders correctly through ForgeFSE, so the base quest API works.
- Mario rig (`work/mario_hero/stage_bindaxis4`): parent-relative ANIM translations stretch the
  segments; mesh-only rest/inverse-bind edits are insufficient. Parked.
- ForgeFSE canonical fork is `D:\Code\ForgeFSE-retail-shadow`; `D:\Code\ForgeFSE` is stale.
- Ubuntu's `7zip` package (dfsg) cannot read RAR5 archives ("Unsupported Method"); use `unrar` (multiverse).
- Ghidra 12.1 release asset is `ghidra_12.1_PUBLIC_20260513.zip`; the GitHub API is blocked from cloud sessions, so probe the download URL directly.
- `LabelApply.java` accepts the whole manifest at once (57k rows, names with `::` and `<>`): 0 failures; it creates functions Ghidra's auto-analysis missed (12,660 on a fresh import).
- A Linux cloud session can author and self-check candidates (g++ syntax, purity, header policy) but cannot grade parity: keep such batches under `rebuild/candidates/manual/batches/` until the VC7.1 box runs `verify_and_land.py`.
- Wine lane: `cl.exe` 13.10.4035 (DDK 3790.1830) runs under wine32 and reproduces catalog MATCH rows; the DDK `<cXXX>` headers have no `namespace std` using-declarations (overlay shims), its CRT libs need `bufferoverflowu.lib` (`export LINK=bufferoverflowu.lib`), and the whole `lib/wnet/i386` tree is too big to sparse-checkout wholesale — name the four `.lib` files.
- `pgrep -f '<pattern>'` inside a bash `until` loop matches the loop's own command line; use `pgrep -x <name>` when waiting for a process to exit.
- VC7.1 codegen rules learned grading the 2026-10-13 batches (122 rows, 118 byte-exact): a bool result written `return cond;` goes through a full register (`xor ecx,ecx ; sete cl ; mov al,cl`), `return cond ? true : false;` gives the retail `sete al`; a member read before a call only stays before the call if it is read into a local first; a `std::vector`/`std::list` element reached through the real iterator OBJECT (`*(begin()+n)`, `*begin()`) is not CSE'd with the size()/empty() loads — model retail containers with the `CRetailVector`/`CRetailList` templates in `rebuild/src/compiled/00/4a/CQuestManager_GetRegisteredQuestFromIndex_004af3c0.cpp` and `.../00/a3/CClassFactory_DeleteAllClasses_00a37130.cpp`; a store that retail re-reads right after writing it means a `long&` parameter of an inlined helper or a const-ref `max` template (Dinkumware `std::max` shape: `left < right ? right : left`, self-store included); `dec eax ; jne` on an argument is a one-case `switch`, not `== 1`; shared epilogues / `and dword ptr [..],0` zero stores / pass-through `push dword ptr [esp+n]` are `#pragma optimize("s",on)` (the lander sweep finds it); `test ah,5 ; jp` is the ordered `<` (`>=` is `test ah,1 ; jne`); a by-value class return with no `add esp,4` after the call is `__stdcall`; callee-saved registers are assigned in REVERSE order of local declaration (declare the local you want in `edi` last); `wchar_t` needs `<stddef.h>` (no `/Zc:wchar_t` in the retail build).
- Wine lane after a container restart: `wineserver -p` (persistent) before grading, or each `cl.exe`/`link.exe` run boots its own server and one row takes ~40 s instead of ~0.5 s (`env.sh` now does it).
- More VC7.1 codegen rules (batch 5, 53/71 landed): a named `float` local for a call result blocks the sibling-call `jmp` (pointer/long locals do not); an 8-byte POD struct returns in `edx:eax`, any user constructor makes it a hidden-pointer return; a `(long, float)` constructor reproduces an x87+register copy; to extract only the plain destructor `??1` from a polymorphic class use a virtual function plus a NON-virtual destructor (a virtual destructor makes the lander grade the deleting `??_G`); `x * 0.5f` (literal) loads `x` first then `fmul [pool]`, `x * g_extern` loads the extern first; `inc dword ptr [mem]` pairs come from an inlined per-object helper; `Helper(a, b)` keeping `this` in ecx is a __thiscall member on `this`; two named locals for `call() + global`; trust `ret N` over the manifest prototype for the parameter count; `#pragma optimize("s",on)` can keep a frame pointer around a tail jump (BirdFlyToFood/BirdGroundWander Initialise).
- A frame-pointer body with `push dword ptr [ebp+8]` and no spills is a size-optimised TU with global optimisation and /Oy both OFF: `#pragma optimize("gsy",off)` then `#pragma optimize("s",on)` at the top of the candidate reproduces it (plain /Od spills, plain "s" tail-jumps).
- Family landing (2026-10-13, scratch `famscan.py` + `gen/fam.py`): grouping the ≤64-byte un-landed bodies by opcode skeleton gives ~4,800 families; one parametrised source per family lands hundreds of byte-identical shapes at once (factories, ctor/dtor shells, Clone, STL node ctors, persist hooks, static initialisers). Rules learned: a by-value enum argument is a plain 4-byte register value (`long`); class names come from `rtti-vtables.tsv` (vtables with no complete-object locator, e.g. 0x012C2F64.., are a no-RTTI module: name them `CVtable_<va>`); a base/member constructor label is trusted only when its (module, name) is unique in the manifest; namespaced RTTI names are used by their leaf in shape templates.
- VC7.1 codegen (family wave): a CCharString temporary passed to a setter must be a named local in its own scope (`{ CCharString name("LIT", -1); Set(name); }`): a bare temporary reuses the constructor's returned `this` in eax where retail re-derives `lea eax,[esp+4]`; `pop ecx` instead of `add esp,4` after a one-argument call is the `#pragma optimize("s",on)` scheduling (the lander sweep finds it, so one template serves both families); a vtable store BEFORE a run of zero stores means the zeroed members live in an embedded member object with an inline constructor (an init list or body assignments sink the vtable store below them); a vtable store AFTER member destructor calls and right before a base-destructor tail call is an inlined empty intermediate-base destructor, and the derived class must be `__declspec(novtable)` (abstract) for its own store to disappear; `xor eax,eax ; cmp ; sete al` is an int/long result (`return a == K;`), a bool result drops the xor; `if (p) delete p;` keeps retail's explicit null test (plain `delete p` emits none); a static std::list initialiser that stores the global LAST builds the node in a local (`node* s = malloc(..); s->next = s; ...; head = s;`), one that re-reads the global between stores assigns `head` first.
- Dynamic initialisers: VC7.1 puts `??__E` initialisers in `.text$yc` and their atexit destructor thunks in `.text$yd`; `verify_and_land.py` now reads every `.text`/`.text$xx` section and its relocations (was `.text` only), so a static-storage object definition (`CCharString g_x("lit", -1);`) lands the retail initializer body. The thunk is the `_$E2` symbol; a thunk that inlines the destructor (Release-shaped) hides the object's address and type.
- Open family DIFFERs (ledgered in `crawl/gen_tried.txt`, see `docs/journal/FINDINGS_LOG.md` 2026-10-13): two-argument ctor storing both members then the vtable (F15, 76 rows: ten source/flag variants all put the vtable store between the member stores); `??_G` with one member destructor then an inlined intermediate-base vtable store (F73, 14 rows); a one-argument forwarder that retail does not tail-call (F148, 9 rows).
