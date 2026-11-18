# ESCALATE 0x00BE2CD0 CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables (DIFFER 1091v1088, /GS)
Best source: differ/00be2cd0.cpp (Ego_d locals + scopes verbatim, shared CArray / C2DCoordI headers, hdr opt-in
FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_GENERATIONTABLES). ref/00be2cd0/ has the retail disassembly; c2prio trace: tmp/prio_be2.txt.
Residue (one cascade, 341 diff lines): retail materialises const 1 in edi (0x00BE2D8C) for the 24 ofs-table stores in array
order (0 from ebx) and keeps it to `and edx,edi` in the y-loop prologue (0x00BE2E42), re-setting `mov edi,1` / `xor ebx,ebx` at
the y latch (0x00BE30C3); poly_within_quad stays in memory ([esp+20h]); vert of i=0 is ebx itself (= poly_verts[0]); the
tri-list cursor is &tri_list[poly_count*3+2] (`lea esi,[edi+esi*2+4]` 0x00BE2E4E). Ours: const 1 split to eax for the table
only (stores grouped by value), poly_within_quad in edi, cursor base +1 element, `mov bx,di` / `mov bp,di` copies.
Tried: declaration orders (egod2a), tri-list arm forms, needs_order_flip flag, FABLE_RETAIL_STLP_VECTOR (no change),
implicit `poly_verts[i] = vert;` (no change).
Best guess: the cursor base (+2 v +1) is a strength-reduction choice and the one non-colouring clue: the original probably wrote
the three tri-list stores through a different index expression or updated poly_count at a different point. Ask for 1-3
hand-written candidates for the tri-list block and the odd_square / ofs-table indexing.
