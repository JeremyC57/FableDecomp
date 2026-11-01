# Naming pass wave 21 (2026-11-01): 165 landed rows respelled -- CSoundSystem, the libpk voice / debug-stream / allocator rows, GDefStringTable, the c689 own rows

Inputs: brief `naming/briefs/BRIEF_W21.md`, QUEUE_W21_input.tsv (26 rows: the DECISIONS_W20 families + the 20 c689 rows appended when chunk 689
landed), wave 20 DEFERRED.tsv. The first agent run stopped early on a usage limit; a second Opus 5.5 agent resumed it in `$S/naming/w21`
(AGENT_README.md; scripts/ here), reusing the first run's hand21 scripts and fixing four defects the first run left (a missing 00c204a0 test, three
tests still linking the old g_DefStringTable stand-in, 0x0054ED90 using a nested NUISystem::CCharString, one address under two names at
0x00C20440 and 0x00C61FB0, the g_DefaultAllocator class key). The coordinator copied ov_final (186 files), applied the 31 RENAMES (git mv +
build_candidates.ps1), BINDINGS (102 RETIRE / 47 ADD / 1 KEEP, 0 CONFLICT; the table equals the agent's runs/reloc_bindings.after.tsv) and LABELS
(34), and rechecked every changed row against HEAD (`logs/coordinator_recheck.log`; agent's own run `logs/recheck_final.log` 165/165, 2 MATCH +
163 RELOCATION_MATCH). `bindings_multi_check.py` 0. Name-only except the re-authored own rows, all byte-identical.

Families, committed in this order (`runs/<fam>.rows.txt`): csound 12 (CAudioOwner -> CSoundSystem for vtable 0x012B1E24; the __stdcall
CreateSoundSystem_00c096e0(CSoundSystemSetup*)), voice 17 (the CPKVoiceParams ctor / setters / Load / SetFilter_00c20440, Play as
CSoundEventManager::Play, the loop rows as one CSoundLoop_00c377f0, 0x00C44D30 as CAmbientEmitter), dbg 11 (0x00C29FC0's callers on PKCSemaphore,
the 0x00C355E0 error stream on CString / CDebugBreak), alloc 7 (the StoreSound*Allocator own rows re-authored; CDefaultAllocator_00c166f0 /
g_DefaultAllocator_0143bb34), misc 2 (0x00CBA8E0 comparator, 0x00C62920's name parameter `const unsigned short*`), gdst 110 (every TU on the
established `?GDefStringTable@@3VCDefStringTable@@A`, except 005dcf70 / 008af850 / 00890f40, deferred; plus the PDB CDefString::SetString
0x009D5060, operator CCharString 0x00415D70, GetStringAsCharArray 0x009AE6B0), c689 6 (the _Rb_tree::_M_erase own rows 0x00613AE0 / 0x006196D0,
the ??_G rows 0x00619F50 / 0x00D3AFF0, __uninitialized_copy 0x00AB0FC0, the std::vector fill ctor 0x00AD30C0, each emitting the spelling its
binder already uses).

Queue outcomes (QUEUE_W21.tsv, 26 rows): 7 respelled, 3 partial (0x013CA828, and 0x00613AE0 / 0x006196D0 whose callers belong to the 119-body
~tree stand-in family), 10 already-correct, 5 deferred (0x00C28690 libpk name unknown; 0x007ACC50 six per-row caller stand-ins; 0x00440990 23 rows
across many hand spellings; 0x00441020 via 00b67ec0; the 0x00D34470 ctor signature and base model), 1 hold (0x00AD3A10). Decisions:
`DECISIONS_W21_RESOLVED.tsv` -- CString for the libpk error stream (the own rows' class; CDebugStream retired), CSoundEventManager for the voice
table (CPKVoiceTable retired), 0x00AD3A10 held as an open tooling item (two retail copies of one symbol), 00b67ec0's three vectors re-modelled
together in wave 22, the ??_G construction-helper pattern accepted on precedent, 00c44f80 per-row in wave 22. Deferred (DEFERRED.tsv, 454 rows):
the wave 20 deferrals not reached (PeekTypeList, HasTC, STLport set callers, 0x00CAF680, the destructor and mega families, cg10) -> wave 22.
