# NF343 progress (restart-safe notes)
- Read: BRIEF, BRIEF_NF_BATCH (incl. NF340), CLAUDE.md, COORDINATION, ASM_RULES, EXTRA_RULES, GOTCHAS, NF340 audit.
- Tools: tools/nf.py (NF339's, retargeted), pk.py (compile H/N at one path + both checkers -> evidence/pk_<H|N>_<a>.txt),
  objsec.py (H v N sections, allowed fixed-copy labels), rdis.py, srcof.py, zs_head.sh, one.py / pk_all.py (copies).
- Group 1 (0x00CD3D2E map<CCharString,STB>::operator[]): rows in work/g1.txt, locks held (plus 00cd3d2e owner).
  Prechecks: work/pre.log (clang), work/pre_zs.log (D1 cl /Zs for the ~map-specialization rows and VC71-header tests). Post-lock rerun done.
  Retail 0x00CD3D2E: size TU, frame pointer, every callee out of line (lower_bound 0xCD28BC, less 0x42AAB3, T() 0x7E72A0, pair ctor 0xCD27C7,
  insert(it,v) 0xCD3CAE, ~T 0x7E74D0, ~CCharString 0x99EAE0). Owner row source sets optimize("y",off) + ("s",on).
  Plan: size-pragma opt-in (EXTRA_RULES 2026-11-20) in engine/fable_script_cutscene.h (already includes fable_map.h + STB header).
- HEAD compiles (evidence/pk_H_*.txt): all 11 rows + owner RELOCATION_MATCH; copies 141..315 bytes, NEQ, leaf-called.
- Isolation probes on 0x00CD3D2E (work/probe/p1*.cpp, evidence/pk_P1*): size TU on shared headers.
  p1a (defaults) 119v118; p1b (+TREE_BOUNDS_OUTOFLINE) 112v118 (less inlined); p1d/e (+LESS_INLINE, FORCEINLINE_LESS, LESS_SIZE_BODY) 115v118:
  only residue = `V()` vs STLport __default_constructed (lea re-address). optimize("y",off) NOT needed: optimize("s") alone gives the ebp frame,
  and pair ctor then EQ 0xCD27C7 (30 bytes, no frame). less EQ 0x42AAB3 (41).
- Census: work/census_g1.tsv = 74 landed rows likely emit this operator[] (STLP_MAP, no INDEX_DECL_ONLY, index use); audit saw 11.
- Group 2 (~MovieBase 0x006E7B80): 25-byte copies (PImp release called, 0x6E7AB0) v retail 66 (expanded). Not decided by opt-ins; inline
  decision inside the implicit dtor COMDAT (T36/T66 rows 25 bytes, T51 row f00c90 66). Likely skip (forcing device / leaf change).
- Next: row probe 00ea59e0 (work/probe/ea_A = BOUNDS_OUTOFLINE + less trio + blockA, ea_B = BOUNDS_OUTOFLINE + blockA).
- Row probes on 00ea59e0 (work/probe/ea_*.cpp, evidence/pk_P?_00ea59e0.txt; pkdiff via tools/pkdiff.py):
  PA/PB (explicit specializations defined BEFORE the leaf): leaf DIFFER 6844v6780 (callee summary changes the leaf). Dead.
  PC (declare before leaf, define at row end): leaf RELOCATION_MATCH; operator[] 120 (__default_constructed template not inlined).
  PD (existing INDEX_DECL_ONLY): leaf RELOCATION_MATCH, copy gone.
  PF (+ explicit __default_constructed<STB> specialization in the size tail): operator[] EQ 118, pair ctor/~pair/less/map::insert EQ;
     but STB copy ctor 0x905330 EQ->NEQ (45->39, now first defined inside the size region) = HOLD.
  PG (= PF + an unreferenced anonymous-namespace inline that copy-constructs STB BEFORE the size region, emits nothing): STB copy stays EQ.
     Collateral: new unreferenced ??M (39 v 40 NEQ, from LESS_INLINE), _Destroy<pair> 9->80 / ??_G pair 30->99 (NOADDR), tree/map
     lower_bound COMDATs 55->21 / 22->21 (NOADDR), 6 tree helper COMDATs dropped, __default_constructed COMDAT added (NOADDR).
- Group 2 finding: rows d58490 / e2ff00 / e3bb70 / ed17d0 carry a row-local `template<> CCountedPointer<MovieBase>::~CCountedPointer();`
  (decl-only specialization) -> the implicit ~MovieBase copy calls 0x6E7AB0 (25 bytes). de8370 / efa4f0 are flag-caused (NF344).
- 10:21: DONE. NOTES.md + STATUS.tsv written, every lock released. 0x00ED8680 measured (B/M/D). Next: final advisor call, then SubagentHandback.
