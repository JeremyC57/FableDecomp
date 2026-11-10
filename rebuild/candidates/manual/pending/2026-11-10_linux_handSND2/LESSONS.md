# agentSND2 lessons (GOTCHAS-ready)

- **Frame packing follows block scope.** At 0x005A94BD, retail builds the SetInventoryAnimRenderBox C2DBoxF temporary at [ebp-34h], over the buy / all-items vectors, while the sell vector keeps [ebp-40h]. Putting the vectors after the first one in a nested `{ }` gives every [ebp-x] slot as retail and frame 0x40; at function scope the box gets 16 bytes of its own (frame 0x44).
- **A `?:` call argument versus two calls.** Retail's `push button ; jle ; push value ; jmp` / `push 0` into one cross-jumped `call` is `if (c) F(a, b); else F(0, b);`. The `F(c ? a : 0, b)` spelling computes the value first (`jg ; xor eax,eax`) and also changes the register webs.
  - 0x005F6793: `this` went from ebx to edi, the ebx push was shrink-wrapped, and the diff fell from 92 to 21 lines.
  - 0x005F64DD: the size went from 512 to 508, retail's length, with the def copy's AddRef null test shared.
- **Use the real callee body, not a stand-in.** The real body of a same-TU callee (0x005EED8C GetPrioritisedExpressionDefs, appending through the header's inline push_back) gives c2 the same no-capture knowledge as an invented `swap(Find...())` stand-in, with identical bytes. Prefer the real body.
- **c2prio `--obj` is a free compile.** It writes an object byte-identical to the grade's. Diff it against the oracle (`pr.sh` / `objcmp.py` here) before spending a grade; it measured every job-3 variant here.
- **Check symbols, not only the grade.** An `#include "engine/CASoundManager.h"` that lands inside an open `namespace NUISystem {` still grades RELOCATION_MATCH, because relocations are masked, but it references `?PSoundManager@NUISystem@@...` (0x00557D20). Check `objdump -t` for the global's symbol.
- **A slot-renaming migration can rename the row itself.** Replacing a local manager slot name throughout the file also renamed the row's own method: 0x006B1900 CAtmosProcess::MixAtmos became SetChannelGain. Rename only inside the local model's body.
- **One unexplained loop residue in both trade rows.** The first erase loop of the normal-shop `else` arm, reached by a `je`, gets a duplicated pre-test (`cmp ; je`) in our build. Retail enters it with `mov reg,[begin] ; jmp bottom-test` (0x005A94BD, 0x005ACEDA). These do not change it:
  - `while` instead of `for`.
  - Declaring the iterator outside the loop.
  - Swapping the two loops: the first one is always the pre-tested one.
