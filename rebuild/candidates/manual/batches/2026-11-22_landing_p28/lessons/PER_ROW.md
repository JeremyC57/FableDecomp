# Codex pass 28: why each matched row was stuck, and how Codex got it out

These are the 21 rows Codex matched (STATUS.tsv grade RELOCATION_MATCH). Each landed source in `rebuild/src/compiled/` is the same as Codex's `<addr>.cpp`, apart from the `// Grade` line. I checked this by diff.

## Sources and how claims are marked

**Source paths.** These short forms are used throughout:
- `ours:` is the best pre-Codex source, column 4 of `pending/2026-11-22_codex_pass28/rows.tsv`. It is identical to the package's `rows/<addr>/best_draft.cpp`.
- `ours-notes:` is the NOTES / STATUS / LESSONS files in that draft's batch folder.
- `cx:` is the Codex hand-back (committed subset: `batches/2026-11-22_landing_p28/codex/`): `rows/<addr>/NOTES.md`, `PROBES.md`, `PROBE_HISTORY.tsv` (PH), `hdr/PATCHES.json` (patch #n is the array index) and `LESSONS.md`.
- `pkg:` is the package we gave Codex, (scratch; rebuilt by `pending/2026-11-22_codex_pass28/build.py`). It holds `rows/<addr>/egod_locals.txt`, `fablewin_proc.txt` and `ROW.md`.

**Evidence marks.**
- **[V]**: I verified it by grepping or diffing our draft against Codex's final.
- **[P]**: a probe in PH shows the byte effect. The probe number is its position in that row's PH sequence. The hand-back does not include the probe sources, so a [P] attribution rests on the probe tag and its PROBES.md description, not on a source diff.
- **[N]**: Codex's NOTES/PROBES state it, and I did not verify it independently.
- **[C]**: a correlate only. It is always present, but no probe isolates its byte effect.

**Probe counts.**
- Probe counts are the PH lines for the row. They include compile failures and repeat verifications.
- "First match" is the first PH line with status MASKED_EQUAL.
- PH holds 1,937 lines for the 21 matched rows and 709 for the 4 unmatched rows.

**Debug-record usage.** The test is: of the local names in the row's `pkg:egod_locals.txt` / `fablewin_proc.txt`, how many appear as identifiers in our draft versus in Codex's final (`_work/local_name_usage.txt` (scratch analysis, not committed)). Our drafts used 0-5 of them, mostly generic names such as `i`. Codex's finals use all or nearly all of them in every row. 00560d00 had no records in the package, because our draft's name (ProcessLogin) was wrong and nothing matched it.

---

## 0x00491AD2 CSubtitleRenderer::ProcessEmbeddedHighlightTags (2,223 B)

**Grades.** Pre-Codex DIFFER(2240v2223). Codex baseline 2240. Final RELOCATION_MATCH. 209 probes; first match at probe 162 (`round2_subparen_top`).

1. **Why stuck.**
   - Our record put the residue down to a compiler effect: "VC7.1 peels the first newline-count compare against the known 0" and "frame 0xAC vs 0xA0 (five extra temp slots)" (ours-notes: `2026-11-05_linux_hand784/STATUS.tsv` line 7, `NOTES.md` line 12).
   - The diff shows the actual differences [V]:
     - Our newline loop is a `for(;;){...break;}`; variants with goto were also tried.
     - Ego_d's locals have a `bool new_lines` flag and a `do{...}while(new_lines)` shape (`pkg:rows/00491ad2/egod_locals.txt` lists `new_lines`, `new_num_newlines`).
     - Ego_d puts `screen_width`/`text_area` in their own scope block; our C2DBoxF box lived to the end of the branch.
     - Our draft used unsigned loop indices; Ego_d's `i` locals are `long` (type 0x12).
     - Our draft has two extents temporaries, but builds the highlight offset from the last-line extents alone, `C2DVector(lastExtents.X, lastExtents.Y)`. Its first `extents` is computed and never used.
     - Ego_d and the final take Y from the full sub-string extents (`sub_string_extents_height.Y`) and X from the last-line extents (`sub_string_extents_width.X`). So the draft's Y offset is also a behaviour error for multi-line text.
   - Our draft also modelled CPlayerGuiDef, CDisplayManager and CWStringParser locally, and renamed the class with `#define CSubtitleRenderer CSubtitleRendererLayout` [V, C].
2. **How Codex recognised it.**
   - It took names, flags and scope blocks from Ego_d.
   - It then ran a long probe sweep: about 25 `round2_subparen_*` probes plus `subsched`/`subcvt` probes on parenthesisation and temporaries. Its own aligned-instruction count was the fitness signal (726/726 at the match).
   - An operand audit found five call sites missing from the old binding table (cx: NOTES "Latest verification").
3. **Fix.**
   - Ego_d names and scopes, signed `long` indices, and the `new_lines` do-while.
   - The text-area box in its own `{}` block.
   - A parenthesised top-edge cast (`// Fidelity:` line).
   - `auto_inline(off)` bodies for `char_traits<unsigned short>::compare` and `operator==`.
   - Patch #132 declares the STLport `basic_string<unsigned short>` == out-of-line, owned at 0x0040F0A0. Patch #77 declares the method in `CSubtitleRenderer.h`.
4. **Lesson.** A "the compiler peels/keeps an extra slot" residue usually means the loop or scope shape is not the source's. Copy the loop-control locals and `{}` blocks from `egod_locals.txt` first. After that, sweep parenthesisation of each float expression.

## 0x00559A90 NUISystem::CScrollBar::ProcessEvent (2,163 B)

**Grades.** Pre-Codex DIFFER(2144v2163). Baseline 2144. 78 probes; first match at probe 44 (`r_scroll_m_original`).

1. **Why stuck.**
   - Our note says "frame-slot sharing only (retail 0x5C frame ... ours 0x30), hence 3 dead def-pointer zero stores" (ours-notes: `2026-11-04_linux_hand752` STATUS).
   - The draft used `if (event == ...)` where the original has `switch(event)` [V].
   - It has Ego_d's `delta_size` as `float top` with a different expression: `PositionOffset.Y + PositionOffset.Y` against `PositionOffset.Y - PositionOffset.Y * TargetZoom.Y`.
   - It adds `end` and `dragged` locals that Ego_d does not have.
   - It reached the list through the global `CList::GPreviousSelected`, not the static accessor [V].
   - Codex's probes imply that the baseline's map-index body was not the original visible one. I did not check this in the include tree [N].
2. **How Codex recognised it.**
   - Probe `r_scroll_donor_getter` restored the switch, `delta_size`, both clamp forms and `CList::GetPreviousSelected()`. That reached 2163 B, but bytes still differed [P].
   - Its PROBES note says: "All 64 displacement and frame allocation differences in the declaration-only version disappear with the complete read-only key path."
   - Making the STLport `map::operator[]` body visible stops the `const unsigned long&` key from escaping, and then VC7.1 shares the slots as retail does [P].
   - The weak map copies then differed. They were fixed one owner at a time (`r_scroll_spec_tree_size`, `r_scroll_final_minimized`).
3. **Fix.** The donor switch and locals. Patch #129 adds `FABLE_RETAIL_STLP_CUISTATE_MAP_OWNERS_PASS28`: a visible `operator[]` plus `_M_lower_bound` / pair constructor with each retail owner's size and frame settings.
4. **Lesson.** A frame-size-only residue can be escape analysis. Try making the real inline body of every callee that takes a reference visible. Then fix the emitted COMDAT copies' optimisation settings so pick-any still passes.

## 0x00560D00 CTCHeroOnlineScoreboard::LogOnToRegistrationServer (1,860 B)

**Grades.** Pre-Codex DIFFER(1844v1860). Baseline 1844. 108 probes; first match at probe 93 (`round2_scorereq_early`).

1. **Why stuck.**
   - It was a first draft with one grade on record ("first draft: this in ebp ... std::string temp order, frame 0x98 v 0x9c"; ours-notes: `2026-10-23_linux_hand324` NOTES/STATUS).
   - The identity was wrong [V]: the draft is `ProcessLogin` with a bare `// Leaf: ProcessLogin`. The real member is the private `?LogOnToRegistrationServer@CTCHeroOnlineScoreboard@@AAE?AW4ENetworkSuccess@1@AAVCCharString@@@Z`.
   - The TU's `/Gr` flag was missing [V].
   - The behaviour was wrong [V]. The draft hashed the user name twice and encrypted the lower-case user-name hash with a fixed key. Retail encrypts the CD key under `MD5(password)` (cx: PROBES "Password evidence"; +0x114 password field).
   - Every type was a row-local model (LHString, CUserProfileManager, CCharString) [V].
   - Because the name was wrong, the package builder found no Ego_d / Ego_r / FableWin record for the row (`pkg:rows/00560d00/` has none).
2. **How Codex recognised it.**
   - It took the signature from the PDB name sets.
   - FableWin locals gave `req`, `result` and the reply variables (cx: NOTES).
   - It used a donor-code fact: "the donor calls GetSignedInPassword for the second MD5 at +0x359". Our lane cannot reproduce that from PDB records (see LESSONS.md).
   - A behaviour test with distinct user name and password exposed the hashing error. Codex's LESSONS: "Source length alone did not catch this error."
   - The final 44 masked bytes came from the "original early-return GetAsCharArray form" and a common result return [P, probe 93].
3. **Fix.**
   - The real name, `/Gr` and the shared `CTCHeroOnlineScoreboard.h`.
   - Patches #91, #92 and #117 for the scoreboard and HTTP status, and #133, #134 and #136 for the STLport string range erase.
   - `StdEncryptString(GetCDKey(), MD5(password))`.
   - Strong same-TU definitions of the TU-mates 0x0055E040, 0x0055E180, 0x0055E2C0 and 0x00560920 (`// Fidelity:` lines).
4. **Lesson.**
   - Prove the mangled leaf, and copy the same-TU neighbours' flags, before shaping any code. A wrong name also hides every PDB record for the row.
   - Write the behaviour test with distinct values for every argument, so a duplicated argument fails.

## 0x00647AB4 NPlayerGui::CDrawHealth::Draw (1,910 B)

**Grades.** Record DIFFER(11v1910). The draft header says 87v1910, and Codex's baseline was 87. 169 probes; first match at probe 140 (`round2_healthshared_site2tmp`).

1. **Why stuck.**
   - It was never written. The draft is an 87-byte "HEAD DRAFT": a hero-alive guard, `Drawn`, the GUI def and the TC-4 lookup. Its header says "the x87 bar layout is not drafted" [V].
   - The 11-byte record came from a flag sweep on the 3-byte skeleton (ours-notes: `2026-10-30_parked_retry2_sec2/STATUS.tsv` line 320).
   - The signature was also wrong [V]. The draft has `void Draw()`; the leaf is `?Draw@CDrawHealth@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z`, so it takes a `const CEnginePrimitivePrepareInfo&`. The draft used local pad models of CThing, CPlayerGuiDef and CDrawHealth.
2. **How Codex recognised it.**
   - It took the signature from Ego_r / FableWin and the locals from FableWin (26 names).
   - It then ran a long sweep of temporaries and site shapes (`round2_healthshared_*`).
   - Its notes: identity GFToScreen conversions, "the authored C2DBoxF copy prevents a bulk copy from taking the registers", and "Explicit pointer temporaries evaluate the destination too early" [N].
   - A pick-any exception (a folded destructor alias) is documented in `BINDING_EVIDENCE.json`.
3. **Fix.** The whole function, with implicit pointer conversions on the two Effects assignments. Patch #123 adds the new header `engine/NPlayerGui_CDrawHealth.h`; patches #1 and #90 edit `C2DVector.h` and `CTCHaste.h`.
4. **Lesson.**
   - A head draft or skeleton is not a residue. Write the complete function from the mangled signature before grading.
   - Prefer the implicit conversion the source would have used over explicit pointer temporaries.

## 0x006C6040 CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed (2,377 B)

**Grades.** Record DIFFER(2793v2377). The draft and Codex's baseline are 2809. 74 probes; first match at probe 32 (`round2_deedsite_parent`).

1. **Why stuck.**
   - We read retail as VC7.1 tail-merging two identical witness arms: "retail tail-merges the vandalism ... and 0x35/9 ... witness arms ... (VC7.1 cross-jump of two identical tails)" (ours `// Grade`).
   - We wrote the village walk out in each arm and tried 11 variants to make the two copies identical (ours-notes: `2026-11-07_linux_hand828` STATUS).
   - Ego_d shows ONE local, `inform_buildings_villagers`, set in either arm, followed by one village walk (`pkg:rows/006c6040/egod_locals.txt`) [V].
   - The draft also called `GetParentThing()` where the source dereferences `*ParentThing` [V].
   - It used local COpinionDeed / CThingFilter_IsEligibleWitness / CTCEnemy / CTCSleep models [V], and the descriptive global `GDeedUnknownThreatWitnesses` [V].
2. **How Codex recognised it.**
   - Probe `pdb_single_village_walk` took 2809 to 2409 B [P].
   - The `shared` probe, which moved to shared headers only, left the bytes at 2809. So the local models were not the byte cause here [P].
   - Probe 32 (`*ParentThing`) gave the match [P].
   - Its pick-any audit found the stale `GetHomeBuilding` binding at 0x00810FC0, an event getter (`BINDING_EVIDENCE.json`, `NAME_FIXES.tsv`).
3. **Fix.**
   - The single building local and one walk.
   - `*ParentThing`.
   - `/d2inlT80`, plus `#pragma inline_depth(0)` around the push_back sites, under an `// Inline-budget:` line.
   - The global `NGlobalConsole::EnableUnknownThreatDeedPostings`.
   - Patches #109 to #112 add the new `CThingFilter_*` headers; #83 edits `CTCCreatureOpinionOfHero`.
4. **Lesson.** If retail looks like two arms tail-merged, first check whether the source had one copy that both arms feed. A single local in Ego_d where you wrote two is the tell.

## 0x0078ECD0 CTCDivineWrath::CTCDivineWrath (2,034 B)

**Grades.** Record DIFFER(2000v2034). The draft and baseline are 2032. 56 probes; first match at probe 29 (`divwrath-shared-typed_named`).

1. **Why stuck.**
   - The draft was 2 B short, with the residue "frame 0x1c v 0x20 slot layout" (ours-notes: `2026-10-30_parked_retry2_sec3` STATUS).
   - It had 28 row-local types, including a hand-written `__forceinline Lookup` with its own CRC loop over `g_CRCTable` [V].
   - `GetSubDef` (0x00669580) was declaration-only, and `CalcAndLookup` was a local copy [V].
2. **How Codex recognised it.** A ladder of callee-body probes, recorded in `cx:rows/0078ecd0/PROBES.md` [P]:

   | Step | Size | Aligned |
   |---|---|---|
   | Shared owners with opaque lookups | 1,878 B | — |
   | Expose the shared effect lookup and CRC | 1,986 B | — |
   | Real intelligent-pointer assignment | 2,013 B | — |
   | Expose the STLport const lookup | 2,031 B | — |
   | Original out-of-line GetSubDef body visible (its "parameter escape information changes register allocation") | 2,017 B | 574 |
   | Donor's typed CRC loop | 2,034 B, one byte off | — |
   | Named length/text locals in the shared effect lookup | match | — |

   An operand-level diff pinned that last byte to the base/index encoding of the byte load at 0x0078F245.
3. **Fix.**
   - Shared headers.
   - `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE`, with the body visible but not inlined.
   - Patch #101 for the inline `CTCSpecialAbilities::LockAbilitiyMutex`; patch #130 for the size setting of the shared `unsigned long` map find owner.
4. **Lesson.**
   - A 2-byte length gap is not "nearly done". Replace every row-local re-implementation of an engine helper with the shared real body.
   - Visible-but-not-inlined callee bodies change the caller's register allocation.

## 0x007EBEA0 CTCActionUseBed::OnActionUse (2,145 B)

**Grades.** Pre-Codex DIFFER(2124v2145). Baseline 2124. 62 probes; first match at probe 50 (`bed_final_separate_denial_source`).

1. **Why stuck.**
   - Our record: "register colouring (VC7.1 puts the hero in ebp ...)" and "FAIL-block cross-jumping (retail keeps the berserk FAIL apart)". Probe j added a separate "neither owned nor useable" FAIL before the chain, and VC7.1 then cross-jumped all three FAIL blocks into one (2024 B) (ours-notes: `2026-11-05_linux_hand784` STATUS line for 0x007EBEA0).
   - The draft's denial chain [V]: berserk `if {FAIL; return;}`, then a separate `if (bed && !bed->Empty) {FAIL; return;}`, then `if (OwnedByHero) ... else if (UseableByHero) { nested if / else-if chain } else {FAIL; return;}`. The unusable-bed FAIL is the trailing `else`.
   - The final has one flat chain: `if (tc_bed && !tc_bed->IsEmpty()) {FAIL; return;} else if (OwnedByHero) ... else if (!UseableByHero) {FAIL; return;} else if (!PBuilding.Get()) ... else if (!building_has_occupants) ... else {...}`.
   - So the decisive change is the order and nesting of the chain: the negated useable test is an early arm, and the occupied-bed test heads the same chain. It is not un-merging hand-merged code.
   - The draft had 25 row-local types [V].
   - Our own note records a harness bug: "q.py does not read a source's `// Flags:` line", so /GS rows graded without their cookie until passed by hand (ours-notes LESSONS).
2. **How Codex recognised it.**
   - It took the shape from the Ego_d locals: `allowed_to_sleep`, `building_has_occupants`, `tc_bed_owned_entity` and the other locals and scopes.
   - Probe 50 (`bed_final_separate_denial_source`) wrote the occupied-bed and unusable-bed denials as arms of one flat else-if chain, each with its own `SetText(...); return;`, and let VC7.1 merge the identical tails [P].
   - The pick-any audit showed the `GetHomeBuilding` copy "unequal" only because the binding table named the event getter at 0x00810FC0. `PICKANY_CORRECTED.txt` gives 13 equal copies (`BINDING_EVIDENCE.json`).
3. **Fix.**
   - The flat denial chain in the source order.
   - Scoped `{ const CIntelligentPointer<CThing> user(&thing); PThingUser = user; }` temporaries.
   - Patches #80, #81, #104, #106 and #113, including the new `CTCActionUseBed.h`.
4. **Lesson.**
   - Match the source's else-if order and nesting, including where a negated test (`!UseableByHero`) sits. Write each arm out even when the arms look identical, and let the compiler merge them. A trailing `else` and an early negated arm give different block layouts and cross-jumps.
   - Before trusting a pick-any NEQ, check the binding line.

## 0x008101F0 CTCDExplosiveTrail::FrameUpdate (1,914 B)

**Grades.** Pre-Codex DIFFER(1608v1914). Baseline 1608. 117 probes; first match at probe 104 (`round2_trailsched_random_product`).

1. **Why stuck.**
   - It was an unfinished first draft: "the steering block (acos/tanh, inline vector temporaries, x87 compare chains) is approximate" (ours `// Grade`; ours-notes: `2026-10-23_linux_hand324`) [V].
   - It had 19 row-local types and a bare `// Leaf: FrameUpdate` [V].
2. **How Codex recognised it.**
   - Ego_d locals gave `NUM_SECS_BETWEEN_TRAPS`, `trap_create_period`, `trap_index` and the rest.
   - Probe `round2_trailtime_index` reached 1914 B by computing `trap_index` from the same division as the period test [P].
   - Codex attributes `IsClosed` before `Kill` to "FableWin 0266AF75 delegates to the fade member". That is a donor-code fact.
   - A scheduling sweep found the parenthesised random product (probe 104) [P].
3. **Fix.**
   - The complete body with Ego_d locals and a strict expiry compare.
   - The `IsClosed` call, and the original `make_pair<int>`, converting pair constructor and list insertion owners, verified as extra owners.
   - Patches #50, #52, #85 and #114, including the new `CTCDExplosiveTrail.h`.
4. **Lesson.** Finish every "approximate" block before parking. Then take every local the debug record lists, because named locals fix evaluation order.

## 0x008664F0 CCombatAction_GenericStrikeResponseKnockdown::Initialise (2,028 B)

**Grades.** Record DIFFER(1963v2028). The draft and baseline are 1957. 45 probes; first match at probe 35 (`round2_knockfilter_find`).

1. **Why stuck.**
   - The draft called a placeholder vtable slot [V]. It modelled the graphic interface as `g00()..g1f()` and then `SetOrientation(const SOrientation&)` at slot 32 (+0x80), and called it with a constructed SOrientation.
   - Codex identifies slot 32 as `ResetRHSet` (cx: NOTES). Its note that the old draft "invoked SetRHSet at +0x12c" does not match anything I can see in our draft; the draft calls the slot-32 placeholder.
   - The facing vector was read through an out-parameter, `GetFacing(C3DVector*)` [V]. The source returns `C3DVector` by value into the caller's hidden result slots.
   - The draft had 29 row-local types, including a local C3DVector with a `__forceinline` copy constructor [V].
   - The descriptive global `g_ResetProjectileModeOnHit` is `NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit` [V].
2. **How Codex recognised it.**
   - Ego_d gave `cos_thet` and both `line_to_target` locals.
   - A donor-code read found "IsNonZero reads Y then X then Z" and the cheat byte read before `ResetProjectileWeaponMode`.
   - Visible `GetComponent` / `Find` bodies removed a 4-byte slot (probe 35) [P].
3. **Fix.**
   - Direct initialisation of the three facing vectors.
   - ResetRHSet.
   - The visible owners, with the 125-byte `GetComponent` at 0x00867F60 verified as an extra owner.
   - Patches #33, #39, #41, #42, #98, #108 and #122.
4. **Lesson.**
   - Never call a placeholder vtable slot. Name every slot from the PDB vtable or class record first.
   - Model getters that return a vector by value as by-value returns, not out-parameters.

## 0x008E8060 NCreatureMode::CModeBriarRoseConcentrate::OnActivate (1,784 B)

**Grades.** Record DIFFER(5v1784), but the draft on record compiles to 1758v1784 (its own header and Codex's baseline). 53 probes; first match at probe 48 (`round2_briarstr_fxall`).

1. **Why stuck.**
   - The record was stale [V]. The 5-byte grade is the old skeleton; the draft is a full reconstruction 26 B short, with five graded variants listed in its header.
   - Our brief told Codex this row "compiles to a stub".
   - Our residue: "frame 0x1A0 vs 0x190 (local packing), ebx used as a function-wide zero" (ours-notes: `2026-10-31_parked_retry2_sec4b`).
   - The draft passed explicit temporaries, `CThingFilter_HasScriptName(CCharString("TeleportMarker"))` and `GetDefGlobalIndexFromName(CCharString("..."))`, and had 28 row-local types [V].
2. **How Codex recognised it.**
   - It ran a `round2_briarstr_*` sweep that made the string arguments implicit site by site: marker 1764, all 1784/502, fxall match [P].
   - It also used the authentic GetSubDef, effect lookup and marker-search bodies. The marker search at 0x008E8AC0 matched independently.
   - It kept the effects def in the Ego_d scope [N].
3. **Fix.** Literals passed straight to `const CCharString&` parameters. Patch #79 adds the gated `CTCAbsorbHits::AddHitsRemaining`.
4. **Lesson.** Pass literals to `const CCharString&` parameters implicitly. A hand-written `CCharString("...")` temporary moves stack slots and register choices.

## 0x00A87960 CSBIAgents::Process (1,824 B)

**Grades.** Pre-Codex DIFFER(115v1824); Codex's baseline graded LEAF_REVIEW. 27 probes; first match at probe 19 (`donor_kinematic`).

1. **Why stuck.** It was never attempted: "skeleton draft, parked on sight ... only the disabled-steering early-out is modelled", with four local structs [V] (ours-notes: `2026-10-23_linux_hand322`).
2. **How Codex recognised it.**
   - It wrote the avoidance maths from the Ego_d locals (25 names).
   - Native float constants and distance order got to 1824 B, but six operand bytes still differed.
   - Using `CKinematic::GetVector` / `SetVector` instead of a direct member copy fixed them (probe 19) [P].
3. **Fix.** The full function, with patches #55, #73, #75 and #76 (`CKinematic.h`, `CSBIAgents.h`, `CSteeringAgent.h`).
4. **Lesson.**
   - Rows parked "on sight" were never stuck: write them.
   - For the last few operand bytes, call the class's own accessors instead of copying members.

## 0x00AB7C20 CStaticFontBank::Render (1,910 B)

**Grades.** Pre-Codex DIFFER(1783v1910). Baseline 1783. 62 probes; first match at probe 37 (`round2_fontsch_count_uv0`).

1. **Why stuck.**
   - It was a first draft with a bare `// Leaf: Render` and a wrong signature [V].
     - Ours: `(const CWideString&, const C3DVector&, float, const CRGBColour*, unsigned long, bool additive)`.
     - Retail: `?Render@CStaticFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z`, which is virtual const, takes the colour by reference, and takes `const CTexture*` last.
   - Our note saw that "retail builds each vertex through named position/UV temporaries copied with rep movsd (7 dwords)". It guessed a double local was forcing an 8-aligned frame, and invented a `MakeVertex(const CVertexPos&, colour, const CVertexUV&)` helper (ours-notes: `2026-10-23_linux_hand323`) [V].
   - The draft had 17 local types.
2. **How Codex recognised it.**
   - It took the signature from the PDBs.
   - It noted "The donor calls the free InitFont2DVertex helper". The helper is `FONT2DVERTEX InitFont2DVertex(const D3DXVECTOR4&, colour, tu, tv)`, and its by-value aggregate return produces the rep movsd copies. Identifying it as the DirectX SDK d3dfont sample helper is my own reading; Codex says only "the donor calls", without naming the record.
   - A probe sweep on corner re-evaluation and UV grouping found the match (probe 37) [P].
3. **Fix.** The real signature and the SDK helper. Patches #115 and #116 add the new `D3DXVECTOR4.h` and `FONT2DVERTEX.h`; #53 and #72 edit `CFontManager.h` and `CRenderManagerCore.h`. Also the grouped UV products.
4. **Lesson.** A rep-movsd struct copy per element means a helper that returns an aggregate by value. Look for the original helper (DirectX SDK, STLport, engine) by name in the object's function list before inventing one.

## 0x00B19C60 NParticleEngine::CPSCLight::PreparePrimitives (2,516 B)

**Grades.** Pre-Codex DIFFER(2546v2516). Baseline 2546. 98 probes; first match at probe 79 (`round2_lightowner_lastparAtt`).

1. **Why stuck.**
   - Our residue: "frame 0xA8 vs retail 0xA4 -- retail packs lifeFraction / radiusScale / spotRadius into one slot" (ours `// Residue`).
   - The draft already had the two fractions, `lifeFraction` and `radiusFraction`, with the upper clamp at `1.0f - DELTA_NOTIONAL_ZERO`. So the behaviour matches [V].
   - Its shape differs from the final [V]:
     - Our draft uses an extra `float limit` local inside an `else` block. The final has a flat `if / else if` clamp on `anim_percent`.
     - Our draft uses its own local names; the final uses the Ego_d names `normalised_time` and `anim_percent`.
     - The draft models two types locally.
   - No probe isolates the `limit` local, so its byte effect is not established.
2. **How Codex recognised it.**
   - "The debug line table distinguishes normalised_time for colour from anim_percent" (Ego_d lines and locals).
   - In its own words: "The visible shared ScaleParticleDimensions body fixes the four-byte frame difference. The visible primitive light constructor fixes temporary placement" [N].
   - A grouping sweep on the attenuation expression gave the match (probe 79) [P].
3. **Fix.**
   - The Ego_d locals with a flat clamp.
   - Byte-colour assignment through `Initialise` with four channels; the midpoint through the conversion operator.
   - The visible `ScaleParticleDimensions` and light-constructor bodies.
   - Patches #64, #65 and #71 (`CPSCBase`, `CPSCLight`, `CRGBFloatColour` byte assign).
   - The grouped attenuation expression.
4. **Lesson.**
   - When the frame is 4 bytes off, first make the bodies of by-reference callees visible.
   - Take local names and clamp shapes from the debug record, without extra helper locals.

## 0x00BBE090 CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail (2,464 B)

**Grades.** Pre-Codex DIFFER(2432v2464). Baseline 2432. 47 probes; first match at probe 19 (`r_trail_donor_constant_blend`).

1. **Why stuck.**
   - We called it unsolvable: "Next idea: none in source; revisit when the lane gap is solved" (ours `// Grade`; the bookmark test/or lane gap in `CRenderStateManager.h`).
   - In the displacement-buffer arm, the draft hand-wrote the three state calls the helpers would make (`SetBlendOp(BLEND_OP_ADD)`, `SetSourceAlphaBlendMode`, `SetDestAlphaBlendMode`) [V].
   - The source calls `SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD)` and `SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA)` [V]. The force-inlined helper's switch and bookmark guards still emit code with a constant argument: the missing 32 bytes.
2. **How Codex recognised it.**
   - "The donor displacement-buffer arm invokes ..." (cx: NOTES). Its files do not say which record showed it.
   - One probe flipped the row (probe 19) [P]. It then ran ablations of plain inline versus `FABLE_FORCEINLINE` and `/d2inlT150`, which all differ (PROBES table).
3. **Fix.** The two helper calls. Patch #48 adds the opt-in; the helpers are `FABLE_FORCEINLINE` with an `// Inline-budget:` line.
4. **Lesson.** Call an inline helper with its constant argument. Do not hand-fold it to the states it would set, because the folded helper still emits its guards. Treat "no source spelling / lane gap" as a hypothesis to test, not a conclusion.

## 0x00BE6880 CLandscapeBackgroundPatch::BuildDefaultSection (1,776 B)

**Grades.** Record DIFFER(19v1776). The draft and baseline are 89. 119 probes; first match at probe 108 (`round2_landcallee_before1`).

1. **Why stuck.** It was a partial prefix: "PARTIAL (prefix only) ... the vertex fill, edge strips and index buffer write after that are not reconstructed yet" [V] (ours-notes: `2026-10-31_parked_retry2_sec6b`).
2. **How Codex recognised it.**
   - It wrote the fan, edges and indices from the retail body and the Ego_d locals.
   - FableWin shows `CEngineLandscapeMap::PeekWorldPosition` returning by value [N].
   - Three PDBs confirmed the `GPEngineDef` spelling.
   - A named `C3DPackedNormal` restored slot layout [N]. Making the same-TU `SetTargetVertexNormal` body visible gave the match (probe 108) [P].
3. **Fix.**
   - The whole function.
   - The `GFFloatToLongNear` retail-asm helper, with 15 cited `fistp` sites (`// Retail-asm:` lines).
   - Patches #27, #44, #45, #58, #59, #63, #67 and #125.
   - The extra 150-byte `SetTargetVertexNormal` at 0x00BE66F0, verified as an extra owner.
4. **Lesson.** Finish the function. Then expose the same-TU helper bodies and verify each one as an extra owner.

## 0x00D43DB0 NScript::CQ_GuildTrainingScript::CPreMeleeMaze::Main (1,799 B)

**Grades.** Pre-Codex DIFFER(1665v1799). Baseline 1665. 80 probes; first match at probe 58 (`evidence_evidence_scope_break`).

1. **Why stuck.**
   - It was never worked: "NOT_REACHED ... no hand edit yet (stopped at the lane-time limit)" (ours-notes: `2026-10-31_parked_retry2_sec6` STATUS).
   - The draft uses `inline_depth(0)` on every in-loop exit (37 inline-device lines) and 16 row-local types [V].
   - It does not use the script generator's `TRetVoid _Fail_Val; ... return _Fail_Val.Return();` form or `for (;; Game->NewScriptFrame())` [V].
2. **How Codex recognised it.**
   - "The donor records identify the two movie and pause-guard scopes."
   - It probed termination placement (`priority_pause_*`, `tie_maze_*`, `evidence_*`). Testing termination with an in-body break "reproduces the original EDX/EAX alternation in all four cold calls" (probe 58) [P].
3. **Fix.**
   - The generator form, with the two movie and pause scopes.
   - A typed pause-owner constructor taking `CGameScriptInterfaceBase&`.
   - Patch #70 for the CPreMeleeMaze layout.
   - Three inline-device lines remain.
4. **Lesson.** For NScript rows, write the script-generator form first: TRetVoid returns, the frame loop and a termination check at each statement. Add inline pragmas only after that.

## 0x00DC08C0 NScript::CQ_OakValeBanditRaidScript::WatchForGateTimer (1,830 B)

**Grades.** Pre-Codex DIFFER(1714v1830). Baseline 1714. 49 probes; first match at probe 33 (`solo_oak_shared_reset`).

1. **Why stuck.**
   - Our record diagnosed the residue correctly as structural: "not an inline residue (three separate return-cleanup copies v two merged, un-cross-jumped else arms, hoisted vtable load); old local-model source with inline_depth regions" (ours-notes: `2026-11-08_linux_hand865`).
   - The draft has 11 local types and 13 `inline_depth` lines, and no TRetVoid or frame-loop form [V].
2. **How Codex recognised it.**
   - "FableWin shows a loop with a frame-advance increment and an explicit termination return inside the body". That is donor code.
   - The `Fred` and `Shouted` locals come from FableWin.
   - It swept scope placements (`solo_oak_scope_*`, 1482 to 1820 B).
   - The last 4-byte store came from sharing the `BanditsKilled` reset after both repopulation arms (probe 33) [P].
3. **Fix.** `for (;; Game->NewScriptFrame())` with `return _Fail_Val.Return()` exits, the named locals and the shared reset. The six late returns keep `inline_depth(0)` under an `// Inline-budget:` line.
4. **Lesson.** Separate cleanup copies in retail mean separate return sites in the source. Add the generator's return statement; do not try to get there with inline pragmas.

## 0x00DFD600 NScript::CQ_TraderConflictGoodScript::AttackPeople (2,088 B)

**Grades.** Record DIFFER(2296v2088). The draft and baseline are 2456. 300 probes, the most of any matched row; first match at probe 273 (`round2_traderlast_flowlast`).

1. **Why stuck.**
   - Our record lists ten failed inline spellings and concludes: otherwise "lane gap: inline-budget site choice" (ours-notes: `2026-11-05_linux_hand782`).
   - The draft has a local model of its own class, no TRetVoid, and no `/d2inlT` flag [V].
2. **How Codex recognised it.**
   - The FableWin locals give a signed loop index and two separate conversation indices.
   - It reached a state where "the primary length and all normalized instructions matched" but the jump at 0x00DFD84A still went to the wrong cleanup block.
   - It then compared all 16 termination branch targets against retail (`TERMINATION_PATHS.tsv`) and found the one edge that differed.
   - "Leaving the trader's scope before returning recovered that edge" (probe 273) [P].
3. **Fix.**
   - `/d2inlT70`.
   - A labelled scope exit, marked `// Inferred:`, because "Its source-level spelling of the final exit is unproven".
   - The shared declaration of the vector copy constructor at 0x00DFDE30. The catalogue had a placeholder class name; a local weak copy mismatched.
   - Codex dropped three unnecessary counted-pointer specialisations and `/d2inlS9`.
4. **Lesson.** When size and instruction alignment match but bytes do not, tabulate every branch target that leads to cleanup, ours against retail. A wrong cleanup block points to a scope exit, not to the inliner.

## 0x00E03F70 NScript::CQ_TraderEscortScript::CMagicBarrier::Main (1,906 B)

**Grades.** Pre-Codex DIFFER(1986v1906). Baseline 1986. 48 probes; first match at probe 36 (`solo_flat_locals`).

1. **Why stuck.**
   - Our record: "retail calls ~CScriptThing out of line (per-function inline budget) ... plus the inline angle's fstp is scheduled after the next null test" (ours `// Grade`).
   - The catalogue named the class `CQ_WaspQueenScript` (ours-notes: `2026-11-08_linux_hand865`). The real class is `CQ_TraderEscortScript`, from the leaf `?Main@CMagicBarrier@CQ_TraderEscortScript@NScript@@EAEXXZ`.
   - The draft tested with a row helper, `BothGone(a,b) { return !a.IsAlive() && !b.IsAlive(); }`, and a virtual `IsAlive` on `CScriptThing` [V]. It had 9 local types and no TRetVoid.
2. **How Codex recognised it.**
   - "The FableWin caller has four direct calls to CScriptThing::IsDead and eight calls to both GetAngleXY and GetPos". That is donor code; the package's `fablewin_proc.txt` has only indirect-call records.
   - It reconstructed the 20-byte `GetAngleXY` owner, whose parenthesised return decides tail jump versus call.
   - "The donor puts RockTroll in the function-level local list": removing the extra scope fixed the last ten slot displacements (probe 36) [P].
3. **Fix.**
   - `IsDead() { return !IsAlive(); }` on each handle.
   - The inline `GetAngleXY`.
   - `RockTroll` at function scope.
   - The generator form.
4. **Lesson.** Call the wrapper the source called, such as `IsDead`, even when it is only `!IsAlive()`. Put every local in the scope the debug record gives it.

## 0x00E49020 NScript::CV_BanditTollScript::CTauntingBandit::Main (2,084 B)

**Grades.** Record DIFFER(2276v2084). The draft and baseline are 2424. 79 probes; first match at probe 64 (`tollfin_toll_exact_timer_edges`).

1. **Why stuck.**
   - Our record: "per-exit inline split of ~CTimer ... the BanditsFlee and BanditsAngered blocks stay one notch too hot" (ours `// Grade`; ours-notes: `2026-11-05_linux_hand791`).
   - The draft had a local CScriptDef model and no TRetVoid [V].
   - Codex found wrong callee signatures [N]:
     - `IsPlayerThreateningEntity` returns `int`.
     - `FadeOutAndKillEntity` takes the float before the two bools.
     - The global is `pGlobalScriptDef` (`CDefPointer<const CScriptDef>`).
2. **How Codex recognised it.**
   - It took the callee signatures from both PDBs and checked the four switch entries against the retail jump table.
   - It probed timer-scope patterns (`tollscope_*`, `tollfin_*`): "The WarningTimer scope exits preserve the six native timer-only edges", with an outer break (probe 64) [P].
3. **Fix.**
   - The WarningTimer scope and outer break.
   - The corrected callee signatures.
   - The shared `fable_script_def_global.h`.
   - The original 18-byte GetPos body.
   - Per-exit `inline_depth(0)` with an `// Inline-budget:` line.
4. **Lesson.** Check every callee's PDB signature, both return type and parameter order, not only the leaf's. Scope each timer as the source did before metering inline heat.

## 0x00EC32F0 NScript::CV_RandomPopulationSimScript::ChooseRandomBanditFromDefs (2,016 B)

**Grades.** Pre-Codex DIFFER(2040v2016). Baseline 2040. 57 probes; first match at probe 31 (`round2_bandit_flat_ONLY`).

1. **Why stuck.**
   - Our record: "frequency + alias residue: with the default label written first ... VC7.1 keeps the freshly zeroed vector words across the virtual GetHeroRoughExperienceLevel call" (ours `// Grade`).
   - The draft wrote `default:` first, as a heat trick, and used a local class model [V].
2. **How Codex recognised it.**
   - It checked case order against the live compiler's jump table.
   - "FableWin shows the returned string copy first, then destruction of the vector and local string". That is donor code.
   - The `_ONLY` (declaration-only) probe matched; the `_OUTOFLINE` and `/d2inlT` variants did not [P].
3. **Fix.**
   - Natural case order.
   - A named `long experience` local.
   - The vector at function scope.
   - `FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY`, so `_M_insert_overflow` resolves to 0x00433530 with no local body.
   - Patch #124 for the class.
4. **Lesson.**
   - Visibility cuts both ways. If a visible callee body lets VC7.1 keep values that retail reloads, try it declaration-only.
   - Do not use case-order tricks the source would not have had.

---

## Note on the aligned-instruction metric

In 00560D00, 00E49020 and 00EC32F0 the final match shows fewer aligned instructions than retail instructions (536/550, 630/635, 553/568). Retail's count includes switch-table data that the linear disassembler decodes as code. Codex's START_HERE says to decide a match by masked bytes plus the relocation audit (cx: `START_HERE_CLAUDE.md` step 5). None of our drafts' notes show this misread.
