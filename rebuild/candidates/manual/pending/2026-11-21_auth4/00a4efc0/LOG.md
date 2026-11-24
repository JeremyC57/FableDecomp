# 0x00A4EFC0 -- AUTH4 log (base = S/w22/auth2/00a4efc0/a2.cpp + hdr, opt-in renamed FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_IFBODY)
Tools: S/w22/auth4/rd.py (residue count, relocs masked), rng.py (side-by-side range), evs.py / lin.py (hook json: refs by line, split lineage).
1. a2 regrade: DIFFER(3491v3491) residue 16 (reproduced). Grades 1.
2. Hook readout h/a2.{prio,trace,json} (readouts 2). Hook line = rel+44 (149 = position compressed line, 191 = scaling).
   Line 149 refs: locals 4 (r), 100 (r,w); temps 187 (prio 132), 189 (66), 190 (88). Scaling line 191: local 27 (r); temps 180 (108), 181 (54), 182 (72).
   Cross-block locals exist only for rotation (99, 101) and position (100, 103): their track sizes are read again at 200..211; scaling's are not.
   So the ROTATION block (matches) is the analog, not scaling.
   Retail at 149: ebx = paletted finish ([esp+54h]) piece, compressed computed after uncompressed. Ours: finish via edx temp, compressed in ebx.
3. Refs (hook json, abs lines): 4 = w119 r116 r140 w143 r149 w153 r201 r209 (paletted_position finish: written by both clears, read at 149/201/209);
   100 = w140 w143 r149 w149 w153 r201; 103 = w145 r148 w148 w155 r203 r211; rotation analogs 29 / 99 / 101.
   Lineage of 4 (lin.py): split -> 4.a (refs 4, prio -75) + 4.b (edi); 4.a -> .a -> .a -> .a.a (refs 3, prio -17) + .b dropped -> memory.
   Retail: a piece of 4 over line 149 in ebx; then c2 codegen evaluates uncompressed first (left operand all-register), = the residue.
   Rotation (matches): finish loaded by a codegen temp (ecx), no register piece either.
4. c2 hook: the stock resort readout walked the OLD list head (LIST_HEAD is stored after 0x1078abac; the new head is eax) -> fixed in
   S/w22/auth4/c2x/c2prio_x.py; + C2X_FORCE2=n:a:b (force at the n-th sorted/resort; b=0 = head). Forcing at a resort (f1 13:142:0,
   f2 11:118:0) -> C1001 ICE in c2 (resort forcing is not a safe diagnostic). Readouts 5.
5. CORRECTION (advisor): hook line = source line - 1. Source 149 (compressed) = hook 148: refs 103 (paletted_position finish), 151 (count).
   Source 150 (uncompressed) = hook 149: 4 / 100 (position_track start / finish), temps 187/189/190 (the /6). 4's lineage = position start (edi), right.
   103's last split: 4 pieces, .a edi, .b/.c/.d dropped (refs 1, spill 0/0/-2). History (FINDINGS 1499/1503, p25P): extra finish read hoists the
   load but closes nothing; clear placement pinned by Ego_d; clear/erase/resize/size opt-ins negative. Not repeated.
6. IL dump (kit 2026-11-20_c2_symbol_order dump2/pp2, copied to S/w22/auth4/tools): ours evaluates compressed first in ALL THREE blocks
   (IL order, before allocation). Retail position evaluates uncompressed first (finish load hoisted into ebx by codegen/scheduling).
   m1 (source swap of the compressed/uncompressed declarations, measurement only, Ego_d contradicts it): byte-identical 16 -> the order is
   chosen by c2's tree optimiser, not by statement order. Hypothesis: the compare's operand evaluation order is set by the node keys
   (0x1075d388: SethiUllman<<24 | size<<16 | hash16; hash16 from symbol numbers), i.e. the symbol-number class (GOTCHAS 466, FINDINGS 1348).
   Grades 2 (m1 measurement).
7. Register-forcing diagnostics (c2x C2X_KEEP / C2X_SETREG, my additions): keeping 103's dropped piece #40 (= block 125) gives it eax
   (kp40 3481, 18) and forcing it to ebx (kp40b 3497, 19): compressed is STILL evaluated first. => not the allocator: the order is in the IL.
8. Tree-node keys (tools/il/keys.py hooks 0x1075d388 returns; keys_all.py / keytree.py): the compare's operands are NOT sorted (op 0x3e
   has ASC and desc child pairs), but its children carry keys su<<24|size<<16|hash16. Final pass (after the count CSE): position
   left 01067a9b v right 01063d0a: su, size equal -> hash decides; left > right -> left first. Scaling bb1b > 3dea, rotation 5a6f > 3c3d,
   also left first (= retail for both). Retail position needs hash(right) > hash(left).
   Hash (0x1075d54e) = sum child_low16 << (i&7) + (op-0x24a), mod 2^16. Position: L = cnt + 2*pal_start + 2*pal_finish + 0x99,
   R = pos_finish + 2*pos_start + 0xd9. Leaves: field symbol = sym<<5 (|1 for finish), temp = t<<6.
   Ours: cnt temp 14 (0x380), pal_start 29, pal_finish 445 (0x37a1), pos_start 23, pos_finish 436 (0x3681).
   Starts 21..31 (six CArray locals' _M_start), finishes 433..445 in one 32-block, except scaling pal finish 576.
9. PROOF (forced, diagnostic only: tools/il/kforce.py, at the compare node key 020df4ed set child#1's key 01063d0a -> 01067fff):
   object = RELOCATION_MATCH, residue 0 (kf1.obj; the pad at 0xA4FBE9 too). So the whole residue is the evaluation order of the
   position compare's operands, decided by the hash16 tie-break of the final tree-pass keys (su and size equal at 1/6).
10. Symbol log (tools/il/symlog.py, 9 min): leaf words: count = k3 temp #14 << 6 (0x380, a free-list reuse; scaling's count k3 #139,
   rotation's 0xe480 = #914), finishes = k3 temps #433..#445 << 5 | 1 (scaling pal finish #576), starts = field symbols #21..#31 << 5.
   Flip conditions (one number moving, others fixed): count temp t in [548, 791] (mod 1024); or pal finish temp in a wrap window;
   a uniform shift of all temps needs +534 or more. => the lever is which temp number the position count CSE temp gets (free list).
   Readout costs: keys.py 5 min, symlog 9 min, dump2 55 s (dump2 shows the five leaf words in the lowered compare: enough to score L v R).
11. CORRECTION (advisor, join by address sl0 x il0): leaves are count = k15 CSE temp #3086 (<<6 -> 0x380; rotation's #2962, scaling's
   #3211), finishes = k4 field symbols #2484 (position_track) / #2493 (paletted_position_track) (<<5|1, mod 2048), starts = k4 fields
   #23 / #29. Flip windows (linear model, keys exact): position count alone in [-490,-247] or [+534,+777] (mod 1024); fields alone
   >= +534 or <= -749. Not reachable by a small uniform shift; but numbering is not smooth: a1 (default AllocateType) moves the
   count word 0x0380 -> 0xf380 and the finishes 0x37a1 -> 0x3301, so texts anywhere in the function re-draw c.
12. Grades so far 3 (a2, m1 swap-statements measurement, m2 named unique_count measurement: 26, residue (3) back). Hook/IL readouts ~27.
13. Scorer: tools/ord.sh + tools/il/order.py (dump2 55 s): per block which operand the IL evaluates first + position/scaling L, R.
   Base a2: L>R in both (7a9b/3d0a, bb1b/3dea). a1: 615b/386a. operator== q1 (named bool local; VC10 88 v Ego_d 86) 731b/394a,
   q2 (nested ternary) = base, q3 (if/return) 731b/394a, q4 (?:) = base. ISNONZERO_ZYX off: 72db/392a. None reaches R>L.
14. VC10 (coordinator's odall10.py): C3DVectorWord::operator== variants 82/82/96/82/80/82/82/80, 74/79/71/73/88/82/82/82 -- none = Ego_d 86.
   GetBoneName: every spelling 14/27/6 (or 14/29/6, 14/34/6); Ego_d 16/27/6: the return line matches, the prolog line is 2 B longer
   (one more 4-byte frame slot: sub esp,0Ch v 8). Row under VC10: total 3297 v Ego_d 3872 (Ego_d is a later revision: loop compare
   lines 79 B v ours 32, separate CArray declarations); position compressed / uncompressed / if lines Ego_d 26/17/29 = rotation's.
