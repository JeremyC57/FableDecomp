# 2026-11-12 landing dh -- naming agent NF25b: queue df (5/5) and NF24's open binder families, 45 landed rows changed, 5 renamed

Source: `naming/2026-11-12_agentNF25b/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, NAME_FIXES, QUEUE, hdr README).
NF25b redoes agent NF25, whose finished hand-back a container restart wiped before landing; the brief passed NF25's findings on as
hints, and NF25b re-proved each one.

**Part 1: queue df, 5 entries (5 done, 0 rejected)**
- 0x005DA4E0 is CTCWeapon::ResetAiming, on engine/CTCWeapon.h (was `sub_005da4e0`; Ego_r donor order + body AimingTime = -1,
  AimingTimeOverride = false). Its test is rewritten in plain C++. Binders 0x006EA450 and 0x008BF500 are respelled.
- 0x006E92C0 is CCombatManager::IsAttacker, const, taking a const reference. Binders 0x00663B10, 0x0096EDB0, 0x007FC820 and 0x008BF500
  are respelled.
- 0x00555A90 UpdateVisibleList is protected (IAE). Binders 0x00555CF0, 0x00555EB0, 0x00556200 and 0x00557000 are respelled.
- BlendInLevel: 0x008889C0 is moved onto engine/CEnvironmentThemePriorityStack.h. That puts its own symbol in NEnvironmentInternal
  with reference parameters, and fixes its GetLevelTheme binding.
- 0x008780D0: the claim holds. Its only caller already binds it; the row itself is unlanded (pending Q18 / Q19), so the catalog label
  goes back to the queue (dh).

**Part 2: NF24's open families**
- 0x005CC10E GetSubDef<CCarryableDef> const: all 19 binders are on the shared headers. The 5 wrong-T rows and the 2 CDefLookup
  rows are fixed, and CDebrisDef's field is now OnKillFX. 0x006A1230 is spelled CTCCarrying::IsCarrySlotFree.
- 0x00EF7AB0 ~CIsThingConscious: 7 binders respelled. 0x00EFB8C0 and 0x00EFE8D0 are on CEntityScriptBase.h, so their Main is now the
  PDB's private virtual (EAE). 0x00EF7800 is skipped: it needs a two-view model.
- 0x00658B20 is CTCVillage::IsWorkTime const (was IsCookTime). 7 binders are respelled; 0x00773950 and 0x00929F10 had bound
  IsCookTime's symbol to the wrong address.
- 0x0095A420's CQueenNative stand-ins are skipped as not cheap (NOTES).

**Headers** (opt-ins only; hdrcheck2 reports default=same for all five): CEnvironmentThemePriorityStackEntry.h `_STACK_FRIEND`,
CTCWeapon.h `_RESETAIMING`, CCombatManager.h `_ISATTACKER`, CCarryableDef.h `_ONKILLFX`, CTCVillage.h `_ISWORKTIME`.

**Verification:** the 45 changed rows were rechecked: 1 MATCH and 44 RELOCATION_MATCH, every test PASS (`logs/recheck.log`).
naming/rebind.py re-derived their reloc_bindings rows (removed 92, added 78). bindings_multi_check stays at its baseline (NullCoord
only), and funclet_includes_check finds 1317 checked, 0 broken.

**Queued (land1011dh, 6):** 0x008780D0 (catalog label), 0x005DA4C0 SetAimingTimeOverride, 0x005DA490 IsAimedForLongEnough,
0x00658A35 IsSchoolTime, 0x00658A80 IsStoryTime (both landed as IsCookTime), and 0x00662410 IsInMeleeAsAttacker (62 call sites
under made-up names).
