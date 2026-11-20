# NF138 header overlay

## engine/CTCCombat.h
Base: the repo working-tree copy (orig/engine/CTCCombat.h, byte-equal to rebuild/include/engine/CTCCombat.h when copied, cmp).
Added (retail branch, public section, right before the existing FABLE_RETAIL_CTCCOMBAT_FLOURISH block; declarations only, no layout change):
- `FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY`: `CDefPointer<const CMeleeCombatAbilityDef> GetPMeleeFlourishAbility() const;` (0x00733480;
  Ego_r 0x0074B600 / FableWin 0x025005A0). Used by rows/00733480.cpp and its test.
- `FABLE_RETAIL_CTCCOMBAT_GETPMELEEBLOCKCOUNTERABILITY`: `CDefPointer<const CMeleeCombatAbilityDef> GetPMeleeBlockCounterAbility() const;`
  (0x007334B0; Ego_r 0x0074B630 / FableWin 0x02500790). Used by rows/007334b0.cpp and its test.
- `FABLE_RETAIL_CTCCOMBAT_ISMELEEBLOCKCOUNTERVALID`: `bool IsMeleeBlockCounterValid() const;` (0x00733290; Ego_r 0x0074B420 / FableWin
  0x02500780). Used by rows/00733290.cpp and its test.
Default path (none of the three macros defined) is token-identical to the base: the only change is the three new `#ifdef` blocks
(19 added lines, no line changed or removed).
pin_asserts.py (rebuild/include -> hdr, engine/CTCCombat.h): pinned 0 (every assert in the header is already FABLE_STATIC_ASSERT_AT).
Header users: 77 landed rows under rebuild/src/compiled include engine/CTCCombat.h (grep -rlE 'engine/CTCCombat\.h'); none defines any
of the three new macros (grep of rebuild/ finds them nowhere), so the opt-ins change none of them. The other agents' overlays (rechecked at hand-back: NF136
CPixelFormat.h, CPersistTraits.h; NF137 six CAIStateGroup_* / CCreatureAction_* headers) do not touch this header.

## engine/CPhysicsMesh.h
Base: the repo working-tree copy (orig/engine/CPhysicsMesh.h).
Added (declarations only, no layout change):
- near the top, `#ifdef FABLE_RETAIL_CPHYSICSMESH_GETRIGIDBODYBOUNDINGPOINTS`: `#include "engine/C3DVector.h"` + `#include "stlport/fable_containers.h"`;
- in the public section before the BOUNDINGBOXINTERSECTED block, under the same macro: `void GetRigidBodyBoundingPoints(std::vector<C3DVector>& points) const;`
  (0x00A6CB20; Ego_r 0x00B4DCE0 / FableWin 0x030F7040). Used by rows/00a6cb20.cpp.
Default path token-identical (only new `#ifdef` blocks; 10 added lines). pin_asserts.py: pinned 0 (the header's assert is already AT form).
Header users: 11 landed rows include engine/CPhysicsMesh.h; none defines the new macro (grep of rebuild/ finds it nowhere), so none
changes. Neither NF136's nor NF137's overlay has this header.

Both orig/ copies were re-compared (cmp) with the working tree at hand-back: byte-equal.
