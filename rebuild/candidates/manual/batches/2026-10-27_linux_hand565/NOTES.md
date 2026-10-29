# Hand chunk 565 (CTCBattleCharge .. CThingAICreature): 10 of 12 byte-exact

All 10 matched rows grade RELOCATION_MATCH with q.py (gall.sh), pass their tests under g++ (tall.sh) and VC7.1 (tall.py),
and pass the vland dry run (10 WINS, no RELOC_IDENTITY / RELOC_CONFLICT / RELOC_DUPLICATE). There are no retail_asm rows.
Every jump table was checked entry by entry against the retail table.

## What matched (10)
- **CTCAssassinRush** 0x007A9D60 `~CTCAssassinRush`: the user body fades out a running radial blur and clears the key.
  The call chain is `GetWorld().GetEnvironment().GetSpellEffectInterpolator().RadialBlurFadeOut(PDef->AssassinEndSeconds, key)`.
  It goes through CTCBase's protected out-of-line `GetWorld()` (0x00686A70) and two inline scoped_ptr accessors.
  CCountedPointer's member dtor is `ReleaseData(); Data = 0; Info = 0;`.
- **NThingShot::CTCShotDefinitionBase** 0x007BF2B0 `~CTCShotDefinitionBase`: a user-declared virtual dtor with an empty body.
  The members are released in reverse order with the PDB-less-4 layout after the hit-list vector. The vptr store marks
  the dtor as user-declared.
- **CTCHealLife** 0x007C3F10 `~CTCHealLife` (catalog label: CTCActionUseBuyHouse): an empty body.
  STLport `~_List_base` is `clear()` (out of line, 0x007C4E80) followed by the node free.
- **CTCGhostSword** 0x007C5DB0 `MakeSwordsAppear`:
  - The sums are written `facing + pos` / `pos - side` to get retail's x87 operand order.
  - `side *= dist` is an inline in-place scale.
  - The CTCPhysicsBase vtable is modelled in full: 28 CTCBase slots plus slots 28-72, with the overloaded
    GetFacingVector pair in reverse declaration order.
- **CTCTurncoat** 0x007C7960 `~CTCTurncoat`: `if (IS_THING_ALIVE(TurncoatedEffect)) TurncoatedEffect->Kill(true);`.
- **CTCFireballSpell** 0x007C89F0 `CastFireball(CThing*)`: 1000 bytes, matched on the first grade.
  - Retail CTCFireballSpell has CWillButtonHelperTimer at +0x0C (0x34, with the 12-byte vector), so every later
    member is the PDB offset less 4.
  - The lookups use the out-parameter `CThing::GetTC<T>(T**)` with `T::INTERFACE_TYPE`. The PDB has
    `??$GetTC@VCTCFireballSpell@@@CThing@@QAE_NPAPAVCTCFireballSpell@@@Z`.
  - The clamp is `if (<0) =0; else if (>max) =max`.
  - ParentThing is re-read for each lookup and named once before the dummy read.
  - One CRightHandedSet serves both the dummy frame and the GetLastRHSet copy.
  - The side axis is passed as an explicit `C3DVector(side)` copy.
  - The default `CDefPointer<const CMeleeCombatAbilityDef>()` temporary releases through
    `CIVCountedPointeeBase::Release()` (it returns the count), then nulls the pointer.
  - The shot gets SetAsDrawable(false) (CThing vtable slot 28), and the IsAffectedByBulletTimeOverride bitfield
    (+0x93 bit 5) is cleared.
  - Then `GetTC<NThingShot::CTCShotDefinitionBase>` (interface 0x77) calls `SetCanBeDeflected(false)`.
  - VC homes the 0x77 lookup key in the dead `target` argument slot on its own, as retail does.
- **CTCHeroReceiveItems** 0x007CA8E0 `~CTCHeroReceiveItems`: the user body is `RemoveFromGlobalTCs();`. The two
  CEnginePrimitiveHandle members use the landed inline virtual dtor model; the list clear is at 0x007CBB80.
- **CTCBurntEffect** 0x007CDC20 `~CTCBurntEffect`: an unsigned index loop that re-reads size() on every pass, calling
  the static `__fastcall CTCSpecialAbilities::FadeOrKillEffect` on each live effect.
- **CPersistContext** 0x007E39D0 `Transfer<CDefString>` (catalog label: Transfer<signed_char>): chunk 561's Transfer
  template plus an explicit `CPersistTraits<CDefString>` specialization. Binary load is
  `value.TransferBinaryIn(stream)` (0x0044FC00); binary save is the inline `WriteSLONG(TablePos)`. T is proved by the
  RTTI of the text-arm wrapper vtable 0x012704F8.
- **CThingAICreature** 0x008320C0 `~CThingAICreature` (catalog label: CTCTavernGame): the body is
  `PBrain.Reset(0); if (!PCreatureInteraction.IsNull()) RemoveFromCreatureInteraction();`.
  - `Reset(0)` stores Info before Data; the member dtor stores Data before Info.
  - The set's `_M_erase` is 0x00834FD0 and the list clear is 0x00834F90.

## Parked for Fable 5.1 (2)
- 007a8be0 CTCBattleCharge::FrameUpdate DIFFER(576v617): over-spanned row. The function is 558 code bytes plus
  `mov edi,edi` plus a 16-byte jump table (576 bytes), byte-exact over them (pfx2: 0 mismatches over the 560 bytes
  before the table; table offsets 0x6B/0x94/0xBD/0xE6 match). The last 41 bytes of the row are three other functions at
  0x007A8E20 / 0x007A8E30 / 0x007A8E40. | tried: 1 grade | next: re-split at 0x007A8E20 (row length 576), then land
  as is.
- 007cbf60 CTCMultiArrow::~CTCMultiArrow DIFFER(241v241): third element access in the ammo loop. Retail has
  `mov eax,[esi+30h] ; push 1 ; lea ecx,[eax+ebp]`; ours has `mov ecx,[esi+30h] ; add ecx,ebp ; push 1`. The loop-head
  alignment filler differs with it; everything else is byte-exact. | tried: `operator[]` as `*(begin()+n)` and as raw
  `_M_start[n]`, a local reference, `.Get()`, int and long index, `begin()+i`, `this->`, nested ifs, an IS_THING_ALIVE
  variant, the QFE-4035 compiler (all inert, ndiff 9 including noise) | next: lane gap: address computation scheduled
  after the argument push (try an inline `operator->` on the element that takes the vector and the index, or a
  `CThing* effect = AmmoEffects[i].Get()` re-read, only if a later wave shows the same shape closing).

## Skipped
None.

## Label corrections
- 0x007C3F10: catalog `CTCActionUseBuyHouse::~CTCActionUseBuyHouse` is `CTCHealLife::~CTCHealLife` (first vtable store
  0x0126E36C, RTTI `.?AVCTCHealLife@@`).
- 0x008320C0: catalog `CTCTavernGame::~CTCTavernGame` is `CThingAICreature::~CThingAICreature` (vtable 0x0127293C).
- 0x007BF2B0: `CTCShotDefinitionBase` is `NThingShot::CTCShotDefinitionBase` (RTTI `.?AVCTCShotDefinitionBase@NThingShot@@`).
- 0x007E39D0: `Transfer<signed_char>` is `Transfer<CDefString>` (RTTI of the wrapper vtable 0x012704F8).
- 0x007BED50: manifest `CTCPhysicsNavigator::EnableNavigator` is `NThingShot::CTCShotDefinitionBase::SetCanBeDeflected(bool)`.
  EnableNavigator is virtual in the PDB, but this is a direct call. The proof is in "Proven" below.
- 0x0044FC00: manifest `CPersistTraits<float>::TransferBinaryIn` is `CDefString::TransferBinaryIn(CDataInputStream&)`.
- 0x006B82E0 (manifest `_Cons_val<...>`), 0x00A22F80 (manifest `CMatrix3x3::MultiplyByQuaternionRotation`),
  0x0066ADD0 (manifest `CGameScriptThing::SetToKillOnLevelUnload`), 0x007C4E80 / 0x007CBB80 / 0x00834F90 (manifest
  `CGuiControlTreePane::SortTreeRecursively`), 0x00834FD0 (manifest `LTextTreeWalkThrough::BuildTreeArray`),
  0x00487150 (manifest `CHTG_Executioner::OnPredicateFail`). The correct spellings are in NAME_FIXES.tsv (22 rows).

## Names
### Proven (two sources each; exact PDB spelling, checked in the objects with objdump)
- Leaves: 0x007A9D60 `??1CTCAssassinRush@@UAE@XZ`, 0x007BF2B0 `??1CTCShotDefinitionBase@NThingShot@@UAE@XZ`,
  0x007C3F10 `??1CTCHealLife@@UAE@XZ`, 0x007C5DB0 `?MakeSwordsAppear@CTCGhostSword@@QAEXXZ`, 0x007C7960
  `??1CTCTurncoat@@UAE@XZ`, 0x007C89F0 `?CastFireball@CTCFireballSpell@@QAEXPAVCThing@@@Z`, 0x007CA8E0
  `??1CTCHeroReceiveItems@@UAE@XZ`, 0x007CBF60 `??1CTCMultiArrow@@UAE@XZ`, 0x007CDC20 `??1CTCBurntEffect@@UAE@XZ`,
  0x007E39D0 `??$Transfer@VCDefString@@@CPersistContext@@QAEXPBDAAVCDefString@@ABV1@@Z`, 0x008320C0
  `??1CThingAICreature@@UAE@XZ`, 0x007A8BE0 `?FrameUpdate@CTCBattleCharge@@UAEXXZ`. Each is backed by vtable or RTTI
  plus the Ego_r.pdb name set.
- 0x007BED50 `?SetCanBeDeflected@CTCShotDefinitionBase@NThingShot@@QAEX_N@Z`. Sources: the store to +0x68 is Ego_r
  CanBeDeflected (+0x6C less 4); the setter run 0x007BED30/40/50 (+0x66/+0x67/+0x68) follows the donor order
  SetKnockDown / SetDecapitate / SetCanBeDeflected; vtable 0x0126E1A4 slot 21 (0x004D5472) returns 0x77; caller
  0x008D5DEC uses it on a CTCShotDefinitionBase it just Init-ed.
- 0x006B82E0 `?RadialBlurFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z`,
  0x00A22F80 `?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z`,
  0x0044FC00 `?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z`,
  0x0066BD00 `?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z`,
  0x0066ADD0 `?StopAttachedSound@CTCSoundPlayer@@QAEXK@Z`,
  0x004C7990 `?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ`,
  0x009AD410 `?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z`,
  0x004C08C0 `?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z`,
  0x007C0AD0 `?CreateShot@NThingShot@@YIPAVCThing@@JPAV2@0PBV2@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@PBVC3DVector@@PBVCRightHandedSet@@@Z`,
  0x0057B1DB `?ModifyStaminaBy@CTCHeroStats@@QAEXJ@Z`,
  0x004C8E50 `?SetAsDrawable@CThing@@UAEX_N@Z` (CThing vtable 0x01239ABC slot 28),
  0x00A13C60 `?Rotate@C3DVector@@QAEXABV1@M@Z`,
  0x004C73D0 `?GetPos@CThing@@QBEABVC3DVector@@XZ`,
  0x007C57C0 `?AddSword@CTCGhostSword@@AAEXABVC3DVector@@@Z`,
  0x00686A70 `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ`,
  0x00686DF0 `?RemoveFromGlobalTCs@CTCBase@@QAEXXZ`,
  0x0072ABD0 `?FadeOrKillEffect@CTCSpecialAbilities@@SIXAAV?$CIntelligentPointer@VCThing@@@@@Z`,
  0x006629B0 `?RemoveFromCreatureInteraction@CThingCreatureBase@@QAEXXZ`,
  0x006661B0 `??1CThingCreatureBase@@UAE@XZ`,
  0x00487150 `?SpecialAbilityUnfreezeControls@CPlayer@@QAEXXZ`,
  0x00833A10 `?SuspendAI@CThingAICreature@@QAEX_NPAX@Z`,
  0x0073ABA0 `?RemoveMode@CTCCreatureModeManager@@QAEXK@Z`,
  0x004C7A30 `?GetPlayer@CThing@@QAEAAVCPlayer@@XZ`,
  0x004C9840 `?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z`,
  0x00662650 `?FinishCurrentAction@CThingCreatureBase@@QAEXXZ`,
  0x007A76F0 `?CreateStartUpEffect@CTCBattleCharge@@AAEXXZ`,
  0x007A8700 `?RushForwards@CTCBattleCharge@@AAEXXZ`.
- STLport instantiations from the PDB member types:
  - 0x007C4E80 `_List_base<CIntelligentPointer<CThingCreatureBase>>::clear`
  - 0x007CBB80 `_List_base<pair<CIntelligentPointer<CThingObject>,bool>>::clear`
  - 0x00834F90 `_List_base<CEnginePrimitiveHandle>::clear`
  - 0x00834FD0 `_Rb_tree<void*,...>::_M_erase`

  The exact symbols are in NAME_FIXES.tsv.
- Member offsets: CSpecialAbilitiesAssassinRushDef::AssassinEndSeconds retail +0x4C (Ego_r +0x50 less 4; the landed
  FrameUpdate reads DistFromFloor at +0x44 under the same rule). CSpecialAbilitiesFireballSpellDef Inclination +0x40 and
  StaminaCost +0x44; CSpecialAbilitiesGhostSwordDef CreationDistance +0x54.

### Probable
- CTCBase slots 0-27 use the names from the agent525 model (only the slot count and order matter for codegen; none is
  emitted). CThing bitfields at +0x91..+0x93 are in PDB declaration order; only Dead and IsAffectedByBulletTimeOverride
  are exercised.

### Descriptive
- CTCPhysicsBase vtable 0x0125D2C4 slots 28-68: `CUnprovenSlotNN_0125d2c4()` (no PDB name proven in retail slot order).
- CThing vtable 0x01239ABC slots 0-27: `CUnprovenSlotNN_01239abc()` (only slot 28 SetAsDrawable is proven).
- `enum { INTERFACE_TYPE = N }` inside each TC model (the interface values are proven from vtable slot 21; the member
  name is ours).
- Parameter names of CreateShot / GetDummyObject (not in the PDB name set).

## Header debt
- engine/CTCTurncoat.h, CThingAICreature.h, CThingCreatureBase.h and CTCBase.h flatten the vptr (and, for CTCTurncoat,
  the member objects) into data, so a destructor or virtual call cannot be written on them. Local models use the
  headers' offsets.
- engine/CMemoryDataInputStream.h flattens its CDataInputStream base, so a `CMemoryDataInputStream&` cannot bind
  `CDataInputStream&` (007e39d0 uses a local derived class).
- No headers exist for CTCAssassinRush, CTCShotDefinitionBase, CTCHealLife, CTCGhostSword, CTCFireballSpell,
  CTCHeroReceiveItems, CTCMultiArrow, CTCBurntEffect, CTCBattleCharge or CThing.
- vland warns SHARED_HEADER_AVAILABLE for CIVCountedPointeeBase, CDefinitionManager, CEnvironment and CWorld. These are
  declaration-only callee models (no `this` of those types is laid out); the headers are flat PDB layouts.

## Review at landing
- No RELOC_CONFLICT or RELOC_DUPLICATE. The RELOC_NAME warnings (0x006B82E0, 0x00A22F80, 0x007BED50) are wrong
  catalog labels; NAME_FIXES.tsv has the spellings.
