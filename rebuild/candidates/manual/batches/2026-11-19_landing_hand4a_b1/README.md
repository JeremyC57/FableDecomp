# Landing hand4a b1 (2026-11-19): 0x006B5890 and 0x0047ED76 -- both INFERRED, flagged for owner review

vland --land, strict headers: 0x006B5890 MATCH + PASS (/Gr), 0x0047ED76 RELOCATION_MATCH + PASS.

**Advisor decisions (owner delegated), 2026-11-19:**
- 0x006B5890 `std::__lower_bound<pair<long,float>*,..>`: landed on hand3a's fitted text -- nested opt-in FABLE_RETAIL_STLP_PAIR_LESS_RIGHT_BYVALUE
  in stlport/fable_containers.h (pair operator< right operand by value; STLport 4.6.2 and the donor mangled name are const&). hand4a's bounded
  search for a sourced reading was empty (the caller passes the same key type, so no converting temporary; implicit instantiation / POD
  comparator no change). No landed row instantiates the opt-in. Owner's 0x006E89B0 precedent.
- 0x0047ED76 CTCInGameMenu::SetToSave: one grouping bracket `(GetBackBufferWidth() * 0.5f) - ...` (ordinary arithmetic bracketing; a no-code PAREN
  tuple moves VC7.1's list scheduler: `fsubp st(1) ; push ecx` at 0x0047EEBB). Of 16 groupings only the X left group is byte-required; the Y
  group is neutral and kept symmetric. Queued cross-check: if 0x0047D296 PCSetQuitToFrontEnd / 0x0047DD9D PCSetToLoad (same TU, same-size
  parked) carry the same centring expression and close with this spelling, upgrade the note to cross-checked; if they match only without it,
  reopen this row.
OWNER REVIEW: revert this one commit if either is not accepted.

Held from this hand4a batch: the twins 0x00853670 / 0x008539C0 (row-side `(AxisY.GetAccurateMagnitude())` wrap; one bounded test of moving the
bracket into shared helper text first). Their three new NCameraModes headers are not merged yet.

**Owner 2026-11-19:** grouping brackets are an accepted lever ("a lot of stuff needs brackets so we should allow them"; EXTRA_RULES). The bracket-only rows here need no further review.
0x006B5890 in this batch is NOT a bracket (a by-value operand): it stays for owner review.

**Review closed -- advisor decision (owner delegated, 2026-11-19: "have the expert advisor decide"):** the Inferred row(s) here stay landed.
Scope / lifetime rows (0x006B2960 inner-block local, 0x00A50C20 block scope) are required constructs whose wording alone is inferred (frame size /
slot sharing fix them) -- accepted outright under the 2026-11-12 Inferred-statements rule. Fitted tie-breakers (0x00F3ABF0 reference local,
0x006B5890 by-value operand) accepted as Inferred on the 0x006E89B0 precedent.
0x006B5890 reopen condition: a later row from the same retail object that instantiates this pair<long,float> operator< and needs the const& text.
Sibling check: the donors' objects using pair<long,float> (Ego_r / FableWin tc_volume_containment_tracker.obj, tc_register.obj, tc_overhead_display.obj)
hold only fill / copy / construct / vector / CArray instances -- no other pair<long,float> comparison to cross-check.
