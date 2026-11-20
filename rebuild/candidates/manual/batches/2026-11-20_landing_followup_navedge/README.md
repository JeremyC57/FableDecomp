# Landing (Account A, 2026-11-20, on the owner's instruction "land the matches here in main chat now")

- New rows (strict vland `--land`, `logs/vland_land_new.log`; oracle lines read from Fable.exe, `oracle_new.tsv`): 0x00A75B60 MATCH,
  0x00A793D0 / 0x00A79680 RELOCATION_MATCH, tests PASS. 0x00A75B60 first got its manifest start: a boundary-starts.tsv row with B's
  evidence, merged by `crawl/manifest_add_boundaries.py --write`.
- Re-ported landed rows (B's src / test copied over the catalog files, which had not changed since 2026-11-11; strict
  `--recheck-landed`, `logs/vland_recheck_landed.log`): 0x00A773E0 MATCH, 0x00A78060 / 0x00A780B0 / 0x00A786C0 / 0x00A78800 / 0x00A78A90
  RELOCATION_MATCH, all PASS. Binding table: one set shrank (0x00A77420 2 -> 1); the only new entries are first bindings.
- 0x00A82FA0 (added here): the new rows store it as CCountedPointer<CNavConnectorNode>'s DeleteFunc under the Ego_r.pdb spelling
  ?DeleteData@?$CCountedPointer@VCNavConnectorNode@@@@SIXPAX@Z, while the landed row defined a made-up extern "C" OnDie (catalog label
  CTCMapwho::OnDie): two names for one address. Re-ported in the landed 0x00BB6330 pattern (explicit instantiation of engine/CCountedPointer.h
  DeleteData on the new engine/CNavConnectorNode.h): RELOCATION_MATCH PASS (`logs/vland_00a82fa0.log`); files, catalog Module and oracle
  label renamed. This closes B's queued name fix (hand3c2 b5) for that address.
- Headers: engine/CNavConnectorNode.h new; CNavNavigableLeafNode.h / CNavQuadTree.h / CNavRegion.h / CNavSwitchableLeafNode.h take B's
  additions only (0 removed lines). hdrcheck2 (`logs/hdrcheck.log`): default path and every existing macro token-identical; the one
  changed macro, FABLE_RETAIL_CNAVREGION_CONNECTIONS, is new and unused by landed files. The pack pragmas are the headers' existing ones.
- One definition (`logs/link_owners.log`: the 10 objects + 13 owners of the COMDATs they emit): 4 LNK2005.
  - ForceRegionTo / IsEveryUniqueIDOpen: 0x00A78800's `// Fidelity: same-TU strong definition` copies of its retail TU-mates 0x00A75B60 /
    0x00A773E0, the nf96a rule (GOTCHAS, advisor decision 2026-11-19): retail expands one site and calls the other.
  - C2DBoxF(const C2DVector&, const C2DExtentsF&) vs owner 0x0041A7E0 and CANavQuadTreeNode::GetSize vs owner 0x00A75B30: strong owners
    against header inlines; the same clashes exist at HEAD with 0x0041AFA0 and 0x00A78E20 (`logs/link_control_head.log`). Queued as debt.
- B's NAME_FIXES.tsv lines were all queued at the 2026-11-20 release.

# navedge (followup author, Account B, 2026-11-20): CNavQuadTree map edges / CNavConnectorNode / CNavRegion / SetAsBlocked -- READY

Base: the audited set `$S/stage/audit_h3c2b/fixed_navedge` (8 rows), rebased on today's tree, plus one new row. The held row
0x00A78800 now matches WITHOUT `#pragma inline_depth`.

| Row | Function | Grade (strict, --recheck-landed) | Test | Mutants |
|---|---|---|---|---|
| 0x00A75B60 | CNavNavigableLeafNode::ForceRegionTo (NEW, uncatalogued) | MATCH PASS (shadow manifest, see below) | PASS | 2/2 |
| 0x00A773E0 | CNavSwitchableLeafNode::IsEveryUniqueIDOpen | MATCH PASS | PASS | audited set |
| 0x00A78060 | CNavRegion::RemoveConnectionToRegion | RELOCATION_MATCH PASS | PASS | audited set |
| 0x00A780B0 | CNavConnectorNode::~CNavConnectorNode | RELOCATION_MATCH PASS | PASS | audited set |
| 0x00A786C0 | CNavRegion::SetAsConnectedToRegion | RELOCATION_MATCH PASS | PASS | audited set |
| 0x00A78800 | CNavSwitchableLeafNode::SetAsBlocked (RE-AUTHORED) | RELOCATION_MATCH PASS | PASS | 22/22 |
| 0x00A78A90 | CNavConnectorNode::CNavConnectorNode | RELOCATION_MATCH PASS | PASS | audited set |
| 0x00A793D0 | CNavQuadTree::ConnectHorizantalMapEdge | RELOCATION_MATCH PASS | PASS | audited set |
| 0x00A79680 | CNavQuadTree::ConnectVerticalMapEdge | RELOCATION_MATCH PASS | PASS | audited set |

logs/vland_strict_recheck.log: all 8 catalogued rows PASS (WINS 8), only the known RELOC_NAME warnings (NAME_FIXES 0x00A7C0A0 /
0x00A82FA0). Rows other than 0x00A78800 / 0x00A75B60 are byte-for-byte the audited set's src / test (only 0x00A78800's test gained
two opt-in defines and a comment).

## 0x00A75B60 needs a manifest row
vland reports it OUTSIDE_MANIFEST (`rebuild/manifest/functions.tsv` has no start there). Evidence it is ForceRegionTo: Ego_r.pdb /
FableWin.pdb `?ForceRegionTo@CNavNavigableLeafNode@@IAEXPBVCNavRegion@@@Z`, defined in lib_nav_quad_tree.cpp (Ego_d lines 412..414,
.cpp file record; parameter `pregion`); FableWin orders RemoveRegionZero / ForceRegionTo / GetMemoryUsage / SetUpNeighbours, and retail
has RemoveRegionZero at 0x00A75B50 and SetUpNeighbours at 0x00A75B70 (both catalogued) with exactly this 10-byte body between them
(`mov eax,[esp+4] ; mov [ecx+10h],eax ; ret 4`; Ego_r's out-of-line copy 0x00BBB820 is the same 10 bytes). Proof run:
logs/vland_strict_00a75b60_shadow_manifest.log = strict vland with FABLE_ROOT pointed at a symlink tree whose functions.tsv has one
added row (`00a75b60 ForceRegionTo CNavNavigableLeafNode __thiscall void 2 ... discovered boundary`) -> MATCH PASS. Add the row
(manifest_add_boundaries / gaps path) before landing; name label `CNavNavigableLeafNode::ForceRegionTo`.

## 0x00A78800: how the inline split happens (no pragma)
Retail expands the same-TU IsEveryUniqueIDOpen for the node itself (0x00A78840..0x00A7886A) and CALLS it for the neighbours
(0x00A789AD). Traced c2.dll 13.10.3077 under gdb (probes/inl2.py = hand3c3's inl.py + hooks; probes/isite.sh prints the two decisions):
- decision 0x1078D1AA: cost = 50 (IL 53 - savings 3) + 104*53/254 (callee byte +0x4E) = 71; score = (cost/f + 6*cost)/7 for site
  frequency f <= 1.0, (cost/f + 2*cost)/3 above (0x10749C7C, k at 0x10881998); accepted when score < 74. So the neighbour site is
  called only if its static frequency is <= 0.77 of the entry.
- c2's static frequency: a `for` with a step expression leaves the code after the loop at the loop BODY's frequency (5x the loop's
  entry here); a loop whose step is the last body statement (or a while) does not. The two loops in front of the neighbour site (the
  first neighbour loop, the SetAsConnectedToRegion loop) each multiply it by 5.
- Ego_d text (both loops stepped in the header): site 2 f = 3.91, score 53, EXPANDED (DIFFER 526v494; probes/fw1.cpp). One loop
  stepped in its body: f = 0.78, score 73, still expanded (probes/ab1.cpp, ab2.cpp: DIFFER 526v494). Both: f = 0.156, score 125,
  CALLED; site 1 stays f = 1.0 / score 71, expanded -> RELOCATION_MATCH.
- Tried without effect on the frequency: `<` / `!=`, unsigned index, hoisted size, `continue` form, `!= NULL`, ASSERT dead arm, all
  FableWin call spellings below. The second neighbour loop stepped in its body makes the site hotter (7.03).
So the source has `for (init; test; )` with `++neighbour_it;` / `c1++;` as the last body statement of those two loops (the block
scopes Ego_d records for c1 / neighbour_it are kept). This contradicts Ego_d's line table (lines 617 / 649 hold the step), which is the
2012 revision; it is marked `// Inferred:` (2) in the source with the numbers. The other two loops keep Ego_d's header step.

The rest of the text now follows FableWin.pdb's debug body (FableWin 0x03282920, same revision as Ego_d; ghidra_out/decomp_navmesh2.c)
and Ego_d line sizes: `FABLE_RETAIL_ASSERT(it!=UniqueIDsOpen.end())` (line 607, 407 bytes; FableWin's DoErrorMessage line 0x25F),
`CNavNavigableLeafNode::GetRegion()` then `ForceRegionTo(NULL)` in the blocking arm (line 628: 14 bytes = a direct call),
`ForceRegionTo(*regions_connecting_to.begin())` (line 645, 77 bytes), `CSmallVector<...>& neighbours = *GetNeighbours()` (Ego_d type: a
reference), `!pneighbour->IsNavigable()`, `GFDebugPDynamicCast(pneighbour, (const CNavSwitchableLeafNode*)0)` (line 662: 13 bytes =
fastcall), `Unlock(*regions_connecting_to.begin())` (line 668, 74 bytes). Each is byte-neutral: the audited text with only the two loop steps moved
also grades RELOCATION_MATCH (probes/np_loops.cpp), and the FableWin text with the header steps grades the same DIFFER as the audited text; the `PRegion = regions_connecting_to[0]` / `pneighbour->Blocked` spellings of the audited text are gone.
`// Inferred:` lines: (1) the C-style casts; (2) the two loop steps (above). The 0x00A75B60 row: the const-to-non-const C-style cast.
Same-TU copies (`// Fidelity:`): ForceRegionTo = row 0x00A75B60's text verbatim; IsEveryUniqueIDOpen = row 0x00A773E0 verbatim.

## Header overlay (hdr/, hdr.diff; 0 removed lines; pin_asserts: nothing to pin)
- engine/CNavNavigableLeafNode.h: NEW opt-in `FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REGION` -- the class-body `virtual const CNavRegion*
  GetRegion() const { return PRegion; }` (Ego_d: header file record, line 221) and protected `void ForceRegionTo(const CNavRegion*
  pregion);` (0x00A75B60). Plus the audited set's FABLE_RETAIL_CSMALLVECTOR_RESIZE block (unchanged).
- engine/CNavQuadTree.h: today's tree (with hand3c3 b8's landed FABLE_RETAIL_CNAVQUADTREE_UPDATELINES) + the hand3c2 b5 block
  FABLE_RETAIL_CNAVQUADTREE_CONNECTMAPEDGES inserted after it, by block.
- engine/CNavConnectorNode.h NEW (audited set), engine/CNavRegion.h, engine/CNavSwitchableLeafNode.h: the audited set's blocks; those
  three headers are unchanged in the tree since the audit, so the overlay copies are current.
- Existing opt-ins now used by 0x00A78800: FABLE_RETAIL_CANAVQUADTREENODE_ISNAVIGABLE, FABLE_RETAIL_ASSERT_DEAD_ARM (fable_assert.h),
  engine/GFDebugPDynamicCast.h.

## Mutants
0x00A78800: mutants/00a78800.tsv, 22 decisions, 22 killed (mutant_logs/00a78800.log; three that mut2 reported SAME_CODE because
the relocation-masked bytes cannot see a callee swap, and one whose first occurrence was in the comment, re-run with MUT_NO_SAMECODE
/ occurrence 2: mutant_logs/00a78800_nosame.log, all KILLED). 0x00A75B60: 2/2. Other rows: the audited set's logs (copied).

## NAME_FIXES
NAME_FIXES.tsv = the audited set's file (unchanged; nothing new). New name: 0x00A75B60 CNavNavigableLeafNode::ForceRegionTo (catalog add).

## Note for the other author (item 5, 0x00A77420 Unlock)
Retail Unlock also CALLS IsEveryUniqueIDOpen from its neighbour loop (0x00A7745A). On the FableWin text (ASSERT(IsEveryUniqueIDOpen()
&& Blocked) dead arm, ForceRegionTo, the same loop) c2 puts that site at f = 1.25 (score 66: expanded), and no loop spelling gets it to
<= 0.77 (stepping in the body makes it hotter). So Unlock's call has another cause (or the landed /Gr + auto_inline(off) text stays);
not investigated further here.
