# Hand chunk 702 (NUISystem::CParticleEffect .. CTCFamilyMember): 10 of 10 byte-exact

All ten rows RELOCATION_MATCH under q.py (gall.sh, honouring `// Leaf:`) and under the read-only vland check (WINS 10,
behaviour PASS; no jump tables). Tests PASS under g++ (tall.sh) and VC7.1 (tall.py). Every relocation of every leaf checked
against retail with $S/w22/calls.py (imports: free 0x00BFEA14 only). No flags, pragmas, retail_asm or volatile. 14 probes in all
(1 per row except 0x00657200: 2, 0x00BFBBF0: 4); logs in probe/<addr>.log. Every extent checked against the padding: all ten end
at their tail `jmp` with int3 padding after; no extent fixes. All ten catalog labels were wrong or incomplete.

## What matched (10)
- NUISystem::CParticleEffect::~CParticleEffect() 0x00543CF0 -- user out-of-class virtual dtor `if (PHandle) { PHandle->Kill();
  PHandle = 0; }` over the landed CComponent / CChangingStateComponent model (0x0052CCF0); VC7.1 hoists the PHandle load above
  the three vtable stores on its own.
- NUISystem::CSwappingStateComponent::~CSwappingStateComponent() 0x005474C0 -- user empty dtor + STLport
  vector<pair<unsigned long, float> > (the landed ~CDynamicList 0x00541350 shape).
- NCreatureMode::CModeOutOfActionBase::~CModeOutOfActionBase() 0x008D7580 -- `if (PEmitter.GetPItem() &&
  !PEmitter.GetPItem()->IsDead()) PEmitter.GetPItem()->Kill(true);` (three out-of-line GetPItem calls), pack(1) PEmitter at
  +0x6D (Ego_r offset 109), implicit CIntelligentPointer<CThing> dtor (no vtable store) -> ~CBaseIntelligentPointer, jmp ~CModeStandBase.
- CTCHeroOnlineScoreboard::ClearScoreboard() 0x00562E10 -- real STLport 4.6 map / _Rb_tree (inline clear, private _M_erase),
  list::clear = inline `_Base::clear()` over out-of-line `_List_base<T>::clear`, `SelectedClan = CWideString::NullString`.
- CLocalDetailPrimitiveMeshZSpriteBatch::CObject::~CObject() 0x00BFBBF0 -- implicit dtor of a non-polymorphic 0x4C struct whose
  only non-trivial member is CEnginePrimitiveHandle PrimitiveHandle (+0x44); needs the fleet's `__forceinline` on the handle's
  virtual dtor (plain inline: 8-byte `add ecx,44h; jmp` call; user ~CObject inline or out of class made no difference).
- NUISystem::CSpellContainerList::AddSpellSelectionChild / AddSpellIndicationChild 0x00544310 / 0x00544330 --
  `SpellXChild = child.Get(); CComponent::AddChild(child);` (qualified base call = direct `jmp`, skipping the list's own AddChild).
- NPlayerGui::CDrawAbilityButtonsPC::GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef&) const 0x00657200 --
  the interface in a NAMED local (`CGamePlayerInterface* playerInterface = GetPlayer().PlayerInterface;`) before the
  MouseButton read; the one-expression chain pushes the argument first (24 bytes, no tail call).
- CTCSmokeGenerator::Copy / CTCFamilyMember::Copy 0x00722570 / 0x00778B10 -- CTCBase slot 12 Copy(const CTCBase&): C-style cast
  to the derived class, member assignments, STLport vector<T>::operator= declared only (out of line, tail jump); no CTCBase::Copy.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Names and their evidence (all proven from two or more sources unless marked)
- 0x00543CF0 ??1CParticleEffect@NUISystem@@UAE@XZ -- stores the three CParticleEffect vtables; ??_G 0x005440B0 inlines the same body;
  Ego_r + FableWin; Ego_r class block order mirrored (??1 first, then Initialise / ChangeState / Update / Draw / GetDataFromDef / ctor).
- 0x005474C0 ??1CSwappingStateComponent@NUISystem@@UAE@XZ -- stores the three CSwappingStateComponent vtables; ??_G 0x00547AB0
  inlines it; Ego_r + FableWin; SwappingStates is the only non-trivial member.
- 0x008D7580 ??1CModeOutOfActionBase@NCreatureMode@@UAE@XZ -- stores 0x0126743C; own ??_G 0x0073DA30 + derived ??_G 0x00740220 /
  0x00740310 call it; Ego_r + FableWin; PEmitter at Ego_r 109 = the `lea esi,[edi+6Dh]`.
- 0x00562E10 ?ClearScoreboard@CTCHeroOnlineScoreboard@@AAEXXZ -- Ego_r (private); the static ConsoleClearScoreboard 0x00562EB0 is a
  bare jmp here; Ego_r block order ClearScoreboard / Close / ConsoleClearScoreboard = retail 0x00562E10 / E70 / EB0; members match.
- 0x00BFBBF0 ??1CObject@CLocalDetailPrimitiveMeshZSpriteBatch@@QAE@XZ -- callers walk 0x4C-stride CObject arrays in
  ~CLocalDetailPrimitiveMeshZSpriteBatch 0x00BFB590 and the array deleter 0x00BFBD20 (the bound spelling of 0x00BFB590 already
  agrees); Ego_r + FableWin; Ego_r 76-byte CObject with PrimitiveHandle +68. Implicit vs user-empty dtor: not visible in the bytes;
  implicit chosen (no evidence of a user body).
- 0x00544310 / 0x00544330 ?AddSpellSelectionChild / ?AddSpellIndicationChild@CSpellContainerList@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
  -- vtable 0x012475C4 slots 142 / 143 (only references); Ego_r + FableWin; donor order and 0x20 spacing match retail; the stored
  members are Ego_r SpellSelectionChild / SpellIndicationChild 0x30 lower. Accessor `child.Get()` vs `&*child`: same bytes (unproven).
- 0x00657200 ?GetAssignableSpecialMoveIndexForButtonDef@CDrawAbilityButtonsPC@NPlayerGui@@MBEJABVCAbilityButtonGuiDef@@@Z -- slot 3
  of 0x0125A7D8; Ego_r + FableWin; donor spacing Draw (0x10) / this (0x20) / DrawAbilityList = retail slots 2 / 3 / 4.
- 0x00722570 ?Copy@CTCSmokeGenerator@@UAEXABVCTCBase@@@Z / 0x00778B10 ?Copy@CTCFamilyMember@@UAEXABVCTCBase@@@Z -- vtable slot 12
  (CTCBase Copy slot, landed 0x005E49E0 / 0x0071CD90 / 0x007305A0); Ego_r (+ FableWin for the smoke generator); member layouts
  from Ego_r (GenerationDistance +0xC, GenerationPositions +0x10; FamilyMembers +0xC).

## Callee names (each matches the PDB; new correct spellings listed in NAME_FIXES.tsv)
??1CChangingStateComponent@NUISystem@@UAE@XZ 0x0052CCF0, _free 0x00BFEA14, ?Kill@CParticleEmitter@NParticleEngine@@QAEXXZ 0x00AEE8C0,
?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50, ?Kill@CThing@@QAEX_N@Z 0x004C9B80, ??1CBaseIntelligentPointer@@UAE@XZ
0x00A01C10, ??1CModeStandBase@@UAE@XZ 0x0073D860, the DownloadedClanScoreboard _Rb_tree _M_erase 0x00569A80 (full STLport spelling),
?clear@?$_List_base@VCClanInfo@... 0x00566DC0, ?clear@?$_List_base@VCClanMember@... 0x00566E10, ??4CWideString@@QAEABV0@ABV0@@Z
0x0099B7D0, ?NullString@CWideString@@2V1@A 0x013BCA24 (queue-accepted spelling), ??1CBaseClass@@UAE@XZ 0x0099A2E0,
?AddChild@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z 0x00533B50,
?GetPlayer@CDrawBase@NPlayerGui@@QBEABVCPlayer@@XZ 0x00643D84, ?GetAssignableSpecialMoveIndexForMouseButton@CGamePlayerInterface@@QBEJW4EMouseButtonControl@@@Z
0x00445F40, the two STLport vector operator= 0x00722F30 / 0x00778C00 (the landed rows' own leaves). No new symbol collides with a
landed row's spelling for a different address. Collisions with wrong landed spellings of the same address are in NAME_FIXES.tsv:
0x00643D7C / 0x00643D84 (landed as Forward_GetMainPlayer / _Cons_val), 0x00569A80 / 0x00566DC0 (invented CRetail* names),
0x00562E10 (bound QAE), 0x00AEE8C0 (bound without namespace).

## Label corrections
All ten rows (NAME_FIXES.tsv): 0x00543CF0 / 0x008D7580 / 0x00BFBBF0 lacked their namespace / enclosing class; 0x005474C0 was a
propagated ~CParticleEffect (really ~CSwappingStateComponent); the five vfunc_N placeholders got their PDB names; 0x00562E10's
name was right but its bound spelling is public (QAE) where Ego_r says private (AAE). Neighbours found mislabelled on the way:
0x00643D7C / 0x00643D84 (CDrawBase::GetPlayer pair, landed as `Forward_GetMainPlayer_via0x8`, file names `_Cons_val`), 0x00643FC8
(GetAssignableSpecialMoveIndexForButtonDef, landed descriptive), 0x00445EE0 (bound without const), 0x00569A80 / 0x00566DC0
(invented container names), 0x00562E70 (probable Close, landed descriptive ResetScoreboard), 0x007305A0 (CTCDoor_Methods stand-in).
Already queued fleet-wide and not repeated here: 0x0099B7D0, 0x00A01B50, 0x00A01C10, 0x0052CCF0 (??1CDraggableInto binding).

## Header debt
- engine/CTCBase.h: flattened vptr, `struct`-era generated layout (as chunks 700/701); the two Copy rows and ClearScoreboard use a
  local polymorphic CTCBase.
- engine/CModeStandBase.h: flattened (vptr as a data member, CCountedPointer members split into Data/Info); 0x008D7580 uses a local
  polymorphic 0x68-byte base.
- engine/CObserver.h: global namespace and flattened (vland WARN SHARED_HEADER_AVAILABLE on four NUISystem rows); retail is
  NUISystem::CObserver (the landed 0x0052CCF0 model is reused).
- engine/CParticleEmitter.h: global-namespace flat struct (vland WARN); retail is NParticleEngine::CParticleEmitter (class key V).
- engine/CEnginePersistentPrimitiveBase.h: flattened; 0x00BFBBF0 declares the polymorphic base locally over
  engine/CIVCountedPointeeBase.h (retail opt-in). CEnginePrimitiveHandle / CIVCountedPointer have no engine headers.
- engine/CGamePlayerInterface.h: flat data layout without methods; 0x00657200 declares the method on a local class (as 0x00643FC8).
- engine/CIVCountedPointeeBase.h retail branch uses FABLE_STATIC_ASSERT without including rebuild_abi.h (the includer must
  include it first).
- No engine headers for NUISystem::CComponent / CChangingStateComponent / CParticleEffect / CSwappingStateComponent /
  CSpellContainerList, NPlayerGui::CDrawBase family, CAbilityButtonGuiDef, CTCHeroOnlineScoreboard, CTCSmokeGenerator,
  CTCFamilyMember, CThing, CBaseIntelligentPointer, CWideString, CLocalDetailPrimitiveMeshZSpriteBatch.
