# Hand chunk 308 (CThingSearchTools .. CThingTrackNode): 29 of 40 byte-exact

## What matched (29)
All are RELOCATION_MATCH; each has a test passing under g++ (C++03) and VC7.1.

**CThingSearchTools rough-area templates (28).** Every body has the same skeleton: GetMapsInArea into the scratch list at +0x0c,
bump the search stamps, loop over the maps -> GetCellExtents -> the levels -> y -> x -> the cell list, apply the stamp check,
the type test and the filter, then clear the scratch list with memmove. The shapes that made them match:
- Type test: the mask is copied into a local, `unsigned long mask = T::TypeMask; return (flags & mask) == mask;`. For CThing
  (mask 0) this keeps `xor dl,dl ; jne`. A byte mask gives `and edx,0Fh ; cmp dl,0Fh`.
- list<T*> collectors (0079bf30, 00785dd0, 0078db20, 007930b0, 007a5190, 007a63d0, 007c3c20, 0080e8e0, 00937080, 0091adb0):
  - push_back is `insert(end(), x)`.
  - The iterator has a user copy ctor (without it VC shrink-wraps `push edi`).
  - The node comes from `malloc` plus placement new, and insert caches `prev`.
- vector<T*> collectors (0074d710, 006b2720, 008ad710, 009595c0): the overflow is a same-TU `auto_inline(off)` stand-in,
  so the `__true_type` tag and the cell pointer take retail's argument slots.
- GetPBest (0079a050, 007f5d20, 00937690, 0093cdb0, 00942390, 009147e0): `best = compare(best, thing)`, then
  `*out = best; return best != 0`.
- GetPFirst (007512e0, 007fcef0, 007fd110, 0079a280, 0096f260, 00987430, 007fe730, 007313b0): on a hit it does
  `*out = thing; return true;` before the clear.
- CFilter_BindN holds references to its sub-filters. Inline filters (the OverlappingCircle body of the landed 0x0063EF50 in
  007512e0, and the ScaresBirds def test in 009147e0) are written out in the TU.
- Whether `maps` is a local pointer or `AreaMaps` is used directly decides the this/maps spill order. Which one is right
  depends on the row (the choice is recorded per row in gen/rows.py).
- The SIB recipe (from the coordinator/advisor, 007fcef0) closed 12 rows that were parked as a "family gap": 0074d710,
  006b2720, 008ad710, 009595c0, 007fcef0, 007fd110, 0079a280, 0096f260, 00987430, 007fe730, 007313b0, 009147e0. The recipe:
  - Declare `long y = box.Y0;` and guard with `if (y < box.Y1) { ... }`.
  - Inside the guard, declare `offset = level*sizeof(CMapwhoLevelInfo)` and then `cellsOffset = level*sizeof(CCellVector)`.
  - Index through integer sums: Width at `offset + LevelInfo + 4`, the row at `Cells + cellsOffset`.

**CThingSwitch::Save (0083bf90).** Appends the four level-script lines after CThingPhysical::Save. It matched on the first
grade. The name lines use an explicit `CCharString("TriggeredByThing ") + (name == "" ? CCharString("NULL") : name) + ";\r\n"`
(0x0099F570 is CCharString + CCharString). The conditional temporaries are tracked by the ebx flag bits. The inline
`CCharString::operator==` has the null-block arm `strcmp(text, "")`.

## Parked for Fable 5.1 (11)
- 0087a7c0 CThingSearchTools::FindThingsInRoughArea DIFFER(590v590): at map-loop exit retail loads `this` ([esp+38h]) before the iterator ([esp+24h]); ours loads the iterator first | tried: SIB recipe (fixed the SIB), maps local/direct flip, `it != maps->_M_finish` (worse) | next: the iterator as a named `CThingMap**` local declared before GetMapsInArea, or a `for` over a `CMapVector&` reference
- 00919780 CThingSearchTools::FindThingsInRoughArea DIFFER(590v590): the same exit-reload order, plus the thing spill (retail stores to [esp+38h] before the filter call, ours to [esp+34h] after) | tried: SIB recipe, maps flip, cond form, `push_back(static_cast<T*>(thing))` (moves the spill but to 34h) | next: a named `T* found` spilled before the HasTC test
- 0096bcb0 CThingSearchTools::FindThingsInRoughArea DIFFER(583v583): same as 00919780 (exit reload order plus the thing spill slot) | tried: same as 00919780 | next: same as 00919780
- 0085b2a0 CThingSearchTools::FindThingsInRoughArea DIFFER(775v775): the GetInterface map lookup `mov ecx,[ebx+ecx]` (ours `[ecx+ebx]`) plus two exit reload swaps (ebp/edx, esi) | tried: SIB recipe, `__forceinline` filter and Bind4 operator, maps flip | next: the SIB recipe idea applied to the LowerBound result inside the hit-locations filter (a user offset variable)
- 008780d0 CThingSearchTools::FindThingsInRoughArea DIFFER(680v680): retail reloads edx ([esp+68h]) before edi/esi at node-loop exit | tried: SIB recipe, box scalars, member assignment, filter-body variants, pointer Bind members, maps flip | next: lane gap: spill-reload order
- 00878430 CThingSearchTools::FindThingsInRoughArea DIFFER(673v673): retail reloads edx before edi/esi at node-loop exit | tried: as 008780d0 | next: lane gap: spill-reload order
- 006fe7c0 CThingSearchTools::FindThingsInRoughArea DIFFER(680v680): retail reloads edx before edi/esi at node-loop exit | tried: as 008780d0 | next: lane gap: spill-reload order
- 009871c0 CThingSearchTools::GetPFirstThingInRoughArea DIFFER(614v614): the ebx reload placement at node-loop exit (maps local); with maps direct the this/maps slots swap | tried: SIB recipe, inline IsCreatureType (0x00662000) with `__forceinline`, maps flip, cond form | next: lane gap: spill-reload order
- 0063f7c0 CThingSearchTools::ForEachInRoughArea DIFFER(785v785): retail reloads edx first at node-loop exit | tried: Bind3 with reference members, the inline CAABox in OverlappingBoundingBox, box scalars | next: lane gap: spill-reload order (the SIB recipe is not needed here)
- 00837220 CThingTrackNode::UpdateFacingDirection DIFFER(716v716): the cross product's z term is `fld dY; fmul sX` (ours `fld sX; fmul dY`), and `mov eax,[ecx]` (the vtable) is scheduled early | tried: 24 grades: a `next` local, GetLengthSquared written in Z,Y,X order, a named RHSet, ctor init orders, a `physics` local, CrossProduct by value / by out-parameter / swapped operands (canonicalised, no effect) | next: a CrossProduct written over `const float*` like the landed CTCTavernGameCoinGolf model, or `up` built in place inside the CRightHandedSet
- 0083c2d0 CThingSwitch::ConstructFromLevelScript DIFFER(760v756): the item CCharString lands in the dead argument slot and its handle is reloaded and re-tested for every else-if; retail keeps it at [esp+14h] with esi and threads the null tests | tried: shared single return, const item, const-ref binding, direct-init, ternary operator== | next: steer the slot assignment so the TriggeredByThing `name` gets the argument slot (e.g. declare it in the loop scope before `item`)

## Skipped
None. All 40 rows were attempted.

## Label corrections
The catalog spreads one label across byte-identical instances, and many of the template arguments are propagated too.
The instance arguments in each source's line 1 were read from that body's own type mask and callees.
- 006b2720: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (vector<T*>)
- 006fe7c0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 0074d710: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (vector<T*>)
- 00785dd0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 0078db20: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 007930b0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 0079bf30: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 007a5190: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 007a63d0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 007c3c20: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 0080e8e0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 0085b2a0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 008780d0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 00878430: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 0087a7c0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (vector<T*>)
- 008ad710: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (vector<T*>)
- 00919780: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (vector<T*>)
- 0091adb0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 00937080: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (list<T*>)
- 009595c0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (vector<T*>)
- 0096bcb0: catalog `GetPBestThingInRoughArea<...>` -> actually CThingSearchTools::FindThingsInRoughArea (vector<T*>)
- The real GetPBest rows are 0079a050, 007f5d20, 00937690, 0093cdb0, 00942390 and 009147e0 (`ret 10h`).
- 0x0049C700 is CThingSearchTools::GetCellExtents (a C2DBoxI by value), not CRegionMinimap::GetRegionMapExtents.
- 0x00662000 is CThingTypeQuery::GetCreatureType, not CPixelShader::CountReferences.
- 0x004AC610 / 0x00515CC0 / 0x00959260 / 0x00788FFC / 0x00436AC0 are vector<T*>::_M_insert_overflow instances.
- The PDB mangles CThingSwitch::ConstructFromLevelScript as `(const char*)`, protected (`?ConstructFromLevelScript@CThingSwitch@@MAE_NPBD@Z`).
