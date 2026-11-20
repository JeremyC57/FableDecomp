# 00e7c010 (p25B, Account B, 2026-11-20): NScript::CV_FishermanScript::NPCAttitudeProcess(CScriptThing) -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E7C010 | CV_FishermanScript::NPCAttitudeProcess(CScriptThing) | 1623 | RELOCATION_MATCH (no flags) | PASS (21 targeted + 20,000 random runs) | 41 / 41 killed |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, no warnings.
- Purity: plain C++. No asm, no pragma, no flag, no header change.
- The draft carried `// Flags: /Gr`; the row does not need it.

## What the parked diagnosis missed
The parked draft (chunk 780: DIFFER 1624v1623, "terminate-exit block merge") was hand-shaped. It had two real errors and the wrong loop shape:
1. **The condition is negated.** Retail reaches the `mov [esp+13h],1` true store from both the third test's null-Data path and its false result
   (0x00E7C0E0 / 0x00E7C0F3). The watch is therefore `MsgIsHitBy(HERO) || (MsgIsHitByAnySpecialAbilityFrom(HERO) && !MsgIsHitBySpecialAbilityFrom(
   HERO_ABILITY_HEAL_LIFE_SPELL, HERO))`: any special ability except Heal Life.
2. **The health test is `<=`.** `GetHealth(Me)<=1.0f` compiles to `fcomp 1.0 ; fnstsw ; test ah,41h ; jnp` into the scene (0x00E7C166). The draft had
   `!(health <= 1)`.
3. **The loop is the generated endless watch.** `for(;!false;Game->NewScriptFrame()){if(IsActiveThreadTerminating())return _Fail_Val.Return(); ..}`
   is the form of the landed 0x00EBFD10. VC7.1 duplicates its head check (0x00E7C063 / 0x00E7C170), so the two prelude exits and the bottom exit share
   retail's single expanded ~Me block. This removed the parked "exit merge" residue.

With the Ego_d names, the generated port matched at the second grade: the first grade had only the two polarity residues.

## Source shape
Ego_d.pdb line table (egod_lines/, lines 68..125) and locals:
- parameter `Me`;
- function scope `_Fail_Val`;
- one block {seh_me, {NPCPlacementMarker, speech_movie, PauseMyEntitiesPlease, {NPCHeroMarker, NPCCamera}}}.

The seh macro and the speak-and-wait macro are each on one line (lines 81 / 107, 213 / 439 bytes). No `// Inferred:` lines.

## Names
- `NPCAttitudes[2]=2` uses the global `extern long NPCAttitudes[3]` (0x0143E928, ?NPCAttitudes@@3PAJA), declared as in the landed 0x00CDBA10 / 0x00E7C8E0.
- I did not use the header opt-in FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS. Its `CQ_SunnyvaleMasterData::RelationCounters` /
  `RelationSuffixes` are invented spellings of NPCAttitudes / NPCAttitudeStrings: vland warned RELOC_NAME, and the header comment itself calls them
  "three unnamed statics".
- NAME_FIXES.tsv has both entries. The landed user of the opt-in is 0x01224CE0.

## Per-device ablation (advisor decisions 2026-11-20)
These are header-contract bodies and opt-ins; each is removed in turn (ablation/):

| device removed | grade without it |
|---|---|
| FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR | DIFFER(1627v1623) |
| `inline CScriptThing::~CScriptThing() { }` (retail expands ~Me at the shared exit 0x00E7C17F) | DIFFER(1586v1623) |
| inline CScriptThing::MsgIsHitBy (null-safe wrapper) | DIFFER(1609v1623) |
| inline CIsThingAlive ctor | DIFFER(1623v1623) |

No flag and no pragma are used, so ladder step 2 does not apply.

## Test (test/00e7c010.cpp)
Links the real row object. These run as compiled:
- the expanded ~CScriptThing / CCountedPointer releases;
- the Msg* wrappers;
- the CIsThingAlive ctor;
- the ScriptedThingBase and movie COMDATs;
- the pause expansion.

Me is passed by value as {row vtable, implementation, real info block}, and the test proves the callee destroys it.

What the inputs script: terminate positions; hit / special / Heal Life results; null Me implementation; low health (exactly 1.0, 0.9999,
1.0001, negative); StartScriptingEntity retries; the two stand-point distances; camera alive; speaker health; IsPerformingScriptTask.

What the test checks:
- the event order, including the destruction order of every live local on each early return;
- every call's arguments (distance 10, stand points, angles per stand point, camera point (0, 0, -1.0f, 0, -1), speech text and flags,
  MoveToThing(MK_FI_HERO, 3.0f, RUN, false, false, false, true), fade 1.5 / 1.5 / opaque black, pauses 1.5 / 2.0, RemoveThing(Me, false, false));
- NPCAttitudes[2] = 2 only, FishermanMustRunAway;
- the release of every string, thing and handle.

## Mutants (mutants/)
41 one-decision mutants, all killed.
