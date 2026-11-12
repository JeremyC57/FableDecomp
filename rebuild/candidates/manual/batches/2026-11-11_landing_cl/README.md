# 2026-11-11 landing cl -- chunk Q7 rework: 4 rows on shared headers

All four were reworked onto shared headers after landing cj sent them back. vland --land: 4 wins (RELOCATION_MATCH), every test PASS.
- 0x0047E651 CTCInGameMenu::VideoOptionsChanged
- 0x0047E9AE CTCInGameMenu::GlobalUpdate (/GS)
- 0x0047F30F CTCInGameMenu::VideoOptionsNotAccepted
- 0x00697490 CGameCameraManager::IsPosCameraInsideThing (const; takes the thing by reference, per Ego_r.pdb)

0x0086BFB0 was left out of this payload because it already landed in cj.

**Headers**

hdrcheck2 reports default=same for all four. Each is a line-superset of the repo copy.
- CTCInGameMenu.h: VIDEODIALOGS
- CUserProfileManager.h: DISPLAYDIALOG
- CTCInventory.h: REVERTVIDEOCHANGES
- CGameCameraManager.h: INSIDETHING

**Warning**

vland's RELOC_NAME warning names `CCountedPointer<NUISystem::CAction>::ReleaseData`, which resolves to 0x00429160, labelled `~CCountedPointer`. The two names share one address: the destructor's body is exactly ReleaseData inlined, and the linker folds identical bodies. This is not a sibling swap.

**Names and labels**
- Labels: ghidra_out/labels_landing_2026-11-11cl.tsv. It also carries cj's 0x0086BFB0, which got no labels file.
- Callee names: RevertVideoChanges, SaveGame, GFIntToWideString, CUIScreenUnitConverter::GetBackBuffer{Width,Height}.
- Q7 returned no NAME_FIXES.

The full chunk record is in pending/2026-11-11_linux_handQ7: 35 near-miss drafts in differ/, plus LESSONS.
