# 2026-11-16 landing nf34: name-fix pass NF34, 83 landed rows respelled (18 renamed)

Owner: "start 2 opus 5.5 agents on naming fix queue now". This covers the 1,167 queue entries added since NF32 (queue line 9237
on). The coordinator pre-screened them against reloc_bindings and each landed row's own Leaf, then split them:
- **NF33:** land1011fj .. land1113fz, 522 entries. Still running when NF34 landed.
- **NF34:** land1115ga .. land1116p23, 645 entries.

Brief: `naming/2026-11-16_agentNF34/BRIEF.md`. Records: `naming/2026-11-16_agentNF34/`, with STATUS, APPLIED, REJECTED, RENAMES,
NOTES, LESSONS, QUEUE and the hdr README.

## Queue (645 entries)

| Outcome | Entries |
|---|---|
| done | 33 (73 binders respelled) |
| done-already | 145 |
| partial | 16 |
| rejected | 1 |
| carry | 450 |

- **done-already:** 116 the screen called ALREADY_OK (5 spot-checked), 10 NO_BINDER, and 19 SELF_WRONG rows that already compile
  to the claimed symbol.
- **rejected:** line 91 `AddResource` at 0x009FC3D0. Queue line 461 supersedes it: the body links into LoadingQueue +0x40, which is
  AddToLoadingQueue, and AddResource is 0x009FC380.
- **carry:** never reached. These stay in the queue under their own tags for the next pass. Most are not proven by both PDBs, plus
  the TransferVector / Rb_tree / STLport families, the NEngineInternal `GP*` globals, the NOT_MANGLED decisions and the Coda/libpk
  entries.

## Rows (83, no tests changed)

**Self renames (RENAMES_applied.tsv section 1; 18 rows, 16 of them file moves via `naming/batch_rerow.py`):**
- 0x00535CD0 ContainsPoint returns bool and now uses the shared C2DBoxF.h.
- 0x00989700 is CShaderRenderManager::UpdateCameraPos (private). It was a second `UpdateAmbient`; the real UpdateAmbient is the
  landed 0x00989760. Both PDBs list UpdateCameraPos before UpdateAmbient, the retail order. 27 landed callers of 0x00989700 were
  graded, and all bind `?UpdateCameraPos@CShaderRenderManager@@AAEXXZ`. The other two, 0x00BE6F70 and 0x00BF4570, are July rows
  built on self-contained `candidates/*_compiled.h` headers that never name the callee.
- 0x00A23B50 / 0x00A23B60 / 0x009886F0 / 0x00988700 are named on shared headers.
- 0x007021F0 GetTeleporterRegionName, 0x009F2550 GetNoAllocatedAreas, 0x00E53670 IsHeroWearingAnyBeard.
- Access and signature fixes: 0x005E3EA0, 0x007C3EB0, 0x00813B80, 0x00A892E0, 0x009BAA50, 0x0074DD20, 0x00842C70, 0x0075C420
  and 0x006CB840.

**Caller respellings (section 2):** 71 lines. Among them:
- IsOpeningHours const (7 rows), GetAbilityLevel(EHeroAbility) const (13), GetWorld protected (5);
- SetAttackerAsReadyToAttack(const&) (6), NCreatureAI IsToAttackTarget (4), GetCRC (3), IsSellable (3), AddToLoadingQueue (3);
- the p23 callers from `2026-11-16_landing_p23/callers_wrong_symbol.tsv` that two sources prove.

**Recheck (`logs/recheck.log`):** all 83 rows, graded from the repo after landing. 7 MATCH and 76 RELOCATION_MATCH, every test
PASS. 0x00927C80 needs its catalog `/Oa`, as landed.

**Bindings:** `naming/rebind.py` over the recheck removed 183 lines and added 158. bindings_multi_check is at its baseline (the one
known `NullCoord` entry), and funclet_includes_check reports 1,355 checked, 0 broken.

## Headers (opt-in only; the default path is unchanged)
- **engine/CTCInventoryItem.h, `FABLE_RETAIL_CTCINVENTORYITEM_ISSELLABLE`:** `IsSellable` is 0x005D9B30. The coordinator corrected two
  comments that gave `IsBuyable` that address. The PDB layout puts IsSellable at +0x5C and IsBuyable at +0x5D, and the retail
  bodies at 0x005D9B30 and 0x005D9B50 read exactly those offsets.
- **engine/CAIStateGroupBase.h, `FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER_PROTECTED`:** a `protected:` label.
- **engine/CMeshDataBank.h, `FABLE_RETAIL_CMESHDATABANK_SETTEXTUREBANK`:** a declaration.

## Open (partial entries; details in NOTES.md)
- **Self rows still wrong while their callers now bind the PDB name:**
  - 0x005D9B30 is the placeholder `_FableIsBuyable_5d9b30`, and the catalog still names it IsBuyable;
  - 0x00B8D090 / 0x00B8D0A0 are placeholders;
  - 0x009FC3D0 is the CResourceBank_Retail stand-in;
  - 0x005EDE93 is spelled `QAEHH`.
- **Stand-in binders left:**
  - 0x0057770A / 0x005778CA (CTCHeroAbilities);
  - four GetPDefinitionManager rows;
  - 0x00B906C0;
  - 0x00A25BD0 / 0x00A42980;
  - 0x0065C781 GetCRC;
  - 0x008909F0 / 0x00929820 GetThingOpinionOfPlayer.
- **Authoring:** 0x008F8F50 Send's binder 0x008F95D0 needs the STLport vector model; 0x005BFB96 Copy overrides; 0x00E1FEB0 needs
  the NScript namespace move.
- **0x00802390 GetMaxTargetingDistance:** the self rename needs the full FABLE_RETAIL_CTCTARGETINGPLAYER chain, and one compile on it
  hung until its timeout.
- **File names:** the tests of renamed rows keep their old names inside, and they pass unchanged.
