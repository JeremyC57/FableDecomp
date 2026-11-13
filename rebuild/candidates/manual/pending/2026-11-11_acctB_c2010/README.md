# 2026-11-11_acctB_c2010 -- acctB hand-back, chunk 2010 (4 reserved rows, all matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x005248B0 NThingManager::CEntityRuntimePersistence::LoadFromGameStateScript | RELOCATION_MATCH 1543v1543, test PASS | 8 grades. New opt-in `CPERSISTCONTEXT_TRANSFER_BODY` makes the `CPersistContext::Transfer<T>` body visible; same-TU AddPersistedDeadEntity / SetGamePersistentScript bodies defined above the row |
| 0x004959E0 CFrontendGameComponent::GetProfileNames | RELOCATION_MATCH 1522v1522, test PASS | `// Flags: /GS`; 2 grades. No shared CFrontendGameComponent header exists: the row declares the class and its static member (header debt) |
| 0x00716DE0 NTCScriptedControl::CActionTalkBase::Update | RELOCATION_MATCH 1514v1514, test PASS | 10 grades. Grade `retail_asm` through the shared GFFloatToLongCeil helper (`// Retail-asm:` line); uses the row-local CActionTalkBase / CSubtitledMessage models of the landed sibling 0x00716B30 (header debt) |
| 0x00492381 CSubtitleRenderer::ProcessEmbeddedGraphicTags | RELOCATION_MATCH 1522v1522, test PASS | size TU (`#pragma optimize("s", on)`); 7 grades. Grade `retail_asm` through the shared GFFloatToLongFloor helper (`// Retail-asm:` line) |

Tests pass under g++ and VC7.1. Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/`
(grades, both test runs, read-only vland: 4 wins, no RELOC_IDENTITY; label-only RELOC_NAME / import-thunk RELOC_ALIAS
warnings, wrong labels in NAME_FIXES.tsv). Purity and padded-struct greps are clean on rows and tests.

REVIEW: the two `retail_asm` rows rely on the existing helpers' proof and carry no new per-site table of C++ attempts
(ASM_RULES rule 4).

To land: the rows need `hdr/` (14 overlay headers, additions-only opt-in blocks; `hdr/README.md`). The overlays also carry
blocks copied from c2001 (CStringParser INLINE_CTOR / READLINE) and c2003 (CLEARINSCRIPT): merge once. Rebuild the oracle
from Fable.exe (no retail bytes are in this folder). NAME_FIXES.tsv: 12 landed spellings proven wrong. One descriptive
global: `g_bLeftAlignText` 0x013B861B (NOTES.md).
