# Detailed current findings

## 0041e6d3 — NUISystem::CManager::Action

Original supplied session-4 baseline reproduced at 35,593 bytes / frame 0x800. The later uploaded session-5 v23 reproduced at 35,697 / frame 0x808. The selected source is based on v23, not a regression to the older, smaller body. It compiles to 35,702 / frame 0x800. Retail is 35,651 total = 34,619 code + 1,032 switch-table bytes. Full masked comparison still DIFFER, first byte difference 18.

Kept v23's reverse iterators, POP_MAP_STATE iterator/entry changes, split FindChild literal calls, scroll temporaries and thirteen named bool setter arguments. The same bool fixes had been independently explored here before v23 arrived; credit them once.

New retained work:

- `CActionParamSLONG* count` lets BUY_BULK/SELL_BULK push the original `[eax+8]` word. A const-long reference produced `add eax,8; push [eax]` instead and was rejected.
- NumberAugmentations is unsigned long; `<=0` restores the native unsigned guard. This is equivalent to zero equality for this type.
- Cache PInventoryTrade inside each successful FindChild branch, after that call, then reuse the receiver through GetNumber and Construct. This is done for four menu arms. Earlier shorter variants cached before FindChild and moved the native reload too early; they are not selected. The original console BUY lookup of `UI_LIST_SELL_ACTIONS` remains preserved.
- Shared CCountedPointer.h gains `FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR`. It exposes the existing real generic constructor body while guarded noinline keeps calls out of line. It does not use an artificial mod/ref stand-in, and the default branch is unchanged. The texture local now shares the native -0x268 slot rather than taking a separate -0x3c8 slot, restoring the 0x800 frame. LOAD_PHOTO now uses the native ebx-held 8 for the four CPixelFormat arguments. Exact constructor symbols and counts remain 25 CComponent, 17 CAction and 1 CTexture.
- PHOTO_CAPTION names the CWideString returned by GetText. Native re-takes its address with a six-byte lea before OnCaptionEntered; the old direct temporary passed eax instead. This correct local addition grows 35,696 to 35,702. Direct-call symbols and counts remain unchanged.

Residue: action/game registers still reversed at entry (native action EDI, game ESI; ours ESI/EDI), shared-tail survivors and block placement, some slot order including photoTexture alias/clan iterator, and two wide-string populated-data branches placed before rather than after the case bodies. Reversing header return arms only flipped the local diamond; early case guards compiled identically. Neither is selected.

Allocator trace explains the entry issue: the cached raw action pointer had priority 1015 / 106 refs, versus game 571 / 62. Introducing a raw pointer local worsened it to 1117 / 130. This is not a declaration-order tie. Trace output was accepted only when the diagnostic build's selected section was byte-identical to the independent ordinary RTM build.

## 00cbfb7d — RunCutsceneMacro

Supplied baseline reproduced at 74,965 / frame 0x18e4. Retail is 74,778 / frame 0x18dc. Selected is 74,906 / exact frame 0x18dc, still DIFFER, first byte difference 21 (entry register allocation). Full extent is compared.

New retained work:

- IS_THING_ALIVE retains the existing exact instantiated symbol `??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z` and its single out-of-line call. The visible body rejects null, then uses CThing::IsAlive. Native 0040dce0 independently rejects null and tests Dead at +0x91 bit 0; the shared header models that same member. This recovers `_pThing` slot sharing at +0x20 and reduces the frame by four bytes. The emitted helper itself is 20 bytes versus native's 21 and is NOT claimed byte-matched or proposed for landing. It supplies semantically faithful visibility for the assigned caller only.
- The remaining four bytes were padding from boolean slot grouping. flagWasSet was at -0x3f among nine bool bytes, while native stores it among the four-byte group -0x28..-0x25. Native sets the flag after IsFalse(params[1]), before either map assignment. Putting `flagWasSet=true` in both actual if/else arms restores this timing and removes the extra padding word. The selected flag home is -0x25 versus native -0x27, so within-group order remains a residue. Direct-call symbol/count multiset is unchanged. Size 74,955 -> 74,906.

Last alternate: `slide_factor_compound` names `(float)i`, copies start into position, then uses `position += step * fraction` in the two SlideTeleport loops. Ordinary compile completed at 74,882 / 74,778, still DIFFER; diagnostic loose hunks 286 versus selected 289. It had not received final local x87 review before the immediate wrap-up request, so it is preserved without promoting it. Its real source and headers are in HISTORY. It may be the next productive probe; do not equate fewer bytes with fidelity.

Residue: entry name/constant/def register roles; scalar and aggregate slot ordering; prefix conditional-cleanup shape near native 00cc0418; SlideTeleport x87 spills/operand order near 00cc5947 and 00cc5c02; CrowdKill top-tested wait loop near 00ccf7be; shared CUTSCENE_FRAME and destructor tails in different locations. Native SlideTeleport spills the scaled Y component while keeping X/Z. Naming only the offset or copy-then-scale canonicalized to selected bytes; a step reference changed a later site but did not solve it. Other shorter scalar/component alternatives are retained but unselected.

## Naming, headers and verification limitations

The uploaded sources already contain local class models and explicitly descriptive/unproven names. This continuation has not completed a two-source audit of every inherited declaration or migrated every existing local model. Do not report the package as passing all naming/header house rules. Source/layout evidence from prior authors remains in the prior handoffs and header comments. NAME_FIXES.tsv contains the four incoming Action corrections, not newly discovered fixes.

Each final selected source was compiled using ordinary cl 13.10.3077 RTM and checked against the complete expected extent with relocated operand bytes masked. Exactly one matching Leaf symbol at section offset zero was selected from each object; objdump symbol listings and COFF relocation metadata are included. A complete relocation target identity audit and behavioral verification are outstanding. No TEST PASS marker, row landing, commit or push is claimed.

Instruction similarity is a diagnostic with registers/addresses or slots normalized according to RESEARCH/local_tools/grade.py. It is not a byte-match percentage and is not directly comparable to Claude's earlier per-case percentages. Whole-function match count remains 0 of 2.
