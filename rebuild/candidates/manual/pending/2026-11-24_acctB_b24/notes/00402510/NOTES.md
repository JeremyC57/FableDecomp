# 0x00402510 GFMain -- in progress (B24C)
## Lessons that apply (hand851 NOTES, Codex pass12 verified_pending/00402510, landed GF helper rows)
- Codex matched the body (RELOCATION_MATCH) in its own 88-header world: the body is right; the work is header debt + names.
- The four helpers (GFInitialise 0x004022B0 fastcall, GFUninitialise 0x00401B80 cdecl, GFHandleSystemInitError 0x00401C00 stdcall(ulong),
  GFGetBuildNumber2 0x00401F30 cdecl) are landed rows and called out of line: declare them, never carry their bodies (one address, one body).
- Inline static setters (CAFile/CThreadedFileManager callbacks, CDefinitionManager flags) are direct stores in retail -> inline bodies on statics.
- fable_*.h headers are local models: spelling reference only, never include.
## State
- Baseline (draft unchanged): cl TIMEOUT -- 8 headers the draft includes never reached the repo (hand851 new headers). Copied
  CSystemManagerInit, CDisplayManagerInit, CInputManagerInit, CSystemRegistry, CBankFileManager, NUISystem_CIME into hdr/engine (from
  hand851 00402510_hdr); dropped CThreadedFile.h / NGameText_CTextEntry.h (conflict with current headers; not needed).
- v1: helpers declared, bodies dropped -> 66 compile errors (zs.py = syntax-only compile with full error list).
- OPEN before hand-back: address-suffixed placeholder globals in the draft (StartupConsoleEnabled_0137548f, SaveSoundSymbols_0138dd3b,
  LoadSoundSymbols_013bc9f0, PrepareParticles_01375754, RenderConfigurationFlag1/2, UseFixedFontFunction_013750ab, ClearStartupLatch_009d81e0)
  need real names or NAME_FIXES/ESCALATE entries; several landed bindings are placeholders too.
- Tool note: unset TMPDIR TMP (Wine cl rc -11 otherwise); fg.py shows only the first few SRC_FAIL errors -- use zs.py.
## RESULT: RELOCATION_MATCH 3952v3952 (/Gr /GS), payload.json + oracle.tsv written, vt.py PASS (Codex test), strict vland PASS.
- What made it match: port onto current headers (v4 compiled, DIFFER 3932v3952), then inline CCharString::GetCRC
  (FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE) + inline CDiskFileWin32::IsOpen (_ISOPEN_INLINE) -> 3948v3952, then the one-level
  CIVCountedPointer destructor FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT -> RELOCATION_MATCH (6 grades).
- Names fixed: Codex's NGlobalConsole::Initialise for 0x00413C50 was wrong (0x00419D90 is NGlobalConsole::Initialise, landed) ->
  InitialiseConsoleVariables (manifest label, descriptive); GAllowWADGeneration -> GAllowDataGeneration (landed + the console string
  "AllowDataGeneration" in 0x00413C50); address-suffixed placeholders replaced by descriptive names (NAME_FIXES.tsv).
- vland warnings left (all explained in NAME_FIXES.tsv: the manifest LABEL is the wrong spelling): RELOC_NAME 0x009D81E0 (placeholder label),
  0x00415530 GetLanguageFolder (Ego_r), 0x009BC8A0 SetNoHangulWordWrap (Ego_r + argument GNoHangulWordWrap), 0x00402130
  TestDirForValidFilePermissions (Ego_r); RELOC_ALIAS _exit (import thunk, expected).
- Descriptive (made-up) names in this row: InitialiseConsoleVariables, DisableVTuneCollection, InitialiseConsoleAtStartup,
  ParticlePreparationEnabled, RenderManagerFlag1Requested, RenderManagerFlag2Suppressed, UseFixedFunctionFonts, the overlay statics above.
- Lesson: a 100-error "header debt" row was mostly the old draft's dead helper bodies + stale defines; drop what the row does not call,
  then most opt-ins already exist (230 added this week). fg.py prints only the first SRC_FAIL errors: use a /Zs compile for the full list.
