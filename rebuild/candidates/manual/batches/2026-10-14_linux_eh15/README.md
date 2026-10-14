# Batch `2026-10-14_linux_eh15` — hand-authored small EH-framed bodies: 9 bodies (9 landed)

Fifteenth hand pass (Wine lane, `/EHsc`): a sprite update whose float result is parked in the frame
and pushed as a dword (a named local reproduces the `fstp [esp+4] ; mov eax,[esp+4]` pair), the
`CInventory` observer notification (virtual slot 4 with `this` when the lookup succeeds), a
`CClothingSuitPartMap` query through virtual slot 15 of a node's embedded member, the `CKrakenData`
destructor (`__ehvec_dtor` over two class arrays, no own vtable so `__declspec(novtable)`), a
five-argument UI component constructor registering with its owner (virtual slot 9), the
`CScriptDefinitionManager` destructor (pair member destroyed inline, then an out-of-line member
destructor), a three-argument factory (`new CObject(this, b, c, a)`), `Advance` under the lock
(step, two member updates, frame counter incremented), and the `CLandscapeBackgroundTreeNode`
constructor (slot built at state 0, two trivially destructible pointers at state 1, task nulled in
the body).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets19`.
