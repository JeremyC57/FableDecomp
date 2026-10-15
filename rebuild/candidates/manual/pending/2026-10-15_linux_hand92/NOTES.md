# Hand chunk 92 (CTCInventory .. CTCInventoryBase): 38 of 40 byte-exact

All 38 src/ rows grade MATCH or RELOCATION_MATCH (grades.txt) and their behaviour tests print PASS under g++ (tall.sh)
and under the lane's VC7.1 cl.exe (tall.py). Many catalog labels in this chunk are propagated: line 1 of each source
carries the class/method the evidence supports. See "Label corrections".

## What matched (38)

- **CTCInventory** (retail layout: a 4-byte EditBoxes list and 12-byte vectors put QuickAccessItems at +0x148 and the
  EnableQuickAccessAssign/Remove bytes at +0x1CC/+0x1CD; local view, the PDB header does not fit)
  - 00587749 ProcessDPadUpReleased: confirm a pending quick-access assign, else cancel a pending removal
    (`QuickAccessItems[0] = 0`), else tail-jump the base version. `optimize("s")` gives the shared ebx zero and the
    `cmp byte ptr [..],bl` flag tests.
  - 0058a729 ConstructStyleCardsScreenPC / 00591e5e ConstructScreenPC: two front-end-manager virtual calls (each
    through its own GetInstance() call) then the list-component builder; `optimize("s")` for the pass-through pushes.
- **CFaction** (catalog "CTCInventory::Copy", propagated)
  - 006b94e0 SetAlliedToFaction / 006b9530 SetEnemyOfFaction: `set<CCharString>::erase` on one set then `insert`
    into the other; the insert's pair<iterator,bool> needs a user constructor to come back through the hidden pointer.
- **CArray<C3DGroup2>** 00ae8490 AddDefault: `push_back(C3DGroup2())`, a bare temporary (push eax from the
  constructor) whose inline CArray destructor is `if (first) free(first)`.
- **No-RTTI 0x00C1-0x00C4 modules**
  - 00c17820 TestThing: `__fastcall` member wrapping the thing in a NAMED message-event local (retail re-derives
    `lea eax,[esp+4]`) and returning the bound filter's bool.
  - 00c27a20 CancelPending: CScopedLock (8 bytes) + `__stdcall` notifier + flag clear, no EH frame.
  - 00c2d920 CSoundFormat::GetPCMBufferSize: Size read into a local first (stays in esi); the PCM twin's three zero
    words are initialisers and SampleRate/Channels/Encoding BODY assignments (retail stores the rate before the two
    16-bit fields); `r > size ? r : size`.
  - 00c443d0 ClearMarkedItems: slot indexed at every use (VC CSEs the address into eax with `lea`, re-reads it for the
    call after the aliasing store) and the 8-byte record cleared by one `__int64` store.
- **CTCHeroStats** 0057aa1b GetNumberOfRegionsVisited (catalog "GetNumAssignedReceivedAbilities", propagated): count of
  non-zero RegionTime (+0x124) entries, `i != size()` under `optimize("s")`.
- **CTCInventoryAbilities** (shipped layout: base 0x14 narrower, 12-byte vectors: ExperienceSpentOnAbilities +0x148,
  ReceivedAbilityOrder +0x154, Abilities +0x160 (8-byte {level, def}), AbilityButtons +0x16C, ReceivedExpressionOrder
  +0x1BC; retail CHeroAbilityDef: Ability +0x3C, IconGraphicIndex +0x44, ExperienceCostsToUpgrade +0x80)
  - 005ee146 / 005ee24b counting loops (`!= 0` / `> 0`), `i < size()` + `optimize("s")`.
  - 005edea3 GetAbilityMaximumLevel: y-off + s; `level < 0 ? 0` stored into the address-taken local, then a
    Dinkumware-style `Min(level, MAX_ABILITY_LEVEL)` over a `static const long` (address select `cmp ; jg ; lea`).
  - 005ee0e7 SetAllAbilitiesAsAvailable and 005f3e7d / 005f3eca / 005f3f14 ForceAllAbilitiesTo{MaxLevel,Level,
    NextLevel}: signed index loops under `optimize("s")` (`1 < size()` folds to `and eax,~7 ; cmp eax,8`).
  - 005eef84 ForceAbilityLevelTo: `this` kept in EDX across GetAbilityMaximumLevel -- reproduced by defining the real
    GetAbilityMaximumLevel body EARLIER IN THE SAME TU (see LESSONS); clamp `if (<0) 0; else if (>max) max`.
  - 005ee207 GetExpressionPDef: y-off + s; converting CDefPointer<CDefClassBase> -> CDefPointer<CExpressionDef>
    with `__forceinline` converting constructor and release accessors.
  - 005ee31d GetSpecialAbilityGraphicSetToButton: `optimize("s")` + `__forceinline` CVectorMap::Find (the
    `(it == end() || key < it->key) ? end() : it` ternary), caller re-reads end().
  - 005eed8c GetPrioritisedExpressionDefs, 005f4c45 GetAvailableAbilityDefs (reference to the entry's def pointer),
    005eefb7 AddAbilityToCorrectList (if/else arms with the same call tail-merge), 005f3e3e InitialiseExpressions
    (implicit `"EXPRESSION"` CCharString temporary, `vector::resize`).
  - 008631b0 (really CCreatureInteraction::RemoveCreature): derived-to-base argument conversion temp in the arg slot.
  - 009bff10 (really CDisplayManager::ChangePresentationInterval(bool, long, bool)): inline CPresentationIntervalDesc
    local built from the three arguments.
- **CTCInventoryBase / CCategory** (retail: Categories +0x20 with 0x2C-byte CCategory, CanItemBeQuickAssigned +0x34,
  CanItemBeUsed +0x35, Dying +0x88, InventoryAnimRenderBox +0xBC)
  - 005bc4df SetCanItemBeUsed / 005bc505 SetCanItemBeQuickAssigned: `optimize("s")` for the shared epilogue.
  - 005bc527 AddCloseInventoryEvent: `/GS` + y-off + s; inline CGameEvent constructor, inlined AddUBYTE(0).
  - 005bc6b3 GetItemDescription: y-off + s; `*out = Lookup(index)` with the `__fastcall` hidden-pointer temporary homed
    in the dead index slot.
  - 005bc754 CCategory::OnKill: VC7.1 keeps the EMPTY item loop (inline empty per-item hook).
  - 005bc9c8 CTCInventoryBase::OnKill: count hoisted and `this` dropped because CCategory::OnKill is defined above it
    in the same TU (side-effect knowledge).
  - 005bca05 / 005bca46: per-category forwarding loops to the landed CCategory bodies (other TU: re-read everything).
  - 005bca87 IsItemSelected: same-TU read-only getter lets VC strength-reduce the element pointer from one read of
    `first` while still re-reading size().
  - 005bccf1 DeselectAll: inline CCategory::Deselect (`or -1` / `and 0` stores under `optimize("s")`).
  - 005bce49 DrawInventoryText: y-off + s; in-place `GFACoordToVCoord(&pos, &pos)`, two alignment arms tail-merging
    `pos.X -= width (* 0.5f)`.

## Parked for Fable 5.1 (2)

- 005bc58f CTCInventoryBase::SetInventoryAnimRenderBox DIFFER(35v35): `lea eax,[ecx+0BCh] ; mov ecx,[esp+4]` (dest in eax, source in ecx) vs ours `mov eax,[esp+4] ; ... add ecx,0BCh` | tried: POD assignment, reference local to the member, optimize("s") (movsd x4), memberwise inline operator= | next: a C2DBoxF member function on the destination (`InventoryAnimRenderBox.Set(box)`) or a copy through a `C2DBoxF*` destination local declared before the argument is read; else lane gap: register allocation of a 16-byte copy

## Skipped

- none (no row holds two functions).

## Label corrections

- 005bc4df: "CTCInventoryBase::SetCanItemBeQuickAssigned" -> CTCInventoryBase::SetCanItemBeUsed (stores +0x35 = CanItemBeUsed;
  0x005BC505 is the real SetCanItemBeQuickAssigned at +0x34).
- 005bc754: "CTCInventoryBase::SetQuestPersistentFlagForItems" -> CCategory::OnKill (no argument, CCategory item loop).
- 005bc9c8: "CTCInventoryBase::SetQuestPersistentFlagForItems" -> CTCInventoryBase::OnKill (vtable 0x01253DBC slot 14; sets Dying).
- 005bca46: "CTCInventoryBase::SetQuestPersistentFlagForItems" -> CTCInventoryBase::UpdateNonQuestPersistentRefCountForItems(bool).
- 006b94e0 / 006b9530: "CTCInventory::Copy" -> CFaction::SetAlliedToFaction / CFaction::SetEnemyOfFaction (leaf names descriptive;
  callers CGameScriptInterface::SetFactionAs*Faction).
- 00ae8490: "CTCInventory::SetHighlightedItemAsQuickAccessItem" -> CArray<C3DGroup2>::AddDefault (descriptive leaf).
- 00c17820: "CTCInventory::Copy" -> a `__fastcall` message-event filter wrapper (descriptive CMessageEventThingFilter::TestThing).
- 00c27a20: "CTCInventory::ProcessInventoryInputLeft" -> a virtual of the no-RTTI vtable 0x012B4048 (descriptive CancelPending).
- 00c2d920: "CTCInventory::Copy" -> CSoundFormat::GetPCMBufferSize (descriptive).
- 00c443d0: module "CTCInventory" is wrong (no-RTTI 0x00C4 table); leaf ClearMarkedItems kept.
- 0057aa1b: "CTCInventoryAbilities::GetNumAssignedReceivedAbilities" -> CTCHeroStats::GetNumberOfRegionsVisited (descriptive).
- 008631b0: "CTCInventoryAbilities::AddLearnedExpression" -> CCreatureInteraction::RemoveCreature (descriptive; caller
  CThingCreatureBase::RemoveFromCreatureInteraction 0x006629B0).
- 009bff10: "CTCInventoryAbilities::AddLearnedExpression" -> CDisplayManager::ChangePresentationInterval(bool, long, bool).
- Callees: 0x005BC99C (landed "CCategory::GetHighlightedItem") reads SelectedItemIndex (+0x20) -> GetSelectedItem;
  0x009AD3B0 "LTextSystem::GetIDFromSymbol" -> CDefinitionManager::GetNumberOfDefsOfType(const CCharString&);
  0x004433F0 "_Umove<EWeaponClass*>" -> vector<long>::resize(n); 0x004BB270 / 0x004AC380 -> set<CCharString>::erase / insert;
  0x00C2D800 / 0x00C2D820 "CTCHeroExperience::AddSuccessfulBlockParry" -> CSoundFormat::BytesToSamples / SamplesToBytes;
  0x00C3C5F0 "CItem::SetQuestPersistentFlag" -> mark-count release; 0x00C35C30 "CTCSpecialAbilities::OnKill" -> `__stdcall`
  notifier; 0x00C31990 "CSBAgents" -> message-event constructor {0, thing}; 0x00686D50 "CViewBase::GetDisplayEngine" ->
  global player-manager getter.
