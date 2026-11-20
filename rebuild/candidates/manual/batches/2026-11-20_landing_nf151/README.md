# Landing nf151 (Account A, 2026-11-20): CShaderRenderManager and CEngineLightingManager names (23 rows)

Agent NF151 (round 10) handed back 18 entries (10 done, 6 done-already, 1 deferred, 1 blocked); its files are in `naming/2026-11-20_agentNF151/`.

- 23 rows: in-place access / parameter respells in existing local models, and ports of 0x00988020, 0x00B47FD0, 0x00B61690 and 0x00B88BC0
  onto the shared headers. Self renames (RENAMES section 1, 8 rows): leaf-only access fixes; both PDBs agree.
- Headers: CEngineLightingManager.h and CRenderStateManager.h add-only; CEngineInternalPrimitiveLight.h gains a new TYPED branch (no
  existing users) and its asserts are re-pinned at their old lines. Overlays' `orig` equal HEAD.
- Recheck (`logs/recheck.log`): 4 MATCH + 19 RELOCATION_MATCH, every test PASS. rebind: removed 53, added 42; bindings_multi_check 1
  (baseline); split gate 0 created. tablecheck (`logs/tablecheck.txt`): many addresses shrink; 0x00988290 shows 1 -> 2 only because the
  table now lists 0x00B63010's ResetWorldTransform binding, which its HEAD object already had (SAME).
- Includers (`logs/includer_recheck.txt`): 156 good; 3 bad at their HEAD state: 0x009BEF80 (asm bake, no verdict line), 0x00BE6F70
  (198v2654) and 0x00BF4570 (316v4431), the July landscape rows.
- Link (`logs/link_landing.log`): 10 LNK2005 (CShaderRenderManager::PrepareForRender: 0x009E1280 strong against nine rows' pick-any
  copies; AttachTextureToStage: owner 0x009A0CF0). The HEAD objects of the same rows give the same 10 (`logs/link_control_head.log`), so
  none is new. PrepareForRender queued as DEBT; AttachTextureToStage was queued by nf150.
- q4541's last binder 0x00BB8090 stays on its stand-in (blocked after NF150's 3 forms); q4669 waits on the 0x009BEF80 asm bake.
