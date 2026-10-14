# Batch `2026-10-14_linux_eh8` — hand-authored small EH-framed bodies: 11 bodies (11 landed)

Eighth hand pass (Wine lane, `/EHsc`): two COM-style releases (`mov ecx,[obj] ; push obj ;
call [ecx+8]` is `__stdcall` slot 2 = `Release` with `this` pushed -- one on an embedded interface
object of the owner, one on a pointer member that is then nulled), the `WaypointData` destructor
(three container members, EH state counting down with a byte-narrowed store), `CArenaRoundDef`'s
constructor (an array of four behaviour elements through `__ehvec_ctor`), the `CVsyncCallbackTimer`
destructor (a member cleared then destroyed, a polymorphic member's inline destructor), and six
scoped-lock bodies (the `_i1117` shape with the body a call on `this`, on a member pointer, on a
member pointer with `this`, and a virtual call whose result survives the lock in esi).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets12`.
