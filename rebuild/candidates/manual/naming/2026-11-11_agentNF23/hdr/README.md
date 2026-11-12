# agentNF23 header overlay

One header, one opt-in. Grade with `FABLE_HDR_OVERLAY=$S/w22/agentNF23/hdr`.

| Header | Opt-in macro | Used by | What it adds |
|---|---|---|---|
| `engine/CTCPhysicsStandard.h` | `FABLE_RETAIL_CTCPHYSICSSTANDARD_FLUSHONGROUND` | row 0x00723A10 | `public: virtual void SetRHSetAsFlushOnGround();` at the end of the class. Ego_r.pdb 0x0073A6D0 / FableWin.pdb 0x02527020 `?SetRHSetAsFlushOnGround@CTCPhysicsStandard@@UAEXXZ`; CTCPhysicsBase declares the slot. It is a declaration only, so the layout does not change. |

The file is a full copy of the current repo header with this one `#ifdef` block added before the class's closing `};`. Nothing else changes.

The other rows use existing repo headers and opt-ins only:
- `engine/CWorldMap.h`: `FABLE_RETAIL_CWORLDMAP`, `_GROUNDFACENORMAL`.
- `engine/CTCNoise.h`, `engine/CThing.h`.
- `engine/CIntelligentPointer.h`: `FABLE_RETAIL_CINTELLIGENTPOINTER`, `_CTOR_OUTOFLINE`, `_ASSIGN_OUTOFLINE`.
- `engine/CMiniMapDisplay.h`: `FABLE_RETAIL_CMINIMAPDISPLAY`.
- `engine/NEntityEvents_CEventHitBase.h` is not used; see NOTES.
- The STLport headers.
