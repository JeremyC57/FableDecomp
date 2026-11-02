# Naming pass, wave 32 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-02_wave31/DECISIONS_W31_RESOLVED.tsv` (+ waves 13-30); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`. Rules, mechanics and hand-back as `briefs/BRIEF_W31.md` / `BRIEF_W30.md` (read both, the wave 31 README +
AGENT_README + DEFERRED.tsv + QUEUE_W31.tsv and the wave 31 `scripts/`, reused as compose32.py etc.; mkindex.py needs an absolute output path).
**Guard every scratch path variable** (`: "${W:?}"`, `: "${S:?}"`); write only under $S; no git at all (not even read-only `git log`: use the
repo files and the coordinator's messages). The Leaf the compiler emits is authoritative. When a family edits a shared header, recheck every
landed includer (real `#include` lines, transitively through other headers) and list them in runs/hdrdep.rows.txt; for a generated header keep the
RETAIL_MARK so gen_class_headers.py keeps it. No modelling device retail never had. Never give a second retail copy of one symbol that symbol's
spelling (hold it). Do not add new invented spellings to an address that has a PDB name. LABELS.tsv must carry the spelling the family settled on
(wave 31's 0x004C7590 label still read the pre-decision name).

## Scope, in this order (every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **CAIStateGroup base ctors** (DECISIONS_W31 D11): one shared CAIStateGroupBase + CBaseIntelligentPointer model (a header family if a shared
   header fits) for the ~70 landed generated CAIStateGroup_* ctors: ??0CAIStateGroupBase@@QAE@XZ 0x008FD255 and ??0CBaseIntelligentPointer@@QAE@XZ
   0x00A01B10 replace CBase_008fd255 / CMemberBase_00a01b10 and the ~20 invented spellings; the landed 0x00900B81 calls
   ??0CAIStateGroup_MinionReactToCrime@@QAE@XZ (chunk 703 landed it). The other CBaseIntelligentPointer members 0x00A01B50 / B90 / C10 follow.
2. **Wave 31 carry-overs**: D12 (prove 0x004C5990 / 0x004C5C60 call 0x004C5690, then respell; 0x004C5DA0's inline list dtor on
   std::list<CPrimitive>), D13 (0x00437CD0 GetPHeroInfoScreensManager, the 0x013B8790 binders on the wave-18 PClass spelling), D10 (0x0082FD90 off
   its inverted CThing / CThingPhysical model onto a shared CThing model), D5's four unrelated-global spellings of the libpk statics, the 7 remaining
   ~CTCBase binders.
3. **The queued rows** (`$S/naming/w32/QUEUE_W32_input.tsv`: wave 31 deferred / partial + c703 rows; the coordinator appends c704 rows when that
   chunk lands), then DECISIONS_W29 D2 / D6 / D3, ccsns, 009994a0 / 00999620, the c690 sort rows and wave 31 DEFERRED.tsv.
Not in this wave (HOLD): the 17 two-view CScriptThing rows (inline_depth owner question); the 121 CActiveEntityScript<T> ??_G rows; 0x00562E70 Close
(one source); 0x00AD3A10, 0x007ADB50 / 0x007ADD50, 0x005136B0, 0x005359A0, 0x0040E350 (ICF / two retail copies), the 0x00613AE0 / 0x006196D0
callers; 0x006BC4A0; 0x0139C900 and 0x0143BB2C; 0x00C2D9C0 owner; engine/CBankFile.h and every CBankFile-family binder (incl. 0x009D58A0 / 0x009CE360
/ 0x009D6490); CThingSearchTools; 0x007E5230; LoadFileList; the ConstantFPS owner item; 0x00C28690; 0x0051B620; VIRTUAL_SCREEN_*; 0x00440730;
0x00EF87D0 / 0x00EF5CB0. The four pre-existing failures 0x00692230 / 0x006922C0 / 0x004D9861 / 0x009E1440 are chunk 705 repairs: expect them to
fail in a baseline recheck and do not edit them.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W32.tsv (every queue row with its outcome), DECISIONS_W32.tsv, the
per-family rows files (+ runs/hdrdep.rows.txt), ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 31 did; leave no lv_* directory
and no background job.
