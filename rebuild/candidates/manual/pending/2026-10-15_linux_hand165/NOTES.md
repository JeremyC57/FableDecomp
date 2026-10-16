# Hand chunk 165 (CTCHeroStats .. CTCInventoryBase): 30 of 40 byte-exact

All 40 rows were authored (a container restart split the session; the resumed session wrote the 30 behaviour tests,
re-graded every src/ file (30/30 MATCH or RELOCATION_MATCH), ran tall.sh / tall.py (30/30 PASS under g++ and VC7.1)
and wrote STATUS/NOTES/LESSONS). Most rows are size TUs: `#pragma optimize("y", off)` + `optimize("s", on)`.

## What matched (30)
- CTCHeroStats
  - 0x0057A989 GetFavouriteRegion: arg-max over RegionTime; `for (unsigned i = 0; i != size(); ++i)` gives `test eax,0FFFFFFFCh ; je`; text bank a named local read before the `best != -1` test.
  - 0x0057AE53 GetWivesInMarriageState: list walk from the +0x1D8 head; `push_back(static_cast<CThingCreatureBase*>(wife))` puts the temporary in [ebp-4] after the tests.
  - 0x0057B05F SetAge: min/max are named locals (reused in the normalisation); def read through an inline `GetHeroStatsDef()` accessor (body read swaps esi/edi).
  - 0x0057B0F7 SetSunTan, 0x0057B169 SetFatness: `GFLimitReturn(v, 0.0f, 1.0f)` + inline out-pointer GetInterface(&morph, 3), no null test.
  - 0x0057B9D7 GetRandomBeardName, 0x0057BA5C GetRandomHairName: `CCharString* names = nameList.begin()` local (indexing the vector re-reads the frame).
  - 0x0057D1A4 OnInitialActivate: float setters take members by value; bare GetDefinitions call is an unused inline def read; def name a named CCharString in its own `{}` block.
  - 0x0057D990 SetHeroKickPositionForCurrentRegion: `optimize("s", on)` only (no "y" off); world a named local taken first.
  - 0x00C44360 / 0x00C6AF50: not CTCHeroStats (see Label corrections); default /O2 bodies.
- CTCIdleScheduler 0x00801BE0 IsPreferredIdle: the tail is an inline `IsPreferred(idle)` member with `&&` (widened 0/1 exits); map key passed by member address.
- CTCInGameMenu 0x0047D1AC DrawPreparePrimitivesForRendering (one zero register for the flag test and four zero arguments), 0x0047D537 IsQuitMenuReceivingInput (unnamed by-value CCharString temporary, state getter called twice).
- CTCInventory 0x005836D2 IsQuitMenuReceivingInput (list local per pass, index in the frame), 0x005849EA IsQuickAccessItemUsable (`__forceinline` GetTC returning a zeroed out pointer shares the dead arg slot), 0x00584A73 AddCloseInventoryEvent (`T* c = 0; if (HasTC(k)) c = GetTCUnchecked(k);` preloads `xor ebx,ebx`), 0x0058502D AddContextSensitivePreferredItem (record a named local), 0x00587B93 OpenPCScoreboard (screen member as a reference local -> `lea esi`).
- CTCInventoryAbilities 0x005EE183 GetNumAssignedAbilityRows, 0x005EE288 GetMenuEntryFromCategory (`switch` with if/return per case; VC hoists the shared lead and merges the six tails), 0x005EEEE6 UpgradeAbility (real GetAbilityMaximumLevel callee body defined above under auto_inline(off); `++Abilities[ability].Level` re-index gives `inc [edx]`), 0x005F3DC6 Copy (vtable slot 12), 0x005F4519 AssignAbilitySwappingPC, 0x005F6B47 OnInitialActivate (inline reference `Max`).
- CTCInventoryBase 0x005BC5C6 GetRightStickPosition (void out-ref; zero fallback as two direct member stores), 0x005BD630 GetCategoryWithIdentifier (signed size local, inline by-value GetDef()), 0x005BDB94 SelectNextCategory / 0x005BDC08 SelectPreviousCategory (unsigned STLport size; `--idx < 0` gives `dec ; jns`), 0x005BEAF5 GetAllSelectedItems (`_M_start[i]` element form keeps the byte offset in the dead arg slot).

## Parked for Fable 5.1 (10)
- 0057b1db CTCHeroStats::ModifyStaminaBy DIFFER(148v148): eax/ecx swapped in the stamina clamp (retail stamina in eax, MaxStamina in ecx) | tried: plain if/else, `MaxStamina < Stamina`, `if ((Stamina += delta) < 0)`, inline GFLimit template | next: lane gap: register rotation (scheduling-only)
- 0057b26f CTCHeroStats::ForceStaminaTo DIFFER(152v156): retail `fstp [ebp+8] ; fld [ebp+8] ; fistp [ebp-8]` float-to-long, ours calls __ftol2 and the GetInterface key moves | tried: cast forms | next: lane gap: bare dword fistp after a float store/reload
- 0077cb60 CTCHighlightItem::OnInitialActivate DIFFER(116v116): zeroed CDefPointer store lands before the GetSubDef pushes (retail after `push eax`), def/thing loads rotate eax/ecx/edx | tried: direct ParentThing->PDef->GetSubDef, thingDef local via operator->, `__forceinline` GetHighlightDef() | next: lane gap: temporary CDefPointer zero store scheduled after the pushes
- 0077cca0 CTCHighlightItem::DoStopRunning DIFFER(122v117): same CDefPointer zero-store gap, plus the def kept in ebp (extra push ebp) and a CSE'd `lea edi,[def+2Ch]` | tried: the three forms of 0x0077CB60 | next: lane gap: temporary CDefPointer zero store scheduled after the pushes
- 0077cd50 CTCHighlightItem::FrameUpdate DIFFER(117v117): CDefPointer zero stored before `mov ecx,[eax+70h]`, plus an eax/edx swap in the release's vtable load | tried: the three forms of 0x0077CB60 | next: lane gap: temporary CDefPointer zero store scheduled after the pushes
- 0047d21c CTCInGameMenu::CloseInGameMenuViaGameEvent DIFFER(105v109): our compiler drops the event ctor's dead `Player = -1` store (`mov byte ptr [ebp-28h],0FFh`) | tried: init-list ctor, body-assignment ctor, declared CMemoryBuffer::Copy, constructing before the alive test (109v109 but stores precede the test) | next: an out-of-line-looking ctor with the store through a volatile-free alias, else lane gap: dead-store elimination of a ctor field
- 00582944 CTCInventory::OnCreate DIFFER(137v137): the zeroed sub-def CDefPointer's `and [ebp-4],0` lands before the def-chain load (retail after `mov edi,[eax+70h]`) | tried: named thingDef copy in a block, by-value GetDef() temporary, `__forceinline` GetInventorySubDef() | next: lane gap: CDefPointer zero-store scheduling
- 0058400a CTCInventory::OnItemSelectedInHand DIFFER(156v156): retail reuses ebx (dead this) for the owner, ours takes esi (dead item) | tried: named owner local, static `__forceinline` SheatheIntoInventory(owner), inverted if/else | next: lane gap: register choice (scheduling-only)
- 005ee418 CTCInventoryAbilities::FrameUpdate DIFFER(132v130): hero component loaded into eax, null-tested, then moved to ecx (retail `mov ecx,[eax+4] ; test ecx,ecx`) | tried: out-pointer GetInterface + `&& hero`, HasTC + GetTCUnchecked, `if ((hero = ...) != 0)`, out-pointer helper via named component | next: lane gap: looked-up component loaded straight into ecx
- 005bdfb1 CTCInventoryBase::SelectItemWithDefIndex DIFFER(127v119): predicate word pushed from memory (`push [eax+3Ch]`) where retail loads ebx=item, eax=[ebx+3Ch], push eax; the thing-def CDefPointer dtor is `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0` vs retail `mov ecx,[ebp-4] ; test ecx,ecx ; je ; dec [ecx+4] ; jne` | tried: end re-read vs `last` local, pointer-local dtor, inline Release(), named identifier local | next: a CDefPointer whose dtor null-tests and reads Object back from the frame (`if (Object && --Object->RefCount == 0)`) declared as a named local before GetDef

## Skipped
None.

## Label corrections
- 0x0057B0F7 catalog "CTCHeroStats::SetFatness" is SetSunTan (+0x34; OnInitialActivate passes SunTan to it); 0x0057B169 is the real SetFatness (+0x38).
- 0x00C44360 catalog "CTCHeroStats::AllocateEventData" is a propagated label: a library object of the 0xC3xxxx-0xC6xxxx block (only caller 0x00C43F1B); authored as descriptive `CEventDataBuffer::Allocate(long)`.
- 0x00C6AF50 catalog "CTCHeroStats::UpdateEventProgress" is propagated: library object (only caller 0x00C54759); authored as `CStreamProgressMonitor::UpdateProgress`.
- 0x005F3DC6 "CTCInventoryAbilities::vfunc_12" = vtable slot 12, `Copy(const CTCInventoryBase*)`.
- 0x0077CB60 "CTCHighlightItem::vfunc_6" = OnInitialActivate; 0x0077CD50 "vfunc_9" = FrameUpdate.
- Callee labels: 0x0057F5E1 ("CGuiControlTreePane::SortTreeRecursively") is std::list<CThingCreatureBase*>::clear; 0x00686AF0 ("GFGetPreMainMemoryUsed") is CTCBase::GetThingManager; 0x0051ED60 ("CStopWatch::GetTicks") is CWorld::GetThingFromUID; 0x0071C0C0 ("SetAge") is CTCHeroMorph::SetSunTan; 0x005BD33C ("PlaySoundForMeshAnimation") is the inventory category-change sound helper.
