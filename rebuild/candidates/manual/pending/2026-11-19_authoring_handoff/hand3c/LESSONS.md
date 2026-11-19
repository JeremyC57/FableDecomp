- CPSC* particle rows parked as 'N bare dword fistp sites': FableWin /Od shows ?GFPackFloat@@YIKMKMMK@Z / GFUnpackFloat calls -- use shared engine/GFPackFloat.h (+ FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT as the landed CPSC rows); three ctors matched in 1-3 grades (hand3c).
- 'LTCG convention' callee taking ecx + caller-cleaned stack in libpk: a same-TU static copy of the kernel (VC7.1 private convention for statics); put the kernel body in the row TU (0x00C6AC90).
- x87 commutative operand-order residues: check FableWin for an accessor (operator()(r,c) returning float&) -- writing through the PDB accessor changes VC7.1's fld/fmul operand order (0x00A9D480).
- (0x00A9D750) FableWin opens every `for` with `xor reg,reg ; je <body> ; jmp <after>`. This is the VC6 for-scope idiom,
  `#define for if (0) {} else for`, and Ego_d gives each loop variable its own scope. Writing `if (false) {} else for (...)` emits
  no bytes, but it changes c2 record ids. It closed a one-pair x87 operand swap that no dead-local ruler (k = 1..8 at three
  positions) could close. Look for the `xor/je` pair in the FableWin body of any row with an x87 commutation residue.
  It had no effect on 0x00989230, whose residue sits in c2's unrolled copies.
- (0x00C1C330) The x87 term order of a sum written as ONE expression is reassociated by VC7.1, ignoring written order and parentheses.
  A STEPWISE accumulation (`r = a; r += b; ...`) keeps its order. A one-instruction store-schedule residue after that can be read off
  the c2 DAG (tools/sched/t20.py + sim.py): search single-node removals and additions in the sim, then find the spelling that removes that
  node. Here a stack-copy pseudo was dropped by merging the last two `+=` into `r = r + c + T`.
- OWNER RULE 2026-11-19: grouping brackets (PAREN tuples) are an accepted lever in row sources and helper text. A helper bracket goes
  under a NEW nested opt-in, with a regrade of every landed user. Mark it with one `// Bracket:` line (address fixed, other placements
  measured). Extra locals, references and by-value params stay `// Inferred:`. Try brackets early on scheduling-order residues.
- (0x006B2960) A const-ref CCharString temporary is constructed only after the other arguments load. A named local in an inner
  block constructs it first and still shares the dead-parameter frame slot with inline-helper temps.
- (0x0052EE00 / 0x007F8C20 / 0x00664BA0) Parked "register colouring" drafts on local class models often close in 1-3 grades on
  the shared headers in the FableWin statement order. Try that first.
- (0x00C40590) Rows in 0x00C3xxxx..0x00C8xxxx are GameCODA 2.10: byte-search CODAApi.lib before any operand-order chase. The SDK's
  Toolkit source (CtkVector.h etc.) carries the header-inline bodies whose structure fixes VC7.1's x87 operand order; 80 grades of
  spelling sweeps on a made-up model could not.
