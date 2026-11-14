# acctC workflow hand-back wfA3b (unworked rows round 2, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00ACDB80 | QuadricOptimiserInternals::COptimisedPrimitive::COptimisedPrimitive | 2674 | RELOCATION_MATCH (`/Gr`) | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; its review
items (inline-budget devices, fitted spellings, label warnings) are for the owner / landing account.
`open/` holds the best drafts of the batch's unmatched rows (residues in NOTES.md).

## 0x00ACDB80
Verifier audit: none. No __asm/naked/_emit/volatile, no local class or pad struct (it now uses the shared QuadricOptimiserTypes.h and COptimisedPrimitive.h; strict-header vland raised no HEADER_POLICY or SHARED_HEADER_AVAILABLE), no dead code. `// Flags: /Gr` is backed by a landed same-TU sibling: the _M_insert_overflow row at 0x00AA94E0 carries /Gr, and the source also cites the __fastcall callees. Header diffs are additive FABLE_RETAIL_* opt-ins only. The only lines removed are FABLE_STATIC_ASSERTs, each re-pinned as an _AT form with matching counts. Warnings to report: 6 RELOC_NAME (catalog labels CShaderPreParser::GetConstantCount, CWound::Kill, GetMember_dword0x2c and std::std::vector_InsertFillN_* on the Ego_r-named callees; the author handed these back in NAME_FIXES.tsv). Review item: the author asks the coordinator to accept an #ifdef alternative to the COptimisedVertex member line; the default text is untouched. Stale text: an older NOTES paragraph still says the row uses row-local quadric classes, but the FIX-UP section and the source show they are gone. Test: a real state model with about 10 conditional checks, printing PASS or FAIL.

Fitted items: NOTES: FITTED = the statement spellings carried over from the earlier near-miss (named weight-loop locals, declaration order in the vertex and unique-vertex loops) and the GetVertex use sites (insert loop and weight loop). EVIDENCED = shared STLport text, accessor names and texts (PDB + fwsize), the long bone index (PDB), and all four new opt-in texts. Nothing is marked FITTED in the source header.
