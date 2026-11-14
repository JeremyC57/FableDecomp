# acctC workflow hand-back wfD6 (unworked rows round 2, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00B1C880 | NParticleEngine::CPSCDecalRenderer::ReadBinary | 3,266 | RELOCATION_MATCH | PASS |
| 0x00B087C0 | NParticleEngine::CPSCSingleSprite::PreparePrimitives | 2,967 | RELOCATION_MATCH (`/d2inlT86`) | PASS |
| 0x00B1F200 | NParticleEngine::CPSCDecalRenderer::PreparePrimitives | 3,195 | RELOCATION_MATCH (`/d2inlT1156`) | PASS |
| 0x00B1E380 | NParticleEngine::CPSCDecalRenderer::DatabaseSerialise | 3,698 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; its review
items (inline-budget devices, fitted spellings, label warnings) are for the owner / landing account.
`open/` holds the best drafts of the batch's unmatched rows (residues in NOTES.md).

## 0x00B1C880
Verifier audit: none. No __asm/naked/_emit/volatile, no flags or pragmas, no dead locals or if(0). Uses the new shared engine/CPSCDecalRenderer.h (overlay NEW_ header, additive). Strict-header check clean. vland warnings: none. Test: a standalone model of the chunked stream and the packed layout; 4 passes (stale 0x00/0xFF bytes, chunk size 3 or 64, oversized values) check every field and the stream position, so it is a real test. Minor doc inconsistency: NOTES.md says 'no engine/CPSCDecalRenderer.h exists', but the source and overlay now ship it as a new shared header.

Fitted items: none marked (NOTES: the earlier hand724 draft's source unchanged, only the header comment rewritten)

## 0x00B087C0
Verifier audit: No hard violations. REVIEW ITEM: inline control `// Flags: /d2inlT86` with an `// Inline-budget:` line (CParticleUpdateParameters::GetUpdateParam + Find expanded at 0x00B08B50..0x00B08B9B; T85 calls it, T86 is the smallest that expands it). This is allowed under the owner decision of 2026-11-08. vland WARNs (allowed): RELOC_NAME ?DELTA_NOTIONAL_ZERO@@3MB -> 0x0129BA3C labelled '?g_NormaliseEpsilon@@3MB' (the author's NAME_FIXES.tsv argues DELTA_NOTIONAL_ZERO is right, citing reloc_bindings, Ego_r and 213 landed rows, and points to labels_landing_2026-11-11cl.tsv as the wrong entry); RELOC_ALIAS __CIfmod -> import thunk 0x00BFEB28. The source declares local class models CEnginePrimitiveSpriteTrail, CPSCSplineBase and CPSCSingleSprite. No engine headers exist for them and the strict-header check passed. Landed siblings (CPSCSingleSprite AddTrailPosition 0x00B081A0, SetTrailPositionsFromSpline 0x00B08250, the CPSCSpline rows) use the same local models, but the source header comment does not say so explicitly as WF_RULES asks (minor). The header comment marks `float fadeAlpha = 0.0f;` as FITTED; its initial value is a dead store. This is last-resort device (a); review it. Test: the body verbatim over a recording model; 6 scenarios with 21 CHECKs; NOTES says mutants fail. A real test.

Fitted items: FITTED: `CIEngine* engine = info.Engine;` in the single-sprite set arm; `float fadeAlpha = 0.0f;` defined with the locals at the top and assigned system.GetFadeAlpha() at its use

## 0x00B1F200
Verifier audit: No hard violations. REVIEW ITEM: inline control `// Flags: /d2inlT1156` with an `// Inline-budget:` line (inline CDecal::SetPrimitiveInfo expanded at 0x00B1FD55..0x00B1FE40; T1155 calls it, T1156 is the smallest; the twin 0x00AFCC30 uses /d2inlT1537 /d2inlS15). This is allowed under the owner decision of 2026-11-08. vland WARN (allowed): RELOC_ALIAS __CIfmod -> import thunk 0x00BFEB28. Header additions are additive opt-ins: FABLE_RETAIL_CDECAL_PREPARE (PDB-cited CDecal inline accessors + SetPrimitiveInfo; it also adds std::vector one-argument resize specialisations under #ifndef FABLE_RETAIL_CPARTICLE_SPRITE, which mirror CParticle.h), FABLE_RETAIL_CPARTICLESYSTEM_GETDECALLIST, and the new CEnginePrimitiveParticleDecal(Group).h and CPSCDecalRenderer.h headers. Moved asserts are pinned as FABLE_STATIC_ASSERT_AT. No __asm/volatile/if(0). Its Retail-asm line points only to existing shared GF* portable-twin helpers. Test: the body verbatim over a recording model with 31 CHECKs (empty-bank clear, both groups, MinLife clamp write-back, set/add, dead-decal clear, fades, flicker, pooling, frame clamp). A real test.

Fitted items: FITTED (NOTES.md): the named locals size_factor / alpha / pool_size; `CRGBColour colour;` declared at function scope before the loop; the if / else-if / else clamp

## 0x00B1E380
Verifier audit: none. No flags or inline controls, no __asm/volatile/if(0). The closer was two existing repo opt-ins, FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT and FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT. The author says the landed sibling CPSCEmitterGeneric::DatabaseSerialise 0x00B13530 defines the same pair. That is evidence by sibling, found by sched2 DAG probing; review it as a fitted choice of opt-in. New additive opt-in FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTBLEND (Ego_r GetDefaultBlendMode/GetDefaultBlendOp returning 2/0, as in the landed CPSCRenderSprite/CPSCRenderMesh DatabaseSerialise). vland warnings: none. Test: save / load / legacy-flag paths over the real packed layout with 21 CHECKs; NOTES says mutants fail. A real test.

Fitted items: none explicitly marked FITTED in the source or NOTES. The choice of the GROUP_PRODUCT opt-in pair was found by scheduling probes and justified by the landed sibling 0x00B13530
