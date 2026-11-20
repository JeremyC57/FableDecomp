# NF141 header overlay (round 10)

Base copies of every overlaid header are in `../orig/` (copied from the repo working tree at the start of this pass).

## engine/CAIStateGroup_StealStealableItem.h (copy of rebuild/include/engine/CAIStateGroup_StealStealableItem.h)
Opt-ins added inside the retail class (FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM branch), right after `virtual void OnReplace();`,
member functions only (no data, no layout change; the access labels inside the block are restored by the existing `protected:` that follows):
- `FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_ISRUNNABLE`: `virtual bool IsRunnable();` public virtual override (slot 11),
  Ego_r.pdb 0x00989760 + FableWin.pdb 0x02C38320 ?IsRunnable@CAIStateGroup_StealStealableItem@@UAE_NXZ (row 0x00964DA0).
- `FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPSTEALABLEITEM`: `private: CThing* FindPStealableItem() const;`,
  Ego_r.pdb 0x00988C30 + FableWin.pdb 0x02C3AE50 ?FindPStealableItem@CAIStateGroup_StealStealableItem@@ABEPAVCThing@@XZ (callee of 0x00964DA0).
- `FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPESCAPEPOINT`: `private: const CThing* FindPEscapePoint(bool nearest) const;`,
  Ego_r.pdb 0x00989660 + FableWin.pdb 0x02C3AED0 ?FindPEscapePoint@CAIStateGroup_StealStealableItem@@ABEPBVCThing@@_N@Z (rows 0x00964CA0, 0x00964DA0).
Without the three macros the header is token-identical to the base copy (the additions are all inside `#if` blocks).
pin_asserts.py: pinned 1 (`FABLE_STATIC_ASSERT_AT(33, sizeof(...) == 0x3c)` of the retail branch keeps its original line tag).
Landed includers (grep of rebuild/src/compiled): 0x008FE4D5, 0x009644F0, 0x009643D0, 0x00964360, 0x00964DA0 (changed here), 0x0121F86F.
None of the others defines any of the new macros, so the opt-ins change none of them.
No other agent (NF139: CBankFile.h; NF140: CMemoryAllocator.h) overlays this header.

## engine/CGraphicDataBank.h (copy of rebuild/include/engine/CGraphicDataBank.h)
- `FABLE_RETAIL_CGRAPHICDATABANK_CGRAPHICHEADER_LOAD`: a `class CDataInputStream;` forward declaration before the nested-class definition
  and `void Load(CDataInputStream& stream);` as the first (public) member of `CGraphicDataBank::CGraphicHeader` (the definition guarded by
  FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT / _CGRAPHICINFOFRAME_FREE / _CGRAPHICINFO_SPRITE). Ego_r.pdb 0x00B568D0 + FableWin.pdb 0x03121EB0
  ?Load@CGraphicHeader@CGraphicDataBank@@QAEXAAVCDataInputStream@@@Z (retail 0x009FD160). Rows 0x009FF590 and 0x009FD6C0. A member
  function only: no layout change.
Without the macro the header is token-identical to the base copy. pin_asserts.py: pinned 0 (FABLE_STATIC_ASSERT_AT(95, ...) keeps its tag).
Landed includers of CGraphicDataBank.h / CGraphicInfo.h: 111 rows (grep of rebuild/src/compiled); none defines the new macro (grep), so the
opt-in changes none of them. No other agent overlays this header.
