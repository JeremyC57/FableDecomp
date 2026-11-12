# 2026-11-12 landing dj -- naming agent FAM09: 307 of the 399 `_static_init::g_*` placeholder rows get their PDB names

Source: `naming/2026-11-12_agentFAM09/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, NAME_FIXES, ROWS, hdr README).
FAM09 redoes the static-initializer family that the container restart lost. The lost Anniversary 360 Shipping PDB is replaced by the
owner-supplied Test PDB, repacked with lane_tools/anniv/repack4k.py. It is used only as a third source.

- **307 rows renamed.** Each object gets its exact Ego_r.pdb + FableWin.pdb `dynamic initializer for '<name>'` name, mapped by the
  initializer's literal and by TU order. Sixteen of them are class or namespace statics: CLevelLoader::LoadAllSectionName /
  GlobalSectionName, CWideString::NullString, CScriptThing::NullString, CDefinitionManager::CompiledDefDirectory,
  CSymbolMap::CurrentFilename, the CModeBase NO_FRAMES_* longs, NFile::CurrentPath, NGlobalConsole::* and more. Names repeated across
  rows (32 REACTION_MANAGER_DEF_INSTANTIATION_NAME copies, the water sine/cosine pair, and others) are per-TU copies of file-local
  statics, one object per address.
- **41 binder rows and 15 tests** that read those objects under the placeholder spelling now use the real name (RENAMES section 2).
- **92 left as they are** (REJECTED.tsv): atexit-only CStatic, list and tree rows whose types are not modelled; objects built through
  stand-in classes (CCRCSymbolMap, CShaderStats, CThreadedFileManager, ...); libpk objects with no PDB name; five unproven scalars.
- **Partial:** 0x0121E1F0 CTCWife::MarriageEnabledMaleVoice is renamed, but its binder 0x008330D0 keeps the placeholder because it
  has a local CCharString model.

**Headers** (opt-ins only; every line added, none removed): CUserProfileManager.h PROFILENAME, CLevelLoader.h GLOBALSECTION,
CTCWife.h MALEVOICE, CWeaponDef.h TOOHEAVY, CTCCreatureModeManager.h CMODEBASE_NOFRAMES, CDefinitionManager.h COMPILEDDEFDIR,
CSymbolMap.h CURRENTFILENAME.

**Verification:** all 348 changed rows recheck RELOCATION_MATCH, test PASS (`logs/recheck.log`). naming/rebind.py: removed 489,
added 480. bindings_multi_check stays at its baseline, and funclet_includes_check finds 1317 checked, 0 broken.

**Follow-up:** FAM09's NAME_FIXES.tsv (97 descriptive aliases such as g_EmptyWideString = CWideString::NullString, ~50 files) went
straight to naming agent NF26, which is running, so they were not added to NAME_FIXES_QUEUE.
