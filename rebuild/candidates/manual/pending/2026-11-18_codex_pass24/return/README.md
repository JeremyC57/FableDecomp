# Applying the shared headers

PATCHES.json contains 135 file entries. Each names the affected rows, original and updated SHA-256 hashes, and exact old/new text. An empty original hash identifies a new file. Full header snapshots under this directory correspond to the updated hashes.

On the supplied baseline, apply each header's changes in reverse original line order, checking the old text at every position. Check the resulting hash. On a newer tree, find the full old context before applying each change and resolve any conflicting context manually. Preserve the original identities of line-numbered assertions.

All 100 submitted production sources compiled against the same final include tree. The package's preprocessing procedure produced identical tokens and identical assertion typedef names for all 600 supplied landed neighbour copies (578 distinct source contents). Both input trees preprocessed successfully for every comparison. NEIGHBOUR_PROOF.tsv has the per-source hashes. This proof covers the neighbours distributed with the handoff. Run the repository-wide header equivalence check when integrating the overlay into a newer checkout.

Baseline include-tree inventory SHA-256: aa6b2ed66077648adebd6ecdaa0849489cfa38eaf1cf6a8962844d0d1be50c88
Final include-tree inventory SHA-256: 2d99d3051c746ea96ba8d9d89d3e4c87e2f0a970f402f104cf6f0496cbd1a7d5
