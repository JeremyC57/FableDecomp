# NF153 lessons (one line each)
- A census of a widely called accessor by compiling every referencing row (tools/probe.py, ~900 rows in ~15 min) finds wrong binders the binding table cannot: only 27 references in 26 rows (0x00935160 twice) of ~1,200 were wrong, and four WORKLIST binders were already right.
- The reloc probe silently skips multi-block TUs (RELOC_UNCHECKED: switch tables); diff `objdump -t` undefined symbols for rows whose binding comes back empty (0x005E3EA0 bound `QBEJXZ` from `typedef long EPlayerMode`).
- Const overloads: including the shared CPlayerManager.h on a non-const receiver binds the non-const QAEAAV spelling at the const address 0x00449960; the faithful fix is the const accessor retail called just before (CTCBase / CAIStateGroupBase::PeekPlayerManager, 0x00686D50 / 0x008FCF80), which also heals those callees' stand-in spellings.
- The mirror case exists: a row on the const PeekPlayerManager() bound the const spelling at the non-const 0x00449970 (0x005AF64E); the inline non-const GetPlayerManager() opt-in fixes it.
- Diff the full before/after (symbol, target) pair list and the defined-symbol list per row, not only the entry target: it shows every callee the port moves.
- An existing out-of-line body in the catalog can disprove a candidate form for free: CThing::PeekPlayer 0x004C7A10 is byte-for-byte the DIFFER shape of 0x004C7CA0, so the PeekPlayer() form was not graded.
- reloc_bindings.tsv can be mid-write while the coordinator lands (a fused 5-field line at 09:39:53 made binds.py throw once); rerun before trusting an empty pair list.
