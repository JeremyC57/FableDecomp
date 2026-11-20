# 00d1a740 (p25A, Account B, 2026-11-20): NScript::CQ_BountyHuntScript::CBH_Hostage2::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00D1A740 | NScript::CQ_BountyHuntScript::CBH_Hostage2::Main() | 2743 | RELOCATION_MATCH (`/d2inlS15`) | PASS (1,464 targeted + 6,000 random runs) | 52 / 53 killed (1 equivalent) |

Strict vland: `RELOCATION_MATCH PASS`. One warning, RELOC_NAME: the TeleportData ctor 0x00D15140 is labelled
CMainGameComponentInit::CMainGameComponentInit. That label is wrong; see NAME_FIXES.tsv. Purity: plain C++.

## Source shape
Ego_d.pdb line table (egod_lines/, lines 1578..1712) and locals (sehMe, _Fail_Val; fadeOutMarker, tData; teleportMkr, Leader, Exit,
sehHero, speech_movie, myMap, PauseMyEntitiesPlease). It is the generated quest-script text of the landed Mains (the sibling
CBH_Hostage1::Main 0x00D19BD0 in the same TU): each wait is `while (!cond) { NewScriptFrame; terminate test }` followed by a
post-loop terminate test, and every return is `return _Fail_Val.Return();` (TRetVoid).
- `// Inline-budget:` the `/d2inlS15` flag, as the sibling 0x00D19BD0 and 0x00D17F20 (S15 expands the implicit MovieBase dtor at the
  cutscene block), plus the existing header opt-in FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN for the
  two cast-map assignments. Retail expands the handle's operator= there; the implicit member stays the COMDAT call 0x008ABD10 under
  every flag value. 0x00D19BD0 forces the same definition inline with a row-local copy; this row uses the shared opt-in instead.
- `// Inferred:` the post-loop terminate test of the GreatwoodLake/FishComplete wait (Ego_d line 1619) is spelled with an empty
  then-arm: `if (IsActiveThreadTerminating()) {} else return _Fail_Val.Return();`.
  - Behaviour is unchanged: both arms leave the branch and nothing follows it.
  - Retail keeps the test (`call 0x00F35B30; test al,al; lea ecx,[esp+78h]; je 0x00D1AA91` at 0x00D1AAA1), and both arms run the
    same tData / fadeOutMarker exit tail (two copies).
  - These spellings all fold the test away (DIFFER 2743v2743, 153 instructions differ): the plain test, `!T()`, a do/while(0)
    wrapper, `return;` after it, an else-return, and `if (!T()) {...} else {}`.
  - Only an empty then-arm keeps it: `{}` and `;` both give RELOCATION_MATCH.
  - A reduction (`call T; test; lea; je` shape) showed the same: with an empty then-arm, c2 merges the not-terminating continuation
    with the in-loop exit tail, and the post-loop terminating arm keeps its own copy.
  - Ego_d line 1619 is 353 bytes and holds the whole wait, so the line table cannot separate the spellings.

## Header overlay
None. Every opt-in the row uses already exists in rebuild/include. No new header file and no landed rows flagged.

## NAME_FIXES.tsv
0x00D15140 is `??0TeleportData@CQ_BountyHuntScript@NScript@@QAE@XZ`, not CMainGameComponentInit::CMainGameComponentInit.
- The body default-constructs four CCharStrings at +0/+0xC/+0x10/+0x14 and leaves +4/+8. That is the Ego_r TPI TeleportData layout.
- Ego_r CMainGameComponentInit is 16 bytes: CWideString / CWideString / CCharString / CWideString at 0/4/8/12.
- The TeleportData ctor is in both PDBs: Ego_r 0x00D18F90 (register_scripts.obj) and FableWin 0x036C3130.
- The Q_BountyHunt callers 0x00D13580, 0x00D18DF0 and 0x00D1A740 build their stack TeleportData with it.
- The shared header and the landed ~TeleportData 0x00CD4550 already say so.

## Test (test/00d1a740.cpp)
The test links the real row object.
- What runs as compiled: the expanded CIsThingAlive, CScriptThing::GetPos, the handle constructors, the forced-inline handle
  operator=, the expanded MovieBase dtor, CWideScreenMagicPauseEntities, and the TU's own map operator[] / lower_bound / ~map / _M_erase.
- Fixtures: the entity at native layout and the parent at native offsets (flags +0x4f..+0x70, TeleportInfo +0x88); the global script
  def (gold +0xb04, wander distance +0xb28, renown +0xb54, reward object +0xb78 read through GDefStringTable); things with fake
  implementations; handles with counted implementation records; an STLport map header and nodes; 24 Game vtable slots.
- Scripted world: it changes the regions and the parent's flags at every NewScriptFrame. Termination, StartScriptingEntity results,
  distances, aliveness (Leader alive and LeaderKilled are independent), possession and null marker / exit things are scripted too.
- Checks:
  - the event order against an independent model;
  - Hostage2Safe / LakeComplete;
  - the teleport record pushed into TeleportInfo;
  - the cutscene cast (Hero = hero handle, Victim = sehMe);
  - the move targets (NullCoord for null things);
  - the balance of strings, things, CBaseObject parts and handle references.

## Mutants (mutants/)
53 one-decision mutants; 52 killed. The survivor is equivalent: it removes the GreatwoodEntrance `if (sehMe.IsValid()) sehMe.Release();`,
where sehMe was constructed just before and is never valid.
