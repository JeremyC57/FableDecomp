# DECISIONS 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams (2073 B), AUTH14, 2026-11-22
This pass measures only: two grades, one forced readout (the advisor asked for it) and a decision table. No cost hunt, no frequency hunt.
- W = S/w22/auth14/008e18d0. Overlay W/hdr = AUTH11's: AUTH10's IMPLICIT_DTOR opt-in plus AUTH11's add-only GETTC_EGOD_WRAPPER.
  Its orig/ equals HEAD.
- Log: W/LOG.md (every grade and readout was pre-registered).
- Readouts: AUTH10's force_host.py / fv.sh. A readout is never a grade.

## Headline findings
1. **The recorded e4 baseline of 2044v2073 is a no-overlay figure.**
   - e4 in its own configuration (/Gr /d2inlT65, the IMPLICIT_DTOR overlay) grades 2165v2073. That holds with AUTH11's overlay and with AUTH10's.
   - e4 with no overlay grades 2044v2073.
   - Header drift since AUTH10's base (4b50840a07) is ruled out. CCountedPointer.h is in the TU, but each change in it sits under FONTBANK opt-ins this row does not define. The other two changed headers are not in the TU.
2. **C5 changes no bytes:** e5 and e5n have identical leaf disassembly (e5.q.out == e5n.q.out). C5 is still live in e5:
   - pp/e5.i holds the wrapper body (overlay CThing.h resolved). pp/e5n.i does not.
   - The e5 readout has Find and GFDebugPDynamicCast row decisions. The e5n readout has neither.
   - Every row call/expand result is the same in both.
3. **Only two forced sites, not four.** e4 with only two decisions forced is a RELOCATION_MATCH:
   - the decisions: loop-1 ~CParticleUpdateParameters -> call, and loop-2 LowerBound -> call;
   - the run: fv.sh with W/force2.json (AUTH10's force1.json minus the two __uninitialized_copy entries), W/hdr, /Gr /d2inlT65;
   - the check: gobj.py / V.mask, in W/e4.f.gobj.txt.
   - mdiff reports 14 cells, all mask cells: 1.0f pushes and stores, g_CRCTable and CRC-string displacements, two vtable stores, and one `or edx,-1`. Each is `<A>` against a literal.
   - So the two sky remainder copies (AUTH10's k3 / k5) are byte-neutral. They sit on a dead path (section 3).

## 1. Grades (stock VC7.1, q.py --flags /Gr /d2inlT65, overlay W/hdr, no hook)
| text | grade | size delta to retail |
|---|---|---|
| e5 (C1..C5) | **DIFFER(1944v2073)** | -129 |
| e5n (C1..C4, C5 off) | **DIFFER(1944v2073)** | -129 |
| e4 baseline, same overlay (also AUTH10's overlay) | DIFFER(2165v2073) | +92 |
| e4, no overlay (= the 2044 on record) | DIFFER(2044v2073) | -29 |

- Size does not rank these texts. None of them matches.
- For reference, AUTH11's earlier one grade of e5 at stock flags (no /Gr, no /d2inlT) was 1948v2073.

## 2. Rules read from the records
- **Always-inlined, no score, iff c1 - 4 <= S** (c1 <= 13 at S9). 0 violations in e4, e5, e5n and AUTH11's e4i (all T65 S9).
- **Scored sites:** c2 expands iff score < T.
  - Ours at T65: 65 calls and 63 expands.
  - AUTH12's p6s9.json (stock flags, S9): T is **74**, with 73 expanded and 74 called.
  - So AUTH11's "calls when score > T" is off by one, and stock T is 74.
- **Footnote:** `_Destroy<pair<ulong,C3DVector>*>` (sz 1, c1 -14) has no score but r=0. It is an empty body, not a retail "call". It is left out below.

## 3. Decision table
Each readout cell is c1 / f / score / ours, with A = always-inlined.
- e4 is AUTH11 inl/e4.json and e5 is AUTH11 inl/e5.json. e5n is W/inl/e5n.json.
- All three use /Gr /d2inlT65 and the same overlay.
- Retail VAs are in the row unless noted.
- **"n/a COMDAT"** means retail calls the parent, so the decision lives in that body. EQ / NEQ is from AUTH10 proof/leafrel.txt.

### ~CParticleUpdateParameters<ulong,ulong> (forced site 1; still needed)
| occurrence | retail | e4 | e5 | e5n |
|---|---|---|---|---|
| loop 1 | **CALL** 0x006E2500 @0x8E1C3A | 29/2.5/23 expand | 29/1.25/27 expand | 29/1.25/27 expand |
| sky | EXPAND 0x8E2098..0x8E20D9 | 29/12.5/20 expand | 29/4.5/21 expand | 29/4.5/21 expand |

The sky span holds:
- the ParamDesc element loop 0x8E20A4 calling ~CBaseClass;
- free at 0x8E20BC and 0x8E20CD;
- the base dtor at 0x8E20D9.

Our c1 of 29 is the small body. Our dtor COMDAT calls the inner ~vector (next table). Retail's 0x006E2500 expands it (NEQ). So retail's cost for this dtor is larger than 29, and these scores are not retail's.

### ~vector<pair<ulong,CParticleUpdateParamDesc<ulong,ulong>>> (other callee: inside the dtor)
| occurrence | retail | e4 | e5 | e5n |
|---|---|---|---|---|
| inside the dtor COMDAT | EXPAND in 0x006E2500 (loop 0x6E2510..0x6E251D, free 0x6E2527) | 47/1.0/65 **call** | 47/1.0/65 call | 47/1.0/65 call |
| loop 1 (only because we expand the dtor) | ours only: retail is n/a COMDAT 0x006E2500 (NEQ) | 47/2.5/52 expand | 47/1.25/60 expand | 47/1.25/60 expand |
| sky | EXPAND 0x8E2098..0x8E20C1 | 47/12.5/45 expand | 47/4.5/48 expand | 47/4.5/48 expand |

### CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (forced site 2; still needed in e4)
| occurrence | retail | e4 | e5 | e5n |
|---|---|---|---|---|
| loop 1 | CALL 0x0040F020 @0x8E1A83 | 52/1.25/82 call | 52/1.0/88 call | 52/0.625/95 call |
| loop 2 | **CALL** 0x0040F020 @0x8E1D58 | 52/6.25/63 **expand** | 52/1.875/74 call | 52/1.875/74 call |
| sky | EXPAND binary search 0x8E1DC9..0x8E1E0B | 52/6.25/63 expand | 52/2.25/71 **call** | 52/2.25/71 **call** |

COMDAT 0x0040F020 is EQ.

### Callees on the LowerBound path
- **Find** (e5 only, the C5 path): 26/1.0/26, 26/1.875/21 and 26/2.25/21, all expand.
  - Retail expands it at all three occurrences.
  - Its end/compare tail is inline after each LB: loop 1 at 0x8E1A8B..0x8E1A96, loop 2 after 0x8E1D58, sky at 0x8E1E00..0x8E1E0B. There is no Find call.
- **bitset<274>::test:** expanded at all three occurrences in every text. Scores are e4 46/40/40 and e5 / e5n 54/43/42.
  - Retail runs it inline as `test ch,8` at 0x8E1A6C, 0x8E1D41 and 0x8E1DC4.
- **HasTC, GFDebugPDynamicCast:** always-inlined (A).

### vector<pair<ulong,C3DVector>>::_M_insert_overflow (parent of forced site 3)
| occurrence | retail | e4 | e5 | e5n |
|---|---|---|---|---|
| loop 1 Insert #1 | CALL 0x0076ADA0 @0x8E1B94 | 71/1.25/66 call | 71/0.625/77 call | 71/0.625/77 call |
| loop 1 Insert #2 | CALL 0x0076ADA0 @0x8E1BE8 | 71/1.25/66 call | 71/0.625/77 call | 71/0.625/77 call |
| sky Insert #1 | EXPAND 0x8E1EA0..0x8E1F4C (malloc 0x8E1ECC, free 0x8E1F44) | 71/6.25/51 expand | 71/2.25/57 expand | 71/2.25/57 expand |
| sky Insert #2 | EXPAND 0x8E1F91..0x8E2040 (malloc 0x8E1FBD, free 0x8E2038) | 71/6.25/51 expand | 71/2.25/57 expand | 71/2.25/57 expand |

COMDAT 0x0076ADA0 is EQ.

### __uninitialized_copy<pair<ulong,C3DVector>*> (forced site 3: AUTH10's k3 / k5 are the sky remainders; NOT needed)
| occurrence | retail | e4 | e5 | e5n |
|---|---|---|---|---|
| sky Insert #1, prefix copy | EXPAND loop 0x8E1EE0..0x8E1F17 | 51/6.25/63 expand | 51/2.25/71 **call** | 51/2.25/71 **call** |
| sky Insert #1, remainder | none: dead path, byte-neutral (forced readout) | 51/3.125/68 call | 51/1.125/84 call | 51/1.125/84 call |
| sky Insert #2, prefix copy | EXPAND loop 0x8E1FE0..0x8E2007 | 51/6.25/63 expand | 51/2.25/71 **call** | 51/2.25/71 **call** |
| sky Insert #2, remainder | none: dead path, byte-neutral (forced readout) | 51/3.125/68 call | 51/1.125/84 call | 51/1.125/84 call |
| in COMDAT 0x0076ADA0, prefix | CALL 0x00768FC0 @0x0076ADEB (outside the row; EQ) | 51/1.0/88 call | same | same |
| in COMDAT 0x0076ADA0, remainder | CALL 0x00768FC0 @0x0076AE46 (outside the row; EQ) | 51/0.5/100 call | same | same |

**The dead path:**
- push_back passes `atEnd = true`, so the expanded overflow's `if(!atEnd)` copy is dead.
- Retail's sky Inserts each have one copy loop and no call. e4's sky call count equals retail's (11).
- The forced readout matches with both remainders left "call".
- e5 / e5n leave them "call" too. That they are neutral in e5 is inferred from the same dead path, not measured.
- `__uninitialized_fill_n` in the sky is dead for the same kind of reason (fillLength == 1, so the `else` arm is dead): e4 58/58 expand, e5 72/72 call. In COMDAT 0x0076ADA0 it is called at 0x0076AE2A, EQ.

### Other callees inside the Insert sites (all expanded everywhere, in retail and in every readout)
- **Insert** is always-inlined (A, c1 10).
- **push_back:**
  - Scores are e4 47/47/40/40 and e5 / e5n 55/55/43/43.
  - Retail tests capacity inline at 0x8E1B9D and 0x8E1F4F. Each Insert #1 test folds on the fresh, empty vector.
- **_Construct** (scores 13..25) and **allocate** (sky 12 in e4, 14 in e5): expanded. Retail calls malloc inline in the sky.
- **Always-inlined (A):** _M_clear, deallocate, ~CVectorMap, ~CArray, ~_Vector_base, ~vector<pair<ulong,C3DVector>> (c1 11), ??_G and ~CParticleUpdateParamDesc.

### Not at the four sites, but it bears on the windows: CCRC::Calc
- Retail expands all four occurrences: CRC loops at 0x8E1AA1 and 0x8E1B00 (loop 1), and 0x8E1E40 and 0x8E1E70 (sky). Retail has no Calc call.
- e4 expands all four (61/61/53/53).
- **e5 and e5n call both loop-1 Calcs (71/71) and expand the sky's (57/57).** That is a wrong decision the C4 idiom brings in at T65.

## 4. Classification (in-row occurrences)
COMDAT constraints follow from the pick-any rule (nf201). This obj's COMDAT copies must equal retail's bodies, whichever obj retail's came from:
- **0x0076ADA0 (EQ):** ucopy must stay CALLED inside the overflow body.
- **0x006E2500 (NEQ in every text):** the dtor copy must EXPAND its inner ~vector. That requirement stands independently of the four sites.

| callee | class | retail | how our scores order against retail's split (call iff score >= T) |
|---|---|---|---|
| ~CParticleUpdateParameters | **MIXED** | call loop 1, expand sky | Ordered in every text: the loop-1 score is above the sky's (e4 23 > 20, e5 27 > 21). The window would be (21, 27] in e5, but the scores come from our small body (c1 29). Retail's body is larger (nf201: inner ~vector expanded), so its real scores are unknown. |
| ~vector<pair<ulong,ParamDesc>> | **UNIFORM** expand (sky, and the dtor body by nf201) | expand | The coldest occurrence is the dtor COMDAT body: f 1.0, score 65, which calls at T65. It would fit with c1 <= 13 (always-inlined; ours is 47), or a score <= T-1 there (T >= 66 at this score). Expanding it makes the dtor bigger, which moves the dtor row above. |
| LowerBound | **MIXED** | call loop 1, call loop 2, expand sky | e4 is not separable: loop 2 and sky are the same record (63/63), which is the frequency problem AUTH10 described. e5 and e5n are ordered (88 or 95 / 74 / 71), window T in (71, 74]. |
| Find (e5 only) | UNIFORM expand | expand | Scores 26/21/21 are already expanded. |
| _M_insert_overflow | **MIXED** | call loop 1 x2, expand sky x2 | Ordered in every text: e4 66 / 51, window (51, 66]; e5 and e5n 77 / 57, window (57, 77]. |
| __uninitialized_copy | **UNIFORM in the row** (expand at both visible prefix copies; the remainders are byte-neutral) | expand (in the row) / call (COMDAT, nf201) | Ordered: expand at 63 (e4) or 71 (e5), call at 88 / 100 in the COMDAT. Window (63, 88] in e4, (71, 88] in e5. The always-inline cost (c1 <= 13; ours 51) is excluded, because it would expand 0x0076ADA0's two calls. So this is a threshold question, not a cost one. |
| push_back, _Construct, allocate, bitset::test | UNIFORM expand | expand | Already expanded in every text. |
| (CCRC::Calc, not a site) | UNIFORM expand | expand | e4 expands all four. In e5 the loop-1 Calcs score 71, so it needs T > 71. |

**Do the windows intersect?** The remainder copies are no constraint (forced readout).
- **e4: no.**
  - LowerBound's tied 63/63 record has no window at all.
  - Even without it, ovf's (51, 66] against the inner ~vector's T >= 66 leaves only T = 66.
  - e4 was never graded at T66. AUTH10 graded e3 at T64..66 and got 2165.
- **e5 / e5n: yes, to first order, at T in {72, 73, 74}.**
  - The windows: LowerBound (71, 74]; ovf (57, 77]; ucopy (71, 88]; Calc T > 71; the inner ~vector in the dtor body T >= 66.
  - **The top of the window is c2's stock T 74.**
  - Caveat: this readout is at T65. Scores do not depend on T, but bodies compiled earlier do. At T >= 66 the dtor body would expand its inner ~vector, so the dtor's c1 and its two row scores change. Whether loop 1 then calls while the sky expands is not predicted here.
  - The one existing grade near this window is AUTH11's stock-flag e5, 1948v2073. That build had no /Gr: T74, but a different calling convention.
  - **The untested configuration is e5 / e5n with /Gr and no /d2inlT** (stock T74). It is the next candidate grade. Not run here.
- **MIXED callees and the frequency hunt:**
  - In e5 / e5n, the score order of every MIXED callee agrees with retail's split. None is inverted.
  - A frequency hunt is needed only if the /Gr-at-T74 candidate fails, or for e4's LowerBound tie.
  - The dtor stays open either way: its scores depend on a body cost we do not reproduce yet (the nf201 requirement).

## 5. Files
- **Texts and grades:** W/e4.cpp, e5.cpp and e5n.cpp; hdr/ and orig/; grades.txt and *.q.out.
- **Preprocess witness:** pp/ (pp.py, e5.i, e5n.i).
- **The forced readout:** force2.json and fv2.out; e4.f.obj, e4.f.json and e4.f.md; e4.f.gobj.txt.
- **Readouts and extractors:** inl/e5n.json; inl/rowall.py (row decisions plus the always-inline check) and inl/fnbody.py (decisions inside a named body).
- **Elsewhere:**
  - e4 / e5 readouts are AUTH11's: S/w22/auth11/008e18d0/inl/e4.json and e5.json.
  - Retail listing: S/w22/auth11/008e18d0/retail.dis.
  - Stock-T record: S/w22/auth12/00cf4ec0/inl/p6s9.json.
