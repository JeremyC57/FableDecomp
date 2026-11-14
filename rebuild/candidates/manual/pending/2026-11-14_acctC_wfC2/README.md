# acctC workflow hand-back wfC2 (big rows, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00F0FB70 | NScript::CQ_ArenaScript::Main | 18,144 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). Matches on the repo headers alone (no header change needed).
REVIEW ITEMS:
- Inline-budget device: scoped `#pragma inline_depth(0)` ... `#pragma inline_depth()` (inside `#ifdef _MSC_VER`, as landed
  0x00D62480) around the final won cut-scene block, marked on the `// Inline-budget:` line. Without it DIFFER(18196v18144),
  4 extra calls, nothing else. Flags (/d2inlT 0..200, /d2inlS 0..40, /Ob1) never reach 18144; the scope is the smallest that
  matches. No natural cause found. Owner test: byte match yes, behaviour same yes, portable yes (pragma under _MSC_VER).
- FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR is needed here while seven landed later functions of the same
  TU use the plain ctor: one of the two header texts is fitted.
- Hero-title switch: case labels are numbers in retail's table order (0x00F14200); no source has the EHeroTitle enumerator
  names. (An earlier draft had the table permuted: the byte grader masks relocations, vland caught it.)
- Labels (LABELS.tsv / NAME_FIXES.tsv): 0x00F14250 = CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>::GetMasterData;
  0x009AC310 = std::map<CCharString,CCharString>::~map; 0x00CBE09A has two spellings in reloc_bindings.
- The test is a standalone state model of Main (does not link the row source), as the lane's other script tests.
