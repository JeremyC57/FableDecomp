# 0x007D75A0 CTCTavernGameCardPairs::GetRandomCard -- ESCALATE (AUTH30, 2026-11-27)
- Bytes: c0 (Codex, two add-only overlay opt-ins) RELOCATION_MATCH 465/465. vftables agree: 85/85 slots, 4 tables.
- Two NEQ copies, both unreached:
  - `??4CDefClassBase@@QAEAAV0@ABV0@@Z`: 102 B at 0x00431F10. The HEAD definers of the same opt-in (FABLE_RETAIL_CDEFCLASSBASE_
    IMPLICIT_ASSIGN) emit 77 B: landed 0x004321F0 and 0x00F2A250, compiled at HEAD into head/. So ours is new, not HEAD debt.
  - `??0CCardDef@@QAE@XZ`: 18 B, a row-written body (`inline CCardDef::CCardDef() {}` under the row's `#pragma optimize("s", on)` and
    `#pragma inline_depth(2)`), of landed 0x007D7A00 (51 B). No other landed row emits it.
- Retail's leaf expands the CCardDef ctor (it calls CParentDefClassBase's ctor 0x009FBEC0 directly at 0x007D75BD), so the body must
  be visible. The out-of-line copy it then emits differs from the owner's.
- The row also needs its row-text optimize toggles, which are allowed only as shared-header opt-ins.
- Question: same class as 00e0ca50: the copy is shaped by the leaf's own inline devices. Object mode, or an opt-in ruling?
Grades: c0 only (no new hypothesis outside Codex's records; its PROBES list the ctor / assignment variants).
