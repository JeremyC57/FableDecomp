# acctB hand-back, author workflows bwf1 / bwf2 (third harvest): 3 rows matched, 21,788 bytes

Same lane and layout as `2026-11-12_acctB_bwf2a` / `_bwf_b` (all agents Opus; overlays based on `FullDecomp` at `bb0dc27e9`).
Nothing is landed.

| address | name (as matched) | bytes |
|---|---|---|
| 0x0046CE2D | `CPlayerGuiDef::operator=` (catalog label `CParentDefClassBase::operator=` is wrong) | 7312 |
| 0x005E5F30 | `CTCQuestCompletionUI::ActivateReal` | 6580 |
| 0x004830A1 | `CTCInGameMenu::OpenInGameMenu` (catalog label `OpenInGameMenuPC`; the PC twin is 0x004804DE) | 7896 |

Coordinator (acctB) re-verification: read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`, each row's overlay: all three
RELOCATION_MATCH + PASS (`coordverify/`); purity scan clean; no RELOC_CONFLICT; no oracle bytes here. The workflow's verifier
lenses and repair pass have not run on these rows yet; a row a repair changes is re-staged. Names, name fixes and made-up names:
`notes/<addr>/NOTES.md`; overlays: `hdr/<addr>/`.
