# Hand chunk 303 (CTCInventoryMap .. CTCKraken): 23 of 40 byte-exact

All 23 rows grade RELOCATION_MATCH on RTM cl 13.10.3077 (gall.sh). Every row has a behaviour test that passes under
g++ (tall.sh) and VC7.1 (tall.py).

## What matched (23)
- **CTCInventoryMap** (6)
  - 005fc0c0 UpdateRegionsCorrespondances: pairs up the region-left and region-entered messages.
    - Lever: an inline `CWorld::FindAllMessages(filter, list&)` wrapper. The filter temporaries are stored before GetWorld and the pushes come after it.
  - 005fc620 FindTeleports: builds the teleport-region list.
    - Levers: the same wrapper idea through `GetAllThings`; a bool-absent/cached-end idiom for the `find` + insert.
  - 005fce60 vfunc_31 (closes the PC map screen).
  - 005fdad0 vfunc_12, which is really CTCInventoryMap::Copy.
    - Levers: `__forceinline` on push_back and `_M_insert_overflow`; a separate `uninitialized_copy` helper; a POD const_iterator for the range insert.
    - A same-TU noinline 0x412330 stand-in that only reads x lets the `__false_type` tag home on a live argument slot.
  - 006116e0 OpenInventory.
- **CTCInventoryQuests** (2)
  - 00621720 CloseInventoryPC and 00621c80 CloseInventory use the landed Trade CloseInventoryPC sound/front-end form.
- **CTCInventoryStats** (3)
  - 005cc9ad GetCreatureGroupAsString: one `return` per bit test.
  - 005cd074 ConstructScreen: CActionParam subobject adjust.
  - 005cd767 InitialiseMannequin: def-pointer temporaries.
- **CTCInventoryTrade** (9)
  - The GetMenuBranchFromDefIndex pair, 0059b79a and 0059c0c6, plus GetTypeFromDefIndexPC 0059c447: twelve named lookups and a flat else-if chain.
  - 0059cb90 AddWantedItemToList.
  - The RecurseRefreshEntries… pair, 0059db82 and 0059e6bd, use the landed PC model.
  - 0059e2c7 CloseInventoryPC and 0059f304 CloseInventory.
  - 005a30a1 InitialiseBuyableItems: a second named key copy.
  - 005a8a5d ConstructSpecialShopListPC: a named front-end manager local.
- **CTCInventoryWeapons** (2)
  - 005c3a95 OnSerialise: dead-argument-slot homing.
  - 005c770c ConstructWeaponDescriptionWeight: GetDef/GetSubDef stand-ins.
- **CTCKraken** (1)
  - 007fbe50 UpdateBeamExplosion (955 bytes) matched on the 13th grade.
  - Levers:
    - inline `GetPhysics()` and `GetGraphicAppearance()` CThing accessors, which load `this` into ecx before the hidden-return/argument pushes;
    - `dir * length` written twice, once for the Create end and once for the line-of-sight end;
    - a C3DVector with a user copy constructor, which fixed the x87 operand order of the beam-end sum.

## Parked for Fable 5.1 (16)
- 0059d270 CTCInventoryTrade::AddWantedItemToListPC DIFFER(996v1002): retail `xor ebx,ebx` + `mov [x],ebx` / `cmp eax,ebx`; ours `and [x],0` / `test eax,eax` with a late `mov ebx,0` | tried: 5 forms (named zero locals, scope blocks, temp order) | next: lane gap: zero-register allocation.
- 005c71aa CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight DIFFER(681v680): retail `cmp [eax+esi*4],ebx ; mov edx,[eax+esi*4]`; ours `lea eax,[eax+esi*4]` first; two push/lea swaps | tried: 11 probes (index loop forms, `*(begin()+i)`, named refs) | next: a const-ref element local inside the loop.
- 005cdd86 CTCInventoryStats::CreateExperienceSliders DIFFER(860v864): register allocation and scheduling across the six slider blocks | tried: 8 probes (per-slider helper, named temporaries) | next: an inline per-slider helper taking the CCharString by const ref.
- 005cb974 CTCInventoryWeapons::OnInitialActivate DIFFER(693v706): retail keeps the viewport/def pointers in callee-saved registers across the calls; ours reloads them | tried: 5 probes | next: lane gap: cross-call register caching.
- 005c66b2 CTCInventoryWeapons::ConstructWeaponItemEntries DIFFER(695v760): this/component pointers are cached across calls | tried: 1 structural probe | next: lane gap: whole-function register caching.
- 005c8176 CTCInventoryWeapons::ConstructWeaponItemEntriesPC DIFFER(617v739): same as 005c66b2 (its PC twin) | tried: 1 structural probe | next: lane gap: whole-function register caching.
- 0059fc17 CTCInventoryTrade::AlreadyHaveSpecial DIFFER(715v716): retail holds the entry in edi with `sar [ebp-0Ch],2`; ours uses ecx with a spill to [ebp-10h] | tried: 10 probes (named pointer locals, size locals, loop forms) | next: try the STLport `_M_start` loop with size from `_M_finish - _M_start` read every pass.
- 005ad305 CTCInventoryTrade::ApplyTattooCard DIFFER(620v620): frame 0x40 vs 0x3c; the card vector sits at [ebp-28h], not [ebp-24h] | tried: 13 probes (scope blocks, temp placement) | next: lane gap: frame-slot sharing.
- 00622a00 CTCInventoryQuests::OpenInventory DIFFER(877v877): ours caches 0 in ebx for the whole function; retail uses immediates and zeroes edi/ebp locally | tried: 2 probes | next: split the list-building into an inline helper so the zero is not function-wide.
- 00621a20 CTCInventoryQuests::InitialiseQuestCardList DIFFER(620v600): an extra eax copy in the inlined lower_bound loop | tried: 1 probe | next: lane gap: inlined lower_bound eax copy.
- 007fb270 CTCKraken::GetClosestTentacleToTarget DIFFER(556v556): frame 0x2c vs 0x28; the fild size temporary and the 0xE3 key do not share a word (variant h has the right frame, but one `mov [esp+18h],0E3h` is hoisted above the `je`) | tried: 15 probes (key scope, inner blocks, `&kind`, FindTC split, early-return GetInterface, loop-scope blocks) | next: lane gap: frame-slot sharing.
- 005fd1d0 CTCInventoryMap::FindTeleportsForActiveRegion DIFFER(562v564): frame 0x48 vs 0x40 because the LowerBound key and out words do not share; retail runs `__ftol2` on Y before X | tried: 13 probes | next: lane gap: frame-slot sharing plus __ftol2 order.
- 005fd5e0 CTCInventoryMap::ConstructRegionDescriptionQuests DIFFER(986v982): retail places the non-core branch after `ret` and keeps the merged tail in the core block | tried: 4 probes (`!IsCore` inversion, goto, per-arm full exits) | next: lane gap: cross-jump block placement.
- 005c7452 CTCInventoryWeapons::ConstructWeaponDescriptionDamage DIFFER(-v698): two `fstp [x] ; fld [x] ; fistp [y]` roundings with no __ftol2 | tried: decode only (no source graded) | next: lane gap: inline-asm fistp rounding helper.
- 005c323a CTCInventoryWeapons::ConstructWieldEntry DIFFER(522v522): retail loads the entry component into esi after the viewport call; ours loads it before | tried: named locals, helpers, implicit conversion, cached pointer (10 probes) | next: lane gap: scheduling.
- 005c3444 CTCInventoryWeapons::ConstructWieldEntryPC DIFFER(638v638): the same esi-load residue as 005c323a | tried: 2 probes | next: lane gap: scheduling.

## Skipped
- 005cacb9 CTCInventoryWeapons::RefreshWeaponDescriptions: the catalog row is truncated.
  - The 653 bytes end on `jmp 0x5CB101`, which jumps past the row, with no ret or int3.
  - The function continues beyond the row length.

## Label corrections
- 0x005FDAD0 "CTCInventoryMap::vfunc_12" is CTCInventoryMap::Copy(const CTCBase&).
- 0x00618100 is `list<CIntelligentPointer<CThing>>::_M_insert_dispatch(pos, first, last, __false_type)`, `ret 10h`, `this` unused.
- 0x0080CBF0 is labelled CCreatureActionBase::AddDependentThing. It is the explosion driver's AddExcludedThing / SetOwner, called on the interface-0x41 driver.
- 0x00669D30 is labelled CCRC::CalcAndLookup. It is CSpecialEffectsDef::GetEffectFromString.
- 0x00686A70 is labelled CViewBase::GetDisplayEngine. It is called with a CTCKraken `this`, so it is a byte-identical CTCBase copy (check with label_trust).
- 0x005BE8D3 is ICF-shared and used as a slot-values getter.
- 0x0053FDE0 is AddParam.
- 0x005D99D0 is an interface-type lookup.
- 0x00487DD0 is GetControlledCreature.
- 0x004AEF50 acts as IsSaving.
- 0x005BC4C4 returns the associated CThing*.
- 0x005D9A10 is GetHairCardDef and 0x005D9AA0 is GetTattooCardDef.
- 0x0071DE20 is GetTattooThings.
- 0x0066E7BD is GetCurrentScreen.
