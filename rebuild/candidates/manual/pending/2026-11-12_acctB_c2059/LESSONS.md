# Chunk 2059 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- (0x00BC25F0) SetRenderState bookmark test / or operand order ("lane gap"): when dead-local rulers do not move the pattern at all, look
  for statements that set constant states directly where a sibling arm uses a helper: `SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
  SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);` folds to the same three SetRenderState expansions as direct
  SetBlendOp / SetSource / SetDest calls, but its extra (folded) IL moved the front transition from block 8 to block 2 and matched the row.
- (0x00BC25F0) An address-taken `bool failed` for GetDiffuseTexture at loop-body scope conflicts with every later temporary of the body
  (frame 0x6C); in its own block `{ bool failed = false; texture = GetDiffuseTexture(...); if (failed) continue; }` it shares the dead `sub`
  parameter home with the fild conversion temporaries (retail frame 0x68).
- (0x00BC25F0) A clamp whose original value stays in one register and the clamped copy in another (`cmp eax,0Ah ; mov ebp,eax ; jb ;
  mov ebp,0Ah`) is `x = Get(); y = x < 10 ? x : 10;` -- `if (y >= 10) y = 10;` on one variable loads straight into ebp.
- (0x00BC25F0) A reference to the destination constant block taken BEFORE an inline conversion constructor (`CRGBFloatColour colour(c)`)
  decides which registers the converted bytes and the block address get (edi / ecx v ecx / eax).
- (0x00BC25F0) An upload count that equals the batch test's right-hand side is pushed from that register when the source passes the same
  expression (`batch_size * 4`), not the counter.
- (0x00BC25F0) FableWin's COMDAT run after a function listed exactly the inline helpers it needed (GetReadZBuffer, Apply*VertexShader,
  GetRepeatingMeshBatchSize, HasValidDiffuseMap); FableWin debug sizes (17b/9i) showed GetRepeatingMeshBatchSize is a plain getter (the
  clamp is in the caller).
Review at landing:
- 0x00BC25F0 needed 22 authoring grades (first RELOCATION_MATCH at grade 49 of the log; the rest are diagnostic rulers / probes).
- Landed rows bind wrong spellings for 0x00A23C50 / 0x00A23C60 (NAME_FIXES.tsv): 0x00B9E950, 0x00BD1F70, 0x00B991D0, 0x00B9CE10.
- vland WARN RELOC_NAME for 0x00A23C50 / 0x00B86A00: the manifest labels are Nop_ret4 placeholders; apply LABELS.tsv.
- The overlay's new opt-ins are used only by this row; SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode bodies are the same text
  as chunk 2045's 0x00BB8EF0 (row-defined, FableWin-only members).
