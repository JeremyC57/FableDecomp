# 2026-11-12 landing ea -- naming agent FAM13 (the last naming run before the pause): 112 landed rows changed, 101 renamed

Source: `naming/2026-11-12_agentFAM13/` (STATUS, APPLIED, REJECTED, RENAMES, LEFT, NOTES, LESSONS, ROWS, QUEUE, hdr README).

**Queue dt / du (5 entries; 4 done, 1 claim reversed):**
- 0x0066AD90 is CTCSoundPlayer::ReleaseAttachedLoopingSound (2 callers respelled).
- 0x01436E3C is NEngineInternal::GPScreenEffectOutlineGlow. In 0x00B33010 the stand-in BeginReflection / EndReflection calls are
  really BeginOutlineRendering / FinishOutlineRendering, and the row is now on the shared header.
- 0x007120D0 is the implicit `~CCreatureAction_PickUpGenericBox`. All three retail callers destroy an object built by the ctor at
  0x00843700. Its row source is the 0x009658B0 caller file with the Leaf pinned to that symbol, because an out-of-line definition
  would change the bytes.
- 0x013BDA50 is `streamingFontBankList` (anonymous namespace; Anniversary PDB public plus the type evidence).
- REVERSED: 0x004460C0 is correctly landed as IsQuickAccessEventInQueue (it scans QuickAccessItemButtons at +0x7c4).
  IsAssignableSpecialMoveButtonInQueue is 0x00445E90. The 4 callers that used the wrong name are respelled: 0x00847520, 0x005EE99D,
  0x00687CB0 and 0x0068E450. 0x00687CB0 is itself renamed CInputProcessGameBase::IsQuickAccessEventInQueue.

**FAM12's follow-up lists:** 23 CALLEE_FIXES rows renamed to their STLport names (14 `operator--`, 6 `insert_unique`, 3 `_M_insert`);
3 more callers of 0x00429160 respelled to ReleaseData. Most BINDER_FIXES callers are left with reasons (stand-in classes,
`map::operator[]` helpers that need header opt-ins, unprovable `~map` vs `~_Rb_tree`); see NOTES.

**Family (275 engine `vfunc_N` rows): 76 renamed, 199 left** (LEFT.tsv). Renamed: 26 in the NUISystem::CComponent family, 23
NHeroInformationScreens::CBase overrides across 10 inventory screens, 8 NInventory::CTCInventoryBase, 13 driver `IsDriverType` and
5 `CreateAdditionalTCs`. Every Leaf is in the Ego_r / FableWin name sets.

**Headers** (18, additions only; hdrcheck2 default=same and no existing opt-in changes): CEngineScreenEffectOutlineGlow.h,
CGamePlayerInterface.h, CInputProcessGameBase.h, CTCSoundPlayer.h, CMenuEntry, five driver `*_DRIVERSLOTS` blocks, eight inventory
`*_HISCREEN` blocks.
**Verification:** all 112 changed rows recheck 88 MATCH and 24 RELOCATION_MATCH, test PASS (`logs/recheck.log`). naming/rebind.py:
removed 126, added 117. bindings_multi_check stays at its baseline, and funclet_includes_check finds 1317 checked, 0 broken.
reserved_check: 0 reserved rows.
**New debt (queued land1011ea):** 0x00570332 binds `~CCountedPointer<CComponent>` to 0x004291DE, which is really ReleaseData.
Naming is paused after this landing (owner, 2026-11-12); only the name-fix queue continues.
