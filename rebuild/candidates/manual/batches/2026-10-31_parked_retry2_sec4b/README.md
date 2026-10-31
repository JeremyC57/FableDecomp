# Parked-row retry sweep, wave 2 section 4b (2026-10-31): 2 of 8 section-4 skeletons landed, 6 fully drafted

Section 4b = the eight 1.1-1.8 KB section-4 rows whose parked files were 1-5 byte skeletons or empty stubs. Each got a complete
first reconstruction from the disassembly. Landed 2 (RELOCATION_MATCH + PASS): 0x008BC1A0 CCreatureAction_CastWillRaiseDeadSpell::
PerformAction (1083 bytes, moved onto engine/CDefClassBase.h + CDefPointer.h) and 0x008C6DB0 CCreatureAction_IceTrollIcicleAttack::
PerformAction (1090 bytes; 194 → 12 diff lines → match over nine grades). The other six are in `../../pending/2026-10-31_parked_retry2_sec4b/differ/`
with grade progressions in STATUS.tsv: 0x008D5AA0 (66 lines; frame 0x44 vs 0x40, ReleaseMissile out-slot order), 0x008DCC80 (72;
x87 spill order), 0x008DE7A0 (25, 3 masked relocations; trap-spawn register choice), 0x008DE080 (52; entry register copy, in-place
normalise with fxch), 0x008E8060 (298; frame 0x1A0 vs 0x190, 0 held in ebx), 0x008DD0F0 (353; same residue; true length 1513 —
BOUNDS_FIX row, catalog row had no length).
Levers (graded improvements): block-scoped locals so stack slots are shared 5; GetInterface shape 4; C3DVector ctor-form
operators / operand order 3; same-TU GetSubDef<T> body on the shared CDefPointer 2; repeating an expression so VC7.1 reuses the
value (fpatan, fps) 2; pointer local in a callee-saved register 2; one each: separate `+=` statements for lengthSquared, one float
variable reused, user copy ctor on a by-value functor, `partition` as a `__forceinline` wrapper, `operator[]` through `begin()`,
IS_THING_ALIVE as a nested-if template, extern epsilon global instead of a literal, move onto shared headers.
NAME_FIXES.tsv: 15 rows (RiseUp ctor by reference, PeekThingSearchTools, NCreatureHiding::GetValidPositions, NCreatureMode::
IsUnconscious __fastcall free function, GetSubDef<CJackOfBladesBattleDef>, CModeBase::GetWorld, GetAllThings<...,CThingFilter_IsNot>,
C3DVector::AccurateNormalise, CTCDExplosiveTrail::AddThingToIgnore, ReleaseMissile private, CTCCarrying::GetThingInPrimarySlot,
SetDecapitate, CTCMultiArrow::ReleaseShot, NThingShot::CreateShot CThing& overload; 0x00787C1B IsOpen inferred from ONE source —
not to be used until a second) + 2 review rows (three RELOC_NAME labels; the folded 8-byte vector::reserve body 0x0077E580).
Review notes: SHARED_HEADER_AVAILABLE on the two landed rows (CCreatureActionBase, CBaseObject, CTCPhysicsBase, CDefinitionManager,
CAnimComponentCombatHandedness); an early `/FAs` compile wrote a stray .asm into the repo root, moved out at once (none remains;
the agent's fa.sh now passes an explicit output path).
Coordinator: vland --land 2/2, bindings_multi_check 0.
