# Hand chunk 656 (TeleportData .. CModeGuildmasterPowerUpHeal): 10 of 10 byte-exact

Never-attempted pool NA002: ten rows whose masked body equals a landed row. Every row was re-authored from its twin with its
OWN identity (class / element type / vtable / symbol proven from callers, vtables, Ego_r/FableWin name sets and COMDAT
placement); no row kept the twin's names, and every manifest label was propagated and wrong. 10 RELOCATION_MATCH, all tests
PASS under g++ and the lane's VC7.1; vland: 10 WINS, no RELOC_IDENTITY, two advisory RELOC_NAME WARNs on 0x007F8140 (both callee
labels are the wrong landed names fixed in NAME_FIXES.tsv). Probes: eight rows matched on probe 1, 0x004C2DF0 on probe 2 (one
real fix), 0x008D69A0's probe 2 was a fidelity-only change; the 12-probe park rule never applied.

## What matched (10)
- **NScript::CQ_BountyHuntScript::TeleportData**
  - 0x00CD4550 `TeleportData::~TeleportData()` -- implicit dtor (4 CCharStrings in reverse order, tail jump on defName).
    Ten callers in the Q_BountyHunt script bodies destroy the stack TeleportData built by the landed ctor 0x00D15140 and pushed by
    the landed vector<TeleportData>::push_back 0x00D18C00; Ego_r 24-byte layout (the only PDB struct with CCharStrings at
    0/0xC/0x10/0x14); Ego_r places ??1TeleportData after ~CTimer as retail does. Shape: no user dtor + emission helper.
  - 0x00CD4FA0 `std::_Destroy<TeleportData>(TeleportData*)` -- STLport 4.6 stl/_construct.h single-element destroy, /Gr. 0 call
    sites; proven by placement: Ego_r's script-manager TU emits _Dest_val<TeleportData> then _Dest_val<CDataMarkerOutput::CElement>,
    retail has 0x00CD4FA0 (inlined ~TeleportData) then 0x00CD4FD0 (inlined ~CElement). Dinkumware _Dest_val = STLport _Destroy(T*).
- **C3DAnimationBlendState**: 0x004C2DF0 `~C3DAnimationBlendState()` -- implicit dtor: CIVCountedPointer<CSharedData> Data
  released, tail jump to ~CBaseClass. Callers destroy stack instances built by the ctor 0x004C2DD0 (stores vtable 0x01239838 =
  C3DAnimationBlendState); its ??_G 0x004C3CD0 inlines the same body. Uses engine/CIVCountedPointeeBase.h (retail opt-in) and
  engine/CBaseClass.h. Shape lesson: CIVCountedPointer::Release must hold Object in a named local (probe 1 reloaded Object after
  the `dec`, 38 v 35).
- **std::pair (map<CCharString, CIVCountedPointer<CAnimComponentBase> > value)**: 0x005E0A80 `pair<const CCharString,
  CIVCountedPointer<CAnimComponentBase> >::~pair()` -- implicit dtor: release second, tail jump ~CCharString on first. Element type
  proven (the TU's landed _Rb_tree::_M_erase 0x005E1F30 inlines this exact body per node; _Cons_val 0x005E0F40; Ego_r map type).
  Which of the three identical 0-caller copies (0x005E0A80 / 0x005E0AB0 / 0x005E1790) is ~pair<const K,V>, ~pair<K,V> or
  _Destroy<pair<const K,V>> is NOT provable from callers: the symbol is tentative (see Review at landing).
- **CCreatureAction_FireMissileWeapon**: 0x0078ADF0 implicit dtor -- CDefPointer<const CMeleeCombatAbilityDef> PSpecialShotDef
  (+0xC4) released through engine/CDefPointer.h, PTarget (+0xA8) ~CBaseIntelligentPointer, tail jump ~CCreatureActionBase. Callers:
  ??_G 0x008D6140 (vtable 0x01285424, RTTI CCreatureAction_FireMissileWeapon) + five stack-instance sites.
- **std::__uninitialized_copy<CTextureMorphMap*, CTextureMorphMap*>**: 0x0086D530 -- STLport 4.6 _uninitialized.h verbatim, /Gr;
  CTextureMorphMap = CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<..> > (landed 0x0086EA80
  model). Callers _M_insert_overflow 0x0086E1C0 / _M_fill_insert 0x0086E4B0 pass non-const pointers; the twin 0x00674CE0 is the
  const-source instantiation (bound PBV), which is why the two identical bodies are two COMDATs.
- **CHeroSoulsCreatureDef**: 0x00F270D0 implicit dtor -- ~CCharString Location, CreatureType, CDefPointeeBase vtable store, tail
  jump ~CResource. ??_G 0x00F270B0 is slot 0 of vtable 0x012FB500 (the landed copy ctor 0x00F27060's table). The retail
  CDefClassBase opt-in reproduces the store exactly (the inline ~CDefClassBase store is dead behind ~CDefPointeeBase's).
- **CPlayer**: 0x00487550 `CPlayer::PrepareForLoad()` -- PHeroLogBook->Clear() (landed 0x0047AE50) then ResetModes() (tail);
  engine/CPlayer.h through the `#define CPlayer CPlayerLayout` derive trick (landed 0x00636E50) so the symbol mangles @CPlayer@@.
- **CTCActivationReceptorDoor**: 0x007F8140 `Deactivate()` -- SetDoorState(false) then CTCActivationReceptorBase::Deactivate
  (tail). Slot 29 of vtable 0x0124146C; Ego_r order SetDoorState / Activate / Deactivate = retail 0x007F80C0 / 0x007F8120 / 0x007F8140.
- **NCreatureMode::CModeGuildmasterPowerUpHeal**: 0x008D69A0 `OnDeactivate()` -- `if (ParentThing->GetType() == 1)
  SuspendAI(false, this)` then CModeStandBase::OnDeactivate (tail). Slot 6 of vtable 0x012685BC (RTTI CModeGuildmasterPowerUpHeal);
  slot 5 is the landed OnActivate 0x008DDD70 with the mirror SuspendAI(true, this). CClass::Type +0x10 via FableWin inline GetType.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Names: proven vs descriptive
Proven (two sources each, listed in each source's Identity line): 0x00CD4550, 0x00CD4FA0, 0x004C2DF0, 0x0078ADF0, 0x0086D530,
0x00F270D0, 0x00487550, 0x007F8140, 0x008D69A0. Tentative: 0x005E0A80 -- the element type (pair<const CCharString,
CIVCountedPointer<CAnimComponentBase> >) is proven, the choice of ~pair<const K,V> among the three identical 0-caller copies is a
best inference (first copy = the value_type's dtor, by analogy with ~TeleportData preceding _Destroy<TeleportData>).
Descriptive (not retail symbols, never graded): the four emission helpers (EmitTeleportDataDestructor, EmitBlendStateDestructor,
EmitAnimComponentPairDestructor, EmitFireMissileWeaponDestructor, EmitHeroSoulsCreatureDefDestructor) and the placeholder virtual
slots v00/v01.. in local polymorphic models. The literal `1` for the creature CClass::Type value (enum name unproven).
Callee names used: CCharString::~CCharString 0x0099EAE0, CBaseClass::~CBaseClass 0x0099A2E0, CResource::~CResource 0x009FC550,
CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CCreatureActionBase::~CCreatureActionBase 0x00693EF0,
vector<pair<unsigned long, CTextureMorph> >::vector(const vector&) 0x00673E10 (bound), CHeroLogBook::Clear 0x0047AE50,
CPlayer::ResetModes 0x00634870, CTCActivationReceptorDoor::SetDoorState 0x007F80C0, CTCActivationReceptorBase::Deactivate
0x007F7700, CThingAICreature::SuspendAI 0x00833A10, CModeStandBase::OnDeactivate 0x00861D00 (all Ego_r / FableWin spellings).

## Label corrections
- 0x00CD4550 "_Dest_val<allocator<CMacro>,CMacro>" -> NScript::CQ_BountyHuntScript::TeleportData::~TeleportData.
- 0x00CD4FA0 "_Dest_val<allocator<CMacro>,CMacro>" -> std::_Destroy<NScript::CQ_BountyHuntScript::TeleportData>.
- 0x0078ADF0 label right (CCreatureAction_FireMissileWeapon::~CCreatureAction_FireMissileWeapon).
- 0x0086D530 "_Uninit_copy<CFeatDef*,CFeatDef*,allocator<CFeatDef>>" -> std::__uninitialized_copy<CTextureMorphMap*, CTextureMorphMap*>.
- 0x004C2DF0 label right (C3DAnimationBlendState::~C3DAnimationBlendState); 0x005E0A80 "~C3DAnimationBlendState" -> std::pair<const
  CCharString, CIVCountedPointer<CAnimComponentBase> >::~pair (tentative symbol, proven type).
- 0x00F270D0 "CAIStateGroup_HoverHornetQueen::~.." -> CHeroSoulsCreatureDef::~CHeroSoulsCreatureDef.
- 0x00487550 "_Dest_val<..pair<long,CMusicManager::CRegisteredMusicEntry>..>" -> CPlayer::PrepareForLoad.
- 0x007F8140 "CTCActivationReceptorDoor::vfunc_29" -> CTCActivationReceptorDoor::Deactivate.
- 0x008D69A0 "CModeDragonRearUp::OnDeactivate" -> NCreatureMode::CModeGuildmasterPowerUpHeal::OnDeactivate.
- Callees: 0x007F80C0 "SetDoorOpen" -> CTCActivationReceptorDoor::SetDoorState(bool) (private); 0x007F7700 "SetMemberConst_007f7700"
  / "CXMVPlaybackInfo" -> CTCActivationReceptorBase::Deactivate; 0x007F8120 "vfunc_28" -> CTCActivationReceptorDoor::Activate;
  0x00CD4FD0 "_Dest_val<..CRegisteredMusicEntry..>" -> std::_Destroy<CDataMarkerOutput::CElement>.
- Twins whose landed names are themselves wrong: 0x004047A0 is fine (~CStringParser); 0x005E0AB0 / 0x005E1790 "CSharedOwner_*"
  are the other two copies of the anim-component pair destroy family; 0x00628F10 is not a true masked twin of 0x00487550 (its body is
  `lea ecx,[esi+0A8h]` + ~CBaseIntelligentPointer + ~CCreatureActionBase, a creature-action dtor, which its landed source models);
  0x00C2D2D0 (OnInitialActivate stand-in
  `h1(0); h2()`) and 0x008D6A10 (CModeDragonRearUp::OnDeactivate with SomeState::Helper) use invented callee names for
  SetDoorState-like / SuspendAI calls.

## Header debt
- 0x007F8140: engine/CTCActivationReceptorBase.h flattens the vptr into a data member; the receptor chain is the landed
  CTCActivationReceptorChest 0x007FFCD0 local model.
- 0x008D69A0: engine/CModeStandBase.h flattened (no virtuals) and engine/CClass.h flattened; CModeBase / CModeStandBase /
  CClass / CThing / CThingAICreature are local polymorphic models with the Ego_r member names (ParentThing +4, Type +0x10).
- 0x0078ADF0: engine/CCreatureAction_FireMissileWeapon.h and engine/CCreatureActionBase.h flatten the vptr; the creature-action
  chain is the landed local model (CCreatureActionBase 0xA8, e.g. 0x008CF190) with the header's member names.
- CCharString is modelled locally everywhere (rebuild/include/fable_string.h carries `#pragma pack` and wchar_t; no
  engine/CCharString.h); STLport pieces are local copies of the 4.6 text, as in the landed siblings.
