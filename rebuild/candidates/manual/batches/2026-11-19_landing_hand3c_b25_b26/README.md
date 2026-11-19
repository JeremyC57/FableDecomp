# hand3c b25 + b26 (2026-11-19, Account B): two rows, both RELOCATION_MATCH + test PASS, independently audited

## 0x00986210 CAIStateGroup_OpinionReactionRoot::CallingChildrenIsRunnable (b25)
- **Identity:** Ego_r 0x009A5D50 and FableWin 0x02C76EB0 spell it `?CallingChildrenIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ`, in ai_state_group_opinion_reaction_behaviours.obj. The Ego_d locals ptc_perception, ret_pcreature and scan_area sit in the inner block.
- **Template body in the TU.** The row TU defines the template body of GetPFirstThingInRoughArea<Bind4<..>, const CThingAICreature>.
  - The reason is that the template must be visible to be instantiated. It is byte-identical to the landed 0x009871C0, and graded from this TU it gives RELOCATION_MATCH at 0x009871C0.
  - Correction to the author's note: in Ego_d it comes AFTER the row, so it is not an object-order predecessor.
  - Without it the frame is wrong (sub esp,0x68 against retail's 0x5c).
- **Bracket:** one `// Bracket:` line, allowed by the 2026-11-19 bracket rule. Without it the row DIFFERs at 0x00986374..7A.
- **Purity:** retail_asm, through the shared GFFloatToLongCeil.h helper, which has its twin and dual-path test.
- **Header:** CAIStateGroup_OpinionReactionRoot.h gains a declaration-only opt-in, FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_CALLINGCHILDREN. hdrcheck2 reports every macro same, and all 14 landed users recheck RELOCATION_MATCH.
- **Audit follow-ups (non-blocking):**
  - The test does not check that IsNot runs before GetCreatureType.
  - The shared template body is now copied into two TUs and belongs under a body opt-in in CThingSearchTools.h.
  - Both are recorded for a later pass.

## 0x00AC7EB0 QuadricOptimiserInternals::COptimisedPrimitive::ComputeSelfContraction (b26)
- **Identity:** Ego_r 0x00C607F0 and FableWin 0x034C9560. The Ego_d locals are sub_contraction, vertex1, vertex2, tex_diff, normal_diff and cost.
- **The landed source is the AUDIT's fix, not the author's.**
  - The author's version squared each distance in place, marked FITTED, and claimed no other spelling matched. The audit found that false.
  - The landed source uses FableWin's /Od statement form instead, as recorded by the earlier hand3c author from FableWin: `cost = 0; cost += GFSqr(tex_diff)*Wt; cost += GFSqr(normal_diff)*Wn;`.
  - GFSqr uses C3DVector.h's existing `(n*n)` body (FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER + _SQUARE_GROUPED_PASS23), the same recipe as landed 0x00789CE0.
  - No FITTED line, no Inferred line, no header change.
  - The author's 11 probes all DIFFER as claimed, and the author's test passes on the fixed object.
- **Mutants:** 28 of 30 killed by the audit's mutants; the 2 survivors are equivalent (swapped difference, commutative Dot).
- **Caveat:** the FableWin GFSqr reading is a single read and cannot be repeated here, because FableWin.exe is not on this lane. If it is ever disproved, the audit's alt_bracket form (`(t*t)*W`, with a `// Bracket:` line) also gives RELOCATION_MATCH. A copy is in pending/2026-11-19_acctB_snapshot.

No name fixes.
