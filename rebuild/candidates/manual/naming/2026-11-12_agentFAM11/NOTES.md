# agentFAM11 notes (2026-11-12)

## Part 1
- land1011do: all three held DeleteData rows ported off their local models. 0x00718080 is CCountedPointer<NTCScriptedControl::CScriptConversation>
  (RTTI vtable 0x01265520 slot 0 = 0x00718840; Ego_r/FableWin ??1CScriptConversation@NTCScriptedControl@@UAE@XZ); the repo has only
  ::CScriptConversation, so a new header engine/NTCScriptedControl_CScriptConversation.h is in the overlay.
- land1011dp: 0x008147D0 is CTCBoastingPosition::SetBoastingPodium(CThing&) (binder 0x00814260 already spells it). The ForEachInRoughArea
  claim holds for every Ego_r / FableWin instance (QBE, ABV Proc); 0x0063E590 respelled through an opt-in.
- NF27 leftovers not done (cost > cheap): the six CUserProfileManager::Get binders (they reach the class through stand-ins), the
  CPlayer::SetupInitialPlayerMode 0x004887C0 / InitModes 0x00636E50 self names (need a FABLE_RETAIL_PLAYER private-member opt-in), the
  0x013CAA28 / 0x013B7D4C global stand-ins (owning headers unproven), and the CCountedPointer.h comment (see hdr/README.md).

## Part 2 (CUnproven family)
- 115 of 438 rows respelled; 79 of 382 ids fully proven (plus id 005f8c1f partly: 2 of 3 rows). 323 rows left, each with a reason in LEFT.tsv
  (120 keep FAM07's LEFT.tsv reason; 187 have no real-typed caller binding and no Ego_r member / PDB tie; 16 named one by one).
- Evidence rule used per id: (1) a landed real-typed caller that calls the row and binds that exact spelling (rebuild/oracles/
  reloc_bindings.tsv), or the ctor+0x40 _M_erase link FAM07 proved; and (2) an Ego_r.pdb member type or a FableWin.pdb / Ego_r.pdb
  _Tmap_traits / _Tset_traits name set for the same element types. Every row's `// Types (proven ...)` block names both.
- Routes: local-text substitution of the landed STLport model (iterator ops, __distance, _M_create_node, find, _M_insert...), or the shared
  stlport/fable_map.h (+ fable_containers.h) when the element is a nested class or needs vector (0x004C65D0, 0x00882EF0, 0x00B2EB20 const
  member-template find, 0x0056D130, 0x00553520 member-template find).
- Forward declarations only (no new local models) for value classes in the local-text rows; VC7.1 accepts the incomplete pair members in
  iterator ops / __distance (host g++ does not -- its ADL instantiates pair -- so syn.sh errors there are expected).
- Changed traits: 0x006DA200 and 0x00AD0A70 are _Const_traits post-increments (the callers bind const iterators); 0x00B2EB20 and 0x00553520 are
  member-template finds (QBE / QAE as bound), not the plain find the placeholder spelled.
- Local models kept as landed (renamed only, none added): 0x005BAA41 keeps its landed local CDefPointer text although engine/CDefPointer.h
  exists (pre-existing; a port is a separate task); 0x0086F340 uses the caller 0x0086FD20's CExpressionSet text (no shared header exists);
  0x0098FF50 / 0x0098FFA0 use the same CThreadedFileManager outline FAM07's 0x009904B0 / 0x00990540 use (no shared header).
- Gaps for the coordinator:
  - 0x007776A0 equal_range (not in ROWS.tsv) still spells the 00776aa0 placeholders in code; that id is now map<long, CShopItem>.
  - 0x0053C97C (list<NUISystem::CList*>::insert) is landed as the stand-in `_Construct_single`; 0x0053CA3C now binds the real insert
    spelling for it (RENAMES section 2).
  - Section 2's other lines are binders inside the family (callers and callees both respelled here), plus 0x0063E590.
  - FAM07's proven rows (e.g. 0x00835140, 0x00ADB200) mention the old placeholders in comments only.
- Tests: the catalog test is copied with placeholders renamed to TestKey / TestValue / TestKeyOfValue / TestTraits (`_M_payload`); the
  handwritten respells (0x005BAA41, 0x00A52210, 0x0086F340, 0x0053CA3C, 0x00AC4290) keep the catalog test unchanged (its comment / local
  stand-in names still say CUnproven*; the test passes and binds by address).
