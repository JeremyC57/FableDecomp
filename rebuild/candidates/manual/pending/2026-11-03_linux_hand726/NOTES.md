# Chunk 726 notes (NUISystem CList / CScrollingViewport / CKeyRedefiner)

Regrade: `FABLE_HDR_OVERLAY=$C/hdr LANDVERIFY_WORK=$C/lv vland.py payload.json $S/batch/chunk726_oracle.tsv` gives 7 wins:
all RELOCATION_MATCH, and every behaviour test passes (log in `vland.log`).

## Rows
| addr | row | result |
|---|---|---|
| 0053e950 | ~CScrollingViewport | RELOCATION_MATCH |
| 0053fc10 | ??_E thunk this-4 (`W3`) | RELOCATION_MATCH (same TU as the dtor, as in the CWorldMap 0x0051CDF0 precedent) |
| 0053fc20 | ??_E thunk this-0x18 (`WBI@`) | RELOCATION_MATCH |
| 00558660 | ~CKeyRedefiner (the catalog said ~CScrollingViewport) | RELOCATION_MATCH |
| 005385ae | CList::ScrollUp(KK_N00) | RELOCATION_MATCH |
| 005392fc | CList::ScrollDown(KK_N00) | RELOCATION_MATCH |
| 0053a0a1 | CList::ChangeState | RELOCATION_MATCH |
| 005379f5 | CList::ComputeChildrenVisibility | DIFFER(2126v2102): differ/005379f5.cpp |
| 0053f1c0 | CScrollingViewport::Update | DIFFER(1601v1601): differ/0053f1c0.cpp |

The main lever was the STLport `map::operator[]` body. Once it is visible (fable_map.h, FABLE_RETAIL_STLP_MAP branch),
VC7.1 knows the key temporary is only read:
- It moves each key store down to just before its call (ScrollUp and ScrollDown, from 209 and 207 diff lines to a match).
- It hoists the eighteen loop-invariant key stores out of ChangeState's state-copy loop.

Before that change the residue looked like plain scheduling. It was not.

ChangeState also needed three fixes:
- The middle index written as one conditional expression. This gives retail's global register roles: edi holds zero
  from the switch prologue and ebx holds the constant one.
- `bool recompute = RecomputeOffsets; if (RecomputeOffsets)`. This gives the flag spilled to `[ebp+0Bh]` and its second test.
- `size() > 0` guards, and no separate guard on the IsScrolling loop.

### Parked residues
- **0x005379F5.** Retail calls LoadAndReturnDef() eight times right to left. It saves only the def pointers. Then it reads
  the ints (`fild [eax+14Ch]`) and converts TLX, TLY, BRX, BRY left to right straight into the box. This pattern is what
  a callee taking `const long&` arguments and inlined after all its arguments are bound would produce.
  - Hypothesis: the source passed references to the ViewArea ints into an inline helper. No PDB function of that shape
    was found, so it was not written (no invented helpers).
  - 18 compiled shapes (7 graded, 11 object probes) all build the box components in argument order instead.
- **0x0053F1C0.** One x87 choice. Retail pops `ratio` and multiplies the window height left on the stack by the memory
  copy (`fstp [ratio] ; fmul [ratio]`). Ours keeps both values (`fst ; fmul st(1)`, then `fstp st(0)` later).
  - 26 graded spellings tried: named or unnamed height, operand order, the C2DVector ctor, `*=`, and scopes.
  - The `map::operator[]` body makes this row worse (3 to 11 masked lines), so its source does not define FABLE_RETAIL_STLP_MAP.

## Overlay (hdr/)
All changed headers print `default=same` in hdrcheck2. `stubinc` was cleaned with git clean.
- `stlport/fable_map.h`: the FABLE_RETAIL_STLP_MAP branch gains the in-class `operator[]` body (STLport 4.6).
  0x00E0A820, the only landed user of that macro, stays RELOCATION_MATCH.
- `stlport/fable_containers.h`: a new FABLE_RETAIL_STLP_LIST branch in `list` with `size()` (the inlined `__distance`
  loop), `front()` and `push_front()` (STLport 4.6). It has no landed users.
- `engine/NUISystem_CList.h`: the FABLE_RETAIL_CLIST branch now also declares
  `public: static std::list<CList*> GPreviousSelected; // 0x013B8A98`.
  - Evidence for the name: the Ego_r dynamic initializer, the FableWin GetPreviousSelected accessor and the chunk 717 label.
  - Evidence for public access: unproven; the landed 0x005E41E0 clears the list from outside the class.
  - This branch also holds the earlier vector<bool> ScrollingChildrenVisibility and the 0x1FC size assert.
- `engine/NUISystem_CManager.h`: the retail branch declares `C2DVector GetUIScale() const`.
- `engine/C2DBoxF.h`: the retail branch adds `ContainsPoint(const class C2DVector&) const` and the inline GetTLX..GetHeight
  accessors. The probe blocks are removed. The elaborated `class C2DVector` avoids a second ifdef block.
- New headers: `NUISystem_CViewport.h` (0x174), `NUISystem_CScrollingViewport.h` (0x19C), `NUISystem_CScrollBar.h`
  (CDraggable 0x1AC, CScrollBar 0x1DC) and `NUISystem_CKeyRedefiner.h` (0x1C0).
- Siblings (SIBLINGS.tsv): 006286f0, 00487570, 00626e40 and 00e0a820 are all still RELOCATION_MATCH under the final overlay.

## Names
NAME_FIXES.tsv lists every landed or catalog spelling these rows prove wrong. Also note:
- vland warns that the thunks jump to `??_E...UAEPAXI@Z` at 0x0053FC30, which is labelled ``scalar deleting destructor`` (??_G).
  In VC7.1 `??_E` is an alias of the `??_G` body when the class has no array delete. Both names live at one address by
  construction, so this is not a fix.
- 0x013B8AC8 has two landed spellings: `g_pSelectedKeyRedefiner` (7 files) and `CKeyRedefiner::s_pSelected` (2 files).
  - FableWin has `?IsRedefinerActive@CKeyRedefiner@NUISystem@@SI_NXZ` but no data symbol. The real name is unproven.
  - This chunk uses the majority spelling. Not filed as a fix.
- 0x013B8AAC (g_pActiveScrollingViewport) is likely the static `CScrollingViewport::PViewportListeningToMouse`
  (the PDB has Get/SetPViewportListeningToMouse). The established spelling is kept; the address is unproven.
