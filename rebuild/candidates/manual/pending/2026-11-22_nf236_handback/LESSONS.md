# NF236 lessons (one line each)
- reloc_bindings keeps one witness per symbol: a "2 -> 1" claim from the table is a guess; compile every caller (bindat over callers.py rows) before scoping a respell (0x00687BD0 had seven QAE binders, not one).
- A held "split growth" line can be stale on the other side: the old line's witness may already bind the new spelling (0x00C954A0 binds @_book_unquantize@12); grade the old witness before planning a respell.
- Rename identifiers on code lines only: a blanket token rename also rewrote member names quoted in comments (PDisplayManager->PRenderManager); ren.py skips full-line comments.
- A held global with three typed stand-ins (bool / unsigned char / char) still respells to one bool: the byte test is the same instruction, so check the grade before assuming a type change costs bytes.
- Header tests on a class with private members: seed and read the object at the header's documented offsets in a raw buffer (no `#define private public`), and stub only what the linker names.
