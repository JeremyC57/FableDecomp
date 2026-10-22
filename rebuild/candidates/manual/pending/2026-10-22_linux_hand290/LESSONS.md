# Chunk 290 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Plain `inline` STLport map dtor in a script Main -> expanded at the FIRST early exit only, out-of-line call (0x00CDBFB0) at every later exit; `__forceinline` expands everywhere (0x00E292C0).
- STLport map ctor written with `head(0)` then malloc(0x24) -> retail's double count store (0x00E292C0; model 0x00F01480).
- Local CScriptThing with an inline-defined virtual GetPos -> devirtualised AND inlined; a declared-only virtual (GetDataString) -> direct call; the same calls on member `Me` stay virtual (0x00E1E960).
- `a != "" && b != "NULL"` on two conditional CCharString temporaries -> the ebx "built" flag plus stored bool (0x00E1E960).
- CList find loops: a cached `last` end local in one search and a fresh `end()` in the other -> VC keeps &vector (not &finish) CSE'd in ebx (0x0053B321).
- A named def-pointer local -> placed in the dead argument slot and released after the call (0x0053B321).
- `if (b) f(0); else f(6);` keeps two call arms; `f(b ? 0 : 6)` becomes sbb/and (0x0053B321).
- Advisor form: a `c = child.Data` named local lets the map key store sink to retail's position (0x0053AD07).
- `fabs(float)` overload against a float literal -> `fcomp dword`; the double overload compares a qword constant (0x0053AD07 / CList TU).
- Inline-asm-free fsub: write the float subtraction before the integer stores to get retail's fsub sinking (0x00417001).
- Inline getter -> early `this` load; re-read member pointers through `this` instead of caching them (CList / CMenuEntry rows).
- Row length: when a body plus jump table ends before the catalog row end, the grade is DIFFER only on length; land with the corrected length (0x00EA4DE0, 0x210).
- Tests: VC7.1 `T()` does not value-initialise a non-POD with std::string members, so a std::string-holding test struct fails only under VC; use char buffers + memset (test 0x00416953).
- finish.py takes the function name from the first `// X::Y(` line; a leading `// Grade:` line in src/ breaks it -- keep `// Grade:` lines in differ/ only.

Review at landing:
- Rows over the 4-grade brief budget (within p290's 5-per-strategy rule): 0x0053B321 (10 grades, matched), 0x0053AD07 (advisor-assisted), 0x00A22F80 (~25 probes, parked), 0x008E7040 / 0x009F20B0 (14 probes each, parked), 0x00A56270 (10, parked), 0x00A64910 (8, parked), 0x008203A0 (7, parked).
- 0x00EA4DE0 is byte-exact at length 0x210 -- re-catalog and land.
- 0x0042A34E is an STLport `_Rb_global<bool>` helper, not a CMemoryAllocatorVariableSize member: check symbol binding against other copies.
- Special flags: 0x00EA4DE0 switch jump table; skipped 0x00418289 needs /GS; no /EHsc rows among the matches.
