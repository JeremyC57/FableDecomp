# NF148 overlay (2026-11-20, q11785 one vtable)

Bases: copies of the repo working tree headers at 08:31 (orig/engine/), edited in place here. Add-only `#ifdef` opt-ins; without them
each header is token-identical to its base apart from pin_asserts' `_AT` form.

- engine/CCreatureAction_ThrowShotObject.h (inside the `FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLASS` branch):
  - `FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_CLONE`: `virtual CCreatureActionBase* Clone() const;` (public; 0x00956960, slot 11;
    Ego_r 0x0097B4B0 + FableWin 0x02BF6900 `?Clone@CCreatureAction_ThrowShotObject@@UBEPAVCCreatureActionBase@@XZ`).
  - `FABLE_RETAIL_CCREATUREACTION_THROWSHOTOBJECT_INITIALISE`: `virtual void Initialise();` (0x008B66D0, slot 12; Ego_r 0x008EF1D0 +
    FableWin 0x029FFB90 `?Initialise@CCreatureAction_ThrowShotObject@@UAEXXZ`).
  - Rule written in the header: every row that emits the vtable from `_CLASS` defines `_CLONE` and `_INITIALISE` together.
  - pin_asserts: pinned 1 (the `_CLASS` branch's `FABLE_STATIC_ASSERT(sizeof(CCreatureAction_ThrowShotObject) == 0x114)` -> `_AT(24, ...)`).
- engine/CCombatActionBase.h (MELEE branch, after `_TARGETCTOR`):
  - `FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR`: `public: CCombatActionBase(const CCombatActionBase& other);` (0x0062F3B0; Ego_r
    0x00545FD0 + FableWin 0x01D919A0 `??0CCombatActionBase@@QAE@ABV0@@Z`; the first branch already declares it).
  - pin_asserts: pinned 0 (the only plain assert, line 133, is above the insertion; the `_AT(245, ...)` one keeps its pin).

Users: engine/CCombatActionBase.h has 168 landed includers under rebuild/src/compiled; engine/CCreatureAction_ThrowShotObject.h has one
(0x009551A0, in this family). No landed row or test defines any of the three new macros (grep), so the opt-ins change none of them.
Sibling overlays read first (NF147: CAnimFilterOfGroup / NCreatureMode_*; RP1: CVsyncCallbackTimer; NF145: CDisplayManager /
CMemoryAllocator): none touches these two headers.

Users of this overlay: only the q11785 family (0x009551A0, 0x009566C0, 0x00956960, all in rows/); 0x00BF3B60 / 0x00994930 include
neither header. Bases re-checked equal to the tree after nf145 landed (08:41).
