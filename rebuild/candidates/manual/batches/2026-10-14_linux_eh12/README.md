# Batch `2026-10-14_linux_eh12` — hand-authored small EH-framed bodies: 11 bodies (11 landed)

Twelfth hand pass (Wine lane, `/EHsc`): the `CEditControlCentre` add-under-lock setter, the
`CConstraintRepeat` destructor unregistering from two registries, `CAInputProcess` and
`CLandscapeBackgroundTreeNode` destructors deleting an owned object through its virtual destructor
(`push 1 ; call [vtable]`), `CJoystickDX`'s destructor (block freed, COM device released through
the `__stdcall` slot-2 release with `this` pushed), a float query returned across the lock's
destructor, two `CHTE_Executioner` virtual slot-13 queries on a polymorphic member, a one-argument
factory and a two-argument transfer, and `GetDamageValue` (virtual slot 5 on the argument, then on
the result). Lesson kept: a COM `mov ecx,[obj] ; push obj ; call [ecx+8]` is a `__stdcall` slot 2
Release with the object pushed, modelled as an interface pointer member.

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets16`.
