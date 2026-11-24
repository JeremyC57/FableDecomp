# NF370 (Account A naming queue, 2026-11-24): tracker T0 (26) and T2 (43)

Result: T0 4 renames proven (catalog only, no file move), 1 proposed for a ruling (0x0040EC50), 21 skipped for identity. T2 10 rows ACCEPT in three land_scripted run dirs
(batches/b1 access labels 6, b2 const 2, b3 virtual added 2); 33 skipped with evidence (16 NF309 Q3 const, 3 port / header,
5 strong same-TU copy units, 9 binder units, of which 13 self fixes are already proven and listed in QUEUE_PROPOSALS.tsv).
No repo write, no git. All NF370 locks read `released` (evidence/locks_final.txt, evidence/locks.log).

## Stay-off screen (evidence/screen.txt)
None of the 69 addresses is in Account B's pending packages as a row, in UNMATCHED_ROWS, in T3_PORT_QUEUE column 2 / 3, in Codex
RESERVED, or in NF366 / NF369's lists. The COORDINATION_CLAIMS hit is this agent's own claim line; acctB hits are header copies / dossiers.
Compile-only census candidates: 0x004CAA00 (B), 0x004FE630 / 0x006F91E0 / 0x00A36DF0 (T3) were NOT compiled; 0x00652FA4 (B parked)
and 0x009DCA80 (T3) were not compiled as link binders.

## T0 (t0/: RENAMES.tsv in NF360 form, CATALOG_EDITS.tsv = old -> new per field, T0_EVIDENCE.tsv = two sources per row)
Every T0 row's only spelling is `(self)`, so "callers agree" is vacuous; the second source has to come from retail. Method:
xrefs370.py (one pass over Fable.exe: E8 / E9 targets and absolute pointers; control 0x0099EAE0 found 30,945 calls), the body against
the Ego_r layout, and a rival scan (rivals370.py: Ego_r classes with the same non-trivial member signature, kept only when a PDB name
of the same kind exists). Rule applied (advisor): a no-caller COMDAT with a PDB-named rival is skipped.
- ACCEPT (4): 2 x ??1CSubTitle (Ego_r layout unique), 0x009F98D0 list<CProfile*>::remove (only caller 0x009F9000 = RemoveProfile shape: add ecx,0x4C,
  ProfileList at +76), 0x00A70130 ~pair<CCharString,C3DMeshVolume> (sibling copy 0x00A6F390 called from the pair __unguarded_partition).
- Compile check (lock, precheck, VC7.1): 0x004133A0 / 0x0040EC50 HEAD objects define the PDB ??_G as a pick-any COMDAT AND a strong
  stand-in with the same bytes (`?CLandscapeBackgroundPatch_vector_deleting_destructor@@YI...` / `?VecDel@T@@QAEPAXI@Z`); the grade's
  "largest function" fallback (equal sizes, first wins) graded the ??_G, which is why self_symbols.tsv carries it (evidence/syms_t0.txt).
  The T0 rename does not touch that stand-in (debt, queued).
- After review: the 6 ??_GCWideString rows are out. All-size rival scan (evidence/rivals_cwidestring.txt): Ego_r CVirtualKeyboard
  (16 B, CWideString at +0, rest scalar) has ??_GCVirtualKeyboard@@QAEPAXI@Z, same /O2 body, so the 5 no-caller copies are skipped;
  0x0040EC50 (caller 0x00E7FFF4 steps 4 B elements, which CVirtualKeyboard is not) keeps CWideString, but its object also strongly
  defines the stand-in ?VecDel@T@@QAEPAXI@Z and its plain Leaf grades the ??_G only by the equal-size fallback: not T0-shaped (tracker
  rule: T3 self), so its rename is t0/DECISION_*.tsv only (ruling 3). The CSubTitle / pair rows also define a strong helper
  (DestroySubTitle / DestroyNamedMeshVolume, different bytes, "not part of the retail body"), but their prefix Leaf names the ??1 COMDAT
  directly (GOTCHAS agentFAM13 form), so they stay T0.
- 0x00C3F600 / 0x00C596A0: coordinator hold (2026-11-24): propagated label shared by ten libpk / CODA addresses (NF372); skipped.
- File names by canonicalize_landed_names.make_base (the 2026-11-22 T0 convention); only the 6 CWideString rows move files (src + test).
  Old basenames appear only in generated indexes (rebuild/manifest/functions.tsv compiled_source, rebuild/compile-gate/vc71-compiled.tsv,
  rebuild/ARTIFACT_INDEX.tsv, old naming indexes); no source, header, test or tool includes them (no funclet include).
- Note: ??1CSubTitle is one PDB name on several addresses (pick-any copies retail kept, unreferenced); SHARED_PDB audit.

## T2
Route: a copy of lane_tools/access_fix.py (tools/access_fix370.py: --gen-only, --any-fix for tracker fix `other`, --open, --overload-ok;
and a fix: OPEN.tsv column 8 is now `landed`, which the original reads as an alias and skips every row with "alias y"). Hand edits for
nested scopes and base-clause models: tools/mkman370.py, proven by tools/vrun370.py (the same proof: vland HEAD then new at one path,
graded symbol = new Leaf, objsec ALL_EQ under old -> new, grade PASS).
Per accepted row: test precheck before the lock; HEAD and new source prechecks after it (work/pre_*.txt, work/pre.log); grade HEAD and
new MATCH / RELOCATION_MATCH PASS; objsec ALL_EQ; pk_all + pickany_vs_retail HEAD v new with the full old / new self symbol as --leaf
(the HEAD Leaf of several rows is missing or a prefix: pk_all then grades the largest function, which first produced a false NEWLY_NEQ on
0x00736590) -> gate PASS, reachflip 0; binders: table lines (old spelling: none) + census370.py (every landed source naming method and
class; declarations' letters) + HEAD compile of all 58 OLD / NONE / OTHER candidates (refhits_t2.tsv: none references the old spelling);
no other strong or in-class definition of the PDB symbol (strongdefs370.py, inlinedefs370.py); link {self + table binder} HEAD
unresolved -> new resolved, 0 duplicates (evidence/link_*.txt).
- b3 virtual rows: the RTTI vftable of the PDB class holds the row (rebuild/manifest/rtti-vtables.tsv: CTCTavernGameCardBase 0x01288294
  slot 33 = 0x008F1720; CThingPhysical 0x01244E84 slot 3 = 0x006A5AF0), both PDB sets give U; the local base is already polymorphic, no
  offset moves, objsec ALL_EQ means no new ??_7 / ??_R COMDAT (NF299 Q3). In the local model the new virtual sits after the base's
  placeholder slots, not at the retail slot; nothing in either TU calls through it.
- Two link binders not compiled (stay-off): 0x004348D0's 0x00652FA4 (B parked) and 0x009FF8D0's 0x009DCA80 (T3); their table lines bind
  exactly the new symbol.
- Findings: (1) reloc_bindings misses old-spelling binders: 0x00736AD0 (UND, compiled) and 0x00A36DF0 (text) bind the old self
  spellings with no table line (NF311). (2) Strong same-TU copies of the PDB symbol in other rows (0x00951FB0, 0x0099F9B0, 0x00652FA4,
  0x008F99A0) turn a T2 self fix into a unit (LNK2005, shown for 0x0094FBB0 in evidence/link_0094fbb0_00951fb0.txt).

- Const skips rechecked against the PDB: 0x005FC840's callee TeleportHeroToTeleporter is QBE in both sets (R1 candidate, not run);
  0x00A25530's BeginLoadingStats is QBE but its owner 0x00A24A80 cannot take const (blocked); the others force const onto callees in no
  PDB set (stand-ins) or assign a member (NF309 Q3 / mutable).
- Shared headers: no rebuild/include header declares an accepted member with the old letters (evidence/hdrdecls.txt).
- done-partial: 00b8fd40, 004348d0, 0082a500, 009f7250, 006a5af0 still carry stand-in binder spellings at their addresses.
- GOTCHAS: before the first compile I read docs/pipeline/GOTCHAS.md lines 1-120 plus a keyword grep (T0 / access_fix / pk_all / reachflip
  / objsec / land_scripted / Leaf ...), not the whole file.
- land_scripted holds every ACCEPT while the NF370 COORDINATION_CLAIMS line is open: close it before landing.

## Rulings wanted
1. The 11 unreferenced ??_GCCharString rows: accept CCharString on spread (11 TUs across the binary) over the FableWin-only rival
   ??_GCShaderParserSymbolSignature (lib_shader_data_bank.obj), or keep them T3 identity?
2. The 13 proven-but-held T2 self fixes (QUEUE_PROPOSALS.tsv): run as units (self + binder respell, or drop the strong copies), or
   hand to the coordinator's unit_fix route? Their candidate texts are run1/rows + run3/rows.
3. 0x0040EC50: rename the catalog onto CWideString::`scalar deleting destructor' now (t0/DECISION_*.tsv), or only with the self port
   that drops the ?VecDel@T@@ stand-in (same question for any ??_G row whose Leaf is the plain `vector_deleting_destructor')?

## Tools (tools/)
nf.py / pass.py / chain361.sh / pkcmp346.py / objsec352.py etc. (NF368 copies retargeted); new: access_fix370.py, mkman370.py,
vrun370.py, pkrun370.py, triage370.py, refcand370.py, census370.py, refhits370.py, strongdefs370.py, inlinedefs370.py, xrefs370.py,
rivals370.py, hdrdecls370.py, t0table370.py, refchain370.sh, linkchain370.sh, mkbatch370.py.
Run dirs run1/ run3/ hold the vland oracles (retail bytes): never copy them into the repo; batches/ and t0/ carry no retail bytes.
