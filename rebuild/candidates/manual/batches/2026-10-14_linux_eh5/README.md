# Batch `2026-10-14_linux_eh5` — hand-authored small EH-framed bodies: 7 bodies (7 landed)

Fifth hand pass (Wine lane, `/EHsc`): a two-level container constructor with a trailing byte flag
(`CThingGraphicByPassFlagState`), `CActionParamString(const char*)` (EH state 0 marks a polymorphic
base with a trivial constructor; the derived class zeroes three members and calls an out-of-line
Set), `CTextSlider::OnDeactivate` (a stack visitor handed to a member's virtual slot 10 between a
helper call and a flag clear), a pair-node destructor whose second is a block holder freeing its
block inline, a three-member constructor over the typed containers of `_i1130` (the second EH
state is stored as a byte: VC narrows the store when the slot's upper bytes are known zero), and
two `novtable` destructors over a chunk member whose inline destructor runs the shared chunk base
destructor (the `DtorToBase` shape).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets9`.
