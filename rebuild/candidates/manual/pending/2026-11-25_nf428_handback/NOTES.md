# NF428 (Account A naming queue, 2026-11-25): the CCountedPointer<T> member family : 71 ACCEPT (Reset self ports), 12 HOLD in a batch, 1,024 HOLD / not attempted / skipped in FAMILY_TABLE

Family: tracker klass `CCountedPointer`, 1,107 open addresses (status standin / split_fixable / split_unknown), `fam.json`.

## Shape histogram (self leaf, tracker run on this tree; `status/`)
| shape | rows | retail callers |
|---|---:|---:|
| standin `??1?$CCountedPointer@VCUnprovenPointee_<A>@@@@QAE@XZ` (53 B "D53", 7 rows 44 B "D44") | 696 | 6 |
| standin `?ShareData@?$CCountedPointer@VCUnprovenPointee_<A>@@@@...` (67 B "SH67") | 146 | 0 |
| split_fixable `?Reset@CCountedPointer_<A>@@QAEXPAX@Z` (self stand-in, callers bind the PDB name; 103 B) | 126 | 126 |
| split_fixable `?OperatorDelete_thunk_<A>@@YIXPAX@Z` (DeleteData stand-ins) | 20 | 0 |
| split_fixable `??0CCountedPointer_<A>@@QAE@PAX@Z` (ctor(T*) stand-ins) | 19 | 19 |
| split_fixable `?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z` | 14 | 0 |
| split_unknown `??1?$CCountedPointer@VCUnprovenPointee_<A>...` | 10 | 10 |
| standin copy ctor `??0?$CCountedPointer@VCUnprovenPointee_<A>@@@@QAE@ABV0@@Z` | 7 | 7 |
| other (ShareData@CCPtr, CCountedPtr_assign, OnDie, ReleaseData / ??4 / ??1 with real T, ...) | ~69 | |

Premise correction (brief said split_fixable selves are usually already the PDB name): for the 126 Reset and 19 ctor rows the SELF is the
stand-in (local SCountBlock model) and the CALLERS bind the PDB name. So these are T3 self ports (NF371's form), not caller respells.

Filters (evidence/blocked.json): 0 nflocks held by others, 0 RESERVED, 0 Account B claims; 53 family addresses named in T3_PORT_QUEUE
(skipped).

## Reset (127 rows in the tracker): T from two sources, NF371 form
1. Callers: the tracker spelling list has the PDB name `?Reset@?$CCountedPointer@<T>@@@@QAEXP..@Z` bound at retail call sites (whole
   FableWin.pdb entry).
2. Retail RTTI (tools/rtti428.py, evidence/rtti.tsv): at a retail call site, the object passed is freshly constructed just before the call;
   its vftable store (rebuild/oracles/data_vas.tsv `RTTI .?AV<X>@@`) or the `??0<X>` constructor call names X == T. RTTI_OK 73 of 83,
   RTTI_DERIVED 5 (another class in the window: a derived class or a neighbouring object), NONE 5.
- NOT counted as a source: the deleter the body stores (`mov dword [eax+4], imm32`, tools/resetdd.py) is bound to DeleteData<T>, but those
  bindings were derived from these Resets' callers (agentFAM10's comments; the binding's only witness is the Reset row itself,
  78 of 83). The deleter bodies themselves rarely name T (tools/ddsrc428.py: 56 virtual delete, 25 inlined dtor, 2 call ??1<T>). Used only
  as a filter (evidence/dd_reset.tsv DD_OK 85, DD_MULTI 7, DD_OTHER 35).
- Also tried: Ego_r struct layout of the caller's class at the `this` offset passed (tools/layout428.py): 10 OK, 17 OTHER (the CPlayerGui
  members are shifted by exactly one 8-byte slot between Ego_r and retail; consistent with T but not usable as stated), 56 no readable site.
Of DD_OK: 1 blocked (T3 queue), 1 template T (vector<unsigned __int64>) -> 83 generated (tools/gen428_reset.py) -> 71 ACCEPT, 12 HOLD.
Form per row: NF371's landed strong specialization (0x00447D90), or NF371 R1 form b (explicit instantiation, landed 0x00A299C0) when another
landed object DEFINES the Reset name as a pick-any copy. Definers: the census of 144 landed sources that name CCountedPointer<T> and Reset
(tools/census428.py) PLUS every table witness row of each Reset name (37 rows, evidence/witness_census.tsv; the first census missed the
CPlayerGui / CEnginePrimitiveRenderer binders, which take the type from an engine header). Final: 70 form b, 13 strong.

## D53 / D44 / SH67 stand-ins (696 + 146): HOLD
- The 53-byte (and 44-byte) shape carries BOTH `??1<T>` and `?ReleaseData@<T>` under PDB names in retail (occ.json, 0x0041BB30
  ReleaseData<CTexture> / 0x0041BC90 ~<CTexture>, ...): every D row needs T and the member; the stand-ins' "Data-first order proves the
  destructor" claim is refuted by retail.
- No retail callers (6 of 696).
- Count gate per TU (NF426 method, tools/tu428.py, gate428.py): D count-equal single-T 34 copies, SH 20; debug (FableWin) link order is far
  from retail's (LIS 451 of 1361 TU segments), so FableWin's module = first user in DEBUG order, not retail's.
- Layout predictor (T of the adjacent named CCountedPointer<T> member), leave-one-out on the named copies (tools/loo428.py,
  evidence/loo.tsv): 11 right, 5 wrong, 6 conflict, 43 no named neighbour -> precision 11/16 = 69%: not a source. RULINGS_NEEDED.

## Result
| folder | rows | verdict |
|---|---:|---|
| batches/acc_r1 | 34 | ACCEPT (32 form b, 2 strong) |
| batches/acc_r2 | 37 | ACCEPT (29 form b, 8 strong) |
| batches/hold_r1 | 12 | HOLD: 9 without the RTTI source (0x0051CEA0 0x00675060 0x006750D0 0x00771B60 0x008AB2B0 0x00A44170 0x00AC38A0 0x00B37B90 0x00BB64A0); 0x00403DB0 / 0x00403E40 / 0x008AB230 form b's pick-any ReleaseData<T> is a new body under that name or NEQ to the name's retail row (0x00403750, 0x007E70E0) |
`runs/r1`, `runs/r2` are the measured run dirs (all 83; not for landing). Family (FAMILY_TABLE.tsv, T3.txt): 71 ACCEPT, 922 HOLD,
61 NOT_ATTEMPTED, 53 SKIP (T3 queue) of 1,107. Hit rate: Reset 71 of 127 (56%); family 71 of 1,107 (6.4%).
direct / rule (R-NF426-1 sense): not applicable (no byte-identical twin pair among the accepted rows: each Reset has its own T).

## Gates (83 rows measured in runs/, 71 accepted)
- hn (tools/hn426.py): HEAD and new RELOCATION_MATCH, leaf bytes SAME; selection strong 1->1, form b 1->2.
- vland --recheck-landed HEAD v new at one LANDVERIFY_WORK path per run, LANDVERIFY_STRICT_LEAF=1 (work/vland_r{1,2}{h,n}.log), rerun
  after the form flips and the test rewrite: 83/83 RELOCATION_MATCH PASS both sides; HEAD 81 STANDIN_LEAF warnings, new 0 warnings.
  The final row texts differ from the measured ones only in comment lines (source 2 now cited as RTTI; diff of non-comment lines empty).
- Tests: new, one per row: include engine/CCountedPointer.h (no local class body), T only declared, DeleteData<T> defined in the test.
  The test adds FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL (Reset declaration only) for form-b rows too, which the row itself does not
  use: without it the header's inline Reset would compile into the test and the row object would not be linked. 83 PASS linked with the
  row object (tools/trun428.py; the self-contained link fails). Mutation control (evidence/mutation.txt): 0x004441A0 without the
  count-down: DIFFER 98v103 and test FAIL rc=2. HEAD tests were self-contained models (local P/B structs), retired by the new tests.
- name_gate.py --test on the final texts: see batches/*/evidence/name_gate.txt.
- objsec (tools/objsec428.py): 83 ALL_EQ under the renames, plus the ruled R2 pick-any count-block ctor copy (25 B both sides, body
  differs; NF371 R2) and, for form b, the extra pick-any ReleaseData<T> (53 B, sel 2).
- pk_all HEAD v new (--leaf per side, --addrs = this hand-back's names): accepted rows: all copies NOADDR, leaf=n, 0 NEQ, 0 reach flips.
  (Held 0x00403E40 / 0x008AB230 show NEQ ReleaseData copies: that is why they are held.)
- R1 / R10 (tools/rdcmp428.py, evidence/rdcmp.tsv): every accepted form-b row's leaf Reset copy and pick-any ReleaseData copy are byte- and
  relocation-identical to every landed definer of the name (census + witnesses). Exposed debt (NF419 r2): the header's default ReleaseData
  body differs from retail's 53-byte copies (0x00403750, 0x007E70E0); the accepted rows add no new body (same as the binders').
- /FORCE link (tools/link428.py, evidence/link_main.txt): 71 rows + 151 definer objects (census + witnesses). Duplicates HEAD 1 -> new 1
  (the pre-existing ??1CWorld), unresolved CCountedPointer names 56 -> 46 (10 now resolve), 0 new. Control (evidence/link_ctl.txt): a
  second copy of 0x0050F2B0's object adds exactly its LNK2005.
- One name, one address: every new Reset name is bound at exactly one target in reloc_bindings (its row).
- reserved_check (RESERVED_REFS=""): 0. No header change (no includer recheck). TABLE_CHANGES header only, so apply_table --dry was not run
  (nothing to apply): the callers already bind the PDB names; the self stand-ins have no table or self_symbols line (SELF_SYMBOLS.tsv adds 71).
- RENAMES: 0x00413520 only (catalog CCountedPointer_004132f0 -> CCountedPointer<CConsole>, file renamed); the other catalogs already say
  CCountedPointer<T>::Reset.
- Repo untouched by this run (find -newer: only docs/pipeline/GOTCHAS.md, not written here: another session).

## Next
- Reset rows held for RTTI (9) and DD_OTHER (35): run tools/rtti428.py on them; the deleter rows (OperatorDelete_thunk / Destroy_<d> / DeleteData@@AutoTinyVirtualObject stand-ins) first: their
  bodies call T's destructor or ??_G (a second source), then these Resets become DD_OK.
- ctor(T*) DD_OK (4: 0x0041956C, 0x0065E198, 0x0065E2E0, 0x0085A840): same two sources; needs the header ctor form measured.
- D53 / SH67: ruling R-NF428-1.
- hold_r1 row texts carry the generator's standard comment, which cites the RTTI source; the 9 RTTI holds lack it (RESULTS says so). Fix the comment if they are regenerated.
