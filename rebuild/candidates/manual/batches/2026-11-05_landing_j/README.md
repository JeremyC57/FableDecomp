# Landing 2026-11-05j: chunk 783 (1 of 6 rows landed, 3 parked, 2 not attempted)

One expert agent authored the rows. The landed row is plain C++ and grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x004321F0 | NUISystem::CUIMiscThingsDef::operator= | RELOCATION_MATCH PASS (4 grades) |

- **Name.** The ledger called 0x004321F0 `CSpecialAbilitiesBerserkDef::operator=`, but BerserkDef's own operator= is landed at
  0x0045F854. Ego_r and vtable 0x01230EEC slot 19 give CUIMiscThingsDef.
- **How it matched.** Retail expands the CParentDefClassBase assignment and the CDefClassBase assignment inside it. Only the
  CResource copy 0x00431F60 stays a call. This needs three opt-ins:
  - the implicit CDefClassBase operator=;
  - a declared CResource operator=;
  - a `__forceinline` member-wise CParentDefClassBase operator=.
  An implicit or plain inline parent assignment leaves a call to the 0x00431ED0 COMDAT.
- **Headers** (`hdr_README.md`). Every change is a new opt-in branch or a new file, and hdrcheck2 reads `default=same` for each one.
  - Opt-ins: CDefClassBase (IMPLICIT_ASSIGN), CResource (ASSIGN), CParentDefClassBase (PDCB_FORCEINLINE_ASSIGN),
    CRenderManagerCore (DRAWASINDEXEDTRIS), CDisplayManager (GETVIEWPORT), CTexture (BLTTORENDERTARGET).
  - New headers: CTVertexRHWColTex1.h, CQ_BanditCampBossBattleScript.h and CQ_PrisonRaceScript.h. The last two hold only the
    nested guard classes.
- **Landing:** `vland --land` (`logs/land.log`). Labels: `ghidra_out/labels_landing_2026-11-05j.tsv`. There are 4 queue lines (tag
  `land0511j`):
  - 0x00431F60 is CResource::operator=; some binders spell it CDefPointeeBase::operator=.
  - 0x00432B38 is the `_Rb_tree<CCharString, pair<const CCharString,long>>` assignment; some binders spell it as a map's or a
    row-local class's.
  - 0x00A0AEA0 CRenderManagerCore::DrawAsIndexedTris.
  - 0x007EBE50 is CTCActionUseBed::OnSerialise (landed as vfunc_1).

## Parked (`pending/2026-11-05_linux_hand783/`)
- **0x009FA780 CTexture::BltToRenderTargetWithGPU:** DIFFER(2028v2028) after 25 attempts. 4 real lines differ: the two
  bookmark-mask loads in the inline SetRenderState (0x009FAA34) use swapped registers. This is the CRenderStateManager.h lane gap.
- **0x00DDB720 CQ_PrisonRaceScript::CPrisonRaceGuard::Main:** DIFFER(2150v2158) after 8 attempts. At four returns after a
  loop, retail calls ~CTimer 0x00CD4470 out of line; VC7.1 expands it.
- **0x00D07490 CQ_BanditCampBossBattleScript::CGate3Guard::Main:** DIFFER(2089v2182) after 10 attempts. VC7.1 makes different
  per-exit inline choices for ~CTimer and the movie dtor.
- **Not attempted (out of time):** 0x00A15890 (CNavigatorManager::OutputWorld, analysed in agent_NOTES.md) and 0x007EBEA0
  (CTCActionUseBed::OnActionUse). Both are carried into chunk 784.
