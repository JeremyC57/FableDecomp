# Batch `2026-10-14_linux_eh24` — hand-authored small EH-framed bodies: 8 bodies (8 landed)

Twenty-fourth hand pass (Wine lane, `/EHsc`), mostly the "null-object warning" family resolved
in eh23: the `CMasterObject` destructor (`If it was not finalised, then how did I get here?`),
a checked `Get` and a state-group getter/creator, the thread-manager pump (lock scoped to the
inner block of the wait loop so the relock needs no `-1` store), the user-reply writer, the
`ReleaseFileObject` global (`delete` through a virtual destructor emits its own null test), the
MFC `CString::GetFileNameWithoutExtension` (`ReverseFind` / `Left` / `Right` chain) and the
landscape-patch task allocator (`new` temporary homed in a dead argument slot,
`return result ? value : 0` as `neg; sbb; and`).

Lessons kept: the warning block's `CString` must be EH-tracked in its own scope or the DDK will
not home it in the slot retail uses; `if (!(count > 0))` on an unsigned count is `test; ja`.
Parked: 0x00C0E1F0 (the DDK refuses to home the tracked `CString` in the argument slot the
trace temporary also uses), 0x00C32060 (hoisted state store / esi reuse).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets29`.
