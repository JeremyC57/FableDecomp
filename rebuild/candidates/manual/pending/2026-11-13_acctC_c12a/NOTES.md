# agentC12 NOTES (acctC lane; continues agentC11 -> prev_C11/, C10, C9)
Tools: tools/ = C11's + C9's (repointed) plus NEW tools/pp.sh (iset packer probe + fmap for 0x5EEFF2), tools/psim.py (packer replay
with weight overrides from an --iset probe), tools/ps2.py (search weight deltas that move an item into another's slot), tools/pw.sh
(grade + per-counter weight/slot), tools/fd2.py (one line per differing instruction of a q.py file), tools/calltg.py (retail call
targets per /FAs call symbol), tools/hq2.sh (grades 0x5EEFF2's same-TU helpers + STLport COMDATs), tools/g.sh (one-line grades).

## Row 1 0x005EEFF2 CTCInventoryAbilities::GetMagicListUIComponent -- RELOCATION_MATCH (src/005eeff2.cpp)
From C11's DIFFER(5619v5619) frame-only residue, in ~25 grades:
1. fmap (pk --iset probe) showed the real difference: retail slot 0x68 = {received, fill/swap counter(s), spellContainer, caption,
   the four CreateComponent name temporaries}; slot 0x64 = {physicalCount, received-loop n, palette n, reorder n, SetEmptySlotDef
   temp}. Ours had the shared fill/swap n with physicalCount and the reorder n with received.
2. Packer replay (psim/ps2): with ONE shared fill/swap n no weight increase reaches retail's partition; with SEPARATE fill and swap
   counters (each `for (unsigned long n ...)`) the fill/swap counters land with received (retail), and the reorder counter needs w+4
   or to conflict with any member of the received slot.
3. The conflict: retail 0x005EF489 `mov ecx,[ebp+68h]` (the residue word C11 saw as [ebp+68h] v [ebp-0Ch]) is the palette loop's
   `spellContainer->AddChildAndSetOffsets(spell)` (ours used static_cast<CSpellContainer*>(container.Get())). That keeps
   spellContainer live across the reorder loop -> reorder n joins physicalCount's slot -> densities give retail's offsets.
   Both changes together: RELOCATION_MATCH (either alone: DIFFER).
Coordinator decision applied: the STLport bodies moved out of the row into hdr/stlport/fable_containers.h as new opt-ins
FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE (_List_base ctor/dtor, 0x005F88ED / 0x005F8908 RELOCATION_MATCH) and
FABLE_RETAIL_STLP_VECTOR_SIZECTOR_VISIBLE_NOINLINE (vector(size_type), 0x005F8EC0; its own COMDAT is DIFFER(50v47): retail calls
uninitialized_fill_n as a __fastcall COMDAT 0x005F8220 (YI mangling in reloc_bindings) with the non-POD _Construct loop; the row is
unaffected). Row grade unchanged (RELOCATION_MATCH). Same-TU helpers (tools/hq2.sh): GetCategory 0x005EDE37 RELOCATION_MATCH,
GetAbilityPDef 0x005EDFD9 MATCH, GetMiscThingsDef 0x005EE3B3 RELOCATION_MATCH once FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
(existing opt-in: GetDef<T> body visible, noinline) is defined (without it 111v101: the def handle escapes -> reload before release).
FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE dropped (row identical without it). GetAbilityLevel stays the sibling's same-TU inline.
Test: test/005eeff2.cpp (tools/mktest3.py = tmp/t3/prefix.cpp + the row text verbatim + tmp/t3/suffix.cpp; mock engine; scenarios A/B);
vt.py PASS; ten one-decision mutants (g++ -fshort-wchar) all fail. Read-only vland: WIN, RELOCATION_MATCH PASS, only WARN RELOC_NAME
(DB labels of 0x005F88ED / 0x005F8908 / 0x0042925C / 0x0042ABCA; see LABELS.tsv). payload.json / oracle.tsv written (addmatch.py).
Flags: none (optimize("s",on) pragma in the file). Defines now: ..._LIST_BASE_VISIBLE_NOINLINE, ..._VECTOR_SIZECTOR_VISIBLE_NOINLINE,
FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (for the helper text), RELEASE_POINTEE removed.
UPDATE: `// Flags: /Gr` adopted for 0x005EEFF2 (row identical with or without): this TU's free template functions are __fastcall
(retail vector<EHeroAbility>(size_type) COMDAT 0x005F8EC0 calls __fastcall uninitialized_fill_n 0x005F8220; under /Gr that COMDAT
RELOCATION_MATCHes, as do all other helpers: tools/hq2.sh with QF=/Gr). payload.json re-written with --flags /Gr; vland WIN again.
Coordinator: the whole CTCInventoryAbilities TU is probably /Gr (row 0x005F59C7 is unchanged by /Gr: same 35 hunks).

## Row 2 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList -- DIFFER(2649v2652) (best still prev_C11/differ = differ/005f59c7.cpp)
New tools: tools/gx.sh (grade + sd hunk count), tools/pr2.sh (c2prio: prio of textBank / const 0 / -1 / inventory, K of b10 / b87),
tools/wq.sh repointed (leaf_x.txt). Measured this session (ATTEMPTS.log):
- What-if bounds: with textBank forced -100, forcing const -1 (whole AND pieces, wig.py forces pieces too) to any value in [-80, 40]
  gives retail (D 13 = masked relocs); 48 / 55 / 60 / 100 do not; UNFORCED -1 with tb=-100 does not (its piece #85 gets prio 76 and is
  chosen before the inventory piece #87 (41), taking ebx over head + loop 2). So retail = (textBank < const 0) AND (-1 piece #85 < 41).
  #85's prio is 96 (b87: 12 refs x K8) + 6 (b85) - ~26 (blocks it spans) -> needs K87 <= 5; textBank = 7 x K87 < 48 -> K87 <= 6.
- b87 (InitialiseOffsets(4) .. the stealth-vector free compare) references 8 candidates: this(5), parent(4), -1(12), textBank, subList
  (essential, 5) + the PeekComponent() result (GetTextBank's inline this, #78), the stealth vector's _M_start field (#31, the cmp that
  ends the block) and const 0 (that cmp). Retail's tail has no branch (0x005F6182..0x005F63C0), so with this model retail must lack
  ALL three incidental candidates, or this/parent are not candidates there. Raising inventory/const 0 instead does not reach retail
  (inventory=80/90 + const 0=57..70: D 104..191).
- No effect (hunks 35 unless noted): /Gr; flags /O1 /Ox /Os /Ot (pragma present) and /O1 /Os /Og without the pragma; pragma
  optimize "gs"/"gsy" (pragma "g"/none: 3772 bytes); PUSHBACK_NOINLINE (as row 1); textBank address taken via a dead pointer-to-it
  (folded); categoryTitle before textBank (b1); tail in its own {} (b8: 36); explicit CCharString temporaries for one call (b5: code
  differs, construction order); raw-cast text bank (f1: K87 7, tb 49 as C11).
- Diagnostic d2 (a real `if (allItems.size()==1000) return subList;` between categories): each category block still has K 7
  (this, parent, -1, textBank, subList + temporaries), so splitting the tail into blocks does not lower K: the five essential
  candidates alone give K >= 5 per block, and the stealth-vector free compare (stealth _M_start + const 0) closes the last one.
  `if (!parent) ...` is folded by c2 (parent was dereferenced before).
- Declaration / const spellings v03..v08 (loop counters declared outside, const miscThingsDef / subList / frame, textBank +
  categories in an inner block): all neutral (35 hunks). c2prio on matched row 1 shows `this` / `parent` are ordinary candidates in
  this TU, so they are candidates in retail's row 2 too.
- Anniversary PDB has no ConstructPCExpressionsList (PC only); its console twin's locals (ptc_inventory, pthing_def,
  pexpression_def, pexpression_sub_def, menuparam) belong to a rewritten Xbox 360 body: no spelling facts for this row.
Open (for the next agent): under the c2prio model retail's tail block needs K <= 5 but straight-line code gives >= 6 (5 essential +
  const 0 of the closing compare). Either the model's piece rule is incomplete (measure piece #85's blocks with a what-if trace:
  wig.py `blocks_once` / watch, then find which blocks retail's piece spans) or retail's IL has a fact not visible in the code
  (e.g. one -1 site of another type / a value that is not a candidate). Not tried: per-piece what-if (force only the tail piece),
  CCharString default-argument constant spelled differently in the shared header (an opt-in), GetTextBank text per fwsize
  (FableWin 10i/22b; ours 8i/15b raw, 9i/20b smart accessor -- a third text may exist).
