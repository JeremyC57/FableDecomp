# NF215 header overlay

## fable_string.h (copy of the repo working tree, 2026-11-21; base in ../orig/fable_string.h, byte-equal to the repo at hand-back)
- Adds one add-only, member-specific opt-in in the CBasicString<char>::operator==(const T*) #if chain, between
  FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE and FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE:
  FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO -- `__declspec(noinline) bool operator==(const T* text) const { return Compare(PData,text)==0; }`
  (the NOINLINE form spelled as the owner 0x004115A0 is; with FABLE_RETAIL_CBASICSTRING_COMPARE_BODY the pick-any copy a calling row emits is the
  owner's 88-byte body, measured equal in bytes and relocation targets on 0x006238A0 and 0x004AFE20; plain NOINLINE gives a 76-byte copy).
- Users in rows/: 0x006238A0, 0x004AFE20 (both include fable_string.h themselves, first in their include order).
- Default path token-identical (the new branch is reached only when the new macro is defined; no other line changed).
- pin_asserts.py: pinned 0 (the four asserts at the end were already FABLE_STATIC_ASSERT_AT).
- Header users: 4241 landed rows include "fable_string.h"; none defines FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO (grep of
  rebuild/src/compiled and rebuild/include: 0 hits), so the opt-in changes none of them.
- No AUTH1 / AUTH2 overlay ($S/w22/auth1/<addr>/hdr, auth2/<addr>/hdr) carries fable_string.h (checked 2026-11-21).
