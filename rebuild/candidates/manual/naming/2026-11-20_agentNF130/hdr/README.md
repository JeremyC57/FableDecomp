# NF130 overlay (round 10, PRIORITY slice 7, fourth remainder)

NF130 adds 2 opt-ins (CModeStandBase.h, CThingCreatureBase.h). Base copies of the repo headers are in ../orig/engine/ (byte-equal to the
repo working tree at hand-back). NF128's CAnimFilterOfGroup.h / NCreatureMode_CModeStrafe.h opt-ins (SETCURRENTGROUPNAME_INLINE,
CMODESTRAFE_ONACTIVATE), used by rows/0073dff0.cpp and rows/008e9a30.cpp, were overlay copies until nf128 reached the repo working tree
mid-session; the copies were then byte-equal to the repo and were dropped (the final recheck ran without them). CThingCreatureBase.h is
now the repo header (which carries NF128's GETDEFHOVERSPEEDS) plus one NF130 block.

## engine/CModeStandBase.h (base: rebuild/include/engine/CModeStandBase.h, repo copy; orig/engine/CModeStandBase.h)
- NF130 `FABLE_RETAIL_CMODESTANDBASE_ANIMUPDATE` (inside the FABLE_RETAIL_CMODESTANDBASE class, after the DEACTIVATE block):
  `virtual void AnimUpdate(CAnimateThing&);` (only when FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE, which also declares it, is undefined;
  Ego_r 0x00817670 / FableWin 0x0283F510 ?AnimUpdate@CModeStandBase@@UAEXAAVCAnimateThing@@@Z), then `private:` UpdateIdleAnim (Ego_r 0x008164C0,
  retail 0x00862780) / UpdateWalkingAnim (Ego_r 0x008171F0 / FableWin 0x0283FC60, retail 0x00862AA0), then `public:` again. Declarations only,
  no layout change. Used by rows/00862ec0.cpp.
- Default path token-identical to the repo header (the block is the only change). pin_asserts.py: pinned 0 (the header's asserts were already _AT-pinned).
- Landed includers: 51 rows (`grep -rlE 'CModeStandBase.h' rebuild/src/compiled`); none defines the macro, so the opt-in changes none of them.

## engine/CThingCreatureBase.h (base: rebuild/include/engine/CThingCreatureBase.h, repo copy with NF128's GETDEFHOVERSPEEDS; orig/engine/)
- NF130 `FABLE_RETAIL_CTHINGCREATUREBASE_PEEKMOVEMENTVECTOR` (public, right before NF128's GETDEFHOVERSPEEDS block): inline
  `const C3DVector& PeekMovementVector() const { return MovementVector; }` (FableWin.pdb 0x01E46990 ?PeekMovementVector@CThingCreatureBase@@QBEABVC3DVector@@XZ;
  retail reads MovementVector +0x10c / +0x110 in place, Ego_r layout). No layout change. Used by rows/00862ec0.cpp.
- Default path token-identical to the repo header (the NF130 block is the only change). pin_asserts.py: pinned 0.
- Landed includers: 563 rows (`grep -rlE 'CThingCreatureBase.h' rebuild/src/compiled`); none defines either macro, so the opt-ins change none of them.

Sibling overlays re-listed at hand-back (agentNF127: CAIStateGroupBase.h, CAIStateGroup_PrepareMeal.h, CThingSearchTools.h; agentNF129:
CParsedItem.h, CStringParser.h, CSystemManager.h) carry neither header.
