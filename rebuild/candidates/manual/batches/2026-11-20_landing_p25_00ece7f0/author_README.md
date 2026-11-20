# 00ece7f0 (p25B, Account B, 2026-11-20): NScript::CV_SickChildScript::CWitch::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00ECE7F0 | CWitch::Main() | 4086 | RELOCATION_MATCH (no flags) | PASS (6 targeted + 20,000 random runs) | 48 live: 47 killed (38 by mut2, 9 relocation-only mutants by running the test on their objects), 1 link fail |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, with one warning:
`WARN RELOC_NAME: ??BCCharString@@QBEPBDXZ -> 0099e4c0 labelled 'CCharString::operator const char *'`.
- The binding is right: retail calls 0x0099E4C0 to convert TextToSpeak for Speak, and labels_*.tsv names 0x0099E4C0
  `??BCCharString@@QBEPBDXZ` (agentNF22 from Ego_r.pdb / FableWin.pdb; acctC 2026-11-14).
- The warning is a tool gap. `reloc_identity.sym_identity` has no `_OPS` entry for `??B` (conversion operators). It parses the symbol as a
  function named `BCCharString`, which does not match the catalog's demangled label.
- Suggested tool fix (not mine to make): map `B` to an operator kind in `_OPS`, so `label_agrees` takes its `"operator" in L` branch.
  GOTCHAS line: "RELOC_NAME on `??B<Class>@@...` (conversion operator) is a reloc_identity parse gap, not a wrong binding."
- No header change. Purity: plain C++. No asm, no pragma, no flag.
- Line 2 `// Inline-budget:` names the inline devices and opt-ins, each with its ablation grade (below).
- One `// Inferred:` line, the `parent` reference (below). No fitted dead statement.

## What the parked diagnosis missed
The parked draft (chunks 808 / 863 / 868: DIFFER(4086v4086), md=4, about 40 attempts) left one residue at 0x00ECF317:
- retail passes the CS_SICK_POTION EndPotion conversion slot as `mov eax,esp ; push eax`;
- the draft passed it as `push esp`.

The cause was the CDefString binding, not the argument spelling. The 2026-11-19 binding swap (FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING)
expands the conversion and calls CDefString::GetString 0x00415D70 out of line, which gives retail's `mov eax,esp ; push eax`. With it, the row
matches.

Two other changes from the draft:
- **The `bool more` loop flag was invented.** The hand-over loop is now `while (Game->IsObjectInThingsPossession(pGlobalScriptDef->IngredientObject,
  Game->GetHero())) {..}`, with the same bytes.
- **`text` is renamed `TextToSpeak`,** from Ego_d.pdb.

## Ladder and per-device ablation (ablation/, each removed in turn from the final source; grades.txt)

| device removed | grade without it |
|---|---|
| FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING -> the draft's OUTOFLINE_CONVERSION | DIFFER(4166v4086) |
| FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING -> neither opt-in | DIFFER(4118v4086) |
| FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR | DIFFER(4086v4086), 605 diff lines |
| row-local `inline CScriptThing::~CScriptThing() {}` | DIFFER(4015v4086) |
| inline CIsThingAlive ctor (the landed DemonDoors Mains' contract body) | DIFFER(4070v4086) |
| `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (out-of-line declaration) | DIFFER(4015v4086) |
| the `parent` reference block (`// Inferred:`), written through ParentClass directly | DIFFER(4086v4086), 15 diff lines |

No flag and no pragma are used. The row matches at default flags, so the flag step of the ladder had nothing to replace.

**The `parent` reference.**
- Retail forms `&ParentClass.seh_Witch` once (`mov eax,[esi+14h]; lea ebx,[eax+58h]`) for both the IsValid and the Release calls. Written through
  `ParentClass` directly, it is reloaded.
- `{ CV_SickChildScript& parent = ParentClass; if (parent.seh_Witch.IsValid()) parent.seh_Witch.Release(); }` is the idiom of the landed twin
  CSickChildsMother::Main 0x00ECD9E0 and nine more landed Mains.
- Ego_d.pdb does not list `parent`, but its old module streams are carved and incomplete. So the line is `// Inferred:`.

## Source shape
Ego_d.pdb's line table (egod_lines/, lines 461..595; prolog 46 = a flagged row, as retail's live `||` / `&&` hit-test chain needs) and locals:
- function scope: `TimesHit`, `_Fail_Val`;
- blocks {speech_movie, PauseMyEntitiesPlease, {YNQAnswer}} and {TextToSpeak, speech_movie, PauseMyEntitiesPlease};
- the post-loop pair of the same blocks.

The hit test is `MsgIsHitBy(HERO) || (MsgIsHitByAnySpecialAbilityFrom(HERO) && !MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, HERO))`,
the form of the landed 0x00E7C010. HERO_ABILITY_HEAL_LIFE_SPELL comes from the shared engine/EHeroAbility.h, replacing the draft's local enum.

## Headers
None changed. engine/CV_SickChildScript.h (in the repo) carries the layout:
- seh_Witch at ParentClass +0x58 (ParentClass is a reference);
- CWitch IntroDone +0x1C / ThingsForPotionGot +0x20.

The retail bytes read the same offsets, and the test runs the row against them. engine/EHeroAbility.h and engine/fable_script_def_global.h
(pGlobalScriptDef: IngredientObject +0x738, EndPotion +0x744) are existing shared headers. No rows are newly flagged.

## Test (test/00ece7f0.cpp)
Links the real row object. These run as compiled:
- the row body;
- the CIsThingAlive predicate handed to SetPredicate;
- the expanded and called movie ctor / dtor COMDATs;
- the expanded ~CScriptThing of the GetScriptThing temporaries;
- the expanded CDefString conversions (GetString bound to a stub);
- the pause expansion;
- seh_Witch's virtual calls.

Every external is bound by /alternatename to a __fastcall stub, and every vtable slot to a trap.

What the inputs script:
- terminate positions and StartScriptingEntity retries;
- talk / hit / special-ability / Heal Life results;
- ingredient possession, per call;
- the answer sequence;
- speaker health and IsPerformingScriptTask;
- the initial ThingsForPotionGot / IntroDone and a held seh_Witch.

What the test checks:
- each run, event by event, against an independent model, including:
  - the evaluation and destruction order of every string temporary (the hit-test chain's three "SCRIPT_NAME_HERO" temporaries, the yes/no strings,
    PlayCutscene's two by-value CCharStrings destroyed by the callee, and the EndPotion conversion);
  - the destruction order of every live local on each early return and at the `break;`;
- every argument;
- the final ThingsForPotionGot / IntroDone and seh_Witch's held state;
- every string, thing and handle released, and the pause balanced.

What the 20,000 random runs reach:

| path | runs |
|---|---|
| seh_Witch release at the start | 9,629 |
| intro cutscene | 4,928 |
| need more | 3,343 |
| potion and leave | 1,003 |
| refused | 4,842 |
| waiting | 11,211 |
| hits 1 / 2 / 3 | 22,187 / 16,311 / 11,748 |
| got cure | 24,932 |

The switch's `default:` arm is unreachable: TimesHit is a local that cycles 1..3 and resets to 0. It is kept because the bytes have it: without
both `default:` arms, DIFFER(4054v4086) (ablation/ece_nodefault.cpp).

## Mutants (mutants/)
48 one-decision mutants (mutants/mece.tsv, mece.log). mut2 killed 38.
- SAME_CODE (9): text and literal swaps that change only a relocation target (mut2 compares bytes): "NULL" -> "", the objective's `""` ->
  `"x"`, NEED_MORE / OBJECTIVE_03 / NOT_GIVEN / WAITING / ONHIT_20 / ONHIT_30 / GOT_CURE. All 9 were compiled and run against the test
  (mutants/sc/: sources, list.txt, results.txt), and all 9 FAIL.
- LINK_FAIL (1): deleting `SetPredicate(CIsThingAlive(Me));` stops the object from emitting the CIsThingAlive vtable that the test binds (as in
  0x00E72630). The test's SetPred check covers this statement.

## NAME_FIXES
None.
- Ego_r.pdb / FableWin.pdb confirm PlayCutscene 0x00ECE460, the Game slots and the CWitch members.
- pGlobalScriptDef is `?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A`.
