# NF91 lessons (one line each)
- Measured on one row (0x0059C6B0, three forms, DIFFER 47v45): a pick-any (inline + noinline) body visible in the TU did not give VC7.1 the callee's register footprint (retail keeps `this` in edx across the call); only the strong same-TU copy did. Compile order versus pick-any linkage was not separated. Template bodies do give alias facts (0x005A30A1).
- For a GetDef / GetSubDef caller, add the header template bodies before anything else: 0x005A30A1 went DIFFER 917 -> 926 -> RELOCATION_MATCH with GETDEF_GLOBAL_BODY then GETSUBDEF_BODY (a missing StatGetTypeName is a one-line _TYPENAME opt-in from FableWin.pdb + the retail literal).
- A sort front end ported onto the STLport spelling exposes every helper below it that still spells a row-local `class pair` (PAV): probe the whole helper chain (insertion sort, linear insert, unguarded insert, copy_backward) first and script them; all eight went first try.
- When a residue is register allocation between two long-lived values, try removing one of them as a named variable (0x005BF77D: dropping the element reference gave retail's allocation at once); stop after three forms.
- CVectorMap<K,V>::Sort / Update instances are FableWin.pdb names (Lionhead template): check FableWin before calling one descriptive.
- `vl.sh` prints only the log tail: count PASS lines in the log file, never in the piped output.
