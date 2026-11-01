# Naming pass wave 20 (2026-11-01): 105 landed rows respelled -- the c686/c687/c688 fixes, the STLport sort / spline / erase rows, FastCompare -> operator<

Inputs: brief `naming/briefs/BRIEF_W20.md`, QUEUE_W20_input.tsv (80 rows: c686 + the c687 / c688 rows appended as those chunks landed), wave 19
DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w20` (AGENT_README.md; scripts/ here); the coordinator copied ov_final (124
files), applied the 46 RENAMES (git mv + build_candidates.ps1), BINDINGS (56 RETIRE / 41 ADD / 12 KEEP, 0 CONFLICT) and LABELS (31), and
rechecked every changed row against HEAD (`logs/coordinator_recheck.log`; agent's own run `logs/recheck_final.log` 105/105, 9 MATCH + 96
RELOCATION_MATCH). `bindings_multi_check.py` 0. Name-only except the re-authored own rows, all byte-identical.

Families, committed in this order (`runs/<fam>.rows.txt`): c686a 8 (CWADBank::AddFile caller, private CTCFlammable::InitFirePoints -- the PDB
name, SetFromVectorAndAngle, the CFlammableDef own rows at MATCH, SetRoomHF / SetRoomLF), sort 4 (the STLport sort bindings incl. the pair copy
ctor at MATCH), spline 4 (CSplinePosParamData on a shared CPSCSplineBase model; TransformFromPosition has three parameters), erase 3, misc 11
(DELTA_NOTIONAL_ZERO x3, GetLength x2, 0x007E6EC0 = GetPFirstThing<CThingFilter_HasDefName> -- DECISIONS_W19 item 6 resolved, CCharString::
operator<, RemapGroups with std::map), fastcmp 36 (FastCompare -> `??MCCharString@@QBE_NABV0@@Z`), c687 20 (the libpk sound-system family), c688 19
(the CLevelLoader::CObject family on engine/CCountedPointer.h, ~CLevelFiles nesting, the vector<CTVertexLandscapeBackground> rows, five map _M_erase
rows, ~CResource on engine/CResource.h).
Queue outcomes (QUEUE_W20.tsv): 43 respelled, 9 partial, 14 already-correct, 14 deferred. Decisions: `DECISIONS_W20_RESOLVED.tsv` (CSoundSystem for
vtable 0x012B1E24, CPKVoiceParams, CDebugStream, the PDB set<unsigned long> copy-ctor form for 0x00AD3A10, GDefStringTable family kept on the
established spelling). Deferred (DEFERRED.tsv, 456 rows): the libpk decision items (now decided -> wave 21), 0x00C73AF0's caller parameter type,
the 0x00AD7400 / 0x00431606 / 0x00AD5B50 STL stand-ins, the StoreSound*Allocator own rows, the wave 19 families (PeekTypeList, HasTC, STLport set
callers, destructor and mega families, cg10) not reached -> wave 21 with the c689+ queue rows.
