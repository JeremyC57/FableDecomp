# acctC workflow hand-back wfE6 (unworked rows round 3, 2026-11-15)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00B73760 | CEngineWaterRenderer::CEngineWaterRenderer | 6,799 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; review items are for the owner / landing account.

`open/` holds the best drafts of the batch's unmatched rows (full residues in NOTES.md):
- 0x00B6EDF0: near_miss, DIFFER(3144v3144). Not reopened in this run; earlier work stands. Equal length. 9 real lines remain, all in the order of the stores in the 14-vertex block. The DAG measurement shows v1 needs Y stored before X, which needs a different IL order or priority, not a count. Best sourc
- 0x00B70A90: parked, DIFFER(3567v3567). Swept all 7 combinations of the 3 repo SetRenderState opt-ins (plus none) against 0..16 dead release asserts, with and without the fitted unused0 local: 272 grades. Best is still the known 4 fg hunks, 2 of them real (0x00B716B7, 0x00B71724: the last setter run
- 0x00B5CA70: byte-matches but broke a lane rule; in the fix-up workflow, not staged yet.
- 0x00B4D370: parked, DIFFER(7160v7176). Not reopened: no measured lead. Residue: frame 0x3B4 v 0x3D0, x87 operand order, the corner pass's common subexpressions. Checked the Ego_r neighbours for TU static helpers: there are none (GFLimit<float,int,int> is already used). Best source: differ/00b4d370.
- 0x00B783F0: near_miss, DIFFER(9343v9343). One real hunk left: 0x00B79938, where the bookmark takes the mask/state registers swapped (retail mask in esi, ours in edx; agentC13 says this is a post-RA decision). Last-resort rulers closed the quartet (0x00B7945D/C1/525/589) and both pair sites (0x00B7A5D0

## 0x00B73760
Verifier audit: none. No __asm/naked/_emit/volatile, no inline controls, no if(0), no local class/struct (no ^class/^struct lines). The new header opt-ins (CENGINEWATERRENDERER_CTOR, CLANDSCAPETESSELATORBASE_INLINE_CTOR, CWATERPATCHTESSELATORBASE_INLINE_CTOR, CVERTEXSHADER_COPYCTOR) are additive FABLE_RETAIL_* blocks. Each cites PDB evidence for a callee retail inlines, not one it calls. No vland warnings for this row. Test: a transcription model of the shader lookup sequence, banks, flags and calls, run on two flag/PS-version cases. It checks real expected values, not an empty PASS.

Fitted items: none (header says 'No flags, no fitted spelling'; NOTES.md agrees)
