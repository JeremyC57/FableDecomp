# Hand chunk 94 (CTCInventoryExperience .. CTCInventoryMap): 39 of 40 byte-exact

## What matched (39)
- **CTCInventoryExperience (16 of 17)** -- the module is size-optimised: every source carries `#pragma optimize("s", on)`
  (frame-pointer bodies add `optimize("y", off)` first). The PDB header is 0x44..0x60 bytes wider than retail ahead of
  these members (16-byte editor containers vs 12-byte retail ones), so all sources use local retail-offset models
  (OrigGeneral..OrigWill +0x1AC, statIndicatorPos/Max +0x1BC/+0x1C0, ExperienceSpendAdd/Remove +0x1C8/+0x1D0,
  ExperienceSpendValues* +0x1E0/+0x1EC/+0x1F8, ExperienceSpendMagnitude +0x204, MenuDisplayListExperienceItem +0x208,
  NumItems +0x218, TutorialsPlayed +0x248; base InventorySelectionState +0x1C, MenuArrowHighlights* +0x38.., PDef +0x68).
  - 0056e5ec / 0056e60f ProcessButtonAReleased / ProcessButtonXReleased: qualified base call, then two `bool` clears --
    the immediate byte stores (no shared `xor al,al`) are the /Os form.
  - 0056e632 / 0056e674 GetHeroTrainableStatType / GetHeroTrainableAbilityType: `for (i..) if (t[i].Category == c &&
    t[i].Button == b) break; return t[i].Result;` -- VC strength-reduces to a pointer at the third word with `jl`.
  - 0056ea4e GetXString(count): `return ::GetXString() + IntToWideString(count);` -- right operand first, the left
    temporary homed in the dead argument slot; `y` off + `s`.
  - 0056ea9a GetOriginalExperience: plain if/else chain (MATCH).
  - 0056ec79 vfunc_37: two `and [..],0` zero stores + tail call.
  - 0056ec8c / 0056eccb ProcessInventoryInputDown / Up: `switch (InventorySelectionState) { case 0: base; case 1: ... }`
    (`sub eax,0 ; je ; dec eax ; jne`); the Up twin's slider callee is a static `__stdcall` (no `mov ecx,esi`) whose
    empty `ret 0Ch` body was ICF-folded with the thiscall one.
  - 0056f03d ResetExperienceSpendValues: three `v._M_start[i].currentSpend = v._M_start[i].initialSpend` loops, the
    third starting at 1 (`push 0Ch ; pop edx`) (MATCH).
  - 0056f26f / 0056f2bd GetExperienceStatName / GetExperienceAbilityName: CDefPointer copy in an inner scope (add-ref
    under a null test, release on the pointee with NO null test), `if (id) return GetInventoryText(id); return g_Empty;`.
  - 00572636 ProcessExperienceAction: the `jle / cmp 2 ; jle / cmp 5 ; jg` ladder is a `switch` with case groups
    {1,2} and {3,4,5} (the if/else form lays the {1,2} arm out first).
  - 00574647 / 0057469f ProcessButtonAHeld / XHeld: the global scale is read into a local BEFORE the def copy (retail
    loads it ahead of the add-ref).
  - 0057598a IsTutorialPlayed: end pointer cached, SGI `find` front end `__fastcall`, `push_back` on a miss,
    `unsigned char` result.
- **CTCInventoryItem label rows (18)** -- only 004daea9, 005d8e70 and 005d92f0 are CTCInventoryItem; the rest carry
  propagated labels and were named from their callers / vtables:
  - 004daea9 CTCInventoryItem::CTCInventoryItem: CTCBase(owner) then init-list zeroes (PDef as a member object with
    an inline constructor); pass-through push = /Os.
  - 005d8e70 GetBoughtPrice: `if (PInventory) return PInventory->GetBoughtPrice(ParentThing->DefIndex); return 0;` /Os
    (the speed build zero-extends into edx).
  - 005d92f0 GetStartingRightHandedSet: def copy, `return GetRHSetFromOrientation(def->Orientation);` (`__fastcall`,
    hidden result forwarded).
  - 0061a260 CTCInventoryQuests::IsQuestCardRegionFree (static `__stdcall`): inner scope for the def copy and a NAMED
    CCharString local, `return !started;` after the scope, `unsigned char` result.
  - 007f59c0 CTCTargetingAI::OnInitialActivate: def copy whose destructor DOES null-test (unlike the inventory module).
  - 009d8960 CSharedObjectHandle_009d8960::operator=: the release must be an inline method on the pointee (the member
    form re-reads the pointer after the decrement) (MATCH).
  - Destructors, all `__declspec(novtable)` derived classes with empty bodies over a base whose destructor is inline
    (stores the base vtable) or out of line (tail-jumped): 0045641a ~CAppearanceModifierDef, 004e3cb3
    ~CContainerRewardHeroDef (over CDefPointeeBase), 004e2bc7 ~CTCPreCalculatedNavigationRoute, 007de440
    ~CTCCreatureFlyInCircles (over CTCBase; `#pragma pack(1)` for the unaligned +0x3B intelligent pointer), 00a55420
    ~C3DAnimFileSequenceChunk, 00a9d3f0 ~C3DMeshFileConvexVolumeChunk (over CChunkedFileSuperChunk), 00bcde40
    ~CEngineInternalPrimitiveWeaponTrail (root CIVCountedPointeeBase store, no call), 00b81e60 / 00b84030
    ~CEngineClothSimulationStaticMesh / ~CEngineClothSimulation2DAnimatedMesh (the inline base destructor calls
    ClearParticleSims). 00a37360 ~CClassFactory and 00af5900 ~NParticleEngine::CParticleSystem store their own vtable
    and destroy a vector inline: `_Destroy_range(first, last, SScalarTag())` + `if (first) free(first)`, the tag in the
    `push ecx` frame slot (`lea eax,[esp+7]`).
  - 00c6a3d0 CTCKraken_00c6a780::Shutdown: the cleanup sequence of the landed Kraken destructor, as its own member.
- **CTCInventoryMagic (2)** -- 0072db90 constructor: two bases (CTCInventoryBase, NHeroInformationScreens::CBase at
  +0x130, as the landed CTCInventoryStats ctor) + a vector member with an inline zeroing constructor; 0072dac0
  DrawPreparePrimitivesForRendering: `if (InventoryOpen) Helper(viewport, GetValue());` (secondary-base virtual).
  Speed-optimised module (no pragma).
- **CTCInventoryMap (3)** -- 005fa910 / 005fa950 ProcessInventoryInputDown / Up: wrap-around `--i < 1` / `++i > n`
  under `InventorySelectionState == 1 && WorldMapActive`, else the base tail call; 005faa30 ResetWorldMapDisplay:
  `ActiveRegionIndex = GetWorld()->GetRegionMap()->GetActiveRegionIndex();`.

## Parked for Fable 5.1 (1)
- 0056ef2d CTCInventoryExperience::vfunc_29 DIFFER(66v66): retail `xor edx,edx ; cmp [esi],edx ; setg dl` (zero register reused as the setcc target) with first/limit in esi/edi and the second result in eax; the lane pushes ebx and uses it for both setcc results | tried: `cond ? 2 : 1`, named `first` local (fixed the start-pointer re-read), named `lastAllowed` local, `(cond) + 1` | next: lane gap: register allocation (setcc into the compare-zero register)

## Skipped
- none (no row spans two functions; trailing int3 padding only).

## Label corrections
- 0056e60f "ProcessButtonAReleased" -> slot 60 twin (Remove event, base 0x005BC68F): named ProcessButtonXReleased (button letter inferred from slot order 54/56/58/60).
- 0057469f "ProcessButtonAHeld" -> slot 56 twin (Remove event, base 0x005BC68E): named ProcessButtonXHeld.
- 0056e632 "GetHeroTrainableAbilityType" -> GetHeroTrainableStatType (10-entry table, only caller UpdateExperienceSpendStat).
- 0056eccb "ProcessInventoryInputDown" -> slot 39 = ProcessInventoryInputUp (increments).
- 0056f26f "GetExperienceAbilityName" -> GetExperienceStatName (reads ExperienceItemLabelStat; the twin reads ...Ability).
- 0057462c / 0057463b "~vector<bool>" -> CTCInventoryExperience Add/RemoveExperienceSpend (`ExperienceSpendMagnitude = +1/-1; ProcessExperienceAction();`).
- 0045641a "CTCInventoryItem::OnDie" -> ~CAppearanceModifierDef; 004e2bc7 -> ~CTCPreCalculatedNavigationRoute; 004e3cb3 -> ~CContainerRewardHeroDef;
  007de440 -> ~CTCCreatureFlyInCircles; 00a37360 -> ~CClassFactory; 00a55420 -> ~C3DAnimFileSequenceChunk; 00a9d3f0 -> ~C3DMeshFileConvexVolumeChunk;
  00af5900 -> ~NParticleEngine::CParticleSystem; 00b81e60 -> ~CEngineClothSimulationStaticMesh; 00b84030 -> ~CEngineClothSimulation2DAnimatedMesh;
  00bcde40 -> ~CEngineInternalPrimitiveWeaponTrail; 00c6a3d0 -> CTCKraken_00c6a780 cleanup (named Shutdown).
- 0061a260 "CTCInventoryItem::GetStartingRightHandedSet" -> a static `__stdcall` CTCInventoryQuests helper (quest card region check, named IsQuestCardRegionFree).
- 007f59c0 "CTCInventoryItem::OnCreate" -> CTCTargetingAI vtable slot 6 (OnInitialActivate by slot majority).
- 009d8960 "CTCInventoryItem::GetEngineGraphic" -> a counted-handle `operator=` (class unknown, named by address).
- 005fa910 / 005fa950 "CTCInventoryMap::vfunc_38/39" -> ProcessInventoryInputDown / Up.
- Callees: 0x0099A300 "CBase" = CBaseClassNonCopyable::~CBaseClassNonCopyable; 0x00686A80 "CViewBase::GetDisplayEngine" = CTCBase::GetWorld() const;
  0x004FB150 "CWorld::DrawGetEnvironment" = a `[this+0x9C]` getter (region index); 0x005BC58C "$E2" = an empty `ret 0Ch` slider refresh (folded thiscall + stdcall).
