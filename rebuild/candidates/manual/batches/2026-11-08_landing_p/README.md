# Landing 2026-11-08p: LHNetworkLib::LHHttpHeaders::GetHeader (chunk 865 follow-up)

0x0083F650: RELOCATION_MATCH + PASS (443 bytes). Read-only vland with a trimmed overlay, then `--land`.

- **Inline budget:** per-row flag `/d2inlS11` with the source untouched. At the default S9, VC7.1 expands the STLport range initialise at
  all three sites (561 bytes). Retail calls it at `return line`. S11..S20 and T60 match.
- **Held once, then fixed:** the first match redeclared its own STL. The landed source now holds only the function body on the shared
  headers. Its leaf is the full PDB name `?GetHeader@LHHttpHeaders@LHNetworkLib@@QAE?AV?$basic_string@...@std@@ABV34@@Z`: the agent
  handed back a prefix, which was completed at landing and graded again.
- **Headers landed** (additive opt-ins, no removed lines, no other user):
  - stlport/fable_string.h STLP_STRING_FIND: char_traits eq, _Eq_traits, `search` (0x00567120), the `__find_if` body under
    `auto_inline(off)` (0x00841900, which must be visible so the two dispatch-tag temporaries share the result slot), and
    basic_string::find / find_first_not_of.
  - fable_string.h STLP_STRING_THROW_INLINE: the length / range throws defined in class. Do not combine it with
    fable_string_bodies.h.
  - engine/LHHttp2.h LHHTTP2_GETHEADER: the LHHttpHeaders::GetHeader and LHHttp2::GetHeader declarations.
- **Extent:** the catalog cut was 442 bytes. VC7.1 owns the int3 after the trailing noreturn `call abort`, so the row is 443 bytes
  (BOUNDS_FIX.tsv `len`).
- The landed sibling 0x00840B60 LHHttp2::GetHeader still uses the old local STL model. On the shared headers it is one 4-byte frame slot
  away (`sub esp,0x48` v `0x4c`; likely the visible `_M_insert_overflow` in vector::push_back). See the pending NOTES.
- **Names:** labels were applied in landing o (`labels_landing_2026-11-08o.tsv`).
