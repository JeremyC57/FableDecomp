- (agentC6, 0x00BB3E80) A dead `if (0)` arm's record weight scales with its IL: calls / string args count, a plain-pointer condition
  counts ~0, a condition with an inline accessor counts AND moves early-record orders (L). Class-typed temporaries in a dead arm change
  live codegen. Dead arms inside a FORCEINLINE helper expanded many times can change frame / inlining.
- (agentC6) The shared release-assert macro (hdr/fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM) closes record-count ties that move only
  the "O" class of orderings; scan the count with tools/nat2.py site labels (2 s per grade) and keep conditions accessor-free.
- (agentC6, 0x00BB30A0) Frame one slot too big with dead param homes unused: put the preloading flag in an inner block that closes after its
  bail-out (and the textures array in a block after the morph info): VC7.1 then shares the dead cloth / primitive param homes (0x38 -> 0x34).
- (agentC6) The record-keyed SetRenderState orders can need FEWER records (row 0x00BB30A0) as well as more (0x00BB3E80); dead asserts only add.
  Measure a variant's shift with a ruler meter (tools/meter.sh: late `const ERenderPassType ruler_K = pass;` until the target flips).
- (agentC6) FableWin.pdb's address order is a free hint: inline COMDATs first instantiated by a function are listed right after it
  (Render2DClothPrimitive -> GetPixelShader2DDiffuse, Set2DAlphaMaskTextureMode, DisableTextureFiltering, GetCurrent2DAlphaMaskInfo, ...).
