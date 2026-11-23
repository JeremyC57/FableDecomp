# NF319 lessons (2026-11-23, alias rows whose member the class header does not declare)
- Triage the header shape before drafting anything: 89 of 190 entries had only the generated flat model (`void* __vftable`, no `virtual`);
  no add-only declaration can sit at an RTTI slot there. A script census settles them in minutes (tools/tri2.py, tools/flatcensus.py).
- objsec cannot see the slot condition (no vftable is emitted). A clang probe settles it: the row's preamble WITHOUT the new opt-in plus
  `struct P : X { <PDB decl> override; };` must pass, and a made-up name must fail (tools/probe.py). It caught 4 entries whose base model
  lacks the virtual (InitAndCompile, two Init, IsNull) and two rows that needed the base switch (CJOYSTICK_VIRTUALS, CAISTATEGROUPBASE_MEMBERS).
- A precedent's whole define block compiles but drags in opt-ins with their own prerequisites (COVERLIST needs std / CThing); start from it,
  then drop lines greedily under clang + cpp + probe (tools/minimize.py). The result is small, and every remaining define is a landed switch.
- The demangled PDB head loses parameter names; keep the HEAD row's names (bodies use them) and take only the types from the PDB.
- llvm-undname reads nothing from stdin without a trailing newline.
- Rows without a `// Leaf:` line grade by the bare catalog name; add the full mangled Leaf before the final grade (GOTCHAS 479).
- An `int` stand-in return that the PDB spells `bool` can be a real width difference: 0x00934100 graded DIFFER(22v25) (NF317 Q4).
- Never `rm -f` even a scratch file in your own folder: the harness refuses it; overwrite or use clean.py.
- ps etimes of a nohup'd job can read short; trust the done files (`work/rc_<tag>.done`) and `kill -0`.
