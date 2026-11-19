# Landing hand4a b6 (2026-11-19): NUISystem::CList::CanMove (0x00537776)

vland --land, strict headers: RELOCATION_MATCH + PASS. FableWin body 0x0219F54D with Ego_d locals; the draft's made-up fields mapped to their
real members (ViewArea via LoadAndReturnDef, IsUsingRelativePosition, RelativeRenderPosition, PInitialPositionDummy, view_area_offset,
TargetPosition.Y). Existing shared FABLE_RETAIL_C2DVECTOR_ADD_COPY_RHS_X_PASS23 (landed 0x0064A639; the plain sum swaps the X addends).
New opt-ins in C2DBoxI.h: SETBR (inline SetBRX / SetBRY, FableWin 0x02185DB0 / 0x02185DD0) and COPY_FORCEINLINE -- the user copy ctor is
sourced (FableWin out-of-line ??0C2DBoxI@@QAE@ABV0@@Z 0x018B1BF0), forcing it inline in this size-optimised TU is an inline-budget device
(plain inline: called out of line, 478v480; implicit copy: movsd block, 484v480), marked in the source. All 76 landed C2DBoxI.h users rechecked.
