# Advisor decisions for Account B, 2026-11-20 (owner delegation)

The owner said: "any decisions or review for me let the expert advisor decide." These are the advisor's decisions under that delegation.
They are not owner rulings.

## Decided (Account B's items)
1. **`inline __declspec(noinline)` IsValid in 0x00D3A4A0 and 0x00EB3BA0: accepted; the rows stay landed.**
   - The cause is an inline function that retail called while its body was visible to the compiler. That is an inline decision; the CSE is
     only the symptom.
   - The row is marked `// Inline-budget:` (relabelled 2026-11-20).
   - Follow-up: ladder step 2 was never shown, a plain `inline` body plus a `/d2inlT` / `/d2inlS` flag. The discriminator is the TU's IsValid
     COMDAT: it should equal retail 0x00CD23B9 (9 bytes, not 11). If a flag gives the row match and a 9/9 COMDAT, it replaces the device.
   - Portability: most landed inline-budget rows write the declspec bare, without `#ifdef _MSC_VER` (24 bare uses among the 371 marked rows).
     Wrapping them is backlog, not part of this decision.
2. **b14's message-less `throw CGenericException();` (0x009BE340): accepted as labelled `// Inferred:`.** Do not invent a message string.
3. **0x009BE3C0 CDisplayManager::IsAntiAliasingModeValid: catalogue it and land it as its own row,** through
   rebuild/manifest/boundary-starts.tsv, using the text of the same-TU copy in 0x009BF7E0. Check Account A's CDisplayManager claims first.
   Queued.
4. **The `#ifndef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER` guard round EEngine2DLayer (0x0041AFA0): accepted as an
   interim.** One definition of the enum is backlog, because it needs all 1,390 includers regraded.
5. **Model-only tests (0x00589A30, 0x00A4D920, the GetRootsNumerically owners): accepted as landed.** Real-object tests are backlog.

## Recommendations for Account A (items released to A at B's wind-down; A decides)
- **CVsyncCallbackTimer header contract (A's RP1):** option A (nested CPendingQuery opt-in plus method declarations), then option B (layout
  opt-in with fable_containers.h) after the calibration probe; never option C.
- **CSoundEventManager (libpk/LHAudioEventPosition.h v libpk/CSoundSystem.h):** the PDB layout decides, and one header owns the class.
- **hand3c3 b12's CRGBCOLOUR_INTERPOLATE_FORCEINLINE:** admissible under the owner's 2026-11-08 inline-budget decision, if marked
  `// Inline-budget:` and a flag attempt is recorded.
- **Navedge 0x00A78800:** the loop-step text wins on the ladder (plain C++ before a pragma), unless FableWin's line records put the step in
  the loop header.
- **reports b6 (0x00B0A140):** A's call.

## Not decided
The duplicate-symbol and b30 wording items need their facts pulled from the session log first.

## Process requirements from these decisions (p25 series)
- **Device minimality.** Every row that uses an inline control states a per-device ablation in its README: each device removed in turn, and
  the grade without it. Owner condition: "the smallest that matches".
- **Ladder.** A plain inline body plus a per-row flag is tried before a source-level `noinline` / `forceinline` / pragma.

## 0x00E72630 CDF_ThreeSuits::Main: a dead conditional temporary (2026-11-20)
- **Ruling: admissible as `// Inferred:`, never as `// Inline-budget:`.** It is a fitted statement, not an inline split.
- **Why:** retail's entry has `xor ebp,ebp ; mov [esp+3Ch],ebp`, the home and initialisation of a conditional-temporary flag. The live form of
  the same store is in landed 0x00E71F10 / 0x00E70BC0. Ego_d's 36-byte prolog shows the later source revision had no conditional temporary;
  that does not show retail had none. Landed 0x00DA0230 is the same case.
- **First, one probe:** a conditional temporary inside an inline body that Main expands. If its flag initialisation lands at function entry,
  check Main's expanded inline bodies for the real carrier.
- **Form:** the matching form that adds no line Ego_d lacks, uses only calls and literals already live in the function, and has the fewest
  tokens.
- **Required:** the comment states that the bytes prove a conditional temporary existed and constrain neither its content nor its placement.
  Mutants of the dead line are reported separately. No new relocation or binding.

## 0x00E97070 CV_HeroDollsScript::CBS_Teacher::Main: a new dead statement (2026-11-20)
- **Ruling: a new dead statement that Ego_d lacks may land as `// Inferred:`.** Landed 0x00DA0230 is already such a statement.
- **Cheaper forms come first:** the probe and the inline-body check; every existing dead position (Ego_d lines with /Od bytes but no retail
  bytes); placement at an inner endless-loop tail before "after a break"; and operands already live in the function, with an empty body.
- **Label:** all four dead-conditional-temporary rows (0x00DA0230, 0x00E72630, 0x00E54E90, 0x00E97070) start their label with
  `// Inferred: dead conditional temporary --`.
- **Lead:** FableWin's callee list (debug build) could name the real construct. It is not in pending/2026-11-19_fablewin_callees_for_B.tsv;
  Account A was asked.

## 0x00E9E420 CV_IntroductionToTrophiesScript::CBriarRose::Main: a redundant second read (2026-11-20)
- **Evidence:** the trophy test is `mov al,[eax+49h]` at 0x00E9F3FE, and `mov ecx,esi` is hoisted above the intro test's jne at 0x00E9F234.
  Ours gives `mov cl` with no hoist; about 40 spellings of the condition keep cl. A byte read once is a code-gen temporary and takes cl,
  because the base pointer is live at the load. A byte read twice, with a merge that survives to c2, becomes a global candidate and takes al.
  Only a second read of DoneBriarTrophySpeech at the blather arm's first T-check, whose two outcomes cross-jump, matches.
- **Ruling: it may land as `// Inferred: redundant second read --`, on the 0x00E97070 terms.** It is plain C++ that uses only a member and
  a call the function already has: no flag, pragma, device or invented type, which separates it from 0x00D01630.
- **Required:**
  - the smallest matching spelling (bare ternary, else `==true`), with every spelling in the ablation;
  - an Ego_d line-size check of that T-check line, citing the line on a surplus, else the label says "retail-only, not in Ego_d";
  - the label cites both retail sites, says the bytes fix the read's position and merge but not its spelling, and gives DIFFER(5923v5915)
    without it;
  - arm-mutants that are unreachable at run time are listed as such, not as killed, and must still fail the byte grade.

## 0x005C2118 CPersistTraits<NInventory::CItem>::TransferIn: a header forceinline against a respelled call (2026-11-20)
- **Case:** retail expands the one-argument `vector<long>::resize(n)` into its call to resize(n, const long&) 0x004428F0. `resize(n, 0)`
  matches with no device; `resize(n)` needs a new FABLE_FORCEINLINE contract in fable_containers.h. Ego_d line 240 is 13 B (one argument).
- **Ruling:** the ladder ranks devices for a given source. It does not license respelling a call against the evidence to avoid a device.
  Keep `resize(n)` if three checks hold:
  1. at /Od, resize(n) gives Ego_d's 13 B and resize(n, 0) doesn't (if they don't discriminate, land resize(n, 0));
  2. a per-row /d2inlS flag doesn't reach the match (if it does, the flag replaces the header device);
  3. retail has a real one-argument resize function somewhere (if none, forceinline is the wrong model; land resize(n, 0)).
- **Required:** line 2 names every device, including the CITEM_TRANSFERIN opt-in that makes ~CItem implicit. If the device lands, the
  README notes that resize(n, 0) is byte-equal with no device.

## 0x00B24850 CEngine::SetDefaultRenderStates: a dead named scalar and fitted assert arms (2026-11-20)
- **Case:** the last residue is the bookmark code in the 38 inline SetRenderState expansions.
  - The test order (s/a) moves only with one unused function-level named scalar, such as a discarded `GetVertexShaderVersion()` result.
  - The load order (C/B) moves only with the count of dead FABLE_RETAIL_ASSERT arms: 5..13 match.
  - About 150 natural spellings fail.
  - This contradicts the fable_assert.h note from 0x00BB3E80, which says arms without an inline call move the test order only.
- **Ruling:** both devices may land as `// Inferred:`. They are plain C++ with no flag or inline control, and fitted arm counts are already
  landed (0x00BB3E80). Because the fit is loose, two checks block the batch first:
  1. Identify Ego_d line 664 (34 B, no local): grade no-local forms of that size, e.g. the call inside an assert condition or an `if` with a
     compiled-out body. A form that matches replaces the local. Also diff the /Od locals against Ego_d's.
  2. The final strict vland runs with no header overlay.
- **Labels if the local stays:**
  - The local's label cites the discarded call 0x009BEDE0 and the DIFFER grade without it. It says the bytes prove one more named scalar
    but not its name, type or initialiser, that it is not in Ego_d, and that the name is descriptive.
  - Five arms, the bottom of the window. The label says the count is fitted to 5..13 and the conditions are unconstrained, and cites
    0x00B3B030 / 0x00B33B50 for 1100.
  - Byte-neutral mutants are listed separately.
- **Also:** the 0x00BB3E80 mechanism is unverified; a probe and a FINDINGS entry follow. The CRenderStateManager.h EARLY_RETURN brace bug
  was landed as its own fix commit.

## 0x00A1E5C0 CNavigatorAStar::CalculateAndMoveToPosProximity: a reference local not in Ego_d (2026-11-20)
- **Case:** with Ego_d's block locals ({one_past_begin, next_section}, {nav_layer, it}) the row is RELOCATION_MATCH only if the draft's
  `CNavWayPoint& last = Route.back();` stays. Ego_d's new_dest_pos block has no `last`. Without it the expanded IsRouteClear's x87 multiply
  order returns at 0x00A1F65A / 0x00A1F9D1, along with a register swap at 0x00A1E81B. A dead scalar in its place fixes only the swap.
- **Ruling:** it may stay as `// Inferred:`. It is the mildest fit so far: a live named reference to an expression the function already
  evaluates, with no flag, type or helper invented.
  - First, calibrate the block at /Od against Ego_d's line sizes. If Ego_d does not fit three `Route.back()` calls, a no-local spelling that
    does fit and matches replaces `last`.
  - The label cites the three retail sites and says the bytes prove one evaluation held in a named local, not its name.
- **Blocking:** the draft's `#pragma auto_inline(off)` round the same-TU 0x00A1A1A0 body is an inline device. Line 2 must name it with its
  ablation, which also tries a per-row flag and a declaration-only callee (if declaration-only is byte-equal, drop the body and the pragma).

## 0x00E5E350 CV_BodyGuardScript::HenchmanCommentary: a mid-body inline_depth region (2026-11-20)
- **Case:** retail calls ~CTimer out of line at one exit only (0x00E5E580/8D). Every return's dtors score alike, so no /d2inlT or /d2inlS
  value splits them; the draft's `#pragma inline_depth(0)` region round that one terminate check gives RELOCATION_MATCH.
- **Ruling:** admissible in class. EXTRA_RULES.md's 2026-11-08 owner decision allows a source-level `#pragma inline_depth` scoped to the
  affected code (preference 3), and landed rows use the same mid-body pattern (e.g. 0x00E6AD20). A per-site region can fit any
  expanded/called pattern, so:
  - **First, one bounded plain-C++ round:** find the frequency at which ~CTimer flips to called, grade natural nestings that give the site
    that frequency, and compare /Od line sizes in the block with Ego_d's.
  - **If the region stays:** it encloses one statement only. Line 2 lists everything it suppresses and says the region is fitted and
    retail's construct is unproven.
- **Its dead conditional temporary** is the fifth row under that ruling (after 0x00DA0230, 0x00E72630, 0x00E54E90, 0x00E97070) and takes
  the same label prefix.
- **Not reopened by this:** 0x00D01630, whose open part needs an expansion forced, not suppressed.

## 0x00AD9980 CVertexGroupBlockOptimiser::ReorderInput: a TU-mate copy's contract decides between byte-equal variants (2026-11-20)
- **Case:** the row grades RELOCATION_MATCH with plain STLP_TREE_CONSTFIND (the landed RemoveAllSubsets 0x00AD9770 set) and with
  STLP_TREE_CONSTFIND_NOINLINE (the landed IsSubsetOf 0x00AD8C30 set). The row carries a `// Fidelity:` copy of IsSubsetOf. In the plain
  variant that copy's COMDAT differs from retail 0x00AD8C30; in the NOINLINE variant p25H reports it byte-equal.
- **Ruling:** land the NOINLINE variant once its COMDAT is confirmed equal. The ladder ranks devices for the row's own residue; it does not
  license compiling a copied TU-mate under a different contract from its landed row. Line 2 names the device, says the row's grade is
  unchanged without it, and cites the copy's COMDAT evidence.
- **Follow-up:** regrade landed 0x00AD9770 with NOINLINE. If it still matches, it is a re-port item for A; if it breaks, the define is a
  per-site fact, not a TU-wide one.

## 0x00A58B50 NBBBMaths::CSphere::GetFirstIntersectionWithTriangle: fitted helper spellings on existing opt-ins (2026-11-20)
- **Case:** RELOCATION_MATCH (/Gr) after the same-TU IsPointInsideTriangleInThePlane is defined above the row with its landed text, using
  Ego_d's locals. Three fitted items, all plain C++ or existing opt-ins:
  - c_a / c_c through GFSqr (PDB-named; inline products DIFFER by 2 and 10 lines);
  - GFSqr's `(n*n)` body via two existing gates;
  - FABLE_RETAIL_C3DVECTOR.
- **Ruling:** may land. The match survives 1..16 dummy locals, so it is not a symbol-number knife edge.
- **Checks, done before landing:**
  - The Fidelity copy's COMDAT is byte-equal to retail 0x00A58A60 (the address is the helper's own, not the sibling 0x00A593F0).
  - Line 2 names the header contracts in place of "none".
  - The GFSqr label cites Ego_d's corroboration: c_c's line is 34 B longer than c_a's, as the call form's are.
  - `(n*n)` stays FITTED.
- **Not blocking:** regrade the parked 0x00A4EFC0 with this row's C3DVector set.

## 0x006F2DE0 CGetBestAttackerForProng::operator(): a shared anonymous helper's local type (2026-11-20)
- **Case:** the row needs GetPreferredCombatDistanceForAttacker's local as `float distance`: Ego_d type 0x40, and `double` gives
  DIFFER(379v375). The landed 0x006E89B0 carries an `// Inferred:` `double`; with `float` it regrades DIFFER(344v344).
- **Ruling:** `float` stands, because Ego_d's type and this row's expansion are two sources and the landed `double` is a fit. Before landing,
  compare the helper's emitted out-of-line copy with retail's copy if one exists:
  - equal: land, and queue 0x006E89B0 as B's re-port;
  - different: one bounded sweep for a spelling that satisfies both;
  - no retail copy: land `float` with the conflict stated.
- **Labels:** the helper here is not verbatim, so it gets `// Inferred:`, not `// Fidelity:`. Line 2 says how the helper expands.

## 0x00763060 NShape::FractureShape (p25L): unlanded helper bodies; a fitted reference local
- **Question (a).** May a row carry the unlanded DIFFER drafts of its same-TU helpers 0x00760020 IsLineIntersectingWithOthersInList and 0x00760190 IsTriangleWithPointsInside? With those bodies in the TU the row is a byte MATCH (frame 0x5c, and the free of the duplicate has no null test). With trivial noinline bodies it is DIFFER(1142v1146); without them, 1151v1146.
  - **Ruling: no.** The helpers' contents are load-bearing, so they match and land first, as their own rows (both now reserved for p25L). The row then carries them as verbatim Fidelity copies.
  - Same reasoning as 0x005CA68E: no strong definitions without a proof line. The 0x006E89B0 float/double debt shows what landing on a body that later changes costs.
- **Question (b).** Is a named `const C3DVector& anchor=*it_anchor++;` acceptable as `// Inferred:`? Ego_d has no such local, and with Ego_d's spelling the row is DIFFER(1132v1146).
  - **Ruling: no.** Ego_d records reference locals in this very function (`result_point`), so the absent local is evidence against it. Inferred covers what the oracle leaves open, not what it contradicts.
  - Re-measure Ego_d's plain spelling once the helpers have matched; a residue measured against provisional helper text may move.
- **Accepted as Inferred, with an ablation line:** `return;` at the end of the found_appropriate block, backed by Ego_d line 280 being 143 B.
- **Must be fixed before a batch:**
  - C2DLineF::IntersectsWith (0x00A56C80) becomes an add-only header opt-in, not a local extern.
  - The functor members disagree (pointers in landed 0x0075FE30, references in the draft); the PDB types decide which side is wrong.
- **Re-measure, with both helpers matched as same-TU copies.** Ego_d's plain spelling in Ego_d's line order is still DIFFER(1132v1146).
  - The residue: VC7.1 increments at the loop top, where retail peels the first increment into the preheader and increments after the call.
  - The only matching source keeps `a` before `it_anchor` and the `anchor` local, which contradicts Ego_d in three places.
  - **Ruling stands.** Three cheap checks come before parking: Ego_r/FableWin line records, Ego_d's TPI iterator type, and the c2 symbol kit on the post-increment spelling.
  - The "TLC text differs from Ego_d" idea is a hypothesis only. Any FINDINGS line says "not a symbol tie within 1..32 literal dummies".
- **Helpers 0x00760020 and 0x00760190 match** on today's headers, and land as their own rows.
  - 0x00760190's `edge` placement is `// Inferred:` with its 314v340 ablation and /Od sizes against Ego_d line 157.

## 0x008E18D0 UpdateLightningBeams (p25N): inline split, an implicit destructor, and do{}while(0)
- **Ruling: no device and no loop respelling yet.** The frequency thresholds p25N derived rest on three header contracts that nothing has validated. Order of work:
  1. cmpcomdat the out-of-line copies of the implicit destructor (v 0x006E2500), LowerBound (v 0x0040F020) and _M_insert_overflow (v 0x0076ADA0).
  2. Apply the TU-mate contract: the same define set on 0x008E16A0 / 0x008E20F0 / 0x008E24F0, with a site / f / score / decision table. A define that breaks a mate is out.
  3. Only then structure: preceding expanded CRC loops, a retail backward branch round the sky block, and Ego_d's 582 B line 3298.
- **`do{}while(0)` stays a device:** not landable, not Inferred.
- **FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR** is an acceptable add-only opt-in only with two sources:
  - one is retail expanding it in the sky block;
  - the other is a cmpcomdat SAME v 0x006E2500, or the PDB method list showing no declared destructor.
  - Landed 0x006E2500 and the header's other includers then become re-port debt (logged, not re-ported now).
- **0x00C73050:** "not a symbol tie within 1..32 literal dummies"; one bounded pass with the c2prio probes, then a report.

## 0x007602F0 NShape::IsShapeConvex (p25O): a float local Ego_d does not list
- **Where it stands.** The closing2 residue is closed: a named iterator assigned inside the dereference, `second = *(it_second = pts.begin() + 1);`, keeps Ego_d lines 336/337. The best Ego_d-conformant source (GFArcCos at both closings) is DIFFER(1126v1126), 2 lines, an epilogue pop/fadd order.
- **Question.** It matches only when closing1's acos result sits in a `float closing_angle` variable. May that land as Inferred?
- **Ruling: no.** Ego_d lists the function's locals and `closing_angle` is not one of them, the same case as 0x00763060's `anchor`.
- **Before parking, one bounded pass:**
  - one spelling across the three 29-B sites (all GFArcCos v all raw acos);
  - a cmpcomdat of the GFArcCos body if retail keeps a copy;
  - existing Ego_d float locals;
  - confirm `it_second` is an Ego_d local.
- **A second fitted row in the same TU** does not count as a second source for the revision-difference hypothesis.
- **Follow-up ruling (TU-mate results).** All four rows are in creature_modes.obj (FableWin #217/#230/#274/#284).
  - /d2inlT plus CCRC_PASS22_BODY is allowed in principle: the owner's inline-budget rule, line 2 with ablations, with 0x008DDAD0 as the same-TU precedent. It is not sufficient: no T splits two sites with equal callee, f and score (loop 2 and sky LowerBound, 6.25/63).
  - **Correction:** "a define that breaks a mate is out" is withdrawn for this TU. Retail expands LowerBound, the destructor and Calc in this row, so their bodies are visible in the TU. The flagless mates 0x008E16A0 / 0x008E20F0 / 0x008E24F0 that break hide those bodies; that is re-port debt on them.
  - One bounded pass before parking: cmpcomdat of the four callees (incl. Calc v 0x004014A0); the instantiation identity of the sky's expanded callees; Calc argument constness per site; the sky's own f (destructor 12.5 v LowerBound 6.25; T 57..62 with no device); FableWin v Ego_d length.

## 0x00C73050 (p25N): one register (tail-item temp edi v retail edx)
- **Measured.** Candidate #32 is coloured after #22 (eax) and #31 (ecx), and every integer candidate's order and register matches retail. Its allowed set is {edx, edi}, with only edi costed (400), yet edi is chosen.
- **Ruling.** Interference hook authorised, bounded, after two zero-compile reads:
  - the cost sign across the trace (if the highest cost wins, the 400 is a preference and no hook is needed);
  - whether edx is actually busy in 0x00C731D2..F4.
  - The hook dumps #32's forbidden set at 0x1078368e, then one targeted respelling pass on the one named interferer, else park with the dump.
  - The hook is saved with the c2 kit before use.
- **No more untargeted respellings** (33 negative). The row stays reserved.
- **Name:** the catalogue says Audio_PlaySourceAndProcessQueue, p25N says CAudioSource::UpdateStream; a two-source NAME_FIXES line is needed, or one name kept consistently.

## 0x00960120 CAIStateGroup_DeedReactionRoot::Update (p25O): a per-TU forceinline opt-in v /d2inlS25
- **Facts.** /d2inlS25 matches the row by expanding CCharString::IsEmpty, but it breaks the TU-mate 0x009609B0 IsRunnable at every S 21..100 (and T275/T290). FABLE_RETAIL_CCHARSTRING_FORCEINLINE_ISEMPTY matches both with no flag.
- **Ruling: land with the opt-in.** FABLE_FORCEINLINE header opt-ins are a landed family (TEXT_EQ, EQUALSCI, LESS), and the owner allows inline controls for inline-budget residues. Line 2 names it, with the grade without it and the S25 alternative.
- **Condition met.** The opt-in had to hold across the TU. FableWin's ai_state_group_deed_reaction_root.obj lists the TU, and none of its landed rows has an IsEmpty site, so nothing contradicts it. The flag form is kept as evidence.

## 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering (p25N): `info` spilled v retail ebp
- **Measured.** No write to info's allowed set before the driver splits it, so it is not the callee-saved mask. The split follows #3 taking ebx. Retail loads info.Engine twice per call (the vtable into ebx mid-pushes, then `this` into ecx); ours keeps one Engine temp across the argument calls.
- **Ruling: no split-path hook, no park yet.** One bounded source-level pass:
  - /Od loads at the Engine call lines;
  - a retail per-site table;
  - a landed DrawPreparePrimitivesForRendering sibling with the two-load shape;
  - one respelling pass on the call expression within Ego_d's locals.
  - The hook is justified only if a sibling with the same spelling matches.

## 0x005B01FE CTCInventoryTrade::SellItemPC (p25O): temp numbering decides the sum order
- **Ruling: no allocator reversing yet.**
  1. Complete the text first: Ego_d's two 12-B lines 7350/7351 and def_index are required, not fitted, so every temp-number measurement without them is provisional.
  2. Widen the target to −1/−2 temps as well as +2/+3.
  3. Read the existing symbol log first: the extra call's temp numbers, the statements behind the temps just below #1178, and the function's first temp number (TU carry-over?).
  - Hook 0x107636A2 only if those leave it open.
- **0x004FEB70 (p25N):** the 260v261 draft's `long map` / `CMap* pmap` are not Ego_d locals, so it cannot be the base. Ego_d's TPI types for map_list / it decide the single-load form.

## 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (p25P): fitted content inside a shared-header dead arm
- **Question.** The stock CArray indexer arm gives 3491v3491. Adding k=4..6 dead calls inside the arm plus a 1..4 local shift gives 8 lines. May the arm carry a fitted size?
- **Ruling: no.** It does not match anyway. Any dead statement moves the temp, so k plus a shift is a free dial with no oracle. And the arm is shared-header text for every row that expands the indexer.
  - The 0x00B24850 / 0x00BB3E80 precedent covers fitted arm counts in a row's own body, not IL padded inside a header arm.
- **When a larger arm would be acceptable:** Ego_d calibrates its content (the 719-B / 261-B lines in units of known one-assert lines), and the same header text keeps every landed CARRAY_INDEX_ASSERT user matching. It would then be a header contract labelled Inferred.
- **The stock-arm configuration is the new base.** The helper-text sweeps for (2)/(3) are redone on it.

## Scoreboard crypto TU (p25N, 2026-11-21): /Gr as the TU's calling-convention default
- **Ruling: accept /Gr.** It is a calling-convention default, not an inline control. Two sources: Ego_r mangles every helper YI, and retail's out-of-line STLport instances take ecx/edx. The same-TU landed 0x00563480 already uses it, and it removes devices from the re-ported rows.
- **Conditions met:**
  - the catalogue records CompilerFlags /Gr for the re-ported rows, and Explode regrading RELOCATION_MATCH proves the flag is honoured;
  - the only vland warnings are import-thunk aliases;
  - the land order kept every copy verbatim.
- **Follow-up:** check whether 0x008E18D0's "fastcall STL helpers" fact is the same /Gr TU default.
- **Note (2026-11-21, p25N):** YI mangling is project-wide in both debug PDBs (Ego_r 3,753 YI against 27 YA), so it is not per-TU evidence for /Gr. The scoreboard ruling still stands on retail's ecx/edx out-of-line STLport instances, the same-TU landed /Gr row, and the Explode ablation.
- **2026-11-21 follow-up (0x00A4EFC0).** p25P traced the residue to the colouring split: the paletted-finish piece is dropped in ours and kept in ebx in retail. Not ruled "no source handle" yet; one more bounded pass first:
  1. FableWin v Ego_d length (revision);
  2. settle whether Ego_d pins the position size==1 branch before the clear();
  3. confirm the mechanism with one extra read, as a measurement only;
  4. clear / erase / resize / size header opt-ins, measured by the candidate's refs.

## 0x008DBC80 CModeDragonFlying::FrameUpdate (p25P, 2026-11-21): one direct ParentThing read
- **Case.** RELOCATION_MATCH needs the GetPos in the look_pos line (Ego_d 5093, 136 B, two accesses) read directly; the getter there gives 2320.
- **Ruling: acceptable as Inferred if three checks pass.** It adds no local, device or flag, and Ego_d neither lists nor excludes it (the 0x00B24850 precedent covers that case). The checks:
  1. the member's access level allows a derived-class read;
  2. the 5093 decode with the new calibration does not fit two calls (one call: Ego_d shape, no label);
  3. the getter bodies are proven, and the const overload and operand order are tried.
- If it lands, the row stays on the Windows request for line 5093.

## 0x007602F0 NShape::IsShapeConvex (p25Q, 2026-11-21): paren cast and three-float edges
- **Facts.** RELOCATION_MATCH with all three sites through GFArcCos, plus a new add-only FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST (`((float)acos(value)) * K`). The parentheses add one no-code IL 0x269 tuple, the float-copy node p25O traced.
  - The edges are built with the three-float ctor, but Ego_d 309/317/328/338 decode as `second - first` plus an 8-B call.
- **Ruling: not landable yet.**
  - The edges get the 0x008DBC80 line-5093 size test (fits: no label; can't fit: out; undecidable: Inferred), and Ego_d's TPI types for first / second / this_line / last_line decide whether a 2D text is the real one.
  - The paren body is acceptable as Inferred (the PAREN family), re-measured after the edges settle. Every landed GFArcCos user is regraded with it, and it lands only with the row.
- **2026-11-21 (0x00763060, coordinator ruling; the advisor call timed out):** p25Q's VC7.1 probe proves Ego_d's pre-increment text cannot produce retail. The post-increment text still contradicts Ego_d 199 / 204, so it is not Inferred. The probe is evidence about the revision, not a second source for the spelling. The row stays reserved behind its bracket and on the Windows request.

## 2026-11-21 owner instruction: next reservation set by size
Owner: "after next reserved rows/functions are done, reserve the next set from the largest ones". When p25Q's and p25R's
current queues are done, B reserves the next set from the largest parked UNMATCHED rows (length descending) that no claim
line, log entry or A lock names. Pre-scout (2026-11-21), largest first: 0x00E20600 Main (14512), 0x00502E90
PopulateRegionWithVillagers (13632), 0x00BD3070 RenderMeshPrimitive (11296), 0x00795300 / 0x0078FD60 FrameUpdate (9312 /
9280), 0x00F1EED0 PlayWave (9264), 0x00C4F600 (9216), 0x00B4D370 UpdateShadowSpace (7184), 0x00CF4EC0 Main (6176),
0x00D09010 Main (5872). Several carry acctC round-six drafts (released 2026-11-16/18, pending/2026-11-16_acctC_open).
Re-check claims at reservation time.
