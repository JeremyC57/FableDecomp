# agentC9 NOTES (acctC lane; continues agentC7 -> prev_C7/, C5, C4)
Tools: tools/ = C7's tools (paths moved to agentC9; `export W` first) plus NEW tools/fmap.py (zip our /FAs listing with retail:
per frame symbol our v retail offset), tools/pkj.py (packer join order per item, from pk.py's json), tools/pm.sh (both on a source).

## Row 1 0x005F1B10 CTCInventoryAbilities::ConstructPCMagicList -- RELOCATION_MATCH (src/005f1b10.cpp), test: see below
From C7's 8383v8383 frame-only residue, three source facts (grades in ATTEMPTS.log, 6 grades):
1. Packer rule (MATCHING_PLAYBOOK): nodes by size then IL weight (+1 per IL reference BEFORE cross-jumping); each node joins the
   LATEST slot it does not conflict with. fmap.py showed retail's only 8-byte difference: the expression-block `icon` (w8) shares
   the word of the spell-block `assign` / the main-loop key temporaries ([ebp+30h]) -- possible only if it is placed before `ring`
   / main-loop `icon` (w9) open their slots. Writing `icon->AddActionOnDroppedNowhere(action);` in BOTH arms of the isSpell
   if / else (c2 cross-jumps the two calls into one: code unchanged) gives icon w9 and retail's whole 8-byte assignment
   (follow -> icons' word 0x10, expr assign -> ring's word 0x08).
2. CategoriesLists[GetCategory(*it)] key temporary: with FABLE_RETAIL_STLP_MAP (existing opt-in: map::operator[] body visible,
   still called out of line) c2 knows the key does not escape, it joins the [ebp+58h] slot and that slot's density then puts it
   above inventory/n ([ebp+54h]): fixes the 0x54/0x58 swap and 0x005F34C2 together (GOTCHAS Q32 lesson).
3. `unsigned long surroundCount=0,physicalCount=0,attackCount=0;` (retail stores surround, physical, attack at 0x005F1CA0).
Same-TU bodies GetAbilityLevel 0x005EDE93 / GetAbilityGraphicIndex 0x005EDE7F as C7 (coordinator accepted). No header change.
Test: test/005f1b10.cpp (tools/mktest2.py = tmp/t1/prefix + the row text verbatim + tmp/t1/suffix; mock engine recording every UI
call; scenarios A/B); vt.py PASS; six one-decision mutants (g++ -fshort-wchar build) all fail. Read-only vland: 1 WIN, RELOCATION_MATCH
PASS, only WARN RELOC_NAME (DB labels of 0x005F88ED / 0x0042925C / 0x0042ABCA / 0x005F8908 differ from the STLport / CCountedPointer
symbols: label-DB naming, no RELOC_IDENTITY). payload.json + oracle.tsv written by addmatch.py.

## Row 2 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList -- DIFFER(2649v2652), best differ/005f59c7.cpp (= probe/b/K1.cpp)
Port of the chunk-867/1008 draft onto today's headers, with two fixes: (1) GetDef for the expression def is the TEMPLATE
`GetDef<CExpressionDef>(long, CDefPointer<const CExpressionDef>*)` -- reloc_bindings.tsv binds 0x005F81BE to
??$GetDef@VCExpressionDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCExpressionDef@@@@@Z (the draft's non-template overload
+ FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF would have been a RELOC_IDENTITY; the landed console twin 0x005F4C8F uses the
template too); (2) the quick-slot loop is written in row 1's matched spelling (n, name, slot, observe, observeDrop, assign, inside,
icon, sprite, action, follow, swap; UI_EVENT_MOUSE_MOVEMENT 25 / UI_EVENT_COMPONENT_DROPPED 30). Both byte-neutral.
Residue (tools/mx.py: D 247, frame-masked 126, frame+register-masked 38 = all register materialisation): retail = const 0 in edi
for the whole function, the -1 constant's LAST piece in ebx (materialised `or ebx,-1` in loop 2's latch 0x005F60F8 and on the
empty-loop exit 0x005F6122), textBank in memory ([ebp+5Ch], shared with inventory's spill home). Ours (c2prio, tmp/prio_D0.txt):
textBank prio 56 (one tail block, K 8 x cost 7) is coloured before const 0 (48) and takes edi; const 0 takes ebx; -1 splits to
immediates. Measured with the what-if hook (tools/pk/wi.py, FUNC=ConstructPCExpressionsList, names with a leading underscore,
e.g. `_textBank=40`; tools/objcmp.py grades the what-if object): forcing const 0 above textBank (`const 0=57`) or textBank below
const 0 gives retail's tail exactly (textBank memory, -1 piece #85 prio 76 -> ebx, D 95 v 247), but the -1 piece then also covers
the head and loop 2 (`or ebx,-1` at 0x005F59E7 / 0x005F5E33) where retail uses immediates: so retail's IL differs in TWO places
(const 0 > textBank, and the -1 constant's split pieces). const 0's +60 comes from block 10 (15 vector zero stores, K 4); -8 from
the loop-2 pre-header that hoists 8 constants (K 8).
Tried, no change in prio / colouring (6 grades + 9 c2prio runs): title..tail in a `{}` block like the console twin (E1/E2: worse;
retail destroys categoryTitle, title, positionDummy, itemsContainer at function end, so no tail block), the quick-slot loop in
its own block (G1), bank as a pointer (G5), `inventory` declared at any of 4 positions (G6, d1_*), loop-1 handle declaration order
(d2_1), row-1 loop spelling (J1), template GetDef (K1), FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS (K2).
Next: find the IL fact that gives const 0 >= 57 (more candidates in block 10, or fewer live-through blocks) AND the -1 piece
shape; compare per-block K of the twin rows (console 0x005F4C8F landed: textBank 70, const 0 115, -1 160 -- no loop there).

## Row 3 0x005FF190 CTCInventoryMap::CreateMapMarkers -- not opened (row 2 still DIFFER; brief: one row at a time). Best remains
prev_C7/differ/005ff190.cpp DIFFER(4096v4176).

## Session summary (agentC9)
Row 1 matched (RELOCATION_MATCH + test PASS + read-only vland WIN; payload.json / oracle.tsv). Row 2: draft ported and two names
fixed (template GetDef<CExpressionDef>; row-1 loop spelling), residue fully characterised with c2prio + what-if forcing (see above).
New tools: tools/fmap.py, fmap2.py (frame-symbol maps v retail, the latter tolerant of register/shape differences), pkj.py, pm.sh,
mx.py (D / DF / DR metrics of a q.py file; SHOW=0/1/2 prints hunks), gq.sh (grade + metrics), pr.sh (c2prio summary),
objcmp.py (metrics of a what-if object v retail), pk/wi.py patched (FUNC env, LOG env). No header change.
