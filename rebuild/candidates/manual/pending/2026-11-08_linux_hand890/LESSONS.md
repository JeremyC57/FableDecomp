# Chunk 890 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Inline decisions follow c2's static call-site frequency (0x1078D1AA: score (X/f + 2X)/3 < T): after a `for (i = 0; i < n; ++i)` loop the frequency of the following code stays at the loop body's (each loop doubles the frequency of everything after it); `unsigned long i = 0; while (i < n) { ...; ++i; }` returns to the loop entry's. Two size loops before the corner code of 0x00551EA0 put it at f 4 (CCountedPointer(T*) expanded at all four sites, a tree find inlined); one while loop gives retail's f 2 (work/micro/f1.cpp, work/inl/*.json).
- A `?:` call argument with a class temporary in one arm (`sep != end ? sep->second : CCountedPointer<C>()`) adds a conditional-temporary flag slot (`or $T,2` / test / dtor, visible in the /FAs frame table) and grows the frame; retail's two calls in if/else arms (cross-jumped where the arms agree) is the source form (0x00551EA0, frame 0x68 -> 0x58).
- Declaration order of zero-initialised locals: the class-typed C2DVector locals declared before the unsigned longs keep their float zero stores as immediates; the integer zero stores follow declaration order and in `a * b` the later-declared operand becomes the imul destination (0x00551EA0 `imul ebp,[nbVertical]`).
- `x += a * b` (unsigned long product added to a float member) keeps the converted product on the x87 stack (`fst` + `fadd`) and reuses it as a CSE for a later `-= a * b`; a named `float total = a * b` local is stored and reloaded (fstp/fld) instead (0x00551EA0).
- A visible map::operator[] body (FABLE_RETAIL_STLP_MAP), even when the call stays out of line, lets VC7.1 schedule the key temporary's store after the argument setup as retail does; with only the declaration the store comes first (0x00551EA0 `GetStates()[0]`).
- `p.X = base.X + h` (copy source + converted int) loads base.X; `p.X += h` after `p = base` loads the copy -- the x87 load source is the spelling, not the order of operands (0x00551EA0 0x005527A8).
- Before trusting a catalog name, check the landed callers' reloc bindings and the PDB layouts: 0x0056AD40 "CQuestManager::UpdateUI_QuestMenu" is CTCBoastUI::Activate (the quest manager is a global read inside a `this` with CTCBoastUI's offsets).

Review at landing:
- 0x00551EA0 needed 12 grades (more than 4); its header opt-ins: engine/NUISystem_CTable.h FABLE_RETAIL_CTABLE_CONSTRUCT and stlport/fable_map.h FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (hdr/README.md).
- Landed siblings 0x005518E0 ConstructHorizontalLine / 0x00551BC0 ConstructVerticalLine use local base models; they can move onto the CTable opt-in.
- Landed 0x00553520 (placeholder CUnprovenKey find) and the landed caller 0x00499880's DrawProgress spelling: NAME_FIXES.tsv.
- Landed CTCBoastUI rows (0x0056A880..0x0056BE00) use local stand-ins (CComponent with 0x40 dummy virtuals, local CBoast) next to engine/CTCBoastUI.h / CQuestManager.h.
- (chunk 890 s2) Before any grade of a big row, compare the call MULTISET (objdump -dr relocs v the annotated retail listing): in
  0x00498490 it was identical except two out-of-line EndsWith calls, which pointed straight at the one inline decision to fix.
- (chunk 890 s2) A header helper whose c2 inline cost is far above a sibling the row must keep out of line (EndsWith 405 v
  GetVStringExtents 126) cannot be reached with /d2inlT: FABLE_FORCEINLINE on the helper (no copy in Ego_r.pdb) is the device.
- (chunk 890 s2) An inline bool helper's block layout follows its return shape: `if (x < 0) return false; return f() == 0;` puts the
  false store first (jns), `x >= 0 && f() == 0` makes a branchy bool, `if (x >= 0) return f() == 0; return false;` gives retail's
  `js` to a false store at the end with `sete` duplicated into each strcmp exit.
- (chunk 890 s2) Retail evaluates a nested `PeekGuiDef()` argument call before loading the singleton for the bank argument only when
  it is a separate statement (`const CPlayerGuiDef& def = PeekGuiDef();`); in the argument list VC7.1 loads the bank first.
- (chunk 890 s2) `C2DBoxF box(...)` and `C2DBoxF box = C2DBoxF(...)` build in place; only `C2DBoxF box; box = C2DBoxF(...)` gives the
  temporary + dword copy. A box argument built before the colour (zeros stored first) is a named local, not an argument temporary.
- (chunk 890 s2) c2 frame packer (c2.dll 13.10.3077): items sorted by size ascending then weight (refcount) descending; each joins
  the MOST RECENTLY created slot whose size >= half the item and whose members do not interfere, else opens a new slot; slots are
  laid out in creation order. agent887's pk.sh / ord.sh (copied to work/pk/) give the order; the symbol number is word 10 of the
  sym record (work/s2/e10.order maps every item to retail's slot through work/s2/slotmap.py).
- (chunk 890 s3) c2's frame packer treats an address-taken local as live over its whole enclosing block: an inner block around a
  short-lived out-parameter (formattedWindow) is the lever that moves it to another slot; a named local live across a block keeps
  a later item out of that slot.
- (chunk 890 s3) A C2DVector temporary in a box ctor is ONE 8-byte packer item; the four-float C2DBoxF ctor makes four 4-byte items
  that pack like retail's (the slot rule is size <= 2 x slot size, no interference, most recent slot first).
- (chunk 890 s3) In the overlay build a repo header that includes "fable_string.h" from rebuild/include itself (fable_definition_table.h
  via CDefString) pulls the repo copy next to hdr/fable_string.h (C2011): mirror that header unchanged into hdr/.
- (chunk 890 s3) VC7.1 inline decisions are per site: score = cost after savings scaled by c2's block-frequency guess, cut-off T=74
  (work/inl/inl.py logs sz / c1 / f / sc / r). When retail calls sites whose scores are below sites it expands, the difference is
  in retail's scores (control-flow frequency or callee cost), not the cut-off; /d2inlT<N> is not that T.
- (chunk 890 s3) Retail Fable.exe calls some COMDATs through ILT jump thunks (0x00429C53 `jmp 0x004291DE` = ~CCountedPointer<CComponent>),
  so a call target that differs from the bound body address can still be the same function.
- (chunk 890 s4) x87 operand order of `a + b` with two direct frame operands follows how each value was defined: the operand stored
  more recently loads first; an inline-ctor parameter temporary outranks a field-assigned member; a C2DVector built by its ctor from
  named floats loads first, and stays first when copied with the copy ctor and then modified (`V q(p); q.X += d;`), but a member-wise
  `V q(p.X + d, p.Y)` flips it (0x00498490 0x00498A7B / 0x00498B73 / 0x00498C88 / 0x00498DA8).
- (chunk 890 s4) CCharString::GetAsCharArray in early-return form (`if (PStringData==0) return &NullChar; return ...;`, opt-in
  FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN) gives retail's `lea esi,[ecx+eax]` (text base, offset index) in an inlined
  EndsWith at every site; the select spelling fixes only the first site (0x00498490 0x0049941A / 0x004994ED).
- (chunk 890 s4) VC7.1's instruction order is set by the list scheduler 0x1078734C (after the FP peepholes, before emission): 2 slots
  per cycle, one per unit (int / x87), x87 latency 3, int 1; a node made ready by a pick joins the SAME cycle, so `call ; fild` co-issue
  but `fstp ; call` puts the call in slot 1 and pushes the next x87 op to the next cycle. `fop ; lea ; push ; FOP ; mov ecx ; fstp ;
  call` (retail 0x00499085) v `... mov ecx ; FOP ...` depends on whether the preceding call sat in slot 0 or 1 (probe t13.py).
- (chunk 890 s4) Big UI rows where retail calls every ~CCountedPointer / STL COMDAT but expands the bitset tests: no /d2inl flag
  separates them; `#pragma inline_depth(0)` ... `()` around each called statement and before each scope's closing brace does
  (0x0056AD40: -740 bytes, calls 213 -> 226 / 238). With FABLE_RETAIL_STLP_MAP a depth-0 map declaration calls the no-argument
  map ctor as retail does; without it the allocator temporaries are built at the call site.
