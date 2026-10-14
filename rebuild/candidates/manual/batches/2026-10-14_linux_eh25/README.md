# Batch `2026-10-14_linux_eh25` — hand-authored small EH-framed bodies: 13 bodies (13 landed)

Twenty-fifth hand pass (Wine lane, `/EHsc`): the `CListNode` destructor shared by the
0x12B4460 / 0x12B4268 / 0x12B42B0 / 0x12B42F8 container family (`CRoot{vptr; m_pNode}` plus an
embedded node), the `CTCDPhysicalObstruction` constructor (four such containers at +8 / +0x38 /
+0x68 / +0x98), the `CResourceArray` destructor, the shader-extension classifier
(`CString::Compare` against the three extension literals), a property adder and a checked
getter, two `GetResult` readers, `OnFinalized`, `QueryDamage` (12-byte POD assignment keeps a
fresh address register; a user `operator=` folds it), the 2D primitive renderer's lighting
setter, the AI state-group renderer initialiser and `CreateConsoleVariable` (this one matched
with retail's `or edi,-1` constant materialisation, unlike the parked eh23 pair).

Lessons kept: a temporary record passed by const reference is built while the arguments are
pushed (right-to-left); a POD member assignment of 12 bytes is three dword moves through a
fresh register. Parked: 0x00C16E10 / 0x00C3CD00 (DDK places the merged early-return block at the
last return site, retail at the first), 0x00CAADB0 (register spill choice: the DDK spills the
name, retail the count).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets30`.
