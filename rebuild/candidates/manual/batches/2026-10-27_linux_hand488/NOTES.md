# Hand chunk 488 (CSimVoice .. CWoundMorphs): 38 of 40 byte-exact

18 MATCH + 20 RELOCATION_MATCH, 0 retail_asm; 1 parked (DIFFER), 1 skipped (Catch@ row). Every matched row has a
behaviour test that prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1/Wine (tall.py). Leaf lines were checked
against `objdump -t $S/wine/q/<addr>/<addr>.obj` (exact symbols; the anonymous-namespace row keeps the `?A0x` prefix).

## What matched (38)

**Components (CTCBase family)**
- 006cb6a0 CTCCreatureNavigation::GetNavigatorType() const -- `PNavigator->GetType()`; CClass taken from engine/CClass.h
  through the `#define CClass DonorCClass` rename pattern so a `GetType()` accessor can be added (`movzx` of the word).
- 006cb920 CTCCreatureNavigation::OnSetInLimbo(bool) (slot 16) -- `PNavigator->Interrupt()` (navigator slot 18).
- 0070b490 CTCAnimationComplex::GetAnimIndex() const -- `PAnimation->CurrentAnim.MainAnimSet.GetAnimIndex()`, inline
  `Anims[0]` over a minimal STLport vector<short> (`movsx`).
- 0070be80 CAnimateThing::SetAsFinished() -- the `Finished` bool bitfield of CAnimSet at +0x51 (`or byte [ecx+51h],2`).
- 007122c0 CTCScriptedControl::IsActionListEmpty() const -- STLport list `empty()` on the 4-byte list at +0x14.
- 0073a6e0 CTCCreatureModeManager::GetCurrentMode() const -- `*PModes.front()` (list of CCountedPointer<CModeBase>).
- 00723350 CTCPhysicsLight::FillPhysicsDebugInfoForSelf (slot 89) -- two `++` on the info struct.
- 00723a50 CTCPhysicsStandard::ReturnToDefaultInteractionFlags (slot 44) -- virtual SetInteractionFlags (slot 43) with
  `PhysicsDef->InteractionFlags`.
- 00736ad0 CTCCombat::OnInitialActivate (slot 6) -- `if (ParentThing->GetType() == 2) UpdateUsableMeleeCombatAbilities();`
  (the call is a tail `jmp`).
- 00746b20 CTCBed::FrameUpdate (slot 9) -- `if (PDef->Animating) UpdateAnimation();` (tail `jmp`).
- 00751a80 CTCLook::GetLookAngleRelToBodyXY() const -- `HeadAngleRelToBodyXY + EyeAnglesRelToHeadXY` (`fld [+34h] ; fadd [+2Ch]`).
- 00702cb0 CThingObject::IsCrop() const (slot 49) -- `(GetProperties() >> 1) & 1` through virtual slot 48.

**Game systems**
- 00687d40 CInputProcessGameBase::GetMouseWheelMovement -- engine-header rename pattern; the float result must go
  through a NAMED float local or VC tail-jumps.
- 006924b0 CCreatureActionBase::GetRandomAnimFromName (static __fastcall) -- `*creature.GetRandomLoadedAnimFromName(name, true)`.
- 006a6be0 CPhysicsMeshObject::operator= -- memberwise: inline CIVCountedPointer assignment, CMatrix3x4 struct copy
  (`rep movsd` x12), Scale, OO_Scale. The release has to go through a LOCAL copy of the object pointer so `dec` sets
  the flags (`dec [ecx+4] ; jne`); releasing through the member re-reads it.
- 006b3750 CEnvironment::RestoreDefaultWeightAllChannels(long, float) -- engine/CEnvironment.h rename pattern with an
  accessor that reinterprets the header's `PriorityStackChannels[0xc]` bytes; the AllChannels loop of the landed
  TransitionToDefaultThemeAllChannels.
- 006bb940 CGameTimeManager::ConsoleSetDaySpeed -- `if (args[0].ValFloat > 0.0001f) TimeStep = 1.0f / args[0].ValFloat;
  else TimeStep = 0.0f;` The method is `private:` static (`CI` in the PDB mangling).
- 00736910 (anonymous namespace)::GiveHeroMeleeAbility(long, bool) -- `g_pMainGameComponent->PPlayerManager->
  GetMainPlayer().GetPControlledCreature()`, `!Dead`, then the inlined CThing::GetInterface(&combat, 0x30) with an
  UNINITIALISED component local (the miss path reads the key temporary's slot), Add/RemoveMeleeFlourishAbility. The
  internal-linkage leaf is kept emitted by an `auto_inline(off)` block plus a small external caller.
- 007431c0 NCreatureMode::CModeStrafe::Alloc(CThing&) -- `return new CModeStrafe(thing, false);` (static __fastcall).
- 00757930 CWoundMorphs::GetPWoundWithIndex(long) const -- the mutable dirty re-sort of the landed sibling, then
  `index >= 0 && index < Morphs.size()` (size() unsigned, so `jae`), `&First[index].second`.
- 006fd400 / 007222b0 CCountedPointer<NAnimationEvents::CManager> / <CTCAppearanceMorphBase::CMeshMorphSet>::DeleteData --
  the `static void __fastcall DeleteData(void* data) { delete (T*)data; }` template of landed 0x0075F830, with the
  members' real destruction order (later member first) and out-of-line callees declared (`__destroy_aux`, ~CSymbolMap,
  ~CEngineTextureMorphSet).

**STLport algorithm / container instantiations (/Gr TUs: `__fastcall` on the template definition)**
- 00685200 _List_iterator::operator++ and 00685620 list::begin -- element type unprovable (CUnprovenValue_<addr>).
- 0069c8b0 __copy_ptrs<CQuake*> -- inline __copy loop, `rep movsd` element copy.
- 006ef000 __copy<pair<C3DVector,bool>*> -- 13-byte packed pair (retail C3DVector is 1-aligned).
- 006e6950 find<CIntelligentPointer<CThing>*, CThing*> -- forwards to __find 0x006E66E0 with a tag temporary.
- 006f1250 _Rb_tree<unsigned __int64, ...>::_M_create_node -- `malloc` declared plain `extern "C"` so it calls thunk
  0x00BFEA0E (a dllimport declaration gives `call [imp]`).
- 0071efc0 pair<const CCharString, pair<long, CDateAndTime>>::pair(const T1&, const T2&).
- 006bef30 __pop_heap_aux -- `__pop_heap(first, last - 1, last - 1, T(*(last - 1)), comp, (int*)0)`, the pair copy is
  built in the argument area.
- 006dc2e0 __copy<const long*, back_insert_iterator<vector<long> > > -- inline push_back; `__true_type` needs a USER
  constructor or VC zero-initialises the `_IsPODType()` temporary (`mov byte [esp+..],0`).
- 006f45a0 vector<CCombatRing>::erase(iterator) -- inline __copy_ptrs -> out-of-line __copy 0x006F4470, then
  `--finish; _Destroy(finish)` with the ring's prong vector destroyed inline.
- 006fb8f0 swap<pair<unsigned long, CSmallVector<CEvent,32>>> -- `T tmp = a; a = b; b = tmp;`, CSmallVector with a
  zeroing copy ctor + Copy, `if (&other != this)` operator=, `delete[] Start` dtor.
- 0070fcb0 __unguarded_linear_insert<list<CAnimSet>::const_iterator*> -- val by value (4-byte class, on the stack).
- 0073d5a0 __find_if<_List_iterator<CCountedPointer<CModeBase>>, CModeBase::CIsType>(..., const input_iterator_tag&) --
  hidden iterator result in ecx, tag reference in edx.
- 00743b70 __final_insertion_sort<pair<unsigned long, CRegisteredModeInfo>*> -- threshold 16 folds to
  `((last - first) & ~7) > 0x80`; the inline __unguarded_insertion_sort passes the null value-type pointer.
- 0074f350 for_each<CThingObject* const*, CAddObjectNotSolidTo> -- functor returned through the hidden pointer (it has
  a user ctor).

## Parked for Fable 5.1 (1)
- 00680840 CSimVoice::GetRandomSampleIndexVariation DIFFER(114v114): `lea edx,[esp+18h]` / `mov [esp+1Ch],ebp` vs retail `lea edx,[esp+14h]` / `mov [esp+18h],ebp` -- the third by-value copy of `type` (GetSampleIndexVariation's LowerBound argument) is homed in the dead `seed` slot instead of the `type` slot; every other byte matches | tried: nested call, named count/index/result locals, direct `Voices.LowerBound(type)` (110 bytes), `[index].second`, iterator local, non-__fastcall and non-forceinline GFRandom, seed pointer local, `#pragma optimize("s")` | next: helper parameter declared `const EReactionSpeechType` or a different inline order of GetNoOf/GetSample (e.g. GetSample defined before GetNoOf in the class), else "lane gap: stack-slot packing"

## Skipped
- 006c9c60 `_global::Catch@004ab920` (106 bytes): skipped under the Catch@ rule. NOTE: it is not a catch funclet -- it is
  an ordinary thiscall function (`sub esp,10h ; mov esi,ecx`, two CBaseIntelligentPointer temporaries via 0x00A01B10 /
  0x00A01B50 / 0x00A01B90 / ~0x00A01C10, calls 0x006C9A10, `ret 8`), so the catalog label is wrong; worth a real row later.

## Label corrections
- 0x00680840 sub_00680840 -> CSimVoice::GetRandomSampleIndexVariation(EReactionSpeechType, unsigned long&) const (Ego_r).
- 0x00687D40 `operator_new` -> CInputProcessGameBase::GetMouseWheelMovement(const CInputEvent&) const.
- 0x00445920 (landed as `_Cons_val`) -> CGamePlayerInterface::GetMouseWheelMovement(const CInputEvent&) const.
- 0x006924B0 `CMainGameComponent::BeginInputLoading` -> CCreatureActionBase::GetRandomAnimFromName.
- 0x006CB6A0 `CIsHitEventBlocked::operator()` -> CTCCreatureNavigation::GetNavigatorType.
- 0x0073A6E0 `CPixelShader::CountReferences` -> CTCCreatureModeManager::GetCurrentMode.
- 0x00751A80 `CMap::DrawGetSeaLevel` -> CTCLook::GetLookAngleRelToBodyXY.
- 0x007122C0 `CScriptTask_IsRunning` -> CTCScriptedControl::IsActionListEmpty.
- vfunc_N rows: 0x006CB920 = slot 16 OnSetInLimbo, 0x00702CB0 = slot 49 IsCrop, 0x00723350 = slot 89
  FillPhysicsDebugInfoForSelf, 0x00723A50 = slot 44 ReturnToDefaultInteractionFlags, 0x00736AD0 = slot 6
  OnInitialActivate, 0x00746B20 = slot 9 FrameUpdate.
- 0x006C9C60 `Catch@004ab920` is a real function, not a funclet (see Skipped).
- Landed 0x006804C0 is named for CVectorMap<ETCInterfaceType, CTCBase*> but it is CSimVoice's
  CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<...>>::LowerBound (0x00680500 the UpperBound).
- Landed 0x006F1280 "CThingCompareMap::Insert" is an invented stand-in type.
- 0x00487DC0 / 0x00487DD0 are two byte-identical `add ecx,2Ch ; jmp GetPItem` accessors of CPlayer::ControlledCreature
  (+0x2C). Ego_r places `GetPControlledCreature() const` (PAV, 0x00493650) immediately before
  `PeekPControlledCreature() const` (PBV, 0x00493660), the same order and spacing as retail, so 0x00487DC0 =
  GetPControlledCreature and 0x00487DD0 = PeekPControlledCreature. Landed sources call 0x00487DC0 `GetHeroThing` /
  `GetHeroCreature` (descriptive), and landed 0x00896EE0 binds `?GetPControlledCreature@CPlayer@@...` to 0x00487DD0
  (vland RELOC_CONFLICT with this chunk's 00736910): that landed row should call the Peek name.
- 0x00445920 is labelled `_global::CdeclCall_DrawGetSeaLevel_00445920` (vland RELOC_NAME); it is
  CGamePlayerInterface::GetMouseWheelMovement.
- 0x005BC0D3 `StdList_Dtor_005BC0D3` / `CActiveFile::OnReadFinished` -> ~CEngineTextureMorphSet (probable: the
  CMeshMorphSet member at +0xC that 0x007222B0 destroys; CEngineTextureMorphSet = CArray<CEngineTextureMorph>).
- Landed 0x00743430 (InsertionSort_SElement8), 0x006F4470 (Copy_SVectorRecord1C), 0x006F44E0
  (DestroyAux_SPointerListRecord1C) are descriptive stand-ins: they are std::__insertion_sort<pair<unsigned long,
  CRegisteredModeInfo>*>, std::__copy<CDefendingCombatantInfo::CCombatRing*> and
  std::__destroy_aux<CDefendingCombatantInfo::CCombatRing*> respectively.
- 0x00886430 sub_00886430 -> NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::SetDefaultThemeWeight(float, bool) (probable).
- 0x0073DCE0 (unlabelled) -> std::find_if<list<CCountedPointer<CModeBase>>::iterator, CModeBase::CIsType> (probable).

## Header debt
- engine/CTCBase.h flattens the vptr into `__vftable`, so 006cb6a0, 006cb920, 0070b490, 007122c0, 0073a6e0, 00746b20,
  00751a80 and 00736ad0 use a local polymorphic CTCBase model (filler virtuals for the slots).
- 00736ad0 CTCCombat and 00723a50 CTCPhysicsStandard / 00723350 (CTCPhysicsBase) have engine headers but are modelled
  locally (need virtual slots the generated headers cannot give).
- 006924b0 CCreatureActionBase has engine/CCreatureActionBase.h; the static member is declared on a local class.
- vland SHARED_HEADER_AVAILABLE warnings (callee/argument classes modelled locally while a header exists):
  CGamePlayerInterface (00687d40), CThingCreatureBase + CCreatureActionBase (006924b0), CGameCameraManager (0069c8b0),
  CNavigatorBase (006cb920), CManager (006fd400; the header is a different CManager -- NAnimationEvents::CManager has
  none), CTCPhysicsBase (00723350, 00723a50), CTCCombat + CPlayer + CPlayerManager (00736910).
- Local models with no shared header: CSimVoice, CWoundMorphs, CPhysicsMeshObject, CGameTimeManager,
  NCreatureMode::CModeStrafe, NAnimationEvents::CManager, CTCAppearanceMorphBase::CMeshMorphSet,
  CDefendingCombatantInfo::CCombatRing/CProng, CThing interface map (GetInterface model of the landed rows).

## Callee names
Proven (PDB name + address evidence): CVectorMap<EReactionSpeechType,long>::LowerBound 0x006804C0 / UpperBound
0x00680500, GFROR13 0x00497890, CGamePlayerInterface::GetMouseWheelMovement 0x00445920,
CThingCreatureBase::GetRandomLoadedAnimFromName 0x00662FA0, CPlayerManager::GetMainPlayer 0x00449970,
CPlayer::GetPControlledCreature 0x00487DC0, CTCCombat::AddMeleeFlourishAbility 0x00736640 /
RemoveMeleeFlourishAbility 0x007366C0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020,
CModeStrafe::CModeStrafe 0x00741FE0, CTCSolidToThing::AddThingNotSolidTo 0x00770FA0, CSmallVector<CEvent,32>::Copy
0x006FB5C0, CWideString copy ctor 0x0099B720, ~CBaseIntelligentPointer 0x00A01C10, ~CSymbolMap 0x006FD040.
Probable / descriptive: SetDefaultThemeWeight 0x00886430, __pop_heap 0x006BEDC0, __copy 0x006F4470,
__destroy_aux 0x006FB8A0 / 0x00722130, ~CEngineTextureMorphSet 0x005BC0D3, __insertion_sort 0x00743430,
__unguarded_insertion_sort_aux 0x0073D720, vector<long>::_M_insert_overflow 0x00440990, __find 0x006E66E0,
UpdateUsableMeleeCombatAbilities 0x00736590, CTCBed::UpdateAnimation 0x007468C0, sort 0x007597C0.

## Names: proven vs descriptive
- Proven (PDB/RTTI/vtable/caller evidence): 00680840, 00687d40, 006924b0, 006a6be0, 006bb940, 006cb6a0, 006cb920,
  0070b490, 0070be80, 007122c0, 00723350, 00723a50, 00736910, 0073a6e0, 007431c0, 00746b20, 0074f350, 00751a80,
  00757930 (19).
- Probable (STLport shape + element/callee evidence, or PDB order): 0069c8b0, 006b3750, 006bef30, 006dc2e0, 006e6950,
  006ef000, 006f1250, 006f45a0, 006fb8f0, 006fd400, 00702cb0, 0070fcb0, 0071efc0, 007222b0, 00736ad0, 0073d5a0,
  00743b70 (17).
- Descriptive: 00685200, 00685620 (CUnprovenValue_<addr> element), 006a5220 (CUnproven_006a5220::IsNotPositive_006a5220).
