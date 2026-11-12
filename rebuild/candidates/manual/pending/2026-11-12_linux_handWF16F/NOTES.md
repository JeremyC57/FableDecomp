## 0x006D8F10

## 0x006D8F10 CHeroExperienceDef::Transfer -- DIFFER(693v692), wf16 author pass 1 (~90 grades/probes)
Ported onto shared headers (overlay opt-ins in hdr/README.md); PDB-true one-arg TransferVector/TransferMap/Transfer<float> (release
macros, CheckCRC("")) give the same bytes as the old draft. Residue is ONLY the ebp save placement: retail shrink-wraps `push ebp`
to 0x6D9064 / `pop ebp` to 0x6D91A3; ours saves ebp in the prologue (pop in both duplicated epilogues).
Tried (no change unless noted): pragma optimize s/y/t, flags /EHsc /Ob1 /G5 /G6 /G7 /Ox /Oa /GS /Ow /arch:SSE, QFE-4035 c2 (no wrap);
stats ref/pointer local (at first use, at top, in a block), block scopes, do{}while(0), for-once, forceinline helper over 8..13;
visible noinline GFSerialise map/vector bodies (stub probe); TransferVector/TransferMap/Transfer<T> as if/else, case order, explicit
default, `return` arms, unused traits locals; maps through Transfer<T> + CPersistTraits<map> partial spec; named 2-arg forms with "";
CPersistTraits<float> as explicit class spec (with/without DefaultVal, const float& out); traits temporary (value-init stores 0 -- wrong);
trailing return / empty for / opaque call; inline-defined Transfer; neighbour functions before/after; param rename; macro-expanded
bodies (lose the lea-before-CheckCRC: retail really uses inline functions).
Probe findings (VC7.1 RTM does shrink-wrap straight-line regions here): with our one-arg bodies the ebp push is sunk only when the
stat region is short (<=3-4 transfers) AND at least two transfers follow it (e.g. [prefix | S Inc S | Lock V] wraps, [.. | S Inc S | V]
never does; float transfers in the prefix help, a 4-map prefix or a CSE'd repeated last vector blocks it). Retail wraps a 6-transfer
region followed by ONE transfer, so retail's IR around the end/region must differ from ours at a level not yet found.
Best next idea: find a landed Def::Transfer with a mid-body shrink-wrapped push (grep catalog bytes for `55` + `68 0E D7 22 01` gave only
0x007846B0, not a Transfer) and copy its idiom; or reconstruct the real GFSerialiseMapBinaryIn/Out + VectorBinary bodies of this TU
(0x6DAE40..0x6DD580, landed) in the row file with #pragma auto_inline(off) (real bodies, not the stub probe); or test whether the
float transfers come from a different inline path (Ego_r also names ?TransferBinaryIn@?$CPersistTraits@M@@QAEMAAVCMemoryDataInputStream@@@Z).
Names: landed 0x006DD1C0 leaf "CHeroExperienceDef::ReadFromStream" and 0x006DC450 "WriteToStream" are wrong (dossier: GFSerialiseMapBinaryIn/Out<CHeroStatIncreaseData,EDamageAttribute,less<EDamageAttribute>>, Ego_r.pdb).

## 0x006D8F10 -- attempt 2 (wf16 author pass 2, ~30 grades/probes): still DIFFER(693v692), same residue (ebp save placement only)
New ideas tried, none moved the ebp save (all logged in ATTEMPTS.log; the overlay is back to its pass-1 state):
- flags /Op (worse: float save arm changes), /Zi /Ob2 /Oi /GX /GX- /EHa /Gy /GR- /Zp4 /Og /Ot /Gm (no change), /Oy- /O1 (worse);
- TransferVector body spelled with stream reference locals (685) or a 3-way switch with return arms (697);
- visible float CPersistTraits<float>::TransferBinaryIn body (inline noinline / auto_inline off; probe body via ReadFloat): no change;
- three-argument TransferMap(name, map, default): V() temporaries add code (931/1043); a null-reference default gives the same bytes;
- V transferred through a two-layer Transfer(name, vector&) -> TransferVector, or its own body, for V only or every vector: no change;
- empty user dtor on CPersistTraits (EH-edge idea): no change without EH; with /EHsc or /GX it puts Transfer<float> out of line (609).
Probe map of OUR compiler's shrink-wrap (q.py outputs w51..wa3.out): it never sinks the push when only ONE transfer follows the second
stat transfer, whatever that transfer is (V, M, F; any region length 2..6), and never for a 6-transfer region even with two transfers
after it ([S Inc Lock Lost Run S | Lock V], [.. | V V'], [.. | F V] all keep the prologue push). It does sink it for regions of 3..4
with two transfers after ([S Inc S | Lock V], [S Inc S | V Lock], [S Inc S | V V'], [S Inc Lock S | Lock V]). Prefix content
(no prefix, 4 floats, 10 floats) does not change that. Retail: region 6, one transfer after. So retail's IR differs in the
region AND/OR the tail at a level that is not inline-layering, flags, compiler build, EH, callee visibility or default arguments.
Only other retail `push ebp ; push ""` site in .text is 0x00784820 (an argument push in CTCCreatureGroupBoastingCrowd, not a save).
Best next idea: find in retail any other function (not only Transfer rows) whose callee-saved push sits mid-body with a long region
and a one-statement tail, and that is already landed (its source shows which spelling VC7.1 RTM wraps); or test whether the tail is a
real statement that compiled to nothing late (e.g. a dead store to a member/local removed after register allocation).

## 0x006D8F10 -- attempt 3 (wf16 author pass 3, ~45 grades/probes): still DIFFER(693v692), same residue (ebp save placement only)
Best source unchanged: differ/006d8f10.cpp (overlay unchanged). New ideas tried, none moved the ebp save:
- flags /GA /Gr /Gz /vd0 /J /QIfist (no change), /Za (src fail), /Os (178 bytes);
- last vector transfer written out at function level with return arms (g1/g2) or a reference local (g3): 689, push stays in prologue;
- visible SIDE-EFFECTING GFSerialise map/vector bodies as `template __declspec(noinline)` (j1..j3): no change
  (`#pragma auto_inline(off)` does NOT keep template instantiations out of line -- they inline; use __declspec(noinline) for probes);
- inline helper around the four combo maps (h1..h3): not inlined by /O2 (499/454);
- macro bodies with a reference local per transfer (n2): same residue -- so the inline-parameter temps are not the cause;
- body spellings grid (grid/: vector switch|if, map switch|switch+default|if-chain, float same): switch forms all give the same
  693 residue, if-forms change code; extra empty cases (MODE_CLEAR / text modes) and `case MODE_SAVE_BINARY: default:` -> no change;
- Transfer<float>: traits declared before CheckCRC / per-case block -> no change; traits temporaries -> 706;
- last transfer swapped to a lower-offset member (k1..k3: FallbackMultipliers / map / float): push stays in prologue.
Probe facts that correct attempt 2's map: our compiler DOES sink the ebp push with ONE transfer after the range when the ebp temp is a
different value: r1 (region interior = 4 floats; ebp = float +0x64 address, wrapped 0x6D8FFA..0x6D9145) and r2 (interior = 4 vectors;
ebp = FallbackMultipliers address). r3 ([S Inc S | V]) does not. So it is not "one transfer after never" -- it depends on which temp gets
ebp and what else is held in ebx across its range (in retail the 4 combo-map temps are held in ebx inside the ebp range).
Retail scan (capstone over manifest function starts, $W/sw_scan.txt): mid-body `push ebp` is common around LOOPS (e.g. landed
0x0082A2A0 RemoveAllDeadMarkers), but in straight-line Transfer code only 0x6D8F10 has it.
Best next idea: the remaining unexplained lever is c2's allocation order. Try (a) making the combo-map temps rematerialisable vs held
(e.g. combo maps transferred via a form whose arms recompute &member) to see whether ebx usage inside the range flips the decision, then
search for a spelling that keeps retail's bytes; (b) compile retail-neighbour TU content (the real GFSerialise* COMDAT bodies 0x6DAE40..
0x6DD580 with their real STLport map code, __declspec(noinline)) before Transfer -- c2 per-TU state is the last untested level.

## 0x006D8F10 -- attempt 4 (wf16f author, 149 grades/probes): RELOCATION_MATCH (strict headers), test PASS, payload.json / oracle.tsv written
Source: src_006d8f10.cpp, test: test_006d8f10.cpp, overlay: hdr/ (README.md lists the four opt-ins). No flags, no pragmas.
What made it match: the one-argument TransferVector<T>(vector&) switches over BOTH binary modes and ends in a braced unreachable
default, `default: { __assume(0); break; }` (opt-in FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NODEFAULT). The transfer's own bytes
are the same as the old `case LOAD / default` model (`sub eax,2 ; je`), but the dead-end default block survives to register
allocation, and that is what moves the ebp save out of the prologue.
How the residue was read (all in probes/, logged in ATTEMPTS.log):
- VC7.1 saves ebp around its range [first stat transfer .. head of the last vector] only if ebx (pushed before it) has no restore
  point of its own inside or before that range. In the retail shape ebx's last use is in the RunningHitsIncrease arms, so its
  restore could be hoisted to the head of the second stat transfer; the two ranges cross and ebp falls back to the prologue.
- Three things each give retail's exact push/pop placement in a probe: an ebx use after the last vector's arms (q2), an early
  `return` after the first ebx use and before the range (x1/x2/w2; not before the first use: w1, not inside the range: x3), and a
  dead-end block reachable from an ebx use (v2). Only the third can be code-free, and only in the vector transfers (the map and
  float switches keep their `dec eax ; jne` skip; an unreachable default there turns them 2-way: f1..g3).
- Spellings of the vector default: braced / do-while / if-wrapped / through an empty inline or an empty noreturn inline all match;
  a bare `__assume(0);`, `(__assume(0));` and `__assume(false);` do not (693, pruned before allocation). The repo idiom
  (`#ifdef _MSC_VER __assume(0); #endif`) is kept, inside braces.
- Needed with it: CPersistTraits<float> as its own class whose writer takes `const float&` (FableWin PDB; opt-in
  FABLE_RETAIL_CPERSISTTRAITS_FLOAT_CLASS). With the by-value writer of the earlier overlay any `__assume` in the function gives
  the float a frame home (710 bytes).
Not modelled (bytes unaffected): CPersistTraits<float>'s PDB constructor (DefaultVal = 0). Its store only disappears when the real
body of TransferBinaryIn 0x0040EFB0 is visible in the TU (probe c2/c3/c4: 693 -> same bytes as without the constructor); the class
is declared without the constructor instead of copying that body into the row.
Made-up names: none in the source (the opt-in macro names are descriptive). Probe-only names (ProbeOpaque, gProbe, ProbeWrap ...)
exist only under probes/.
Wrong landed names proven (NAME_FIXES.tsv): 0x006DD1C0 "CHeroExperienceDef::ReadFromStream" and 0x006DC450 "...WriteToStream" are
GFSerialiseMapBinaryIn / Out<CHeroStatIncreaseData,EDamageAttribute,less<EDamageAttribute>>; 0x006DBA50's leaf lacks its template
arguments (Ego_r.pdb + the call sites in this row).
Coordinator: the overlay CPersistContext.h still carries the pass-1 opt-in FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERMAP_BINARY_BODY.
The new vector opt-in is nested in FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY and leaves that body's default path as it
was, so nothing that already includes it changes. Not checked here: whether other Def::Transfer rows parked for a register-save
residue match with the braced default -- it is the first thing to try on them.

## 0x006D8F10 -- repair pass (wf16f, 3 grades): still RELOCATION_MATCH (default and strict headers), test PASS, hand-back rewritten
Code unchanged; only the header comment of src_006d8f10.cpp and the hand-back tables changed. Both verifier findings were real and are applied:
1. 0x00466A75 is GFSerialiseVectorBinaryOut<long>, not the landed made-up `GFSerialiseVectorBinaryOut_float`. Proof: FableWin.pdb
   ??$GFSerialiseVectorBinaryOut@J@@YIXAAVCDataOutputStream@@AAV?$vector@JV?$allocator@J@std@@@std@@@Z; the body writes the count and
   each element through CDataOutputStream::WriteSLONG 0x00993EB0 (0x00466A86, 0x00466A98) and never calls WriteFloat 0x00993EE0;
   labels_chunk190_195_corrections_2026-10-19.tsv; the row's three vector<long> members (Ego_r layout) at 0x006D8F35 / 0x006D8F60 /
   0x006D8F8B, each paired with the landed <long> reader 0x00466AA7. The row's callee comment now carries that proof, and
   NAME_FIXES.tsv has the 0x00466A75 entry (wrong spellings listed: the landed definition, the address-suffixed declaration in
   0x00466A47, `GFSerialiseVectorBinaryOutLong` in 0x007B1F10 / 0x0074C5F0, the manifest / catalog <float>).
2. NAME_FIXES.tsv now also has 0x006DC350 / 0x006DCFA0 (GFSerialiseMapBinaryOut / In<CComboMultiplierData,long,less<long>>) and
   0x006DC550 / 0x006DD580 (GFSerialiseVectorBinaryOut / In<CAttackHistoryCombo>): their landed `// Leaf:` lines are cut off after
   the template name. Full mangled names are in both Ego_r.pdb and FableWin.pdb; call sites 0x006D8FE9 / 0x006D8FF5 / 0x006D91AB /
   0x006D91B9 pass &ComboMultiplierMap (+0x58) and &AttackHistoryCombos (+0xA8).
Extra check: the row's own object file imports exactly those PDB symbols (repair/row_externals.txt, from the graded .obj), so every
correct_symbol in NAME_FIXES.tsv is what this row links against. LABELS.tsv now lists the ten proven GFSerialise callees.
Verifier findings not applied: none.

