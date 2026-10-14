# Batch `2026-10-14_linux_eh13` — hand-authored small EH-framed bodies: 10 bodies (10 landed)

Thirteenth hand pass (Wine lane, `/EHsc`): the DirectInput device release of `CInputManagerDX`
(IAT-called imports under the scoped lock), `CPhysicsMeshObject::HasContainmentVolume`, the
`CSeaQuadElement` constructor (two three-dword link members zeroed, then registered with the
owner), the `CEditControlCentre` find and set-mode bodies, the `CScriptNode` destructor whose pair
member is destroyed inline (second at state 1, first at state 0), `CGuiControlTree::SelectNextEntryByName`,
`CAudioManager::Shutdown` (critical section held through the dllimport Enter/Leave pair, state
conditioned on the member at +0x64), and the two `GetLeafDataFromWindow` bodies calling a
`__fastcall` helper. Lesson kept: `extern "C"` imports cannot throw under `/EHsc`, so no EH state
change is emitted around them.

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets17`.
