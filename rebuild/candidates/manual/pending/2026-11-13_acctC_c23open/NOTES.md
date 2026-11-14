# agentC23 NOTES (acctC lane, w22)

## Row 1: 0x00601310 CTCInventoryMap::ConstructQuestsScreenPC (catalog label ConstructQuestsScreen_MainMenu is wrong)
- Name: Ego_r.pdb 0x00655BC0 + FableWin.pdb 0x020ABAB0 ?ConstructQuestsScreenPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z;
  reloc_bindings.tsv binds it at 0x00601310 (caller CManager::Action 0x0041E6D3). Overlay opt-in FABLE_RETAIL_CTCINVENTORYMAP_QUESTSSCREENPC (decl).
- Draft port: DIFFER(6962v6931), 179 hunks; sole decision residue = last end-of-scope ~CCountedPointer<CComponent> (pastEntry)
  retail CALLS 0x00429C53, ours expands (pen 0 at a+g 8638; needs pen 27 => a+g >= 9327 at the LAST end dtor only).
- /d2inlT sweep: T70 calls too many (6197), T71..T80 no change; /d2inlS5/20 no change. A threshold flag cannot isolate the site.
- LEAD (measured, inline trace tools/inl.sh + tools/ag.py): dead-arm ASSERTs (FABLE_RETAIL_ASSERT_DEAD_ARM form) inside inline
  CCountedPointer accessors ADD to the caller's a+g (dead IL counts for the budget, not for sz). FableWin /Od sizes show an assert
  in Get (+13i = one M), operator-> (+17i), IsNull (+M), copy ctor (+M), ReleaseData (+4M+4i), ctor(T*) (+18i).
  First end-dtor a+g: base 8302; +Get 8518 (+216); +arrow alone -> 9354 (overshoot: all end dtors called); full set 10156.
  Window: first end dtor in [8991, 9047) (7 end dtors, 56 apart; only the last >= 9327).
- Per-member first-end-dtor a+g (agentC6 dead-arm form, +FABLE_RETAIL_ASSERT_DEAD_ARM): get +216, ctor(after if/else) +378,
  isnull +12 (2 sites), copy 0 (unused), arrow +1052, ReleaseData(2 arms) -> 9548 (overshoot). Row arms ~+18 each.
  Row-level asserts change ONLY the budget (n6 == a6 byte for byte); accessor arms can also change codegen (ctor0 v ctor).
- DIAG budget states (hdrx = differ/CCountedPointer_diag.h, NOT for hand-back): get,ctor,isnull + 6 row arms -> last dtor
  called, 6926v6931 118 hunks; best: get,ctor0,isnull + 12 row arms (differ/00601310_diag.cpp) 6901v6931, 95 hunks (24 non-frame),
  call count 272 = retail.
- Clean source fixes found on top of the budget (keep): `if (!x.IsNull())` null tests (sibling ConstructQuestsScreen spelling;
  an asserting Get forbids `if (x.Get())`), `description` declared before the `textBank` reference (retail zeroes description
  between the PMainGameComponent load and `mov ebx,[eax+14h]` 0x00602780..0x0060278D), named `title` for
  `frame->AddPositionChild(title)` (m2: -2 hunks).
- Residue of the DIAG best (all register / frame, no call differences):
  (a) CAction ignoreUp slot: retail [esp+48h], ours 50h (16 sites; 0x006014D2..); end-phase textBank home retail 40h ours 48h,
      description retail 50h ours 40h.
  (b) retail keeps `menu` in ebp from block 4 (0x006023D6) through title FindChild (0x006026A8) to ConstructPastQuestsListPC
      `push ebp` (0x00602708), then edi to 0x006027DB; ours reloads menu and CSEs the DeleteData<CComponent> address 0x00428B10
      into ebp (`mov ebp,offset` 0x00602726, used by the pastList and curList (T*) ctors); retail uses immediates.
- DECISION NEEDED (coordinator/owner): which device may realise the budget? Evidence for asserts: FableWin.pdb /Od sizes of the
  accessors = default text + one assert unit (Get, IsNull, copy ctor, operator=), ctor/arrow/ReleaseData larger; the row itself
  FableWin 2048i v ours 1897i (+151i). But the exact assert set/expressions are not pinned (fitting).
  Tried and rejected: /d2inlT (70 calls all end dtors, 71+ none), /d2inlS, `#pragma inline_depth(0)` around pastEntry decl (dtor
  state is taken at the closing brace), inner block + inline_depth(0) at return (dtor called but frame 0x68 and early code moves).
- Budget states (m3 base = FABLE_RETAIL_CCOUNTEDPOINTER ctor text, needed? no: clean best is without it): row-arm knob k with
  get,ctor0,isnull: k2-6 none called (6979), k7-9 ONE called = retail decision (6943v6931, 109 hunks, 38 non-frame:
  differ/00601310_diag.cpp), k10-12 two called (6901, 95 hunks: fewer hunks but WRONG call list), k13+ more.
  In the right state the residue is register choice in the tail (0x006026A8..0x00602A80: menu / viewport / textBank pieces,
  DeleteData<CComponent> address CSE'd into ebp in ours, immediates in retail) + the frame slots above.
- Clean differ/00601310.cpp (no asserts): DIFFER(6958v6931) 177 hunks. Grades used on row 1: ~170 (ATTEMPTS.log) incl. sweeps.
- Row 1 parked (budget device needs a decision; tail register residue depends on it). Moved to row 2.

## Row 2: 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC
- Draft port: enumerator respelled QUEST_CARD_PREREQUISITE_RENOWN -> QUEST_PREREQUISITE_RENOWN (today's CQuestCardDef.h, "both
  original PDB enum records"). Draft (6 `#pragma inline_depth(0)` devices) DIFFER(12357v12356) 627 hunks; + the landed sibling
  ConstructQuestList 0x0061B610's opt-in set (RELEASEDATA, SMART_POINTER_ACCESSORS, GETTC/PEEKTC_ORIGINAL_CHAIN, ...): v2 12340
  610 hunks; same without the pragmas v3 12404 600 hunks.
- KEY (second TU, corroborates row 1): v3 + FABLE_RETAIL_ASSERT_DEAD_ARM + a dead-arm ASSERT in CCountedPointer::Get (hdrx
  mkx.py set `get`) and NO inline_depth device = DIFFER(12356v12356) = retail size, call count 464 = retail (v2 with the six
  pragmas: 466). Adding isnull/ctor/arrow arms: identical bytes. So the six "caller-size" sites are explained by the dead IL of
  an ASSERT in Get (FableWin Get = default text + exactly one assert unit 13i/35b).
- TU consistency check (important, against the key finding): the landed sibling ConstructQuestList 0x0061B610 (RELOCATION_MATCH
  as landed) breaks with any Get or ctor arm (12287v12341) -- BUT matches again when its fitted budget opt-in
  FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND is dropped (get / ctor / get,isnull: RELOCATION_MATCH; get+ctor overshoots). Row 2
  without REBIND needs get,ctor,isnull (12356v12356) or get,isnull + 12..14 row arms (differ/0061e6d0_diag.cpp). No single
  CCountedPointer arm set fits both rows without row-level arms, so the row-own ASSERTs remain fitted counts. 0x005FE090 is
  unaffected by get,ctor,isnull (RELOCATION_MATCH either way).
- Clean differ/0061e6d0.cpp = DIFFER(12340v12356) 610 hunks (keeps the six inline_depth(0) devices). DIAG twin 599 hunks,
  residue frame packing over the whole 12 KB body (frame 0x424 v 0x41C at the draft, slot permutation from 0x0061E6E0) + 169
  non-frame hunks (register choice). FableWin /Od row size is +801i (Dinkumware STL in FableWin): no use as an assert count.
- Grades used row 2: ~45.

## Row 3: 0x0058578B CTCInventory::OpenInventory (catalog label vfunc_30)
- Name proven: Ego_r.pdb 0x005F4D20 + FableWin.pdb 0x0212841C ?OpenInventory@CTCInventory@@UAEXXZ; CTCInventory vtable 0x01250DE4
  slot 30 = NInventory::CTCInventoryBase::OpenInventory slot (every sibling's slot 30 is its OpenInventory); only reference is the
  vtable slot (acctB DOSSIER, rtti-vtables.tsv).
- Ported: 4 stale opt-ins re-applied (OPENINVENTORY, ISCHILD, JOYSTICKS) and QUESTTEXT/QUESTNAME replaced by today's
  FABLE_RETAIL_CTCQUESTCARD_ACCESSORS + _STATGETINTERFACETYPE (same declarations). Grade reproduced: DIFFER(7948v8081) 377 hunks.
- /Os TU (every landed CTCInventory_* row of 0x0058xxxx carries `#pragma optimize("s", on)`). CCountedPointer dead-arm asserts:
  no effect (row uses FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR; different inliner regime). Not worked further (context).
- Residue from the prologue: frame 0xE8 v retail 0xF4 (12 bytes), [ebp-14h] v [ebp-8], top_level esi v edi, `push 4 ; pop edi`
  TC-key constant. Nearest landed twin: OpenPCInventory 0x0058F649 (same prologue idioms) -- start there next time.
