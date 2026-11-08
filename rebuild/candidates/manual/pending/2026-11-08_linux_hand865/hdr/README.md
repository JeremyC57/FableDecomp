# hdr/ -- header overlay for chunk 865 (searched before rebuild/include via FABLE_HDR_OVERLAY)

Every file is a copy of the rebuild/include file of the same path plus ADDITIVE `#ifdef FABLE_RETAIL_*` branches only
(`diff rebuild/include/<f> hdr/<f>` shows `>` lines and no `<` lines; the default-path preprocessed text is identical up to the
blank lines the opt-in blocks leave). Nothing is landed from here by the agent; the coordinator merges at landing.

## Files added in the 0x0083F650 follow-up (shared-header rewrite of LHHttpHeaders::GetHeader)

- `stlport/fable_string.h` (+91 lines, 0 removed). Two opt-ins:
  - `FABLE_RETAIL_STLP_STRING_FIND` -- STLport 4.6.2 `find` / `find_first_not_of` for `basic_string`, as retail compiled them:
    `char_traits<char>::eq`; `_Eq_traits` (on fable_containers.h's `binary_function`), `_Eq_char_bound`, the `search<const char*,
    const char*, _Eq_traits>` DECLARATION (`__fastcall`, retail 0x00567120, out of line); the `__find_if<_RandomAccessIter,
    _Predicate>` BODY (STLport's four-way unrolled loop + switch, `__fastcall`, between `#pragma auto_inline(off)` / `(on)`; retail
    0x00841900, landed as std_findif_00841900.cpp) -- the body must be visible in the TU because c2 then lets the two 1-byte
    dispatch-tag temporaries of the row share the result-pointer argument slot as retail does (research861 2.4; with a declaration
    only the second tag gets its own home and the row is one `lea` off); `find_if` (random_access tag dispatch); `_Not_within_traits`;
    in `basic_string`: `find(const C*, pos)`, `find(const C*, pos, n)` (via `search`), `find_first_not_of(const C*, pos)`,
    `find_first_not_of(const C*, pos, n)` (via `find_if`).
  - `FABLE_RETAIL_STLP_STRING_THROW_INLINE` -- `_String_base::_M_throw_length_error` / `_M_throw_out_of_range` DEFINED in the class
    (`puts("basic_string"); abort();`) instead of declared: retail expanded the length error inline in this TU (the
    `push 0x0124C5B0 "basic_string"; call puts; call abort` tail at 0x0083F7FE..0x0083F809). Do NOT include
    `stlport/fable_string_bodies.h` with this opt-in (it defines the two out of line).
- `stlport/fable_containers.h` -- UNCHANGED copy. Needed only because `fable_string.h` includes it by bare name and MSVC resolves a
  quoted include relative to the including file's directory first, so the overlay copy of fable_string.h would otherwise fail with
  C1083.
- `engine/LHHttp2.h` (+9 lines, 0 removed). Nested opt-in `FABLE_RETAIL_LHHTTP2_GETHEADER` (inside the existing
  `FABLE_RETAIL_LHHTTP2` branch) declares `std::string LHHttpHeaders::GetHeader(const std::string&)` (0x0083F650, Ego_r.pdb
  `?GetHeader@LHHttpHeaders@LHNetworkLib@@QAE?AV?$basic_string@...@std@@ABV34@@Z`) and the landed sibling
  `LH_HTTP_STATUS LHHttp2::GetHeader(LHHttpHeaderStatus&)` (0x00840B60, declared for the carriability probe in probe/; harmless,
  drop it at landing if unwanted).
- `engine/LHNetworkTypes.h` -- UNCHANGED copy, same reason as fable_containers.h (LHHttp2.h includes it by bare name).

## Files carried over from earlier chunks (unchanged from those hand-offs, used by the landed rows of this chunk)

- `engine/CQ_BountyHuntScript.h` (chunk 833 opt-ins; 0x00D17F20, 0x00D19BD0, parked 0x00D18DF0)
- `engine/CV_ExposeMayorScript.h` (chunk 836 via 856; 0x00E799F0)
- `engine/CQ_BanditCampBossBattleScript.h` (chunk 852; 0x00D067A0)
- `engine/NUISystem_CComponent.h` (chunk 862; parked 0x00531EC0)

No includer of these headers is affected unless it defines the opt-in macro.
