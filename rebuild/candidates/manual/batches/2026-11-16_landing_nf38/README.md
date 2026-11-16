# 2026-11-16 landing nf38: name-fix round 3, NF38 (379 landed rows, 19 renamed; 228 funclets rechecked)

Owner: "Continue the naming fix queue with two agents until complete". Round 3 continues NF36's half: tags land1115ga ..
land1116p23, 299 entries. Brief and records: `naming/2026-11-16_agentNF38/`.

## Queue (299 entries)

| Status | Entries |
|---|---|
| done | 30 |
| done-already | 7 |
| rejected | 32 |
| partial | 17 |
| blocked | 13 |
| carry | 200 |

- **rejected:** among them, entries 50-54. The Codex two-PDB evidence says those statics are private
  DefaultBackBufferWidth/Height, against the queue's public ScreenWidth claims.
- **blocked:**
  - authoring: 8;
  - header-wide stand-ins that need a coordinator decision: entries 157-159 and 163;
  - one port attempt that failed: 0x00A763F0.
- **carry:** 23 examined with a specific finding; 177 probed only, each with fresh ok/bad binder counts. Their fix is a port of
  every binder onto its shared header.

## Rows (379; 228 funclets rechecked)
- **RENAMES section 1:** 19 self renames, including:
  - 0x00B6D510 CEngineWaterRenderer::InitialiseDynamicTextures;
  - 0x0079F730 CTCWillLightning::CLightningBeam::AllChildrenInactive;
  - 0x00556570 NUISystem::CRedefinerList::SetAcceptButton;
  - 0x0077C360 CTCGraphicAppearanceSprite::GetInterpolatedAnimStage;
  - 0x00E4E0E0 CV_BanditTollScript::CTollBandit::SetUpBanditToll;
  - 0x0075C9B0 CKeyCamera::GetEvent;
  - descriptive libpk names, each matching a header or a landed caller: CMailboxQueue::ProcessMessage, PKCMailbox::GetResult,
    PKCDefaultFileManager::ReleaseFileObject.
- **RENAMES section 2:** 426 caller respellings, completed by an old-versus-new relocation diff of every changed row.
- **Headers:** no header changes; the rows use existing opt-ins only.
- **Recheck (`logs/recheck_part0..2.log`):** all 607 graded from the repo after landing, in three parallel runs: the 379 changed
  rows plus the 228 landed EH funclet rows that include a changed parent by path (`naming/.._agentNF38/FUNCLETS_RECHECKED.tsv`).
  2 MATCH and 605 RELOCATION_MATCH, every test PASS.
- **Funclet includes:** four funclet rows (0x01215AE0, 0x01215E90, 0x01215E98, 0x01212280) included parents that the renames moved.
  The coordinator repointed them. funclet_includes_check reports 1,355 checked, 0 broken.
- **Bindings:** rebind run, bindings_multi_check at baseline.

## Coordinator decision: the nil character pointer 0x0139C900 (entry 204)
126 of the rows now bind `?_afxPchNil@@3PADA` at 0x0139C900, the spelling engine/CString.h declares (`CString::Init` reads it)
and 40 rows already used. Before, about ten junk spellings were bound there (g_pchNil, g_pDebugBuffer, g_logBuffer_0139c900,
...). The list is `naming/.._agentNF38/D4_AFXPCHNIL_SET.tsv`.

Wave 25's decision D4 (2026-11-01) held that family because no retail name exists for the address. The coordinator lands the
consolidation anyway, under the later owner rule (2026-11-10): where no original name exists, one consistent name per address
everywhere. MFC's own name for the CString nil pointer, already in the shared header, is the most accurate one available.
One other spelling is still bound: `?g_logBuffer_0139c900@@3PAXA` in 0x00C13620. The owner left the call to the coordinator ("You can decide as the expert"). It is settled in
`naming/2026-11-16_decisions/D4_0139C900.md`: `_afxPchNil` everywhere, and the last six rows were respelled after this landing.

## Open (NOTES.md)
- 0x00B6D510 keeps its landed `#define CPixelFormat CPixelFormatLayout` wrapper and the wrong 0x01436E98 binder.
- 0x00B242A0 (GPCamera, entries 73 and 168) was held by NF37.
- The recheck's SHARED_HEADER_AVAILABLE warnings are on classes the landed rows already modelled; no new local model was added.
