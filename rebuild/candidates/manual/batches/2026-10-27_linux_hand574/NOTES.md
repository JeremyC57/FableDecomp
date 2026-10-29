# Hand chunk 574 (NScript::CQ_FireHeartScript .. NScript::CV_MurderTwistFinalScript): 6 of 6 byte-exact

All six rows are RELOCATION_MATCH. They are compiler-emitted C++: no retail_asm, no MATCH-only rows. Each has a behaviour test
that prints `<addr>_TEST PASS` under g++ (tall.sh) and under VC7.1 (tall.py).

Dry vland (`tmp/vland.log`): WINS 6, no RELOC_IDENTITY. Two RELOC_NAME warnings come from wrong catalog labels (listed under
Label corrections). lv/ is deleted.

Every string literal was paired with retail by hand, because vland skips `$SG` / `??_C` relocations. `bin/rel.py` maps each
relocation offset to its retail target and compares the literal bytes: 0 mismatches. Every call target was checked the same
way and is the retail one, including scalar `operator delete` 0x00BFE9BC and scalar `operator new` 0x00BFEA1A.

Extents: each row has int3 padding before and after it, and its first and last instructions are `push`/`sub esp` and
`ret`/`ret 4`. The full-length matches prove the extents.

## What matched (6)

- **0x00D37D00 NScript::CQ_FireHeartScript::CPuzzleBlockMoon::Main** (753) and **0x00D384A0 ...::CPuzzleBlockSun::Main** (753,
  twin) -- the Fireheart sun/moon pad blocks. Each pad:
  - waits until Me's data string is no longer "NULL", then takes `PadNumber = GFCharStringToInt(Me.GetDataString())`;
  - waits until the hero is `PadRadius * 1.5` away;
  - then loops until the parent's MissionSucceeded flag is set:
    - on ResetBlocks: turns into the other sign when `StartingPattern[PadNumber]` is set (Moon) or clear (Sun), bumps
      NumBlocksReset, waits out the reset and the hero's distance;
    - otherwise, when the hero is within PadRadius and neither RoundEnded nor RoundBeginning is set: plays
      SND_COMBAT_MULITIPLIERLOCK_02/_04 and turns.
  - Both matched on the first probe. Shape: the rotated script wait idiom; `!(x != "NULL")` gives the `neg/sbb/inc` bool.
    `PadRadius * 1.5` is a double product (`fmul qword`) narrowed to the float argument. The trailing
    `if (IsActiveThreadTerminating()) return;` compiles to a bare call.
- **0x00DFC320 NScript::CQ_TraderConflictGoodScript::UpdateLiveEnemies** (777) -- rebuilds AllCreatures:
  - clears it and refills it with GetAllCreaturesExcludingHero;
  - erases chickens (def CREATURE_NEW_CHICKEN_04) and the quest's own people (TraderToRescue, BodyGuard, RingFighter,
    FisticuffsMember, Tyler);
  - then, for each entity following the hero, erases the first equal creature.
  - Shape: **one `else if` arm per name, each arm with the same `{ IATT; erase(it); }` body**. A single `||` over the
    GetDefName temporary adds conditional-destruction flags (838v777). An `if (def) X else if (a || b || ...) X` form is
    777v777, but VC7.1 then keeps the cross-jumped X block at the second arm and places it at the loop end. With six
    separate arms the first arm's block survives in place, as in retail. The followers pass is a nested search with `break`.
    It uses the real STLport 4.6 `vector<CScriptThing>` template (`_Vector_base` + `_STLP_alloc_proxy`). erase(pos),
    erase(first, last) and ~vector are out of line. The inline `CScriptThing::operator==` is
    `PImp->IsEqualTo(*other.PImp)` (slot 78).
- **0x00E56D10 NScript::CV_BookCollectingScript::CBS_Teacher::AdjustHeroReputation(EOpinionDeedType)** (745):
  - posts the hero's deed to Me;
  - then posts it to "boy0", "boy1", ... while GetThingWithScriptName finds a living thing, then to "girl0", "girl1", ... the
    same way.
  - Matched on the first probe. Shape: the first name is built in place; each next pupil is a name assignment plus a
    `CScriptThing` assignment. Both operator= are `__forceinline`; CCountedPointer::operator= reads other.Info, then
    other.Data, before the self-compare. `__forceinline virtual ~CScriptThing() { }` stores the vtable inline. VC7.1 merges
    the "girl" start block with its loop block by itself.
- **0x00E690F0 NScript::CV_ChapelOfEvilScript::OnPersist(CPersistContext&)** (759) -- 29 CPersistContext::Transfer calls (long
  0x00410BE0 / bool 0x004045C0 / float 0x00410620) in retail order with zero defaults. It uses the landed CV_AmbushScamScript
  OnPersist and ChapelOfEvil Init models and matched on the first probe.
- **0x00EAEA20 NScript::CV_MurderTwistFinalScript::Main** (732). Steps:
  - binds MurderTwistSheriff / SheriffGuard / Assassin / AssassinGuard, then calls both PostAddScriptedEntities;
  - waits until SheriffState is 4 or 3, then calls SetQuestAsCompleted("V_MurderTwistFinal", false, false, false);
  - in state 3, waits while the OakBay level is loaded;
  - finally calls DeactivateQuestLater(GetActiveQuestName(), 1).
  - Model: chunk-573 TrophyDealer Main. Second grade: the OakBay wait must be `while (!(!Game->IsLevelLoaded("OakBay")))`;
    the double negation gives retail's `test al,al ; setne bl` and loop layout, where a plain `while (IsLevelLoaded(..))`
    gives `mov bl,al`.

## Names and evidence (two sources each; spellings confirmed with objdump -t / rel.py)

- 0x00D37D00 `?Main@CPuzzleBlockMoon@CQ_FireHeartScript@NScript@@EAEXXZ`:
  - slot 1 of entity vtable 0x012CCED8, which the landed CPuzzleBlockMoon::Alloc 0x00D363A0 stores;
  - Ego_r.pdb 0x00D463A0;
  - it calls the PDB private member `?TurnIntoASunSign@CPuzzleBlockMoon@CQ_FireHeartScript@NScript@@AAEX_N@Z` 0x00D38000
    (catalog CPuzzleBlockMoon::TurnIntoASunSign; Ego_r 0x00D45FC0, with Main right after it).
- 0x00D384A0 `?Main@CPuzzleBlockSun@CQ_FireHeartScript@NScript@@EAEXXZ`:
  - slot 1 of entity vtable 0x012CCEF4, which the landed CPuzzleBlockSun::Alloc 0x00D36470 stores;
  - Ego_r 0x00D46B20;
  - it calls `?TurnIntoAMoonSign@CPuzzleBlockSun@...@AAEX_N@Z` 0x00D387A0.
  - Members (Ego_r):
    - CPuzzleBlockMoon/Sun ParentClass +0x14, PMasterData +0x18, PadNumber +0x1C;
    - CQ_FireHeartScript at retail offsets (Ego_r - 0x14 before the two maps, - 0x1C after them; the landed
      SetupPuzzleBlocks agrees): MissionSucceeded +0x49, ResetBlocks +0x8C, NumBlocksReset +0x90, RoundEnded +0x94,
      RoundBeginning +0x95, PadRadius +0xA4, StartingPattern +0xBC.
- 0x00DFC320 `?UpdateLiveEnemies@CQ_TraderConflictGoodScript@NScript@@QAEXXZ`:
  - Ego_r 0x00E3C790 / FableWin 0x038B7740;
  - donor order (WatchForKilledPeople -> UpdateLiveEnemies -> WatchForHittingEnemies -> WatchForRegionTransitions) is
    retail's 0x00DFC290 -> 0x00DFC320 -> 0x00DFC630 -> 0x00DFC920;
  - all six callers are members of the same class;
  - it rebuilds the PDB member AllCreatures (Ego_r +0x5C -> retail +0x48).
- 0x00E56D10 `?AdjustHeroReputation@CBS_Teacher@CV_BookCollectingScript@NScript@@AAEXW4EOpinionDeedType@@@Z`:
  - Ego_r 0x00EAA390;
  - it is the only one-argument CBS_Teacher member, and its argument is forwarded as the EOpinionDeedType of
    EntityPostOpinionDeedToRecipient (slot 627);
  - its only caller 0x00E55CE0 is CBS_Teacher::AskForBook: `ret 8` = (long, CCharString); it calls GetBookSpecificArgs
    0x00E57020 and reads MoralityReward +0x30 right before `AdjustHeroReputation(0x2C)`.
- 0x00E690F0 `?OnPersist@CV_ChapelOfEvilScript@NScript@@UAEXAAVCPersistContext@@@Z`:
  - slot 4 of quest vtable 0x012E8080 (slot 1 landed RegisterMain 0x00E69030, slot 3 landed Init 0x00E68FA0, slot 2 Main
    0x00E693F0);
  - every persisted string is the Ego_r member at retail offset + 0x14.
- 0x00EAEA20 `?Main@CV_MurderTwistFinalScript@NScript@@UAEXXZ`:
  - slot 2 of quest vtable 0x012EF978 (slot 1 landed RegisterMain 0x00EAE980, which spawns slot 2 as "Main"; slot 4 landed
    OnPersist 0x00EB6520);
  - Ego_r 0x00F225C0, right after RegisterMain as in retail.
  - Binding factories by string + PDB:
    - CMurderTwistSheriff::Alloc 0x00EAED10 (news 0x38, calls the landed CMurderTwistSheriff ctor 0x00EAED90);
    - CMurderTwistSheriffGuard::Alloc 0x00EB1F80, CMurderTwistAssassin::Alloc 0x00EB3C60 and
      CMurderTwistAssassinGuard::Alloc 0x00EB5450, all landed under these names;
    - the Ego_r donor order of the four Allocs is retail's.
  - SheriffState +0x48 = Ego_r +0x5C.
- Callees: bound PDB spellings were reused, and the new ones were checked against the PDB name sets.
  - CScriptThing methods: GetName / GetDefName / GetDataString (slots 1-3), IsAlive (slot 75) and protected IsEqualTo
    (slot 78, FableWin `?IsEqualTo@CScriptThing@@MBE_NABV1@@Z`).
  - CGSI slots from ghidra_out/cgsi_slots_aligned.tsv: 7, 11 IsLevelLoaded, 64, 70, 72, 84, 231, 281, 288, 627, 655, 690
    PlaySoundOnThing.
  - Free functions:
    - GFCharStringToInt 0x0099E7F0;
    - GFIntToCharString 0x0099F830;
    - `??H@YI?AVCCharString@@PBDABV0@@Z` 0x0099F690;
    - `??9CCharString@@QBE_NPBD@Z` 0x0099E960;
    - `??4CCharString@@QAEABV0@ABV0@@Z` 0x0099EFB0;
    - IsDistanceBetweenThingsUnder 0x00CBE2FF / IsDistanceBetweenThingsOver 0x00CBE3EA.
  - Member functions:
    - `?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ` 0x00F35B30;
    - CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities
      0x00CB8930;
    - STLport vector<CScriptThing> erase 0x00CD3152 / 0x008AC9D0 and ~vector 0x008AC970.
- Descriptive or unproven:
  - The ESheriffState enumerator names are not in the PDB dumps, so the states are written `(ESheriffState)3/4`.
  - The TurnInto*Sign parameter is left unnamed.
  - CGSI / CScriptThing padding virtuals (gNNN / tNNN) only fill slots and are not emitted.

## Parked for Fable 5.1 (0)

## Skipped
- none.

## Label corrections
| Address | Manifest / catalog label | Correct name |
|---|---|---|
| 0x00D37D00 | sub_00d37d00 | NScript::CQ_FireHeartScript::CPuzzleBlockMoon::Main |
| 0x00D384A0 | sub_00d384a0 | NScript::CQ_FireHeartScript::CPuzzleBlockSun::Main |
| 0x00DFC320 | CScriptBase_ProcessNPCConversations | NScript::CQ_TraderConflictGoodScript::UpdateLiveEnemies |
| 0x00E56D10 | CScriptBase_AskForBook_Impl | NScript::CV_BookCollectingScript::CBS_Teacher::AdjustHeroReputation |
| 0x00E690F0 | sub_00e690f0 | NScript::CV_ChapelOfEvilScript::OnPersist |
| 0x00EAEA20 | sub_00eaea20 | NScript::CV_MurderTwistFinalScript::Main |
| 0x00EB1F80 (callee, vland RELOC_NAME) | _gapscan::sub_00eb1f80 | NScript::CV_MurderTwistFinalScript::CMurderTwistSheriffGuard::Alloc (already landed under this name) |
| 0x00CB8930 (callee, vland RELOC_NAME) | CTCVillage::OnInitialActivate | CScriptBase::PostAddScriptedEntities (bound spelling; chunk 573 noted it too) |
| 0x00EAED10 (callee) | (no catalog row) | NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::Alloc (not landed) |
| 0x00EB6A00 (landed, quest vtable 0x012EF978 slot 3) | CV_SwordInTheStoneScript::Init (landed source CVSwordInTheStoneScript_Init_00eb6a00.cpp) | NScript::CV_MurderTwistFinalScript::Init: its only reference is slot 3 of the CV_MurderTwistFinalScript vtable 0x012EF978, and its vectors +0x60 / +0x7C are exactly Guards / seh_Guards (Ego_r +0x74 / +0x94, less 0x14 and the 4-byte vector shrink); Ego_r ?Init@CV_MurderTwistFinalScript@NScript@@UAEXXZ (NAME_FIXES.tsv) |

The landed spellings this chunk proved wrong are in NAME_FIXES.tsv.

## Header debt
- engine/CScriptThing.h and engine/CGameScriptInterface.h are flattened data views without the vtables these rows call
  through. Every row models them locally, as the landed chunk 570-573 siblings do; vland raised no SHARED_HEADER warning.
- No engine headers exist for CScriptBase, CEntityScriptBase, CEntityScriptBinding, CPersistContext or the six script classes.
  All are local models at retail offsets.
- The STLport vector is a minimal real-template model in namespace std: `_Vector_base`, `_STLP_alloc_proxy`, out-of-line
  erase / ~vector. FireHeart's StartingPattern is a byte-based `std::vector<bool>`, as the landed SetupPuzzleBlocks 0x00D36140
  models it, because retail indexes it per byte. This disagrees with the owner note that retail vector<bool> is 0x14 bytes;
  in this class every member offset fits 0x0C.
