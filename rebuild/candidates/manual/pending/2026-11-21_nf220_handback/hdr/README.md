# NF220 overlay: fable_string.h (base: repo working tree 2026-11-21 = NF219's landed hdr version; orig/fable_string.h)

Add-only, member-specific opt-ins (all for row 0x0099C120, q12401). The default path is token-identical (cpp -P of
`#include "fable_string.h"` with and without FABLE_RETAIL_CWIDESTRING: orig and overlay outputs are byte-identical).
pin_asserts.py: pinned 0 (the header's 4 asserts are FABLE_STATIC_ASSERT_AT(565..568, ...) with explicit line keys, so the inserted lines move none). No landed row (grep rebuild/ tools/) defines
any of the new macros, so none of the 4,406 landed includers of fable_string.h changes. AUTH6's overlay touches only engine/GFAngles.h.

- FABLE_RETAIL_CWIDESTRING_COPYCTOR_INLINE (with FABLE_RETAIL_CWIDESTRING): the copy constructor defined in class (owner 0x0099B720;
  expanded in operator+ 0x0099C120). The pick-any copy it emits equals the owner (cmpcopy 66 = 66, bytes and relocs; the HEAD row
  emitted the same copy from its local model).
- FABLE_RETAIL_CWIDESTRING_DTOR_INLINE (with FABLE_RETAIL_CWIDESTRING): the destructor defined in class (owner 0x0099B510). Its pick-any
  copy (65 v 68) differed at HEAD too (the row's local model had the same body): HEAD debt, unchanged.
- FABLE_RETAIL_CWIDESTRING_ADDCHAR: declaration of operator+=(wchar_t) (Ego_r / FableWin ??YCWideString@@QAEABV0@_W@Z; landed 0x0099B9A0).
- FABLE_RETAIL_CWIDESTRING_MAKEOWNCOPYOFDATA: declaration of protected MakeOwnCopyOfData (both PDBs IAEXXZ; 0x0099B560), guarded
  against FABLE_RETAIL_CWIDESTRING_REPLACE_PASS24, which declares the same member.
- FABLE_RETAIL_CWIDESTRINGDATA_CTOR_TEXT: declaration of CWideStringData(const wchar_t*, long) (both PDBs ??0CWideStringData@@QAE@PB_WJ@Z).
- FABLE_RETAIL_CWIDESTRINGDATA_APPEND_CHAR: declaration of CWideStringData::Append(wchar_t) (both PDBs ?Append@CWideStringData@@QAEX_W@Z).
- FABLE_RETAIL_CBASICSTRING_TEXT_EQ_OWNER_BODY (needs FABLE_RETAIL_CBASICSTRING_COMPARE_BODY; q12383, rows 0x005A4CDD / 0x005A5B31): the
  owner 0x004115A0's 88-byte ??8?$CBasicString@D@@QBE_NPBD@Z as an explicit specialization under optimize("ty", on) ... optimize("", on)
  (size-pragma scaffolding: conditions and the condition-1 measurement in NOTES.md q12383). No landed row defines it; a row using it
  must not carry a row-head optimize pragma before its include of fable_string.h. Re-checked after this opt-in: the default path and
  the FABLE_RETAIL_CWIDESTRING + COMPARE_BODY path both preprocess byte-identically to orig/ (cpp -P), pin_asserts pinned 0.
