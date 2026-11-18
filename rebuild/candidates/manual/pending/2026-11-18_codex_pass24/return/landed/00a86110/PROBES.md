# CalculateDividingPlane probe history

The package baseline redeclared a class that now has a shared header. The first compiling port used that header. Copy assignment of the plane normal, division through the vector member, addition through the member, and a separate scalar dot-product result together gave all 546 instructions. Several source forms compile identically. The final rebuild restores the real axis table and scoped face indices.

| Source strategy | Grade | Result |
|---|---|---|
| package baseline | COMPILE_FAIL | — bytes and — differing instruction ranges |
| avg vector add | DIFFER | 2292 bytes and 146 differing instruction ranges |
| compound average | DIFFER | 2286 bytes and 146 differing instruction ranges |
| distance local | DIFFER | 2286 bytes and 146 differing instruction ranges |
| plane member assign | DIFFER | 2286 bytes and 146 differing instruction ranges |
| real headers | DIFFER | 1967 bytes and 6 differing instruction ranges |
| scoped indices | DIFFER | 2286 bytes and 146 differing instruction ranges |
| base vector div | DIFFER | 1967 bytes and 6 differing instruction ranges |
| base component product | DIFFER | 1975 bytes and 8 differing instruction ranges |
| base component order | DIFFER | 1965 bytes and 37 differing instruction ranges |
| base return product | DIFFER | 2003 bytes and 81 differing instruction ranges |
| copy assign vector div | DIFFER | 1967 bytes and 3 differing instruction ranges |
| copy assign component product | DIFFER | 1975 bytes and 5 differing instruction ranges |
| copy assign component order | DIFFER | 1965 bytes and 35 differing instruction ranges |
| copy assign return product | DIFFER | 2003 bytes and 78 differing instruction ranges |
| via initialise vector div | DIFFER | 1967 bytes and 6 differing instruction ranges |
| via initialise component product | DIFFER | 1975 bytes and 8 differing instruction ranges |
| via initialise component order | DIFFER | 1965 bytes and 37 differing instruction ranges |
| via initialise return product | DIFFER | 2003 bytes and 81 differing instruction ranges |
| const source vector div | DIFFER | 1924 bytes and 33 differing instruction ranges |
| const source component product | DIFFER | 1932 bytes and 33 differing instruction ranges |
| const source component order | DIFFER | 1922 bytes and 56 differing instruction ranges |
| const source return product | DIFFER | 1958 bytes and 92 differing instruction ranges |
| copy ctor vector div | DIFFER | 1967 bytes and 6 differing instruction ranges |
| copy ctor component product | DIFFER | 1975 bytes and 8 differing instruction ranges |
| copy ctor component order | DIFFER | 1965 bytes and 37 differing instruction ranges |
| copy ctor return product | DIFFER | 2003 bytes and 81 differing instruction ranges |
| end member reuse inline dot | DIFFER | 1957 bytes and 69 differing instruction ranges |
| end member reuse reverse dot | DIFFER | 1957 bytes and 69 differing instruction ranges |
| end member reuse scalar distance | DIFFER | 1957 bytes and 68 differing instruction ranges |
| end mul self inline dot | DIFFER | 1979 bytes and 75 differing instruction ranges |
| end mul self reverse dot | DIFFER | 1979 bytes and 75 differing instruction ranges |
| end mul self scalar distance | DIFFER | 1979 bytes and 73 differing instruction ranges |
| end named factor inline dot | DIFFER | 1969 bytes and 3 differing instruction ranges |
| end named factor reverse dot | DIFFER | 1969 bytes and 3 differing instruction ranges |
| end named factor scalar distance | DIFFER | 1975 bytes and 5 differing instruction ranges |
| end operator div value inline dot | DIFFER | 2003 bytes and 78 differing instruction ranges |
| end operator div value reverse dot | DIFFER | 2003 bytes and 78 differing instruction ranges |
| end operator div value scalar distance | DIFFER | 2003 bytes and 78 differing instruction ranges |
| end plain inline dot | DIFFER | 1967 bytes and 3 differing instruction ranges |
| end plain reverse dot | DIFFER | 1967 bytes and 3 differing instruction ranges |
| end plain scalar distance | DIFFER | 1967 bytes and 3 differing instruction ranges |
| end single expr inline dot | DIFFER | 1979 bytes and 75 differing instruction ranges |
| end single expr reverse dot | DIFFER | 1979 bytes and 75 differing instruction ranges |
| end single expr scalar distance | DIFFER | 1979 bytes and 73 differing instruction ranges |
| end vector add inline dot | DIFFER | 1967 bytes and 2 differing instruction ranges |
| end vector add reverse dot | DIFFER | 1967 bytes and 2 differing instruction ranges |
| end vector add scalar distance | DIFFER | 1973 bytes and 5 differing instruction ranges |
| schedule add | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule add plane group product | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule box center | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule centroid | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule centroid group product | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule dot | MASKED_MATCH_PENDING_AUDIT | 1967 bytes and 0 differing instruction ranges |
| schedule group product | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule group sub | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule group sub group product quotient dot | MASKED_MATCH_PENDING_AUDIT | 1967 bytes and 0 differing instruction ranges |
| schedule plane | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule quotient | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule quotient add dot | MASKED_MATCH_PENDING_AUDIT | 1967 bytes and 0 differing instruction ranges |
| schedule scale | DIFFER | 1967 bytes and 2 differing instruction ranges |
| schedule scale add | DIFFER | 1967 bytes and 2 differing instruction ranges |
| proven table | MASKED_MATCH_PENDING_AUDIT | 1967 bytes and 0 differing instruction ranges |
| donor indices table | MASKED_MATCH_PENDING_AUDIT | 1967 bytes and 0 differing instruction ranges |
| main combined 11 | MASKED_MATCH_PENDING_AUDIT | 1967 bytes and 0 differing instruction ranges |

The selected result is RELOCATION_MATCH (1967 object bytes, 1967 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
