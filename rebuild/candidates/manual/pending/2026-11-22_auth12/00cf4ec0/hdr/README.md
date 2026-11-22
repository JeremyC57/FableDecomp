# AUTH12 overlay (0x00CF4EC0), base copies in ../orig/
- engine/CTimer.h: add-only opt-in FABLE_RETAIL_CTIMER_FORCEINLINE_SET_GET (FABLE_FORCEINLINE operator= / GetTime through pDSTGame
  slots 89 / 90), placed after the size assert so no assert line moves. pin_asserts.py rebuild/include hdr engine/CTimer.h: nothing to
  pin (no FABLE_STATIC_ASSERT changed line). Default path token-identical. Why a header opt-in and not the row-local FABLE_FORCEINLINE
  of the 98 landed rows: advisor ruling 2026-11-21 (a) (row-local FABLE_FORCEINLINE bodies of header-modelled members are a duplicate
  class model; the bodies go in the shared header as add-only opt-ins). Needs header claim: engine/CTimer.h.
- Not overlaid (needs header claim, not mine): engine/EScriptAIPriority.h is gated on Codex pass-26 FABLE_RETAIL_PHASE1_ARCHER_PASS26,
  so the row keeps the local EScriptAIPriority enum every landed sibling carries.
