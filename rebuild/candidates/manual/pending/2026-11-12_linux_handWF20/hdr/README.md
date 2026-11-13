## 0x008C12D0

- engine/CTCBoss.h (full copy of the repo header): `FABLE_RETAIL_CTCBOSS_GETGENERICCOUNTER` adds the inline FableWin.pdb accessors ?GetGenericCounter@CTCBoss@@QBEJXZ / ?SetGenericCounter@CTCBoss@@QAEXJ@Z (row 0x008C12D0 reads GenericCounter +0x10 through it). Default path unchanged.
- engine/CCombatAction_GenericStrikeResponseBase.h (full copy): `FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_TYPED` types PThingThatHit (CIntelligentPointer<const CThing>, Ego_r.pdb) / PointOfHit / DirectionOfHit (C3DVector) and declares virtual Initialise (0x00865F40); same layout. Default path unchanged.
- engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h (full copy of the generated header): `FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSEJACKOFBLADESINTORUSH` gives the real class over CCombatAction_GenericStrikeResponseBase with virtual Initialise and CauseRush +0x148 (sizeof 0x14c). Default (generated) path unchanged.
- Repair 3 (2026-11-13): all three overlays re-based check vs the current repo headers -- `diff repo overlay | grep -c '^<'` = 0 for each
  (only the #ifdef blocks above are added). engine/CIntelligentPointer.h is NOT overlaid (the repo copy is used as is).
- (coordinator, at landing) CTCBoss.h overlay rebased by 3-way merge onto the current repo header: it keeps the SETPHASESTARTPOS block landed for 0x00972990 and adds only GETGENERICCOUNTER (the earlier '0 removed lines' claim was made before that landing).

## 0x00AD9F10 (not landed)

Overlay for row 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks (wf20). Each file = the CURRENT repo header + only the opt-ins below
(repair 3 pruned every dead attempt leftover; `diff rebuild/include/<f> hdr/<f> | grep -c '^<'` = 0 for all three).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL -- list operator= / pop_back declared only (out of line in retail, 0x00ADD020 / 0x00ADC790).
- stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE -- tree const find body visible but __declspec(noinline) (0x006699C0).
- engine/CVertexGroupBlock.h: FABLE_RETAIL_CVERTEXGROUPBLOCK_REMOVELAST -- RemoveLast(unsigned long) declaration (0x00AD9470).
- engine/CVertexGroupBlock.h: FABLE_RETAIL_CVERTEXGROUPBLOCK_ADDGROUP -- declares ?AddGroup@CVertexGroupBlock@@QAEXK@Z (landed 0x00AD91F0) so the row can carry InsertBlock's real body.
Removed in repair 3 (never defined by the final source): FABLE_RETAIL_STLP_LIST_POPBACK_NOINLINE, FABLE_RETAIL_STLP_LIST_ASSIGN_BODY,
FABLE_RETAIL_STLP_SORT2_BODY (+ _NOINLINE), FABLE_RETAIL_CVERTEXGROUPBLOCK_HASGROUP_DECL.
Repo opt-ins the row also defines (already in the shared headers): FABLE_RETAIL_STLP_VECTOR, _VECTOR_RESERVE, _VECTOR_PUSHBACK_NOINLINE,
FABLE_RETAIL_STLP_LIST, _LIST_PUSHBACK_NOINLINE, FABLE_RETAIL_STLP_SORT2, FABLE_RETAIL_CARRAY, FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS.

