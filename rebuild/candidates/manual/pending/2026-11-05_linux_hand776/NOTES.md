# Chunk 776: the last 3 landed asm bakes, as plain C++

| address | name | grade | attempts | file |
|---|---|---|---|---|
| 0x0053C332 | NUISystem::CList::DoRecomputeOffsets | RELOCATION_MATCH (vland PASS) | 2 graded + 10 compile-only probes | src/0053c332.cpp |
| 0x00597BF2 | NUISystem::CFrontEndManager::GotoPreviousScreen | RELOCATION_MATCH (vland PASS) | ~50 | src/00597bf2.cpp |
| 0x009BEF80 | CDisplayManager::SetViewport | DIFFER(477v479) dl=3, parked | ~30 families / ~860 grid builds | differ/009bef80.cpp |

Matched rows: Flags none. Each uses `#pragma optimize("s", on)` + `#pragma optimize("y", off)` (the same bytes as `/O1 /Oy-`).
The CList sibling GetDataFromDef 0x0053822B uses the same TU form. Tests pass under g++ and VC7.1 /Od.

## Key spellings
- **0x0053C332.** Follow GetDataFromDef's order:
  1. `PositionOffsetToUse = PDefPositionOffset;`
  2. `PositionOffset = PDefPositionOffset;`
  3. `PositionOffsetToUse.X = -PositionOffsetToUse.X;`
  Negating before the second copy puts the fstp ahead of the Y store. That was chunk 774's 2-byte residue.
- **0x00597BF2.** All three levers are needed; no one or two of them is enough:
  - (a) `#define FABLE_RETAIL_STLP_MAP`. The visible STLport map operator[] moves the spilled map references from [ebp-8] into the dead `force` slot [ebp+8].
  - (b) A named `CComponent* current = m_stackScreens.top();` for the first compare.
  - (c) The main-menu (screen 20) branch is an early return that plays the back sound itself. VC7.1 cross-jumps it, which gives retail's tail schedule.

## Parked: 0x009BEF80 SetViewport (dl=3)
- **What is exact.** Every edge but the right one is exact. Per edge: copy `viewport.<edge>` (+1 for BR) into a local, then `if (v < 0) out = 0; else { if (v > dim) v = dim; out = v; }`, then assign `box.<edge> = out` right after the edge.
- **Residue.** On the right edge, retail emits `inc eax; cmp eax,ebp; jge`, comparing against the zero register (ebp, which also holds `top`). VC7.1 folds our `brx < 0` into the inc flags and emits `jns`.
- **What failed.** Spellings that stop the fold (compare on a CSE'd `viewport.BRX + 1`, a named zero, `0 > brx`, `>=` forms, helper inlines, std::min/max, GFLimit/GFLimitReturn, a clipped box copy, Initialise or the 4-arg ctor, declaration orders) either keep `jns` or move right into ebp and lose the zero register (dl 48+).
- **Next ideas:**
  - Make the `< 0` test compare against a value the optimiser already holds in a register. For example, compute `top` before right, so the zero register lives across the right test: hoist the top-edge `tly < 0 ? 0 :` result ahead of the right edge, then store box.TLY later.
  - Try a `long zero = left - left`-style value that the optimiser cannot fold until after scheduling.
  - Try the `/O2` TU (all rows so far were the default TU with no flags).

## Header changes
- None. The `hdr/` overlay is empty; a C2DBoxI CTOR4 overlay was tried and removed, so no hdrcheck2 run was needed and there are no siblings to recheck (SIBLINGS.tsv is empty).

## Header debt
- **SHARED_HEADER_AVAILABLE warnings.** vland warns for CUserProfileManager, CVirtualKeyboard and CFrontEndManager on 0x00597BF2 (these are WARNs, not rejects).
  - The shared engine/CFrontEndManager.h stores m_stackScreens and m_mapScreens as byte arrays (`unsigned char[0x28]` / `[0xc]`).
  - It does not declare CloseVirtualKeyboard 0x00594FA9, IsScreenOnStack 0x005959AB, GotoNextScreen 0x00596763 or GotoSpecificPreviousScreen 0x0059686D.
  - CUserProfileManager.h lacks Load 0x0040D350 and the `UseNewProfileValues` member (+0x0C).
  - CVirtualKeyboard.h (non-retail branch) lacks the ctor 0x00851700 and Initialize 0x00851770.
  - The row keeps chunk 774's local models: the STLport deque/stack, FRONTEND_SCREEN 7/20/23/26/27, CNewFrontendGameComponent, and the local CFrontEndManager.
  - Retiring them needs a header wave: a shared stlport deque/stack, typed container members in CFrontEndManager.h, and the method declarations above.
- **RELOC_NAME warnings, unverified, not in NAME_FIXES (one source only).**
  - vland maps our `std::deque<CComponent*>::back` to 0x0059B039, which is labelled `CScreenStack::Top`. But the retail bytes there (`push esi; push 0; push [esp+0Ch]; mov esi,ecx; call; mov eax,esi`) look like a deque constructor, as labels_familypass7 says.
  - vland maps `_Deque_iterator_base<CComponent*>::_M_subtract` to 0x0059A8F3, which is labelled `NThingMap::CThingMapManager::GetMemoryFootprint`. The landed file there is named FrontEndDequeIterator_Distance_0059a8f3.cpp, so that label looks wrong.
  - Worth a label_trust pass.

## Name fixes (NAME_FIXES.tsv)
- **0x0042DBEE** `?ResetAttractModeTimer@CNewFrontendGameComponent@@QAEXXZ`. Chunk 774's source spelled it RestartAttractModeTimer.
- **0x00594FA9** `?CloseVirtualKeyboard@CFrontEndManager@NUISystem@@QAEX_N@Z`. Chunk 774's source spelled it `EnableInput@CFrontEndManager`.
- Both spellings are from Ego_r.pdb and FableWin.pdb, and the catalog already has them right.

## GOTCHAS one-liners
- When a member is set by a copy and then negated, negate after the sibling's second copy (GetDataFromDef order) to keep retail's fstp/mov order.
- A shared tail written once after an if/else is scheduled like the branch. Repeating it inside an early-return branch (VC7.1 cross-jumps it) gives the other schedule (GotoPreviousScreen 0x00597BF2).
- FABLE_RETAIL_STLP_MAP (visible STLport map operator[]) moves spilled map references from [ebp-8] into a dead parameter slot [ebp+8].
- VC7.1 folds `x < 0` right after `x = y + 1` into the `inc` flags (`jns`); retail's `cmp reg,<zero reg>` form has no plain-C++ spelling found yet (SetViewport 0x009BEF80).
- SetViewport clamp shape: a per-edge local copy clamped in place inside the else of the `< 0` test, with the box member assigned right after each edge.
