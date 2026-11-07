# Landing 2026-11-07e: chunk 829 (1 of 6 rows landed, 5 parked)

One expert agent authored five new ledger rows and one retry, sharing the lane with the chunk 828 and 830 agents.

| Row | Grade | Note |
|---|---|---|
| 0x0068BC70 CInputProcessInventoryQuestsScreen::ProcessInput | **RELOCATION_MATCH + PASS, landed** | `// Flags: /GS`; one if / else-if chain with no early returns |
| 0x00B5AF80 CEngineScreenEffectColourFilter::RenderColorFilteredQuad (retry) | DIFFER(2865v2865), md 14 | one test/or operand flip in the inline render-state setter (same as parked ApplyColourFilter); the parked text's `scaledContrast * 1.0f` is an unjustified x87 reorder, review before reuse |
| 0x0068CB90 CInputProcessControlFreeCamera::ProcessInput | DIFFER(2369v2339), md 27 | InitialiseFromAngles copy and cross-product order |
| 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine | DIFFER(2611v2613), md 125 | one colour temp shares a slot retail keeps separate |
| 0x00435530 CDisplayEngine::DoRender | DIFFER(2571v2617), md 401 | frame 0xcc v 0x90: retail does not reuse slots across the late blocks |
| 0x00678760 CPersistTraits<std::list<CReactionMatch*> >::TransferIn | DIFFER(3554v3579), md 1215 | PStringData kept in esi across five compares |

- **Spelling held for consistency:** the agent proved 0x00687B30 = IsEventButtonHeld and 0x00687BD0 = IsEventButtonReleased (event
  types 0x14 / 0x15; both PDBs order Pressed, Held, Released), the reverse of the landed leaves. The landed 0x0068BC70 keeps the old
  spellings (one symbol, one address) and the swap is queued PRIORITY (`land0711e`, 36 landed files).
- **Names:** 20 queue lines (`land0711e`, incl. Render 0x00435F70, RenderHeroFuckShit 0x00435000, SetMoneyAsDisplaying 0x004376B0, five
  ClearEffects, SetAsPerformedEvent 0x00A0D390, the EOpinion TransferIn fold 0x00679910, eight CInputEvent accessors, and the
  coordinator's 0x0129BA3C DELTA_NOTIONAL_ZERO respell of g_NormaliseEpsilon). Labels for three unlanded rows:
  `ghidra_out/labels_landing_2026-11-07e.tsv`.
- **Headers:** opt-ins `FABLE_RETAIL_C3DVECTOR_INITIALISEFROMANGLES` and `FABLE_RETAIL_CRENDERMANAGER2D_DRAW3DSCALEDTEXT` are used only by
  parked rows; they stay in `pending/2026-11-07_linux_hand829/hdr/`.
