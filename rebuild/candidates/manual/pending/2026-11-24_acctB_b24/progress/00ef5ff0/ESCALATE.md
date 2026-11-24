# 0x00EF5FF0 CPersonalScript_EmptyScript::CCinemaMan::Main -- compile-order residue (B24S1, 2026-11-24)

Best source: differ/00ef5ff0.cpp (= v/c2.cpp, shared headers only, stock flags): DIFFER(3265v3265), 24 hunks, all in the
SetPredicate(...) temporary teardown at 0x00EF6207..0x00EF6323 and the padding after it.

## The residue is ONLY four c2 inline decisions (measured)
Diagnostic decision override (inl/inl_force.py, copied from $S/w22/b24b/00d0d910/b8/inl, plus a depth hook at 0x1078CF71 [esp+18h]
taken from $S/w22/b24e/00daadd0/tr/inl_gdb.py): stock cl 13.10.3077, v/c2.cpp, in Main only, every ??1?$CPredicate_And decision
forced to "accept at depth 1, reject at depth >= 2". Forced sites (inl/force.json):
  - And5 temp dtor, depth 1, callee size 88: stock REJECT -> forced accept (retail expands it at 0x00EF623D..0x00EF6260);
  - ~And4 nested in it, depth 2, size 70; ~And3 nested in the And4 temp, depth 2, size 55; ~And2 nested in the And3 temp, depth 2,
    size 33: stock ACCEPT -> forced reject (retail calls 0x00EF6D90 / 0x00EF6D50 / 0x00EF5B50 at 0x00EF6254 / 0x00EF627E / 0x00EF62A8).
Result: objgrade inl/force.obj = RELOCATION_MATCH. Nothing else in the 3280-byte function differs.

## Why this is compile order (evidence)
1. Retail emits the out-of-line ~And3/~And4/~And5 copies (0x00EF6D50 / 0x00EF6D90 / 0x00EF6DE0, each FULLY expanded: 3/4/5 ~CIsThingAlive
   calls + nested vptr stores) AFTER Main in the same object, and only Main calls them (xref.py: one caller each, plus 0x00EF6EA3/ED4
   for ~And5 from the landed ??_G 0x00EF6E40). Our one-row object compiles and emits them BEFORE Main (c2 PIPE order in tr/c1.cpp.trace.txt;
   COFF section order: ~And2..~And6 secs 21-33, Main sec 79). ~And2's copy 0x00EF5B50 sits before Main (earlier users 0x00EF5937, 0x00EF5B94).
2. With compiled sizes (88 for ~And5) the stock heuristic cannot accept the And5 temp at depth 1 (trace: adj 82, freq 1e8, rejected);
   Main must have seen ~And5 before it was compiled (smaller IL with its nested calls not yet expanded).
3. Contrast in the same object: the landed later siblings with 4-term predicates (CArcherMan 0x00EF71E0, CChatter1 0x00EF7BE0,
   CChatter2 0x00EF80A0, stock flags, plain inline dtor) never call 0x00EF6D50 or 0x00EF5B50 -- compiled after the copies, they
   expand the nested dtors as stock c2 does.
4. Probe d5 (explicit specialisations of the five ~CPredicate_And under #pragma inline_depth(0) = bodies whose nested calls are not
   expanded, i.e. "not yet compiled") gives retail's teardown exactly except that ~CPredicateBase's vptr reset is called instead of
   expanded at the five temps (the pragma depth limit also blocks it; __forceinline ~CPredicateBase does not pass the limit, probes
   d5fi/d7fi). Depth-1 variants (d3, d7) = the same; depth 2 (d4, d8) = 109 hunks.
Unexplained by sizes alone: the depth-2 reject of ~And2 (copy compiled before Main, size 33). Consistent with c2 not re-inlining
non-trivial calls inside a not-yet-compiled callee body (only tiny always-inline ~CPredicateBase, adj < 0, is expanded there); not
verified.

## Not landable / not tried as a landing device
- d5 form (inline_depth(0) explicit specialisations): 19 hunks (five ~CPredicateBase calls); a forcing device for compile order, which
  the 2026-11-24 owner decision rules out anyway.
- /d2inlT29: teardown = retail but the first terminate exit then calls ~CScriptThing (diagnostic flag, not landable).
The row waits for the object-order build mode (EXTRA_RULES 2026-11-24): CPersonalScript_EmptyScript's object with Main compiled
before the ~CPredicate_And<And3..And5> instantiations.
Note (advisor): the ~And2 copy 0x00EF5B50 also sits AFTER its first caller (0x00EF5937), the same "copy emitted after its first user"
pattern as the three copies after Main.
