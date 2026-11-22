# NF245 overlay headers

## engine/CStringParser.h (base: orig/engine/CStringParser.h = the repo working tree at start, 2026-11-22)
One add-only partial mode, `FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY` (nf243 ruling 2 (a), the 0x004046B0 family):
- line 183: `#else` -> `#elif !defined(FABLE_RETAIL_CSTRINGPARSER_COMMENT_BOUNDARIES_ONLY)` (the one edited existing line: with the macro
  undefined it selects the same branch as the `#else` did);
- at the end, a new `#else` branch (guard FABLE_ENGINE_CSTRINGPARSER_COMMENT_BOUNDARIES_H) that declares only
  `class CStringParserCommentBoundaries {public:const char* PStart;const char* PEnd;};`, token-identical to the full mode's line 12
  (Ego_r.pdb: size 8, char* PStart +0, char* PEnd +4). It is reached only with the macro defined and FABLE_RETAIL_CSTRINGPARSER undefined.
`diff orig hdr`: 1 line changed (the `#elif`), 10 lines added.
- Equivalence (work/eq/): equiv_check.py over the 579 landed rows whose source or test reaches CStringParser.h (7 headers in the
  include closure, funclet `#include "../src/compiled/..."` chains followed): 0 differ (equiv.tsv empty). Sanity: the same run with a
  deliberately changed class line flags 0x004A0D90 (a CStringParser.h user) and not 0x005DC550 (a non-user), so the tool sees the header.
- pin_asserts.py: pinned 0 (every assert is already FABLE_STATIC_ASSERT_AT with an explicit line).
- No landed includer defines the new macro; its users are the hand-back rows: 0x004046B0, 0x00404690, 0x004046F0 (+ their new
  tests), 0x004047A0, 0x004C88E0, 0x004C8A30, 0x00686E10, 0x009ADE90, 0x009AF1E0, 0x009E5AD0, 0x00A38E50.
No other header was changed (stlport/fable_containers.h untouched: every list opt-in used already exists).
