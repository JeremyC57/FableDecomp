# Naming pass wave 17 (2026-11-01): 414 landed rows respelled to the proven c675-c681 and ChatGPT pass 10 names

Inputs: brief `naming/briefs/BRIEF_W17.md`, QUEUE_W17_input.tsv (296 NAME_FIXES_QUEUE rows: 520 x1, c675 30, c676 17, c677 24, c678 38, c681 31,
cg10 155 -- the queue grew while the wave ran; the agent re-read it before each step), wave 16 DEFERRED.tsv, the ChatGPT audit NAMING_QUEUE.
One Opus 5.5 agent authored the overlay in `$S/naming/w17` (AGENT_README.md is its own write-up; scripts/ here); the coordinator copied ov_final
(416 files: 414 sources + 2 rewritten tests), applied the 35 RENAMES (git mv + build_candidates.ps1), BINDINGS (110 RETIRE / 32 ADD / 27 KEEP) and
LABELS (46), and rechecked every changed row against HEAD (`logs/coordinator_recheck.log`; agent's own run `logs/recheck_final.log` 414/414,
4 MATCH + 410 RELOCATION_MATCH). `bindings_multi_check.py` 0. Name-only: no byte changed.

Families, committed in this order (`runs/<fam>.rows.txt`): gdt 13 (??_G stand-ins -> compiler-emitted ??_G<Class>@@UAEPAXI@Z, incl. the nine c681
rows), qctor 1 (CQ_DragonBossFightScript ctor 0x00D25410 + vtable binding), stl 21 (real STLport 4.6 instantiations: map<long,COpinionDeed*>, the
009aa780 map<CCharString,NBankFileManager::CContainedBank> siblings, set<unsigned long>, deque<CSetTextInfo>, allocator spellings, iterator traits),
own 4, data 11 (anonymous-namespace orientation constants, g_AudioContext unified, alSourcef), c677 6, queue 16 (CountRemainingGroups,
FindASimplyDelightfulShop, ControlledMove, GetPWorldSeed protected-member mode), w16d 10, cg10 319 (36 of the 155 ChatGPT targets; SetPItem
0x00A01B90 alone 201 rows), c678 3, c681 15 (CMemoryDataOutputStream::PeekData / GetLength callers).
Queue outcomes (QUEUE_W17.tsv): 55 respelled, 42 respelled-partial, 152 deferred, 47 already-correct; 0x00440730 HOLD untouched.
Decisions: `DECISIONS_W17_RESOLVED.tsv`. Deferred (DEFERRED.tsv, 397 rows incl. 229 carried from wave 16, 5 of which resolved here): the
PeekThingSearchTools / PeekTypeList / HasTC re-modelling, the STL stand-in callers and placeholder-class own rows, the PClass singleton 013b89fc and
PMainGameComponent 013b86a0, the destructor families, the mega families (GetPItem, ~CCharString, bitset::test, GetSubDef, ConstantFPS), the c678/c681
own rows that need re-authoring (7 quest-script ctors, Alloc 00cf91c0, 00dec7f0, GetLength 00994b40) -> wave 18 with the c680/c682/c683 queue rows.
