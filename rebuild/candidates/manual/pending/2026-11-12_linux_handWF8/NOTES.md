## 0x0091B3A0


## 0x0091B3A0 CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition -- RELOCATION_MATCH (2 grades)
- What made it match: the port onto the shared headers with the PDB signatures (GetTC(TCI_PERCEIVE_THING) uninitialised component
  local, inline IsAwareOfThing / IsThingLineOfSight / GetCreature, direct LastPerceivedPos reads, GetPerceivedThing() with no argument,
  EAIActionReturn + bool& for the move) left one residue (frame 4 bytes larger, `false` pushed before the action ctor); making the
  CCreatureAction_PlayAnimation a NAMED local (`action(...)` then `AIAction_PerformActionAndWait(action, false)`) instead of a
  temporary argument fixed both.
- Needs: overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_SEEKPERCEIVEDTHING, FABLE_RETAIL_CTCPERCEIVETHING_SEEKPERCEIVEDTHING_FRIEND,
  FABLE_RETAIL_CTHINGPHYSICAL_ISINRANGE (hdr/README.md); existing opt-in FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR.
  No flags, no pragma. Strict-header regrade passes. Test 0091b3a0_test.cpp PASS (vt.py).
- Names: no invented accessor. Locals `perceive`, `pos`, `nav_route_calculated`,
  `anim`, `action`, `ret` are descriptive. Old draft spellings proved wrong by the dossier (not landed): PCreature -> Creature,
  Alertness -> AwarenessTimer, Seen -> LineOfSight, GetPosition -> GetPos, GetPerceivedThing takes no argument.
- Repair (verifier finding 1): the invented inline CTCPerceiveThing::GetLastPerceivedPos() is withdrawn. FableWin.pdb emits the
  trivial inline accessors this function uses out of line (IsThingLineOfSight 0x01DA32C0, IsAwareOfThing 0x01F43D20,
  CAIStateGroupBase::GetCreature 0x0288D7B0, CAIBrain::GetCreature 0x0288D7D0) and has its own RunToPerceivedThingPosition
  (0x02BD3B90, 1127 bytes), yet no GetLastPerceivedPos symbol, so no such function existed; GetAwarenessPosition (FableWin 0x02B80DF0,
  236 bytes) is not a trivial return. Both reads are now `perceive->LastPerceivedPos`; the member is private in the retail branch, so
  the overlay grants `friend class CAIStateGroup_SeekPerceivedThing;` (opt-in FABLE_RETAIL_CTCPERCEIVETHING_SEEKPERCEIVEDTHING_FRIEND,
  descriptive: the PDBs carry no friend records; access-only, no layout/code change). Still RELOCATION_MATCH (also strict headers);
  test updated, vt.py PASS. The pre-existing SMELL-branch declaration of GetLastPerceivedPos in the repo header is untouched (not this
  row's opt-in) but is equally unsupported by the PDBs.

## 0x00B81F30


## 0x00B81F30 CEngineClothSimulationStaticMesh::RunSimulation -- RELOCATION_MATCH (author, 19 grades incl. strict regrade)
- What made it match: (1) the mesh pointer is a REAL shared `CCountedPointer<const C3DMesh2>` local with plain early `return`s (no manual
  Release/Destroy as the sibling 0x00B82100 has): VC7.1 by itself expands the destructor at the GetMesh-failure return and at the end
  (cloned head + `jmp 0xB820DA`) and calls the COMDAT 0x00A36EC0 at the empty-primitive return; (2) ClothPrimitives as the shared
  `CArray<CCountedPointer<C3DClothPrimitive> >` with `.empty()` / `.size()` / `operator[]` (raw start/finish pointers gave the wrong
  register roles: zero in ebp, CSE'd count); (3) CParticleContext is a POLYMORPHIC base at +0 (an empty non-virtual base sits at +4 and
  adds `add ecx,4`); (4) step loop names the cloth primitive BEFORE the simulation (sim-first swaps ebp/frame roles); set-up loop names
  the simulation before the program; (5) LOD pick is the FableWin inline pair HasLOD / GetBestCurrentDetailLevel (unrotated loop).
- Headers / flags: no flags, no pragmas. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL,
  FABLE_RETAIL_C3DPRIMITIVE2_CLOTHPRIMITIVES, FABLE_RETAIL_CPARTICLEPROGRAM_UNSKINNED_SIMULATION. Existing opt-ins used:
  STLP_MAP, STLP_VECTOR_RESIZE_ONE_DECL (resize(n) 0x00B840A0 out of line), CCOUNTEDPOINTER_RESET_CAMERA_DECL (Reset 0x00B82A50),
  CMESHDATABANKENTRY, CMESHDATABANK_BASES, CBANKFILE_ENTRYQUERY, CGRAPHICDATABANK_BANKFILE, CMESHDATABANK_TEXTUREBANK,
  C3DMESHINFO_GETMESH, C3DMESHINFO_BOUNDINGBOX, CPARTICLEPROGRAM. Strict-headers regrade: RELOCATION_MATCH.
- Local models (no engine/ header exists): CParticleContext, CEngineSimulationBase, CEngineClothSimulationMeshBase,
  CEngineClothSimulationStaticMesh, C3DClothPrimitive. Vtable slot names are FableWin.pdb names (slots 1..5 via the overriders in vtables
  0x012A38FC / 0x012A3924); which of them CParticleContext declares pure is inferred from its 6-slot vtable 0x012A38E0.
  `_transforms` (+0x18..+0xA8 of the mesh base) is a descriptive pad name, not a PDB member.
  `g_ParticleMemoryPool` (0x013D2D70, CMovableResourceMemoryPool*) is a made-up DESCRIPTIVE name: no PDB extract or ghidra_out TSV
  names it; reused verbatim from landed rows 0x00B82100 / 0x00AC3BD0 / 0x00AC3E00 so one address keeps one name (it is the pool
  passed to CParticleProgram::SetupSimulation).
- Repair (verifier): MeshInfo respelled `C3DMeshInfo*` (non-const, Ego_r.pdb CEngineClothSimulationMeshBase +172 and sibling
  0x00B82100); regrade RELOCATION_MATCH, strict-headers regrade RELOCATION_MATCH.
- Names proved wrong (NAME_FIXES.tsv): 0x00ADF5B0 landed as `CParticleSimulationUser_00adf5b0::SavePositions` is
  `?SaveStateToInterpolateFrom@CParticleProgram@@QAEXAAVCParticleSimulation@@@Z` (Ego_r.pdb 00beb250 + FableWin.pdb 01816a64; the only
  CParticleProgram method with that signature; called on cloth->ParticleProgram right before SimulationStep here and in 0x00B82100).
  Sibling 0x00B82100's local `IsLODPresent` / `GetLoadedLOD` are FableWin.pdb `?HasLOD@C3DMeshInfo@@QBE_NK@Z` /
  `?GetBestCurrentDetailLevel@C3DMeshInfo@@QBEKXZ` (inline, returns unsigned long); its manual CMeshPointer Release()/Destroy() model
  can likely be replaced by the real CCountedPointer local as here. 0x00B840A0 (landed label `_Umove<EWeaponClass>`) is
  `?resize@?$vector@V?$CCountedPointer@VCParticleSimulation@@@@...@std@@QAEXI@Z` (dossier: Ego_r 00bf9f40, FableWin 01831be3).
- Ideas tried before the match: sibling-style local models (485v455), nested if(GetMesh) with shared release (421..437), chained
  SetupSimulation call (469), begin()==end() / size()==0 empty tests (455 with 6 / 458), sim-first step loop (5 hunks).

## 0x008C61C0


## 0x008C61C0 CCreatureAction_TrollWhackGroundBase::WhackGround -- RELOCATION_MATCH (wf8 author, 6 real grades + 4 compile-fix grades)
- What made it match: (1) port onto the shared headers with the PDB callee spellings from the dossier; (2) the normalise is NOT a
  helper: written out in the row as a named difference `movement`, `float magnitude = movement.GetAccurateMagnitude()`, and
  `C3DVector direction = magnitude > DELTA_NOTIONAL_ZERO ? movement * (1.0f / magnitude) : C3DVector(0,0,0)` (gives retail's three
  vector slots and the copy through eax; a free inline helper was called out of line, 368v457); (3) the GetSubDef<T> template body
  visible in the TU (engine/CDefClassBase_GetSubDef.h, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE) -- that alone moved `this` to
  edi and the def to ebx (465v457 -> 457v457 with one hunk left); (4) GetEffectName on slot 82 needs CCombatActionBase's eleven own
  virtuals declared.
- Headers / flags: no flags, no pragmas. Overlay opt-ins FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS and
  FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS (hdr/README.md). Strict-headers regrade: RELOCATION_MATCH. Test 008c61c0_test.cpp PASS (vt.py).
- Local declarations only where no shared header exists: NCombatActionHelpers::CreateEffect (0x008F7F50), CTCDRumble::Create (0x007F1B70), both PDB spellings.
- Made-up names: none for functions; locals only (movement, magnitude, direction, names, pos, effectName, def).
- Dossier corrections: section 5 "GetSubDef<CSpecialEffectsDef> other TU -> declare only" is wrong for this row (the template body must
  be visible; it is a COMDAT every using TU instantiates). The landed 0x008C6390 row is a free-function forwarder
  (`CCreatureAction_TrollWhackGroundBase_PerformAction`); with this class opt-in it can be respelled as the virtual
  `PerformAction() { WhackGround(); }` (not done here; one row per agent).
- Repair (verifier findings, applied): (1) the `// Identity:` comment now lists all four xref call sites of 0x008C61C0 (jmp at
  0x008C6390; calls at 0x008C63A3 / 0x008C63C4 / 0x008C6438 in the AngryGroundPound / Tunnel / ProjectileAttack PerformActions,
  vtable slot 3 of 0x0128E23C / 0x0128E504 / 0x0128E3AC) -- each derived PerformAction begins with the inlined base PerformAction.
  (2) The 0x008C6390 respelling is handed back in NAME_FIXES.tsv with evidence: `?PerformAction@CCreatureAction_TrollWhackGroundBase@@UAEXXZ`
  in ghidra_out/egor_pdb_names.tsv (line 24135) and ghidra_out/fablewin_pdb_names.tsv (line 92761, next to ?WhackGround@...@@AAEXXZ),
  and rebuild/manifest/rtti-vtables.tsv: vtable 0x0128E0EC slot 3 = 0x008C6390. Regrade after repair: RELOCATION_MATCH (also strict
  headers); test PASS (vt.py).

## 0x006D0630


## 0x006D0630 CTCWife::ReceivedNiceGiftIsOpinionReactionHappening -- RELOCATION_MATCH (wf8 author, first attempt)
Source: $W/006d0630.cpp (also copied over differ/006d0630.cpp), test $W/006d0630_test.cpp (vt.py PASS), strict-headers regrade
RELOCATION_MATCH, payload.json / oracle.tsv written by addmatch.py (module CTCWife, name ReceivedNiceGiftIsOpinionReactionHappening).
Flags: none (base /O2).

What made it match (in the order found):
1. Port onto the shared headers (CTCWife / CThing / CWorld / CGameDefinitionManager / CGiftDef / CHeroMarriageDef /
   CTCHeroOpinionDeedLog / EOpinionDeedType, std::map from stlport/fable_map.h); every callee by its PDB name (see DOSSIER 3).
2. The tree's member template find<long> must be VISIBLE BUT CALLED (0x005937E1): new opt-in
   FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_OUTOFLINE (noinline body). Declaration-only is worse (477 v 459).
3. FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (GetDef<T> body visible, noinline): without it the object def's handle
   releases as `mov esi,[eax+4] ; dec esi ; mov [eax],esi` and the epilogue pops move behind the releases.
4. `long frame = CWorld::GetFrame();` as a local before the repeat-interval test (retail calls GetFrame first, then reads the
   def's interval and ConstantFPS from memory in the imul); this also moved the def index into ebp and the iterator into ebx.
5. The block layout (the old draft's residue, 16 grades on it here): FrameLastReceivedNiceGift is stored by ONE statement
   after the insert / update if-else, `if (result) FrameLastReceivedNiceGift = CWorld::GetFrame();`. VC7.1 duplicates it into
   the two success paths (retail 0x006D07B0 / 0x006D07E1 are those copies). With that statement sitting between the
   already-got arm and the post, the post block has no fall-through predecessor, so c2 pulls `result = false` + post + epilogue
   up behind the refused-ring arm and leaves the accepted-ring arm and the whole gift-map arm behind the epilogue, jumping
   back. The store written once in each success path gives the same instructions with the post at the end (447 v 459).
   The deed is a local starting at OPINION_DEED_TYPE_NONE with the post behind `if (deed != NONE)` (threaded away); the inner
   test is `if (!IsInLoveWithHero()) { refused } else { ring }`.

Headers / opt-ins it needs (overlay $W/hdr, see hdr/README.md):
- engine/CTCWife.h: FABLE_RETAIL_CTCWIFE_NICEGIFT (the declaration). The row also defines FABLE_RETAIL_CTCWIFE_MARRY only to
  get the CHeroMarriageDef class body out of engine/CHeroMarriageDef.h (that header is gated on it).
- stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_OUTOFLINE (used). The tried ..._DECL branch was dropped in repair.
- Existing opt-ins used: FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY,
  FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, FABLE_RETAIL_CTCHEROOPINIONDEEDLOG, FABLE_RETAIL_CTHING and the creature /
  player / world set of the landed CTCWife::Murdered row.
- A callee's real body in the row file: CTCWife::IsInLoveWithHero 0x006CF900 (landed), defined `inline` before the function
  (`return IsCourtingHero() && OpinionAttitude == OPINION_ATTITUDE_LOVE;`), expanded here as in retail. No stand-in bodies.

Names: no made-up function or member names. Enumerators: OPINION_ATTITUDE_LOVE = 12 as the landed CTCWife rows spell it (the
enum is only forward-declared in CTCWife.h); EOpinionPostDeedType is defined empty and the value 1 passed by number (as the
landed OnGreet row does). Locals (object_def, gift_def, is_wedding_ring, result, deed, frame, hero, deedLog) are descriptive.
Old parked draft's wrong names (never landed, so no NAME_FIXES row): GetDefWorld, GetObjectDef, GetGiftDef, IsNiceGift,
GetWorldFrame, GetCurrentPlayer, GetCharacterThing, GetPosition, g_FramesPerSecond, SecondsBetweenNiceGiftReactions.
For the coordinator: the landed 0x006CF900 row models IsInLoveWithHero as `int` on a local CTCWife (PDB: `bool ... const`,
?IsInLoveWithHero@CTCWife@@QBE_NXZ) and the landed 0x006CEC80 row is a free `CTCWife_IsMarriedToThePlayer(CTCWife*)`; both
are candidates for a port onto engine/CTCWife.h (not touched here).

Ideas tried that did NOT change the layout residue (do not repeat): do { } while (0) + break; early returns per arm (each
return duplicates the two handle releases: 579..863 bytes); goto forms in retail's block order (c2 re-sorts them); inverted
outer condition (`!ring || married`); deed sentinel v bool post flag; a single `result = false` behind `if (deed != RING)`;
deed default assigned before the love test; ternary deed; the love test as a bool local / `== false` / `? true : false`;
inline IsMarriedToThePlayer(); the landed if/return body of IsInLoveWithHero; three literal PostDeed copies (no cross-jump);
GETTC_EXPANDED / two-argument GetTC; ParentThing as a local or the raw member; #pragma optimize("s").

## Repair (verifier findings applied)
1. hdr/README.md now states the one rewritten repo line in stlport/fable_map.h (line 201 `#ifdef FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE`
   -> `#elif defined(...)` behind the new OUTOFLINE branch; default and FIND_MEMBER_TEMPLATE paths unchanged).
2. Dropped the unused FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL branch from the overlay (diff to repo is now only the OUTOFLINE branch + that #elif).
Regrade: RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1); vt.py PASS; addmatch.py rerun. Source unchanged.

## 0x0083D990


## 0x0083D990 CThingPhysicalSwitch::ConstructFromLevelScript -- RELOCATION_MATCH (wf8 author, 4 grades)
- What made it match: port onto the shared headers in the TrackNode 0x00837E40 idiom (`const char*` param, `CStringParser parser(script)` with header default args, `break` on EndThing, `TriggerableObjectScriptName = (value == "NULL") ? CCharString("") : value;`) PLUS the row-local template bodies of CBasicString<T>::Compare / operator==(const T*) (landed 0x00837E40 / 0x009BB110 precedent). Without those bodies the key lands in the dead `script` arg slot, the value in the frame, and the key's PStringData is reloaded instead of cached in esi (464v460).
- Headers / flags: no flags, no pragmas. Overlay hdr/engine/CThingPhysicalSwitch.h adds FABLE_RETAIL_CTHINGPHYSICALSWITCH_LEVELSCRIPT (virtual override declaration only). Strict-header regrade: RELOCATION_MATCH. Test 0083d990_test.cpp PASS (vt.py).
- Ideas graded and rejected: ternary `? "" : value` (427v460), if/else with two assignments (473v460).
- Names: draft spellings wrong per dossier (FindKeyword -> SkipPastString 0x009B9BA0; CBase -> CBaseClass; members Triggered / TriggerableObjectScriptName). No made-up names.

## 0x0072EB40


## 0x0072EB40 CTCActionUseBuyHouse::ReceivedConfirmation -- RELOCATION_MATCH (wf8 author, 4 grades, first grade matched)
- What made it match: the port onto the shared headers alone. The old 467v463 residue (`shr ; and` vs `test dword ptr [esi+28h],80000h`)
  was the local bool HasTC; engine/CThing.h's __forceinline HasTC (FABLE_RETAIL_CTHING_HASTC_FORCEINLINE) + GetTC (GETTC_EXPANDED) give
  retail's in-place test. Signatures fixed from the PDBs: reference parameter, private; GetPrice() / GetPriceForNextDressLevel() const
  with no argument; AddMoney(long, bool) -- the `push 1` belongs to AddMoney; this->GetPlayerManager().GetMainPlayer().GetPControlledCreature().
- Needs: no flags. Overlay opt-ins (hdr/README.md): FABLE_RETAIL_CTCBUYABLEHOUSE_RECEIVEDCONFIRMATION (engine/CTCBuyableHouse.h, declarations),
  FABLE_RETAIL_CTCHEROSTATS_PROPERTYOWNED (engine/CTCHeroStats.h, AddMoney + NumberOfShopsOwned +0xE0 / NumberOfHousesOwned +0xE4).
  Base class is CTCActionUseBase (RTTI rtti_bases.tsv), from engine/CTCActionUseBase.h; CPlayer / CThingPlayerCreature from the shared headers.
- Made-up name: enumerator HOUSE_QUESTION_DRESS (= 3) of EHouseQuestion -- descriptive, no enumerator names in either PDB (siblings name 0..2 the same way).
- Landed spelling proved wrong: the caller GlobalUpdate 0x0072EE30's comment spells this callee with a pointer parameter (CTCBuyableHouse*);
  both PDBs say `AAEXAAVCTCBuyableHouse@@@Z` (reference, private). Same bytes; comment/declaration respell only (NAME_FIXES.tsv).

## 0x00BA4A40

## 0x00BA4A40 CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures -- RELOCATION_MATCH (wf8 author, 12 grades incl. 2 SRC_FAIL)
- What made it match: (1) the port onto the shared headers (mesh.MeshInfo->PeekTextureBank(), GetCurrentFrame, C3DTextureStages::operator==,
  PeekGraphicInfoFrame(idx,0)->GetTextureType(), first_block reference + GetStaticBlock(c) loop, Set accessors); (2) FABLE_FORCEINLINE on
  CGraphicDataBank::PeekGraphicInfoFrame in the row file (plain `inline` leaves two calls, 267 bytes, because it is used twice; `// Inline-budget:` line);
  (3) `texture_type` (with its default) declared BEFORE `graphic_index=material->Get...MapBankIndex()` -- the other order moves the material to eax and
  the size() result to ecx (467 bytes, an extra `mov edx,eax`).
- Headers / flags: no compiler flag, no pragma. Overlay opt-ins in hdr/README.md (4 new macros, 5 headers). Passes LANDVERIFY_STRICT_HEADERS=1.
  Define set = AddStaticMesh 0x00BA0FC0's (retail class branch of CEngineInternalPrimitiveStaticMesh) + SetupStaticBlocksNoMorph 0x00BA48F0's +
  FABLE_RETAIL_CMESHDATABANKENTRY / _CMESHDATABANK_BASES / _CBANKFILE_ENTRYQUERY / _CMESHDATABANK_TEXTUREBANK / _C3DMESHINFO_GETMESH; the
  C3DMeshInfo GETMESH path compiles together with the TEXTURE_HELPERS branch of CGraphicDataBank.
- The PeekGraphic / PeekGraphicInfoFrame inline bodies are the ones the landed sibling 0x00BA48F0 defines in its row file (not in the header).
- Tried and rejected: plain inline (v1, 267v463); forcing PeekGraphic too (no change); FABLE_RETAIL_CARRAY begin()-index (no change); ternary inside the
  GetBlankTexture argument (483); `long graphic_index` (no change).
- Names: no made-up names (parameter / local names are descriptive; all functions and members are PDB spellings). Landed stub of 0x009FD150 spells the
  callee non-const with a non-PDB signature (see NAME_FIXES.tsv); this row uses the header's PDB form `const CTexture* GetBlankTexture(ETextureType) const`.
- Repair (verifier findings): source now cites landed row 0x00BA48F0 for the copied PeekGraphic / PeekGraphicInfoFrame bodies; hdr/README.md
  explains the split one-line C3DTextureStages class (default path token-identical, `};` on its own line, static-assert counter _6 -> _12).
  Regraded RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1); test passes.

## 0x007BBE34


## 0x007BBE34 NThingShot::CTCShotDefinitionSpreadShotProjectile::InitMovement -- RELOCATION_MATCH (wf8 author, 81 grades)
Source: final.cpp (= differ/007bbe34.cpp, payload.json); test: test.cpp (vt.py PASS); strict-header regrade RELOCATION_MATCH.
What made it match (in the order the residues fell):
1. Port onto the shared headers with the dossier's identities (RotateXY, GetPos, GetFacingVector, ResetRHSet, EnableGravity,
   AccurateNormalise, GetWorldMap()/GetWorld() references, NThingShot::RandomiseShotDirection, GetShotAccuracyPercentage,
   CMainGameComponent::GetConstantFPS).
2. Target pointer in edi + separate by-reference copy at ebp-4: `CThing* target = const_cast<CThing*>(PTargetThing.Get());
   IS_THING_ALIVE<const CThing*>((const CThing*)target)` (the cast makes the argument a temporary; a plain `const CThing* target`
   passed directly is addressed itself and reloaded from ebp-4 twice).
3. Frame 0x5C (CRightHandedSet temporary NOT overlaying the C3DVector call temporary): the CRightHandedSet constructor's body must be
   VISIBLE while still called out of line -> FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE instead of _CTOR (declaration only gave 0x50).
   This is the "frame-sharing 0x44 v 0x5c" blocker of the twin 0x007BC004 (NymphFun) and the "RandomiseShotDirection temp slot" of
   0x007BD845 (DrainLife) too -- try the same opt-in there.
4. 1.5 scale held on the x87 stack (`fld [K] ; fld [eax+8] ; fmul st(1) ... fst Z ; fstp st(0) ; fstp st(0)`): the DEFAULT
   C3DVector::operator*(float) -- do NOT define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN (the Predictive sibling's opt-in); with the
   grouped X product VC7.1 uses `fmul [K]` three times.
5. `push eax ; mov ecx,ebx` of ApplyAcceleration after the velocity products: FABLE_RETAIL_C3DVECTOR_CROSS_XY_GROUP (two no-code PAREN
   tuples in operator^ move the scheduling-region cut; CROSS_X_GROUP (one) and a named `speed` local did not).
Headers / flags: no compiler flag; pragmas optimize("y", off) + optimize("s", on). Overlay: NEW
`hdr/engine/NThingShot_CTCShotDefinitionSpreadShotProjectile.h` (no repo header existed; land it with the row). Macros used:
CTCBASE, CINTELLIGENTPOINTER, CTHING, C3DVECTOR, C3DVECTOR_CROSS_XY_GROUP, CRIGHTHANDEDSET, CRIGHTHANDEDSET_CTOR_INLINE, CTCPHYSICSBASE,
CTCPHYSICSBASE_APPLYACCELERATION, CTCSHOTDEFINITIONBASE_ONHITBYMELEE / _MOVEMENT / _SHOTACCESSORS, CWORLDMAP, CWORLDMAP_HEIGHTZ.
Made-up (descriptive) names, found in no PDB / string / FSE source: `SPREAD_SHOT_ANGLE_STEP` (0.333f, .rdata 0x0126DF24),
`SPREAD_SHOT_TARGET_OFFSET` (1.5f, .rdata 0x0126DF28) -- this TU's own non-COMDAT constants (one reader each, placed between the
CTCDrunkenness vtable and the next COMDAT), modelled as file `static const float` in that order; `s_SpreadShotAngle` (.bss 0x013BAE7C,
zero-initialised file static float). Local names (target, aim, dir, side, velocity, physics) are descriptive.
No landed name proved wrong (NAME_FIXES.tsv not needed). No stand-in bodies in the row file.
Ideas that did NOT matter (all graded): extern / class-static / function-local / selectany / array / struct spellings of the two
constants, /Oa, early return v if-block, the real RandomiseShotDirection body in the TU, named or reference-bound facing /
randomised / rhset locals, operator*= forms, paren tuples round the facing expression or the constant, C3DVector ctor spellings.

## 0x00A128E0


## 0x00A128E0 CPhysicsMeshObject::IsInContainmentVolume -- RELOCATION_MATCH (2 graded rounds, 6 variants + strict regrade)
- What made it match: port onto the shared headers (named local `CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix)`, two-argument
  `TransformPoint(pos, local)` in the SOURCE_DEST_SUMS form, `const CCharString&` first argument), then `float scaled_radius = radius * OO_Scale;`
  as a separate local declared AFTER the transform and BEFORE the named counted copy `CIVCountedPointer<const CPhysicsMesh> mesh(Mesh);`.
  VC7.1 homes that local in the dead `pos` argument slot ([esp+50h]). The dossier's "open risk" (operand order of the cofactor products)
  needed no header math change: it follows from the counted copy / late radius by itself.
- Residues of the other spellings: `radius *= OO_Scale` -> 464v464, radius homed at [esp+54h] instead of [esp+50h] (2 hunks);
  `radius * OO_Scale` inline at the call with GetMesh() temporary -> 459v464 (6 hunks); named mesh declared first -> 459v464 (19 hunks).
- Headers / flags: no flags, no pragmas. Opt-ins FABLE_RETAIL_DEF_CLASS_BASE, _CPHYSICSMESHOBJECT, _CPHYSICSMESHOBJECT_MATRIX, _CIVCOUNTEDPOINTER,
  _CMATRIX3X4, _CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS (all existing) plus two NEW declaration-only opt-ins in the overlay ($W/hdr, see hdr/README.md):
  FABLE_RETAIL_CPHYSICSMESH_CONTAINMENTVOLUME (engine/CPhysicsMesh.h: IsInContainmentVolume 0x00A6CA20, PeekRoughProportionOfSphereContained 0x00A6CA80)
  and FABLE_RETAIL_CPHYSICSMESHOBJECT_CONTAINMENTVOLUME (engine/CPhysicsMeshObject.h: IsInContainmentVolume 0x00A128E0,
  PeekRoughProportionOfSphereContainedInVolume 0x00A12AB0). Strict-headers regrade: RELOCATION_MATCH. Test: vt.py PASS.
- Names: all from Ego_r.pdb + FableWin.pdb; parameter and local names descriptive (no PDB locals). No landed name proved wrong.
- For the sibling 0x00A12AB0 (PeekRoughProportionOfSphereContainedInVolume, float result, calls 0x00A6CA80): the same body with
  `return mesh->PeekRoughProportionOfSphereContained(name, local, scaled_radius);` should match; both declarations are already in the overlay.

## 0x00B903D0

## 0x00B903D0 CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive -- RELOCATION_MATCH (3 grades + strict regrade, first variant matched)
- What made it match: the port onto the shared headers alone. The old ebx/ebp "register tie" was a model error: the made-up
  pointer-taking `DrawIndexed` helper with `long type` replaced by the PDB's seven-argument `DrawBuffersIndexed(CVertexBufferWin32&, K, K,
  CIndexBuffer&, K, K, EPrimitiveType)` = `SelectVertexBuffer` + six-argument overload, called with `*primitive->VertexBuffer`,
  `*primitive->IndexBuffer` (scoped_ptr), `block->GetStartIndex()`, `GetPrimitiveCount()`, `GetPrimitiveType()` passed directly; block count
  and vertex count through GetStaticBlockCount()/GetVertexCount() locals before the loop; IsFixedFunctionVertexMode() accessor.
  Plain `inline` for the 7-arg overload is enough here (FABLE_FORCEINLINE also matches; named start/count locals not needed).
- Headers / flags: overlay hdr/ (three opt-ins, see hdr/README.md); no extra compiler flags, no pragma. Source defines
  FABLE_RETAIL_SCOPED_PTR(+_RESET), _CINDEXBUFFER, _CVERTEXBUFFERWIN32, _CRENDERMANAGERCORE, _CVERTEXSHADER,
  _CSHADERRENDERMANAGER_INLINE_BLOCKDRAW, _INLINE_BUFFERDRAW, _CBASEBLOCK_ACCESSORS, _C3DPRIMITIVE2_INDEXBUFFER, _VERTEXBUFFER,
  _ANIMATEDBLOCKS (for GetVertexCount), _STATICBLOCKS_TEXTURE_HELPERS, _CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_RENDERUNTEXTURED.
- Names: no made-up names. Globals: NEngineInternal::GPShaderRenderManager 0x01436E14 (labels_landing_2026-11-12ef.tsv; older rows
  spell it g_pShaderRenderManager -- one-address-one-name conflict for the coordinator), g_pRenderManagerCore 0x01436E18 (labels spelling).
- Landed spelling to fix: DrawVisibilityTestBox 0x00B906C0 models this callee locally with void* args; the PDB signature is
  `void RenderUntexturedStaticMeshPrimitive(C3DPrimitive2*, const CMatrix3x4&)` (private).

