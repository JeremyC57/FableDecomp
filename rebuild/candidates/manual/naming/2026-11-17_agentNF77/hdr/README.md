# agentNF77 header overlay (round 7)

Base copies of every changed repo header are in `../orig/` (copied from rebuild/include at first change; `cmp` against the repo
before the final recheck: all fresh). Each opt-in is `#ifdef`/`#ifndef` only. Without the opt-in macro every changed header is
token-identical to its repo copy: checked with `cpp -P` on a one-line TU per header (orig vs overlay, with and without the
HoverHornetQueen ATTACK_PASS23 branch macros, and for the mesh headers with and without the PASS22 / STATICBLOCKS / StaticMesh /
SubPrimitiveRenderer ADD retail-branch macros), all identical.

## Changed repo headers
- `engine/CCreatureAction_SheatheItemToInventory.h` -- `#ifndef FABLE_RETAIL_CCREATUREACTION_SHEATHEITEMTOINVENTORY_IMPLICIT_DTOR` around the
  PDB-generated `virtual ~CCreatureAction_SheatheItemToInventory();` (row 0x0048C950: retail's COMDAT stores no vptr). pin_asserts.py pinned 1
  (the sizeof assert, now FABLE_STATIC_ASSERT_AT(38, ...)).
- `engine/CAIStateGroup_HoverHornetQueen.h` -- in the ATTACK_PASS23 retail branch, `#ifdef FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_KEEPOUTOFWAY`
  declares the protected `EAIActionReturn KeepOutOfWay()` (0x0095B6F0) and `EAIActionReturn GotoSafestCirclePoint(EMovementType)` (0x0095AD20)
  (Ego_r.pdb + FableWin.pdb, IAE). No layout change. pin_asserts.py pinned 1 (the ATTACK branch sizeof assert, now FABLE_STATIC_ASSERT_AT(41, ...)).
- `engine/CCreatureAction_BlowHorn.h` -- `#ifndef FABLE_RETAIL_CCREATUREACTION_BLOWHORN` around the generated flattened layout, and a new
  `#else` retail branch: CCreatureAction_BlowHorn : CCreatureActionBase (RTTI), PObjectToUse CIntelligentPointer<CThing> +0xA8 (Ego_r +0xB4),
  sizeof 0xB0, the PDB method set (ctor and Clone in both PDBs; copy ctor, PerformAction and the IsTo* pair FableWin only), no user-declared destructor (retail's is compiler-generated). Row 0x007EB9B0.
  pin_asserts.py pinned 3 (the generated branch's three asserts, now _AT(53/54/55, ...)); the retail branch's own
  `FABLE_STATIC_ASSERT(sizeof(CCreatureAction_BlowHorn) == 0xb0)` is new (not pinned; nothing used its line before).
- `engine/CCreatureAction_PickCrop.h` -- same shape: `#ifndef FABLE_RETAIL_CCREATUREACTION_PICKCROP` around the generated layout, `#else` retail
  branch on CCreatureActionBase with PObjectToUse CIntelligentPointer<CThingObject> +0xA8, sizeof 0xB0, ctor 0x009405F0 / copy ctor 0x009406E0 /
  PerformAction 0x00844720 / Clone 0x009406B0, no user-declared destructor. Row 0x009400F0. pin_asserts.py pinned 3 (_AT(53/54/55, ...)); the
  retail branch's sizeof assert is new.

- `engine/CEngineInternalPrimitive2DStaticMesh.h` -- `#ifndef FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH` around the generated flattened
  layout and a new `#else` retail branch: CEngineInternalPrimitive2DStaticMesh : CEngineInternalPrimitiveStaticMesh (RTTI), the Ego_r.pdb members
  (Initialised +0xAC, ClipBox +0xB0, Layer +0xC0, BoundingSphere +0xC4, Orientation +0xD4 opaque 52 bytes, WorldPos +0x108, the alpha-mask
  members +0x114..+0x12C; sizeof 0x130), the PDB virtual overrides (??0 / ??1 FableWin only), the class's new virtual slot 33
  CalcInterpolatedObjectTransform(float, bool) and Initialise (0x00B9E120). Row 0x00B9E950. pin_asserts.py pinned 21 (all the generated
  branch's asserts); the retail branch's sizeof assert is new.
- `engine/CEnginePrimitiveManagerMeshBase.h` -- `#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_RENDERMODE2D` declares the static fastcall
  GetAppropriateRenderMode2D (FableWin.pdb only); rows 0x00B9E950 / 0x00BD1F70 give its inline body. pin_asserts.py pinned 0.
- `engine/CEngineSubPrimitiveRenderer.h` -- in the _ADD branch, `#ifdef FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_SORTGRANULARITY` adds the inline
  FableWin.pdb GetSortGranularity() (MaxSortDistance / 2^30). Rows 0x00B9E950 / 0x00BD1F70. pin_asserts.py pinned 0.
- `engine/CEngineInternalPrimitiveAnimatedMesh.h` -- `#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_SETUPMESH2D` declares the public
  ?SetupMesh2D@CEngineInternalPrimitiveAnimatedMesh@@QAE_NXZ (0x00BCF430). Row 0x00BD1F70. pin_asserts.py pinned 0.

## New headers (no repo copy)
- `engine/CCreatureAction_PlayIntoLoopOutOfAnimation.h` -- the retail class (the repo has only a quarantined generated layout under
  engine/_quarantine/): CCreatureAction_PlayIntoLoopOutOfAnimation : CCreatureActionBase (RTTI), Ego_r.pdb members Stage .. OutOfAnimName at
  retail +0xA8..+0xC0 (as the landed ctors 0x00842740 / 0x008427E0 store them), sizeof 0xC4, the four Ego_r/FableWin constructors, PerformAction (FableWin only) /
  FinishAction / IsUsingAnimationMovement / Clone; the destructor is declared unless FABLE_RETAIL_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_IMPLICIT_DTOR
  (row 0x0093A7A0). Needs FABLE_RETAIL_CCREATUREACTIONBASE.
- `engine/NTCScriptedControl_CActionDropGenericBox.h` -- NTCScriptedControl::CActionDropGenericBox : CTCScriptedControl::CActionBase (RTTI),
  Ego_r.pdb ActionID +0x0C / ActionPluggedIn +0x10, sizeof 0x14, ctor 0x009036A0 and virtual dtor (FableWin only), Update (slot 1, 0x00715560), GetActionName
  (slot 4, 0x009036D0). Includer contract as NTCScriptedControl_CActionPlayAnimation.h. Row 0x00715560.

- `engine/CEnginePrimitiveManager2DStaticMeshes.h` / `engine/CEnginePrimitiveManager2DAnimatedMeshes.h` -- the two row classes (NF74's drafts
  re-based on the RTTI bases CEnginePrimitiveManagerStaticMeshesBase / CEnginePrimitiveManagerAnimatedMeshesBase; NF74 had MeshBase /
  AnimatedMeshesBase): no own members, the PDB method set (??1 FableWin only) with the retail vtable slots 0/1/2/5/10. Rows 0x00B9E950 / 0x00BD1F70.

## Repo opt-ins used without change
FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR (rows 0x00941E00, 0x0095B6F0), FABLE_RETAIL_CCREATUREACTION_GENERICBOX (0x00715560,
whose branch is already implicit), FABLE_RETAIL_CTHINGCREATUREBASE_BEDSLEEP (SheatheAllWeapons, 0x0093A7A0), _ANIMATION (PeekPCurrentAction,
0x00715560), FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS (AIAction_LookAtPos, 0x00941E00)., and for the mesh rows the 0x00BA1140 / 0x00BD71B0 preambles plus C3DMeshInfo _ISLOADED / _ARETEXTURESLOADED,
CEnginePrimitiveTypeInfo / CEnginePrimitive2DViewportManager / CDisplayManager _GETVIEWPORT / CEnginePrimitiveRenderer2D _MEMBERS /
_LAYER2DTODISTANCE / _LAYER2DTOZ, CENGINEINTERNALPRIMITIVE2DANIMATEDMESH and CENGINESUBPRIMITIVEBASE_SETTERS_ALPHAFLOAT.

## Name collisions checked
Every new macro name and the four new header basenames were grepped in rebuild/src, rebuild/tests, rebuild/include,
rebuild/candidates/manual/pending and the NF75 / NF76 hdr/ overlays (see NOTES.md for the result).
