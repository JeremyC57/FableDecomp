# Landing nf159 (Account A, 2026-11-20): tavern game / carrying / AI creature names (47 rows)

Agent NF159 (round 10) handed back 21 entries (9 done, 4 done-already, 7 blocked, 1 deferred); its files are in
`naming/2026-11-20_agentNF159/`.

- 47 rows: 30 respelled in place and 17 ported onto shared headers (the tavern game headers, CTCHeroStats.h, CDefPointer.h,
  stlport/fable_containers.h). Callee families moved whole, so each address keeps one spelling: 0x008EF190, 0x008EF390, 0x007D2EF0,
  0x008F09C0, 0x004C10E0, 0x0088C160, 0x00436AC0 (onto the existing vector<CThing*>::_M_insert_overflow spelling).
- Self renames (18, RENAMES section 1): the shove-ha'penny, coin golf, card pairs and card base members (0x007D3510 vfunc_32 ->
  NewRound, 0x008F09E0 TurnAllCards -> CardsFinishedMoving, 0x007D2900 stand-in -> CTCTavernGameCoinGolf::NewGame), CTCTavernGame::
  GetWinPhrase, CTCCarrying::IsCarryingThing / GetAllCarriedThings, CThingAICreature::SetNewBrain (CDefPointer<const CBrainDef>).
- Headers (add-only declaration opt-ins, `orig` equals HEAD, default path token-identical): CTCTavernGameCardBase.h (QUITMIDGAME),
  CTCTavernGameCardBlackjack.h (TURNPLAYERCARDS, REORDERCARDS), CTCTavernGameCardPairs.h (CARDSMAINGAME).
- Recheck (`logs/recheck.log`): 47 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 214 good, 0 bad. tablecheck: 0x0041649C shows a third spelling. It comes from 0x005EADC0, whose text declared it
  at HEAD too (the binding was missing from the HEAD table). Queued.
- Link (`logs/link_landing.log`): 8 LNK2005 vs 7 at HEAD (`logs/link_control_head.log`). The extra one,
  CDefString::GetString, comes from 0x007CF530, which now uses FABLE_RETAIL_CDEFSTRING_FORCEINLINE_GETSTRING. Its pick-any copy is
  byte-identical to the owner 0x00415D70's no-duplicates definition. The clash already exists at HEAD: landed 0x007D7FD0 with the
  same opt-in against the same owner (`logs/link_control_head_getstring.log`). Queued as DEBT.
- Queued: the TU-emitter rule for ??_G stand-ins and per-row GetAllCarriedThings copies (q4047, q4049, q11671); the 0x008F1CD0
  ctor family (q5079); the 0x008F7BD0 port (q4981, q11769, q11770); 12 standalone tests whose local copies keep old names.
