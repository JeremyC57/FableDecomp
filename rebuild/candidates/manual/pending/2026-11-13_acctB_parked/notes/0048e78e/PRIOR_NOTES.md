# Hand chunk 2056 (CSubtitleRenderer .. CSubtitleRenderer): 0 of 1 byte-exact

## What matched (0)
Nothing landed-ready. The one row is handed over in place (best source `differ/0048e78e.cpp`, no src/ and no test/ yet).

## Parked for Fable 5.1 (1)
- 0048e78e CSubtitleRenderer::DrawButtonGraphics DIFFER(2656v2662): (1) 0x0048E861..0x0048E887 retail `mov ecx,[eax+4] ; fld [eax+4] ; mov eax,ecx ; mov [ebp+54h],eax ; mov [ebp+64h],ecx` then `fld [ebp+64h] ; fadd [eax]` (only button1_pos.Y forwarded on the x87 stack, button2_pos.Y / button3_pos.Y copied as integers, the second a copy of the first) v ours `fld [eax+4] ; fld [eax+4] ; mov eax,[eax+4] ; mov [ebp+54h],eax` (two forwarded); (2) 0x0048E9C6 retail `mov ecx,[esi+7Ch] ; test ch,8` at the top of the box block, `push 45h ; pop edx`, `mov al,[esi+0DEh]`, `test cl,4 / 48h / 2` v ours flags in eax (loaded at 0x0048EA07), 0x45 in ecx, InCutscene in cl; (3) 0x0048EA97 retail `lea eax,[ebx+5FCh] ; fadd [eax] ; lea ecx,[ebx+600h]` v ours `fadd [ebx+5FCh] ; lea eax,[ebx+600h]` (X offset address folded into both fadds), then the argument / fldz scheduling of arm A (0x0048EAB5..0x0048EBAB) and of the override tail (0x0048F0B7 `fld [eax] ; fadd [edi]` v ours `fld [edi] ; fadd [eax]`, 0x0048F182..0x0048F1AF movsd / fldz order) | tried: 71 grades -- reference select for the Update argument, if / else Open arms, IsOpen out of line, forced Clear / Reset / Release cascade, repeated `(pulse + 1.0f) * graphic_width` extents, flag-based arms, all six position declaration orders, copy-ctor chains (p3(p2), p1(p2), full chains), struct assignment chains, `p1(0, pos.Y + off)`, layer as ternary / if-else / before the boxes, IsInCutscene() and IsFlagSet() accessors, by-reference flag setters, width as ternary of calls / conversion inside the arms, statement orders and float references in arm A, a do-while(false) / constant-if / bare block around button3_box | next: (2) is ONE c2 candidate: the flags temp has priority 29 v InCutscene 26 because nine integer candidates are referenced in the block from the width join to the 0x800 branch (timer address temp, graphic_max_width, flags, drawn, button1_drawn, layer, const 0x45, this, const 0); with eight (cand/c47.cpp, cand/c67.cpp) retail's eax / ecx / edx assignment and the whole arm dispatch appear. The only one of the nine whose absence leaves the bytes unchanged is the `this+0x104` address temp (`lea ecx,[esi+104h]` at 0x0048E9B2 is the same instruction as a fresh, single-use address), so look for the spelling that stops c2 sharing the ButtonPulseTimer address between the top section and GetFadeInOutAlpha. For (1) retail's IL is button2_pos.Y = pos.Y, button3_pos.Y = button2_pos.Y with neither on the x87 stack; for (3) trace when c2 frees the first address temp of the first offset set (it exists at the reference walk and is gone before the x87 pass).

## Names (exact PDB spellings, two sources each)
- 0x0048E78E `?DrawButtonGraphics@CSubtitleRenderer@@AAE_NABVCEnginePrimitivePrepareInfo@@AAV?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@ABVC2DVector@@_N@Z`
  (Ego_r.pdb + FableWin.pdb; private, `ret 10h`). Callees and globals: see NAME_FIXES.tsv (eleven spellings, each with its Ego_r / FableWin
  evidence and the wrong landed / reloc_bindings spelling seen) and the row's own declarations (GFVXToAX 0x009E1E10, NPlayerGui::GFAnchorToTopRight
  0x00643CA1, GPDisplayManager 0x013B8390).
- Members: Ego_r.pdb names through the shared headers (ButtonPulseTimer +0x104, Button1Text .. Button1OverrideText +0x64..+0x70, Button1Alpha /
  Button2Alpha +0x78 / +0x79, DisplayFlags +0x7C, InCutscene +0xDE, CutsceneActionMode +0xE2, Button1TextOverridden +0xE3,
  Button2TextOverridden +0xE4; CPlayerGuiDef TextBox* members by the scout's FABLE_RETAIL_CPLAYERGUIDEF_TEXTBOX block).
- Descriptive (no PDB carries parameter or local names): parameters `info`, `primitive_handles`, `pos`, `in_text_box`; locals `set`, `def`,
  `font`, `button1_pos` / `button2_pos` / `button3_pos`, `offset_y`, `button1_width` .. `button3_width`, `graphic_max_width`, `graphic_width`,
  `pulse`, `half`, `button1_box` .. `button3_box`, `layer`, `drawn`, `button1_drawn`, `huge_pos`, `mouse_sprite`, `left_sprite`, `sprite*`,
  `right`; the row macro `BUTTON_SPRITE`. The DisplayFlags bits stay numeric (FableWin names the type ESubtitleFlag and the inline
  `IsFlagSet`, but no enumerator survives anywhere).

## FITTED v PROVEN (differ/0048e78e.cpp)
- PROVEN by retail bytes + PDB: `#pragma optimize("s", on)` alone (biased frame `lea ebp,[esp-68h]`); out-of-line IsOpen 0x0048DA30 (called at
  0x0048E81F and at all ten xref sites); the expanded Clear cascade at 0x0048EE4B..0x0048EE84; the arm structure and the two byte flags
  ([ebp+7Fh] returned, [ebp+7Bh] set only in arm E and tested at 0x0048F050); C2DBoxF::Move 0x0041A810 is a move-TO (not used here).
- FITTED (matches the bytes, original text unknown): the reference select for the Update argument and for `offset_y`; if / else with two
  Open calls and two GetGraphicMaxWidth calls into `unsigned long graphic_max_width`; the repeated `(pulse + 1.0f) * graphic_width`
  extents; four `+=` statements per offset in arm A; declaring layer / drawn / button1_drawn after button2_box; the bare block that
  ends button3_box's lifetime before the override tail (this alone moved the frame from 0x174 to retail's 0x16C: the tail's C2DBoxF
  temporary then shares button3_box's slot [ebp+10h..1Ch], 704 -> 821 equal instructions); `button2_box = C2DBoxF(..)` in the tail;
  FABLE_FORCEINLINE on CEnginePrimitiveHandle::Clear and CIVCountedPointer Reset / Release (no `// Inline-budget:` line measured yet:
  needed before this source can land).

## Header overlay (hdr/, all opt-in, default text of every repo header unchanged -> no landed row needs a regrade)
- engine/CFadeInFadeOutBase.h: FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPEN_OUTOFLINE (inside the scout's _FADE block): `bool IsOpen() const;`.
- engine/CEnginePrimitiveHandle.h: FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR (same Clear body, forced).
- engine/CIVCountedPointer.h: FABLE_RETAIL_CIVCOUNTEDPOINTER_FORCEINLINE_RELEASE (same Reset / Release bodies, forced; adds fable_compiler.h).
- Scout blocks kept as found: CSubtitleRenderer.h _BUTTONGRAPHICS, CPlayerGuiDef.h _TEXTBOX, CFadeInFadeOutBase.h _FADE and the others listed
  in scout/headers.md. Details in hdr/README.md.

## Skipped
None.

## Label corrections
- 0x0048E062 is CSubtitleRenderer::GetTextWidthFromText (landed as global_operatornew_0048e062.cpp); 0x0048FAA0 is DrawTextBox; the other
  respellings are in NAME_FIXES.tsv.
