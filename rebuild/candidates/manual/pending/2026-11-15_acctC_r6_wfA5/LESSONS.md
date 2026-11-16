- (wfA5, 0x00B555A0) A render row that expands only SetRenderState blocks can match on the DEFAULT SetRenderState text: try the plain headers before the RenderClouds levers (SETRENDERSTATE_DESIRED / CAPTUREBLOCK_INLINE made it worse here).
- (wfA5, 0x00598A1C) Retail's `cmp ; je ; mov key,-1 ; jmp tail ; mov key,20 ; jmp tail` + the other arm laid out last = `if (!flag) { if / else } else { ... }` with three calls cross-jumped; a ternary key gives neg/sbb/and/add.
- (wfA5, 0x00598A1C) A pointer named before a second FindChild sat in ebx (retail: frame word, vtable in ebx); naming the second call's result too (`T* list = ...; list->F(accept);`) gave retail's allocation.
- (wfA5) The EARLY_RETURN variant of CRenderStateManager::SetRenderState spins cl (unbalanced #if in that branch?): do not define FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN.
- (wfA5, 0x00AA0FA0 / 0x00AA2110) x87 operand orders keyed on the IL of an UNROLLED loop body: a helper text expanded in that loop (SetPosition) flips them where 1..48 dead locals do nothing; twins can then need different texts of one helper -- sweep helper-text combos on both rows together.
- (wfA5, 0x00AA2110) A loop-local __declspec(align(16)) matrix let VC7.1 schedule its stores early; the same matrix at function scope gave retail.
- (wfA5 successor, 0x00AA2110) A twin conflict on x87 operand orders (one helper text flips both twins) closed with a row-local spelling in a DIFFERENT section (the matrix concatenation: `ToMatrix(matrix_a)` v `ToMatrix(*current)`); sweep every section's equivalent spellings (pointer v object, template v manual swap), not only the section with the hunks. Named locals never moved these keys; extra expression copies (`keyframes[j - 1]` twice) did.
- (wfA5 successor, 0x00B9AC30) A long render row went 222 -> 19 hunks by STRUCTURE first: duplicated assignment arms instead of a flag (retail cross-jumps the common tail), `++n, ++j` order in a two-IV copy loop, a `count - done` derived remaining count (retail keeps -batch in a slot), and a member-wise C2DBoxF local copy (one int-copied field = the missing 4 frame bytes).
- (wfA5 successor, 0x00B9AC30) FableWin inline accessors (C3DPrimitive2::GetVertexCount / GetTriangleCount / GetIndexBuffer ...) instead of direct members moved argument evaluation late (imul after the inlined calls) and fixed later register picks: prefer the PDB accessors a sibling row already opted into.
- (wfA5 successor) Check struct field names in struct_layouts_egor.tsv before accepting a header's "descriptive" names (CClipListElement had Ego_r Start / Size).
- (wfA5 b10, 0x00B9AC30) "19 hunks" counted frame-MASKED was really 93 raw: always read the raw fg count too. A whole section of permuted esp slots had ONE cause found in 2 probes: c2 packer probe (pk.py + pkslots.py) + a what-if re-sort of the slot weights (tools/pk/whatif.py, search.py) named the node that joined the wrong slot (a scoped-object `layout` sharing clip_end's slot); retail = the address-taken pointer declared at function scope (exposed objects conflict by SCOPE, not liveness).
- (wfA5 b10) Moving an address-taken local to function scope turned its block-scope initialisation into an assignment and made c2 reload an unrelated member before the calls (alias kill); a named local for that member restored the single load.
- (wfA5 b10) An inline helper whose FableWin /Od size is 40 instructions off (GetTexture 192 v 149) is a different TEXT: model it on the sibling helper of the same class with a FableWin-verified text (GetFrame: TouchFrame + if/else + IsLoading) and check /Od /EHsc size (149/471 v 149/470) AND the out-of-line copy (still RELOCATION_MATCH); it fixed register picks in the expansion.
- (wfA5 b10) Two operand-order bits immune to every count lever: 32 named-v-inline spelling combos, 1..12 initialised unused locals, SetRenderState DESIRED/BOOKMARK/DIRTY, dead asserts 0..140 (top/tail/inside a helper) all byte-identical or moving other sites only. Comparator 0x1076A394 compares the full 32-bit key; the copy-loop bound's Size tree carries top byte 0x01 (ADD node) v 0x00 for Start, so no hash shift can put Start first -- the retail tree for Start must itself contain a binary node (open).
- (wfA5 b11, 0x00B9AC30) A loop-bound re-read whose two loads come out swapped (Size before Start) under every reference / index / pointer
  spelling can be a NAMED pointer IV (`const T* clip = list; for (c...; ++c, ++clip)`): with the IV as a source symbol the operand order is id-keyed
  instead of structural. If its init then lands before the loop's count test, a redundant `if (count > 0)` guard (device a) moves it after.
- (wfA5 b11) SetRenderState bookmark test/or order per expansion = c2 temp ids of the two test operands (flags temp v mask temp, higher id first;
  the two series grow at different rates per site, so the flip point drifts). Dead asserts, unused locals and per-site text asserts did NOT move it
  here; removing a CSE before the sites did (`const unsigned long render_flags = PeekRenderFlags()` for two flag tests fixed one site of two).
- (tools) fg.py's norm() turns ours' literal `0` into REL whenever the bytes contain 00000000: `mov [x], 0` lines show as false hunks; count
  real hunks with sw11.sh / sd2.py.
- (0x00B9AC30) VC7.1 bookmark test/or operand order in inline SetRenderState expansions: c2 sorts the two temps by key = (2*record index)
  mod 2048, i.e. by record index MOD 1024; with f = flags temp (late phase) and m = mask temp (phase-A, program order), flags goes first
  iff f - m > 1024 here. Dead asserts / unused locals shift f and m TOGETHER (only wrap effects); live inline member expansions with
  field stores through a global pointer placed AFTER a site raise f only (that is the differential lever); extra CSE-able reads
  between the first site and the target lower f. Trace with tools/lk/lka.py + tools/lk/pairs.py (5 min, full log on a quiet box).
- (0x00B9AC30) Probing same-TU callee bodies: an EMPTY visible body is auto-inlined (and even with noinline it tells c2 the callee
  writes nothing, so globals stop being reloaded); use an opaque body that calls an extern with the arguments to isolate capture effects.
- (wfA5 pass 6, 0x00B9AC30) The c2 "late" test/or temp (flags temp f) moves with codegen-surviving IL anywhere in the function (3 live RMWs after
  the sites: f +14 at every site), NOT with IL c2 simplifies early: merged RMW chains, duplicate dead stores, dead loads, dead local stores and
  empty-class locals all add 0. FableWin-evidenced helper texts (fwsize SAME) moved it by +1 each at most. Measure f with a 5-min trace
  (tools/lk/sites.py) or a ruler of live RMWs (+5 each) before sweeping spellings.
- (wfA5 pass 6) fwsize on the whole row TU lists every expanded helper whose debug size differs from FableWin: check the cold-path helpers too
  (CIVCountedPointer Release/Reset of the CFrameData pointer, TouchTexture, GetCurrentFrame, PeekTextureBank, SetEnableFadedShadowFactor had
  FableWin-exact texts already in the repo as opt-ins or one line away).
- (wfA5 b14, 0x00B9AC30) Before ~1,100 grades of record-count work on a bookmark test/or site, copy the matched same-family twin's idiom for the
  block around the site VERBATIM (here the stipple block of 0x00BA2350: named float locals left/top/right/bottom read from the window copy):
  four named float locals moved the late-temp key the residue needed where unused locals, dead asserts, flags and helper texts could not.
