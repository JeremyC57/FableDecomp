# Naming pass wave 18 (2026-11-01): 1,486 landed rows respelled -- the class-static globals the PDB way, DELTA_NOTIONAL_ZERO, bitset::test, ConstantFPS

Inputs: brief `naming/briefs/BRIEF_W18.md`, QUEUE_W18_input.tsv (301 NAME_FIXES_QUEUE rows: c678/c681/cg10 + the c680/c682/c683 rows appended
as those chunks landed + the coordinator's DELTA_NOTIONAL_ZERO decision), wave 17 DEFERRED.tsv, the ChatGPT audit NAMING_QUEUE.
One Opus 5.5 agent authored the overlay in `$S/naming/w18` (AGENT_README.md; scripts/ here); the coordinator copied ov_final (1,518 files: 1,486
sources + 32 tests), applied the 13 RENAMES (git mv + build_candidates.ps1), BINDINGS (153 RETIRE / 12 ADD / 39 KEEP) and LABELS (23), and
rechecked every changed row against HEAD (`logs/coordinator_recheck.log`; agent's own run `logs/recheck_final.log` 1,486/1,486, 1 MATCH +
1,485 RELOCATION_MATCH). `bindings_multi_check.py` 0. Name-only except the re-authored rows below, all byte-identical.

Families, committed in this order (`runs/<fam>.rows.txt`): rfail 4 (the wave 17 recheck failures, tests fixed), qctor 10 (the seven quest-script
ctors on (CScriptDataBase*, CGameScriptInterfaceBase&) re-authored, Allocs 0x00CF91C0 / 0x00DF1080, 0x00DEC7F0), own 2 (0x00994B40 ->
CMemoryDataOutputStream::GetLength const, MATCH, renamed; 0x0074EE90 -> CMainGameComponent::Get()), stl 7 (STLport 4.6 instantiations), pmgc 320
(PMainGameComponent through ?Get@CMainGameComponent@@SIPAV1@XZ), pclass 62 (the private PClass singletons through the CTBaseSingletonNoDefaultAlloc
Get accessor), dstgame 184 (pDSTGame), pgui 155, thingmgr 65 (?GetPCurrentThingManager@CThing@@), cfps 291 (?GetConstantFPS@CMainGameComponent@@SIJXZ),
eps 146 (DELTA_NOTIONAL_ZERO, coordinator decision from chunk 680), bitset 229 (std::bitset<274>::test), gpdm 35 (?GPDisplayManager@@3PAVCDisplayManager@@A),
isqrt 36 (?GFastOneOverSqrtTable@@3PAKA), queue 101 (171 function targets: CTCDParticleEmitter::Create 62, CCodaContext::SetError 14, ...).
Queue outcomes (QUEUE_W18.tsv): 18 respelled, 32 respelled-partial, 188 deferred, 63 already-correct; 0x00440730 HOLD untouched.
Decisions: `DECISIONS_W18_RESOLVED.tsv` (the 12 ConstantFPS rows that read the static directly stay deferred: retail read it from ten unrelated
classes, the access mechanism is unproven). Deferred (DEFERRED.tsv, 437 rows incl. 213 carried from wave 17, 11 of which resolved here; 3
[recheck-fail]: 0x004E34B2 + 0x004DD782 destroy chain, the pgui edit of 0x007E5230): the scope-1 leftovers (PeekThingSearchTools 0x008FD000,
PeekTypeList, HasTC, GetPWorldSeed stand-ins, STL stand-in callers), the destructor families, the remaining mega families (CBasicString::Compare,
GetPItem, ~CCharString, GetSubDef, GetInstance, Transfer<>, AddEvent) and the c680 rows without a mangled final -> wave 19 with the c684-c686 queue rows.
