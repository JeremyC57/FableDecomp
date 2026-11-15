# 0x00B98E10 lessons

- S_FRAMEPROC HasInlineAssembly0x8 is decisive ORIGINAL compiler evidence. Module915flags0x28208, nativeframe12/savedregs12; record it before relying on opcode heuristics.
- PDB S_LABEL32 records retain original asm label names! Six labels match kernel-relative offsets with uniform+4 donor/native shift. Restore those names, not made-up labels.
- Decode LF_NESTTYPE / original member types: CClipListElement and CSIMDVariableBlock are nested in the manager; @1 mangling backrefs are easy to misread as namespace ownership. Clip fields Start/Size are original.
- A fully opaque SSE block forces natural C++ mask homes/this spill; unmodified plain C++ outside that block reproduced205bytes/79instructions exactly. Never rewrite those C++ flanks as asm.
- Destructive ANDs in retail really discard earlier pass nibbles; preserve this actual culling behaviour in the twin and independent oracle.
- NaN SIGN is game logic when movmsk supplies rejection bits. Initial stock scalar twin failed676fuzz cases; explicit ordered binary32/first-NaN quieting closed allcases and196971operation patterns. Do not dismiss it as payload-only.
- Assert pinning must distinguish repeated identical expressions in different #if branches. Every old occurrence retains its OWN old line, not a dictionary's last line.16old profiles match and20pins are checked.
