# e04 header overlay opt-ins (copies of the CURRENT repo headers plus additions only)
- engine/NCameraModes_CTools.h  FABLE_RETAIL_NCAMERAMODES_CTOOLS_ROLLING_AVERAGE_PAREN  -- a new top-level block AFTER the class's
  static assert (nothing moved; no existing block edited), active with ROLLING_AVERAGE + ROLLING_OUTOFLINE: the inline out-of-class body
  of GetRollingAverage<T> with the return expression parenthesised `return ((cur*w+new)/(w+1.0f));`. Evidence: row 0x00871FD0
  RELOCATION_MATCH (unparenthesised: one fld/mov swap at 0x00871FF5); the other users regraded RELOCATION_MATCH with the parenthesised
  text: 0x00697CC0 UpdateRightStickLookAround (OUTOFLINE + PAREN), 0x0084EAF0 GetRollingAverage<C3DVector> and 0x008FCC60
  CPhysicsCoordRigid::FrameUpdate (their in-row bodies parenthesised; 0x008FCC60's in-row `weight+1.0` double also matches as 1.0f).
  Ego_d / FableWin ??$GetRollingAverage@M@CTools@NCameraModes@@QBEMABM0M@Z: no named locals.
- engine/CTCHeroMorph.h  FABLE_RETAIL_CTCHEROMORPH_ADDTATTOO  -- nested in the FABLE_RETAIL_CTCHEROMORPH* class block: declarations
  AddTattoo(CDefPointer<const CThingBaseDef>&) 0x0071D940 and AddTattoo(long) 0x0071DA70. Evidence: Ego_r.pdb ?AddTattoo@CTCHeroMorph@@QAEXAAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z
  (0x00568390) / ?AddTattoo@CTCHeroMorph@@QAEXJ@Z (0x005684E0), same order as retail; row 0x005AD305 calls both with the morph in ecx.
  Inserted before the TATTOONAMES block; this header has no static asserts.
- engine/CAIStateGroup_MazeBase.h (NEW header; no repo header existed)  FABLE_RETAIL_CAISTATEGROUP_MAZEBASE  -- CAIStateGroup_MazeBase :
  CAIStateGroupBase from the Ego_d class record (public virtual OnActivate / OnReplace, protected HasEnemyBeenKnockedDown() -> bool,
  protected PTargetThing / EnemyKnockDownFrame; compiler-generated dtor / operator= not declared), offsets / size 0x20 from Ego_r.
- engine/CAIStateGroup_MazeTeleport.h  FABLE_RETAIL_CAISTATEGROUP_MAZETELEPORT  -- `#ifdef` around the generated flattened struct: the
  opt-in branch declares the polymorphic class : CAIStateGroup_MazeBase (Ego_d: no own members; virtual Update / IsRunnable / OnActivate);
  the default branch is the unchanged generated struct. Its two FABLE_STATIC_ASSERT_AT(18 / 19) lines moved down (ids pinned by the macro
  argument, so the generated names do not change). Row 0x00954F40.
