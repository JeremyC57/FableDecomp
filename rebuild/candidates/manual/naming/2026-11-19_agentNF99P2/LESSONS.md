# NF99P2 lessons (one line each)
- Before porting an owner, compute its split-free closure from the SDK relocations of every byte-identical body, then count it: here eight owners pulled in the whole reverb engine (about 27 rows), so the closed Translate family went first.
- reloc_bindings.tsv lists one first caller per (symbol, target): scan the exe for E8 sites of every renamed address (0x00C54190 binds Commit and was in no table).
- When the SDK release body of a header-inline helper (ctor / dtor) is in the .lib as a COMDAT, read it for the store order before writing the opt-in (CReverbConstants: 48000 at +8 before 2 at +4).
- A same-file sibling already on the header (0x00C6B050 Create) is the template for the next port: Commit matched first time by copying its Translate / base-pointer Commit / decay tail.
- SDK data symbols with an undecorated `_sc_f...` name bind from C++ only as `extern "C"` declarations.
- Get RENAMES old symbols by compiling the landed sources without grading (work/tools/oldsyms.py); objdump marks external symbols `(nx 0)`, so match both nx values.
