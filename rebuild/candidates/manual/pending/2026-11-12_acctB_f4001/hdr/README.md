# hdr/ -- overlay headers of chunk 4001 (row 0x00CE7670 NScript::CGameflowScript::Main)

Grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent4001/hdr`. The row does not compile without it.

## engine/CGameflowScript.h -- NEW (no repo header existed)
- What: `namespace NScript { class CGameflowScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> }` with the four virtuals
  RegisterMain / Main / Init / OnPersist, DeclareGossipCategories, CoreQuestReminder, CheckBarrowFieldsGuards, and the members
  CoreQuestWaiting +0x48 (unsigned long), SavedScriptNames +0x4C, SavedCardDefNames +0x58 (std::vector<CCharString>),
  `FABLE_STATIC_ASSERT(sizeof == 0x64)`. Requires FABLE_RETAIL_NSCRIPT (an #error otherwise), like the other NScript models.
- Evidence: (1) Ego_r.pdb layout (ghidra_out/struct_layouts_egor.tsv, CGameflowScript: the three members in this order and with
  these names; PDB offsets 0x5C / 0x60 / 0x70 shift to retail's 0x48 / 0x4C / 0x58 because retail's CLevelScriptBase is 0x48 and
  STLport vectors are 12 bytes); (2) retail bytes: Main stores CoreQuestWaiting at `[esi+48h]` (0x00CE84DE and 15 more), the
  landed OnPersist 0x00CEF8E0 transfers +0x48 / +0x4C / +0x58, the landed CoreQuestReminder 0x00CEF3B0 reads +0x48. Method
  names: Ego_r.pdb and FableWin.pdb name sets; slots from the script vtable 0x012C3FA4.
- Path: a new file, so no landed includer changes. Debt: the landed siblings (0x00CEF3B0, 0x00CEF8E0) carry local class
  models and can be retyped with it.

## engine/CScriptGameResourceObjectBase.h -- one opt-in added, default text unchanged
- What: `#ifdef FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR` declares `CScriptGameResourceObjectBase();`
  (declared, not expanded), placed before the virtual destructor. Same kind as the existing
  FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR / ..SCRIPTEDTHINGBASE_OUTOFLINE_CTOR opt-ins.
- Evidence: (1) retail calls the COMDAT 0x006E7A60 at 0x00CEE967 inside the expanded CScriptGameResourceObjectMovieBase ctor
  of Main (the expansion then sets up the movie's own members itself), and at 0x00D315EC and 0x00D3173E (`python3 $S/w23/xref.py 006e7a60`);
  (2) 0x006E7A60 is the landed row CScriptGameResourceObjectBase::CScriptGameResourceObjectBase. With the implicit inline ctor,
  c2 expands it at this site (trace: sz 18, cost 21, f = 1, score 21 < any threshold) and calls 0x0099A380 instead.
- Path: opt-in; without the define the header is byte-for-byte the repo file (diff: 8 added lines).
