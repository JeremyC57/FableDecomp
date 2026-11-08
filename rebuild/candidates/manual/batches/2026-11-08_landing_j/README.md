# Landing 2026-11-08j: chunk 858's seven matches

All RELOCATION_MATCH + PASS (read-only vland grade with a trimmed overlay, then `--land`). Hand-back: `pending/2026-11-08_linux_hand858/`.

| Row | Note |
|---|---|
| 0x00494900 CFrontendGameComponent::Output | the last residue (CursorHighlight loaded before the `fadd` at 10 sites) went with `line_height = MainFont->GetVFontMaxHeight(); y += line_height;`; shared engine/CSystemManager.h |
| 0x00CB6E20 std::vector<std::pair<CCharString,CScriptInfo> >::~vector | was unnamed gap-scan `sub_00cb6e20` |
| 0x00D3B3D0 NScript::CQ_GuildTrainingScript::Init | script vtable 0x012CD458 slot 3 |
| 0x00D7AE30 / 0x00D8DCC0 / 0x00EB6980 OnPersist (CQ_HeroSoulsThunderScript, CQ_HobbeCaveScript, CMurderTwistAssassin) | script / entity vtable slot 4 |
| 0x0121AD80 `$E_InventoryOrientationRHSet9` | dynamic initializer of BACKWARDS_FORTY_FIVE_DEGREES_ORIENTATION, shared engine/CRightHandedSet.h (`$E` index descriptive) |

- **Headers landed** (additive opt-ins; `coordinator/hdrcheck2.py` `default=same` for all seven; no landed file defines the new macros):
  CDisplayManager FRONTEND, CQ_GuildTrainingScript INIT, CQ_HobbeCaveScript ONPERSIST, CRenderManager2D DRAW2DTEXTUREBOX, CSurface BITMAP,
  CSystemManager FONTMANAGER (header debt: see the pending README), CTexture NEXTVALIDDIMENSIONS. Not landed (parked rows only):
  CIntelligentPointer OUTOFLINE, CTCCreatureOpinionOfHero THREAT / COpinionTransientOffset TRAITS.
- **Names:** `ghidra_out/labels_landing_2026-11-08j.tsv`; queue `land0811k` (IsThingAsPosBeingDrawn 0x00829D20, const IsOpen 0x0048DA30,
  GetFadeInOutAlpha 0x004348D0, NPlayerGui::GFAnchorToTopRight 0x00643CA1). vland's RELOC_NAME on 0x00D3B410 is the stale catalog name.
- Catalog sync restricted to this landing's addresses. `bindings_multi_check.py` prints 1, the pre-existing NullCoord.
