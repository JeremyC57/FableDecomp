# Chunk 833 header changes (all opt-in; the default path is unchanged)

Every header is a fresh copy of rebuild/include/engine/<name>.h. The edits only add `#ifdef` blocks: nothing removed, no existing line
reflowed (checked with plain `diff` against rebuild/include). Every new declaration sits inside the opt-in.

| header | opt-in macro(s) | lines | what it adds |
|---|---|---|---|
| CQ_BountyHuntScript.h | FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY1 / _HOSTAGE1 / _HOSTAGE2 | +77 | in-class forward declarations of CBH_GangDeputy1 (Alloc 0x00D15FC0), CBH_Hostage1 (Alloc 0x00D160B0) and CBH_Hostage2 (Alloc 0x00D16290), plus `void WatchForDeputy1GruntsKilled(); // 0x00D18B20`; the three class definitions (CEntityScriptBase + ParentClass& +0x14 + CQ_SunnyvaleMasterData* PMasterData +0x18, assert 0x1c) with Alloc / GetParentScript / Main / Init / OnPersist / OnPredicateFail |
| CQ_WhiteBalverineKnotholeGladeScript.h | FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_CKG_CHIEF | +22 | CKG_Chief: Alloc 0x00E17750, GetParentScript 0x00E177F0, private Main 0x00E17840 / Init 0x00E17800 / OnPredicateFail 0x00E17810, ParentClass +0x14, PMasterData +0x18, assert 0x1c |
| CQ_WhiteBalverineWWScript.h | FABLE_RETAIL_CQ_WHITEBALVERINEWWSCRIPT_CKG_CHIEF | +22 | CKG_Chief: Alloc 0x00E192E0, GetParentScript 0x00E19380, private Main 0x00E19D80 / Init 0x00E19390, ParentClass +0x14, PMasterData +0x18, `bool InitialChatted; // +0x1c`, assert 0x20 |
| CQ_NewOakValeIntroScript.h | FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_AFFAIRWOMAN | +24 | in-class forward declaration of CNOVI_AffairWoman (Alloc 0x00DB1DB0); the class definition (Alloc, GetParentScript 0x00DB1E50, private Main 0x00DB1F00 / Init 0x00DB1E80, ParentClass, PMasterData, assert 0x1c) |

## Evidence
- Layouts: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv). Entity offsets equal retail's (CEntityScriptBase is 0x14 in both), and each
  size equals the landed Alloc's `new` size (0x1c, or 0x20 for WW CKG_Chief, whose landed Init 0x00E19390 clears +0x1c).
- Vtables read from retail .rdata (bin/rd.py):

  | Script | Vtable | Main | Init | GetParentScript | Other slots |
  |---|---|---|---|---|---|
  | KG CKG_Chief | 0x012E0B98 | 0x00E17840 | 0x00E17800 | 0x00E177F0 | OnPredicateFail 0x00E17810, OnPersist inherited 0x00CDEBC0 |
  | WW CKG_Chief | 0x012E0E68 | 0x00E19D80 | 0x00E19390 | 0x00E19380 | OnPersist / OnPredicateFail inherited |
  | AffairWoman | 0x012D8C08 | 0x00DB1F00 | 0x00DB1E80 | 0x00DB1E50 | OnPersist / OnPredicateFail inherited |
  | Deputy1 | 0x012CA554 | n/a | n/a | n/a | n/a |
  | Hostage1 | 0x012CA570 | n/a | n/a | n/a | n/a |
  | Hostage2 | 0x012CA5A8 | n/a | n/a | n/a | n/a |

- Main / Init are private: the PDB spellings are `EAEXXZ`.
- WatchForDeputy1GruntsKilled: Ego_r.pdb `?WatchForDeputy1GruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ`. Its member pointer
  0x00D18B20 is stored at CSpawnedFunc +0x34 by Main 0x00D17F20.

## Includers rechecked (read-only, FABLE_HDR_OVERLAY=hdr, vland --recheck-landed)
All 10 landed includers are RELOCATION_MATCH + PASS (SIBLINGS.tsv):
- 00E158F0, 00E18630, 00E13F10
- 00D17470, 00D13130
- 00DB09E0, 00DB86B0, 00DB2B10, 00DBCD60, 00DB6C60

Since no default-path line changed, no INCLUDERS_<Class>.txt is needed.
