# Chunk 720, agent 720d (2026-11-03)

Grade everything with `FABLE_HDR_OVERLAY=$S/w22/agent720d/hdr`. The `hdr/` overlay is the full merged overlay: round4's overlay plus this round's opt-ins. hdrcheck2.py reports `default=same` for every file. Diffing each overlay file against the repo removes no repo lines except the round4 `FABLE_RETAIL_CPLAYERGUI` LiveGuiMode split, which is intended.

## Rows
| row | result | attempts |
|---|---|---|
| 0x0065B854 CTCVillage::AddThingToVillage (1502) | **RELOCATION_MATCH**. The source is `src/0065b854.cpp` and the test `test/0065b854.test.cpp` (prints `0065b854_TEST PASS`). | 13 graded, plus 3 rechecks on the final overlay |
| 0x00659800 CTCVillage::CalculateSecuritySweepTasks (1074) | **DIFFER(1074v1074)**, parked at 8 diff lines in `differ/00659800.cpp`. All 8 lines are stack-slot choices; see below. | 15 graded. Attempt 12 brought it to 8 lines; attempts 13 to 15 moved nothing. |

### 0x0065B854 levers
- **Push arguments as implicit-conversion temporaries.** Write `VillageDummies.push_back(&thing)` so the `CIntelligentPointer<T>(T*)` constructor runs on a temporary. Retail gives each temporary its own slot and re-reads it with `lea`. Named locals make the sibling switch arms share slots.
- **`CTCVillage::GetPDef()` must be `__forceinline`.** Plain inline is called out of line under optimize("s") and the frame comes out at 0x84. Forced inline keeps the two `CDefPointer<const CVillageDef>` copies in registers and gives retail's frame of 0x7C.
- **Property bits as `(bool)((props >> n) & 1)`.** This reproduces retail's `shr eax,n; test al,1`. The plain form `(props >> n) & 1` gives `test ah,..` instead.
- **The object case (5) as an `if {...; break;}` chain**, not `else if`. Only the chain reproduces retail's tail-merge placement.
- **Patrol track name comparison.** Compare `node.TrackName == PDef->PatrolTrackName` through CDefString's implicit `operator CCharString()`, which calls the out-of-line `GetString` (0x00415DD0). Retail then re-reads the `[ebp-4]` temporary. An explicit `.GetString()` reuses eax instead.
- **String equality.** `CBasicString<char>::operator==` is declaration-only and called at 0x004A93C0; `CCharString::operator==` is `__forceinline`. Both are in the overlay `fable_string.h`.

### 0x00659800 levers, in grading order
1. **Duplicated loop test.** One source call site, `while (!navigation->GetNearestNavigablePosTo(rotated + Pos, 5.0f, position))`. VC7.1 duplicates the loop test, and both copies share one candidate temporary. In the first copy, copy propagation reads `offset` for the just-copied `rotated`. `goto nextGuard` leaves the loop on angle exhaustion, so retail's jump-threaded `test al,al` comes out without a `found` flag. Effect: 231 → 119 lines and frame 0x50 before the x87 fix.
2. **`>=` angle tests.** Write `if (angle >= 0.0f) { angle += step * 0.2f; if (angle >= step) goto nextGuard; } else angle *= -1.0f;`. This gives retail's `test ah,1; jne` and its block layout, with the negate block after the epilogue.
3. **Named `float distanceSquared = offset.GetSquaredMagnitude2D();`** (with `sqrt(distanceSquared) * 0.8f`). This produces retail's x87 spill pattern: Y goes to a temporary with `fst`, X to its home with `fstp`, and X squared is computed from memory. Size becomes 1074.
4. **2D guard distance as `(Pos - nearestGuard->GetPos()).GetMagnitude2D()`.** Retail reads Y through `[ebx+4]`; the separate dx/dy form read `[esi+18h]`.
5. **Scope-based stack packing.** VC7.1 packs address-taken locals by scope, not by liveness, and proved it here:
   - `exit` at function scope never shares its slot. Inside a block that ends before `Normalise` it shares with `step` and the inverse-sqrt bit temporary, exactly as in retail.
   - So the predicate and iterator get their own block, as does the nearest-guard search.
   - The exit search, radius and guard code sit in a block whose only output is `const C3DVector* exitPosition = &exit->GetPos()`. Assigning `offset` inside that block instead makes a temporary plus `movsd`, as do `operator-=` and assign-then-subtract.
   - Effect: frame 0x54 → 0x50 and 202 → 8 lines.
6. **`const C3DVector& playerPosition = player->GetPos(); CThing* exit = 0; Position... = playerPosition;`** places `and [exit],0` between the GetPos call and the `movsd` copy, as retail does.

**Residue, 8 lines, all slots:**
- The uninitialised `HasGuardProperty isGuard` is pushed from count's slot `[ebp-18h]`. Retail gives it its own slot `[ebp-1Ch]`, shared later with nearestGuard and the Y spill, so retail pushes it before storing the count. VC treats a never-defined POD local as dead; retail's local evidently had a live range.
- The CVectorMap key temporary of the inlined `GetTC<CTCCreatureNavigation>` lands in navigation's slot `[ebp-0Ch]` instead of the dead iterator's `[ebp-14h]`.

**Tried without effect:** the predicate at function scope, count at function scope, the predicate as a const reference to a temporary, navigation zeroed inside the iterator block, and navigation declared before the block. A `HasGuardProperty()` temporary zero-fills it with `stosb`. GetTC as plain `inline` is not inlined (988 bytes).

## Task 2: the CDefPointer<const CThingShotDef> residue (0x005C6B26 / 0x005C90FC / 0x005CACB9): not solved; no header change shipped
**Sandbox.** Round4's 186-byte 0x005C3180 model (round4 `work/p1.cpp`, copied to `work/c/p1.cpp`) reproduces the residue exactly on the repo `CDefPointer.h`. The only difference is two lines: we emit `dec [esi+4]` before `mov eax,[esi+54h]`, and retail loads first.

**Variant runner.** `work/c/hv.py` rebuilds a `CDefPointer.h` variant into a scratch overlay and grades it.

| variant | result |
|---|---|
| (a) typed `T* Object` storage: Get / Release / Assign typed, InternalGetObject casting back | no change (2 lines) |
| (b) const-reference read, header side: `const CDefPointeeBase& object = *Object; if (--object.IVRefCount == 0)` | no change |
| (b) const-reference read: `const CIVCountedPointeeBase& o = *Object; o.Release() == 0` | no change |
| (b) const-reference read, caller side: `const CThingShotDef& shot = *shotDef;` | no change |
| (b) `operator->` as `&**this` | no change |
| (c) PDB member set: `Get()` through `GFDebugPDynamicCast(Object, (T*)0)` (FableWin pairs the instantiations with `GFDebugPDynamicCast<CDefPointeeBase*, const T*>`) | no change |
| `const CIVCountedPointeeBase*` local | no change |
| `object->Release(); if (object->GetRefCount() == 0)` | no change |
| const local `shotDef` | no change |
| a count reference (`unsigned long& count`) or count pointer, with `--count` either fused into the test or as a separate statement | the named pointer becomes load-before-dec, as retail, but the by-value temporary changes from retail's long form (`add eax,4; dec [eax]; cmp [eax],0`) to the flags form, so this is 4 lines instead of 2 |
| caller `SetDynamicCast` or `shotDef = ...` assignment | 189 bytes, worse |

**Conclusion.** One template gives retail's long form for the memory-resident temporary and the scheduled `dec` for the register-held named pointer. Retail's named release behaves as if the count were reached through an alias-opaque path, but no single Release body reproduces both, so no `FABLE_RETAIL_CDEFPOINTER` branch was added.

**Two further reasons not to adopt the typed form:**
- Typed `T*` storage contradicts Ego_r's layout (`CDefPointeeBase* Object`) and FableWin's `InternalGetObject` returning `CDefPointeeBase*`.
- Landed 0x005C3180 only matches through invented helpers (`CDefPointerCopy` / `DecRefToZero(long&)`). That is the same two-shape split, done by hand.

No landed-includer recheck was needed, because nothing was adopted.

## Header changes (overlay, all opt-in; default unchanged)
| file | macro | change |
|---|---|---|
| `engine/CTCVillage.h` | `FABLE_RETAIL_CTCVILLAGE_MEMBERS` | typed members: `PositionPlayerWasInWhenSecuritySweepCalculated` +0x14; the STLport lists +0x28..+0x60 typed as `list<CIntelligentPointer<..>>` or `list<CCountedPointer<CVillageTask..>>`; VillageMarkers +0x70, VillageDummies +0x74; `PDef` as `CDefPointer<const CVillageDef>` +0x94. Also `__forceinline GetPDef()` and the private `CalculateSecuritySweepTasks()`. |
| `engine/CThing.h` | `FABLE_RETAIL_CTHING_VILLAGE` | `HasScriptThing() const` (0x004C7E70); inline `SetPersistOnUnloadWhenInLimbo`. |
| `engine/CThingBuilding.h` | `FABLE_RETAIL_CTHINGBUILDING` | `bool OverrideAutomaticLocking : 1` (Ego_r bool:1, same offset); inline `SetOverrideAutomaticLocking`; `LockAllDoors(bool,bool)` (0x0082DB00). No landed includer defines the macro. |
| `engine/CThingCreatureBase.h` | `FABLE_RETAIL_CTHINGCREATUREBASE_PROPERTY` | `GetProperty() const` (0x00662010) |
| `engine/CIntelligentPointer.h` | `FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR` | the out-of-line `CIntelligentPointer(T*)` constructor |
| `engine/CWorld.h` | `FABLE_RETAIL_CWORLD_GAMETIME` | `PeekGameTimeManager()`, `GetGameTimeManager()`, `PeekMyPlayer()` (0x0049D9D0) |
| `engine/CTCThingOwner.h` | `FABLE_RETAIL_CTCTHINGOWNER` | `StatGetInterfaceType` |
| `engine/CTCCreatureNavigation.h` | `FABLE_RETAIL_CTCCREATURENAVIGATION` | `StatGetInterfaceType` |
| `engine/CTCBase.h` | `FABLE_RETAIL_CTCBASE_GETWORLD` | `GetWorld()` (0x00686A70) |
| `engine/CThingSearchTools.h` | `FABLE_RETAIL_CTHINGSEARCHTOOLS_BEST` | the `GetPBestThing<Filter,Compare,T>` template declaration |
| `engine/C3DVector.h` | `FABLE_RETAIL_C3DVECTOR_NORMALISE` | `GFastOneOverSqrtTable`, `__forceinline __fastcall GFFastOneOverSqrt`, `Normalise`, `GetMagnitude2D`, `RotateXY` (0x00A14040) |
| `engine/CDefString.h` | `FABLE_RETAIL_CDEFSTRING_GETSTRING` | the implicit `operator CCharString()` over the out-of-line `GetString()` (0x00415DD0) |
| `engine/CVillageTaskPatrol.h`, `engine/CVillageTaskPickCrops.h`, `engine/CVillageTaskSecuritySweep.h` | `FABLE_RETAIL_<CLASS>_CTOR` | the retail constructor declarations (0x0065EE66 / 0x0065EEFF / 0x0065DCE8) |
| `fable_string.h` | `FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE` | `CBasicString<char>::operator==` is declaration-only; `CCharString::operator==` is forced inline. The overlay also carries identical copies of `fable_definition_table.h` and `fable_game.h`; see the gotchas. |

## Header debt
- **TU-local models without engine headers:** CVillageDef, CThingTrackNode, CGameTimeManager, CTCLight, CThingCompare_Nearest, HasGuardProperty, binary_compose / compose2.
- **CThingObject is a TU-local model.** Its virtuals 4 to 47 are positional placeholders (`VtableSlotNN`) so that `GetProperties()` falls at vtable slot 48. It needs a real header with the PDB virtual names.

## Names
- **NAME_FIXES.tsv**, 11 entries:
  - 0x004C7E70 HasScriptThing (was `GFHasScriptThing`)
  - 0x00662010 `CThingCreatureBase::GetProperty` (was GetLifeState / GetVillagerState / GetCurrentStateGroupType)
  - 0x006BB390 `CGameTimeManager::IsTimeBetween` (was IsTimeOfDayInRange in several classes)
  - 0x006BB2C0 `IsDay` QBE (QAE and IsDaytime variants)
  - 0x006604EB the STLport `list<CIntelligentPointer<CThingAICreature>>::push_back` (was CResultList / CAICreaturePointerList)
  - 0x004CC340 the `IS_THING_ALIVE<CIntelligentPointer<CThing>>` template spelling
  - 0x0049D9D0 `CWorld::PeekMyPlayer` (was GetCurrentPlayer / GetPlayerManager)
  - 0x006CB8E0 `CTCCreatureNavigation::GetNearestNavigablePosTo` (was CTCWalk::FindNearestWalkablePos)
  - 0x0065F744 the `GetPBestThing<..,CThingAICreature>` template spelling
  - 0x0066000A `std::find_if<..,HasGuardProperty>`; the landed row invents `CRangeString_0066000a::Construct`
  - 0x0065F152 `HasGuardProperty::operator()(CIntelligentPointer<CThingAICreature>)`; count_if 0x0065F6A6 binds the `<const CThing>` spelling
- **LABELS.tsv:** both rows, every relocation target of both builds, and the inline names.

## Gotchas (one line each, for docs/pipeline/GOTCHAS.md)
- VC7.1 packs address-taken locals by scope, not liveness: a function-scope `CThing* exit` passed as `&exit` never shares its stack slot. Put it in a block (and the iterator / predicate) to get retail's shared slots (0x00659800: frame 0x64 -> 0x50).
- One source `while (!f(rotated + pos))` loop: VC7.1 duplicates the test, both copies share one temporary slot, and the first copy-propagates the just-copied vector. Two source call sites give two slots.
- `if (a >= b)` gives `fcomp; test ah,1; jne` with the else block out of line; `if (a < b)` / `!(a < b)` gives `test ah,5; jp`. Match the comparison operator to the retail mask.
- Building a C3DVector local by assignment (`offset = a - b;`, or `= a; -= b;`) makes a temporary plus `movsd`; only copy-initialisation constructs it in place.
- A never-defined POD local (an empty functor such as `HasGuardProperty isGuard;`) is treated as dead: VC pushes it from whatever slot it likes, often a later variable's.
- Overriding a `#pragma once` repo header (`fable_string.h`) in the overlay also needs copies of every repo-root header that includes it (`fable_definition_table.h`, `fable_game.h`). Otherwise the repo copy is found from the including file's directory and you get C2011.
- The CDefPointer load / dec order in 0x005C6B26 / 0x005C90FC / 0x005CACB9 is not a header question: typed `T*`, const-reference reads and the PDB `Get` via `GFDebugPDynamicCast` all leave it unchanged.
