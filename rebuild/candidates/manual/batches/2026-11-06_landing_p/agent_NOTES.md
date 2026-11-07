# chunk 815 notes

Six unnamed gap-scan entity Mains, all named from two sources (the only .rdata reference is slot 1 of the entity vtable the
row's Alloc / ctor stores, and the PDB spelling is in both Ego_r.pdb and FableWin.pdb): LABELS.tsv. No descriptive names.

| addr | name | result |
|---|---|---|
| 0x00D58490 | CQ_GuildTrainingMeleeScript::CTheRealGuildmaster::Main | RELOCATION_MATCH (src/ + test/, g++ and VC7.1 PASS), 5 attempts |
| 0x00DFE0F0 | CQ_TraderConflictGoodScript::CTraderToRescue::Main | parked DIFFER(9501v9501), 59 md lines, 10 attempts |
| 0x00E50FD0 | CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main | parked DIFFER(10006v9885), 521, 7 attempts (blocker 1) |
| 0x00E33EA0 | CV_ArcheryCompetitionScript::CAC_Owner::Main | parked DIFFER(9303v9228), 648, 5 attempts (blocker 1) |
| 0x00E5FD40 | CV_BodyGuardScript::CBodyGuard::Main | parked DIFFER(9535v9381), 887, 4 attempts (~CScriptThing budget split) |
| 0x00D973D0 | CQ_HobbeCaveScript::CBanditSacrifice::Main | parked DIFFER(9883v9821), 2115, 3 attempts (frame slots + blocker 1) |

"md lines" counts relocation-only lines too (pDSTGame / pGlobalScriptDef / vtable / string immediates show as `[0]` in the build).

## 0x00D58490 CTheRealGuildmaster::Main (matched)
Shape: generated entity script (T-check at the start of each nested block and after each WAIT loop); comment loop while the hero
walks up, the attack / block cut scenes with counted attacks and blocks, the Thunder + Whisper battle graded A+..F from
pGlobalScriptDef->GUI_MeleeGrades[0..6] (first threshold the score reaches), retake question, then remove Whisper and create the
apprentices. Key spellings: row-local inline ~CScriptThing (VC expands the first temporary only, as retail), inline CTimer
ctor/dtor + forceinline operator=/GetTime, IMPLICIT_COPY, `(EarlyHitWhisper && WhisperArrived) || MsgIsHitBy(...)`, end healths as
named locals before the score. Callees and their evidence are in the source header (lines 3-19). New binding:
AddLogbookTutorialEntryPC 0x00CBEA81 (`void __fastcall (const CCharString&)`, the PC twin of the bound AddLogbookTutorialEntry
0x00CBE9EE, called in the same place with the PC text key).

## 0x00DFE0F0 CTraderToRescue::Main (parked, closest)
Residue after 10 attempts: retail cross-jumps the whole TRADERB body (T-check + push) into the TRADERC body (`jne 0x00DFEE05`); ours
keeps B's own T-check (`je` / `jmp` to the exit) plus three alignment pads; everything else is relocation-only. Tried: separate
ifs, goto into the shared block, a bool selecting B/C (frame moves), TRetVoid returns, RemoveThing duplicated per branch (fixed the
teleporter tail), explicit returns. Header: CQ_TraderConflictGoodScript.h opt-in (CTraderToRescue).

## 0x00E50FD0 CBB_BeardyBaldyMan::Main (parked, blocker 1)
Retail expands the by-value `Me` copy for the CSpawnedFunc1 argument (vptr 0x01238C8C store + refcount inc at 0x00E5101E) and
calls 0x004ABE90 for `Par1(par1)` inside the expanded CSpawnedFunc1 ctor (0x00E51090). Implicit copy: both expanded; explicit
row-local inline copy: both expanded; plain-inline (not forced) CSpawnedFunc1 ctor: identical; copy defined after Main: identical.
The row-local inline ~CScriptThing made it worse (retail calls 0x004AA840 at all 27 sites). Callees: GetRandomSpeech 0x00E53CB0
is CBB_BeardyBaldyMan's (NAME_FIXES), IsHeroWearingAnyBeard 0x00E53670 (NAME_FIXES). New header CV_BeardyBaldyScript.h.

## 0x00E33EA0 CAC_Owner::Main (parked, blocker 1)
Same split twice: CIsThingAlive(Me) ctor expanded with its member copy called (0x00E33EC8), the spawn argument copy expanded and
Par1 called. Best = explicit inline copy ctor (648 lines); inline ~CScriptThing 821, both 858. Frame 0x1FC v 0x1F8. Callee name
fixes: ResetRandomSpeechTime 0x00E337C0 is bound as CQ_AmbushTradersScript's in reloc_bindings.tsv (NAME_FIXES); GetRandomSpeech
0x00E362B0 needs its scope (NAME_FIXES). Header opt-in in CV_ArcheryCompetitionScript.h.

## 0x00E5FD40 CBodyGuard::Main (parked)
Shape decoded completely (for-hire loop: hostile 20 s on hit, attention line when targeted, hire conversation with too-expensive /
too-many / question; hired loop: override-state park, stand-and-wait hold, anger warnings LOW/MEDIUM/HIGH with +30 s, health state
from (long)GetHealth against HenchmanHealthCritical/Low and the HEALED / HEALTH_* comment, freak-out / unsheathe / sheathe with
pause = min(distance * 0.5, 3.0), fire question, heal / summoner comments, state-group change comments, MADE_REDUNDANT and break).
Residue: retail expands ~CScriptThing for 2 of the 8 CScriptThing temporaries and calls it for 6; ours expands 4 (row-local inline
dtor) or 0; the duplicated return tails are cross-jumped differently. Text keys are "TEXT_QST_016_BODYGUARD" + Me.GetDataString()
+ "_" + suffix (operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600,
operator const char* 0x0099E4C0 for Speak). MakeHenchmanComment(text, 1, HM_COMMENT_SINGLE, Me) at every comment site.

## 0x00D973D0 CBanditSacrifice::Main (parked)
Shape decoded completely (see the row's line 1). Residue is mostly stack-slot assignment inside the same 0xC8 frame (retail
seh_me +0x2C, temp-flags +0x20, unused CCharString +0x24, call-for-help timer +0x1C, follow-comment timer +0x18; ours +0x34, +0x24,
+0x20, +0x18, +0x1C), which shifts nearly every line, plus blocker 1 (first SetIsPushableByHero by-value copy expanded, the
CIsThingAlive copy and the later three called). Scoping the three bools in blocks changed nothing; an explicit inline copy expands 2
copies (retail 1). Callees: MakeBanditComment 0x00D905C0 (bound), CheckIfGotTreasure 0x00D99A30 in a CSpawnedFunc (vtable
0x012D567C), IsDistanceFromThingToPositionOver 0x00CBE45C (bound), FadeOutAndKillEntity 0x00CBE716 (M_N1 spelling).

## Labels to correct
See NAME_FIXES.tsv: 0x00E53CB0 (CAC_Owner::GetRandomSpeech -> CBB_BeardyBaldyMan::GetRandomSpeech), 0x00E362B0 (scope),
0x00E53670 (IsHeroWearingBeard -> IsHeroWearingAnyBeard), 0x00E337C0 binding (CQ_AmbushTradersScript -> CV_ArcheryCompetitionScript),
0x00CBE716 second binding (_NM1 is wrong), 0x0143E90C (six spellings). Also bound elsewhere and seen while checking:
0x0099F570 has an extra ?sub_99f570 binding and 0x0099E4B0 / 0x004ABE90 carry placeholder bindings next to their real spellings.

## Header debt
None beyond the opt-ins handed back. A plain C++ spelling for blocker 1 (outer-expanded / nested-called CScriptThing copy) and for the
~CScriptThing partial-expansion budget would unblock 0x00E50FD0, 0x00E33EA0 and probably 0x00E5FD40 / 0x00D973D0; no
`/d2` flag or `#pragma inline_depth` was used in any row.

## vland (read-only, final overlay)
0x00D58490 RELOCATION_MATCH + PASS, no RELOC_IDENTITY. Three RELOC_NAME warnings are labels to correct (placeholder names on
STLport map members the matched row calls; evidence = the relocation pairing of this match plus the call shape -- ctor before the
operator[] calls, dtor at scope end): 0x00CDBFB0 std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map (labelled
CRetailTree_erase00cd344a::~...), 0x009AC2D0 std::map<CCharString,CCharString>::map (CUnprovenContainer_009ac2d0), 0x009AC310
std::map<CCharString,CCharString>::~map (CRetailTree_erase009aa830::~...).
