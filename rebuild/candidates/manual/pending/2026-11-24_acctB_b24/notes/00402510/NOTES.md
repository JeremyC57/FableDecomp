# 0x00402510 GFMain -- ready (B24C4 repair, 2026-11-24)
NOTE FIRST: strict vland shows FIVE RELOC_NAME lines, not four: the fifth is ??1CAFile@@UAE@XZ -> 0x0099A300 (ICF-folded with
~CBaseClassNonCopyable), the price of the declaration-only ~CAFile that removes a NEQ pick-any copy. The existing
FABLE_RETAIL_CDISKFILEWIN32_DTOR_DECL_ONLY instead was tried (b24c4/v12.cpp): DIFFER(3926v3952) 21 hunks, so it is not usable here.
RELOCATION_MATCH 3952v3952, /Gr /GS; payload.json + oracle.tsv written by addmatch.py; vt.py PASS; strict vland PASS (vland.log);
pick-any check: 12 COMDAT copies, all EQ (pickany.txt). B24C's notes, header, test and payload: b24c4/old_b24c/.

## Lessons that apply (hand851 NOTES, Codex pass12 verified_pending/00402510, landed GF helper rows)
- Codex matched the body (RELOCATION_MATCH) in its own 88-header world: the body is right; the work is header debt + names.
- The four helpers (GFInitialise 0x004022B0, GFUninitialise 0x00401B80, GFHandleSystemInitError 0x00401C00, GFGetBuildNumber2
  0x00401F30) are landed rows called out of line: declare them, never carry their bodies.
- Inline static setters (CAFile / CThreadedFileManager callbacks, CDefinitionManager flags) are direct stores in retail.
- New (B24C4): data globals are not checked by vland -- b24c4/relmap.py (obj relocations -> retail targets) + reloc_bindings.tsv is the
  check. ghidra_out/egod_carved_names.tsv has Ego_d data publics (class statics too): grep it before calling a data name descriptive.
- New (B24C4): run the pick-any checker BEFORE editing headers; a header-implicit base destructor or an Info-first ReleaseData shows
  up there, not in the grade.

## What changed in the B24C4 repair (all regraded: RELOCATION_MATCH unchanged; grades v10 / v11 in b24c4/ATTEMPTS.log, v12 + final in ATTEMPTS.log)
1. Data globals: every address now spelled as the landed rows spell it (b24c4/rel10.tsv vs rebuild/oracles/reloc_bindings.tsv: 52
   data relocations, one DIFF left = 0x013B864F GDisableCapsLock, unbindable unsigned-char landed symbol, see ESCALATE.md). Respelled:
   g_bFrontEndFlag_137544A / _137548F, g_bParticlesEnabled, g_FeatureA/F/G, g_bStartMainGame, g_bRunFromDVD, g_bRuntimeLevelFiles,
   g_bUseCompiledDefs, g_bUseCompiledSoundSymbols, g_bLeftAlignText, g_PresentationInterval, g_bCompileFrontendDefinitions, and in the
   header opt-ins g_bSaveBinaryDefinitions / g_bCheckDefDependencies / g_bLoadBinaryDefinitions / DebugMessage / HardDiskErrorCallback.
   The addresses with no landed spelling keep the Ego_d spelling (GSoundOn, GResolution*, ..., m_errorCallback@CThreadedFileManager,
   verified one by one in egod_carved_names.tsv; no landed `// 0x..` comment names them either). PDB proofs: NAME_FIXES.tsv.
2. Sound block: `unsigned long Body_38[8]` + reinterpret_cast<float*> replaced by `CSoundManagerInit SoundManagerInit` (new
   engine/CSoundManagerInit.h; type name from the landed leaves 0x00991910 / 0x00A3EF00, member name from landed 0x009A6610). Readers:
   CASoundManager ctor 0x00991910 copies the block; CanAllocateEmitter 0x009915C0 compares the emitter count with the +0x00 copy
   (MaxEmitters); CSoundManagerLH ctor 0x00A3EF00 reads ChannelMask; ResourceMemory feeds CResourceList::SetMaximumMemory. +0x08 /
   +0x0C / +0x10 are float (both writers store float constants). All member names descriptive (no PDB covers the PC sound module);
   Setting04/08/0C/18 are named by offset as in the landed reader rows (no reader of their copies found).
3. Made-up names: B24C's "descriptive" class statics are Ego_d PDB names (see ESCALATE.md); B24C's flag names RenderManagerFlag1Requested
   etc. are gone (landed 0x004017B0 sets 0x013750A4 from cf_UseAlternateRenderTargetClear, contradicting it). DisableVTuneCollection
   0x009D81E0 stays descriptive: no Ego_r / FableWin / Ego_d public, no naming_stragglers signature match (PC-only 17-byte body).
   InitialiseConsoleVariables 0x00413C50 is the landed row's spelling.
4. Opt-ins: the twelve `_GFMAIN` opt-ins and the FABLE_RETAIL_GFMAIN gate are gone. Reused: CDEFINITIONMANAGER (+CRESOURCELIST) and
   _COMPILEDDEFDIR, CTHREADEDFILE_PROCESS_PASS27 (+CTHREADEDFILE), CSTREAMINGFONTBANK_SETUSEFIXEDFUNCTION (extended to the default
   branch under the same name), CCOUNTEDPOINTER_RELEASEDATA, STLP_LIST_DTOR_DECL_ONLY. New, named after what they add: CAFILE_SETCALLBACKS,
   CAFILE_CURRENTPATH, CAFILE_DTOR_DECL_ONLY, CTHREADEDFILEMANAGER_SETCALLBACKS, CDEFINITIONMANAGER_STARTUP_SETTERS,
   CBANKFILEASYNC_SETENABLEFAILUREHANDLING, CFILEINSTALLER_SETIGNORETIMESTAMP_INLINE, CXMVPLAYER_TEXTLAYOUT_SETTERS,
   CDEFSTRINGTABLE_LOADTABLE, CSYSTEMMANAGER_INITIALISE, CGAME_PLAY, CMAINGAMECOMPONENT_GENERATEMETFILES, NGAMETEXT_CDATABANK_CTOR_DECL.
   GGame / GDefStringTable are row-local externs (no header opt-in). New headers (plain include guards, Ego_r layouts): CSoundManagerInit,
   CSystemManagerInit, CInputManagerInit, CDisplayManagerInit, CSystemRegistry, CBankFileManager (STLport containers, not byte pads),
   NUISystem_CIME (Ego_r members, not Body_0[84]). None of the six existed in the repo (checked; only local fable_*.h models).
   Details and evidence: hdr/README.md; generator b24c4/mkhdr2.py.
5. Flags: /Gr /GS confirmed: the same object's landed GFConfigDetection 0x004017B0 is built '/O2 /Oy /W3 /GS /Gr'
   (build_candidates.ps1); 0x00402130 / 0x004023F0 use /Gr; the three GF helper rows (0x004022B0 / 0x00401B80 / 0x00401C00) have no
   CompilerFlags entry (their conventions are spelled in source). GFMain needs /GS (security cookie 0x0139C8A8 at entry).
6. Test: the Codex fixture's "verbatim" body was stale (GAllowWADGeneration, NGlobalConsole::Initialise ...). It now carries the row's
   body exactly (b24c4/test11.cpp = 00402510_test.cpp) with the fixture renamed. Mutations (made in the fixture's copy of the body:
   vt.py compiles the test alone, never the row source): Setting0C 5.0f->6.0f -> FAIL; `if(!g_FeatureF)` -> `if(g_FeatureF)` -> FAIL;
   restored -> PASS.
7. Pick-any (new): B24C's object had 3 NEQ copies (~CAFile 5-byte jmp vs folded 0x0099A300; ReleaseData<CDataBank> Info-first vs
   retail Data-first 0x00403750; ~list<CStringParserCommentBoundaries> 5-byte jmp vs folded 0x004046B0). Fixed with
   CAFILE_DTOR_DECL_ONLY, CCOUNTEDPOINTER_RELEASEDATA, STLP_LIST_DTOR_DECL_ONLY: now 12/12 EQ.

## vland warnings left (strict, vland.log), all explained in ESCALATE.md / NAME_FIXES.tsv
- RELOC_NAME x5: the four manifest labels NAME_FIXES proves wrong or placeholder (0x00415530, 0x009BC8A0, 0x00402130: Ego_r publics;
  0x009D81E0: placeholder -> descriptive) + ??1CAFile@@UAE@XZ -> 0x0099A300 (ICF-folded with ~CBaseClassNonCopyable; GOTCHAS nf104).
- RELOC_ALIAS _exit -> import thunk (expected). No RELOC_CONFLICT / RELOC_IDENTITY / NOT_GENUINE.

## NAME_FIXES.tsv against the 20 lines Account A queued (B24C's file is b24c4/old_b24c/NAME_FIXES.tsv)
- Changed evidence (now Ego_d PDB spellings, not "descriptive"): 0x0138E188 / 0x0138E189 / 0x013CA7D8 (CDefinitionManager statics),
  0x013BCA00 (CAFile::m_harddiskErrorCallback). The graded PDB-spelled form of these class statics is B24C's overlay b24c4/old_b24c/hdr.
- Added: 0x013BC864 (?m_harddiskErrorCallback@CThreadedFileManager, landed HardDiskErrorCallback), 0x0137544A GFullScreen,
  0x013B8648 GOnlyBuildStaticMaps (Ego_d); 0x009D81E0 line reworded (descriptive proposal only).
- Kept: 0x0137548F InitialiseConsoleAtStartup as a descriptive proposal (the row uses the landed g_bFrontEndFlag_137548F).
- Open doubts: CSoundManagerInit Setting04 / Setting08 / Setting0C / Setting18 are offset-named (no reader of their copies found);
  CInputManagerInit.h declares no constructor (no row here constructs one).
