# agent719c: chunk 719 row 0x00509A20 `CWorldMap::GetMapNavigationAreaUpdateInfo` (3,120 bytes)

**Grade: DIFFER(3220v3120).** 754/937 instructions match exactly and 871/937 match once stack offsets are masked. There were 13 graded attempts; see LOG.tsv.
The row is parked in `differ/00509a20.cpp` (the same file is in `src/`). It needs the overlay `hdr/` and is graded with `--flags /Gr`. The test is `test/00509a20.test.cpp` (and a copy in `differ/`). It prints `00509a20_TEST PASS` under g++ and compiles cleanly under cl 13.10.3077. There is no payload.json, and oracle.tsv holds only its header, because no row matched.

## Residue by section
1. **Prologue.** Built copies `this` into ebx (`mov ebx,ecx; mov eax,[ebx+8]`). Retail loads World through ecx before the pushes, stores ecx to its slot and holds `area` in ebx. This is the same allocator decision as in section 6.
2. **Things list, Bind3 filter, GetAllThings, NavigationLayers.clear().** Match.
3. **Switchable branch.** VC7.1 inlines the implicit `CSwitchableBlockage` copy ctor in the `SwitchableBlockages.push_back` growth path; retail calls 0x005199E0.
   - The fast path uses `add edi,8` where retail has `lea esi,[edi+8]`.
   - The inlined copy adds one spill. This accounts for most of the +100 bytes and for frame 0x12c v 0x128.
   - Probes (probe/p*, q*, e*, t1) show the inline decision flips with loop nesting and with what the function contains around it:
     - 1 loop in a tiny function: retail shape.
     - 2 nested loops: both copies inlined.
     - The full function: both called.
   - None of these changed it: a user copy ctor or dtor, a faithful list allocator/insert typedef, extra code before or after, looped consumers, or a standalone _M_insert_overflow.
   - The non-switchable branch matches.
4. **ACTION_POINT dummies.** Match, except alignment padding.
5. **TRANSITION_POINT helpers** (CStringParser, `g_.. + "_"`). Match, except padding.
6. **Topology weights.**
   - Register allocation: built has ebx=this, esi=area, and map reloaded from the stack; retail has ebx=area, esi=map, and edi=this loaded late.
   - Index association: built computes `(w*j + data) + i`, retail `(w*j + i) + data`.
   - Retail hoists the area TLX/BRX loads above the IsPrefNavAt `je`. This happens because both arms allocate registers identically.
   - Built rotates the reload of the outer loop's TLX.
   - Attempts 10 and 11 tried two loop forms (resize outside the block; counters declared outside the `for` headers). Neither helped.
7. **Frame.** Slot order differs: this and lastName are swapped, and the helperNames/dummyNames group sits in a different place from the filter/actionPoint group.

## Levers that worked (3272 -> 3220 bytes, 725 -> 754 exact)
- A same-TU `__declspec(noinline)` body for `CThingSearchTools::GetAllThings` (under `#pragma auto_inline(off)`). It keeps the list sentinel in edi and drops the dead `_M_data=0` store, as retail does: -24 bytes.
- `CCharString::GetLength` written as `!PStringData ? 0 : len`, which gives retail's `jne`-over-zero-arm. Also the `const CThingSearchTools& searchTools` local.
- Topology code inside a `{}` block. The clip box then shares a slot with the blockage: frame 0x13c -> 0x12c, +21 exact instructions.
- Filter classes on `std::unary_function` bases (non-POD, so there is no value-init zero store) plus the `__fastcall GFFilter_Bind3` builder (PDB).
- Neutral: plain `bool switchable` is identical to `const bool&`.

## Header changes (overlay `hdr/`, built on round3/hdr; hdrcheck2: every CHANGED header default=same)
New headers: generated default branch plus a `FABLE_RETAIL_<CLASS>` typed branch. Defaults come from a scratch copy of gen_class_headers.py writing to gen/; nothing was written in the repo.
- `C2DLineF.h`, `CNavigationPosition.h`, `CNavigationTransitionPoint.h`.
- `CNavigationAreaUpdate.h`, with nested `CSwitchableBlockage` 0x10 and `CNavigationLayerUpdate` 0x18; sizeof 0x30.
- `CMap.h`: generated layout + `IsPassableAt` / `IsPrefNavAt const`.
- `CTCGraphicAppearance.h`: layout inside `namespace NGraphicAppearance`, + `GetAllDummyObjectNames` / `GetDummyObject const`.

New hand-written headers (PDB TPI):
- `CThingFilter_OverlappingArea.h`, `CThingFilter_HasNavigationInformation.h`, `CThingFilter_IsInQuestOrNullQuest.h`.
- `CFilter_Bind3.h`, with `GFFilter_Bind3` `__fastcall`.

Opt-in additions or edits to existing headers:
- `CWorldMap.h`: GetMapNavigationAreaUpdateInfo, GetNavigationLayerAt (0x0063E590), ClipAndConvertWorldMapAreaToLocalArea (0x004FC4D0).
- `CThingPhysical.h`: GetNoNavCollisionLineLists 0x006A4F10, GetNavCollisionLineList 0x006A4F60.
- `CPhysicsMeshObject.h`: GetAllHelperPointNames 0x00A11820, GetHelperPointPositionStatic 0x00A11760.
- `CStringParser.h`: FindKeyword, ReadAsInteger.
- `CThingSearchTools.h`: `GetAllThings` declaration.
- `C2DBoxI.h`: GetWidth, GetHeight, ContainsPoint.
- `C2DVector.h`: `C2DVector(const C3DVector&)`.
- `CThing.h`: `GetTCGraphicAppearance() const`.
- `fable_string.h`, under FABLE_RETAIL_CCHARSTRING:
  - `CBasicString::operator==(const T*)` 0x004115A0.
  - `CCharString::GetSubString` 0x0099EC70.
  - Inline `operator==(const char*)`.
  - `#pragma intrinsic(strcmp)`.
  - **GetLength respelled `!PStringData ? 0 : len`.** This changes the shared retail branch. The two landed includers still RELOCATION_MATCH with the overlay: 0x00522720 RuntimeLoadEntities and 0x00521620 SaveToGameStateBinary.
- `stlport/fable_containers.h`:
  - New opt-in `FABLE_STLP_BACK_INSERTER` (back_insert_iterator, back_inserter, `copy` -> `__copy_aux(__false_type)`).
  - Rebased on the repo's newer **D2** tag-ctor default (`__true_type() { }`). The bytes did not change (attempt 12).

## Header debt
- The local `GFGetMapThingUIDFromThingUID` inline. FableWin spells it `?GFGetMapThingUIDFromThingUID@@YI_K_K@Z`; there is no header for it.
- The `g_CCharString_013bac94` / `..9c` externs ("TRANSITION_POINT" / "ACTION_POINT").
- The same-TU GetAllThings stand-in body. Its loop shape is not retail's 0x00516200 body; it is there only for the effects VC reads.

## Gotchas (one line each)
- VC7.1 inlines an implicit copy ctor in a vector push_back growth path or not depending on loop nesting and function size. A tiny 1-loop function calls it; 0x00509A20 inlines it, and nothing local flips it back.
- Putting a trailing section in a `{}` block lets VC7.1 share frame slots with the earlier loop locals (0x00509A20: frame 0x13c -> 0x12c).
- An overlay copy of `fable_string.h` (which uses `#pragma once`) breaks any TU that also includes an un-overlaid top-level header, because `""` includes resolve the repo copy next to the includer and you get C2011 redefinitions. Regrade such TUs with every top-level `rebuild/include/*.h` copied into the overlay.
- `CCharString::GetLength` as `!p ? 0 : len` (not `p ? len : 0`) gives retail's `jne` over the zero arm and reuses the zero register.
