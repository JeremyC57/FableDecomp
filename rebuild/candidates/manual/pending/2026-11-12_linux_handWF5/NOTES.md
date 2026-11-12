## 0x009ADAE0


## 0x009ADAE0 CDefinitionManager::InstantiateSubDef -- RELOCATION_MATCH (wf5, 4 grade calls, 8 grades)
- What made it match: port onto the shared headers with the InstantiateDef 0x009ADC90 idiom (long indices, adopting
  CDefPointer(T*), record built in its own statement with a default CDefString() instantiation name, named `parent` handle from the
  inline non-const GetPDefFromGlobalIndex, `return pdef;`, miss path = discarded GetPDefFromGlobalIndex(parent_index) + null handle)
  left one residue: `and byte ptr [esi+24h],0FDh` v retail `mov al,[esi+24h] ; and al,0FDh ; mov [esi+24h],al`. A bool local /
  const local / `= 0` did not change it; the inline setter `CDefClassBase::SetAsTemplate(bool is_template) { Template = is_template; }`
  (FableWin.pdb ?SetAsTemplate@CDefClassBase@@QAEX_N@Z 0x03060C40 / thunk 0x01811208) called with `false` did.
- Headers / flags: Flags none. Overlay opt-ins (hdr/README.md): engine/CDefinitionManager.h
  FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATESUBDEF (InstantiateSubDef decl, LoadDef(unsigned long) const decl, inline body of the
  protected non-const GetPDefFromGlobalIndex(long)); engine/CDefClassBase.h FABLE_RETAIL_CDEFCLASSBASE_SETASTEMPLATE.
- Names: nothing made up except parameter names (class_name, parent_index, is_template, index: public symbols carry none).
  SetAsTemplate has one PDB source (FableWin.pdb; it is inline in retail so Ego_r.pdb's public list cannot carry it) plus the
  byte evidence of this row.
- Landed spellings proved wrong (see NAME_FIXES.tsv): LoadDef 0x009AD6E0 is `const` (?LoadDef@CDefinitionManager@@IBEXK@Z, Ego_r
  00b05680) -- the landed row declares it non-const; the landed caller CompileSubDefinition 0x009AE150 declares InstantiateSubDef
  with a non-template CDefPointer return; landed 0x009B4BD0's Leaf uses the placeholder element `CUnprovenElement_009b4bd0*` where
  both callers (InstantiateDef, InstantiateSubDef) prove CDefinitionManager::CInstantiatedDefInfo*.
- Not tried / open: whether InstantiateDef 0x009ADC90's `pdef->Template = is_template;` was also SetAsTemplate(is_template) in the
  original (it matches either way presumably; not graded here -- one row only).
- Repair (2026-11-12): hdr/engine/CDefinitionManager.h was stale (missed NF28's CInstantiatedDefInfo decl blocks); replaced by the
  rebased copy (current header + only this row's opt-in). Regrades RELOCATION_MATCH, also with LANDVERIFY_STRICT_HEADERS=1; test PASS.

## 0x00499AA0


## 0x00499AA0 CProgressDisplay::StartProgress -- RELOCATION_MATCH (author pass 1, 6 grades: 1 SRC_FAIL batch, 5 match grades incl. controls)
- What made it match: the port onto the shared headers (engine/CProgressDisplay.h, CSystemManager.h, CMainGameComponent.h, CFontManager.h, CCountedPointer.h, fable_string.h) plus spelling both strings as implicit-conversion temporaries: `PFont = GFGetSystemManager()->GetFontManager()->GetFont("ENG_ARIAL_16");` and `RenderProgress("Initialising progress display", -1.0f, false, renderFlag);`. With the header's real operator= / ShareData / ReleaseData bodies VC7.1 itself calls the operator= COMDAT (0x00419463) and expands the temporary's destructor with its `Data = 0 ; Info = 0` stores -- the old 410v418 residue was the local model's bodiless operator=, not a compiler tie.
- Headers / flags: current repo headers, NO overlay and no new opt-in (hdr/ is empty). Defines in the row: FABLE_RETAIL_CCHARSTRING, _CWIDESTRING, _CTEXTURE, _CSYSTEMMANAGER, _CSYSTEMMANAGER_FONTMANAGER, _CMAINGAMECOMPONENT, _CMAINGAMECOMPONENT_EDITOR, _CFONTBANK. No pragma, no extra compiler flag. Strict-headers regrade: RELOCATION_MATCH. Test 00499aa0_test.cpp: vt.py PASS.
- Controls graded: a `__declspec(noinline)` operator= opt-in (v1.cpp) also matched, so it is not needed and was removed; the decl-only variant (v2.cpp) was never reached by fg.py (it stops at the first match) -- ungraded.
- Access: PMainGameComponent is protected and PFontManager private in the shared headers; the row uses the public inline accessors CMainGameComponent::Get() and CSystemManager::GetFontManager().
- Made-up names (no PDB data symbol found; spelled as in landed DrawRetailDisplay 0x00498490 / DisplayProgress 0x00499C50): g_bProgressDisplayEnabled 0x01375445, g_bProgressDisplaySuppressed 0x013B89D3, g_bAutoSaveProgressShown 0x013B89C1, g_AutoSaveProgressEndTime 0x013B89C8, g_ProgressDebugDisplay 0x013B85F0, g_bCompileFrontendDefinitions 0x013B8648. Parameter names are descriptive (not in the PDBs).
- Name fixes (see NAME_FIXES.tsv): manifest label `CProgressDisplay::CProgressDisplay::StartProgress` doubles the class; landed WordWrap_SetOption 0x00A0FDB0 declares its 2nd parameter `unsigned long` (leaf YIXIK) where both PDBs say ?WordWrap_SetOption@@YIXII@Z (dossier evidence; this row declares (unsigned int, unsigned int)).
- differ/00499aa0.cpp is the OLD parked draft (superseded by 00499aa0.cpp); v1/v2/v3.cpp are graded variants, not deliverables.

## 0x00EF4320


## 0x00EF4320 NScript::CPersonalScript_EmptyScript::CMeanBandit::Main -- RELOCATION_MATCH (12 grades)
- What made it match: port onto the shared headers (CPersonalScript_EmptyScript.h entity opt-in, CGameScriptInterfaceBase.h, CIsThingAlive.h with
  the inline ctor defined in the row, dtor left out of line) + `FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY` + flag `/d2inlT28`. With the copy
  implicit VC7.1's inline budget decides per site: T23..T28 expands the two Set* by-value argument copies and calls 0x004ABE90 for the
  predicate's Thing and the two Reset* arguments (retail exactly); T<=22 calls all (381 B), T29..T32 460 B, T34+ expands all (492 B).
  T28 is also inside the T28..T36 window of the sibling CTestPerson::Main 0x00EF4B40 (same source file), so one budget fits both.
- Needs: defines FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_CPERSONALSCRIPT_EMPTYSCRIPT_ENTITIES,
  FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY (all already in the repo headers; no overlay header); flag /d2inlT28. Strict-headers regrade passes.
- Names: SetPredicate / IsActiveThreadTerminating (old draft's RegisterFilter / IsDone were wrong; draft never landed). No made-up names.
- No tagged InlineCopy ctor, no local class models, no callee bodies in the row file.

## 0x00A11D10


## 0x00A11D10 CPhysicsMeshObject::GetNavigationLevelAt -- RELOCATION_MATCH (wf5 author, first attempt)
- What made it match: the mesh-space point is a NAMED local filled by the two-argument inline
  `CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) const` (FableWin.pdb name; plain `E1k*x + E2k*y + E3k*z + E4k` rows,
  forced inline), not `inverse * pos` through operator*. Statement list: `if (!Mesh->GetNavigationContainmentVolumes().empty())
  { CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix); C3DVector local_pos; inverse.TransformPoint(pos, local_pos);
  return Mesh->GetNavigationLevelAt(local_pos); } return 0;` with the shared header's SetInverse body unchanged.
- Headers / flags: Flags none. Overlay hdr/engine/CMatrix3x4.h (`FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS`) and
  hdr/engine/CPhysicsMesh.h (`FABLE_RETAIL_CPHYSICSMESH_NAVIGATION`); see hdr/README.md. Strict-header regrade passes.
- Residues on the way (all 422v422 unless noted), so nobody repeats them:
  * operator* forms (zyx / xyz / parenthesised sums, ctor or result-local body): determinant and the stack-held E43 translation
    sum come out 3,1,2 (`B*E31` before `C*E11`; `E33*E43` before `E13*E41`) where retail has 1,3,2; term order in the SetInverse
    source does not change this (VC7.1 re-sorts the sum), and local / parameter names do not either.
  * `float inv = 1.0f / (det expression)` without the `det` local (also through an inline GetDeterminant()): fixes the E43 row but
    swaps three cofactor products' operand order (E33*E12, E13*E31, E31*E12) and leaves the determinant 3,1,2.
  * cofactor locals c11/c12/c13: 435v422. Parenthesised determinant or translation sums: more hunks.
  * early return `if (empty) return 0;`: 418v422 (the `xor eax,eax` tail moves to the top); retail wants the positive `if`.
  * the header's default two-argument TransformPoint (plain inline, accumulated rows): called out of line, 388v422.
- How the form was found: FableWin debug sizes. GetNavigationLevelAt is 113 bytes / 39 instructions there; counting the /Od
  statement list gives 38 for the operator* form and 39 for the TransformPoint(pos, local) form; the debug two-argument
  TransformPoint is 61 instructions = three plain 18-instruction row sums.
- Same-family parked rows 0x00A11F20, 0x00A128E0, 0x00A12AB0 (same inverse): try this form first (named C3DVector +
  two-argument TransformPoint under FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS, det local kept).
- Names: all from the PDBs; locals `inverse` / `local_pos` are descriptive. The landed caller 0x0063EFB0
  (rebuild/src/compiled/00/63/CThingFindNavigationLayerAt_operator_0063efb0.cpp) models this method in a local class; the PDB
  signature is `long GetNavigationLevelAt(const C3DVector&) const` (QBEJ) -- see NAME_FIXES.tsv if its spelling differs.
- Scratch: the exploration variants (v*/s*/c*/g*/n*/t*/u*/f*.cpp, explore_CMatrix3x4.h.txt) are still in the row directory; I
  could not remove them (the removal was blocked by a safety check). Only 00a11d10.cpp, 00a11d10_test.cpp, hdr/, payload.json and
  oracle.tsv are the hand-back; differ/00a11d10.cpp is the OLD parked draft, superseded.

## 0x007F53B0


## 0x007F53B0 CTCSoundAtmosphere::FrameUpdate -- RELOCATION_MATCH (9 grades, first attempt)
- What made it match: (1) the port onto the shared headers (CTCBase, CWorld, CAtmosProcess, CGameDefinitionManager, CDefPointer,
  CDefString, CMainGameComponent, fable_string/fable_map) with the dossier's identities; (2) the atmos-group test spelled as an inline
  taking the layer name BY VALUE (`bool CSoundThemeDef::HasAtmosGroup(CCharString name) const { return AtmosGroupMap.find(name) !=
  AtmosGroupMap.end(); }`): retail copies *it first and reads end() after the copy; an explicit `find(CCharString(*it))` temporary
  reads end() before it (422 v 424); (3) FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY (tree const find body visible -> only
  _M_find<CCharString> 0x0042B467 is called; declaration-only find gave the hidden-iterator call, 434 v 424) with
  FABLE_RETAIL_STLP_TREE_MFIND_DECL; (4) FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_BODY (GetDef<T> body visible: def in esi, handle
  slot reused for gain, null-test-free release).
- Headers / flags: no compiler flags, no pragmas. Overlay hdr/stlport/fable_map.h adds FABLE_RETAIL_STLP_MULTIMAP_CONSTFIND
  (multimap inline const find / end + const_iterator typedef; default path unchanged). std::map<CCharString,long> matches too without
  the overlay, but both PDB layouts type AtmosGroupMap as multimap<CCharString,long>.
- Made-up name (descriptive): CSoundThemeDef::HasAtmosGroup(CCharString) -- the by-value inline is proven by the bytes, its original
  name and owner are in neither PDB (FableWin lists no find/end over multimap<CCharString,long> at all, so the debug build's source
  differs here). Local class models (no includable shared header): CTCSoundAtmosphere, CSoundAtmospheresDef, CSoundThemeDef.
- Landed name proved wrong: manifest label `CTCSoundAtmosphereVillage::FrameUpdate` for 0x007F53B0 -> `CTCSoundAtmosphere::FrameUpdate`
  (?FrameUpdate@CTCSoundAtmosphere@@UAEXXZ): slot 9 of both vtables 0x01271DFC and 0x0124111C; Ego_r.pdb + FableWin.pdb have the
  symbol only on CTCSoundAtmosphere.
- Repair (2026-11-12): hdr/stlport/fable_map.h was stale (dropped the landed FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE blocks); replaced
  with the copy rebased on the current repo header + only the MULTIMAP_CONSTFIND block. Regraded RELOCATION_MATCH (also with
  LANDVERIFY_STRICT_HEADERS=1); test PASS; payload rewritten with addmatch.py. Old overlay kept at verify_mechanical/stale_fable_map.h.bak.

## 0x009692C0


## 009692c0 CAIStateGroup_TavernKeeper::ReceivePayment -- RELOCATION_MATCH (wf5 author, 6 graded variants in 2 grading calls after 2 compile-fix calls)
- What made it match: port onto the shared headers (PlayAnimation sizeof 0xB4, const-ref callees, GetWorld().GetMessageEventManager(),
  CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 20, temporaries C3DVector(0,0,0) / &GetCreature() / &*PPayee in the AddMessage
  call, IS_THING_ALIVE(PPayee)) fixed everything but the first exit block; that block came in place (0x9692F0) once the early exits
  were written as TWO separate `if` statements with identical bodies (`if (!WalkToPoint(..)) {..}` then `if (GetPaymentCancelledMessage()) {..}`).
  As one `||` condition VC7.1 sinks the block to the end of the function (436 v 426 bytes). The else form and the nested-positive form were not graded (fg.py stopped at the match).
- Needs: overlay opt-ins FABLE_RETAIL_CAISTATEGROUPBASE_SETPUSHABLEBYHERO and FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER_RECEIVEPAYMENT
  (hdr/README.md); flags none; strict-headers regrade RELOCATION_MATCH; test 009692c0_test.cpp PASS (vt.py).
- Named-local variant (creature / pos as locals) differs more (12 hunks); `fps*20 + GetFrame()` order differs more (18 hunks).
- No made-up names. No landed name proved wrong by this row beyond the draft's own stand-in spellings listed in DOSSIER section 3.

## 0x0094D780

## 0094d780 CAIStateGroup_TrollBase::Initialise -- RELOCATION_MATCH (2 grades: first port matched, then the strict-header regrade)
- What made it match: the port onto the shared headers with the REAL out-of-line template bodies of CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*)
  and CDefClassBase::GetSubDef<T> visible (existing opt-ins FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY / FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY in
  engine/CDefinitionManager.h, both __declspec(noinline)): VC7.1 then homes every def handle in a dead argument slot (no `sub esp`) and copies the thing def to
  esi. Plus the right def chain (GetSubDef<CAreaOfEffectAttackDef> -> ExplosionIndex, else TrapIndex -> GetDef<CThingBaseDef> -> GetSubDef<CTrapDef> ->
  ExplosionDefIndex; then GetDef<CThingBaseDef> -> GetSubDef<CExplosionDef> -> ExplosionRadius) written as plain nested if / else (no goto), GetTC<CTCPerceiveThing>
  through FABLE_RETAIL_CTHING_GETTC_EXPANDED, and GetGiveUpChaseRadius out of line. No stand-in bodies, no flags, no pragmas.
- Headers it needs (overlay $W/hdr, see hdr/README.md): engine/CAIStateGroup_TrollBase.h `FABLE_RETAIL_CAISTATEGROUP_TROLLBASE` (class on its real base
  CAIStateGroupBase), engine/CExplosionDef.h `FABLE_RETAIL_CEXPLOSIONDEF_MEMBERS` (Damage / ExplosionRadius +0x2C / FireDamage, inline StatGetTypeName).
  CAreaOfEffectAttackDef and CTrapDef have no shared header: local models in the row (as the landed GetSubDef rows spell them) plus the members read.
- Names: all callee names are the PDB spellings through the shared headers. Old draft's names (GetSightRange, GetSpecialAbilityDef, GetWeaponDef,
  GetCreatureSubDef, GetAbilitySubDef, GetThrowRangeSubDef, CTrollCreatureDef ...) were fictional; none survive. No made-up names in the row.
- Landed name proved wrong: 0x00744460 is landed as the address stub `C00744460::M` in DrawGetSeaLevel_DrawGetSeaLevel_00744460.cpp; it is
  CTCPerceiveThing::GetGiveUpChaseRadius() const; the landed caller CAIStateGroupMeleeCombatNearbyBase_IsEnemyRunningAway_0093d3a0.cpp spells the same
  address `GetPerceptionRange` (lines 81, 130) -- both respellings in NAME_FIXES.tsv.
- Test: 0094d780_test.cpp (10 cases: no AoE sub-def, direct explosion, explosion before trap, via trap, defaults, trap without explosion, missing defs;
  ref counts balanced), vt.py PASS. Strict-headers regrade RELOCATION_MATCH.
- Repair (verifier findings, all applied): the -4 retail offset of CAreaOfEffectAttackDef / CTrapDef members is the STLport container size
  (CParentDefClassBase 0x39 v Ego_r 0x3D via CVectorMap; CTrapDef Sound map 12 v 16), not CDefClassBase (0x25 in both); CExplosionDef
  members sit at their PDB offsets (no shift) -- comments fixed in row + overlay. Regrade RELOCATION_MATCH, strict too.

## 0x0064C869


## 0x0064C869 NPlayerGui::CDrawSpiritTargetingCursor::Draw -- RELOCATION_MATCH (wf5 author, 7 grade calls / 13 variants)
- What made it match: (1) the port onto the shared headers with the dossier's identities (CPlayer& player, const CThingPlayerCreature*
  hero, PeekTC(TCI_TARGETING), `GetPTargetedThing() != PTargetedThings[i]`, PeekDisplayEngine().IsLetterBoxMode(), SetAsDrawing(true),
  `pos.Z = pos.Z + GetPhysicsTC()->GetRadius() + 0.35f`, GetMultiplayerColour(player.GetNumber())) took the old 435v428 draft to 429v428
  with only the hero's frame slot wrong; (2) the Q32 lesson closed it: IS_THING_ALIVE's REAL body visible but out of line
  (engine/ThingAlive.h, FABLE_RETAIL_IS_THING_ALIVE + FABLE_RETAIL_IS_THING_ALIVE_NOINLINE, plus
  FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT + _DEREFERENCE for the CIntelligentPointer<CThing> instantiation). With a bare
  declaration the hero gets its own slot and is reloaded after the call; with the body visible it stays in edi and its home word
  [ebp-10h] is shared with PeekTC's key and the gui def reference, as retail.
- Flags: none (source carries #pragma optimize("y", off) + ("s", on)). Strict-headers regrade: RELOCATION_MATCH.
- Headers: hdr/README.md (new NPlayerGui_CDrawSpiritTargetingCursor.h; opt-ins in CDisplayEngine.h, CPlayer.h, CPlayerManager.h).
- Variants tried on the way (ATTEMPTS.log): explicit IS_THING_ALIVE<const CThingPlayerCreature*> on a non-const hero and `const` hero
  (no change, 429); IS_THING_ALIVE<const CThing*>(hero) (428, temp stored one instruction late); a second named copy (slot not shared);
  a block-scoped copy (also RELOCATION_MATCH, but an artefact -- superseded by the visible-body source).
- Names: no made-up names. Draft names proven wrong by the dossier and replaced: GetCharacterThing -> CPlayer::GetPControlledCreature
  (0x00487DC0), TargetDiffers -> operator!=(const CBaseObject*, const CIntelligentPointer<CThing>&) (0x005CBDD2), GetTargetPosition ->
  NTargeting::GetTargetingPositionOnThing (0x007EA2F0; arg 1 is *World). No landed spelling proven wrong.
- Test: test.cpp (model of the loop: main player skipped, dead hero skipped, dead target remembered but not drawn, letter-box mode,
  colour per player number, Z = pos.Z + radius + 0.35); vt.py PASS.

## 0x0095E0B0


## 0x0095E0B0 CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Update -- RELOCATION_MATCH (wf5 author, 7 grades)
- What made it match: (1) the port onto the shared headers (real class over CAIStateGroupBase with `CIntelligentPointer<const CThing> PTarget`,
  shared CCreatureActionBase / Aim / Fire models, ThingAlive.h IS_THING_ALIVE, inline copy ctor `FABLE_RETAIL_CINTELLIGENTPOINTER_COPY`) fixed the
  frame (0xCC) and gave the inline -> out-of-line switch of IS_THING_ALIVE / the copy ctor / the action dtors by itself; (2) the esi/edi swap
  (this vs &PTarget) was the exit structure: every failed step is its own `{ Terminate(); return; }` (nested ifs falling into one Terminate()
  keep `this` in edi); (3) the fire result is a named local (`mov ebx,eax`; unnamed gives `cmp eax,2 ; setne bl`), the aim result is not named.
- Needs: overlay hdr/engine/{CAIStateGroupBase.h, CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets.h, CCreatureAction_AimMissileWeapon.h}
  (three opt-ins, see hdr/README.md). Flags: none. Strict-headers regrade: RELOCATION_MATCH. Test: 0095e0b0_test.cpp (vt.py PASS).
- Local model: CCreatureAction_ReloadMissileWeapon (no engine/ header exists; Ego_r.pdb / FableWin.pdb members, implicit dtor, PAmmo +0xA8, 0xB0).
- Ideas graded and rejected: nested ifs with one `&&` chain (flag dword + frame 0xD0); nested ifs with named aimed/fired (428v428, esi/edi
  swapped); one `&&` chain for the first three tests; a single reused `ret` local; early-return exits with the fire result unnamed (431v428).
- Name evidence: sibling landed row 0x0095E260 IsRunnable spells the member `PTargetThing` and models it as CBaseIntelligentPointer; Ego_r.pdb
  (struct_layouts_egor.tsv) names it PTarget, CIntelligentPointer<const CThing> (see NAME_FIXES.tsv). Landed 0x0069CE20 carries the descriptive
  leaf `IsThingAlive_0069ce20`; FableWin.pdb has ??$IS_THING_ALIVE@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z.
- No made-up names in this row (bool local `inRange`, `fired` are locals).

## 0x00A11F20


## 0x00A11F20 CPhysicsMeshObject::IsSpherePartContainedInBoundingBox -- RELOCATION_MATCH (wf5 author, 2026-11-12)
- What made it match: ported onto the shared headers (no local models), an in-place `CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix);`
  (header body as is), the real callee `Mesh->HasBoundingBoxIntersectedFromTo(scaled_radius, local, local)` (0x00A17690), and the transform as the
  two-argument `inverse.TransformPoint(pos, local)` FORCE-INLINED with one plain left-to-right sum per component
  (`E1k*X + E2k*Y + E3k*Z + E4k`). VC7.1 reassociates / orders the sums by itself (retail X, Z, Y; E3k, E2k, E1k in the translation), so
  source term order inside one sum expression never mattered; what mattered was the CONTEXT of the inline (which member, by-reference
  destination) -- it decides the operand order of every product with E11 and the first-two-terms order of the determinant / E43 / Z-row sums.
- Headers / flags: no flags. Overlay opt-ins (hdr/README.md): engine/CPhysicsMesh.h FABLE_RETAIL_CPHYSICSMESH_BOUNDINGBOXINTERSECTED (declaration),
  engine/CMatrix3x4.h FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_SUMS (forceinline plain-sum body of TransformPoint(pSource,destination)).
  Row defines: FABLE_RETAIL_DEF_CLASS_BASE, _CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS, _CPHYSICSMESHOBJECT, _CPHYSICSMESHOBJECT_MATRIX,
  _CIVCOUNTEDPOINTER, _CMATRIX3X4, plus the two above. Strict-header regrade: RELOCATION_MATCH. Test: vt.py PASS.
- Ladder (grades): header SetInverse + accumulated rows in the row file 428v428/16 hunks (E11 product operand order, E4k and det term order);
  source-order permutations of det / E4k / E11 products, DATA_UNION, optimize("s"), /Oa, /G5, /G7: no effect or worse; named cofactor locals worse;
  D3D-signed (`-inv*(..)`) elements worse (fchs appears, 448); `radius *= OO_Scale` and an inline GetOneOnScale() worse; `local = inverse * pos`
  through an inline operator*(const C3DVector&) returning C3DVector(...) 426v428/6 hunks (only the stack-resident third column's sums swapped);
  forceinline TransformPoint(pos, local) plain sums = MATCH.
- Evidence for the spelling beyond the bytes: FableWin.pdb debug sizes -- ?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z is 61
  instructions (= one 3-product sum + translation per component, not the accumulated form) and the row's debug copy is 38 instructions
  (= default-constructed C3DVector + TransformPoint call, 2..3 more than the operator* spelling).
- Likely to transfer to the parked inverse family (0x00A11D10 GetNavigationLevelAt, 0x00A128E0 IsInContainmentVolume, 0x00A12AB0
  PeekRoughProportionOfSphereContainedInVolume): same `SetInverse` + forceinline plain-sum `TransformPoint(pos, local)` pair.
- No landed name proved wrong. No made-up function names (locals `scaled_radius`, `inverse`, `local` are descriptive).
- differ/00a11f20.cpp is the stale parked draft; the matched source is 00a11f20.cpp (in payload.json).

