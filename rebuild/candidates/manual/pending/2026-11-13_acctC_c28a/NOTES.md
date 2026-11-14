# agentC28 NOTES (acctC lane, w22) -- continues agentC26 (prev_C26/)

## Row 0x0061E6D0 ConstructQuestListPC -- session log
- Baseline probe/a/b0.cpp = prev_C26/differ/0061e6d0.cpp: DIFFER(12356v12356), 236 hunks / 214 real (tools/g.sh). NOT a pure
  frame-order residue: the first real hunk is a REGISTER decision at 0x0061ECC4 (j-loop body head):
  retail `xor edi,edi` (quest_card = 0 kept in edi until 0x0061EEDE, home store [esp+14h]) + `xor ebx,ebx` (const-0 piece in ebx
  0x0061ECC9..0x0061F062) and `manager` read from its home at 0x0061ED8A, manager in edi only from 0x0061EF63;
  ours: quest_card in memory, const 0 in edi from 0x0061ED71, manager piece in ebx from 0x0061ED81 (tools/rdef.py, tools/sbs.py).
  Same family as hand883 0x006096E0 (pending/2026-11-08_linux_hand883/TRACE.md): c2 live-range split pass, piece priorities.
- Regime: 5130 IL tuples (tools/pk/tc.py) -> regime 0.
- Frame order follows the allocator (packer weights are counted after register allocation): fix registers first.
- Sibling-style ports tried (no gain): one-arg GetTC + no GETTC_EXPANDED (b1: 12308, 211 real), `if (count != 0)` before the j
  loop (b2: same as base; b3 = b1 + it: 12308).
- b4: named `CViewport* viewport = static_cast<CViewport*>(movie_viewport.Get());` between the two mini-map creates (sibling 0x0061B610 line 238; retail `mov esi,[esp+90h]` 0x0061EFB7): 214 -> 124 real hunks, first region (quest_card edi / const 0 ebx) now retail; size 12388 (b5 = +one-arg GetTC: 12340, 121). What-if tooling: tools/wf.sh, tools/ptf.sh (c2prio_f.py forced priority by id:origprio), tools/og.py.
- b7 = b5 + sibling CBasicString::Compare text (while (*a != 0 || *b != 0)) + boast section INSIDE the type == CURRENT arm (retail jne 0x0061FF68 jumps over it): DIFFER(12356v12356), 16 real hunks (b6 without one-arg GetTC: 12388, 27).
- b8 = b7 + named `std::pair<const unsigned long, CUIStateDef> state(state_index + 8, state_def);` then insert(begin(), state) (sibling line 420;
  retail `inc ebx` 0x00621090 before the pair/state_def frees): 7 real hunks, frame now = retail slot for slot.
- b9 = b8 + named `long num_boasts = ...NumBoasts; long num_entries = (num_boasts != 0) + 1;` (sibling 381-383; retail reads NumBoasts
  into esi before the def-pointer release 0x0062085A): DIFFER(12356v12356), 2 real hunks left: 0x0061FB58 / 0x0061FB5E
  `[eax+ebx]` v ours `[ebx+eax]` (SIB operand order in the inlined CBasicString<char>::Compare of the boast loop). differ/0061e6d0.cpp = b9.
- LESSON: the whole "frame order" residue was five sibling idioms not ported to the PC twin (named viewport, Compare text, boast block
  inside the CURRENT arm, named pair state, named num_boasts) + one-arg GetTC without GETTC_EXPANDED.
- c1 (sibling `delete_action(new CAction)`, no named object), c2 (PDB inline IsQuestCardAvailable instead of `bool started`), c3 (count
  guard), c4 (frame_parent ternary), c5 (region_offset(0,0)): all byte-neutral. Kept c1 + c2 (sibling / PDB forms).
- c6 = c2 + min_zoom declared before max_zoom (sibling order; retail stores 0.2 first at 0x0061EB16). differ/0061e6d0.cpp = c6.
  STATE c6: 3334/3334 instructions; exact residue (tools/fo.py, frame offsets NOT masked):
  (1) SIB order 0x0061FB58 / 0x0061FB5E retail [eax+ebx] v ours [ebx+eax] (inlined Compare in the boast loop);
  (2) x87 operand order 0x00620FCF retail `fld origin.X [20Ch]; fadd Position.X [338h]` v ours swapped (`state_def.Position += origin`);
  (3) frame: retail slot order [0x34: key-local `type` / state_index / inventory_map (4 bytes)] [0x38: entry (8)], ours entry first:
      packer density entry w23/8 = 2875 v slot72 {state_index, inventory_map, #89, #90, #139} w11/4 = 2750 -> retail needs slot72 w >= 12.
  DIAG rulers (never hand back): `inline long DiagRuler2(const long* v){long t=*v; long u=t+1; return u;}` called 4 or 5 times at the
  function top fixes (1) only (k=3 fixes (1) but flips x87 at 0x0061EADC; k=1,2 nothing); empty one-parameter inline x1..8: no effect on (1),
  flips 0x0061EADC at k=2,3,6,7; dead `long x = 0;` locals: no effect. `Position = origin + Position`: worse (c7). Function-level
  `inventory_map = 0`: no change (c8).
- c9 = c6 + single unavailable arm in the sibling form `if (renown < needed || !IsQuestCardAvailable(*quest_card)) { MakeUnavailable; RemoveChild } else { take }`: our two copies were cross-jumped into one but both counted in the packer weight of entry (23 v 21) -> frame now EXACTLY retail. Residue = (1) SIB 0x0061FB58/5E and (2) x87 fadd order 0x00620FCF only. differ = c9.
- d1 = c9 + `state_def.Position.X += origin.X; state_def.Position.Y += origin.Y;` (per component, like the neighbouring lines) instead of `Position += origin`: x87 order 0x00620FCF fixed. ONLY residue: SIB 0x0061FB58/5E. differ = d1.
- DIAG big ruler on d1: 32 / 48 / 64 units of `inline long DiagRuler3(long v){long t=v; long u=t+1; return u;}` at the function top, or 32..128 units at the very END of the function (before the function-level inventory_map) = RELOCATION_MATCH; 16 units = SIB still wrong (top 96/128 changes code). So the SIB order is a THRESHOLD on the function's symbol-record count (free 32-record chunks), not a position: retail's source creates more records = more no-byte inline layers / named forwarded locals.
- n1 = d1 + #define FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB (FableWin ??GC2DVector 26i/68b = named result; existing opt-in): ruler threshold at function end k* drops from 20 (d1) to (12,17]; n2 = n1 + sibling idioms (count guard, region_offset(0.0f,0.0f), frame_parent ternary): k* in (14,17]. differ/0061e6d0.cpp = n2 (still the 2 SIB hunks). operator-> through Get() (diag overlay hdrx): no effect. IsQuestCardAvailable text v FableWin 48i/160b (/EHsc /Os): current 44i/146b; named def pointer 46i/159b; none exact.
- n3 / n4 (IsQuestCardAvailable with a named def pointer / named region string): change code (3340 / 3333 instructions): rejected.
- n5 / n6 (#define FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE, the STLport 4.6 allocator::deallocate layer): no flip and no change of
  the ruler threshold (k* still > 14): pure inline LAYERS (no named locals) do not count; operator-> through Get() likewise.
  What counts: named locals inside expanded inlines / the row that fold away (the ruler unit has two; ONE named-result C2DVector
  operator- expansion was worth >= 3 units).

### Row 0x0061E6D0 -- state at hand-over (differ/0061e6d0.cpp = n2)
- DIFFER(12356v12356), 3334/3334 instructions, all frame offsets retail, 2 instructions differ: 0x0061FB58 `cmp byte ptr [eax+ebx],0`
  and 0x0061FB5E `mov dl,[eax+ebx]` (ours [ebx+eax]). No device, no flag, no new header opt-in (uses the existing
  FABLE_RETAIL_C2DVECTOR_NAMED_RESULT_SUB; GETTC_EXPANDED dropped; one-arg GetTC / PeekTC as the landed sibling).
- Measured lead (DIAG only): the order flips to retail when the function has more c2 symbol records: k* = 15..17 units of
  `inline long DiagRuler3(long v){long t=v; long u=t+1; return u;}` (tools/mkr.py, default position = end of the function; position
  does not matter, top or end). Base d1 needed 20; operator- named result brought it to <= 17.
- Next ideas, in order: (1) ConvertVector inline text (expanded once at 0x0061EA22): FableWin 28i/81b v ours 29i/95b at /Od (28i/93b
  /EHsc /Os) -> the original text differs (fewer absolute operands); try texts with named C2DVector locals, check the landed
  0x0052E580 still matches; (2) CAction::CAction inline (6 `new CAction` sites): FableWin 26i/86b v ours 28i/88b at /Od;
  (3) SetStaticCast FableWin 14i/38b v ours 12i/30b (/EHsc /Os): a named local `T* data = static_cast<T*>(other.Data)`?;
  (4) CDefString::GetString 19i/54b v 17i/46b; CBasicString<char>::Compare 40i/102b v 41i/93b; IsQuestCardAvailable 48i/160b v 44i/146b
  (texts A-D in probe/fw/ gave 44/45/42/46); (5) PC-only statements that may have had named C2DVector / pointer locals.
  Each candidate: grade directly, else measure its worth with tools/mkr.py (k = 14, 12, 8 ...). tmp/fw_d1p.txt (plain /Od) and
  tmp/fw_d1.txt (/EHsc /Os) hold the full fwsize tables of the row's helpers.

## Rows 0x00603000 / 0x005FF190 -- NOT opened (row 1 not matched); one unverified lead for the frequency construct
- The shared STLport overlay already has the authentic STLport 4.6 no-return arm: `bitset::test(n) { if (n >= _Nb)
  __stl_throw_out_of_range("bitset"); ... }` with `inline __stl_throw_out_of_range(msg) { puts(msg); abort(); }` (stl/_range_errors.h
  without iostreams; `abort` is __declspec(noreturn) in VC7.1's stdlib.h). Every HasTC / GetTC / PeekTC expands it with a constant
  index, so each lookup carries a folded `if (const) { noreturn }`. C26 measured "asserts inside an inline helper do not change the
  caller's frequencies" with RETURNING handlers only; a NO-RETURN arm inside an expanded helper was not measured. Check first
  (one inline trace): does a top-level `thing->HasTC(TCI_X)` / GetTC(&p) statement halve the frequency of the code after it? If it
  does not, check whether it does when __stl_throw_out_of_range is only DECLARED (out of line, __declspec(noreturn)) instead of
  the inline puts/abort body. Also count the early exits: row 0x00603000 has exactly THREE early `return`s before the second hero
  block (the not-hero-region block, `if (maps.empty()) return;`, the IsMapLoaded loop) = the three halvings C26 needs; check
  whether row 0x005FF190 has two early returns at the top and one before the third cards loop (C26's placement there). If the
  counts agree, the construct is the early return itself (how c2 scores `return` when destructors are pending v not), not an assert.

## ROW 0x0061E6D0 MATCHED (RELOCATION_MATCH) -- src/0061e6d0.cpp
- Closer of the last residue (SIB 0x0061FB58 / 5E): the inline ConvertVector written through the PDB's per-axis converters
  ConvertX / ConvertY (new nested opt-in FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE_CONVERTXY in
  hdr/engine/NUISystem_CUIScreenUnitConverter.h; the existing body moved under #else unchanged, default path untouched).
  Evidence: Ego_r / FableWin PDB names ?ConvertX / ?ConvertY @CUIScreenUnitConverter (SIMM@Z); FableWin ConvertVector 28i/81b v
  new text 27i/77b v old 29i/95b (plain /Od, probe/fw/cv1.cpp); out-of-line copy 0x0052E580 = RELOCATION_MATCH from the new text
  (probe/fw/cvo.cpp); the row itself. ConvertX / ConvertY bodies are INFERRED (FableWin 15i/45b each, ours 13i/42b).
- Full recipe v C26's draft: see the `// Evidence` block at the top of src/0061e6d0.cpp. No flag, no device, no assert.
- test/0061e6d0.cpp: the twin's mock engine + the row statements unchanged; vt.py 0061e6d0 PASS; mutants (|| -> &&, boast filter negated, state key + 9, Position.X += dropped, frame choice inverted) each FAIL natively. addmatch.py done (payload.json / oracle.tsv). vland read-only: see tmp/vland.log.
- vland read-only (tmp/vland.log): `0061e6d0 RELOCATION_MATCH PASS`, WINS 1, 0 RELOC_IDENTITY, 4 WARN RELOC_NAME (catalog labels of
  callees that differ from the bound PDB spelling, e.g. 0x0099EE20 labelled 'CCharString::ToUpper' v ?GetAsUpperCase@CCharString;
  the same warnings as the landed twin's callees; nothing to fix in the row). lv/ deleted.

## Rows 0x00603000 / 0x005FF190 -- opened only for two DIAG grades (context ran out after row 1)
- probe/m2/d0.cpp = prev_C26/differ/00603000_diag_freq.cpp regraded on today's overlay: DIFFER(9571v9571), 303/303 calls, 6 real
  hunks (the three known sites) -- C26's state reproduces.
- probe/m2/d1.cpp = the same three no-return guards moved INSIDE an inline helper (`inline void GFDiagCheck() { if (0) {
  GFDiagNoReturn("a"); } }` called three times at the same place): DIFFER(9698v9571), 303/304 calls. MEASURED: a no-return arm
  inside an expanded inline helper does NOT halve the caller's static frequency. So the STLport `bitset::test` range check
  (`__stl_throw_out_of_range` -> puts + abort, already in the shared overlay and expanded under every HasTC / GetTC) cannot be the
  construct, and my "unverified lead" above is REFUTED. The construct must sit at ROW level (a macro expanded in the row's own
  statements), as the coordinator decided.
- The early-return idea is also refuted by C26's own numbers: the destructor decisions of the early returns (decisions <= 76,
  including the IsMapLoaded loop's) need k < 1.27, so nothing before the second hero block may halve.
- Not done (for the successor): the `fable_assert.h` no-return macro itself, its placement in both map rows, the sibling checks
  (0x0060C900, 0x0060EEB0, 0x00605570, 0x005FE090, 0x0061B610) and the three remaining hunks of 0x00603000. Row 1's lesson applies
  first: diff both map-row drafts line by line against their landed siblings (0x00605570, 0x005FE090, 0x0060C900 family) for
  statement idioms, and check every expanded inline helper against FableWin sizes + PDB-listed per-part helpers (tmp/fw tables).
- differ/: rows 2 / 3 sources are agentC26's, copied UNCHANGED (00603000.cpp, 005ff190.cpp = C21/C26 device states; *_diag_freq.cpp = DIAG frequency states with GFDiagNoReturn guards, never a hand-back). The last non-matching state of row 1 is probe/a/n2_final_differ.cpp.
