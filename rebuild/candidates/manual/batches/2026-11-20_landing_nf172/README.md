# Landing nf172 (Account A, 2026-11-20): render / shader / texture / engine names (21 rows)

Agent NF172 (round 10) handed back 33 entries: 18 done, 6 done-already, 6 blocked, 1 deferred (acctB lock), 2 open. Its files are
in `naming/2026-11-20_agentNF172/`. q5054 and the 2 open entries (q11918, q11978) go back to the queue unclaimed.

- 21 rows. Self renames (12), among them CRenderManagerCore::IsRendering / SetAWindow 0x00A0AA60 / 0x00A0AA90 (were CTCInventoryBase
  stand-ins), CEngineCamera::SetGameWindow 0x00B2FC00 (was a `C_00b2fc00::M` placeholder), CPixelFormat::FindFormatInfoIndexFromD3DFormat
  0x009E3620 (static, was a free stand-in), CEngine::SetPersistentPrimitive 0x00B23BC0, CEngineResourceManager::OnRestoreDefaultPoolResources
  0x00B3AFF0, CEngineScreenEffectRadialBlurRenderer::Render / ShowBlurTexture 0x00B5EA30 / 0x00B5E700 (derived shells dropped),
  CEngineSubPrimitiveRenderer::AddSubPrimitive 0x00B846E0, CEngineStaticMapFileBlockManager::GetBlockData const 0x00BE04C0,
  CShaderResource::GetPixelShader const 0x00A5D620, CProgressDisplay::RenderProgress private 0x00499880. Four files moved to their
  new names; six tests were rewritten (linker alias + recording stub).
- Headers (add-only `#ifdef` opt-ins, `orig` equals HEAD, pin_asserts pinned 0): CEngineStaticMapFileBlockManager.h (RELEASEREQUEST,
  GETBLOCKDATA_CONST), CEngineCamera.h (SETGAMEWINDOW), CEngineResourceManager.h (ONRESTORE), CGPUScratchBufferTextureManager.h
  (ENDSHUTDOWN), CEngineScreenEffectRadialBlurRenderer.h (RENDER), CEngine.h (SETPERSISTENTPRIMITIVE).
- No casts of stand-ins to real types and no new local model of a class with a shared header. 0x00A63200 / 0x00A63240's header-less
  local IDirect3DVertexBuffer now derives from the shared IDirect3DResource9 (duplicate slots 0-10 dropped).
- Recheck (`logs/recheck.log`): 2 MATCH + 19 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline, NullCoord).
  Includers (`logs/includer_recheck.txt`): 329 good, 2 bad: 0x00BE6F70 / 0x00BF4570 DIFFER with the same sizes as at HEAD (nf127
  differ_head_check), not this landing's.
- Split gate (`logs/split_gate.txt`) reports 2 "created" at 0x00BC04F0 and 0x00BE0930. Neither is new. The gate and tablecheck count
  only callers' bindings, and in both cases the self row's `// Leaf:` already defines the spelling the moved caller now binds
  (0x00BBC130 -> RenderSingleSprite(CEngineSubPrimitiveBase&), 0x00BE04C0 -> the const tree find). The split was self vs callers at
  HEAD; this landing narrows it. The remaining stand-in binders, 0x00BAD2E0 (render-state wave) and 0x00BE0480 (D1 member-template
  family with IsBlockLoaded), are queued. The coordinator first held these two rows by mistake and re-landed all 21 after checking
  the self rows; both runs gave the same result.
- tablecheck: seven addresses shrink (0x00A0AA70 4 -> 2, 0x00BE08E0 3 -> 1, among others). Owner recheck 8 of 8 good.
- Link (`logs/link_landing.log`): 0 LNK2005, the same as the HEAD control (`logs/link_control_head.log`).
- Coordinator calls: q5054 (0x00C4D440 / 0x00C4D4B0, libpk CreatePooledEvent / UpdateSlot) goes back to the queue: one name per
  address, so the callers move onto one spelling through shared headers; a recorded descriptive split is not acceptable. 0x00BE0930's
  family stays one hand-back (MATCH draft in `naming/.../work/`). 0x00BE0400 keeps its existing landed test (the long-symbol
  /alternatename did not link).
