# Chunk 856 -- expert retry of ten parked near-miss rows

Result: **0 matched, 10 parked.** Every parked source is the earlier chunk's best source, unchanged except for lines 2-3
(differ/<addr>.cpp + differ/<addr>.test.cpp; both tests PASS under g++ and VC7.1). No row moved: every variant either
reproduced the parked bytes exactly or got worse. Attempts (real grades, DIAG probes excluded; every grade is in STATUS.tsv
and tmp/attempts.log):

| row | name | grade | md | chunk 856 grades | kind |
|---|---|---|---|---|---|
| 0x00D33730 | NScript::CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main | DIFFER(3292v3292) | 0 (1 masked byte) | 103 | per-site ~CTimer inline split |
| 0x00DDAE40 | NScript::CQ_PrisonRaceScript::Main | DIFFER(1180v1180) | 2 | 102 | x87 fstp scheduling |
| 0x0050A650 | CWorldMap::GetMapNavigationAreaInit | DIFFER(2233v2233) | 6 | 100 | register colouring tie |
| 0x00AAD650 | CWaveLoader::LoadWaveFile | DIFFER(1575v1574) | 8 | 100 | register colouring tie |
| 0x00E799F0 | NScript::CV_ExposeMayorScript::CEM_Rhodri::Main | DIFFER(2580v2580) | 8 | 57 | STRUCTURAL (forbidden device proven) |
| 0x00D3A4A0 | NScript::CQ_FireHeartScript::CFH_Prisoner::Main | DIFFER(1140v1140) | 13 | 67 | register colouring |
| 0x00EF4B40 | NScript::CPersonalScript_EmptyScript::CTestPerson::Main | DIFFER(1252v1268) | 13 | 54 | INLINE-BUDGET park (/d2inlT30) |
| 0x00EC4A00 | NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main | DIFFER(1524v1540) | 15 | 51 | per-site CScriptThing copy split |
| 0x005253F0 | CThingManager::LoadGameState | DIFFER(2263v2263) | 21 | 51 | register choice in one loop |
| 0x00E72630 | NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main | DIFFER(2236v2252) | 22 | 88 | dead store + loop-head alignment |

All ten names were already right (LABELS.tsv: Ego_r + FableWin name sets plus the vtable / caller evidence); no NAME_FIXES.
No descriptive names were introduced. The only ledger correction: **0x00AAD650's extent is 1574 bytes** (+ the int3 that follows
the noreturn `_CxxThrowException` call), not the 1840-byte ledger row, which runs into the vector<short> COMDATs at 0x00AADD80;
graded here with a 1575-byte cut (bin/gr.py `CUT`), the coordinator should recut the oracle row.

Tooling written for this chunk (bin/): gr.py (grade one source against the chunk oracle, md + masked-byte count, logs every
grade), batch.py (spec of exact-text edits, 2 grades at a time), sweep.py (per-site mechanical variants: do{}while(0) /
block round each statement, `== true` at each T-check, direct-init / const locals, while->for, postfix ++, !(a==b)), rdis.py /
rexe.py / odis.py (retail and built disassembly), mt.py (compile any TU with the overlay and print one function: used for
compiler-behaviour micro probes), st.py (STATUS.tsv from the log).

## 0x00D33730 CTheRealGuildmaster::Main (1 masked byte)
Shape and levers as chunk 838 (generated entity script, TRetVoid returns, CTimer block, `for (;; NSF) { if (T) return; ... }`).
Residue: the talk branch's first T-return (jne at 0x00D33D4A) targets the loop-exit expanded ~CTimer (0x00D343B9); retail
targets the pad that calls ~CTimer 0x00CD4470 (0x00D343E6). Per-site cleanup inlining: loop-top return expanded, aside / talk /
hit returns called in retail; ours expands the talk one too and cross-jumps it into the loop-top copy.
Tried (103): diagnostic /d2inlS0..S20 and /d2inlT50..T100 (none reproduces retail: not a pure budget park), QFE-4035; ~CTimer
body spellings (local game pointer, forceinline, plain inline members), operator<= / compare spellings for the aside timer,
loop / break / wait-loop forms, talk-check spellings, a do{}while(0) round every statement of the function and `== true` at every
thread check (sweep). Probes (DIAG) showed the choice follows block frequency: a duplicated loop-top T-check makes the talk
return call ~CTimer (p9), removing the talk block makes the hit return expanded (p5).

## 0x00DDAE40 CQ_PrisonRaceScript::Main (2 lines)
Residue: x87 store placement. Retail: `call [edx+0x28]` (GetAngleXY), `mov ecx,[esp+0x20]; cmp ecx,ebp` (the next GetPos null
test), then `fstp [esp+0x18]`; ours stores right after the call. Our VC7.1 never sinks the store in this row: GetAngleXY x
GetPos inline cross product (IsNull / Get() / operator-> / ternary / reversed / r-local / forceinline), (float) / *1.0f /
double-local spellings, call-site angle / pos / hero locals, StartPos scope and reference forms, /G5 /G6 /G7 /GB /Op (DIAG), QFE.
The parked 0x00E03F70 (pending/2026-10-23_linux_hand319, classified x87-order) has the same sunk store at 0x00E04064 and
0x00E044CF; landed rows with expanded GetAngleXY all push the result directly (`push ecx; fstp [esp]`), so none is an oracle.

## 0x0050A650 CWorldMap::GetMapNavigationAreaInit (6 lines)
Residue: after the region_exits loop the vector free loads _M_start into ebx (retail eax); because the loop iterator was
initialised from the same slot into ebx, the empty-loop edge is threaded past the reload (je 0x0050AE63, retail 0x0050AE5F).
A colouring tie on the dtor temporary. Tried (100): last-loop body forms (named CNavigationPosition, C2DVector / layer / pos
locals, CThing&, no p_thing, const p_thing, direct-init), iterator forms (iterator / const_iterator, predeclared, while, postfix,
reversed !=), every scope-block removal (correctly paired; an earlier mis-paired batch is marked DIAG invalid in the log),
all-loop spellings, statement sweeps; DIAG overlay probes of the STLport `_Vector_base` dtor (single null test in deallocate,
plain `if (p) free(p)`) change nothing.

## 0x00AAD650 CWaveLoader::LoadWaveFile (8 lines)
Residue: the `fmt ` chunk's FormatTag and BlockAlign reads lower `StreamPos + 2` as `mov ecx,[ebp+4]; add ecx,2` in retail,
`mov eax,[ebp+4]; lea ecx,[eax+2]` in ours (the other reads match, including the loop reads that use the lea form in retail
too). Retail uses the coalesced form at only 6 sites in the whole exe (0x00AAD808, 0x00AAD960, 0x00BE7FCF, 0x00A2F650,
0x00BE8227, 0x00BFA7AE). The landed C3DMeshStats::LoadBinary 0x00A2F2D0 reproduces it under our compiler (mt.py) inside its
high-pressure volume loop, but a reduced copy of that loop does not, and no bare-read micro probe does: the form depends on
global register pressure, not on the Read spelling. Tried (100): field locals then struct, ReadType / Read(&field) mixes,
discarded AvgBytesPerSec, reference to format, format declared at top / before WAVE, RIFF / WAVE temporaries, loop forms,
local types (unsigned int / long / size_t / int), const and direct-init locals, data-branch spellings (switch, sample / byte
locals, ReadType, loop forms, result field order), flags /Oa /GS /Ow /Ot /Gr, statement sweeps; DIAG header probes of
CDataInputStream::Read (an IsValidPosition helper as in the landed 0x00A2F2D0 model, the positive range test, `remaining >=
size`, five newPosition spellings) are all byte-identical.

## 0x00E799F0 CEM_Rhodri::Main (8 lines) -- structural park
Residue: the chunk 822 nested predicate-dtor case (retail expands ~CIsThingConscious and calls the nested ~CScriptThing
0x004AA840). **Proven**: an `inline_depth(0)` pragma round the row-local `inline ~CIsThingConscious() { }` gives
RELOCATION_MATCH (probe e79_p1, DIAG) -- an owner-forbidden device, so this is chunk 835's open owner question, not a spelling
gap. Plain C++ (57): forceinline / implicit / in-class predicate dtor (an experimental CIsThingConscious.h opt-in, removed:
all expand the nested ~CScriptThing too), dtor spelling and definition order, statement sweeps.

## 0x00D3A4A0 CFH_Prisoner::Main (13 lines)
Register colouring at the seh_Prophet[MyCageNumber] element address (`mov ecx,[esi+0x20]; shl ecx,4; lea edi,[edx+ecx]` v
`mov edi,...; shl edi,4; add edi,edx`) and ParentClass in eax v ecx. Tried (67): element spellings, statement sweeps.

## 0x00EF4B40 CTestPerson::Main (13 lines) -- inline-budget park
Per-site STB ctor split (retail expands seh_me's ctor, calls 0x007E72A0 for seh_hero). With the implicit ctor (OUTOFLINE_CTOR
dropped, variant v1, md 113) the **diagnostic /d2inlT30 gives RELOCATION_MATCH**; /d2inlS2..S20 and T40..T60 do not. Per the
brief that is an inline-budget park. Plain C++ (54): seh_hero spellings, statement sweeps.

## 0x00EC4A00 CRTFE_RockTroll::Main (15 lines)
Per-site CScriptThing copy split at the two CSpawnedFunc1 spawns: retail expands only the WatchForRockTrollKilled argument
copy and calls 0x004ABE90 for the Hit argument and both Par1 member copies. Implicit copy (g): every copy expands; under
/d2inlT40 the length is right (1540) but md 168, so no budget setting reproduces it either. Tried (51): named / functional-cast
/ static_cast / reference arguments, a named CSpawnedFunc1 pointer, statement sweeps.

## 0x005253F0 CThingManager::LoadGameState (21 lines)
Text NPC-name loop register choice: retail keeps the UID low dword in ebx and reloads `this` into esi; ours hoists `this` into
ebx. Tried (51): uid const / direct-init / predeclared / cast, map reference hoisted, self pointer, while / count-down / long
loops, def_name / name / line spellings, the outer && condition, statement sweeps.

## 0x00E72630 CDF_ThreeSuits::Main (22 lines, 16 bytes short)
Entry dead store `mov [esp+0x3c],ebp` and the two outer loop heads unaligned in ours (retail aligns 4 loops: acquisition, main,
and two wait loops; ours only the two wait loops). Tried (88): plain returns, block-scoped filter, forceinline
CWideScreenMagicPauseEntities ctor, g_fZero, main-loop for(;;)/while(true)/for(;!T;NSF) forms, acquisition loop forms,
statement sweeps. The landed DemonDoors siblings with an aligned acquisition loop (CDF_SummonsCreatures 0x00E74D70 without an
inline_depth pragma; several others with one) are the reference to compare against.

## Header debt
None from this chunk (hdr/README.md: the overlay only carries pending copies from chunks 834-853 unchanged).
