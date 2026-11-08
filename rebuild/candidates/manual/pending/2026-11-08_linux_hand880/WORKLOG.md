# WORKLOG chunk 880 -- 0x0058AF10 CTCInventory::OpenPCOptionsMenu (13,048 bytes)

## Decoded phases (retail order)
search loop (Components, GetName()=="PC_OPTIONS_LIST") -> creation block { CreateComponent, frame children (+viewport
fake children), quit pictures, in-game menu OpenInGameMenuPC, gameplay/audio/video/redefine page actions (9 CAction each),
23 slider-page pushes, 23 slider name pairs, widget pairs, arrow actions, resolution text slider, anti-aliasing text slider
(switch 0..16), Initialise, Components.push_back } -> tail: scroll observers, detail sliders (CSlider Min==Max),
text sliders (MaxChild==0), refresh-rate " Hz" suffix, ChangeState(5).

## Grade history (see ATTEMPTS.log)
- v01/v02: compiles, 13147v13048; call sequence already 849/849 identical.
- v04/v05: list not address-taken in retail (c2 keeps it in edi with home [ebp+10h]).
- v08: DROP FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE: the inline push_back body VC declines (budget) makes the
  `const T&` argument a coalesced temp; frame becomes 0xE4 like retail. 13051v13048.
- v09-v11: named CRGBColour local, named CRedefinerList local, widget loop (entry iterator first, `++page, ++entry`,
  nested null tests), `if (!p.IsNull()) { CText* t = ...` in the two text-slider loops, switch `case NONE` separate.
  Normalised diff (ignoring frame offsets) now 45 lines.

## Next
- pictures loop `mov eax,[ebx+8] ; mov edx,[esi] ; push eax` (retail) vs `push [ebx+8]`.
- gameplay page: retail caches the ULONG vtable in edi (0x58b912), ours caches 0xF9.
- frame-slot permutation (list 0x10 vs 0x8, pictures 0x3c vs 0x38, etc.).
- v12: page decl first -> load order wrong; v13: entry first + `++entry, ++page` fixes the iterator slots (NDIFF 45 = only pictures push + edi const)
- v14: picture loop with a named `CComponent* picture = *it;` -> RELOCATION_MATCH
- v15: without FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ the GetName()=="PC_OPTIONS_LIST" compare is called out of line
  (13019v13048); /d2inlS10 13153, /d2inlS12 13514, /d2inlS15 13520 (over-inline everything), /d2inlT200 = default.
  So the existing shared-header __forceinline opt-in is the inline-budget device (step 3), recorded on line 2.
- v16: v14 minus the unused CList include -> RELOCATION_MATCH (best).
- FINAL: src/0058af10.cpp RELOCATION_MATCH (gall + vland RELOCATION_MATCH PASS), test PASS g++ / VC. Done.
