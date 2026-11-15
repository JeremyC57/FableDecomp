# Verified MultiStrike match

Selected: main47_verified_header_grade; accepted merged_headers_43.

The entire1088-byte assigned span matches after relocation masking:1071code bytes,1alignment byte,16jump-table bytes. All85code/data operands bind correctly, including internal case labels and the intelligent-pointer vtable fallback. Eight reachable helpers also match,779bytes; helper bytes are excluded from reserved-row progress.

Fresh behavior tests passed254016cases on each ofVC7.1 /Od /W3 /GR- withoutEH andGCC64C++03. The exact production body is pasted into the self-contained test; its sole success output is007ccf50_TEST PASS. The final Grade-comment edit was recompiled, and all emitted named bodies and symbolrelocations remained identical to the tested source. Native shared-header checks verify11sizes31offsets; old/default headers retain their preprocessed tokens. All85accepted rows were freshly rebuilt against the combined overlay.

Final fix: restore the actual CTCMultiStrike user-method declarations in original PDB order, leaving its compiler-generated copy constructor implicit. Both original PDBs distinguish generatedcopy attr0x103 from userdtor attr0x7. Keeping an explicit declaration of that generatedcopy retained13wrong load-order bytes; removing it closed them. No synthetic filler declarations, local counters, fake helpers or compiler patch was used.

Two original local names were normalized to ptcEmitter1/2 to satisfy the package substring ban. Their exact original BPREL spellings stay in evidence and ROW.json; no emitted symbol changed. Older parked notes/probe snapshots are historical, not the current selection.
