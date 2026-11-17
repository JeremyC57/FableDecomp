# NF77 notes (round 7): the q10884 implicit-destructor binders and the 2D mesh renderer pair

## q10884 (??1CCreatureActionBase@@UAE@XZ 0x00693EF0): seven rows ported
All seven rows are on the shared headers and bind ??1CCreatureActionBase@@UAE@XZ. Each derived action class has no user-declared
destructor (hdr/README.md lists each opt-in). The grades used were 1, 1, 8, 1, 2, 2 and 1 (0x0048C950, 0x009400F0, 0x0093A7A0, 0x0095B6F0,
0x00715560, 0x00941E00, 0x007EB9B0). All are RELOCATION_MATCH and their tests PASS (recheck_final.log).

### Implicit-destructor COMDAT rows (0x0048C950, 0x009400F0)
- **Emitter.** Both rows use a `DestroyOne` emitter: a destruction by qualified name, ungraded.
  - This is the landed NF69 form (~CTCTargetingBase 0x004DA8B7), which the brief cites as the model, and 0x006DF860 uses the same idea.
  - NF72's blocking line for 0x0048C950 said "graded from a TU that calls it out of line, not a stand-in emitter function". Grading from
    a landed caller's TU would mean pinning `// Leaf:` to the COMDAT inside a copy of that caller's body (for example TerminateMinigame
    0x006DE280). NF77 chose the landed emitter form instead.
  - 0x009400F0's only caller, VillageTaskPickCrops::Update 0x00940310, is not landed.
  - The coordinator may prefer the caller-TU form for 0x0048C950. Its landed callers are 0x0058400A, 0x0062B7C0 and 0x006DE280.
- **0x009400F0 owner identified: CCreatureAction_PickCrop.**
  - Its one call (0x0094049E, in the unlanded CAIStateGroup_VillageTaskPickCrops::Update 0x00940310) destroys the temporary that the
    CCreatureAction_PickCrop constructor 0x009405F0 builds at the same frame address.
  - The body is the class's one Ego_r.pdb member (PObjectToUse, CIntelligentPointer<CThingObject>, +0xA8) followed by its RTTI base.
  - ??_G 0x00940680 expands the same body.
  - FableWin.pdb lists ??1CCreatureAction_PickCrop@@UAE@XZ (0x02B98AA0); Ego_r.pdb has the class (ctor 0x009643F0, PerformAction, Clone) but no ??1.

### Self-row renames (RENAMES.tsv section 1, 6 rows)
| Row | Old | New |
|---|---|---|
| 0x0048C950 | stand-in CPhysicsMeshInfo_Derived | ??1CCreatureAction_SheatheItemToInventory@@UAE@XZ |
| 0x009400F0 | Obj / Sub stand-in | ??1CCreatureAction_PickCrop@@UAE@XZ |
| 0x0093A7A0 | QAEJXZ | ?Singsong@..@@AAE_NXZ |
| 0x0095B6F0 | CQueenNative | ?KeepOutOfWay@CAIStateGroup_HoverHornetQueen@@IAE?AW4EAIActionReturn@@XZ |
| 0x00715560 | QAE | ?Update@CActionDropGenericBox@NTCScriptedControl@@UAEXXZ |
| 0x00941E00 | QAE | ?Update@CAIStateGroup_KidReactToBlockage@@UAEXXZ |

0x007EB9B0 kept its leaf.

Landed callers, from probes (NF77 compiled each caller and read its relocations):
- **Already bind the new spellings:** 0x0095C0B0 (KeepOutOfWay), and 0x0058400A / 0x0062B7C0 / 0x006DE280 (~SheatheItemToInventory).
- **Still wrong (NAME_FIXES.tsv):** 0x0093AD70 SitMain (Singsong QAE_NXZ); 0x00487DE0 and the three ??_G rows 0x006A0950 / 0x006A0970 /
  0x006A0990 (~SheatheItemToInventory, through stand-ins).

absrefs on the seven addresses finds only vtable slots (no landed data reference). No landed row or test includes a changed row's
file, and no funclet label names one.

### New layout facts
- **0x0093A7A0.** Ego_d.pdb names only `ret` and `singsong_time` as Singsong's locals. The three CCharStrings must still be named
  locals: call temporaries interleave the pushes (graded, 217v227).
- **The bool tail.** Retail's `xor eax,eax ; cmp esi,2 ; sete al` comes from declaring `ret` outside a block that holds the strings, the
  creature and the call, as Eat 0x0093A5C0 holds its result. A function-scope `ret` (enum or long), `?:`, `!= 0` or two returns all drop
  the xor or grow the body.
- **Descriptive names.** None are new. The class and member names are PDB names; the parameter names in the new headers come from the
  landed constructors.

## Known deviations for review
- **Opt-in macro name.** FABLE_RETAIL_CCREATUREACTION_PLAYINTOLOOPOUTOFANIMATION_IMPLICIT_DTOR is declared in a NEW header, so its
  default path keeps the PDB-generated declaration, matching the sibling headers.
- **CCreatureAction_BlowHorn.h / CCreatureAction_PickCrop.h.** Their new retail branches have no destructor declaration at all: they are
  new branches, so the retail truth is their default.
- **0x00715560.** It defines inline bodies for the header-declared CTCScriptedControl::GetParentCreature,
  CActionBase::GetParentCreature and CActionBase::Terminate, as the landed sibling 0x00712900 does.

## The 2D mesh renderer pair (0x00B9E950, 0x00BD1F70): both ported
Both rows are RELOCATION_MATCH + PASS. 0x00B9E950 took 4 grades and 0x00BD1F70 took 4; the last grade of each re-confirms the final file
with its comment block. Before the first grade, header probes compiled each draft without grading it: 3 for 0x00B9E950 and 1 for
0x00BD1F70 (hp.py, logged in grades.log). The entries they serve are all done: q10478, nf41:e120 / e188 and nf44:70 / 71 / 73.

### Templates
- **Preambles.** Each 2D renderer is the 2D twin of a landed 3D renderer on the shared headers, so the ports start from those 3D
  preambles and their inline helper bodies. 0x00B9E950 starts from 0x00BA1140, CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive.
  0x00BD1F70 starts from 0x00BD71B0, CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive.
- **Alpha-mask clip test.** Both use the landed 2D sprite row 0x00BB0970 (GetCurrent2DAlphaMaskInfo).

### Names corrected from the old local models
- **Slot 33.** It is ?CalcInterpolatedObjectTransform@CEngineInternalPrimitive2DStaticMesh@@UAEXM_N@Z, the class's own new virtual (vtable
  0x012A42E4 slot 33 = 0x00B9D220). It is fed by the inline GetCurrentInterpolate. The animated twin's slot 33 is the base's const
  CalcInterpolatedObjectTransform(CMatrix3x4&, float, bool); the old models called these SetLOD / GetLODTransform.
- **Slot 15** of the animated mesh is GetWorldPosition. Its result is unused, but the virtual call stays.
- **The local "CSubMesh"** is C3DPrimitive2, and "list" is ERenderMode: GetAppropriateRenderMode2D, 2D_TEXTURE / 2D_UNTEXTURED.
- **The flags** are the inline CalculateSubPrimitiveRenderFlags(false, IsTransparent, IsBooleanAlpha, EnginePrimitiveFlags, false).
- **The depth bias** is FableWin's inline CEngineSubPrimitiveRenderer::GetSortGranularity() (MaxSortDistance / 2^30).
- **The manager classes** derive from CEnginePrimitiveManagerStaticMeshesBase / CEnginePrimitiveManagerAnimatedMeshesBase (RTTI). NF74's
  drafts had used MeshBase.

### Other notes
- **No local class models remain** in either row. They keep the six `namespace NEngineInternal { extern ... }` data declarations, which
  is the landed form for these globals, and the inline bodies of header-declared helpers, as 0x00BA1140 / 0x00BD71B0 have them.
- **recheck_final.log carries two RELOC_NAME warnings.** In both mesh rows, ?GPLightingManager@NEngineInternal@@... is labelled
  'g_pEngineLightingManager' at 0x01436E9C. The old rows already bound that same symbol, so this is a stale catalog label, not a new
  disagreement.
- **Shared fix for another agent.** reloc_bindings still lists ?IsBuilt@C3DMeshInfo@@QAE_NXZ / ?SetLoadPriority@C3DMeshInfo@@QAEXJ@Z only from
  these two rows (now respelled). The generated CEngineInternalPrimitive2DStaticMesh layout is a `struct`, while the PDB and the new
  retail branch spell it `class`. Any landed binder that mangles PAUCEngineInternalPrimitive2DStaticMesh@@ was not checked here.

## Rule slips
1. One Bash call exited 1. The first hp.py header probe ran before `source $S/wine/env.sh`, so cl.exe was not found. Nothing was
   compiled or written outside my directory.
2. One call ran `llvm-pdbutil` on Ego_d.pdb to resolve a type index. The brief allows Ego_d.pdb reads only through `python3 -I` and
   egod_locals.py. The permission layer denied the call and nothing ran; it was not retried.

git was never run, nothing was written under /home/user/FableDecomp, and no process I did not start was touched.

## Checks before hand-back
- **PDB attributions.** Every cited symbol was grepped in egor_pdb_names.tsv and fablewin_pdb_names.tsv separately; the wording names only
  the tables that list each symbol. FableWin-only:
  - ??1CCreatureAction_PickCrop;
  - CActionDropGenericBox ??0 / ??1;
  - the BlowHorn copy constructor, PerformAction and IsTo*;
  - the PlayIntoLoop PerformAction;
  - GetAppropriateRenderMode2D and GetSortGranularity;
  - the 2D static mesh's ??0 / ??1;
  - the two 2D managers' ??1.

  Everything else cited is in both tables.
- **Subclass layouts.** The three CCreatureAction_SheatheItemToInventory_For* subclasses are 188 bytes with no own member in
  struct_layouts_egor.tsv.
- **Name collisions.** None of the nine new macro names and none of the four new header basenames appears in rebuild/src, rebuild/tests,
  rebuild/include, rebuild/candidates/manual/pending or the NF75 / NF76 hdr/ overlays. No file includes the quarantined
  engine/_quarantine/CCreatureAction_PlayIntoLoopOutOfAnimation.h, which shares the new header's include guard. NF75 / NF76 have not
  overlaid any header changed here.
- **Old leaves.** The RENAMES section-1 old leaves were read from the old rows' compiled objects.
- **Unprobed leads.** reloc_bindings.tsv lists stand-in spellings from other landed rows for 0x00A256F0 (C3DMeshInfoMethods, QAE BeginLoading),
  0x00A23E40 (C3DMeshInfoMethods), 0x00A25E70 (C3DMeshInfoCallView / CMeshPointer), 0x00A36EC0 (?Destroy@CMeshPointer) and 0x00BEB540
  (??2CEngineSubPrimitive2DViewport). They are outside this slice and were not probed.
- **Landed callers of the six renamed addresses** (rebuild/oracles/reloc_bindings.tsv, target column):
  - 0x0048C950: callers 0x00487DE0, 0x006A0950/70/90 and 0x008916A0 bind wrong spellings; NAME_FIXES lists all five. 0x007EDCD0 already binds ??1...UAE@XZ.
  - 0x0093A7A0: SitMain 0x0093AD70 binds QAE (in NAME_FIXES).
  - 0x0095B6F0: the only caller, 0x0095C0B0, already binds the IAE KeepOutOfWay spelling.
  - 0x009400F0, 0x00715560 and 0x00941E00: no landed caller. Their callers are the unlanded VillageTaskPickCrops::Update and the vtables.
- **RELOC_NAME warnings.** Both 2D rows bind ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A for 0x01436E9C, the PDB spelling. The warning compares it with the stale Ghidra label g_pEngineLightingManager; it is not a name conflict between rows. The landed 3D twins 0x00BA1140 / 0x00BD71B0 name the same GPLightingManager in their source, and reloc_bindings shows 0x00B23EA0 binding the same spelling.
- **Old names in comments.** The old catalog names (Dtor_CCreatureActionBaseBase_009400f0, vfunc_1) appear in the rows only as history comments. vland's label column reads the repo catalog, which RENAMES section 1 respells.
- **Final pin pass.** pin_asserts.py was rerun on all eight changed headers after the last comment edits. It pinned three more asserts: HoverHornetQueen's ATTACK_PASS23 sizeof assert to line 41, and the retail-branch sizeof asserts of BlowHorn (to 53) and the 2D static mesh (to 81). The default path is still token-identical to the repo, and all eight orig/ copies match the current repo headers. recheck_final.log was rerun after this pass: 9/9 RELOCATION_MATCH PASS.
