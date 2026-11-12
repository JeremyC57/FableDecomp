## 0x00959BF0

## 0x00959BF0 CAIStateGroup_HoverWander::Update -- RELOCATION_MATCH (wf18)
- Matched by: porting onto shared headers (GetWorld/GetWorldSeed, GetTC expanded, GFRandom/GFSin/GFCos headers, LookAtPos enums),
  `perp = dir.CrossWithUp()` (FableWin ?CrossWithUp@C3DVector@@QBE?AV1@XZ, inline) and `dir = target - here; dir.Z = 0.0f;`
  (the XY-constructor spelling left an x87 allocation residue: c/-X popped vs consumed, T.Y+Ysd operand order).
- Needs: FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP (repo) + overlay opt-ins FABLE_RETAIL_C3DVECTOR_CROSSWITHUP,
  FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS (slots 14..21; names = manifest labels, only GetCombatMinDistance proven in Ego_r),
  FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS. Flags none. Strict headers: RELOCATION_MATCH.
- Repair (2026-11-12): class now comes from the shared header engine/CAIStateGroup_HoverWander.h via overlay opt-in
  FABLE_RETAIL_CAISTATEGROUP_HOVERWANDER (base CAIStateGroup_HoverMovementBase, public virtual Update; default flat layout unchanged);
  slot 20 IsCombatStateGroup made public in the VSLOTS overlay (UBE in both PDBs). The earlier "name note" on
  CTCCreatureNavigation::GetNearbyWanderPosition was wrong and is withdrawn: Ego_r 0x00701120 / FableWin 0x024787D0 both give
  ?GetNearbyWanderPosition@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@_N@Z, matching the landed header. Still RELOCATION_MATCH (strict too).

## Repair 3 (2026-11-12) -- still RELOCATION_MATCH (default and LANDVERIFY_STRICT_HEADERS=1); test PASS (vt.py)
- Enums: the source now uses the PDB enumerators (EAIAction.h / EMovementType.h, PDB TPI enums): ACTION_REPEAT_UNTIL_FINISHED (1)
  for LookAtPos, ACTION_REPEAT_CALLBACK (2) for MoveToPosAndFacePos, AI_ACTION_FINISHED (2), AI_ACTION_IN_PROGRESS (4),
  `GFRandom(3, seed) + AI_MOVEMENT_HOVER_SLOW` (8..10 = HOVER_SLOW..HOVER_FAST). No byte change. The LookAtPos flags 0x11 stay a
  literal: the parameter is a plain `long` in AIAction_LookAtPos(EAIActionRepeat,C3DVector,long,float), no enum. The earlier
  "LookAtPos enums" claim is now true. Test uses the same enumerators.
- Headers re-based on the CURRENT repo (repo moved during the day): C3DVector.h (CROSSWITHUP) and CAIStateGroup_HoverMovementBase.h
  (MOVETOPOSANDFACEPOS) and CAIStateGroup_EnemyCreatureBase.h (VSLOTS) are now in the repo; the C3DVector / HoverMovementBase
  overlay copies were identical to the repo and are removed. Overlay keeps CAIStateGroup_HoverWander.h (opt-in + new declaration
  `virtual bool IsCombatStateGroup() const;` = FableWin ?IsCombatStateGroup@CAIStateGroup_HoverWander@@UBE_NXZ 0x01811FCD, retail
  slot 20 0x00900C13) and CAIStateGroup_EnemyCreatureBase.h (VSLOTS comment rewritten with evidence; slot 20 public + marked INFERRED).
- Evidence correction: the Ego_r extract (egor_signatures.tsv, ego_r.exe.signatures.tsv) has only GetCombatMinDistance for
  EnemyCreatureBase; the other six slot names (Init, IsRunnableLeaf, OnActivateLeaf, CleanupLeaf, GetCombatProximity,
  IsRequireDirectAwarenessOfThing) with MAE/MBE come from FableWin.pdb (fablewin_pdb_names.tsv) plus the retail signature transfer
  (Fable.exe.signatures.tsv 0x008FE0EE..0x008FE102). The verifier's "Ego_r proves all seven" is not reproducible from the on-disk
  extracts; the header states what is.
- IsCombatStateGroup on EnemyCreatureBase: no symbol in Ego_r/FableWin for EnemyCreatureBase, CAIStateGroupBase or HoverMovementBase
  (0 hits); only the 15 derived classes (all UBE). Placement is INFERRED for the slot layout; commented in the overlay.
- Sibling recheck: landed 0x00959F80 CAIStateGroup_HoverKeepInArea::Update still RELOCATION_MATCH with this overlay.

