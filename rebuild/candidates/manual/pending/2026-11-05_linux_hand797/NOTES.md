# Hand chunk 797 (NScript::CV_BanditCampPathScript .. NScript::CQ_BountyHuntScript::CBH_GangDeputy2): 3 of 6 byte-exact

Each source's header lists its callees: the retail address and the bound or PDB spelling, checked with `objdump -r` against the
oracle and relpair797. The matched rows have 0 string mismatches and every bound callee binds the same. Every symbol name comes from
the Ego_r.pdb / FableWin.pdb name sets plus a vtable slot or caller. No descriptive symbol names are used. Only locals are
descriptive: seh_me, seh_hero, escort, escort_gone, speech, sex_suffix, conversation, deputy_pos and similar. Enum values with no
PDB enumerator name are written as casts: `(EBadDeeds)2`, `(ETextGroupSelectionMethod)1`, `(EOpinionReactionType)0x22`.

## What matched (3)

All three are RELOCATION_MATCH, with tests passing under g++ and VC7.1.

- **00E46D30 NScript::CV_BanditCampPathScript::Main** (2 grades).
  - What it does: binds BCP_Bandit, waits for BanditCampPath_1, takes the hero under scripted control, fades out, drops Q_OakValeBanditRaid, sets the save marker, then loops on the overheard-conversation flag.
  - Shape lesson: retail expands the CWideScreenMagicPauseEntities ctor inline. FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR does it.
  - New header engine/CV_BanditCampPathScript.h.
- **00E18630 NScript::CQ_WhiteBalverineWWScript::Main** (4 grades).
  - What it does: binds KG_Chief and the white and soldier balverines, posts them, waits for WitchWood_7, sets objective 07, and spawns the MonitorBalverine and SpawnBalverines threads (`CSpawnedFunc` named `CCharString("ParentClass.") + CCharString("<Fn>")`).
  - Shape lesson: retail inlines both the CScriptThing and the ScriptedThingBase operator=. That needs FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT and the OUTOFLINE_ASSIGN declaration, plus TU-level `__forceinline` bodies for both (precedent: 16 and 1 landed sources).
  - New header engine/CQ_WhiteBalverineWWScript.h.
- **00F0CAA0 NScript::CQ_AmbushTradersScript::CAT_ConvoyMember::Main** (2 grades).
  - What it does: waits for one of the three convoy regions and takes scripted control (priority 4 in Bowerstone Jail, else 2). In the Deserted Farm it holds "WaitForTrigger" members. It records a TeleportData (defName, seed, health, scriptName, items) and pushes it onto TeleportInfo.
  - Shape lesson: CScriptThing::GetPos must be spelled `if (PImp.IsNull()) return NullCoord; return PImp->GetPos();`. The ternary form is not inlined (it becomes a call to 0x004AA980, 176 diff lines).

## Parked for Fable 5.1 (3)

- 00DA91A0 NScript::CQ_MinionClifftopChaseScript::CMapKeeper::Main DIFFER(2765v2765)
  - Differs: at the escort loop head retail hoists `lea ecx,[esp+0x1c]` above the terminate `jne` (exit entered at 0x00DA9C57); VC7.1 emits `mov eax,[esp+0x1c] ; lea ecx,[esp+0x1c]` after the jne (exit at 0x00DA9C53). 1 instruction, scheduling only.
  - Tried: 10 grades: GetTime forceinline, IMPLICIT_ASSIGNMENT plus a single-site do-while (Inline-split), escort bound to a reference (virtual IsAlive, as retail), do-while / while / for(;;) loops, the flag before or after the escort, `== false`, a flag instead of break, a by-value escort (devirtualises IsAlive).
  - Next: a spelling whose first IsAlive `this` comes first in the fall-through; else lane gap: branch-hoist scheduling.
- 00DADF80 NScript::CQ_NewOakValeIntroScript::CNOVI_Villager::Main DIFFER(2790v2787)
  - Differs: the eight speech-list picks. Retail sizes each list as `mov eax,[ecx+end] ; mov edx,[ecx+begin] ; add ecx,begin ; sub eax,edx`. With `.size()` we match byte count but swap registers (parent eax, size ecx: 192 diff lines). With `end() - begin()` we match registers but fold to `sub eax,[ecx+begin]` and lose the dead add (3 bytes short, tail shift, 58 diff lines).
  - Tried: 10 grades: OUTOFLINE_DTOR on/off, pointer ParentClass, (long) cast, *(begin()+i), begin()[i], named index, end()-begin(), size_type(end()-begin()), size()-0.
  - Next: a `.size()` form that keeps the vector `this` in ecx, e.g. STLport 4.6 `size()` as `size_type(end() - begin())` in an opt-in vector branch; else lane gap: register colouring.
- 00D18DF0 NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main DIFFER(3337v3321)
  - Differs: at the two MovieBase dtor sites the user-declared forceinline dtor adds `mov [esp+0x50],<MovieBase vtable>` (8 bytes each). Retail expands the compiler-generated dtor: `call 0x006E7AB0 ; mov [esp+0x50],0x0126008C ; call 0x0099A430`, with no own-vtable reset.
  - Tried: 14 grades: predicate scope forms, body in else, do-while(0) around the movie block and around three exits (also a 20-wrap probe sweep), forceinline pair (C2908 fixed by declaring the PImp dtor specialization before the header), ctor-only forceinline (14 diff lines: the implicit dtor stays the COMDAT call 0x006E7B80), plain inline PImp release.
  - Next: make VC7.1 expand the implicit dtor in a 3.3 KB body; else lane gap: inline budget for a compiler-generated dtor.

Row details for the parked rows:

- 00DA91A0:
  - Escort spelling: `CScriptThing& escort = Game->GetNearestWithScriptName(Game->GetHero(), "MinionEscort");` (the hidden-return temporary bound to a reference) gives retail's virtual `[eax+0x12c]` IsAlive calls.
  - The loop is `bool escort_gone = false; do { ... } while (!escort_gone);` with a `break` when the hero leaves the escort.
  - Line 4 of the differ source carries the `// Inline-split:` note: line 2 holds the Grade, as the parked format requires.
- 00DADF80:
  - The loop-exit handle dtor must stay plain inline: do not define OUTOFLINE_DTOR.
  - CTimer GetTime and operator=(long) are inline through `extern CGameScriptInterfaceBase* pDSTGame;` (0x0143E8F8).
  - The a2 `.size()` spelling (2787v2787) is the alternative if landing prefers same size over fewer diff lines.
- 00D18DF0:
  - The source declares `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` (0x006E7AB0) right after engine/CCountedPointer.h, before the movie header.
  - DELTA_NOTIONAL_ZERO (0x0129BA3C) is a local `extern const float`.
  - The pGlobalScriptDef fields come through engine/fable_script_def_global.h: BountyHuntBanditDeputy2WanderDistance +0xb34 and BountyHuntBanditDeputy2AttackProximity +0xb38.

## Skipped

None.

## Label corrections

All are in NAME_FIXES.tsv (22 lines).
- 0x00DAEBF0: the landed GetVillagerSpeechIndex is a global `CNOVI_Villager`. It belongs nested in NScript::CQ_NewOakValeIntroScript, and is private.
- 0x00DAEA70: AddBadDeed takes the enum EBadDeeds, not long. reloc_bindings also has a spelling without NScript.
- The landed CQ_NewOakValeIntroScript::Main 0x00DABAC0 binds all 16 nested-script Allocs as `C<Name>` (CVillager, CBully, ...). The PDB spelling is `CNOVI_<Name>`, and `COVI_DeadFather` for DeadFather.
- 0x00F0D470 is landed as `SScriptQuery_00f0d470`. It is the CQ_AmbushTradersScript TeleportData ctor.
- 0x00D15140 is landed as `CMainGameComponentInit`. It is the CQ_BountyHuntScript TeleportData ctor: 24 bytes with four CCharStrings, while CMainGameComponentInit is 16 bytes of wide strings.
- 0x00CDBC50 is a local helper `CLandscapeBackgroundPatch_dtor_00cdbc50` in the landed 0x00CDBD00. It is ~TeleportData of CQ_AmbushTradersScript. Review 0x00CDBD00's class too.
- 0x00CDBFB0: the cast-map dtor (std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map) is still catalogued as the placeholder `CRetailTree_erase00cd344a`. The vland RELOC_NAME warning on 00E18630 / 00E46D30 points at it, and four spellings are bound to the address.

## Header debt

None beyond hdr/README.md. Landing choice: the MovieBase FORCEINLINE opt-in's dtor half is only right as far as the inline expansion goes. See README.
