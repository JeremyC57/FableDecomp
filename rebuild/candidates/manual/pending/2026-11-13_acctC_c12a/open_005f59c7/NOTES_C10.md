# agentC10 NOTES (acctC lane; continues agentC9 -> prev_C9/, C7, C5, C4)
Tools: tools/ = C9's tools (paths repointed to agentC10) plus NEW: tools/cp.sh (c2prio one-liner: prio/register of textBank,
const 0, const -1, inventory, manager, frame), tools/vb.py (batch grader: variants.py with V=[(name,note,[(old,new)..])]; one line
each: D/DF/DR + grade; grades log to ATTEMPTS.log), tools/bsum.py / rsum.py (per-block / per-region priority shares from
c2prio --blocks), tools/pk/tc.py + tcg.py (regime-gate probe; reading unreliable, ignore). A grade takes ~1.5 s; c2prio ~25 s.

## Row 1 0x005F59C7 CTCInventoryAbilities::ConstructPCExpressionsList -- DIFFER(2649v2652), base probe/a/B0.cpp (= C9 differ)
KEY MEASUREMENT (what-if, tools/pk/wi.py FUNC=ConstructPCExpressionsList ALL=1, objcmp.py):
  B0 + `const -1=30,_textBank=-100` (also tb=0/20, -1=40, or -1=-67 with tb=-100) -> len 2652, D 13 DF 13 DR 13 = RETAIL (the 13 are
  masked vtable relocations). B0 + tb=40,-1=30 -> DR 39; -1=45,tb=-100 -> DR 47. So retail differs from ours ONLY in two priorities:
  (1) textBank prio must be < the -1 tail piece and < const 0 (ours 56 = K87 8 x cost 7; needs <= ~29 or non-candidate);
  (2) every -1 PIECE prio must be < inventory's head..middle piece (41) (ours: -1 piece after the split pass ~76 with tb absent 64)
  so that inventory's piece takes ebx before the -1 piece; the -1 tail piece then gets ebx, textBank memory.
  Mechanism (trace tmp/tr_D1.txt): the split pass runs right after const 0 takes edi and splits -1 AND inventory together.
  Diagnostic D1 (textBank address escapes to an extern: non-candidate) alone gives -1 ebx in head/middle (DR 41); D1 + -1=40 -> DR 22
  (= the diag call only). Region shares (rsum.py on B0): -1: head -10, loop1 -40, middle+loop2 -118, tail +101; inventory: head
  +34, loop1 -40, middle+loop2 -36; textBank: tail +56 only. K of the tail block b87 = 8 (this, parent, -1, textBank, PeekComponent
  result, a vector-free local, subList, const 0).
Tried (no change at all, 10 grades, probe/b/v1.py): const misc def, inventory decl order, frame!=0, i++, slot!=0, !QuickAccess,
  title copy-init, this->ParentThing, return cast, nested sprite if. Named intermediate `game` for PeekComponent / categoryTitle
  declared first / bank as pointer: prio unchanged (cp.sh).
Retail-like what-if colouring order (tmp/s6/wi_x12506.json): const 0 edi (48) -> inventory piece #87 (41) ebx -> ... -> -1 tail
  piece (#38) ebx; textBank never coloured (memory [ebp+5Ch]). Ours: textBank (56) edi first, const 0 ebx, inventory #87 edi.
  With tb low but -1 natural (x6281): after const 0, the split pass makes -1 piece #85 prio 76 which takes ebx before inventory's #87
  (41) -> inventory re-split, -1 ebx in head/middle/loop2. INEQUALITY: tb < -1 piece < 41 and tb < 48. Simplest fit: the tail block
  b87 has K=5 instead of 8 (tb 7K=35, -1 piece 12K-20=40). b87 candidates: this, parent, -1, textBank, PeekComponent result
  (local78), local29 (vector-free read), subList, const 0 (vector-free compare). Inner scopes (s1/s3/s4) do not move the vector
  frees out of b87 (K stays 8).
Insensitive (probe/b/v2..v7.py): forwarded named copies (inv2, subComponent, sl, tb2, list, item, type: K unchanged), GetTC texts
  (expanded / default HasTC+Find / ORIGINAL_CHAIN / ORIGINAL_FIND / HASTC_FORCEINLINE), textBank pointer declared at top / before
  loop 2 / `=0` (dead store removed), loop2 as guarded do-while / index outside; while loops change code. Header GetDef body
  (GETDEF_GLOBAL_BODY, H2) no change; same-TU helper bodies do not byte-match retail (GetDef 105v98 / 101v98 with the header body,
  GetMiscThingsDef 98v101, GetSubDef 159v182) but the row is insensitive to that.
More row-1 probes: k1 (no cached bank: 4 PeekComponent calls) and D1 (bank escapes) both = "textBank absent" -> const 0 edi, but
  the -1 piece then takes ebx in head/middle/loop 2 (DR = D1's). k3 (CMainGameComponent::Get()) / k4 (const pointer const) no
  help; k2 (copy-init) worse. So BOTH facts are needed; the -1 piece fact is the open one. Open question for the successor: why
  the -1 whole is split at the first split pass (right after const 0 takes edi) although ebx is still free -- the trace shows the
  split pass over 20 ids (incl. -1 #1 and inventory #5) immediately after `const 0 -> edi`; find c2's rule (0x1078b2aa caller)
  and what keeps -1 whole (unsplit) until after inventory's piece in retail.

## Row 2 0x005FF190 CTCInventoryMap::CreateMapMarkers -- DIFFER(4096v4176) unchanged, base probe/m/M0.cpp (= C7 differ)
Not in regime 1 (loop blocks weighted). c2prio (tmp/prio_M0.txt): const 0 -316 split (ebx #4, eax, edi..), manager -742 split
  (ebx #218 + memory), this -829, parent -965 split (ecx pieces), const -1 -1056 immediate. Retail head: `xor ebx,ebx` (const 0
  piece in ebx: two stores + `cmp edi,ebx`), then `mov ebx,[ebp+8]` (parent piece ebx), manager stored to [esp+10h] at once.
What-if (objcmp D/DF/DR v baseline 712/723/456): manager=-5000 708/719/454; const 0=300 worse (DR 1007); const 0=-2000 681/681/444;
  parent=-300 no change; this=-2000 worse. Name-wide forcing is too coarse here (it hits every piece): next, force single piece
  ids (extend wig.py to match by candidate id) along retail's head order.
Threshold scan (what-if on B0, objcmp): retail (D 13) for {-1=30,tb<=20}, {-1=40,tb=-100}, {-1=40,tb=35}, {-1=-67,tb=-100};
  NOT for {-1=30,tb=30} (tie), {-1=30,tb=40}, {-1=45,tb=-100}, {const 0=57,-1=30} (tb 56 then takes ebx in the tail),
  {const 0=57,-1=50}. So: tb < -1 piece <= ~40 < inventory piece (41), tb < 48. The per-block model (tb = 7*K87, -1 main
  piece ~= 12*K87 - 20) fits retail EXACTLY at K87 = 5 (tb 35, piece 40): retail's tail block most likely references three
  candidates fewer than ours (ours: this, parent, -1, textBank, PeekComponent result, vector-free local, subList, const 0) or the
  tail is split into blocks of lower K. Not reproduced: inner scopes, do{}while(0) (folded before blocks), named copies.
Rows 1 and 2 hand-back: differ/005f59c7.cpp (= probe/a/B0.cpp + updated residue lines), differ/005ff190.cpp (= C7's, unchanged).
No header change, no new names (row 1 names as C9: template GetDef<CExpressionDef> 0x005F81BE).
Also insensitive (probe/b/v9.py, v10.py): CTCInventory& alias for all / loop-2 uses, const frame / subList pointers, STLport
  allocator texts (MALLOC_ALLOC_DEALLOCATE, ALLOC_PROXY_REBIND), STLP_VECTOR / _C / _INDEX_START: K of the tail block stays 8.
  The tail block in retail has no incoming jump (no label between 0x005F6182 and 0x005F63C3), so it is one block there too:
  the K difference must come from candidates (memory-only ones count), not from block shape.
Grades this session: row 1 58 (plus ~25 c2prio / what-if runs), row 2 1 (plus 8 what-if runs). Budget not reached: the
  successor should continue on row 1 from the inequality above (find which tail-block candidates retail lacks: the
  PeekComponent result temp and the vector-free local/compare are the only removable ones) and on row 2 with per-piece forcing.
