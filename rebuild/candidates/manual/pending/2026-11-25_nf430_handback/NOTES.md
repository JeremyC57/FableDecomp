# NF430 hand-back (2026-11-25): the `CVtable_<va>` placeholder-class family

Nothing was written to the repo. No git command was run. Grades come from VC7.1 on the Linux lane, and retail bytes appear in no file here.

## Measured, not as briefed
- **Open addresses.** The tracker (HEAD 8736b9f1a1) lists **1,118 open addresses** that spell a `CVtable_` placeholder. They name **431 vftable VAs**; two are written with 7 hex digits (`CVtable_12c3fa4`, `CVtable_12dfd3c`).
- **The "227 addresses" stand-in** is `?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAX...` (NF424 r2's S0 family), not `??0C3DAnimationUtil@@QAE@XZ`.
  - `??0C3DAnimationUtil@@QAE@XZ` sits on 14 row files, all in libpk (0x00C3xxxx / 0x00C5xxxx). 8 are open, and 6 are listed in SHARED_PDB.
  - These rows store CSal* render vftables (0x012B8A78..). They are libpk holds under R-NF430-1.
- **Breakdown by region**, taken from the vptr stores in retail .text (`tools/xref430.py`):

  | region | VAs | open addresses |
  |---|---:|---:|
  | libpk / CODA (0x00C086C0..0x00C89C10) | 317 (+3 also stored from Fable code) | ~1,000 (vftable 150, code 734, EH unwind 106, + 14 mixed) |
  | Fable code (NScript scripts, CFeatDef, CScriptBase, strmbase) | 105 | 114 |
  | retail RTTI COL present (NPlayerGui) | 6 | 6 |

- The libpk / CODA region holds 2 more VAs that only callers spell (open addresses 0x012C3FA4 and 0x012DFD3C were wrongly filed here before; t1 closes them).

## Identification method and hit rate
- **RTTI COL** (`tools/col430.py`): retail `[va-4]` is decoded as a Complete Object Locator. Only **6 of 431** have one: NPlayerGui::CDrawSpiritsHealth, CDrawReceivedItems, CDrawFishingGame, CDrawSpiritScores, CDrawDamage and CDrawScriptScreenText. The NScript script modules were built without RTTI in retail.
- **FableWin.pdb vftables** (`tools/fwpub.py`, `tools/vtpdb430.py`): the PDB's `??_7<Class>@@6B@` is read slot by slot (following the incremental-link thunks) and compared with the retail slots.
  - Discriminating check (`evidence/own_slots.tsv`): every FableWin slot whose class is the candidate class must land on a retail slot in a row whose catalog module, or landed self symbol, is that class.
  - Result over the 98 t1 VAs that need the full check: **395 own-override slots agree, 0 disagree**. Every one of these VAs has at least one agreeing own slot.
  - The method-name-only comparison does not discriminate (every level script has the same 5 slots), so it is not cited as a source.
- **Second source:** the vptr-storing binder's own `// Leaf:`, which is a PDB public (FableWin / Ego_r) of the same class (`evidence/src2.tsv`). All 100 Fable-side VAs pass it.
- **HEAD object census** (`tools/census.sh`, `nftools/pairs.py`): all 197 binder rows of the Fable-side VAs were compiled at HEAD (CEntityScriptBase's 613 binders were excluded), and the symbol each object emits at each placeholder VA was read.
  - At 108 of 110 VAs, every binder emits exactly one symbol, and that symbol is the real one.
  - Exceptions: 0x00CAB120 still emits `??_7CVtable_012bf6bc` (strmbase), and 0x00F344C0 still emits `??_7CVtable_012fb778` (T3 nf303).
  - Every one of the 102 deleted lines has its witness among the rows that emit the real name.
- **libpk / CODA:** no Fable PDB covers it (FableWin and Ego_r publics hold no PKC* / CCoda* / CSal* class).
  - A strict masked-byte match of the GameCODA 2.10 release `CODAApi.lib` vftable slot bodies (`tools/codavt430.py`, `evidence/coda_match.tsv`) scores only 1-4 trivial slots per vftable. That is "not shown", not a disproof; the Dbg lib and non-strict matching were not tried.
  - These classes stay placeholders under R-NF430-1.
- **Hit rate:** 100 of the 108 Fable-side or RTTI VAs are identified from two sources (98 NScript scripts with the own-slot check, plus CEntityScriptBase and CSP_UndeadTrigger, which are held), and 6 NPlayerGui. That is 0 of 317 libpk VAs.
  - Closed in this hand-back: **99 open addresses** (99 `split_unknown`, 0 regressions; see the control below).

## Shape histogram (the 1,118 open addresses)
| shape | count | disposition |
|---|---:|---|
| `??_7` split at an NScript vftable (stale placeholder table line, the row already emits the PDB name) | 99 | **t1a/b/c ACCEPT (table-only)** |
| same shape, witness in T3_PORT_QUEUE (0x012DDF3C, witness 0x00DEF6A0, nf328) | 1 | T3 identified_port |
| CEntityScriptBase (0x012C3224): rows 0x00EB2000 and 0x00DEF6A0 model the base locally as `struct CVtable_012c3224` | 1 | T3 identified_port (port onto engine/CEntityScriptBase.h) |
| NPlayerGui RTTI vftables (rows emit `??_7CDraw*@@6B@` without the namespace) | 6 | T3 identified_port + NAME_FIXES 0x00442760 |
| CFeatDef / CScriptBase ctor bindings (0x00F344C0, 0x00CB8110, 0x012FB778, 0x012C1648) | 4 | T3 (nf303 queue lines) |
| strmbase CEnumMediaTypes ??_G (0x00CAB120, 0x012BF6BC) | 2 | vendor-library route |
| CActiveEntityScript<CQ_MinionCampScript> (0x00DA0130, 0x012D6E5C) | 2 | R-NF424-2, not re-derived |
| D3-decided CODA classes (CCodaObjectBase 0x012B5868 + 3 binders) | 4 | T3 d3_port (15 spellings over 13 rows) |
| libpk / CODA, no RTTI and no PDB | 1,000 | HOLD, R-NF430-1 |

## Batches (each holds at most 34 rows; all are ACCEPT, table-only)
`batches/t1a`, `t1b`, `t1c`: 34 + 34 + 34 rows and **102 `delete` lines** (99 VAs; three of them have two case-variant lines, `CVtable_012C3688` / `012c3688` and the like).
- **Rows, tests and orig copies** are the HEAD sources, unchanged. RESULTS `old` = `new` = the row's current self symbol (from self_symbols.tsv).

## Gates
- **apply_table --dry:** 102 deletes, 0 misses. Multi-target count is 1 before and 1 after (no new one).
- **vland HEAD/new at one path** (`tools/runpair430.sh`):
  - HEAD runs from the repo table; new runs from `work/rsnew` (repo symlinks plus a copy of the table with the deletes applied).
  - Result: 102/102 RELOCATION_MATCH PASS on both sides, identical grade lines, identical warn kinds (SHARED_HEADER_AVAILABLE only).
- **Object section compare:** `objdump -s -r` of HEAD and new objects is equal for all 102.
- **Tracker** (naming_status from `work/rsnew`): exactly 99 `split_unknown` addresses leave OPEN, and nothing else changes.
  - The 47 `rtti_identified` -> agree shift that a symlinked root shows also appears in a control run with the unchanged table (`work/rsctl`). It is an artefact of running from that root, not of this change.
  - New OPEN placeholder addresses: 1,019 = T3.txt's non-debt lines.
- **name_gate** (a copy, `tools/name_gate430.py`, reads the table named by NG_TABLE): **FAIL on all 102, on pre-existing debt only.**
  - The debt is SHARED_HEADER_AVAILABLE / LOCAL_CLASS in rows and tests, plus two old splits: 0x00D2F6B0 `??_7CScriptThing` and 0x00EB2000 `CVtable_012c3224`.
  - HEAD shows the same failures plus the split_unknown this delete closes (measured on 0x00CDD550).
  - This is why R-NF430-2 asks whether the batches may land. If the answer is no, the folders become HOLD and nothing needs redoing.
- **Emitted vftables** (`tools/vt430.py`): WRONG H = N (151 / 120 / 178 slots). The local models bind the base CScriptBase slots and the `?Hook@T@@` stand-in selves. The debt is unchanged and is queued per row in T3 (`debt_header`, 102 lines).
- **pk_all, /FORCE link, equiv_check, hdrcheck, mutation control:** not applicable. No row, test or header changes, and the objects are byte-identical. The mutation control cites NF419 r1.
- **reserved_check** (RESERVED_REFS=""): 0 reserved and 0 mentions in all three batches.
- **Locks and claims:** no Account B claim is open, and no row is in RESERVED. Witness 0x00DEF6A0 (T3 nf328) was excluded with its VA.

## Not done / resume
- **NPlayerGui (6 rows):** port each ctor onto `engine/NPlayerGui_CDrawBase.h` (CTOR + VSLOTS opt-ins) with class headers. The PDB ctor is `??0<C>@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@...@Z`.
  - This must land together with the 0x00442760 respell (NAME_FIXES), or VSLOTS slot 1 creates a split.
- **CEntityScriptBase:** port 0x00EB2000 and 0x00DEF6A0 onto the header, then delete `??_7CVtable_012c3224@@6B@`. Do the same for 0x012DDF3C once nf328 lands.
- **CODA D3 consolidation** (0x012B5868, 13 rows) and the whole libpk family wait on R-NF430-1.
- **Locks:** the 102 witness rows of t1a/b/c stay locked as NF430. No other lock was taken.
