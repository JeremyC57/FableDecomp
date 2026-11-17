# NF62 notes (round 7, 49 blocked rows re-ported onto the shared headers)

Conventions
- grades_used in STATUS.tsv counts every q.py compile of the row in work/grades.log, including the one baseline regrade of the landed
  file (`g.sh <addr> <landed> zzzz base`, which only records the old bindings in work/base_<addr>.xref). Attempts = grades_used - 1.
- Final checks: work/vl_batch*.log (vland --recheck-landed with this overlay); recheck_final.log is one run over every changed row.
- Section 2 of RENAMES.tsv is generated from base_<addr>.xref vs last_<addr>.xref (work/addsec2.sh).

Coordinator header decisions (rows are ported, the choice is the coordinator's)
- 0x00691AE0: Ego_r.pdb UInputEventData has anonymous union members with vector types (MouseMovement C3DVector +0x0C, MousePos +0x18,
  JoystickPos ...), which a C++03 union cannot hold (constructors). engine/CInputEvent.h keeps Raw[0x20]; the overlay opt-in
  FABLE_RETAIL_INPUTPROCESS_GETMOUSEMOVEMENT reads X / Y from Data.Raw[0x0C] / [0x10] inside CInputProcessGameBase::GetMouseMovement
  (Ego_r 0x006B2E30, inline in retail). A typed union (POD stand-ins of the vectors) would replace the raw read.
- 0x007E7C90: retail calls an argument-less push_back at 0x005ED940 (default handle local, inline vector push_back, local destroyed).
  The only argument-less push_back in Ego_r.pdb / FableWin.pdb is CArray<T>'s (18 instances), so the row types Handles as
  CArray<CEnginePrimitiveHandle> (overlay FABLE_RETAIL_CARRAY_PUSHBACK_DEFAULT_DECL); Ego_r.pdb (Xbox, Dinkumware) types
  CTCFireheartMinigame::Handles and CTCTavernGame::Handles (the other caller, 0x005EC7C0) as vector<CEnginePrimitiveHandle>. The
  alternative is an STLport 4.6 `vector::push_back()` anachronism (not provable here: no STLport source on disk). One source each.
- 0x00799900: the customer latch is spelled `{ CIntelligentPointer<CThingCreatureBase> c; c.Set(customer); Customer = c; }` (FableWin
  ?Set). It is the only spelling found that reads the address-taken pointer after the temporary's base ctor, as retail does; the
  by-value (T*) ctor reads it first (NF55's DIFFER).
- 0x006DCB20: 0x004F97B3 now binds ??0?$map@W4EDamageAttribute@@M..@@QAE@ABV01@@Z (FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE); the landed
  row at 0x004F97B3 calls itself NPairLongMapTree::_Rb_tree::_Rb_tree (a tree copy ctor). The bytes cannot tell map(const map&) from
  the inlined _Rb_tree copy; one address must keep one name -- not put in NAME_FIXES.tsv (unproven), flagged here.
- 0x0063A540: CMap::GetThemeMinCameraSizeZOffsetAt (0x0081FA90) is protected in Ego_r.pdb; the overlay makes CWorldMap a friend of CMap
  (as CRegion does for CWorldMap, GOTCHAS NF29).

Descriptive names kept: 0x004E9550 CPersistContext::TransferHelper (no PDB name found).
- 0x004FE2A0: the repo CWorldMap.h / CThingManager.h LOADFROMFILE block declares LoadAllLoadableGlobalThingsFromSingleFile public
  (QAE); Ego_r.pdb spells it protected (IAE). The row keeps QAE as the header gives it; respelling needs the coordinator to move the
  declaration under `protected:` in the repo header (then the row's self leaf and its callers move together).
- 0x0069AD10: Ego_r.pdb + FableWin.pdb ?PushCombatCameraMode@CGameCameraManager@@QAEXXZ for 0x0069A8B0 (landed as
  CTCCombat::AddMeleeFlourishAbility, a propagated label) and the private const ?IsObservedThingInMelee@..@@ABE_NXZ for 0x006994F0;
  both in NAME_FIXES.tsv with the landed caller 0x0069AC30.
- 0x006C7370: the named vector transfer is TransferVector (Ego_r.pdb and FableWin.pdb each spell all 40 named vector transfers
  ??$TransferVector@..QAEXPBDAAV?$vector@; none ??$Transfer@ over a vector). The map range copy needs the STLport iterator typedefs
  (overlay FABLE_RETAIL_STLP_TREE_ITERATOR_TRAITS) and the input-iterator __copy out of line (FABLE_RETAIL_STLP_COPY_INPUT_NOINLINE).
- 0x00713510: retail expands the CCreatureAction_PerformExpression temporary's destructor; only an implicit destructor does that in
  VC7.1 (an in-class `{}` body was still called out of line, grade 4). ??1 is in FableWin.pdb only.
- 0x0072E130: 0x0072DD60 is ?AddThingToOwnerTC@CTCMeshAutomaticEntityCreator@@AAEXAAVCThing@@@Z (the only PDB member of that class
  that fits; landed as AddOwnedEntityTC(CThing*)). CTCMeshAutomaticEntityCreator has no shared header: local model.
- 0x006C3620: 0x006C31B0 is bound as the member template ??$_M_find@VCCharString@@@..ABE.. (38 landed bindings spell _M_find that way;
  the landed self row spells it as a plain member). C3DSkeletalMorphTarget gets its retail size (0x10) with BoneMorphs opaque.
