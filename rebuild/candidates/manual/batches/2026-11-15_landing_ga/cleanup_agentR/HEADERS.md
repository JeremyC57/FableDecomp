# agentR (w22) header overlay -- two shared headers, each change behind an opt-in; default paths unchanged

| header | change | users |
|---|---|---|
| engine/CIntelligentPointer.h | NEW opt-in `FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_ASSERTED_BODY` (+ its `#include "fable_assert.h"` at the top, inside the same `#ifdef`): `operator->` = FableWin's debug body with its one assert (`CBaseObject* item = GetPItem(); T* pointer = static_cast<T*>(item); FABLE_RETAIL_ASSERT(pointer); return pointer;`). The old `#ifdef FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_CHECKED_LOCALS` becomes `#elif defined(...)` with its text unchanged (reserved acctC drafts wfB2 / wfB2p / wfE9 / wfE9fix carry it in their overlays). | 0x00652FA4 (switches from ARROW_CHECKED_LOCALS to the new opt-in). No other landed row defines either macro. |
| fable_string.h | `FABLE_RETAIL_CCHARSTRING_STARTSWITH_ASSERT` (existing opt-in): the dead arm's condition `PStringData != 0` -> `prefix.PStringData != 0` (non-empty prefix), comment rewritten in place with the same line count (the arm keeps its line, `__LINE__` unchanged; the file's static asserts are pinned anyway). | 0x00CBFB7D only (grep of rebuild/src, rebuild/tests and every pending folder: no other user). |

Proof: `coordinator/hdrcheck2.py hdr/` = default same and every single opt-in same for both headers; the combinations
`FABLE_RETAIL_CINTELLIGENTPOINTER` + `_ARROW_CHECKED_LOCALS` / + `_GET_ORIGINAL_CHAIN` preprocess identically (gcc cpp, REPORT.md);
the StartsWith text differs only under `FABLE_RETAIL_ASSERT_DEAD_ARM`, which only 0x00CBFB7D defines.

Testing note: `../hdr_test/` = this overlay plus UNCHANGED copies of the root-level repo headers that include `fable_string.h` by
bare name (fable_definition_table.h, fable_game_text_bank.h, ...). An overlay copy of a root header needs them, or MSVC resolves the
bare include next to the includer and pulls the repo copy twice (C2011). They are not part of the change; land only hdrdiff/*.diff.
