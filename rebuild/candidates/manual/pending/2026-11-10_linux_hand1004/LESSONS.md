# Chunk 1004 lessons

- `if (live) p.Reset(Create("PC_X")); else p.Reset(Create("UI_X"));` is the spelling of retail's cross-jumped `je ; push PC ; jmp ;
  push UI ; call`; `Create(live ? "PC_X" : "UI_X")` materialises the literal in a register first.
- A pointer-from-pointer cast that retail tests twice (`if (p) { q = cast(p); if (q) ... }`) is a nested `if`; the ternary form adds a
  null arm.
- An overlay copy of a repo header that another repo header includes by a sibling quoted path (`#include "fable_string.h"` from
  rebuild/include/fable_definition_table.h) is not seen through that includer: MSVC searches the includer's directory first and
  `#pragma once` is per path, so copy the includer into the overlay too (else: redefinition errors).
- c2prio.py is a free oracle for register-tie rows: compare the priorities of the two candidates that swap registers before spending a
  grade. When they are a few points apart and every natural spelling leaves them unchanged (0x005CA68E: `this` -72 v constant 0x48
  -76), park instead of probing.
- Rows that are dead COMDAT runs (no rel32 or absolute reference anywhere in the image) cannot be named from callers; scan the whole
  image once (`.text` rel32 + every section's dwords) before searching further, then author them as CUnproven shapes from a generator.
- `return other.Node == Node;` (not `Node == other.Node`) gives retail's `mov eax,[esp+4] ; mov eax,[eax] ; sub eax,[ecx] ; neg ; sbb ;
  inc` operand order for a one-word iterator `==`.
- Heredocs: write Python that contains backticks with a quoted delimiter (`<<'EOF'`); an unquoted one runs the backticks as commands
  and silently changes the text being matched.
- split_oracle.py's chunk number was '1001' in this chunk's copy; check `N` before trusting the oracle it writes.
