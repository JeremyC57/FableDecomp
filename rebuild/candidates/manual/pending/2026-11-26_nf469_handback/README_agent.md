# NF469 hand-back (2026-11-26): r467a decided (implicit), step 2 re-gated

No repo writes, no git, no `wineserver -k`. Continues NF467 (all 42 NF467 locks taken over).

## r467a: IMPLICIT. Full evidence in R467A.md
- Both PDBs flag `~CAIStateGroupBase` `public virtual compiler-generated` (controls ~CBaseClass / ~CCharString are not).
- VC7.1 bytes do decide here: implicit = 19 bytes, no vptr store; any declared form = 25 bytes with a vptr store (the class is not novtable:
  retail 0x008FD255 stores 0x01288C60). Retail 0x008FD288 = 19 bytes, no store.
- Retail placement: COMDAT run of the base ctor's TU (FableWin: ai_brain_tables.obj holds the base ctor/dtor and 214 derived ctors/dtors).
- (b) HEAD does not coexist: 102 objects emit an EQ pick-any copy and 11 a NON-EQ one, beside the strong 0x008FD288 (LNK2005 / LNK4006
  measured with real objects). `_VIRTUAL_MODEL` view rows emit nothing; 640 local models only declare (142 explicit callers reference it).
- (c) harness exp/q1link_r467a: sel 1 beside sel 2 = LNK2005 in either order; all pick-any links clean.

## Batches (NF464 layout; land in this order)
| batch | rows | verdict |
|---|---:|---|
| b0_hdr | 3 headers | unchanged from NF467 (add-only opt-ins; orig/ = repo today); needed by b4 rows 0x008FDE1B / 0x008FD459 / 0x008FEB3D (HEADERS_NEEDED_BY repointed) |
| bR0_basedtor_declonly | 11 | HOLD r469a (ruling on the decl-only base dtor in their local models). Compile-graded + objproof only. |
| bR1_008fd288_thin | 1 | HOLD r469b (owner 0x008FE862) + T3 line 651 (nf303 census port of 0x008FD288's 6 stand-in binders). RELOCATION_MATCH, Leaf unchanged. |
| b4_aisg_ctor_fold | 40 (20 ctor + 20 thin ??_G) | HOLD r467a (until bR0 + bR1 land); 9 ctor rows (and their thin rows) also r467b/r469c. Every other gate passes. |
b1_standstill_pilot / b2_aisg_ctor_ready are superseded by b4 (same ctor texts). b3 (22) not redone (step 3 not started); step 4 not started.

## b4 gates
- vland HEAD v new: 40/40 RELOCATION_MATCH PASS both sides; WARN ctor 1-2 -> 0, thin 1 -> 1 (STANDIN_LEAF from the underscore catalog
  label; CATALOG_CHANGES.tsv relabels, GOTCHAS NF424).
- census (evidence/fullcensus.tsv): 20/20 OK, 0 BAD: each ??_G home's only retail referencer is its vftable slot 0 (emitted by the ctor row);
  no table lines; rel32 refs 0. NF467's table census 28/28 OK.
- link (20 new ctor objects, thin objects excluded as the same TU, all slot selves): with the reshape owner dup 2 -> 2, unresolved 295 -> 285
  (the 10 new names are r469c's misspelt selves); with the strong 0x008FD288 dup 2 -> 3.
- sandbox (light shadow, tools/sandbox469.sh): apply_table --dry 0 misses; bindings_multi_check 1 -> 1; descriptive_check 2,030 / 0 FAIL;
  s_unit_check 0 failed (20 thin members DEF-verified); **tracker: 20 addresses rtti_identified -> agree_pdb, none worse**.
- Catalog: CATALOG_CHANGES.tsv (20 relabels) + CATALOG_MANGLED.tsv (addr / mangled); SELF_SYMBOLS.tsv (20 respells + 20 unchanged ctors).
- T3 screen (T3_SCREEN.tsv): only 0x008FD288 (bR1) is in column 2/3.

## Other notes
- r467c per row (b4/evidence/r467c_align.tsv): 19 ctors start unaligned, 0x008FDE40 is 16-aligned; all 20 have no padding byte before the
  start (previous byte = a `ret`), so 0x008FDE40 rests on "no padding". Leaf identical with/without the pragma: NF467 objproof.
- Not run: bR0 / bR1 are compile-graded + objproof only (no vland pair, link or sandbox); the tracker was measured for b4 only; steps 3 (b3
  rework) and 4 (CInputProcess) not started.
- bR1 depends on its owner 0x008FE862: recheck 0x008FD288 when 0x008FE862 gets its wave-2 r459a port (15 slots).
- Correction to NF467: slot targets 0x00920710, 0x00924420, 0x0091D120, 0x00911A00 do have rows (b4 unresolved_slots.tsv).
- Scan completeness: 1,642 sources (a one-line mid-file model, 0x0090F770, added: `none`); in every kept definer ??1CCharString /
  ??1CBaseClass are UND; 0 of the 113 definers is a `_VIRTUAL_MODEL` view row. No other landed object defines the 20 ??_7 / ??_G.
- The base ctor 0x008FD255 port (work/bR_probe_008fd255) is not proposed: HEAD and port both emit a non-EQ ??_GCAIStateGroupBase (debt).
- Disk was at 100% (130-270 MB free) throughout; I deleted only my own files (PDB dumps, sandbox rec.json). NF467's work/ (581 MB) untouched.
- -I disclosure: repo tools apply_table / naming_status / descriptive_check / bindings_multi_check / s_unit_check ran with -I; vland /
  recheck / verify_and_land (via nf.py) without, as NF464/NF467.
- Locks: NF469 holds 74 (LOCKS.txt): the 42 from NF467, 0x008FD288, the 20 ??_G homes and the 11 bR0 rows; 0x008FD255 / 0x008FD29B taken then released (probe only).
