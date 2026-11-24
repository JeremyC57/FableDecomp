# NF228 notes (2026-11-22, round 10): the nf227 follow-ups (q12589-q12604)

Base: the repo tree with nf227 landed. Locks NF228 (rows.txt; 0x0041E1CD / 0x0041E3F6 were `released`, the rest new mkdir). No Codex pass 27
row, 0x00A4EFC0 or 0x0042BEC0 touched. Tools: NF227's (re-pathed, tools/), owners.tsv + 3 lines (the new STLport spellings).
hdr/: two add-only opt-ins for 0x0041E3F6 (hdr/README.md). Flags lines: none. EH funclets: none. Descriptive names added: none
(every new spelling is the STLport one the repo headers produce, descriptive-by-library, or a PDB name).

## Entry 1 (q12590-q12596, q12601): the callee self rows -- done, 10 rows
- Explicit instantiations of the header members (the 0x008353B0 / 0x0042CE63 pattern), defines and pragma copied from the nf227
  binder row of the same tree: clear 0x0042C02F / 0x0042ACC3 / 0x0042AC76; _Rb_tree(const less&, const allocator&) 0x0042B76C /
  0x0042B7AD / 0x0042B7EE; _List_base<CActionParam*> ctor 0x0042AB01 (the 0x0042AB58 pattern, CActionParam from engine/NUISystem_CAction.h;
  the HEAD file's two unreferenced placeholder functions Copy_MemoryStepped / CActiveFile_CopyRange are dropped: nothing binds them);
  _M_find<long> 0x0042A879 (member template, the 0x0042B467 pattern; MATCH). 1 form each. Leaves read from the objects; each equals the
  spelling its binders bind (work/hb_e1.tsv: every leaf-bound binder of each address, compiled on today's tree / rows/).
- 0x0042D246 operator[]: FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE (1 form, RELOCATION_MATCH). Its map::insert pick-any
  copy equals 0x0042CE46 (the forwarder is kept out of line by VC7.1 here, no opt-in needed). Callees: 0x0042AE33 moves from the old
  2-argument model's _M_lower_bound to the STLport tree _M_lower_bound (2 -> 2, the self row 0x0042AE33 is a placeholder: NAME_FIXES);
  0x0042CE46 moves onto its own self leaf (2 -> 1).
- 0x0041E1CD SetMetaLayer ported onto engine/NUISystem_CManager.h (3 forms): f1 (bounds declared only) DIFFER 485v485: the `or [ebp+8],-1`
  / `and [ebp+8],0` key stores land after the value store (the key's address escapes into the opaque _M_lower_bound); f2 adds
  FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22 (the visible _M_lower_bound): RELOCATION_MATCH, but the template COMDATs take the TU-end mode
  `y off`, so the map::insert / _M_lower_bound copies got frame pointers (NEQ 0x0042CE46 / 0x0042AE33); f3 = f2 + a row-tail
  `#pragma optimize("y", on)` (Fidelity: scaffolding) that restores the command-line /Oy after the function: every owned copy EQ
  (operator[] 0x0042D246 69 B, insert 0x0042CE46, _M_lower_bound 0x0042AE33, ??8 0x0041C820). **Coordinator: confirm** (the nf226/nf227
  ruling "restores the TU default"; same device as 0x0041E3F6's HEAD row-tail `optimize("y", on)`). g_metaLayerBase (0x01230118) kept
  as at HEAD (only binder; no second source for another name).
- Census after: 0x0042A879 2 -> 1, 0x0042D246 3 -> 1, 0x0042C02F / 0x0042ACC3 / 0x0042AC76 / 0x0042B76C / 0x0042B7AD / 0x0042B7EE -> 1
  (self + binders on the STLport spelling), 0x0042AB01 3 -> 2 (the folded list ctor alias 0x005953E2, q12602, queued). Second layer
  (count-neutral, binder moved off a stand-in; self rows queued in NAME_FIXES): 0x0042B8FC 0x0042A264 0x0042A237 (_M_erase),
  0x0042AC9F 0x0042ACEC 0x0042AD10 (_Rb_tree_base ctors), 0x0042AE33 (_M_lower_bound). Note 0x0042A264 / 0x0042A237 share one self
  spelling ?BuildTreeArray@Tree@@QAEXPAUNode@@@Z (LNK4006 at HEAD, nf227).
- Pick-any (pkf.sh, work/pk_e1.tsv / pk_fin.tsv): every copy with a retail owner EQ; the rest NO-OWNER (allocator / alloc_proxy /
  _M_empty_initialize / less / _Select1st / iterator inlines).

## Entry 2 (q12597 / q12604): 0x0042BE7B -- done
NF227's work/0042be7b_f1.cpp regraded on today's tree: RELOCATION_MATCH (1 form; only a Leaf line and a landing note added). vtcheck: the
11-slot ??_7CObservable COMDAT binds every slot at retail 0x01230044 under that slot's self-row leaf (6 by table line, 5 with no table
line whose self Leaf equals the slot spelling). 0x0055CB10 SetEvent loses ?v00@CObservable@NUISystem@@UAEXXZ (2 -> 1); 0x0042AC0A moves
from the CRetailListBase stand-in to the STLport _List_base<CObserver*> ctor (2 -> 2; self row placeholder queued: NAME_FIXES). The object
also defines the unreferenced 5-B ~list<CObserver*> copy (the q12585 folded alias of 0x0042AC25 that 0x0041E641 / 0x0041E3F6 already
emit) and owner-less list / allocator copies. New test on the header (the HEAD test modelled CObservable locally).

## Entry 3 (q12598 / q12599): 0x0041E3F6 -- done (0x004299A8), partial (0x013B871C)
One draft with both fixes (1 form, RELOCATION_MATCH): SetIsUsingRelativeCoords is now the header static member (new opt-in
FABLE_RETAIL_CUISCREENUNITCONVERTER_SETISUSINGRELATIVECOORDS: Ego_r + FableWin.pdb); the front-end check reads
CNewFrontendGameComponent::Get() (engine/CNewFrontendGameComponent.h) and the new inline FABLE_RETAIL_CGAMECOMPONENT_ISRUNNING
(FableWin.pdb ?IsRunning@CGameComponent@@QAE_NXZ + the retail `cmp byte ptr [eax+9],bl` at 0x0041E558; Ego_r.pdb has no IsRunning: one PDB
+ the body). The row-local CNewFrontendGameComponent model is gone. Pick-any as at HEAD (34 EQ + the ~list alias) plus two new owner-less
inline copies (Get 6 B: the unlanded out-of-line Get at 0x00405FD0 is the same `mov eax,[0x013B871C] ; ret`; IsRunning 4 B). 0x004299A8 2 -> 1. 0x013B871C: 0x0041E3F6 moves onto the PDB spelling; 6 landed sources still use
the ?g_pNewFrontendGameComponent stand-in in code (source grep, comments excluded; not compiled: NAME_FIXES lists them). The HEAD test is updated
(rows/0041e3f6_test.cpp: the header class, its static set through a stubbed Init(); the header static member stub).
Debt seen (unchanged from HEAD): 0x0041E3F6 keeps local CDisplayManager / CSystemManager models (verifier SHARED_HEADER_AVAILABLE warnings).

## Entry 4 (q12603): tests -- partial
New header tests (self-contained, one case per condition, existing PassPatterns kept): the 12 changed rows, plus test-only rewrites for
the six nf227 STL rows 0x0042B7D7 0x0042BF85 0x0042B796 0x0042BF67 0x0042CE63 0x0042D28B (their landed sources unchanged; the tests print
their catalog PassPatterns). Stubs only (no class model): the STLport _Rb_global<bool>::_Rebalance (no-op: the tree stays a valid search
tree), and for the CCharString maps the CCharString ctors / dtor / conversion (counted, one key so no comparison runs).
Open (context): 0x0042BE50 and the 12 vtable-slot rows (0x0055CA40 0x0055C9C0 0x0055CAE0 0x0055CA90 0x0055CA00 0x0041DED2 0x0042D592
0x0041D98A 0x0041E3B2 0x0055C950 0x0042D4F0 0x0042D5AA) keep their HEAD tests.

## Decisions for the coordinator
1. 0x0041E1CD's row-tail `optimize("y", on)` (restores /Oy so the template COMDATs match their owners; without it two new copies are NEQ).
2. The two header opt-ins (hdr/README.md): IsRunning has one PDB (FableWin) + the body.
3. 0x0042AB01 / 0x0042BE7B keep the folded aliases (list ctor at 0x0042AB01; ~list<CObserver*> at 0x0042AC25), as queued at nf226 / nf227.
4. The 0x0041E3F6 test defines FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_INIT (test only) so a stubbed Init() can set the private static.

## Self-check
recheck_final.log: one vland --recheck-landed run over the 12 changed rows + the 6 test-only rows. work/recheck_12rows.log: the 12 rows
with their tests (all MATCH / RELOCATION_MATCH + PASS). Census: work/hb_e1.tsv, work/callers_e1.txt, work/callers_l2.txt,
work/absrefs_e3.txt. Symbol / relocation diffs HEAD v new: work/sc_e1.log and the sc.sh runs above (work/sym_* / xr_*).
- Final: recheck_final.log 18/18 MATCH / RELOCATION_MATCH + PASS, no RELOC_CONFLICT (verifier warnings: RELOC_NAME / STANDIN_LEAF = the
  table and catalog lines TABLE_CHANGES / RENAMES move; SHARED_HEADER_AVAILABLE CDisplayManager / CSystemManager on 0x0041E3F6 = HEAD debt).
- Link (work/link_check.txt): the 12 changed rows + 0x0041E3B2 0x0041E641 0x0042CE46 0x0042AE33 0x0042B7D7 0x0042CE63 0x0042BF85 0x0041DED2
  0x0042BE50 0x0042AC0A (22 objects, overlay on): 0 LNK4006 / LNK2005.
- Header proof: pin_asserts run (CGameComponent.h: 1 pinned; NUISystem_CUIScreenUnitConverter.h: 0); includers listed in hdr/README.md.
- Locks: the six test-only rows 0x0042B7D7 0x0042BF85 0x0042B796 0x0042BF67 0x0042CE63 0x0042D28B read `released` and are claimed NF228 (rows.txt).
