# 00e72630 (p25B, Account B, 2026-11-20): NScript::CV_DemonDoorsScript::CDF_ThreeSuits::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E72630 | CDF_ThreeSuits::Main() | 2236 + 16-byte switch table | RELOCATION_MATCH (no flags) | PASS (10 targeted + 20,000 random runs) | 31 / 38 killed (6 same code, 1 link fail); dead line reported separately |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, no warnings (no new relocation or binding).
- Purity: plain C++. No asm, no pragma, no flag.
- Line 2 `// Inline-budget:` names the header-contract inline devices, each with its ablation grade (below).
- One `// Inferred:` fitted statement, admitted by the coordinator's ruling of 2026-11-20 (advisor, under the owner's delegation): a dead
  conditional temporary. Details below.

## The residue and the fitted statement
The generated port compiled to retail's bytes except at the entry. Retail has `xor ebp,ebp` before the seh_me ctor and the dead store
`mov [esp+3Ch],ebp` (0x00E72641). That store is the home of a conditional-temporary flag: VC7.1 puts a flag's init at function entry. The live
form of the same store is in the landed DressAsBandit 0x00E71F10 (`mov [esp+2Ch],ebx`, then `or ebx,1/2/4..` for its && chain) and DoDamage 0x00E70BC0.
The 4 bytes also move both loop heads off retail's 16-byte pads (0x00E72709 `lea esp,[esp]`, 0x00E727DD `lea ecx,[ecx]`). Grade without the
fitted operand: DIFFER(2236v2252).

What did not produce the store (all byte-identical to each other; ablation/, grades in ablation/grades.txt):
- every placement of the enum declaration and its initialization (top of the function, before seh_me, at Ego_d line 690);
- the loop as `for(;!false;Game->NewScriptFrame())` or `while(!IsActiveThreadTerminating()){..Game->NewScriptFrame();}`;
- `default: break;`, and the last arm with or without `break`;
- a post-loop plain `return` or terminate check;
- the DressedAs* bodies defined in the TU, plain or `inline` (c2 traces them as inline candidates and calls them: scores 3911 / 4987 / 6442).

Folded spellings keep the flag tests live: `!false&&Me.MsgIsTalkedToBy(..)` / `false||..` DIFFER(2284v2252). The 0x00DA0230 `if (false)` form
does not fit this row: `if(false&&Me.MsgIsTalkedToBy(..))` and `if(false)Me.MsgIsTalkedToBy(..);` are folded by the front end, DIFFER(2236v2252).

The probe the ruling asked for (ablation/e72_p1.cpp): a conditional temporary inside a `__forceinline` body that Main expands mid-function. The
flag's init lands at the entry (`xor edi,edi; mov [esp+1Ch],edi` before the seh_me ctor), so an inline body could explain it. The bodies Main
expands (c2 trace) are:
- the ScriptedThingBase / CScriptGameResourceObjectBase / CCountedPointer ctors and dtors;
- the CIsThingAlive and CPredicateBase ctors;
- the movie ctor and dtor;
- the pause ctor and dtor;
- TRetVoid::Return.

All of their expanded retail bytes are visible and equal ours, and none takes a class temporary in a conditional operand. The list is clean,
so per the ruling the row lands with a fitted statement.

**Fitted statement.** Ego_d line 741, the dead post-loop terminate check, gets one more operand:
`if(IsActiveThreadTerminating()||Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO"))return _Fail_Val.Return();`.
- It adds no line Ego_d lacks. It uses only calls and literals this function already uses (the same `Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")` as line 694).
- The `&&` spelling (ablation/e72_f4.cpp) and the same operand after the bandit arm's return (e72_f5.cpp) also match.
- **So the bytes prove that a conditional temporary existed in dead code, but they constrain neither its content nor its place.**
- Ego_d's prolog line 672 is 36 bytes, the size of the flag-free siblings (KeyHoarder 853, SexMad 797, EatPies 498, SummonsCreatures 264). The rows
  with a flag are 46-47 (DressAsBandit 589, DoDamage 426, RockTroll 186, Spectator 720, LostTraderBrother 516). So Ego_d's later source revision
  had no such temporary. The ruling: Ego_d is a later revision (shown twice this session: navedge, 0x006E5A00), so it does not veto the fitted text.

**Entry for docs/journal/FINDINGS_LOG.md:** 2026-11-20 (p25B, 0x00E72630) dead conditional-temporary flag.
- VC7.1 initializes every conditional-temporary flag at function entry: Main's own, and also one in a `__forceinline` body expanded mid-function.
- It keeps the init's home store even when every test of the flag is dead.
- So an entry store `mov [esp+N],0/reg` with no reader is a conditional temporary that the generated script had in unreachable code.
- 0x00E72630 (CDF_ThreeSuits::Main) and 0x00DA0230 (MinionCamp DoMission) both land with a fitted dead conditional temporary.
  0x00E72630 uses an extra `||Me.MsgIsTalkedToBy(..)` operand on its dead post-loop check. 0x00DA0230 uses `if (false) AddSpawnedFunction(new ...)`.
- In both rows the Ego_d prolog (36 bytes) shows that the later debug revision lacked it.
- The parked 0x00E54E90 (BookCollecting CBS_Teacher::Main) and 0x00E97070 (HeroDolls CBS_Teacher::Main) carry the same residue. Their Ego_d prologs are 36 / 37.

## Ladder and per-device ablation (advisor decisions 2026-11-20; ablation/)

| device removed | grade without it |
|---|---|
| the fitted `||Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")` operand (`// Inferred:`) | DIFFER(2236v2252), 184 diff lines: no entry store, no loop pads |
| `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (out-of-line declaration) | DIFFER(2236v2252), 93 diff lines: the whole movie dtor is called at the block end, where retail expands it and calls ~CCountedPointer 0x006E7AB0 |
| inline CIsThingAlive ctor (the landed DemonDoors Mains' contract body) | DIFFER(2236v2252), 194 diff lines: the ctor is called |

No flag and no pragma are used. The row matches at default flags, so the flag step of the ladder had nothing to replace. The pause ctor is plain
inline here (expanded without the FORCEINLINE opt-in).

## Source shape
Ego_d.pdb line table (egod_lines/, lines 672..742) and locals:
- function scope: `region_exit`, `seh_me`, `_Fail_Val`, `WhatSuitIsNext` (an anonymous local enum, LF_ENUM 0x148AD);
- one block: {speech_movie, PauseMyEntitiesPlease}.

Generated quest-script text as the landed DemonDoors Mains:
- the seh macro and each speak-and-wait macro on one line;
- the movie and the pause on one line (696, 101 bytes, the same as DressAsBandit's 606);
- `switch(WhatSuitIsNext)` at 698 (35 bytes at /Od, equal), with fall-through cases;
- breaks at 712 / 722, and none after the last arm;
- the bandit arm's `return _Fail_Val.Return();` (732);
- the endless `for(;!false;Game->NewScriptFrame()){if(T)return..;}` (692). Retail duplicates the head check at 0x00E72D0F.

The static S_LDATA32 of the Ego_d proc is the switch table: the other switch Mains (AppleGirl, BanditKing, ApprenticeSpeedTest) have it too, and the
+17 bytes on line 742 over DressAsBandit's 632 is the 16-byte table.

## Headers (hdr/, hdr.diff)
- engine/CV_DemonDoorsScript.h: ADD-ONLY opt-in FABLE_RETAIL_CDF_THREESUITS. It declares the private non-virtual DressedAsBandit / DressedAsGoodKnight /
  DressedAsEvilMage (0x00E72F00 / 0x00E730A0 / 0x00E731F0; Ego_r.pdb / FableWin.pdb `..AAE_NXZ`), carried unchanged from chunk 834/856's
  pending overlay. No layout change. The existing size assert is pinned with pin_asserts (`FABLE_STATIC_ASSERT_AT(179, ...)`).
- NEWLY FLAGGED (owner policy: listed, not a blocker). These landed rows define CDF_ThreeSuits locally:
  0x00E72560 (Alloc), 0x00E72610 (scalar deleting dtor), 0x00E72F00 (DressedAsBandit), 0x00E730A0 (DressedAsGoodKnight), 0x00E731F0 (DressedAsEvilMage).

## Test (test/00e72630.cpp)
Links the real row object. These run as compiled:
- the row body;
- the expanded seh_me / movie ctors;
- the seh_me release expanded at the first exit;
- the ScriptedThingBase dtor and movie dtor COMDATs;
- the pause expansion.

Every external is bound by /alternatename to a __fastcall stub, and every vtable slot or unreached COMDAT reference to a trap.

What the inputs script:
- terminate positions and StartScriptingEntity retries;
- talked-to results;
- the three outfit tests;
- speaker health (positive, tiny positive, zero, negative);
- IsPerformingScriptTask sequences.

What the test checks:
- each run, event by event, against an independent model of the WhatSuitIsNext state machine (fall-through cases, the state carried across
  talks, the destruction order of every live local on each early return);
- every argument: priority 4, the four entity setters, REGION_EXIT_POINT from Me, the region exit off / on, the movie sequence, each speech text
  and its flags, FadeOutFaceAndOpenDoor(Me, "DD_ThreeSuits") on ParentClass;
- Me untouched, no store to this;
- every string, thing and handle released, and the pause balanced.

The 20,000 random runs reach the door 4,384 times.

## Mutants (mutants/)
**Live code.** 38 one-decision mutants (mutants/m72.tsv, m72.log): 31 killed.
- SAME_CODE (6): a literal or callee swap that changes only a relocation target (mut2 compares bytes). The literals are REGION_EXIT_POINT,
  SCRIPT_NAME_HERO, the OPENING and WANT_BANDIT texts, and the DressedAsEvilMage / DressedAsBandit callee swaps. The test's strcmp checks on every
  string and its per-callee stubs would catch them.
- LINK_FAIL (1): deleting `SetPredicate(CIsThingAlive(Me));` stops the object from emitting the CIsThingAlive vtable that the test binds.
  The test's SetPred check covers this statement.

**Dead line** (the fitted operand, mutants/m72dead.tsv, m72dead.log; the behaviour test cannot kill these, the grade does):
- dropping the operand: SURVIVED the test; the grade is DIFFER(2236v2252);
- `||` -> `&&`: SAME_CODE (RELOCATION_MATCH, as ablation/e72_f4.cpp);
- `"SCRIPT_NAME_HERO"` -> `""`: SAME_CODE;
- `return _Fail_Val.Return();` -> `;`: SAME_CODE.
