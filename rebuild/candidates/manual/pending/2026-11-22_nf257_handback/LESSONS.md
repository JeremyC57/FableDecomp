# NF257 lessons (one line each)
- Count a class's vtable emitters from absrefs before scoping the unit: L72 had three constructors and Clone storing 0x0127ADDC, and the table's ??_7 witness was a ctor the entry did not name; porting only the named rows would have split one COMDAT that matched at HEAD.
- A base constructor that only one header branch declares (PRIORITYCTOR in the third branch) is added to the class's prelude branch under a NEW macro name: reusing the old macro would change a landed row that defines both (0x008B9010).
- vs955.py (slot-for-slot against the landed emitter's HEAD table by retail target) reproduced NF255's 76 / 76 for L74 before I trusted it on L72 / L100: check a new checker against a known answer first.
- `^// Flags:\s*(.*)` with re.M reads the NEXT line when the Flags line is empty (0x00955620's `// Flags:`): a payload then passes a `#define` line as compiler flags; use `[ \t]*`.
- For a port off local models, keep the HEAD row's named locals that fix the load order: 0x0096B600 needed `creature` and `target` named before the two GetPos calls (form 1 without them was 351 v 355).
- Gate every grade launch on the precheck's output inside the same script (`[ -z "$r" ] && launch`), not on reading it afterwards.
- A hard-link copy of rebuild/include (`cp -al`) costs no disk for whole-tree equiv_check; replace an overlay file with rm + cp, never cp over a link, and cmp the repo header against orig/ after.
