# Wave 3: RTTI-less NScript vtables named from TLC's own registrations (2026-11-04)

Read-only wave. Nothing in the repo was changed. Apply `labels_alloc.tsv` (2,420 rows) or one of its part files.

## Method (all evidence is inside TLC Fable.exe, plus the PDB name sets)
1. **Registration sites** (`tools/regscan.py`). Each `T::Main` inlines `new CEntityScriptBinding<T>(name, this, &CInner::Alloc, flags)`:
   `push "<name>"; call CCharString 0x0099EBF0; mov [B], <binding vt>; ... mov [B+8], this; mov [B+0x10], <Alloc>; mov byte [B+0x14],1; mov [B+0x18], flags`.
   The scan found 613 `mov [r+0x10], imm` sites, plus 7 register-form sites (`mov ebp, Alloc ... mov [edi+0x10], ebp`, in OrchardFarmRaid and PrisonEscapeRescueMother).
   Total: **620 registrations in 119 enclosing Mains (119 outer scripts), 614 distinct Allocs.** An Alloc is accepted only if its body
   calls operator new 0x00BFEA1A and stores the counted-pointer deleter 0x00CDEE00, or has a fastcall `ret 8` epilogue
   (CArcheryCompetitionSignpost's Alloc 0x00CDEB40 has no manifest start of its own).
2. **Alloc -> vtable**: walk the Alloc and take the last `mov [obj], imm(.rdata)` after `new`. The inline ctor stores the
   CEntityScriptBase vtable 0x012C3224 first, then the class vtable. When the Alloc calls an out-of-line ctor (19 cases), the walk
   follows it. Cross-check (`tools/xcheck.py`): each of the 614 vtables has exactly one vtable store in .text, inside its own
   Alloc or ctor. 3 vtables have extra byte matches, and all of them fall inside unrelated `or byte [reg+0x12e],8`-type instructions.
   Every one of the 614 vtables has slot 3 = `mov eax,[ecx+0x14]; ret` (GetParentScript).
3. **Name** (`tools/name.py`). Outer script = the catalog/manifest name of the enclosing function. Two checks must agree:
   (a) the PDB Alloc set of the outer holds the `C<string>` names, and (b) the other refs=1 slots of the RegisterMain-anchored
   script vtable name that outer. If the enclosing function has no name, the outer is the only NScript class whose PDB
   `?Alloc@C<s>@X@NScript@@SI...` set covers every registered string, and (b) must agree with it. Inner = `C<string>` (PDB exact),
   or by elimination for shared Allocs (below). Verdicts: 610 OK, 10 STRING_MISMATCH_ELIM, 0 unresolved.
4. **Slots**: entity layout 0 sdd, 1 Main, 2 Init, 3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
   A slot is renamed only if its dword occurs exactly once in the whole exe and the PDB lists that member for that exact
   `Inner@Outer@NScript` (sdd: `??_G` or `??1`). The same rule covers the 5-slot level-script vtable
   (0 sdd, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist; slot 1 must push the "Main" literal 0x012C2F94).
5. **Follow-ons from the same evidence**:
   - `templates.py`: `CEntityScriptBinding<T>` = the binding vtable stored at T's registrations (slot 0 GetEntityScriptName, body
     `lea eax,[ecx+4];ret`). `CActiveEntityScript<T>` = the vtable its CreateActiveEntityScript stores after new. `CSpawnedFunc<T>` =
     the vtable `T::RegisterMain` stores after new(0x3c), slot 4 SuspendableProcess.
   - `scriptalloc.py` / `scriptreg.py`: the level-script static Alloc and ctor via `CScriptInfoManager::RegisterScripts` 0x00CD52D0
     (name literal "<N>", then factory, then vtable; class `C<N>Script`, PDB `?Alloc@C<N>Script@NScript@@SIPAVCScriptBase@@`). This
     also covers 41 scripts that have no entity registrations (Expression_*, Global_*, test scripts).
   - The CEntityScriptBase sdd 0x00CDEBF0.
   - Validation: the template chain reproduced every already-named template slot. 119 CActiveEntityScript<T>::SuspendableProcess,
     116 CreateActiveEntityScript and 119 CSpawnedFunc<T> sdd are SAME, with 0 disagreements.

## Rows (`labels_alloc.tsv` = union, no address conflicts, no duplicate names)
| part file | rows | content |
|---|---|---|
| labels_entity.tsv | 1,814 | entity slots + Alloc + ctor, and the outer level-script slots of the enclosing scripts |
| labels_templates.tsv | 392 | 119 binding GetEntityScriptName, 117 CActiveEntityScript<T> sdd, 156 CSpawnedFunc<T>::SuspendableProcess |
| labels_scriptalloc.tsv | 101 | 86 level-script Allocs + 15 ctors (outers proven by step 3) |
| labels_scriptreg.tsv | 150 | 41 more level scripts from the registry: Alloc, ctor, script slots, CSpawnedFunc slots |
| labels_base.tsv | 1 | CEntityScriptBase sdd |

Kinds: 2,011 replace placeholders or absent names; 365 complete under-qualified names (e.g. `COF_MainGates::Main` ->
`NScript::CPersonalScript_GlobalThingsScript::COF_MainGates::Main`, evidence "was under-qualified"); 44 replace provably wrong names
("replaces ..." with the reason: the name's real owner is another vtable slot found here, or a duplicate holder, or no such PDB
member, or a non-virtual PDB member). 2,088 rows are landed (flagged `LANDED row (respell source)`). 10 rows are also in
labels_high.tsv with the same name.

## Wrong names found (included as rows, provably wrong)
- Six level-script `Main`s carried their own `RegisterMain` name, so two addresses had one name: 00d5ab40, 00d5dd40, 00e90830,
  00eed360, 00efca00, 00efe4b0. Each is slot 2; slot 1 is the real RegisterMain.
- 00decad0 and 00e00db0 are manifest-named `CV_TravellingHeroesScript::Main`, but they are `NScript::CQ_SecretPassageScript::Main`
  (10 registrations) and `NScript::CQ_TraderEscortScript::Main` (14 registrations). The real CV_TravellingHeroesScript::Main is 00eded10.
- Script Init slots carrying a sibling's name: 00d7ad10 (was CChestHidingAssassin::Init) -> CQ_HeroSoulsThunderScript::Init;
  00dc25b0 (was CQ_SecretPassageScript::Init) -> CQ_OpeningGraveyardSecretPassageScript::Init; 00eb6a00 (was CV_SwordInTheStoneScript::Init)
  -> CV_MurderTwistFinalScript::Init.
- 31 entity slots carried a sibling's or a foreign name (propagated labels). Examples: CMK_HS3/HSE_SOUL::Main both named CMK_HS2_SOUL::Main;
  three CDummy::Main named CEndGameScarletRobe::Main; 00d43190 CKillBird::Main was CPretendGuard::Main; 00e10bf0 was
  CV_ChapelOfEvilScript::WatchForLeaving (non-virtual in PDB).
- 0x00DCDE20 is not `CGuardTeamMember::Alloc`, because the PDB has no such class: it is `CCrateTeamMember::Alloc`.
- 0x00CDEB40 is not `_global::CreateArcheryCompetitionSignpost`: it is `CArcheryCompetitionSignpost::Alloc`.

## Shared Allocs registered under several strings (STRING_MISMATCH_ELIM)
| strings | Alloc | vtable | class (PDB) | rule |
|---|---|---|---|---|
| GuardTeamSpawn, BanditTeamSpawn | 00dcd0e0 | 012db2d4 | CQ_OrchardFarmRaidScript::CTeamSpawn | suffix "TeamSpawn"; ctor 00dcd160 already catalog CTeamSpawn |
| GuardTeamMember, BanditTeamMember | 00dcde20 | 012db364 | CQ_OrchardFarmRaidScript::CCrateTeamMember | last unmatched PDB Alloc |
| StoreRoomDoor, TortureChamberDoor, BarracksDoor | 00dd90c0 | 012dc544 | CQ_PrisonEscapeRescueMotherScript::CPrisonKeyDoor | last unmatched PDB Alloc |
| RandomPopulationVillager/Guard/Bandit | 00ec1b70 | 012f1390 | CV_RandomPopulationSimScript::CRandomPopulationCreature | sole PDB Alloc |

## Anniversary cross-check (614 entity vtables)
548 agree, 61 have no Anniversary candidate, and 5 disagree (`disagreements.tsv`). The disagreeing candidates are the off-by-one
neighbour rows of the order matcher. Examples: 012d29b8 (medium) says CActiveEntityScript<CQ_HangingTreeGoodScript>, the registration
says CHTG_Executioner (Alloc 00d731a0, as in chunk 744's notes); 012e70c8/e4/7100 (CV_BodyGuardScript, medium) are each shifted by one.
The low-confidence CFH_* and CClickableDoor rows are also shifted, and the registration agrees with the strings in each Main
(CFH_Prisoner Main uses CAM_FH_PRIS, ProphetCage uses OPENCAGE, FH_Scythe uses CS_FIREHEART_SCYTHE).

## Kept, not overridden (`disagreements.tsv`)
11 slots carry descriptive non-placeholder names: Quest009_Gamesmaster_*, Script_Minion_Summoner, Script_Guard_Prison_Race,
CScriptGameResourceObjectScriptedThingBase_Handle*, CCreatureAction_TrollWhackGroundBase_Action, Script_TourGuide_Main,
Script_TravHeroWhisper_Main, Script_Villager_Reputation_Dialog, Script_PickpocketExpression. For each, the PDB name is in
disagreements.tsv (e.g. 00d067a0 = CBCGameMaster::Main). 00ce0ef0 carries an `ExternalRetry_00ce0ef0::` tool prefix.

## Name clashes for the coordinator (`name_clashes.tsv`)
Six catalog rows at other addresses carry names that this wave proves belong elsewhere: 004538eb, 00487120, 00487150, 00594f28,
005da4e0, 00431773. All have unrelated bodies (two are unaligned starts) and should lose those names when these labels land.

## Placeholder accounting (`tools/remain.py`, `remaining.json`)
The catalog plus manifest hold 799 distinct `CVtable_<va>` classes (case-folded). The ~690 in the brief is an underestimate, and
only 36 of them were entity vtables: most entity classes were already named, with placeholder slots.
After this wave, 435 are cleared (36 entity, 82 level-script, 40 more level-script via the registry, 276 template, 1 CEntityScriptBase).
**364 remain:**
- 344 are not NScript at all: engine/library vtables, 340 of them outside the NScript .rdata range, with functions mostly in
  0x00C0xxxx-0x00CExxxx (vfunc_N / sdd-only placeholders). They need the def/engine waves, not registrations.
- 10 CSpawnedFunc/CActiveEntityScript-shaped vtables: the slot has refs>1 (00e0e970, 00d30000, 00da0130), the storers have
  under-qualified names (TestTheInventory, Gameflow, HauntedHouse, CS_OakValeRevisited), or there is no storer chain (012c2f9c base
  CSpawnedFuncBase etc.).
- 7 CDefClassBase-derived (def/resource) vtables (012fb2f0 etc.; the 5 def classes of the high wave plus 012ccc50 and 012e5ff0).
- 1 script sdd with refs=2 (00d80c00, CQ_HeroSoulsArenaScript), 1 script Alloc with refs=3 vs 1 registry site (00e9c300), and CVtable_ffffffff.
- Not renamed by rule, but implied: the CEntityScriptBase base slots 00cdebb0 Init, 00cdebc0 OnPersist, 00cdebd0 OnPredicateFail and
  00cdebe0 OnInterrupted (refs>1 because every derived vtable inherits them; PDB lists all four on CEntityScriptBase).

## Recheck of 12 applied names (`recheck_high.tsv`)
All 12 are **confirmed** by TLC-internal chains. The 9 entity classes are confirmed by registration, Alloc and vtable store. The
3 level scripts are confirmed by RegisterScripts name, factory and vtable.

## Files
labels_alloc.tsv (deliverable), labels_{entity,templates,scriptalloc,scriptreg,base}.tsv (parts), review.tsv (all 620 registrations
with verdicts), slots.tsv / templates_log.tsv / scriptalloc_log.tsv / scriptreg_log.tsv (every proposal and skip), disagreements.tsv,
name_clashes.tsv, recheck_high.tsv, remaining.json, scan.json, tools/ (regscan, name, templates, scriptalloc, scriptreg, merge,
remain, xcheck, recheck). Re-run order: regscan -> name -> templates -> scriptalloc -> scriptreg -> merge -> remain -> recheck.
