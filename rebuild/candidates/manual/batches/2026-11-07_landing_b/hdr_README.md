# chunk 826 header overlay

One header, one new opt-in, default path textually unchanged (only `#ifdef` lines and a comment added; no existing line reflowed).

- `fable_string.h` (copied fresh from rebuild/include, rediffed before hand-back): new per-TU opt-in
  `FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY` puts `__forceinline` on `CCharString::GetLength` and `CCharString::IsEmpty`
  (same pattern as `FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ` / `_EQUALSCI`). Evidence: 0x00960120
  CAIStateGroup_DeedReactionRoot::Update tests `animation->IsEmpty()` in place at 0x00960376 (`mov ecx,[ecx] ; test ecx,ecx ; je ;
  mov esi,[ecx+4] ; test esi,esi ; jne`) where the plain inline body is called out of line in that function (md 269 -> 185).
  User: the parked 0x00960120 differ source only. The macro is new, so no landed row is affected
  (`grep -rlw FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY rebuild/src/compiled` is empty).

A trial opt-in in engine/CSpawnedFunc1.h (plain-inline ctor, 0x00ED5F50 g6) was worse and is not handed back.
