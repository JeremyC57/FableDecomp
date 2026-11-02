# Chunk 714 header hand-backs

Grade with `FABLE_HDR_OVERLAY=$A/hdr`. Both changes are opt-in or additive; no member retail had is dropped.

## engine/CTCBase.h (additive, retail branch `FABLE_RETAIL_CTCBASE`)
- Rebased on the repo copy as of 05:56, which already has chunk 712's `RemoveFromGlobalTCs` line in the retail branch.
  The only chunk 714 addition is `class CWorldMap;` plus a protected
  `const CWorldMap& PeekWorldMap() const; // 0x00686D20 ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ`.
- Evidence: Ego_r.pdb and FableWin.pdb both spell it `IBE` (protected const). Ego_r has PeekWorldMap 0x006B2110 then
  GetWorldMap 0x006B2120; retail has 0x00686D20 then 0x00686D30. The body is `mov eax,[0x013B8A1C]; mov eax,[eax+28h]; ret`.
- Used by 0x00729CA0. Without the overlay, that row is SRC_FAIL.
- Merge note: if chunk 712's hand-back of the same file lands first, add only the two PeekWorldMap lines.
  `tmp/CTCBase.h.repo0556` is the base this overlay was cut from.

## engine/CSimpleMemoryPool.h (new opt-in branch `FABLE_RETAIL_CSIMPLEMEMORYPOOL`)
- Retail CSimpleMemoryPool is polymorphic: `: CBaseClassNonCopyable` with vtable 0x0129A064, sizeof 0x1C.
  - nested EAreaStatus, CArea{Size, NextFreeArea, Status} and CMemoryBlock;
  - `CSimpleMemoryPool(unsigned long, unsigned long)` 0x00A5C570;
  - `virtual ~CSimpleMemoryPool() { ClearMemoryBlocks(); }` inline (C3DAnimationManager's implicit dtor expands it twice);
  - private `ClearMemoryBlocks()` 0x00A5C600;
  - members at +4 / +8 / +0xC / +0x10.
- The old flattened text is unchanged in `#else`, so no existing includer sees a difference.
- Used by 0x00AA4170. Without the overlay, that row is SRC_FAIL.
- The retail branch was proved load-bearing with a layout perturbation: 00aa4170 turned DIFFER(75v75), then was restored.

## Includer recheck
- `INCLUDERS_CTCBase_retail.txt` (37) and `INCLUDERS_CSimpleMemoryPool.txt` (15) list the landed rows that include either header.
- All 52 were rebuilt with the overlay through read-only vland (`tmp/recheck_includers.log`): 4 MATCH and 48 RELOCATION_MATCH, all 52 test PASS.
- The WARN lines in that log are pre-existing landed-row name warnings, not caused by the overlay. Two of them are NAME_FIXES rows:
  - 0x00A5C570 ??0CSimpleMemoryPool@@QAE@KK@Z
  - 0x0099A310 ??0CInitBaseClass@@QAE@XZ
