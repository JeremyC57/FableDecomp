# Landing 2026-11-09e: chunk 892, the scheduling / test-or playbook family (24 rows)

The first one-family-at-a-time playbook push (PLAYBOOK_CANDIDATES.tsv, family `sched`): 27 parked rows whose residue was
instruction scheduling, x87 order or commutative `test`/`or` operand order. 24 match: 22 RELOCATION_MATCH + 2 MATCH, all PASS
under VC7.1 and g++ -std=c++03; 0 bare `__forceinline`. Three stay parked (pending/2026-11-09_linux_hand892/).

Levers (each row's source comments name the retail addresses; LESSONS.md in the hand-back has the rules):
- No-code PAREN tuples (c2 counts a parenthesised float sub-expression toward the 81-tuple scheduling region): 0x004BC800,
  0x00807250, 0x008FC430, 0x00BE1C10, 0x00963880 and 0x0096E830 (new opt-in FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN).
- No-capture knowledge from same-TU bodies (the callee's body visible, kept out of line): 0x008E8FF0, 0x0052EAF0, 0x00BEEDA0,
  0x0076E4D0, 0x00783850, 0x008F99A0, 0x005A887D, 0x00B21A50, 0x0090BC60; 0x00808AA0 uses INLINE (COMDAT) comparator bodies,
  which give no-capture without the callees' register usage.
- Commutative operand order follows candidate ids (single-use named locals are forwarded late): 0x008A8010 (plus an out-of-line
  HasTC call operator, so the filter temporary's store issues first).
- Shared-header rewrites: 0x00B482A0 (real STLport vector constructor), 0x00B5E0F0, 0x00BF01E0 (inline render-state setters).
- Single levers: 0x00EE2490 (parent reference local), 0x00C6E790 / 0x00C6E820 (accumulator indexed through a counter copy),
  0x005B6767 (game pointer read first; the never-taken `if (game == 0) return;` after the dereference is the only spelling that
  matched -- review).

Headers: 6 new (CTCSmashableBase, CVectorCompare_BestForTargeting, C2DAlphaMaskInfo, CEnginePrimitive2DViewport,
CEngineMeshEffect, CCRC) and 24 changed, every change behind a FABLE_RETAIL_* opt-in; hdrcheck: default path unchanged
(moved `__LINE__` static asserts only), no lost opt-ins, no repo header changed since the overlay was copied.

Warnings at landing:
- RELOC_CONFLICT g_XAxis / g_YAxis / g_ZAxis (0x013893B8.. here, 0x0137CDFC.. in landed 0x005BD699): a namespace-scope
  const C3DVector has internal linkage, so per-TU copies are plausible; queued land0809e for audit.
- RELOC_CONFLICT CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 v 0x0040F020: queued land0809e.
- RELOC_NAME x11: 14 NAME_FIXES rows queued land0809e (incl. the swapped 0x00963810 FindChargeEvent / 0x008E8A10
  CIsImposterHit::operator() names).

For review: local models still beside existing engine headers in 0x00783850, 0x004BC800, 0x005A887D, 0x008E8FF0, 0x00EE2490,
0x00BEEDA0, 0x0076E4D0; whether NEWTON_PAREN can become the default path.
