# Naming pass, wave 26 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave25/DECISIONS_W25_RESOLVED.tsv` (+ waves 13-24); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv` (new: 0x0139C8FC `?_NullString@@3PAUCStringData@@A`); the class statics are reached through their PDB inline
accessors. Rules, mechanics and hand-back as `briefs/BRIEF_W25.md` / `BRIEF_W24.md` (read both, the wave 25 README + AGENT_README + DEFERRED.tsv +
QUEUE_W25.tsv and the wave 25 `scripts/`, reused as compose26.py etc.; mkindex.py needs an absolute output path). **Guard every scratch path
variable** (`: "${W:?}"`, `: "${S:?}"`) -- wave 25 wrote /pw and /ov_final at the filesystem root from unset variables; write only under $S. No git
at all. Search Fable.exe's own strings before choosing a library spelling by analogy (wave 25: _NullString). A hand-mangled spelling in a
NAME_FIXES row is a claim: the Leaf the compiler emits is authoritative. No modelling device retail never had. Never give a second retail copy of
one symbol that symbol's spelling (hold it). Add a `// Leaf:` line to a landed source before trusting a recheck verdict on it. Do not touch
0x00BC9C30 / 0x009CDF50 (chunk 693 repairs them).

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The open queue rows** (`$S/naming/w26/QUEUE_W26_input.tsv`; the coordinator appends c693 rows when that chunk lands): the _afxDataNil binders
   0x00C0B910 / 0x00C0B9F0 / 0x00C0BA40 -> `_NullString`; 0x00744580 (identity of the last 0x00686A80 binder); the wave 25 deferred / partial queue
   rows; 0x00EE76F0 / 0x00D9F270 and the 0x00F35B10 SetPredicate binders (the NScript predicate re-model); 0x009F31C0's WriteColour by reference.
2. **The per-row re-models** (scope 2 of waves 24-25, not yet reached): the 23 vector<long>::_M_insert_overflow rows (size_type `I`), the six
   0x007ACC50 callers, the six 0x00412330 stand-in callers, the 0x0099B800 in-TU callers 009994a0 / 00999620, the c690 sort-row leftovers, the 14
   ccsns rows.
3. **Wave 25 DEFERRED.tsv** (`DEFERRED_W25_INPUT.tsv`): PeekTypeList, the STLport set stand-in callers, 0x00CAF680, the STL stand-ins, the destructor
   families (0099a2e0 / 0099a410 / 00aa75c0 / 00f35b40 / 00cbd510) and the mega families (GetPItem, AddEvent, GetInstance, GetSubDef), then cg10.
Not in this wave (HOLD): 0x00AD3A10, 0x007ADB50 / 0x007ADD50, 0x005136B0, the 0x00613AE0 / 0x006196D0 callers (two retail copies); 0x0139C900 and
0x0143BB2C (no retail name); 0x00C2D9C0 owner (no two sources); the CBankFile / CThingSearchTools rows (engine-header migration); 0x007E5230; the
GetWorldSeed / GetPRandomSeed / ConstantFPS rows (owner item); 0x00C28690; 0x0051B620; VIRTUAL_SCREEN_*; 0x00440730.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W26.tsv (every queue row with its outcome), DECISIONS_W26.tsv, the
per-family rows files, ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 25 did; leave no lv_* directory and no background job.
