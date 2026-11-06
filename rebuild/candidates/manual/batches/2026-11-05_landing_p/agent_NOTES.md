# Chunk 789 notes

Result: 2 RELOCATION_MATCH (0x00E776F0, 0x00EB8900) and 4 parked (0x00CFE030 23 difflines, 0x00D82640 38, 0x00DEA3F0 161,
0x00D7FE90 398). All six tests PASS under g++ and VC7.1. vland is clean: 2 wins, no RELOC_IDENTITY, no warnings.
Every row name is proven from the Ego_r.pdb name, the vtable slot or the spawned-thread literal, plus the retail body.
No descriptive names.

## 0x00E776F0 NScript::CV_ExposeMayorScript::CEM_SuspiciousVillager::Main -- RELOCATION_MATCH (src/00e776f0.cpp)
- **Shape:** generated entity-script code, the 0x00EACF00 model.
- **Key spelling:** the seh_Suspicious IsValid / Release pair goes through a local parent reference,
  `CV_ExposeMayorScript& parent = ParentClass;`. Retail does `mov eax,[esi+14h]; lea ebx,[eax+58h]`. A handle reference or pointer
  gives `mov ebx,[esi+14h]; add ebx,58h` instead.
- **~CScriptThing:** keep it out of line. The inline form loses the predicate frame slot.
- **CTimer:** plain inline ctor / dtor, `__forceinline` operator= / GetTime.
- **Callees:** listed in the source header. All of them are bound labels.
- **New header:** engine/CV_ExposeMayorScript.h.

## 0x00EB8900 NScript::CV_MayorsInvitationScript::CMI_InfoKeeper::Main -- RELOCATION_MATCH (src/00eb8900.cpp)
- **Early return:** the HeroLearnedSecret test is an early return. Nesting the rest under `if (!HeroLearnedSecret)` adds an exit block.
- **Single-site wrap:** the first movie's implicit dtor is expanded at its normal exit only when the terminate check inside the
  first-loop StartScriptingEntity wait is wrapped in `do { } while (0)`. The source carries an `// Inline-split:` note. Wrapping all
  seven waits gives 20 difflines.
- **SecretsLearned:** compared as a float (fild + fucompp).
- **Callees:** GFCharStringToInt 0x0099E7F0 (__fastcall), handle IsValid / Release / GetScriptThing / Speak / IsPerformingScriptTask
  / dtor, and the movie ctor / dtor 0x006E7B60 / 0x006E7B80 through the MovieBase OUTOFLINE opt-ins.
- **New header:** engine/CV_MayorsInvitationScript.h.

## 0x00D82640 NScript::CQ_HeroSoulsBriarScript::CBriarRose::Main -- parked DIFFER(3286v3260), 38 difflines
- **Lever:** a `{ }` block around the CTimer / taunt part up to the idle loop makes every frame slot exact.
- **Residue 1:** the prologue CIsThingAlive(Me) expands the CScriptThing copy where retail calls 0x004ABE90. This is the open
  "copy inside inline special members" family (GOTCHAS chunk 786). IMPLICIT_COPY is still needed for the first SetIsPushableByHero
  by-value copy.
- **Residue 2:** the final idle loop duplicates its test where retail jumps to one bottom test. while / for / do-break / goto all
  normalise to the duplicated test.
- **Callees:** GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690, IsDistanceBetweenThingsUnder
  0x00CBE2FF.
- **New header:** engine/CQ_HeroSoulsBriarScript.h.

## 0x00D7FE90 NScript::CQ_HeroSoulsArenaScript::WatchForRoundsInArena -- parked DIFFER(2105v3148), 398 difflines
- **Matches:** every non-exit instruction, slot and register.
- **Residue:** VC7.1's per-site inline split of ~vector<CScriptThing> 0x008AC970. Retail expands it at 13 loop exits and calls it at
  the CreatureCount-loop exits and the wave-scope end.
- **Lever 1:** a declared-only `template<> std::vector<CScriptThing>::~vector();` makes every ~vector call out of line. Without it,
  every exit expands (4460 bytes).
- **Lever 2:** `do { ... } while (round < 3);` followed by the trailing block gives retail's round-loop rotation.
- **New headers:** CHeroSoulsWaveDef.h and CHeroSoulsCreatureDef.h. The CScriptDef include also needs FABLE_RETAIL_DEF_CLASS_BASE
  and FABLE_RETAIL_CDEFSTRINGTABLE; without the latter, CDefString.h hits `#error` and cl.exe hangs.

## 0x00DEA3F0 NScript::CQ_RansomVictimScript::CVictimsBrother::Main -- parked DIFFER(3056v3076), 161 difflines
- **Levers:**
  - first loop as `for (;;) { if (ParentClass.RescuedVictim) break; ... }`
  - a local parent reference for the seh_Brother release
  - the movie `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` declaration, which gives the
    PImp dtor 0x006E7AB0 at the expanded movie exit
  - inline ~CScriptThing and inline ~CTimer, which reproduce retail's per-site split
- **Residue 1:** frame-slot order. The filter temporary is at 0x68 (retail 0x58), and the marker and CScriptThing temporaries swap
  slots.
- **Residue 2:** placement of the four first-loop exit stubs.
- **Callees:** IsDistanceBetweenThingsUnder 0x00CBE2FF, IsDistanceBetweenThingsOver 0x00CBE3EA and FadeOutAndKillEntity 0x00CBE716,
  all __fastcall.
- **Script defs:** RV_HeroEvilMoralityValue comes from pGlobalScriptDef +0xf74.

## 0x00CFE030 NScript::CQ_AwakeningTheOracleScript::COracleTablet::Main -- parked DIFFER(3511v3575), 23 difflines
- **Residue:** retail expands ~CScriptThing (sparkles) once, in the exit block shared by the first in-loop and the post-loop
  terminate checks, and calls 0x004AA840 at every other exit.
  - Out of line (the parked source): every exit calls it.
  - Plain inline: VC7.1 also expands it at the final exit (183 difflines).
- **Spellings that fixed it:**
  - `Me.GetDataString() == "BAD_ONE" || ...` needs FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE plus FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ.
  - IsAlive on the sparkles local stays virtual only through `const CScriptThing* p = &sparkles`.
  - CScriptThing::operator== is defined inline as `PImp->IsEqualTo(*other.PImp)`. This is 0x008AAE20's own body
    (`mov ecx,[ecx+4]; ... jmp [edx+138h]`). The `PImp.Data->` spelling CSEs &Me into ebx and grows the frame by 4.
- **Callees and members:** in the differ header.
- **Header:** opt-in COracleTablet in the CQ_AwakeningTheOracleScript.h overlay. RemoveCursedTablets 0x00CFEE30 is a new label.

## Labels to correct (LABELS.tsv)
- 0x00CFEE30 has no label. It should be COracleTablet::RemoveCursedTablets.
- 0x00704580 carries the placeholder `_global::CCountedPointer_Set_00704580`. It should be CCountedPointer<CScriptThing>::operator=.
- The six rows' own labels are listed as well.

## Header debt
None. Every header need was met by a new header or an opt-in block (hdr/README.md).
