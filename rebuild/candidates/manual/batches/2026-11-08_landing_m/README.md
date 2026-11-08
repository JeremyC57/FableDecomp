# Landing 2026-11-08m: chunk 863's match, the CPlayerGui ctor

0x0043B570 CPlayerGui::CPlayerGui: RELOCATION_MATCH + PASS from a read-only vland grade with a trimmed overlay of six headers, then `--land`.
The last residue went with the one-argument `resize(n)` on the ability-button vectors. Hand-back: `pending/2026-11-08_linux_hand863/`.
The other seven rows are parked with behaviour tests that pass. The inline-budget devices were tried on the inline-split ones and do not
reach them: /d2inlT has a cliff on 0x00ECBDF0, and inline_depth(0) also outlines the forceinline pause dtor.

- **Headers landed** (additive `#ifdef` opt-ins, no removed lines; `hdrcheck2.py`: default=same for all six; no landed source defines
  any of the new macros): CPlayerGui CTOR (the first five members with their Ego_r names), CPlayerGuiDef ABILITYBUTTONS (a typed
  vector<CAbilityButtonGuiDef>, 0x5C elements), CSubtitleRenderer CTOR, NHeroInformationScreens_CManager CTOR (pad to 0x54),
  NPlayerGui_CDrawBase CTOR, NPlayerGui_CDrawNPCName CTOR (inline). Not landed (parked rows only): the CQ_BanditCampBossBattleScript
  CAssassinMarker opt-in and the new CQ_OrchardFarmRaidScript.h.
- **Names:** `ghidra_out/labels_landing_2026-11-08m.tsv` (the row plus three parked rows that were gap-scan or wrongly scoped). Queue
  `land0811m`: GetDef<CPlayerGuiDef> 0x0043FF30, the landed PeekGuiDef 0x00438570's invented callee names (PRIORITY), and the folded
  CCountedPointer<T>::Reset bodies 0x00444050 / 0x00444130 (vland RELOC_NAME, CHECK). 0x0099ED10 GetAsLowerCase was already queued
  (land0811f). The catalog sync was restricted to this landing's addresses. `bindings_multi_check.py` prints 1: the NullCoord entry that
  was already there.
