# acctB hand-back, author workflow bwf2 (first harvest): 9 rows matched, 42,558 bytes

Rows authored by the workflow lane (`lane_tools/workflows/author_rows.js`, adapted for acctB's reserved rows; all agents Opus).
Base for the header overlays: `FullDecomp` at `bb0dc27e9` (merged into `FullDecomp-B` as `fbde0c2c3`). Nothing is landed.

| address | result | name (as matched) | bytes |
|---|---|---|---|
| 0x0045A159 | RELOCATION_MATCH, test PASS | `CInventoryDef::operator=` (catalog label `CParentDefClassBase::operator=` is wrong) | 6481 |
| 0x005AAF28 | RELOCATION_MATCH, test PASS | `CTCInventoryTrade::ConstructTradeDescriptionClothing` | 4550 |
| 0x005AD571 | RELOCATION_MATCH, test PASS | `CTCInventoryTrade::ConstructTradeDescription` | 4414 |
| 0x007CACB0 | RELOCATION_MATCH, test PASS | `CTCHeroReceiveItems::GlobalUpdate` | 2974 |
| 0x00A04630 | RELOCATION_MATCH, test PASS | `CRenderStateManager::SetupRenderStates` | 4609 |
| 0x00A079B0 | MATCH, test PASS | `CRenderStateManager::CDeviceState::CDeviceState` (catalog label `CPSCOrbit::CPSCOrbit` is wrong) | 4950 |
| 0x00B72180 | RELOCATION_MATCH, test PASS | `CEngineWaterRenderer::CreateDynamicTextures` | 5587 |
| 0x00BB5040 | RELOCATION_MATCH, test PASS | `CEngineClothRenderer::CEngineClothRenderer` | 4824 |
| 0x00BBF4A0 | RELOCATION_MATCH, test PASS | `CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive` | 4169 |

## Verification state

- **Coordinator (acctB) re-verification, done:** each row's `payload.json` + oracle through the read-only `vland.py` with
  `LANDVERIFY_STRICT_HEADERS=1` and the row's own overlay: every row prints MATCH / RELOCATION_MATCH and PASS
  (`coordverify/<addr>.vland.log`). Purity scan (comments stripped) clean for source and test. No oracle bytes are in this folder.
- **Workflow verifier lenses (mechanical + evidence) and the repair pass: NOT RUN YET** for these rows. In the pipeline they
  run after the workflow's remaining authors; if they change a row, this folder is re-staged. Treat names, tests and
  "faithfulness" claims as the author's until then.
- vland warnings: `RELOC_NAME` lines on six rows (catalog labels that differ from the spellings bound; see each
  `notes/<addr>/NOTES.md`), and one `RELOC_CONFLICT` on 0x00BBF4A0: `?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA` binds
  0x01396DD8 here and 0x01396DE8 in landed 0x00BBC460 (one of the two table names is wrong; needs a look at landing).

## Layout

- `rows/<addr>.cpp`, `rows/<addr>.test.cpp`; `payload.json` (the nine rows combined, landing format).
- `hdr/<addr>/`: each row's header overlay (current repo header + `#ifdef FABLE_RETAIL_*` additions; `README.md` inside lists
  the opt-ins). Rows without a folder need no header change. Two rows may add an opt-in for the same inline: compare at landing.
- `notes/<addr>/`: the scout's `DOSSIER.md` / `OUTLINE.md`, the author's `NOTES.md` (what made it match, name fixes with
  evidence, made-up names) and `ATTEMPTS.log` (one line per grade).

## Update (23:20 UTC): three rows re-staged after the workflow's repair pass

0x005AAF28, 0x005AD571 and 0x00BBF4A0 were changed by the Opus repair agents (verifier findings: incomplete NAME_FIXES, an
enum compared as a number, a test comment, undeclared made-up names, local helper bodies). The repaired rows are staged here
(rows, notes, payload) and re-verified by the coordinator: RELOCATION_MATCH + PASS under strict headers, purity clean. The
`RELOC_CONFLICT` on 0x00BBF4A0 (`D3DAlphaFuncModeTable`) is unchanged and still needs a decision at landing.
