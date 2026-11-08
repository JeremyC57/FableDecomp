# Chunk 858 (2026-11-08): six small gap-scan rows + four 3.3-3.5 KB engine / UI rows — 7 of 10 matched (landed `batches/2026-11-08_landing_j/`)

Matched (src/, test/; all RELOCATION_MATCH + PASS): 0x00CB6E20 vector<pair<CCharString,CScriptInfo> >::~vector, 0x00D3B3D0
CQ_GuildTrainingScript::Init, 0x00D7AE30 / 0x00D8DCC0 / 0x00EB6980 the three script OnPersist twins, 0x0121AD80 `$E` initializer of
BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION, 0x00494900 CFrontendGameComponent::Output (font height as a named float temporary).

| Parked row (differ/) | Best | Lines | Grades | Residue |
|---|---|---|---|---|
| 0x009D9C80 CRenderManager2D::DrawBufferedPolys | 3451v3435 | 17 | 710 | zero register of `memset(&current_window,0,16)` + two loop pads from it |
| 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed | 3385v3442 | 464 | 51 | frame packing, flirt-section registers, two CDefPointer null tests merged |
| 0x00648820 NPlayerGui::CDrawMiniMap::Draw | 3419v3464 | 648 | 50 | frame 0x324 v 0x334, the two side borders (probe/mm26.cpp has their x87 sequence); local models |

Header debt: engine/CSystemManager.h's retail branch puts the font manager at +0x7C where retail reads +0x84 (Output, landed ~CText 0x0054F6E0);
the landed nested opt-in FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER swaps it with the unnamed 8-byte block -- fold it into the main retail branch once
its includers are rechecked. Names: LABELS.tsv; NAME_FIXES.tsv (queued land0811k); NOTES.md also has 0x0082C060 = CMiniMapDisplay::DrawPreparePrimitivesForEngine
(manifest `TailJmp_DrawMap_0082c060`) and the stale catalog name of 0x00D3B410 (chunk 853 respelled it GossipSetup).
