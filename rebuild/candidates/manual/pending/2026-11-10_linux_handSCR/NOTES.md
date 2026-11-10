# agentSCR notes (2026-11-10)

## 0x005C8459 CTCInventoryWeapons::ConstructScreen: RELOCATION_MATCH (3,235 bytes)
## 0x005C9A95 CTCInventoryWeapons::ConstructScreenPC: RELOCATION_MATCH (3,065 bytes)
Both rows share one TU shape. Grade them with the overlay (hdr/README.md). Both portable tests pass.

Shape devices (all plain C++; no asm and no inline_depth pragmas):
1. **Same-TU template bodies.** The bodies of `CDefClassBase::GetSubDef<T>` and `CDefinitionManager::GetDef<T>` sit under `#pragma auto_inline(off)`.
   - With the bodies visible, VC7.1 sees that the CDefPointer locals do not escape. That gives retail's in-place `dec [ecx+4]; jne` releases, which fixes hand900 diff (a), and its register choices.
   - The calls still bind the landed instantiations (0x005935C4-style GetSubDef, 0x0042AF3C GetDef).
2. **MSVC-only noinline stand-ins** for the unlanded same-TU callees ConstructAugEntry(PC) and ConstructWeaponItemEntries(PC). Their mangled names are exactly the Ego_r ones (checked in the objs).
   - Each stand-in calls a never-defined `void StandInBody();`. An empty body would be folded away together with its call sites.
   - These stand-ins fixed the misc-def release, the `empty()` operand order, the InitialiseOffsets registers and the viewport spill.
3. **FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR** fixes the frame, 0xE4 → 0xE0 (diff (b)).
4. **The new FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE opt-in** puts the shot-def `HitThingDamage` load (+0x54) before the dec.
5. **Hand-expanded damage blocks.** Retail expands GetActiveMeleeWeaponDamage 0x005C4769 and GetActiveProjectileWeaponDamage 0x005C3180 inside the screens, so the source writes them out.
   - The melee block uses `PeekTC` with a `const CTCWeapon*`, which binds the const LowerBound 0x004365B0.
   - The ranged block reads CWeaponDef::Shot (+0xA0) and CThingShotDef::HitThingDamage (+0x54). Both use shared headers, so the unproven local models are gone.
6. **Spelling levers:**
   - `augmentations.size() > 0` (diff (d));
   - implicit-literal `SetValue("...", true, true)` (diff (c));
   - `description.Reset(p)`;
   - a named `CText*` for the ranged item title.
   - All 32 (console) and all PC string literals were checked against retail.

Callees not yet landed, named by exact Ego_r spelling and bound by RELOCATION_MATCH:
- ConstructAugEntry 0x005C574E
- ConstructAugEntryPC 0x005C5F11
- ConstructWeaponItemEntries 0x005C66B2
- ConstructWeaponItemEntriesPC 0x005C8176

Header debt:
- engine/CWeaponDef.h is a new retail model. The flattened Ego_r copy stays in _quarantine.
- engine/CTCInventoryWeapons.h is new (from hand900).

## 0x005B8743 CTCInventoryClothing::ConstructScreenPC: PARKED (not compiled)
differ/005b8743.cpp is a flow skeleton built from the annotated listing. It is not a compilable draft, so there is no instruction diff to report.

Function profile: 3,838 bytes, frame 0x124, eight vectors and about 20 CCharString / CWideString temporaries.

Blockers:
- **No retail header.** engine/CTCInventoryClothing.h is a thin generated header with the _MENUENTRIES / _ARMOUR / _CALCULATE opt-ins only. It lacks:
  - the NHeroInformationScreens::CBase members (+0x130.., including the +0x15C flag set here);
  - the ConstructScreenPC / ConstructClothingListPC / InitialiseMannequin / SetUpSelectedClothes declarations.
  A FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN opt-in would be needed.
- **Six callees have no proven spelling:**
  - 0x005BC237: vector insert/push_back of a vector<pair<long,long>> (called six times);
  - 0x005BBA94: vector<long>::push_back;
  - 0x005BB3D1: map ctor;
  - 0x005BBC4E: map operator[];
  - 0x005BB090 / 0x005BC080: list dtors;
  - 0x005B6A66: probably ConstructClothingListPC (Ego_r donor 0x00562A81). It is called once per part from the menu-entry loop.
  The landed catalog names for these are descriptive (`std::map<T_005bb3d1>`, `StdList_Dtor_*`), not mangled.
- **agentNF owns the suit-part instantiations.** The suit-part walk iterates a set<EClothingSuitPart> (`_Helper` 0x004292C0), and 0x005BBC4E is a map keyed by EClothingSuitPart. Both belong to agentNF's respelling (codex20 namefix_work.json entries 5..10), which is still in progress.
- **Conditional CCharString temporaries.** The `HERO_SUIT_NAKED` … `OBJECT_HERO_NO_BOOTS` short-circuit chain keeps its temporaries alive behind a flag word ([ebp+6Ch] bits 1..0x20) with conditional destructors. This is the VC7.1 shape for a `&&` chain of comparisons against `CountOf(CCharString(...))`, so it should come out of one `if` expression.

Suggested next step: write the CTCInventoryClothing screen opt-in from Ego_r CTCInventoryClothing plus the CBase layout, wait for agentNF's spellings, then draft from the skeleton.

## Extra: 0x005C3180 GetActiveProjectileWeaponDamage (landed row, wrong symbol)
The landed row uses local struct hacks and the NInventory namespace. Its correct symbol is `?GetActiveProjectileWeaponDamage@CTCInventoryWeapons@@QBEMXZ` (see NAME_FIXES.tsv).

With this overlay, the body as written in the screens (shared headers plus the POINTEE opt-in) reached RELOCATION_MATCH in a probe. The landed row could be re-authored that way; it is not handed back as a row here.
