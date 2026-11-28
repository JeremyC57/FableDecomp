# 00ef5840 LOG (AUTH30)
c0 (Codex draft, overlay): DIFFER(736v727). Residue: at the two predicate cleanups (outer And<And<A,A>,A> temp at esp+0x8c, And<A,A> temp at esp+0x68)
retail stores the CPredicateBase vptr 0x012C2FF4 (hoisted into ebx); ours calls ??1CPredicateBase out of line (lea ecx; call). c0 also defines
??1CIsThingAlive strongly (pk_all: strong, a2) = new LNK2005 against landed owner 0x00E241B0.
g1 hypothesis: the out-of-line base dtor calls come from the row-local inline_depth(0) And-dtor specialisations; with the header's existing
opt-in FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR (in-class inline And dtor, no row specialisation) the base dtor expands at both sites.
g1 result: DIFFER(768v727) = Codex's baseline length. Every CPredicateBase vptr store is now inline (ebx hoisted, as retail); the one residue is
the inner And<A,A> destructor EXPANDED inside the outer And<And<A,A>,A> expansion where retail calls its 33-byte copy 0xef5b50 (it is still
expanded for the separate And<A,A> temp, as retail). pick-any 5 EQ / pk_all 8 EQ; vftables 0x12E1B70 3/3, 0x12F6628 3/3.
g2 hypothesis: the strong ??1CIsThingAlive (new LNK2005 v landed 0x00E241B0) and its optimize("gt") toggles are not load-bearing once the
row-local specialisations are gone: spell it `inline` as 89 landed callers do; Main still calls 0x00E241B0 out of line.
g2 result: DIFFER(768v727), instruction text identical to g1; pk_all ??1CIsThingAlive now pickany (EQ 84, no strong duplicate); pick-any 5 EQ,
pk_all 8 EQ, no NEQ. vftable-reached bodies NEQ (not checked by pk tools, GOTCHAS 524): ??_GCIsThingAlive 30 (retail 45),
??_GCScriptGameResourceObjectScriptedThingBase 30, ??_G outer And 75. Best legal base = g2 (no row devices, no strong duplicate).
Stop: Codex's PROBES.md lists 2,000+ `- ` probe lines on this residue (depth sweeps, /d2inl sizes, force/noinline, definition order, visibility); the
only lever left is how VC7.1's inliner treats the inner And<A,A> destructor at depth 2, and no shared-header opt-in reaches it.
g3 hypothesis (advisor): Codex's 727-byte probes are "implicit" variants; an undeclared, compiler-generated And destructor is not the same as
g1/g2's explicit in-class `{}`. g3 = g2 with a new add-only overlay opt-in FABLE_RETAIL_CPREDICATE_AND_IMPLICIT_DTOR (#elif branch, no
destructor declared) in hdr/engine/CPredicate_And.h.
g3 result: DIFFER(727v727), refuted. The length is equal only by coincidence: with compiler-generated destructors VC7.1 drops BOTH derived
vptr stores (retail stores 0x012F6628 at esp+0x8c and 0x012E1B70 at esp+0x68) and expands the inner And<A,A> destructor (retail calls
0xef5b50); pickany ??1 inner And 27 v 33 NEQ (not referenced by the leaf). Retail's stores show the And destructor is user-declared, so
Codex's 727-byte "implicit" probes are the same coincidence. The opt-in (an `#elif defined(FABLE_RETAIL_CPREDICATE_AND_IMPLICIT_DTOR)`
branch declaring no destructor, between the INLINE_DTOR branch and `#else`) is removed from the overlay again; g3.cpp needs it to rebuild.
Best legal base stays g2.
