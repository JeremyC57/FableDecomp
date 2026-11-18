# 2026-11-17 landing nf77: name-fix round 7, NF77 (9 rows: the 2D-mesh pair and the q10884 implicit-destructor binders)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF77 took the two 2D-mesh Render2DPrimitive rows that NF74 had left blocked and seven binders of q10884 and its neighbouring
entries. It ported all nine. Brief, list and records: `naming/2026-11-17_agentNF77/`.

## Rows (9 ported; 0 not-a-binder, 0 blocked)

| Row | Grades | Now |
|---|---|---|
| 0x00B9E950 | 4 | `CEnginePrimitiveManager2DStaticMeshes::Render2DPrimitive`, on the new manager header and the retail branch of CEngineInternalPrimitive2DStaticMesh.h (sizeof 0x130) |
| 0x00BD1F70 | 4 | `CEnginePrimitiveManager2DAnimatedMeshes::Render2DPrimitive` |
| 0x0048C950 | 1 | `??1CCreatureAction_SheatheItemToInventory@@UAE@XZ` (was `??1CPhysicsMeshInfo_Derived_0048c950`, module `_unattributed`) |
| 0x009400F0 | 1 | `??1CCreatureAction_PickCrop@@UAE@XZ` (FableWin.pdb 0x02B98AA0; was `_global::Dtor_CCreatureActionBaseBase_009400f0`) |
| 0x0093A7A0 | 8 | `?Singsong@CAIStateGroup_BanditSitRoundCampfire@@AAE_NXZ` (private, bool; leaf was `QAEJXZ`) |
| 0x0095B6F0 | 1 | `?KeepOutOfWay@CAIStateGroup_HoverHornetQueen@@IAE?AW4EAIActionReturn@@XZ` (leaf was `?KeepOutOfWay@CQueenNative@@QAEJXZ`) |
| 0x00715560 | 2 | `?Update@CActionDropGenericBox@NTCScriptedControl@@UAEXXZ` (was `vfunc_1`; vtable 0x0128D7BC slot 1) |
| 0x00941E00 | 2 | `?Update@CAIStateGroup_KidReactToBlockage@@UAEXXZ` (leaf to the virtual `UAE`) |
| 0x007EB9B0 | 1 | `CTCActionUseBlowHorn::OnActionUse`, now on the CCreatureAction_BlowHorn retail branch |

- The two implicit destructors are emitted by calling the qualified destructor directly, the form NF69 landed for 0x004DA8B7.
- Both 2D rows bind the PDB spelling `?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A` at 0x01436E9C, like their
  landed 3D twins and 0x00B23EA0. vland's two RELOC_NAME warnings come from the stale Ghidra label `g_pEngineLightingManager` there
  (queued).

## Entries (APPLIED.tsv): 7 done
q10884, q10478, nf41:e120, nf41:e188, nf44:70, nf44:71 and nf44:73.

## Rows and checks
- **Section 1: 6 self renames (6 file moves; the leaf-only renames keep their catalog names).**
- **Headers (9 opt-ins):** eight existing headers gain additions under new `FABLE_RETAIL_*` opt-ins (CCreatureAction_SheatheItemToInventory.h,
  CCreatureAction_PickCrop.h, CCreatureAction_BlowHorn.h, CAIStateGroup_HoverHornetQueen.h, CEngineInternalPrimitive2DStaticMesh.h,
  CEnginePrimitiveManagerMeshBase.h, CEngineSubPrimitiveRenderer.h, CEngineInternalPrimitiveAnimatedMesh.h), and there are four new headers
  (CCreatureAction_PlayIntoLoopOutOfAnimation.h, NTCScriptedControl_CActionDropGenericBox.h, CEnginePrimitiveManager2DStaticMeshes.h,
  CEnginePrimitiveManager2DAnimatedMeshes.h), each used by a landed row. NF77's base copies were identical to the repo at hand-back.
  Every assert the opt-ins moved is pinned; `lane_merge/assert_names_check.py` reports 0 mismatches against 49b943df65. Details:
  `naming/2026-11-17_agentNF77/hdr/README.md`.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 9 rows graded from the repo after landing: 9 RELOCATION_MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 37 binding lines and added 8; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 4)
NF77's NAME_FIXES.tsv (2): SitMain 0x0093AD70 still binds Singsong as `QAE_NXZ`; five binders of 0x0048C950 bind wrong spellings
(0x00487DE0 through a local model, the `??_G` stand-ins 0x006A0950 / 0x006A0970 / 0x006A0990, and 0x008916A0). Two coordinator lines:
the Ghidra label of 0x01436E9C, and stand-in spellings NF77 saw bound for 0x00A256F0, 0x00A23E40, 0x00A25E70, 0x00A36EC0 and
0x00BEB540 (not probed).

## Rule slips
- One Bash call exited 1 (a helper ran before `source env.sh`).
- One `llvm-pdbutil` call on Ego_d.pdb was denied by the permission system; it was not retried.
