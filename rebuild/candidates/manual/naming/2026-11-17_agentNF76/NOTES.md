# NF76 notes (round 7: the nine rows NF72 / NF73 left blocked)

Work dir: $S/w22/agentNF76. Tools copied from NF68 (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh, xref2.py) and repointed; ndiff.py is
a masked retail-vs-built listing diff, mkren.sh assembles RENAMES.tsv from ren1.tsv / ren2.tsv. Grades through g.sh (ledger
grades.log), drafts in work/, port relocation dumps work/x_<addr>.txt, per-row vland logs work/vl_<addr>.log, final recheck
recheck_final.log (all nine RELOCATION_MATCH + PASS). Ego_d.pdb read only with `python3 -I .../egod_locals.py`.

## Result
- Ported 9 / 9 (all RELOCATION_MATCH + test PASS), not-a-binder 0, blocked 0, deferred 0. No extra row claimed.
- Entries: 13 / 13 done (APPLIED.tsv). nf44:54 / nf44:7: done for the slice; the twin 0x0057770A (NF70 blocked/port, unlocked)
  still binds the stand-ins and the 0x005778CA recipe ports it.
- No `blocked/header` item: every missing member went into an overlay opt-in (hdr/README.md, 22 headers, base copies in orig/,
  all still identical to rebuild/include at hand-back).

## Rows (status per STATUS.tsv)
- 0x006D53C0 CTCFollowed::FrameUpdate (7 grades): NF73's draft failed because the shared CTCBase names the owner `ParentThing` (no
  `Thing` member); the VC7.1 error only surfaces after a later statement. Rewritten from the landed body on the headers; the
  idle-reaction PostDeed reads the owner through GetParentThing() (DIFFER 786v788 -> RM).
- 0x00970CD0, 0x00A79930, 0x00B41FA0, 0x00B42530 (1 grade each): written from the retail disassembly, the Ego_d locals and a landed
  sibling's macro block (0x0097CCD0 / 0x00970590, 0x00A79A30, 0x00B42750, 0x009CE5E0 / 0x00BEAE50). Local models kept only for
  classes with no shared header: CBankFileAsyncEntry (0x00B41FA0, Ego_r.pdb members) and a CEngineLandscapeMap declaration (0x00B42530).
- 0x005778CA (2 grades): self name CloseInventoryInternalPC (UAE, slot 90). The sibling's `#pragma optimize("y", off)` adds a frame
  pointer (DIFFER 503v502); removed -> RM.
- 0x00599E3F (1 grade): NUISystem::CFrontEndManager::Update on the header's full-layout (DESTRUCTOR) branch with the 0x00596CCE macro block.
  CNewFrontendGameComponent::StartGame has no out-of-line copy and no PDB body: the inline `SetQuit(); m_bCreateGame = true;` is
  reconstructed from the retail bytes (name Ego_r.pdb). The CFileInstaller singleton data 0x013CA818 is spelled `PClass` because the
  shared engine/CTBaseSingleton.h names the member so (one source: by analogy with CTBaseSingletonNoDefaultAlloc); landed rows spell it
  `Instance` (0x0098E320), `PInstance` (0x0059A238) and `g_pointer_013ca818` (0x00404490). Not in NAME_FIXES (no second source);
  the coordinator may want one spelling per address.
- 0x00549B20 (1 grade): its real name is NUISystem::CTextSlider::GetDataFromDef (Ego_r.pdb 0x005C3860 `?GetDataFromDef@CTextSlider@
  NUISystem@@QAEXXZ`, declared on engine/NUISystem_CTextSlider.h; Ego_d.pdb's GetDataFromDef record has exactly this body's locals:
  pdefinition, paction x2, paction_click x2, paction_accept). "Initialize" was a forge autoname. The def is CUIDef (Action +0xC4,
  ActionOnSelected +0xCC, InputDelay +0x184, SliderLeft +0x18C, SliderRight +0x190; no CTextSliderDef exists). The old descriptive
  callees are header members: SetTarget 0x0053FDE0 = CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) with the slider's
  CActionParam base (+0x18); slot 67 = AddActionOnSelected, slot 80 = AddActionOnLeftUnclicked (the arrows), slot 108 =
  LoadAndReturnDef, slot 143 = GetValueFromProfile (the old "RefreshSelection"). Action types are UI_ACTION_TYPE_SEND_OWNED_EVENT_
  FORCE_OBSERVATION (291) with a CActionParamSLONG(UI_EVENT_SLIDER_RIGHT / _LEFT) and UI_ACTION_TYPE_ACCEPT_CHANGES (87).
  The only landed caller, the ctor 0x00549F60, binds ?Initialize -> NAME_FIXES. Overlay opt-in: CACTION_ADDPARAM_INLINE.
- 0x0041B550 (1 grade): NUISystem::CSprite::SetGraphicIndex (virtual, slot 145). The landed CIconText::AddIcon 0x00542780 has the same
  PeekGraphicInfoFrame / BeginLoading / GetFrame sequence; its macro block over engine/NUISystem_CSprite.h matched on the first
  grade with no overlay change (Size is CComponent::Size +0xCC/+0xD0). NF73's draft used the SURFACECOMPOSITION branch, which lacks
  the width/height virtuals and the GetFrame touch block.

## Entries
- APPLIED "done" means every slice binder is ported. callers.py / reloc_bindings.tsv on the targets: 0x0065F17B is also called by
  0x006DEEE0 / 0x0089FEB0 (0x0089FEB0 on the queued spelling); 0x00970B60 by 0x0097A630 (queued spelling); 0x00A84160 by 0x00A79A30
  (queued) and 0x00A84590 / 0x00A84B10; 0x009D58D0 by 13 rows (NF72 ported 3). The four 0x0041B550 targets (0x009FBF90, 0x009FBFB0,
  0x009FE620, 0x009FCBA0) and 0x012366F4 had no other wrong-spelling binder in reloc_bindings.tsv.

## Coordinator items
- RENAMES: section 1 has 9 self renames (two change the catalogue name: 0x005778CA vfunc_90 -> CloseInventoryInternalPC, 0x00549B20
  Initialize -> GetDataFromDef; the landed files keep the old names, e.g. NUISystemCTextSlider_Initialize_00549b20.cpp and
  NUISystemCSprite_vfunc145_0041b550.cpp). Section 2 lines were read from object dumps (port: work/x_<addr>.txt) against
  reloc_bindings.tsv; a few landed-side spellings come from the landed declarations (CullNullFollowers QAE, ??0CBaseClassNonCopyable
  QAE, the two ~CMemoryDataInputStream lines, ??_7CActionParamLong for 0x00549B20's dir32), not from an object dump.
- NAME_FIXES (13): callers / other binders outside the slice that bind a wrong spelling, and catalogue labels the final recheck
  flags (0x0066E799, 0x0059B039, 0x0087C390, 0x0087C3F0, 0x009D52B0; 0x00A84160's label is the demangled sort, a format difference).
- recheck_final.log warnings: `WARN STANDIN_LEAF` on 0x00549B20 is only the leaf-versus-catalogue mismatch (standin_leaf.py says
  "names another function" whenever a non-placeholder catalogue name differs; 0x005778CA's vfunc_90 is a placeholder, so it stays
  quiet); it goes away when the RENAMES section-1 catalogue rename Initialize -> GetDataFromDef is applied. No other address carries
  GetDataFromDef (auto-re-candidates.tsv, labels_*.tsv, rebuild/src/compiled: none). `WARN RELOC_ALIAS __wcsicmp` on 0x00599E3F is
  the MSVCR71 import thunk 0x00F3FB84 (the CRT name, expected). The RELOC_NAME lines are catalogue labels (NAME_FIXES above).
- PDB citations were checked in ghidra_out/egor_pdb_names.tsv / fablewin_pdb_names.tsv: SetGraphicIndex (Ego_r 0x0042CD20, FableWin
  0x01D2EFC0), ??0CFrameData@CGraphicInfoFrame (Ego_r 0x00B589A0, FableWin 0x031274B0), both AddParam overloads (Ego_r 0x005B9350 /
  0x005B93A0, FableWin 0x021A8630 / 0x021A8660), GetDataFromDef (Ego_r 0x005C3860, FableWin 0x02273B90). `PClass@?$CTBaseSingleton`
  is in neither file (hence no NAME_FIXES for 0x013CA818). The 0x0054A2A0 definer is protected with std::__false_type in its own
  model, i.e. the IAE ... ABU__false_type@2 spelling in NAME_FIXES.
- No EH funclet under rebuild/src/compiled/01/21 includes any of the nine rows (grep of each file name).
- The tests are unchanged: each is self-contained (copies its own model) and still PASSes against the ported rows.
