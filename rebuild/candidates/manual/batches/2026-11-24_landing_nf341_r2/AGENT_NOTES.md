# NF341 round 2, step 1: the existing REREAD macro in the four emitters (an alternative; RESULTS verdict HOLD_R1)

The gate condition "objsec ALL_EQ except the fixed copy" is NOT met as written; see R1. The candidate that meets every condition is ../r2b (step 2).

## Step 1: the existing macro, with no header change
- How the owner row gets its body: 0x0066C340's row is a local model. Its own CDefPointer::SetDynamicCast re-reads `other.m_p` after the
  release; it does not define a macro. That body is the one the existing CDefPointer.h opt-in `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`
  gives, which is proven on 0x00410890 and used by landed 0x0066B370, whose copy is already EQ.
- Change: one line in each of the four emitters, right after their `#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY`:
  `#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF341 r2: ...` (rows/ against orig/rows). There is no header change, so
  HEADERS.tsv is empty and hdr/ does not exist. The 13 off-limits GLOBAL_BODY definers and every other row are untouched.
- How I compiled: HEAD text and define text at one path each (work/one/<addr>/<addr>.cpp) against the repo include as it is now (no
  overlay). The objects are work/obj/H_<addr>_r2.obj and work/obj/<addr>_r2.obj. Clang prechecks pass, HEAD and new.
- Gate:
  - Grade: RELOCATION_MATCH before and after for all four (pass logs); vland --recheck-landed RELOCATION_MATCH PASS before and after
    (logs/recheck_head.log, recheck_new.log).
  - pk_all.py --tsv + pickany_vs_retail.py, HEAD v new (evidence/pk_gate/, comparison evidence/pk_gate.txt): 0 copies turned NEQ, 0 newly
    NEQ, 0 dropped. 7 copies FIXED in each checker:
    - GetDef<CMaterialDef> 0x0066C340 in all four rows;
    - GetDef<CEngineThemeDef> 0x0063F3A0 in 0x008F7950 and 0x007F0300;
    - GetDef<CThingBuildingDef> 0x004AB840 in 0x0082D5C0.
    GATE PASS. The planted control (sides swapped, evidence/pk_gate_control_reversed.txt) gives GATE FAIL.
  - objsec (evidence/proof1_r2.txt, classified in evidence/objsec_r2_classified.txt): the leaf section and every other section are equal,
    with three exceptions, all inline helpers or copies of the same template:
    (a) the GetDef<T> copies the gate lists as FIXED, now byte-equal to retail; the CMaterialDef copy also equals the owner row's object;
    (b) the SetDynamicCast<CDefClassBase const> helper COMDAT of the same T, which changes body (REREAD form) and has no retail address (NOADDR);
    (c) the Assign helper COMDAT of the same T, which is no longer instantiated: unreferenced and NOADDR.
    This is wider than "ALL_EQ except the one fixed copy": the same macro also repairs the row's other GetDef<T> copies. See R1.
  - Link (evidence/link_r2.txt; the owner's object with the 4 HEAD objects, then with the 4 new ones): 0 duplicates; nothing unresolved for GetDef / SetDynamicCast / Assign.
- Step 2 was built as well (../r2b) because objsec is wider here than the one fixed copy. The round-1 header text is withdrawn.

## Step 3: the dropped helpers (applies to r2b too: SetDynamicCast + Assign of CDefPointer<const CMaterialDef>; r2 drops Assign for 3 Ts)
- ref_scan.py (evidence/ref_scan_REFS.tsv) answered nothing: it returned TOO_MANY for both symbols and compiled none. The member-template
  symbol ??$SetDynamicCast... gives it no identifier, and ?Assign@ matches 120 sources.
- reloc_bindings.tsv: 0 lines for any of the six helper symbols (SetDynamicCast / Assign for CMaterialDef, CEngineThemeDef,
  CThingBuildingDef). The table keeps one witness per (symbol, target), so no landed row binds any of them at a retail site.
- Object scan (evidence/helper_refs_scan.txt): this covers only what I compiled, the 157 HEAD objects (the 148 GLOBAL_BODY rows + B's
  0x00839940, then the 4 emitters). Results:
  - no relocation outside a helper's own COMDAT names a helper;
  - no object holds one as undefined;
  - pk_all lists them as NOADDR (no retail address) in every row.
- They are inline members, so a TU that uses one emits its own copy. Dropping them from these four objects cannot leave another object
  unresolved.
- Conclusion: nothing references them, so the drop is part of the copy repair, which the ruling accepts.

## D3 count (no compiling)
- From round 1's HEAD pk_all evidence (evidence/pk/H_*.pkall.tsv, 149 objects): 123 leaf-called NEQ GetDef<T> copies in 109 rows.
  Of those, 120 are the global-index form GetDef<T>(long, CDefPointer<const T>*) (r2/evidence/d3_count.txt). pickany_vs_retail on
  the same objects: 113 LEAF-REFERENCED NEQ GetDef copies in 102 rows (it flags only direct leaf references). The 13 off-limits rows
  are not included. Round 1's "121 NEQ copies in 110 objects" came from my own census (tools/getdefcensus.py). That census found the
  owner address through reloc_bindings only, so it could miss a copy whose address pk_all finds from the leaf's own relocations, and it
  also counted copies that are not leaf-called. pk_all's 123 / 109 is the number to use.

## Ruling to confirm
- R1: step 1 passes the pk gate and the grade gate. Its objsec is wider than the one copy: 3 more GetDef<T> copies become EQ, and their
  NOADDR SetDynamicCast / Assign helpers change or drop. It is also a partial start on D3:
  - GetDef<CEngineThemeDef> 0x0063F3A0 is NEQ and leaf-called in 8 rows at HEAD (0x0063EB20, 0x007F0300, 0x0081FA20, 0x0081FA90,
    0x0081FB00, 0x0081FCC0, 0x008201E0, 0x008F7950); step 1 fixes 2 of them.
  - GetDef<CThingBuildingDef> 0x004AB840 is NEQ in 7 rows (0x0082D3B0, 0x0082D5C0, 0x0082D6B0, 0x0082D6F0, 0x0082D7D0, 0x0082D870,
    0x0082D8C0); step 1 fixes 1.
  Step 2 (../r2b) changes only GetDef<CMaterialDef> and meets every condition as written. Choose one; I recommend r2b now, and the REREAD
  define as part of the D3 port.
- B's 0x00839940: my round-1 probe (B's preamble + REREAD) gave a 108 B copy, bytes_equal True, so B can take the same one-line define.
