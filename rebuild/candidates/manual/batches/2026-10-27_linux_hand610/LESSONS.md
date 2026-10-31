# Chunk 610 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Landed siblings are the strongest lever for parked rows: 0x00A10A60 TokenizeW matched by copying the landed free-function
  WordWrap_TokenizeW 0x00A10720 loop (inlined PDB `WordWrap_CanBreakLineAt(cursor, 0)` + `for (remaining = wcslen(); remaining
  > 0; ...)`), and 0x0062B7C0 matched once `player = GetPlayer(event->Player);` was re-assigned exactly as the landed sibling
  EAControlledCreatureUseAbility 0x00629C40 does. Both had been called register-colouring gaps. Grep the same class / same
  library family in rebuild/src/compiled before tuning.
- A `for (n = count(); n > 0; ...)` loop guard (instead of an explicit `if (n <= 0) return count;`) makes VC7.1 materialise the
  count's zero in a register (`xor esi,esi`) shared by the count store and the skipped-loop return (0x00A10A60).
- Third-party rows: diff the parked body against the vendor file first. LZO 1.08 swd_accept's verbatim `while (n--)` matched
  where the parked `if (n) do ... while (--n)` moved n into esi before the test and swapped an index (0x00C071B0).
- Re-prove every callee spelling: 0x0062B7C0's parked file had 9 wrong spellings (CPlayer::GetHeroThing instead of the const
  GetPControlledCreature, public GetPlayer(long) instead of protected GetPlayer(unsigned long), pointer-returning
  CThing::GetPlayer, non-const GetThingInPrimarySlot, ...). The const/reference fixes together with the sibling shape changed
  the register colouring.
- A by-value C2DCoordI `operator-` whose result ctor takes `(X - o.X, Y - o.Y)` evaluates Y first and also re-orders the
  following colour-byte stores and pushes like retail (0x00637C80, 38 -> 8 diff lines); member-by-member assignment and a
  two-argument local ctor evaluate X first.
- x87 enregistration: in a call-free loop VC7.1 puts ONE float on the x87 stack. Taking the line width through an STLport-style
  `const T& max(const T&, const T&)` stops it being the candidate and the maximum height is chosen instead, as retail does
  (0x00AB7B00), but the reference return adds an address select; a struct or an inline `KeepMax(T&, const T&)` is scalarised
  and changes nothing.
- 0x00733CF0: re-reading `found->second.first->SecondsDuration` at the end (no data local live to the end) gives retail's
  length and reload but costs a dead frame word; keeping the data local to the end keeps the frame but drops the reload.
- Residues no lever moved (park after 2-3 grades): x87 operand choice with the first product kept on the stack (0x0080DB70, 7
  forms), the if-conversion of a final return-a/return-b pair where retail tail-merges (0x00934F20), two-load scheduling
  (0x005B6767, 0x00637C80), equal-use register colouring with stores sunk past pushes (0x004B1A80).

Review at landing:
- 0x00C071B0 keeps a same-TU stand-in `extern "C" int find_match(...)` (reduced body) as swd_accept's caller; it defines
  `_find_match` in the object next to the graded `_swd_accept` (same as the parked file). Only `_swd_accept` is graded.
- 0x00A10A60 defines the three character classes as same-TU statics (`?IsNonBeginningChar@@YA_N_W@Z` etc., native wchar_t)
  like the landed 0x00A10720; only the TokenizeW leaf is graded. Needs /Zc:wchar_t (`// Flags:` line).
- 0x0062B7C0 vland: RELOC_NAME warnings come from wrong catalog labels (0x00487DC0, 0x0048C950); the bound spellings are correct.
- NAME_FIXES.tsv (24 rows): respell landed callers of 0x00487DC0 (CPlayer::GetHeroThing users), 0x00634160/0x00634190,
  0x0049D850, 0x004C7A30, 0x006A1440, 0x006644F0, 0x0069FFD0, 0x0048C950, 0x004AF7A0, 0x004B73A0, 0x00687540, 0x00449970,
  0x009C95E0, 0x00405F00, 0x005B336A, 0x005B4EBA, 0x008128F0 (landed body spelled CTCCreatureGroupTag), 0x004C73D0,
  0x009E1DD0, 0x009DE3F0, 0x00497890, 0x006622A0, 0x00662650. The CWideString `PBG` vs `PB_W` split (0x0099B6B0) is the
  fleet-wide lane wchar_t decision and is not listed.
- Rows that took more than 6 grades: 0x00AB7B00 (9), 0x0080DB70 (8), 0x00637C80 (8).
- Parked differs still carry fleet-style stand-ins: filler virtual slots v00..v80 on CTCPhysicsBase (0x0080DB70), the this-less
  `GetWorld_00686a80` and `CTCMap_Retail` models (0x00733CF0).
