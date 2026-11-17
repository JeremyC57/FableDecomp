# e10 lessons (one line each)
- A same-TU inline that retail expands at one site and calls at another (CCard::Move in 0x008F0BD0) needs no inline control: define it plainly in Ego_d object order before the row and VC7.1's caller budget reproduces both sites.
- Defining the same-TU caller of an out-param helper (GetCardDef after the row, plain) plus the shared GetSubDef body made the CDefPointer stay in a callee-saved register with retail's `dec [reg+4]` release (0x008F0BD0, 0x006C14D0): escape analysis, not spelling.
- Implicit const char* -> CCharString arguments build the temporary BEFORE the other argument pushes and re-read its address; an explicit CCharString(...) is built at its push position (rows 0x006C14D0, 0x008F0BD0, 0x007C7C40).
- No-code grouping parentheses `(a * b) + c` closed a one-store scheduling swap in 0x008F0BD0 at the first try; apply them consistently to both copies of the expression.
- A one-register residue (PDef temp esi v ebp, 0x007C7C40) closed by computing an independent local (cur_stage) first in its block: statement order moves allocation priority.
- STLport push_back called out of line in retail: the declaration-only FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE matched (no inline-budget device needed).
- Ego_d LDATA32 entries in procmod.tsv give file-static names (mult_to_stay_off_table, DRAW_LINES_GROUND_HEIGHT_OFFSET); same-named statics in other TUs are separate variables (expected vland RELOC_CONFLICT).
- Ego_d LF_ENUM records carry enumerator names the shared headers call unproven (CTCTavernGame::ETavernGameStates).
