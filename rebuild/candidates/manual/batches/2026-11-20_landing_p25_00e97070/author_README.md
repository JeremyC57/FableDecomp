# 00e97070 (p25B, Account B, 2026-11-20): NScript::CV_HeroDollsScript::CBS_Teacher::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E97070 | CBS_Teacher::Main() | 3619 | RELOCATION_MATCH (no flags) | PASS (8 targeted + 20,000 random runs) | 51 live: all killed (44 by mut2, 7 relocation-only mutants by running the test on their objects); dead line reported separately |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, no warnings.
- No new relocation or binding: the object's external symbols are identical with and without the fitted line (undef lists compared), and
  the fitted line compiles to no bytes.
- Purity: plain C++. No asm, no pragma, no flag.
- Line 2 `// Inline-budget:` names the inline devices, each with its ablation grade (below).
- One `// Inferred: dead conditional temporary --` fitted statement, admitted by the coordinator's rulings of 2026-11-20 (advisor, under the
  owner's delegation) for this row. Details below.

## The residue
The parked draft (chunk 845) and the draft in the generated text (ablation/e97_g1.cpp) are both DIFFER(3619v3619), 70 diff lines. The only
residue is retail's dead entry store `mov dword ptr [esp+20h],ebx` (0x00E97081). ebx is zeroed at 0x00E9707A and also feeds the seh_me ctor's
zero stores. That store is the home of a conditional-temporary flag that nothing tests, the same residue as 0x00E72630 and 0x00E54E90.

## Ruling steps
**(1) Probe and inline-body check** (ablation/e97_p1.cpp). A conditional temporary inside a `__forceinline` body that Main expands mid-function
(in the HeroHasAllDolls test) puts its flag init at the entry: `xor ebp,ebp; mov [esp+20h],ebp`, before the first call. So an inline body could
explain the store. The bodies Main expands (c2 trace of the final source src/00e97070.cpp, `tools/trace.sh`) are:
- the CCountedPointer ctors / dtors (movie, ScriptedThingBase, CScriptThing);
- the CScriptGameResourceObjectBase ctor / dtor;
- the movie ctor / dtor and the ScriptedThingBase ctor / dtor (expanded and called);
- ~CScriptThing (the row-local contract body);
- CCountedPointer<CScriptThing> operator= / ShareData / ReleaseData (from the forceinline CScriptThing::operator=);
- the pause ctor / dtor (the ctor is plain inline and expanded, with no opt-in);
- TRetVoid::Return.

Their expanded retail bytes equal ours, and none takes a class temporary in a conditional operand. The list is clean, and the evidence matches
0x00E72630's. There was nothing to stop and report.

**(2) Existing dead positions.** None. Every Ego_d line of this proc (egod_lines/, lines 100..209) has retail code. There is no code after an
unconditional return and no unreachable arm.

**(3) Placement.** This function has no inner endless `for(;!false;..)` loop, the class used by the landed rows. The only dead position is
after the DONT_SWAP arm's `break;`. Placements graded (ablation/grades.txt):

| placement | grade |
|---|---|
| after `break;`, empty body `{}` | RELOCATION_MATCH (every matching operand below) |
| after `break;`, with `return _Fail_Val.Return();` as the body (e97_f1 / f3 / f6) | DIFFER(3641v3619): the dead return leaves an exit tail |
| `if(false)Game->GetNearestWithDefName(KidIntro,"CREATURE_BS_VILLAGER_BOY").IsAlive();`, the 0x00DA0230 form (e97_op_z1) | DIFFER(3619v3619), 70 lines: folded, no flag |
| `if(false&&<that call>){}` (e97_op_z2) | DIFFER(3619v3619), 70 lines: folded, no flag |
| `if(false){if(T\|\|Me.MsgIsTalkedToBy(..))return ..;}` at the top (e97_f2) | DIFFER(3407v3619) |
| the reachable post-loop terminate check given the operand (e97_f4) | DIFFER(3714v3619): a live temporary, not a dead one |

**(4) Operand** (after `break;`, empty body):

| operand | grade |
|---|---|
| `IsActiveThreadTerminating()\|\|Game->GetActiveQuestName()!=""` (e97_op_q1, **chosen**) | RELOCATION_MATCH |
| `IsActiveThreadTerminating()\|\|Game->GetActiveQuestName().IsEmpty()` (e97_op_q2) | RELOCATION_MATCH (IsEmpty is not used by this function) |
| `IsActiveThreadTerminating()\|\|Game->GetHealth(seh_me.GetScriptThing())>0.0f` (e97_op_h1) | RELOCATION_MATCH (the speak guard, live verbatim at lines 77 / 86 / 91) |
| `Game->GetHealth(seh_me.GetScriptThing())>0.0f` alone, no `\|\|` (e97_op_h2) | DIFFER(3619v3619), 70 lines: the temporary is not conditional, so no flag |
| `IsActiveThreadTerminating()\|\|Game->GetNearestWithDefName(KidIntro,"CREATURE_BS_VILLAGER_BOY").IsAlive()` (e97_o1) | RELOCATION_MATCH |
| the same with `Game->GetHero()` for `KidIntro` (e97_o3) | RELOCATION_MATCH |
| `IsActiveThreadTerminating()\|\|Game->GetNearestWithDefName(KidIntro,"CREATURE_BS_VILLAGER_BOY").GetName()!=""` (e97_op_n1) | RELOCATION_MATCH |
| `IsActiveThreadTerminating()\|\|KidIntro.GetName()!=""` (e97_o2) | DIFFER(3619v3619), 70 lines: no class temporary, so no flag |
| `IsActiveThreadTerminating()\|\|Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")` (e97_f5) | RELOCATION_MATCH |
| `IsActiveThreadTerminating()&&Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")` (e97_o4) | RELOCATION_MATCH |

Every `||` / `&&` operand that builds a string or CScriptThing temporary matches. By the ruling's criteria (only calls and literals live in this
function, fewest tokens), the pick is q1. It uses:
- GetActiveQuestName, whose string temporary is live at lines 125 / 126 / 141 / 142;
- CCharString::operator!=(const char*) with `""`, live at line 99.

h1, the verbatim live speak guard, is the next shortest. Me.MsgIsTalkedToBy (not used by this function) was not needed.

**(5) What the bytes prove.**
- **The bytes prove that a conditional temporary existed in dead code, but they constrain neither its content nor its place.**
- Ego_d has no such statement.
- **The Ego_d prolog of 37 bytes.** A flag init at /Od is a 10-byte `mov dword ptr [ebp-disp32],0`:
  - this row's own VC7.1 /Od prolog line is 16 bytes without the fitted line and 26 with it (`tools/odl2.py`);
  - Ego_d's flag-free siblings are 36 and its flagged ones 46: DressAsBandit, DoDamage, RockTroll, Spectator, Witch.
  - So 37 cannot hold a flag init. Ego_d's later revision had no such temporary.
- **The extra byte is a `push esi`.** At /Od (VC7.1, `odl2.py`), the rows whose Ego_d prolog is 1 byte over the base all save esi in the
  prolog; the base rows do not:

  | row | Ego_d prolog | /Od `push esi` |
  |---|---|---|
  | HeroDolls 0x00E97070 | 37 | yes |
  | AssassinTrigger 0x00E37E40 | 47 | yes |
  | LostTraderBrother 0x00EA59E0 | 47 | yes |
  | Book 0x00E54E90 | 36 | no |
  | ThreeSuits 0x00E72630 | 36 | no |
  | KeyHoarder 0x00E740C0 | 36 | no |
  | DressAsBandit 0x00E71F10 | 46 | no |
  | Witch 0x00ECE7F0 | 46 | no |

  So 37 = the flag-free 36 + a 1-byte callee-saved register push. BriarRose 0x00E9E420 (38) is next in my queue and will be checked there.
- The ruling: Ego_d is a later source revision, so it does not veto the fitted text.

## Ladder and per-device ablation (ablation/, each removed in turn from the final source)

| device removed | grade without it |
|---|---|
| the fitted dead statement (`// Inferred: dead conditional temporary --`) | DIFFER(3619v3619), 70 diff lines: no entry store, `xor ebx,ebx` moves after the seh_me CBaseObject call, the code after it shifts |
| row-local `inline CScriptThing::~CScriptThing() { }` | DIFFER(3379v3619): retail expands it at some sites and calls 0x004AA840 at others |
| row-local `FABLE_FORCEINLINE CScriptThing::operator=` dropped (the header's out-of-line declaration, 0x008AB980) | DIFFER(3468v3619) |
| the same as plain `inline`, the spelling of 147 landed rows | DIFFER(3532v3619) |
| `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (out-of-line declaration) | DIFFER(3507v3619) |

- **The operator=.** EXTRA_RULES prefers plain `inline` where an out-of-line copy exists (0x008AB980 does), when the bytes hold. Here they do
  not, so the row uses `FABLE_FORCEINLINE` (the portable spelling), as the 24 landed rows that need it do (e.g. 0x00D52E90, 0x00D74430).
- **The pause opt-in.** The draft's FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR is not needed: without it the plain inline pause
  ctor is expanded at both sites, and the bytes are the same (e97_pauseoptin.cpp, the final source plus the opt-in: RELOCATION_MATCH). So it is
  dropped and not named in the budget.
- No flag and no pragma are used. The row matches at default flags, so the flag step of the ladder had nothing to replace.

## Source shape
The draft's statement order is kept: its bytes already matched apart from the entry store. It is written in the generated text of the landed
Mains:
- `TRetVoid _Fail_Val;` and `return _Fail_Val.Return();` at every exit;
- each speak-and-wait macro on one line;
- movie and pause on one line;
- each terminate check on its statement's line.

Names come from Ego_d.pdb's locals:
- function scope: `seh_me`, `_Fail_Val`, `DoesHeroHaveAnyDolls` (the draft's `hero_has_dolls`);
- blocks {`answer`, `speech_movie`, `PauseMyEntitiesPlease`} and {`seh_KidIntro` (the draft's `seh_kid`), {`speech_movie`, `PauseMyEntitiesPlease`}}.

## Headers (hdr/, hdr.diff: one NEW file)
NEW engine/CV_HeroDollsScript.h, carried from chunk 845's pending overlay. One addition: the CV_HeroDollsScript size assert, with its evidence in
the comment. Checked against two sources:
- **Ego_r.pdb** (struct_layouts_egor.tsv):
  - CV_HeroDollsScript is 96 bytes, with HeroDollScriptEnded at 92 and KidIntroDone at 93. Retail = PDB - 0x14 (retail CLevelScriptBase is
    0x48), so +0x48 / +0x49 and size 0x4C.
  - CBS_Teacher is 40 bytes: ParentClass +0x14, PMasterData +0x18, KidIntro +0x1C.
  - CQ_SunnyvaleMasterData::HeroDollsScriptUsingTeacher is +0xF4 (244).
- **Retail bytes:**
  - this row reads and writes +0x48 / +0x49 on ParentClass (0x00E970F6, 0x00E9754D, 0x00E97582, 0x00E97AE8) and +0xF4 on PMasterData
    (0x00E9713D / 0x00E97B95);
  - CV_HeroDollsScript::Alloc 0x00E98640 does `push 4Ch` (0x00E98643);
  - the landed Alloc 0x00E96FB0 allocates 0x28 for CBS_Teacher.
- **Methods:** RemoveHeroDolls 0x00E97EA0, HeroHasAllDolls 0x00E97FD0 and HeroHasAnyDolls 0x00E98200 (Ego_r.pdb / FableWin.pdb, all landed).
- The test runs the row against those offsets.

NEWLY FLAGGED (owner policy: listed, not a blocker). These landed rows define CV_HeroDollsScript / CBS_Teacher locally:
0x00E96E30 0x00E96E60 0x00E96E70 0x00E96F10 0x00E96FB0 0x00E97050 0x00E97060 0x00E97EA0 0x00E97FD0 0x00E98200 0x00E98430 0x00E98460
0x00E98610 0x00E98640 0x00E98680.

## Test (test/00e97070.cpp)
The test links the real row object. These run as compiled:
- the row body;
- the expanded seh_me / seh_KidIntro / movie ctors;
- the ScriptedThingBase and movie dtor COMDATs;
- the expanded KidIntro assignments (ShareData / ReleaseData);
- the expanded and called ~CScriptThing of the GetNearestWithDefName / GetScriptThing temporaries;
- the pause expansion.

Every external is bound by /alternatename to a __fastcall stub, and every vtable slot or unreached COMDAT reference to a trap. ParentClass and
PMasterData are raw buffers at the header offsets. KidIntro and every returned thing carry real counted-pointer info blocks.

What the inputs script:
- terminate positions;
- KidIntro alive;
- HeroHasAnyDolls / HeroHasAllDolls;
- the StartScriptingEntity results for Me and KidIntro;
- distances;
- the answer sequence (-1 waits, 0 no, 1 yes, 2);
- speaker health and IsPerformingScriptTask;
- the kid's name test;
- the hero-control / cutscene / tavern wait;
- received items;
- frames that end the doll script from outside;
- the initial HeroDollScriptEnded / KidIntroDone / UsingTeacher.

What the test checks:
- each run, event by event, against an independent model, including:
  - the evaluation and destruction order of every string temporary (right-to-left construction of the yes/no strings, the quest-name temporaries);
  - the release of KidIntro's old thing inside each assignment, before the name string dies;
  - the destruction order of every live local (seh_me, seh_KidIntro, movie, pause) on each early return and at the `break;`;
- every argument: priority 4, the information flags (false, true, false), distances 20 / 5 from the hero to Me / KidIntro, FollowThing(hero,
  1.0f, true), Pause(0.5f), each speech text and its speaker, the yes/no strings, GiveHeroObject(.., -1, false), the quest card / objective /
  completion / deactivation arguments;
- the final HeroDollScriptEnded / KidIntroDone / HeroDollsScriptUsingTeacher and no other store;
- Me untouched, KidIntro owning its last thing exactly once;
- every string, thing and handle released, and the pause balanced.

What the 20,000 random runs reach:

| path | runs |
|---|---|
| swap accepted | 1,469 |
| swap refused (`break;`) | 2,983 |
| doll-kid arm | 1,796 |
| second kid lookup | 868 |
| quest card | 1,381 |
| doll received | 1,737 |
| seh_me release | 1,062 |
| seh_KidIntro release | 335 |
| ended from outside | 4,711 |

## Mutants (mutants/)
**Live code.** 51 one-decision mutants (mutants/m97.tsv, m97.log). mut2 killed 44. The other 7 are SAME_CODE: they change only a
relocation target, and mut2 compares bytes. They are:
- the text swaps of INTRODUCTION / DO_SWAP / DOLLKID and of the YES/NO strings;
- the objective's `""` -> `"x"`;
- HeroHasAllDolls <-> HeroHasAnyDolls (twice).

All 7 were compiled and run against the test (mutants/sc/: sources, list.txt, results.txt), and all 7 FAIL. The `!= ""` -> `== ""` mutant links
because the test binds CBasicString<char>::operator== to the same scripted name, and mut2 kills it.

The live mutants ran on the source with the o1 operand. The final source's object is section-for-section identical to it apart from
.debug$S, since the dead line compiles to nothing, so each live mutant compiles to the same object with either operand.

**Dead line** (the q1 statement, mutants/m97dead.tsv, m97dead.log). The behaviour test cannot kill these; the grade does:
- dropping the statement: SURVIVED the test; the grade is DIFFER(3619v3619), 70 lines;
- `||` -> `&&`: SAME_CODE (RELOCATION_MATCH);
- `!=""` -> `==""`: SAME_CODE;
- `""` -> `"x"`: SAME_CODE;
- the operand -> `KidIntro.GetName()!=""`: SURVIVED the test; the grade is DIFFER(3619v3619), 70 lines (no class temporary).

## Entry for docs/journal/FINDINGS_LOG.md (addendum to the 0x00E72630 entry)
2026-11-20 (p25B, 0x00E97070) HeroDolls CBS_Teacher::Main.
- It lands with a fitted dead conditional temporary like 0x00E72630 / 0x00E54E90, but as a new dead statement: Ego_d has no dead line here, and
  there is no inner endless loop.
- The statement is placed after the DONT_SWAP arm's `break;` with an empty body: a `return` body leaves a dead exit tail (3641v3619).
- The operand is the shortest matching one built from the function's own calls: `Game->GetActiveQuestName()!=""`. Every `||` / `&&` operand
  with a string or CScriptThing temporary matches.
- The inline-body probe gives the same result as before: the flag init lands at entry, and the expanded-body list is clean.
- The Ego_d prolog is 37: the flag-free 36 plus a `push esi` (the same +1 as the 47-byte rows 0x00E37E40 / 0x00EA59E0, which also save esi at
  /Od). A flag init is 10 bytes at /Od.
- The draft's PAUSEENTITIES forceinline opt-in proved unnecessary. The forceinline CScriptThing::operator= is needed (plain inline: 3532v3619).
