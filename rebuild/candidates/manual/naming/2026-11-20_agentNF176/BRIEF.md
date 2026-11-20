# Name-fix agent NF176 (2026-11-20, round 10): PRIORITY reconciliation, long-tail engine / sound DSP / navigation / misc class naming

Owner (2026-11-19): "start the 3 naming agents on the naming queue". This account (A, branch FullDecomp) does naming and header re-ports; Account B (branch FullDecomp-B) is only landing its 25 reserved parked rows (locks `acctB`). Read `/home/user/FableDecomp/docs/COORDINATION.md` first. Agents NF174 (script / UI / creature mode namespace naming) and NF175 (creature action / AI / hero / sound voice naming) are running; the NF173 hand-back is pending landing (its rows, incl. CNavigatorManager 0x00A157C0 / 0x00500230, are held); all slices are disjoint. Some entries were left unfinished by landed agents: check rebuild/candidates/manual/naming/2026-11-20_agentNF1*/APPLIED.tsv and NOTES.md for an entry before redoing its census.

Read first (all under /home/user/FableDecomp, read-only):
- `rebuild/candidates/manual/naming/2026-11-17_agentNF92/` BRIEF.md, LESSONS.md, NOTES.md (the last round; your brief is that brief adapted);
- `rebuild/candidates/manual/naming/2026-11-16_agentNF44/BRIEF.md` sections "Kinds of entry", "Lessons of earlier passes", "Disk",
  "Round-3 / Round-2 / Round-1 lessons", "Per entry", "Hard rules", "Lane", "Hand-back". They apply unchanged EXCEPT where this brief
  overrides them (Lane tools and the GameCODA rule below);
- LESSONS.md of `2026-11-17_agentNF90`, `_agentNF89`, `_agentNF88`;
- `docs/pipeline/GOTCHAS.md`; `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md`; `docs/pipeline/ASM_RULES.md`.
- The batch README behind an entry's tag: `land1119<x>` = `rebuild/candidates/manual/batches/2026-11-19_*<x>*/README.md` (+ its
  NAME_FIXES.tsv); `land1118eg` / `land1117fu` / `land1118h1` = the 11-17 / 11-18 landing READMEs that queued them (grep the tag in docs/ and batches/).

## Your slice ($S/w22/agentNF176/)
- `ENTRIES.tsv`: your open queue lines (q id = queue line number - 1; count them yourself). **`REFS.tsv` is the binder list:** for each
  entry address, every landed row whose retail body calls / jumps to / holds that address (a Fable.exe scan,
  `python3 $S/w22/nftools/refs.py`). `WORKLIST.tsv` (from reloc_bindings.tsv) is partial: that table keeps ONE witness row per symbol.
  Data reached at an offset (+4 ...) and virtual calls are not in REFS: grep sources for those. `rows.txt`: rows locked as NF176 in
  `$S/w22/nflocks/` (self rows + WORKLIST binders; claim REFS rows before you touch them). `HELD.txt`: entries that touch a row
  Account B holds: record them `deferred: held by acctB` and do not touch those rows.
- The land1119 entries (q11350+) were copied from today's batch NAME_FIXES.tsv files whose formats varied; a few carry `(raw ...)`
  evidence with no separate correct_symbol column. Read the source batch file and settle what the claim is before acting.
- Some entry addresses are not landed rows (catalog-only or parked). For those there is no self row to port: fix the catalog name
  if two sources prove it (put it in NAME_FIXES.tsv of your hand-back with address, correct_symbol, evidence, wrong spelling) and respell
  any landed binders. Never author a new byte match: that is Account B's work (record `blocked: authoring`).
- Rows that need a row outside rows.txt: claim it first
  (`mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF176 > $S/w22/nflocks/<addr>/owner; true`). If it is held, record `deferred: held by <owner>`.
  Any row listed as `handed off` or `reserved (acctB` in `rebuild/candidates/manual/UNMATCHED_ROWS.tsv` is Account B's: never touch it.

Do the entries in the order your specifics give (else queue order), one entry to a terminal state before the next.

## Rules carried from NF92 (unchanged)
- **Every row ends `ported`, `not-a-binder` or `blocked`** (no `carry`). An entry ends done, done-already, rejected, blocked (reason, rows
  left) or deferred.
- **Two sources** for every claim; a claim that does not hold goes to REJECTED.tsv with the reason. Do not re-probe the whole slice.
- **Budget (round 10, advisor):** no family grade cap. Count failed forms per row: after 3 failed forms on one row mark it blocked with
  the exact compiler error or diff. A row that models a dozen classes locally is blocked/port after 3 failed forms; name the classes that must move first.
- **Transformer first, then batch:** prove a shared rewrite by hand on 2-3 rows, script it, grade the rest in one payload of 20-30 rows.
- **Static asserts:** never move a `FABLE_STATIC_ASSERT` to another line. After editing an overlay header run
  `python3 /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py /home/user/FableDecomp/rebuild/include $S/w22/agentNF176/hdr <header>`
  and say in hdr/README.md what it pinned. Base copies of overlaid headers go in `orig/` (outside hdr/). Edit the existing hdr/ copy;
  never re-create it with a fresh cp.
- **Overlay headers carry `#ifdef` opt-ins only**, member-specific macro names (`FABLE_RETAIL_<CLASS>_<MEMBER>`); the default path must be
  token-identical; say so in hdr/README.md. The other two agents may add opt-ins to the same headers: read their overlays
  (`$S/w22/agentNF9x/hdr/`, read-only) before adding a member so it is never declared twice. A RENAME of an existing shared-header member
  is a coordinated `header` item: do not apply it; record it in NOTES.md as "needs header claim" with the rows it unblocks.
- No new local model of a class that has a shared header, in a row or a test (renaming a local stand-in to the real class name counts:
  the nf95b landing held 29 rows for it); no cast addresses; no `__asm` / `naked` / `_emit` / `register` / bare
  `__forceinline`, `volatile` only where retail used it; no `#error` probes; no `Unproven` / `VirtualNN_` / `FUN_` placeholder names.
- If you change a row that EH funclets include, regrade those funclets. A self rename moves the parent's file: list such funclets in NOTES.
- **Write STATUS.tsv and APPLIED.tsv after EVERY row.** Containers restart without warning; the first grade after one can time out while
  wineserver starts (retry once).
- Count before you report: every number comes from your files.

## Changed since NF92 (2026-11-19)
- **GameCODA:** the GameCODA 2.10 SDK was found legitimately on archive.org (`docs/BUILDING.md` "GameCODA 2.10 audio SDK";
  `docs/pipeline/LIBRARY_SIGNATURES.md`). Its CodeView libs (`$S/sdk/gamecoda/tree/CODA/.../CODAApiDbg.lib`, read-only) are now a valid
  name source. This supersedes NF44's Round-2 "GameCODA claims are rejected" lesson. A CODA name still needs a second source (the body,
  the callers, vtable slots). Never copy SDK files into your hand-back.
- **Unreachable end:** `__assume(0)` only as an `_MSC_VER`-guarded unreachable-end marker with an `#elif defined(__GNUC__)
  __builtin_unreachable();` twin (EXTRA_RULES).
- **Header users:** after any overlay change, list the landed includers of that header
  (`grep -rlE '<header name>' /home/user/FableDecomp/rebuild/src/compiled`) and say in hdr/README.md that your opt-ins change none of them.

## Lane (overrides NF44's Lane section: the old $S/w22/nftools set is gone; use these)
Start every Bash call with:

    export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S; cd /home/user/FableDecomp

- **Callers:** `python3 $S/w22/nftools/callers.py <addr>` (catalog name, self source, landed binders and the symbol each binds).
- **Grade one row:** `S=$S/w22/agentNF176/pS timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`
  (flags from the row's `// Flags:` line, else the CompilerFlags of its entry in rebuild/build_candidates.ps1 minus /O2 /Oy /W3; add
  `FABLE_HDR_OVERLAY=$S/w22/agentNF176/hdr` when you use your overlay). `python3 $S/w22/nftools/xref.py <addr> <obj> '<leaf>'` shows the
  symbol each relocation binds.
- **Grade + test (required for every row you hand back):**
  `python3 $S/w22/nftools/mkpay.py $S/w22/agentNF176/work/b1 $S/w22/agentNF176 <addr> <addr> ...` (takes rows/<addr>.cpp and
  rows/<addr>_test.cpp when present), then
  `LANDVERIFY_WORK=$S/w22/agentNF176/wd1 FABLE_HDR_OVERLAY=$S/w22/agentNF176/hdr timeout 1800 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/w22/agentNF176/work/b1_payload.json $S/w22/agentNF176/work/b1_oracle.tsv --recheck-landed > $S/w22/agentNF176/work/b1.log 2>&1; true`
  Must say MATCH or RELOCATION_MATCH and PASS. Run long ones in the background and poll the log; a fatal cl error under Wine can spin.
- **Binding pairs:** tools/decomp_pipeline/reloc_identity.py `check()`.
- **Disk:** delete each `wd*` directory once its log is saved; never copy large trees. On "no space left", stop and report.
- One compile at a time, always inside `timeout`. Every Bash call must exit 0 (`; true`). Quoted heredocs (`<<'EOF'`) for any text with
  backticks. Absolute paths only. Never run git (not even `git status`). Never write under /home/user/FableDecomp. Never land anything.

## Hand-back (in $S/w22/agentNF176/)
- rows/ (finished rows only; blocked drafts stay in work/); hdr/ and hdr/README.md;
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence` (0x-prefixed
  addresses; `name` without `Class::`), then a `# Section 2` line with caller respellings (caller row, old symbol, new symbol, callee address);
- STATUS.tsv (address, change, grade, test, grades_used, entries, note: one line per row you touched or listed);
- APPLIED.tsv (one line per entry in ENTRIES.tsv: entry, address, status, binders respelled, note);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside your rows (catalog-only names, rows
  held by others, new findings);
- REJECTED.tsv, NOTES.md (mark descriptive names), LESSONS.md;
- recheck_final.log: one vland --recheck-landed run over every changed row.

**Long slice.** If your context gets heavy before the slice is done, finish the current row, make sure every file above reflects what
is finished (APPLIED lines `open` for entries not started), run recheck_final.log over the rows changed so far, and report: the
coordinator lands what is done and resumes you. Report briefly: entries done / done-already / rejected / blocked / deferred / open; rows
ported, not-a-binder, blocked; header opt-ins; rule slips.

## Rules since round 9 (advisor decisions, owner delegated; apply them)
- **No new name splits.** A hand-back may leave an existing split (one address bound under two spellings by landed rows) but must never
  create one. If you rename an address (self leaf, callee or data), EVERY landed row in REFS.tsv for it must end on the new spelling in the
  same hand-back, or the rename waits. The coordinator gates each landing with an exe scan of all referencing rows (`$S/w22/nftools/gate.py`).
  A family (rows that must move together to avoid a split) hands back whole or stays blocked.
- **Same-TU strong callee copies** are legitimate when retail's caller keeps a register live across the call; the copy uses the PDB spelling,
  the owner's body verbatim, and `// Fidelity: same-TU strong definition of 0x... (retail TU-mate)` (GOTCHAS nf96a line).
- **In-place respelling** inside a local model the landed row already had is accepted; a NEW local model (or a stand-in renamed to the real
  class name) is not. A partial shared-header mode (e.g. `FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY`, which declares only `NThingMap::CLayerInfo` and
  `LayerInfo[]`) is accepted when it lets a row drop a local declaration.
- `__assume(0)` only in the guarded EXTRA_RULES form with an `// Inferred:` line.

## NF176 specifics: long-tail engine / sound DSP / navigation / misc class naming
Entries (ENTRIES.tsv, 55), from the PRIORITY pick list (several queue lines may name one address: one unit each):
  q3886 0x009D58A0 ?Close@CBankFileAsync@@UAEXXZ
  q3968 0x013CAA54 ?desc@?4??GetSectionDesc@CCodeSectionManager@@AAEAAUCSectionDesc@2@K@Z
  q4080 0x0099A330 ?IsValid@CInitBaseClass@@QBE_NXZ
  q4103 0x0129CD78 ??_7CMeshData@CPhysicsMeshInfo@@6B@
  q4178 0x00C68F90 ?SetEffectProperty_00c68f90@CEaxContext@@QAEJKKPBXKPAPAUSEaxEvent@@@Z
  q4181 0x00C5F150 ?CalcDifference@CImaAdpcmCodec@@AAE_KPAE0IIK@Z
  q4255 0x0047A9F0 ?AddEntry@CHeroLogBook@@QAEXABVCWideString@@00W4ECategory@CHeroLogBook
  q4261 0x008688F0 ?IsExplosionStrike@CHitParameters@@QBE_NXZ
  q4305 0x00A3B9D0 ?Init@CVideoSys@@QAEJABVCWideString@@PAVCTexture@@_N@Z
  q4312 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
  q4357 0x009E9390 ?Close@CMemoryFile@@UAEXXZ
  q4404 0x00C4D740 ?ALGetProcAddress@CSalOpenAL@@QAEPAXPAE@Z
  q4455 0x00519EF0 ??$__destroy_aux@PAVCSwitchableBlockage@CNavigationAreaUpdate@@@std@@Y
  q4569 0x00C2A890 ?SendStateAndReset@CNavigatorSwitch@@QAEXXZ
  q4572 0x00F3EA50 ?GenerateTriangleListAndStrip@CTriangleStripifier2@@QAEXAAV?$CArray@G@
  q4611 0x0041A060 ?GetLevelDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
  q4658 0x009BA330 ?ReadAsStringUntilString@CStringParser@@QAE?AVCCharString@@ABV2@@Z
  q4797 0x009E5090 ?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@J@Z
  q4827 0x00F36010 ?Get2DNoise@CPerlinNoise@NFractal@@QBEMABVC2DVector@@@Z
  q4835 0x00A567A0 ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z
  q4883 0x00A9D480 ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z
  q4901 00c25c60 ?GetSample@CSampleRef@@QAEPAUCSoundSample@@XZ
  q4920 0x00C465D0 ?GetCount@CRegionMap@@QBEJXZ
  q4978 00a01b50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
  q5003 00a3a840 ?DecompressStream@CCompressorZlib@@QAEKPAXKAA_N@Z
  q5009 00a475c0 ?FindNearestNavigableNode@CNavigatorBase@@IBEPBVCANavQuadTreeNode@@PBV
  q5012 00a01b50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
  q5013 00a01b90 ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z
  q5022 00a8ff50 ?ConvertToVBFormatNonBump@CAnimatedBlock@N3DPrimitive@@AAEXPAVCTVertex
  q5058 00c6b720 ?Reset@CStateVariableFilter@@QAEXXZ
  q5059 00c78150 ?UpdateTapDelays@CReverbProcessor@@QAEXPAUSReverbState@@PBUSReverbPara
  q5064 0x00993EB0 ?WriteSLONG@CDataOutputStream@@QAEXJ@Z
  q5119 00c74850 PROPOSAL ?SetShelves@CEffectFilter@@QAEXMMMMK@Z
  q5127 0x00864580 ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z
  q5152 0x00A76770 ?GetLeafNodeAt@CNavQuadTreeNode@@MBEPBVCANavQuadTreeNode@@ABVC2DVector
  q5153 0x00C3A7D0 ?ApplyGainToFrame@CRampedParam@@QAEXAAUCCodaFrame@@K@Z
  q5167 00c44dc0 ?UpdateVolume@CAmbientEmitter@@QAEXXZ
  q5170 0x00C89350 ?ProcessRamped@CReverbDiffuserStage@@QAEXPAUSDiffuserStageParams@@PBU2
  q5172 0x00C11F00 ?AcquireEvent@CSoundEventManager@@QAEPAUCSoundEvent@@K@Z
  q5187 0x00C3A8F0 ?IsSettledAtZero@CRampedParam_00c3a8f0@@QAE_NXZ
  q5188 0x00C483A0 ?Attach@CRegistry_00c38b20@@QAEXJPAX@Z
  q5190 0x00C48860 ?Take@CMailbox_00c48860@@QAE?AUSDecoderRequest@@XZ
  q5203 006159a0 ?TransferIn@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@@@QAEX
  q6188 0x0042DBEE ?ResetAttractModeTimer@CNewFrontendGameComponent@@QAEXXZ
  q6205 0x00BEAE50 ?Load@CWaterPatchMesh@@QAE_NPAVCDataInputStream@@@Z (landed 0x00BEB490
  q6253 00dcec50 ?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z (landed rebuil
  q6303 009a1140 ?GlyphLookup@CStreamingFontBankX@@QBEPAUCStreamingGlyphLookup@@G@Z
  q6323 0138306c ?CurrentCreatureGlobalDefNum@CTCSpecialAbilities@@2JA (landed 0x0072AB
  q6334 0092f310 Interaction_EvaluateAndAct (manifest forge-autoname); ?OnCreate@CComba
  q6396 0073a690 ?GetTimeToBlendIn@CAnimEntryLite@@QBEJPAVCThing@@@Z
  q6492 00411570 strcmp_signed (landed global_global_00411570.cpp) ?Compare@?$CBasicStr
  q6503 01375748 ?g_PlayAnimationDefaultFlag@@3_NA (row 00d08790); ?g_bScriptAnimationF
  q11718 0x00669580 ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointe
  q12068 0x00669580 ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointe
  q12070 0x005EA3C0 ??$GetSubDef@VCTavernGameDef@@@CDefClassBase@@...
Census the referencers first (REFS.tsv), move wrong binders in families, and hand back partial work with each entry's remaining binders
listed exactly. Read the shared headers' opt-ins first. WORKLIST.tsv has the wrong binders the table shows. Rows locked to you: rows.txt;
lock more as the brief says. Locks whose owner reads `acctB` are Account B's reserved rows: never touch them.
- **Self renames on referenced rows (coordinator, 2026-11-20):** a leaf-only self rename on a row that landed rows reference is fine when
  every referencing row already binds the new spelling (the address ends on one spelling, nothing grows). Record the census; no need to ask.
- **No `FABLE_FORCEINLINE` in a shared header opt-in (coordinator, 2026-11-20)** unless a per-row `/d2inlT<N>` / `/d2inlS<N>` flag sweep
  failed first and the failure is recorded (EXTRA_RULES "Owner decision, 2026-11-08", order of preference).
- **Base lists of local models (coordinator, 2026-11-20):** a row-local model of a class that HAS a shared header is never re-based: port
  the row onto the header. A row-local class with NO header may keep its token and sit on a shared base (narrow rule). Acceptance test for
  every row: objdump -t defined-symbol diff, landing vs HEAD object: only the row's own leaf, shared-header inlines / pick-any COMDATs, and
  implicit members of header-less local classes that nothing binds.
- **Tests:** rewrite a test on the shared headers, not as a standalone host model of a class that has a header.
- **Local class models (advisor decision, owner delegated, 2026-11-20):** a "base-list local model" is a row-local definition
  `class|struct X [: bases] {` of a class X that HAS a shared header (verify_and_land.py `_DEF_RE` + `shared_header_redefinitions`); never add
  one. Before defining any class a row did not define at HEAD, grep `class X` / `struct X` across ALL of rebuild/include (mode classes live in
  files named otherwise, e.g. NCreatureMode_CModeSitFloor.h holds CModeSitBase): any hit means use that header. A new local model is allowed
  only when all three hold: no header for the class exists anywhere under rebuild/include; a landed sibling row already uses the same shape;
  its base comes from a shared header. List each such row in NOTES.
- Leaf-only self renames of rows outside your entries are accepted when both PDBs agree and the exe scan finds no landed row referencing
  the address; list them in NOTES.
- A new PDB spelling replacing a stand-in on a callee is accepted when the address's spelling count does not grow; list each in NOTES.
- **Flags:** if a row needs flags its build entry lacks, put them in a `// Flags:` line AND list the row in NOTES "Flags lines".
- **cpp precheck** before every grade (a missing include spins cl); grep your overlay first.
- RENAMES section 2 caller column: 0x-prefixed addresses.
- **Context:** hand back early; after every 2-3 finished entries make every hand-back file current.
- **Self-check before you hand back (required):** `python3 $S/w22/nftools/selfcheck.py $S/w22/agentNF176 > $S/w22/agentNF176/selfcheck.log`.
- Copy your orig/ and hdr/ bases from the repo working tree when you start. Read the other two agents' `$S/w22/agent<NAME>/hdr/`
  (read-only) before adding an opt-in to the same header.
- Claim a lock with plain `mkdir` (never `mkdir -p`, which does not refuse an existing lock).
- tools/ holds helper scripts earlier agents wrote; read one before you use it (st.py reads stdin when given `-`).
- **Lock marker:** an owner file reading `released` means FREE: claim it (write your name). Claim new locks with plain `mkdir` (never `mkdir -p`).
- **In-place respells of an existing local model (advisor decision, 2026-11-20):** changing the namespace or template-ness of a class the row
  ALREADY defined locally at HEAD (same class token) is accepted, provided (1) the token check passes (no class token the row did not
  define at HEAD), and (2) the row's object defines no symbol naming the respelled class beyond its own leaf (list them with objdump; a
  symbol another landed row owns or the shared header defines inline means one-definition clash: hold the row). Turning a namespaced
  stand-in (NGameText::CCharString, N006bebf0::CWideString ...) into the real class token is NOT in-place: it renames a stand-in to a class
  that has a shared header (nf95b): port the row onto the header instead.
- **Self signatures:** a row's own parameter list changes only with two sources (both PDBs, or a PDB + the body); one source = blocked.
- **Header opt-ins on widely used headers** (fable_string.h, CThing.h ...): add-only, member-specific macros are fine; changing an existing
  member (access, signature) is a coordinated header item: record "needs header claim" in NOTES.
- tools/ holds helper scripts earlier agents wrote; read one before you use it (st.py reads stdin when given `-`).
