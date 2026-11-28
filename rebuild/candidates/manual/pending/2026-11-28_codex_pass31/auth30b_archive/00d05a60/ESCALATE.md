# 0x00D05A60 CAssassin2::Main and 0x00D06130 CAssassin3::Main -- ESCALATE (AUTH30, 2026-11-27; one file for the pair)

## Measured (our gates)
- Codex's drafts (c0.cpp) grade RELOCATION_MATCH 1400/1400. Both are blocked by a hidden strong duplicate that the pick-any checkers
  read as EQ:
  - 00d05a60 defines `?Alloc@CAssassin2@..` and `?GetParentScript@CAssassin2@..` strongly;
  - 00d06130 defines `?Alloc@CAssassin3@..` (landed row 0x00D05FE0) and `?GetParentScript@CAssassin3@..` (landed row 0x00D06080)
    strongly.
  - The Alloc rows are landed (rebuild/src/compiled/00/d0/..._Alloc_00d05910.cpp / _00d05fe0.cpp), so each is a new LNK2005.
- The `inline` on Main is load-bearing.
  - 00d05a60 g1 (strong bodies removed, Main out of line) and g2 (Main out of line only) both grade DIFFER(1365v1400).
  - Out of line, VC7.1 calls the CIsThingAlive ctor/dtor and the scripted-thing base ctor instead of expanding them (udiff of g2.out).
  - g3 (/Og- /d2vol10 device) gives DIFFER(1842v1400). No more flag probes: no sweeps.
- With the inline Main kept, a scaffolding helper replaces the strong bodies: 00d05a60 g4.cpp and 00d06130 g1.cpp.
  - The helper is in an anonymous namespace, marked `Fidelity: scaffolding, not retail`, and does
    `return new CAssassinN(parent, data, me);`, so the TU emits the entity vftable and, through it, the inline Main.
  - Bytes: both grade RELOCATION_MATCH 1400. No strong duplicate is left (only the TU-local helper).
  - vftables: CAssassin2 at 0x012C8D28 and CAssassin3 at 0x012C8D84 agree 7/7 (placed by their own slots). Every other table agrees.
    The CIsThingAlive table has 3 slots, which equals its Ego_r field list (dtor, operator(), Clone).
    Its slot 1, `?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ`, is in Ego_r and FableWin but bound nowhere: an external
    reference, not a copy.
  - Pick-any: every copy is EQ, except `??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z`, which is 46 v 87 at 0x006E7BD0.
    Landed 0x00EA2720 and 0x00E50FD0, compiled at HEAD (head/*.obj), emit the identical copy (bytes and relocations), so it is
    existing debt. The 2026-11-24 rule applies; it needs a `// Pick-any:` note.
- Ego_d: the procedure records (codex29 package egod_locals.txt) match the source's locals and scopes exactly:
  - function scope: seh_me, TalkedTo, _Fail_Val;
  - each of two blocks: speech_movie and PauseMyEntitiesPlease.

## Questions for the ruling
1. Is `inline` on a script Main an accepted inline-budget device? The landed pass-29 rows 0x00E16E00 and 0x00E17320 already use it.
   Or is this the compile-order class (owner, 2026-11-24) that waits for object mode?
2. If it is accepted, may the vftable be emitted through a constructing anonymous-namespace scaffolding helper? It is the nf86 / nf88
   "small per-TU helper" idea, but it constructs the object instead of taking an address: a virtual member's address is a vcall
   thunk and emits nothing.
   - The alternative is an Option S unit with the landed Alloc / GetParentScript / Init / ??_G rows (owner .cpp in address order).
   - That unit would still need the inline Main.
3. Note: landed 0x00E16E00 / 0x00E17320 define their Alloc / GetParentScript / Init / OnPredicateFail strongly beside landed Alloc rows
   0x00E16D10 / ...: existing LNK2005 debt at HEAD.

If 1 and 2 are yes, the remaining work per row is my own behaviour test (Codex's 00d05a60 test passes name_gate but compiles its own copy of
the source), name_gate, purity, and the Pick-any note.
Grades: 00d05a60 c0 + g1..g4 (5); 00d06130 c0 + g1 (2).

Update (AUTH30, end of session): all seven gates now pass for this row (NOTES.md): test 00d05a60_test.cpp PASS with the row object (LINK_FAIL without), purity genuine, NAME_GATE PASS (NAME_GATE.txt). The only open item is the ruling above.
