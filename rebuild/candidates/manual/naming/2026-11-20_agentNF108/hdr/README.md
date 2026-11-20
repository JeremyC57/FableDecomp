# NF108 header overlay (FABLE_HDR_OVERLAY=$S/w22/agentNF108/hdr)

Each file is the CURRENT repo header (`rebuild/include/engine/<name>`, base copy in `../orig/engine/`, identical to the repo copy when
taken) plus `#ifdef` opt-ins only. Every opt-in macro is new and member-specific; no landed row defines any of them, so the default
path of every header is token-identical to the repo's apart from the pinned static asserts (pin_asserts.py), and the opt-ins change
none of the landed includers listed below. No opt-in renames or re-declares an existing member; the sibling agents' overlays
(NF105 / NF107 / NF9x) carry none of these headers' members.

| header | opt-in (declaration only unless noted) | row | landed includers (grep -rlE '<header>' rebuild/src/compiled) | pin_asserts |
|---|---|---|---|---|
| engine/CQuestManager.h | FABLE_RETAIL_CQUESTMANAGER_PREPAREQUESTSWHENFINALQUESTISCOMPLETED: ?PrepareQuestsWhenFinalQuestIsCompleted@CQuestManager@@QAEXXZ 0x004AF270 | 004af270 | 131 | pin_asserts run: pinned 0 |
| engine/CWorld.h | FABLE_RETAIL_CWORLD_SETMAPTABLETOSHOWQUESTCARDSWHENUSED: the inline FableWin.pdb setter SetMapTableToShowQuestCardsWhenUsed(bool) (body; added in both layout branches after the MAP_TABLE_QUERY blocks; no layout change) | 004af270 | 740 | pin_asserts run: pinned 0 |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | FABLE_RETAIL_CTCGRAPHICAPPEARANCE_GETFOCALPOSIFLOADED (nested in FOCAL): ?GetFocalPosIfLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_NAAVC3DVector@@M@Z 0x004C11D0 | 004c11d0 | 126 | pin_asserts run: pinned 0 |
| engine/CTCScriptedControl.h | FABLE_RETAIL_CTCSCRIPTEDCONTROL_ISACTIONLISTEMPTY: ?IsActionListEmpty@CTCScriptedControl@@QBE_NXZ 0x007122C0 | 004c70f0 | 16 | pinned 1 |
| engine/CBulletTimeManager.h | FABLE_RETAIL_CBULLETTIMEMANAGER_RECALCULATESOUNDPITCH: private ?RecalculateSoundPitch@CBulletTimeManager@@AAEXXZ 0x004C5F10; FABLE_RETAIL_CBULLETTIMEMANAGER_SETSLOWDOWN: ?SetBulletTimeSlowdownMultiplier@CBulletTimeManager@@QAEXJPAXM@Z 0x004C6170 (both inside the FABLE_RETAIL_CBULLETTIMEMANAGER member block, after REMOVESLOWDOWN) | 004c6140 (RECALC), 00890ab0 00890b00 (SETSLOWDOWN) | 20 | pinned 7 (the seven FABLE_STATIC_ASSERT lines became FABLE_STATIC_ASSERT_AT(69..75), their base line numbers) |

Rows that use the overlay must be graded with FABLE_HDR_OVERLAY set; the coordinator merges the opt-ins into the repo headers.
