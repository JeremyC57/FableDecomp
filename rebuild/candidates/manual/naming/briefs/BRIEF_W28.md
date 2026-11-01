# Naming pass, wave 28 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave27/DECISIONS_W27_RESOLVED.tsv` (+ waves 13-26); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`. Rules, mechanics and hand-back as `briefs/BRIEF_W27.md` / `BRIEF_W26.md` (read both, the wave 27 README +
AGENT_README + DEFERRED.tsv + QUEUE_W27.tsv and the wave 27 `scripts/`, reused as compose28.py etc.; mkindex.py needs an absolute output path).
**Guard every scratch path variable** (`: "${W:?}"`, `: "${S:?}"`); write only under $S; no git at all. The Leaf the compiler emits is
authoritative; `objdump -t` it after any nesting or access change. When a family edits a shared header, recheck every landed includer of it
(wave 27 GOTCHA). No modelling device retail never had (no row-local wrapper for a missing STLport member). Never give a second retail copy of one
symbol that symbol's spelling (hold it).

## Scope, in this order (every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The DECISIONS_W27 approvals**: D4 the 0x007345D0 / CTCCombat family (0x00666B50, GetCombatInfo 0x00669CB0, the three caller stand-ins) as one
   family; D5 the invented namespace NTCScriptedControl -> the PDB's nested CTCScriptedControl::CActionBase in 00713660 / 00713850 / 00714900; D3
   0x004091C0 only after 0x004125A0's tag and 0x0040E350's identity are proven from the bytes; D6 0x006A5230 only with a second source.
2. **The open queue rows** (`$S/naming/w28/QUEUE_W28_input.tsv`: wave 27 deferred and partial rows; the coordinator appends c696 rows when that
   chunk lands -- expect the ~CScriptThing 0x004AA840 binders: ten spellings are bound to that one address today).
3. **Not reached in waves 24-27**: the 14 ccsns rows, the 0x0099B800 in-TU callers 009994a0 / 00999620, the c690 sort-row leftovers, then wave 27
   DEFERRED.tsv (`DEFERRED_W27_INPUT.tsv`: the 00666b50 family findings, the 005955ab callers 00595845 / 00599e3f, PeekTypeList, the STLport set
   stand-in callers, 0x00CAF680, the destructor families, the mega families, cg10).
Not in this wave (HOLD): 0x00AD3A10, 0x007ADB50 / 0x007ADD50, 0x005136B0, 0x005359A0 (ICF), the 0x00613AE0 / 0x006196D0 callers (two retail
copies); 0x006BC4A0 (owner class unknown); 0x0139C900 and 0x0143BB2C (no retail name); 0x00C2D9C0 owner; engine/CBankFile.h and its binders
(0x009CBFE0 / 0x009D0380 / 0x009CEA70), CThingSearchTools (engine-header migration); 0x007E5230; the GetWorldSeed / GetPRandomSeed / ConstantFPS
rows (owner item); 0x00C28690; 0x0051B620; VIRTUAL_SCREEN_*; 0x00440730; 0x00EF87D0 / 0x00EF5CB0 (keep their inline_depth sources).

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W28.tsv (every queue row with its outcome), DECISIONS_W28.tsv, the
per-family rows files, ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 27 did; leave no lv_* directory and no background job.
