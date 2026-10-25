# rename1 -- cleanup / rename pass 1 (2026-10-24)

Status: DONE. Every row in RENAMES/FIXES (main and `_chunk420plus`) re-graded MATCH/RELOCATION_MATCH under VC7.1
(RTM 13.10.3077 lane, ps1 CompilerFlags) and its test printed PASS when it was written, and again in a final full
re-grade (tools/regrade_all.py, log work/regrade_all.log). Queue (1): FINDINGS chunks 350-421 (lines 473-556). Queue (2):
callees LABEL_FIXES/CANON rows with PDB/RTTI evidence below 0x00C08AE0 (section "Queue (2)").
Scope split (coordinator, mid-pass): chunk 420 and 422+ belong to sweep1 ($S/w22/sweep1). Rows from chunks 420 and
422-426 that this pass had already verified before the split are kept apart in RENAMES_/FIXES_/LABELS_/
UNRESOLVED_chunk420plus.tsv (sources still in renames/ and fixes/) for the coordinator to dedupe against sweep1.

## Method
- Tools (tools/, read-only on the repo): cat.py (catalog / ps1 / manifest / label lookup), xr.py (cached call/jmp/abs
  xrefs over Fable.exe .text), g.py (re-grade exactly like recheck: catalog oracle bytes, ps1 CompilerFlags, `// Leaf:`
  from the source, then the test with the source object as link fallback; renames use the `<addr>_TEST PASS`
  pattern), row.py (grade + append/replace the TSV row; refuses anything not MATCH/RELOC + PASS or with a HARD reloc
  error), cp.py (copy the landed source/test into renames/ or fixes/), sub.py (grade a secondary function of a TU
  against retail bytes at any VA), cc.py (compile one file and show diagnostics).
- Evidence: every claim checked against >= 2 of: Ego_r/Ego_d/FableWin PDB name sets (used as NAME SETS only),
  ghidra_out/struct_layouts_egor.tsv, RTTI descriptors / rtti-vtables.tsv, xrefs, retail bytes, landed callee sources.
- STL spelling: STLport 4.x names (`__uninitialized_copy`, `__destroy_aux`, `__copy_backward`, `_M_fill_insert`, ...),
  written in `namespace std` because retail/PDB symbols mangle `@std@@` (the brief's `_STL` is STLport's macro for it).
- Placeholder signatures: where retail drops a parameter the PDB name has (e.g. unnamed binary-only
  `CPersistContext::TransferMap<V,K>(map&)`), the template name/arguments come from the PDB and the `// Leaf:` pins the
  prefix; the comment says the parameter list is a placeholder (precedent: landed Transfer<float> 0x431061).

## Counts (header line excluded)
| file | main (chunks 350-419, 421 + queue 2) | `_chunk420plus` (chunks 420, 422-426; dedupe vs sweep1) |
|---|---|---|
| RENAMES.tsv | 252 | 29 |
| FIXES.tsv | 100 | 10 |
| LABELS.tsv | 92 | 7 |
| UNRESOLVED.tsv | 17 | 1 |

## Notable corrections so far
- CDateAndTime vector cluster (chunks 370/378): 0x40DED0, 0x40DF00, 0x485239, 0x48521A, 0x485372, 0x48563D,
  0x48608C, 0x48623F were Dinkumware / CHeroMarriageInfo (24-byte) / EWeaponClass / descriptive labels; the chain
  resize(n) 0x48623F -> (CDateAndTime ctor 0x9E9AB0) -> resize(n,x) 0x4861F7 -> _M_fill_insert 0x48608C -> helpers
  pins the 28-byte element to Ego_r.pdb CDateAndTime (save-file name/time lists). The _M_fill_insert stand-ins are
  now explicit template specialisations, so its call targets carry the same symbols as the renamed helpers.
- 0x47280C was a fake CEngineLightingManager::UpdateShadowScene stub; it is the unnamed binary
  CPersistContext::TransferMap<CCharString,long> for CGlobalDef::ConversationCameras.
- 0x47BAF0 is __destroy_aux<CHeroLogBook::CHeroLogBookEntry*> (three ~CWideString calls), not a vector member.

## For the owner
- New VC7.1 rule worth a GOTCHAS line: under `#pragma optimize("s", on)` an explicit `p->~T()` (STLport `_Destroy`) of a
  class whose destructor is not inlined is emitted as `push 0 ; call ??_GT` (the scalar deleting destructor with flag 0),
  even for a non-polymorphic implicit-destructor type such as std::pair -- reproduced for 0x5799DE/0x5799FD; the same
  shape is 0x5B1B5E (CDefPointer ??_G called with 0 from __destroy_aux 0x5B26E9).
- Callers still bind old names after this pass's renames (RELOC_NAME warnings only, bytes unaffected): ~40 landed TUs
  declare 0x833A10 as CThingAnimatable::SetSuspendAnimation (now CThingAICreature::SuspendAI); 0x523790/0x5237F0/
  0x523860/0x523910 declare 0x52C6A0 as a local CEntityScriptArray::resize; 0x42AB73 callers use list_T_0042ab73.
- 0x99B6A0's catalog/label name is a propagated "NScript::operator&&<...>" although its landed source is
  CWideString::CreateFromCharString (Ego_r-exact); the label file should follow the source.
- 0x59DE04/0x59DE76 (CTCInventoryTrade::UpdateBuy/SellActionsList) deliberately model CAction::UIActionParams as
  list<CComponent*> (PDBs: list<CActionParam*>) with byte evidence in the source; with 0x42B6B8 also taking a long from
  AddLearnedExpression, the TLC list type is an open model question (UNRESOLVED 0x42B6B8).
- Out-of-queue renames made while checking evidence chains (same >= 2-source standard): 0xA3AA10 CCompressorZlib::
  Initialise, 0x449970 GetMainPlayer (FIX), 0x5797FA SExpSpendUndo dtor, 0x42AB73 list<long>::~list, 0x59B098 tree dtor.
- pack(1) rows: 005d96a0, 0095f7d0 (model a packed struct; decide whether to keep or re-model with the header).
- 0x00418B7A binds `??1CBaseClass@@UAE@XZ` to 0x0099A300 (= CBaseClassNonCopyable) -> RELOC_CONFLICT warnings in
  every TU that declares a virtual ~CBaseClass.
- Manifest label of landed 0x472896 (`GFSerialiseMapBinaryIn<pair<long,long>,CCharString,less<CCharString>>`) is
  wrong: the PDB-exact symbol its callers reference is GFSerialiseMapBinaryIn<CCharString,long,less<long>>.
- Identical-code folding: the CDateAndTime helpers are shared with other 28-byte POD vectors (e.g.
  CActionInputControl via 0x4125A0's CInputManager::EventScanStart caller); the names follow the attested chain.
- Follow-ups not done (out of queue): 0x451E11 is the same unnamed-binary Transfer family as 0x47280C (fake
  UpdateShadowScene label); 0x411B00 "vector_push_back_copy" is vector<28-byte>::erase(first,last).
- 0x0043E430 (landed as CThingFilter_IsValid::operator()) is the CDrawBase slot-1 vcall thunk; rename candidate.
- ~121 `CCountedPointer_<deleter>::Reset` batch rows can be named from their deleter's dtor; the 132 collision-pass
  `std::list<T_addr>::~list` rows should be checked (some call _Rb_tree::clear and are tree dtors, like 0x4518E1).
- 0x807520 could take the template name std::__adjust_heap<CIntelligentPointer<CThing>*> (PDB name set).
- Stand-in wrappers 00905600/00945de0, 0066bd00/008a8f20; weak local models 0056c820/004af220/004c34a0/00c4b040;
  0049d550/0049d6b0 local struct CWorld.

- GF converters (chunk 382): retail 0x9E1D00/0x9E1D50/0x9E1DA0/0x9E1DD0/0x9E1E10/0x9E1E40 are GFRoundVXToNearestPixel,
  GFRoundVYToNearestPixel, GFAXToVX, GFAYToVY, GFVXToAX, GFVYToAY (Ego_r.pdb symbol order; V = the 65536-unit virtual
  space, A = pixels). ghidra_out/labels_chunk294 row `009e1da0 GFVXToAX` is wrong and should be dropped. Landed callers
  still binding the swapped pair (reloc names only; DrawArc 0x9DE2A0 fixed in FIXES): 0x438AA0 AddScreenMessage,
  0x48DBC3 GetBlackBarHeight, 0x9DB700 AddText, 0x9DCA80 Draw2DGraphic, 0x9DD210 Draw2DScaledGraphic, 0x9DDA50
  Draw2DBoxColours, 0x9DDFF0 Draw2DTriangle, 0x9DE140 Draw2DBoxOutline (several also declare them __stdcall; PDB YI).
- Chunk 382 follow-ups: callers of 0x438010 still bind CGameUI::Draw2DText and callers of 0x4FB490 bind
  GetCategoryWithIndex (reloc names only). 0x62F610/0x62F690 declare `struct CCountedPointer` (U) where the PDB has a
  class (V). 0x630A10/0x630D90 (FreeTree_CTreeOwner) are the _M_erase of the CUIDef trees (rename follow-up). 0x63A5F0
  (byte twin of 0x63A7E0, calls CMap 0x81FA90) is ambiguous between GetThemeMinCameraSizeZOffsetAt and ...AtBlock. The
  landed 0x631020 comment wrongly claims identical-code folding. 0x6313C0 is an uncalled map::operator= wrapper.
- 0x43119E (third byte twin of Transfer<unsigned long>/<long>, load helper 0x4311DF, 4 callers in 0x430900) keeps the
  same pack(1) UpdateShadowScene stand-in; its T is not pinned yet. 0x993EA0/0x993EB0 are landed as `S::WriteCBYTE`
  stand-ins although 30+ TUs bind them as WriteULONG/WriteSLONG.
- 0x655B69 is the out-of-line STLport _List_base constructor (not list::list): the uncalled 0x6561E2 is the 18-byte
  list(const allocator&) forwarder that calls it.
- Chunk 383/384 follow-ups: 0x65FF8E (landed "IsListedNodeActive") also calls the Torch find_if 0x65F943, so it is a
  CTCVillage TorchTasks member too (rename candidate). The 26-body GetSubDef_00xxxxxx family can follow the PDB template
  model now used for 0x65F604 (`GetSubDef<T>(CDefPointer<const T>*) const`, const GetPDefFromGlobalIndex); its 17 callers
  still bind GetSubDef_0065f604 (reloc names only). Landed 0x692E50 declares SetCreature public (QAE); the PDB has it
  protected (IAE). Landed 0x682140 and its callers model EOpinion as `typedef long` (mangles J) while other TUs have an
  enum EOpinion (W4): pick one. CEngineGraphic: chunks 379/384 model it as 14 bytes (PDB size, pack(1)), landed
  0x67E550/0x67E650 use 16 -- settle before more CDegradableInfo rows. 0x66BD00/0x8A8F20 still carry stand-in bodies.

- Chunk 385-388 follow-ups: the CFaction setters 0x6B94E0/0x6B9510/0x6B9530 are renamed to their PDB names
  (SetAsAlliedWithFaction / SetAsNeutralToFaction / SetAsEnemyOfFaction(const CFaction&)); 0x6B9510 had been a
  CSpellContainerList stand-in. 0x6A8A80 is CThingFilter_IsOnFire::operator() (0x6A8B40 is the real HasFreeActionPoint).
  0x6C20B0 moved module (CLevelLoader::CObject::OnAdd). 0x6C20A0 (catalog CTCUserActionPointBase::IsActionPointFree) is
  CLevelLoader::IsBusy and 0x4FB4A0 is CWorldMap::IsLoadingAnyMap (tail jump into it). 0x6CA950 belongs to
  CEngineTextureMorphSet (CTextureMorphs is the hero-morph CVectorMap). 0x6CAC20 / 0x5BAF28 are the mutable / const
  __uninitialized_copy<CCompositeBlendDesc> twins. 0x6DABE0's test is a model test (anonymous-namespace predicate:
  the instantiation cannot be linked from another TU).
- New VC7.1 note (0x6E97A0): in a `bool` function, `return !x;` (x a bool call result) widens to `sbb eax,eax ; inc
  eax`, while `return x ? false : true;` or `(bool)!x` keeps retail's byte-wide `neg al ; sbb al,al ; inc al` -- the
  landed `unsigned char` return was a workaround for this.
- 0x6E9E40's own name is still open (UNRESOLVED: SetCreatureAsAttacking candidate, no callers); 0x6EA770 still binds
  0x662410 as IsRegisteredCombatant (chunk 407 says IsInMeleeAsAttacker).
- Chunk 389-390 follow-ups: the def-pointer vector at 0x5B3275 (_M_insert_overflow) / 0x5B3325 (push_back) is
  vector<CDefPointer<const CThingBaseDef> > (CTCHeroMorph::Tattoos, CTCObjectAugmentations::Augmentations,
  GetTattooThings), NOT the CTattooDef vector: GetTattoos' vector<CDefPointer<const CTattooDef> > grows through
  0x721B10 and destroys through 0x580A0B / ??_G 0x57F9BF. Landed comments (0x71DA70, the old 0x5B3325 header) that call
  the two "identical-code folded" are wrong: retail has no ICF. Same cluster: 0x5B31EC (an AUTO_TINY
  `OnReadFinished` stand-in) is that vector's _M_clear (renamed); 0x5B26E9 is its __destroy_aux but stays UNRESOLVED
  (the `push 0 ; call ??_G` call form is not reproducible with the PDB's user destructor; see UNRESOLVED);
  0x5B149F (landed UninitFillN_IVCountedPointer) is its __uninitialized_fill_n -- left as is, name only, owner may
  rename. 0x71DD50 (GetTattoos) moved from Dinkumware to STLport spellings (FIXES).
- TC id 0x47 is CTCCarryable (RTTI vtable 0x0123B35C slot 21 = `push 47h ; pop eax ; ret`): 0x848AE0 / 0x980770 had
  it as CTCBoxContents / CTCClaim (FIXES). 0x686D50 is CTCBase::GetPlayerManager (CThingManager +0x30).
  0x711CF0 / 0x711520 are CAnimateThing::CCompleteAnimation's ctor and CAnimSet::Clear. 0x70C800's callee 0x70BD80 is
  GFGetInterpolatedLoopingValue. 0x6E9450 only declares BlockAttackerProng (never called there).

- Chunk 391-398 follow-ups:
  - CTCBase virtual slots (retail, 28): 0 dtor, 1 0x686960 (`ret 8`, name open: OnSerialise(CPersistContext&, bool)
    fits), 2 OnRemove 0x6867F0, 3 ConstructFromParams, 4 OnCreate, 5 InitialActivate, 6 OnInitialActivate, 9
    FrameUpdate, 11 DrawPreparePrimitivesForRendering, 12 Copy, 13 OnDie, 14 OnKill 0x6868F0, 15 OnLevelUnload, 16
    OnSetInLimbo, 21 GetInterfaceType, 22 GetClassTypeName, 26 GlobalUpdate, 27 HandleMessage (FableWin.pdb definition
    order; the editor-only Edit* virtuals are absent in retail). Slot 2 = OnRemove was pinned by elimination
    (CTCAssassinRush / CTCForcePushed / CTCAvoidRegionExit define exactly OnKill + OnRemove and override exactly 2 + 14).
    Renamed on that basis: 0x7AA830 / 0x7AA820 (AssassinRush OnRemove / OnKill thunks), 0x798540 ForcePushed OnRemove
    (was "FinalCleanup": the PDB's FinalCleanup is private non-virtual and is inlined into both slots), 0x7E93C0
    AvoidRegionExit OnRemove (was vfunc_2), 0x6867F0 / 0x6868F0 CTCBase::OnRemove / OnKill (were `$E2` stubs).
    Many other landed rows model these overrides as non-virtual (QAE) members, e.g. the landed CTCForcePushed::OnKill
    0x798510 (leaf prefix only) -- not changed here.
  - 0x798510 (ForcePushed OnKill) declares 0x833A10 as CThing::SetComponentRegistered(bool, CTCBase*) while 0x798540
    declared CThingAnimatable::SetSuspendAnimation (the callee's landed name); reloc names only.
  - Stale caller bindings after this block's renames (reloc names only): the landed 0x7E9410 OnKill still declares
    0x7E9250 as UpdateMaxExitWeight (PDB UpdateBestDist, as now in 0x7E93C0); 0x7C2800's F3 call binds 0x661640 as
    CThingFilter_IsNot.
  - 14 GetDef<T> twins share the 108-byte body with generic names (GetDef_<addr>(long, CDefPointer&)); three are now the
    PDB template (0x63F3A0 CEngineThemeDef, 0x66C340 CMaterialDef renamed; 0x74D220 fixed); the other 11 need their T
    from callers.
  - The 8 chunk-397 SGI rows over CThing** / CThingCompare_BestForTargeting2 are now STLport templates (FIXES); the
    repo has many more plain `__fastcall` SGI rows (e.g. 0x748C90 __adjust_heap in namespace _STL) -- same fix pattern
    (work/sgi/gen.py), not done wholesale.
  - Out-of-queue follow-ups: 0x580D21 "CTCHaste::PausedSetInsert" / 0x58119B "PausedSetErase" (chunk-314 descriptive
    labels) are the CTCHaste multiplier-map insert/erase (0x580D21 may be SetActionSpeedMultiplier(void*, float));
    0x801570 / 0x8015B0 / 0x4DBBED / 0x4DBC3A ("RbTree_Clear_*", landed under CTCCarryable::OnKill stubs) call the
    renamed EIdleStateGroup _M_erase pair and are clear()/destructor paths of the CTCIdleScheduler maps.

- Chunk 400-403 follow-ups:
  - Retail STL namespace is `std` (Fable.exe RTTI: .?AVlength_error@std@@, .?AVlogic_error@std@@,
    .?AU?$unary_function@VCThing@@_N@std@@, CPersistContext_TransferableVector<U?$pair@VCCharString@@J@std@@>; no
    `@_STL@@` string anywhere). Rows renamed this pass move `_STL::` sources to `namespace std` (e.g. 0x4110D0
    vector<char>::_M_fill_insert); STLport pair is a struct (`U?$pair@`).
  - CMeshDataBank Peek family: retail 0xA24250..0xA242C0 are eight byte-identical 16-byte bodies = Ego_r.pdb's eight
    one-line Peek accessors 0xB60500..0xB60570 in order (PeekPhysicsMeshInfo, PeekMeshInfo, PeekMesh, PeekStaticMesh,
    PeekRepeatedMesh, PeekParticleMesh, PeekAnimatedMesh, PeekAnimation). FINDINGS named only 0xA242C0; the other seven
    were all landed as "CTCInventoryAbilities::GetAbilityLevel" and are renamed on the same evidence (callers fit:
    CTCPhysicsBase::GetPhysicsMeshObject -> PeekPhysicsMeshInfo, CLocalDetailObjectCollectionType::Load ->
    PeekRepeatedMesh, CEngineInternalPrimitiveParticleMeshGroup::Update -> PeekParticleMesh). PeekMesh vs PeekMeshInfo
    and PeekStaticMesh vs PeekAnimatedMesh rest on the TU order alone (same types and similar callers).
  - CAnimationEntry getters: 0x5DC1D0 / 0x5DC1F0 / 0x5DC210 were all "CCombatActionBase::GetAnimationTransitionInTime";
    they are CAnimationEntry::GetTransitionTime / GetDelayTime / GetFlags (Ego_r TU order + the GetComponent key each
    calls: 1 / 2 / 0); 0x5DC230 next is GetTimeToBlendIn and the real CCombatActionBase::GetAnimationTransitionInTime
    is 0x8571A0 (calls GetComponent<TransitionInTime> itself).
  - NCameraModes::CBase attractors: 0x84E1A0 is AddAttractorGoTo (labelled SetGoToPosition in chunk 291 /
    callee_canon), 0x84E200 AddIncidentalAttractorGoTo, 0x84E260 AddAttractorLookAt (LABELS); the retail CBase list
    offsets follow from Ego_r CPhysicsState with 4-byte STLport lists.
  - 0x877C40 is the plain NCameraModes::CTools::IsLineOfSight overload (references, const, ELOSTest), not
    "IsLineOfSightFiltered"; 0x877840 is the thing-vector overload.
  - 0x834CC0 CAnimFilterAnyGroup::operator() renamed (was a gap-scan `T::Get(int)`); the 0x8350D0 FIX swaps its
    slot comment (slot 0 predicate, slot 2 scalar deleting dtor).
  - 0x6AFF40 (WorldUpdate's "GetTurnSpeed") is one of CTCPhysicsBase::GetTurnSpeedAngleXZ/XY/YZ (two fpatan); which
    one is not pinned, so the placeholder name stays.
  - Stale caller bindings after this block (reloc names only): callers of 0x84F4D0, 0x841E90, 0x877C40, 0x5DC1D0..
    0x5DC210, 0xA24250..0xA242C0 and 0x85AA90 still use the old landed names.

- Chunk 404-408 follow-ups:
  - Anonymous-namespace leaves (0x84F4D0 C3DFrustrum ctor, 0x8D43C0 CEventFilter_HitEventWithWeapon::operator()) pin
    the Leaf prefix (the ?A0x hash is per-TU) and keep the body emitted with a `g_keep... = &F;` pointer, the GOTCHAS
    convention; FINDINGS 406 notes such keepers / caller wrappers are not retail functions.
  - CAIStateGroupBase::Terminate 0x8FCF40 is declared inside the derived class in many landed AI sources (e.g. 0x93D030,
    0x93DDE0, 0x93E2D0, 0x93D720); only the queued 0x93C830 is fixed here; the rest are reloc-name debt.
  - 0x662410 / 0x662430 (renamed to CThingCreatureBase::IsInMeleeAsAttacker / IsInMeleeAsAttacked earlier in the pass)
    are still bound as CThing::IsCombatant in other AI sources; 0x93E5E0 and 0x96EE90 are fixed, others are reloc-name
    debt. 0x6621A0 ("IsInCombatMode") has no PDB-confirmed name yet (chunks 279/313 disagree), so it stays.
  - 0x7A2F90 is CThingCompare_NearestToPos::operator()<CThing> (bool), not CThingCompare_Nearest (whose operator()<T>
    returns T* in both PDBs, cf. the RETAIL BUG pointer-returning 0x91F740); this resolves the chunk-407 comment
    conflict by renaming instead of changing the return type (a T* return cannot emit `mov al,1`).
  - 0x49D870 is CWorld::GetFrame (static __fastcall; PDB run GetFrame / GetFrameAsString / SetFrame /
    StartCountdownTimer = retail 0x49D870 / 0x49D880 / 0x49D8A0 / 0x49D8B0). FINDINGS 406 calls it GFGetCurrentFrame
    and older label files CWorld::GetWorldFrame; neither is a PDB name. 806 call sites bind the old names (reloc only).
  - 0x6E9360 / 0x6E9390 are the two CCombatManager::GetCombatantInfo overloads (public const first, private second, the
    PDB order; external callers prove the first is the public one).
  - 0x9328D0 sort / 0x9322D0 __median now use `namespace std` while their landed TU siblings 0x9327F0 / 0x932660 /
    0x913E70 still spell `_STL::` (owner decision std vs _STL applies; bytes are unaffected).
  - CMemoryDataOutputStream.h is data-only: 0x996BC0 ReserveSize and 0x996B60 Clear use local class models; the landed
    0x996B60 carrier struct mangled the body as ?Clear@CMemoryDataOutputStream_Methods@@ (fixed in the rename row).
  - 0x95F7D0 pack(1) (chunk 407) is the same owner decision as 0x5D96A0 (genuine byte-packed classes vs pack-as-crutch).
  - The catalog row 0x8ECF60 spans two functions (second: an unreferenced find_if wrapper at 0x8ECFD0; chunk 405).

- Chunk 409-421 follow-ups:
  - Stale caller bindings after these renames (reloc names only): 941 landed callers still bind 0x99A380 by its old
    name (now CBaseObject::CBaseObject, protected `IAE`); the NUISystem::CChangingStateComponent::Initialise /
    AddChildDynamically callers (0x52C730 / 0x52CAB0); the CWideString::GetLength 0x99B220 callers.
  - 0x8EF180 (CTCTavernGameCoinBase slot 32, catalog vfunc_32) only tail-jumps to CTCTavernGame::NewRound 0x5E9A90
    (renamed here, `GameInSession` at retail +0x29); its own name is still open.
  - 0x85AD30 GetSpeedVariation binds a callee labelled CThingCreatureBase_GetCombatActionFrameTime (warn only).
  - 0xA43E30 is not a function start (UNRESOLVED; the manifest row should be 0x00A43DF0).
  - 0xAE71A0: the chunk-361 comments-only FIX is superseded by the chunk-426 RENAMES_chunk420plus row (same file, all
    edits); it was removed from FIXES.tsv (kept in work/superseded_FIXES_00ae71a0.tsv and fixes/00ae71a0.cpp -- re-add it
    if the coordinator drops the 420plus row in favour of sweep1's).
  - For sweep1 / the owner (chunks 420+): 0xAE5AF0's catalog `std::_Push_heap` is the STLport `__push_heap`; landed
    0xAE7760 spells `_STL::__partial_sort` and declares `_STL::__adjust_heap` / `sort_heap` while the renamed 0xAE5FB0 /
    0xAE71A0 use `std` (one family, two spellings); work/held/004cf810.cpp (chunk 426, `_Rb_tree<unsigned long>::_M_erase`,
    RELOCATION_MATCH) is parked for sweep1; the PKCTR red-black sources name one class per address
    (PKCTRRedblackMap00c1b250, PKCTRRedblackTree00c18ad0, ...).

## Queue (2): callees LABEL_FIXES / CANON (PDB/RTTI rows, < 0x00C08AE0)
Done (RENAMES unless noted): 0x9B2780 / 0x9B4D80 (vector<pair<unsigned long,CDefClassBase::CSubDefInfo> >
_M_allocate_and_copy / reserve), 0x99A380 CBaseObject::CBaseObject, 0xA62FB0 CVertexShader ctor, 0xAF2620
NParticleEngine::CPSCBase ctor, 0xB493C0 CEngineLightingManager::CManagerShadowedSpotLight ctor, 0x9FE380
CGraphicInfoFrame::CFrameData ctor, 0x9CBD10 CBankFile::GetEnumerationType (protected virtual, `ret 8` = one unsigned long),
0x8571C0 CCombatActionBase::GetFrameUpdateTime, 0x5E9A90 CTCTavernGame::NewRound, 0x7F53A0
CTCSoundAtmosphere::OnInitialActivate, 0x52C730 / 0x52CAB0 NUISystem::CChangingStateComponent::Initialise /
AddChildDynamically, 0x55AEB0 NUISystem::CClickable::OnHovered (protected virtual, slot 143), 0x656795
NPlayerGui::CDrawQuestInfo::CQuestInfoElement ctor (PDB `QAE@M@Z`: the argument is a float Scale, was modelled as long),
LABELS 0x4ABA70 (`_Rb_tree<CCharString,...>::_M_erase`), and the inventory family below.

Inventory family (method worth reusing): ghidra_out/naming_stragglers/ego_r.exe.signatures.tsv lists Ego_r.exe's
functions in address order with byte and instruction counts. A class TU's run in retail lines up 1:1 with its Ego_r run
(same order, same or near sizes), so each retail address gets the Ego_r symbol of the same position; the RTTI vtable slot
of every address then has to agree (it did everywhere below). This resolved the twin bodies BSim had collapsed:
- NInventory::CTCInventoryBase run 0x5BC638..0x5BC6AB = Ego_r 0x61F4AC..0x61F51F: 0x5BC638 ProcessInventoryInputLeft
  (was ProcessButtonAReleased), 0x5BC649 ProcessInventoryInputRight (was ProcessButtonAReleased), 0x5BC65C
  ProcessButtonAReleased (confirmed), Pressed/Held/Released for A/B/X/Y (slots 50-61: 0x5BC65A, 0x5BC65B, 0x5BC66D,
  0x5BC66E, 0x5BC68E, 0x5BC68F, 0x5BC690, 0x5BC691, 0x5BC692 renamed from vfunc_N / CTCInventoryClothing::vfunc_N /
  `$E2`; 0x5BC68D XPressed is not landed -> LABELS), D-pad releases (slots 62-65: 0x5BC69B / 0x5BC6A3 / 0x5BC6AB renamed
  from VTableCall_Offset_*, 0x5BC693 FIX: right name, wrong source function). Slot 46 = SetInventorySelectionState,
  +0x1C = InventorySelectionState (Ego_r.pdb), so the old "Selection / Close(bool)" models were retyped.
- 0x5BCE2F CWorldMap::DrawGetNoMaps -> NInventory::CTCInventoryBase::GetCategoryCount (13-byte Ego_r twin, 0x2C =
  retail CCategory, all callers are inventory screens).
- Slot 38 = ProcessInventoryInputUp, 39 = Down in every inventory vtable. CTCInventoryMap 0x5FA910 / 0x5FA950 and
  CTCInventoryExperience 0x56EC8C / 0x56ECCB had Up and Down SWAPPED (each override tail-jumps to the base handler of its
  own slot; Ego_r defines Up first; base Up = previous). Each pair must land together (the new source paths of one row
  are not the old paths of the other: the file names carry the address).
- CTCInventoryQuests 0x61A120 / 0x61A140 = ProcessInventoryInputLeft / Right (were ProcessButtonAReleased / vfunc_41,
  the source of 0x61A140 called itself Left); 0x61A0E0 / 0x61A100 = GetInitialCategoryRepeatDelay /
  GetContinuedCategoryRepeatDelay (were vfunc_42 / 43); base 0x5BD404 renamed from vfunc_42 (its source already had the
  name), base 0x5BD427 / 0x5BD44A / 0x5BD46D (not landed) -> LABELS.
- CTCInventoryExperience 0x56F497 / 0x56F4A1 / 0x56F4B0 = ProcessInventoryInputLeft / Right / ProcessButtonYReleased
  (were vfunc_40 / 41 / 61); 0x56F4AB ProcessExperienceActionReset (not landed) -> LABELS. CTCInventoryTrade 0x59B790 /
  0x59B795 = ProcessInventoryInputUp / Down (were _gapscan forwarders).
- Namespace only (method names confirmed by Ego_r + slot): 0x5BDD8C / 0x5BDDB8 (Up / Down), 0x5BD1EB OnInitialActivate,
  0x5BF9E3 InitialActivate, 0x5BC527 AddCloseInventoryEvent, 0x5BF3F1 GetItemOfTypeInInventory, 0x5BF91A
  ReturnAllConfiscatedItems, 0x5BFB01 ConfiscateItems (protected; Leaf pins the prefix: the PDB parameter is `const
  vector<CItem>&`, the local model is non-const).
- Follow-ups: 0x5BC9C8 sits in the OnKill slot (14) but Ego_r's run points at SetQuestPersistentFlagForItems (65 B) --
  left; CTCInventoryQuests slot 58 0x61B590 has no catalog row; the other screens' vtables can be named the same way.
- New VC7.1 lever (GOTCHAS candidate): VC7.1 tracks the registers a same-TU, non-inlined callee clobbers. Retail
  0x56F497 is `call 0x56F03D ; jmp base` with no `this` reload because ResetExperienceSpendValues only uses
  eax/edx/edi; a declaration-only callee costs `push esi ; mov esi,ecx ... mov ecx,esi ; pop esi` (16 v 10). The landed
  rows faked it with a no-argument free function; the renames carry a same-TU `#pragma auto_inline(off)` copy of
  0x56F03D instead (the landed 0x56F03D row is unchanged).

Rejected queue (2) rows (not applied, by reason):
- Destructor rows whose evidence is the vtable store: VC7.1 drops the derived entry vptr store, so a single base vtable
  store before a tail-jump is the INLINED base dtor and names nothing: 0x430300, 0x4E108C, 0x453E51 (CDefPointeeBase /
  CCombatAbilityBlockDefBase stores, then `jmp 0x9FC550`), 0x73D860 / 0x743000 (both would be CModeBase::~CModeBase),
  0x993B10 / 0xB9C660 (ambiguous), 0x4560A9 / 0x4567DB / 0x4570B7 / 0x4DA967 (no store at all). RTTI-store evidence is
  reliable for constructors (the last store is the class's own), not for destructors -- worth a caveat in the canon tool.
- Downgrades to `vfunc_N` where the landed name is evidenced: 0x7BEE10 (keeps GetShotDamage), and the inventory rows
  above (renamed to the Ego_r names instead).
- Unchanged names (canon = catalog): 0x401630, 0x44F81A, 0x47756C, 0x47832D, 0x499CE0, 0x4DA8B7, 0x52C780, 0x52E890,
  0x54DAE0, 0x55ACF0, 0x55B460, 0x56E2D0, 0x5BC6B3, 0x5BDD39, 0x64A450, 0x656B1B, 0x66E734, 0x693B30, 0x693DC0,
  0x69F0F0, 0x6A0D50, 0x717D40, 0x7F77C0, 0x850410, 0x881370, 0x8DBB30, 0x8EF180, 0x8F0920, 0x8F09C0, 0x956760,
  0x98DFD0, 0x994430, 0xAAC2E0, 0xB260B0, 0xB90270.
- 0x925780 `?A0x29922355::CCreatureAction_KrakenHighAttackLoop` ctor: the per-TU anonymous-namespace hash is not a
  reproducible name; the catalog convention (0x84F4D0 C3DFrustrum, 0x8D43C0) omits it, so the name is unchanged.
- 0x9E1DA0 GFAXToVX -> GFVXToAX (labels_chunk294): conflicts with the landed 0x9E1E10 GFVXToAX; Ego_r.pdb order says
  0x9E1DA0 = GFAXToVX (chunk 382 note above) -- the label row is the one to drop.

## Purity / rule notes for the owner
- FIXES 0x9C31E0 keeps the landed source's two `volatile` members (Running +0x7B, Paused +0x7C). The landed row and its
  landed twin 0x9C3250 already use them; without them VC7.1 keeps the incremented count in a register where retail
  re-reads `[esi+7Ch]` (probe in work/probe_9c31e0.cpp). The fix itself (global -> static CFileInstaller::DirtyDiscError,
  Leaf pinned) does not add any. The brief says "no volatile": accept as pre-existing or move the row to UNRESOLVED.
- FIXES 0xAAC580: the member was renamed m_emitterNumber -> m_number because crawl/purity.py flags any `_emit`
  substring as an asm bake.
- STL namespace: the brief says `_STL`; this pass writes `namespace std` because retail itself mangles `std` (RTTI
  .?AVlength_error@std@@ and friends; no `@_STL@@` anywhere; UNRESOLVED 0x81C270). sweep1's brief also says `_STL`, so
  the two hand-overs may disagree on spelling -- owner decision (bytes are unaffected; mangled names are).

## Header debt (list only; needs header regeneration, not per-row edits)
CParentDefClassBase.h (0x3D vs 0x39); CParticleProgram.h; CObserver.h (0x18 vs 0x14); CBulletTimeManager /
CTCGraphicAppearance maps (16 vs 12); CTCPhysicsBase has no virtuals; CTCCombat; CTCInventoryAbilities; CThingPhysical;
CEnvironment; LHHttpHeaderStatus (0x8C vs 0x48); CScriptConversation; CThingCreatureBase; CTCTrapBase/CTCWillLightning;
AI state groups; CTCHeroStats; CCategory; CTCInventoryClothing; CThingGameObject/CCompressorZlib/CHoverable sizes;
CEngineGraphic 14 vs retail 12 (CGlobalDef offsets shift); the handle pointee has three names; CTCBase.h has no
constructor/virtuals (flattened `__vftable`), so every CTCBase-derived override in this pass is a local class model;
CTCTurncoatSpell.h (PDB offsets past the will-button timer, retail 4 lower); CTCTargetingPlayer.h (0x8025D0 / 0x802D10
use local models, chunk 398); CThing / CWorld locals (0x662410 family); LHHttp2.h, LHHttpHeaderStatus.h and
CCreatureAction_PlayAnimationWithLookTurning.h carry Ego_r layouts (0x1C strings, 0xC0 class) vs TLC 12-byte strings and
0xB4 class (chunk 401); CMeshDataBank.h (MeshesAndAnimations retail +0x380, PDB +0x3F4); CTCTargeted.h is data-only (no
methods possible); CThingPhysical.h puts Scale at +0xB4 (retail Scale +0x9C, Health +0xB4; chunk 403);
CMemoryDataOutputStream.h and CWorld.h are data-only (no method declarations; 0x996B60/0x996BC0/0x49D8B0 local models);
CStringParser.h (PDB 0x30 vs retail 0x28; chunks 409/411); CTCTavernGame.h (GameInSession +0x31 in the header, retail
+0x29; 0x5E9A90); CStreamingFontBank.h and CDefinitionManager.h are data-only (UNRESOLVED 0x9A0EC0 / 0x9AD580); no
header exists for CTCInventoryBase / its screens (every inventory row above is a local model; retail CTCInventoryBase is
PDB minus 4 per STLport vector), CQuestInfoElement (retail 0x1C vs PDB 32), CClickable / CHoverable (retail 0x194 /
0x16C vs PDB +0x30).

## Progress
Queue (1): chunks 350-421 done (FINDINGS lines 473-556); chunks 420 and 422-426 were verified before the scope split and
are handed over separately in the `_chunk420plus` files. Queue (2): done (see above). Final full re-grade: see
work/regrade_all.log.
