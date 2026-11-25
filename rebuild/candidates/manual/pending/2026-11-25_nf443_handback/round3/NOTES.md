# NF443 round 3 (2026-11-25)

## h1_hdr: r2 header batch plus 0x00691680 (1 row)
- **Headers** (hdr/engine/, HEADERS.tsv, orig/hdr_CTCOracleMinigame.h):
  - New engine/EOracleMinigameInput.h, in the EGameAction.h form. It holds the enum line, moved unchanged.
  - CTCOracleMinigame.h line 25 (inside the FABLE_RETAIL_CTCORACLEMINIGAME_CLASS branch) now has the include in place of the enum. One line changed.
- **Row:** includes the new header and keeps its own model. PushSymbolToQueue(long) becomes
  `?ProcessInput@CTCOracleMinigame@@QAE_NW4EOracleMinigameInput@@@Z` (0x007E5F70); the calls cast the literal to the enum.
- **Gates:** vland HEAD (repo headers) v new (FABLE_HDR_OVERLAY=hdr/) over the row and all 19 includers of CTCOracleMinigame.h
  (evidence copied from gating batch h1x):
  - grades equal, PASS, LITERAL 0, no new warn kind;
  - objgate ALL_EQ 20/20 and pk OK 20/20;
  - link: duplicates 2->2, the new name defined;
  - name_gate NO_WORSE (run with the overlay);
  - xrcheck OK.
- **Table:** add ProcessInput and delete PushSymbolToQueue at 0x007E5F70; apply --dry 0 misses. Reserved 0.

## u1: the GetDamageMultiplier unit (6 rows: 3 self renames + 3 callers)
Files: RENAMES.tsv (old_catalog_name as in auto-re-candidates.tsv), CATALOG_CHANGES.tsv (name + Module for 0x007CBF50 and
0x00789CC0), SELF_SYMBOLS.tsv, TABLE_CHANGES.tsv (3 add, 3 delete), renames.tsv, and renames_obj.tsv (objgate only).

**Self renames:**
| address | from | to | body reads checked |
|---|---|---|---|
| 0x0078E7B0 | `?GetDamageMultiplier@@YIMPAUS@@@Z` | `?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ` | PDef +0xC, SpellLevel +0x50, def +0xE8 |
| 0x007CBF50 | `?GetDamageMultiplier@@YIMPAUS@@@Z` | `?GetPerArrowDamageMultiplier@CTCMultiArrow@@QBEMXZ` | PDef +0xC, SpellLevel +0x1C; interface 0x88 = TCI_MULTI_ARROW |
| 0x00789CC0 | `?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ` | `?GetProjectileWeaponFirstPersonTargetingModeZoomFOV@CTCHeroRangedAttacks@@QBEMXZ` | def +0x10, index +0x44 (header-asserted), x 1/360 |

- **0x00789CC0 resolves T3_PORT_QUEUE line nf384,** which reached the same identity independently. Its test model was renamed with the row.

**Callers:**
- 0x00698D10: layout-rename include of CTCHeroRangedAttacks.h (the 0x00789C90 pattern). The header's default branch does not declare the getter.
- 0x007BDCFC: local model renamed. The inline GetInterfaceUnchecked<T> COMDAT is renamed with its class (renames_obj.tsv).
- 0x00868B40: bodiless CTCDivineWrath (r3), interface 0x85 = TCI_DIVINE_WRATH.

**Gates:**
- **Clean:**
  - vland: same grade on both sides, PASS, tests PASS, LITERAL 0.
  - objgate ALL_EQ 6/6. The DEF is mapped for the selves; the callers' UND changes old -> new.
  - link: duplicates 1->0, 0 new unresolved, the 3 new names defined by the batch's own self objects.
  - Table: apply --dry 0 misses, multi-target 1 -> 1. Reserved 0.
- **Residual flags, all pre-landing state this unit corrects. I set ACCEPT by hand; each RESULTS reason says why:**
  - **link "FAIL":** `?GetDamageMultiplier@CTCDivineWrath@@QBEMXZ` is the OLD name for 0x00789CC0 / 0x007BDCFC and the NEW name for 0x00868B40
    -> 0x0078E7B0, with exactly one definer.
  - **pk NEW_NEQ (0x0078E7B0):** pvr compares the leaf with 0x007CBF50, where the table binds the name until this batch's delete. HEAD
    0x00789CC0 shows the same NEQ.
  - **New warn kinds:**
    - STANDIN_LEAF (0x00789CC0) and RELOC_NAME (0x007BDCFC) are the old catalog labels; CATALOG_CHANGES fixes them.
    - RELOC_CONFLICT (0x00868B40) is the 0x007CBF50 binding deleted here.
    - SHARED_HEADER_AVAILABLE x2 (0x0078E7B0) are local models of CTCDivineWrath and its def, the same form HEAD 0x00789CC0 used.
      engine/CTCDivineWrath.h kept VC7.1 busy for over 4 minutes in a trial compile, so I killed it (my own PID).
  - **name_gate:** WORSE 6 -> 8 on 0x007BDCFC only. Its two added FAILs read the old table and catalog. The three selves go 3/3/4 -> 0.
- xrcheck: the 3 callers are OK. The 3 selves are marked SELF: a self rename is a DEF, not a relocation.
- Also seen, not touched: `?C@Component@Row00868b40@@QAEMXZ` binds 0x00793CB0, whose catalog label is CTCDivineWrath::GetDamageMultiplier too.
  It is probably CTCUnholyPower::GetDamageMultiplier (interface 0x86); it needs the same check.

## e1: 00e96660, 00ead800 (NScript, outside the audio closure)
- Re-gated on the current HEAD (orig 0 drift): vland PASS both sides; objgate, pk and link PASS; name_gate NO_WORSE; xrcheck OK.
- Table: 1 add, 2 delete; apply --dry 0 misses; reserved 0.

## 00b32820: identity check
- **0x006C35E0 is CSkeletalMorphResourceManager::UnloadBoneConfigs**, so the binding there via 0x004A2E90 is RIGHT:
  - Ego_r donor order UnloadBoneConfigs -> GetMorphTarget -> PreloadBoneConfigs = retail 0x006C35E0 -> 0x006C3620 -> 0x006C37D0;
  - the catalog TU agrees.
- **The target 0x00B93DF0 is mislabelled.** Its self row is named UnloadBoneConfigs, but it sits in the CEngineScene TU:
  - neighbours: CheckVisibility 0x00B93CC0, ~CEngineSceneGrid, RenderScene;
  - the body clears a container at this+0x2C;
  - the caller 0x00B32820 calls it "the scene's per-frame list reset".
- **Its real name is not proven.** The only candidate is by donor order: Ego_r InvalidateBoundingBoxesAndDrawDistances or ResetStats sit at
  that position. That is one source, so there is no NAME_FIXES line.
- **00b32820 stays held:** its respell to UnloadBoneConfigs would have been wrong. Respell 0x00B93DF0's self first, after an identity proof.

## Skipped (as ruled): the virtual (12) and SIG (38) groups stay in round2/RESIDUE.tsv.

## Locks
NF443 holds 9 rows: 00691680 (h1_hdr), the 6 u1 rows, and 00e96660 / 00ead800 (e1). None of the rows is in NF445's lock set or the
audio closure.
