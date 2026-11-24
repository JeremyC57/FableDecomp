# NF229 notes (2026-11-22, round 10): the nf228 follow-ups (q12608-q12618)

Base: the repo tree with nf228 landed. Locks NF229 (already claimed for every row touched; nflocks/<addr>/owner = NF229). No AUTH18 lock
(0x0093D270 0x00B4D370 0x00BD3070 0x00502E90), Codex pass 27 row or 0x0042BEC0 touched. Tools: NF228's (re-pathed, tools/) + bindat.sh
(compiled census of the symbols each row binds at one target). owners.tsv + 1 line (??_G pair -> 0x0042A8E0).
hdr/: one add-only opt-in (hdr/README.md). Flags lines: none. EH funclets: none. Descriptive names added: none (every new self spelling is the
STLport one the repo headers produce, descriptive-by-library; every new callee spelling is a PDB one).

## Entry 1 (q12608-q12615, q12617): the eight second-layer self rows -- done
- Each is an explicit instantiation of the header member under its binder row's defines / pragma (the nf228 0x0042C02F / 0x0042B76C /
  0x0042AB01 pattern): _M_erase 0x0042B8FC / 0x0042A264 / 0x0042A237 (binders: clear 0x0042C02F / 0x0042ACC3 / 0x0042AC76), _Rb_tree_base
  ctor 0x0042AC9F / 0x0042ACEC / 0x0042AD10 (binders: _Rb_tree ctor 0x0042B76C / 0x0042B7AD / 0x0042B7EE), _M_lower_bound 0x0042AE33
  (binder map::operator[] 0x0042D246; the self adds FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22 for the body, as 0x004B9D00; MATCH),
  _List_base<CObserver*> ctor 0x0042AC0A (binder 0x0042BE7B; the 0x0042AB01 pattern with engine/NUISystem_CObservable.h). 1 form each.
- Census (work/callers_e1.txt: E8/E9 sites): each address has one landed binder (+ the self recursion of the three _M_erase); each new
  leaf string-equals its reloc_bindings line (7 rows) or, for 0x0042B8FC (no line: the nf228-held one), what 0x0042C02F binds when compiled
  (work/bind_0042c02f.txt). Absolute references (vtable slots, data; nftools/absrefs.py, control 0x013B871C reproduces nf228's 45
  lines): none to any of the 8 (work/absrefs_e1.txt is empty). Spellings: each of the 8 addresses 2 -> 1. 0x00429E16 (~pair): 3 -> 2 (0x0042B8FC moves off
  ?Cleanup_00429e16@SubObject@@QAEXXZ onto the self leaf of the landed ~pair row; 0x0042A8E0 still binds @CAssoc_dtor@4: NAME_FIXES).
  The old placeholders: ?BuildTreeArray@Tree@@QAEXPAUNode@@@Z stays defined by 0x0042A1B6 (its table line), so the 0x0042A264 / 0x0042A237
  LNK4006 pair is gone; ?DoublyLinkedList_InitializeEmpty@@YIPAPAXPAPAXPAX1@Z stays on its byte-twin rows (0x0065F1DF ...).
- Symbol diffs HEAD v new (work/sc_e1.log): only the self leaf and shared-header pick-any COMDATs. pk.py (work/pk_e1.tsv): 0x0042B8FC's
  ~pair / ReleaseData / ~CCountedPointer EQ 0x00429E16 / 0x004291DE / 0x00429C53; its ??_G pair copy equals 0x0042A8E0's bytes (cmpcopy:
  28 = 28; relocs equal once ~pair is bound, TABLE_CHANGES) -> NAME_FIXES 0x0042A8E0; the rest NO-OWNER (allocator / alloc_proxy /
  _Destroy / deallocate / _M_root / _S_* / less / _Select1st inlines).
- pk.py with the ??_G owner line (work/pk_fin.tsv): the ??_G pair copy is EQ 0x0042A8E0 (bytes and relocs).
- recheck RELOC_NAME warnings on 0x0042B8FC / 0x0042A264 / 0x0042A237: the self recursion against the old catalog label (RENAMES moves it).
  0x00558170's RELOC_NAME (char_traits compare -> 0x00403C60 'MemCmp_Unsigned16') comes from its unchanged local wstring model (HEAD).
- RENAMES section 2: the 0x0042B8FC / 0x0042A264 / 0x0042A237 self-callee lines are the self recursion; the 0x013B871C lines are data
  references read through the header's inline Get().
- Catalog names move to the STLport names (RENAMES section 1; files not renamed, as nf227 / nf228).
- Tests: new self-contained header tests for all 8 (the HEAD tests only printed the pattern). _M_erase on the long / EUsedKeys maps and the
  CCharString map insert 5 keys in an order that gives the unbalanced tree a right subtree and a left chain (the rebalance is stubbed), so
  the right recursion and the left continuation both run; the CCharString tests define FABLE_RETAIL_CCHARSTRING (test only, as the binders
  0x0042B7EE / 0x0042B76C do in their sources) and stub the out-of-line CCharString ctors / dtor / operator< on PStringData (no string
  model). _M_lower_bound: empty / exact / between / below all / above all.

## Entry 2 (q12616): 0x013B871C -- partial (3 of 6 ported)
- Compiled census of the 28 exe referencers (work/bind_013b871c.tsv; nf228 work/absrefs_e3.txt): ?g_ users = exactly the 6 of the queue
  line; two further stand-in spellings exist (NAME_FIXES): ?System_013b871c@@3PAUSystem@@A (0x00541F20, 0x00542140) and the public-static
  @@2PAV1@A (0x00597BF2).
- Ported (1 form each, RELOCATION_MATCH): 0x00446720 and 0x00558170 drop the row-local CNewFrontendGameComponent model and read
  `CNewFrontendGameComponent::Get()->GetTextBank()` (FableWin.pdb Get / GetTextBank, both public inline; the 0x0041CFB2 pattern, default
  header path); 0x00599D5C reads `Get()->GetLastActiveJoystickDeviceNumber()` (new opt-in, hdr/README.md: FableWin.pdb name + the retail
  read 0x00599E1B as the second source). 0x013B871C: ?g_ 6 -> 3 users; total spellings 4 -> 4 (?g_ stays until its last user moves).
- pk.py (work/pk_e2.tsv, HEAD objects work/pk_e2head.tsv): the new copies Get / GetTextBank / CCountedPointer<CDataBank>::operator* /
  GetLastActiveJoystickDeviceNumber have no landed owner; the Get copy equals the unlanded out-of-line Get at 0x00405FD0 (cmpcopy: 6 = 6,
  relocs bound). 0x00599D5C's GetProfileNames (36 v 0x005955AB's 558) and ~vector<CWideString> (41 v 0x00412130's 49) copies are NEQ
  identically at HEAD (row-local models, unchanged).
- The ?g_ table line's witness 0x00599D5C moved: TABLE_CHANGES re-witnesses it to 0x0041D0A8.
- Blocked (triage, no form spent): 0x0042EBB6 Shutdown (header FileNames / FileTimes are opaque arrays; CDefPointer<const> v the row's
  non-const; local CWideString / CDateAndTime / CFrontEndDef / CRGBColour / CDisplayManager / vector models), 0x0041D0A8 PlaySoundW (PSampleBank
  has no accessor in either PDB). Open (context): 0x0059A238 (requirements in STATUS).
- Tests: 0x00558170's HEAD test models no CNewFrontendGameComponent (unchanged). 0x00446720's and 0x00599D5C's HEAD tests are standalone host
  models that keep a local `CNewFrontendGameComponent` struct (HEAD debt, unchanged): porting them needs NGameText::CDataBank / fable_string.h
  (0x00446720) or the CFrontEndManager host model (0x00599D5C) moved first; queued.
- Verifier debt seen (HEAD, unchanged): SHARED_HEADER_AVAILABLE CMainGameComponent / CKeyboard / CInputManager / CSystemManager /
  CActionInputControl (0x00446720), CMainGameComponent (0x00558170), CUserProfileManager / CTBaseSingleton / CGameJoystickManager /
  CFrontEndManager (0x00599D5C).

## Entry 3 (q12618): tests -- partial
- 0x0042BE50: new test on engine/NUISystem_CAction.h (the row's member initialisers; type, Valid, two empty distinct usable lists). PASS.
- Open (context): the 12 CManager / CObservable vtable-slot rows (0x0055CA40 0x0055C9C0 0x0055CAE0 0x0055CA90 0x0055CA00 0x0041DED2
  0x0042D592 0x0041D98A 0x0041E3B2 0x0055C950 0x0042D4F0 0x0042D5AA) keep their HEAD tests.

## Decisions for the coordinator
1. The opt-in GetLastActiveJoystickDeviceNumber rests on FableWin.pdb (name) + the retail body (0x00599E1B); Ego_r has the member only.
2. 0x00446720 / 0x00599D5C land with their HEAD host-model tests (each keeps a local CNewFrontendGameComponent struct in the test only;
   files unchanged, queued as nf228 did). A header test of 0x00599D5C cannot use the INIT-stub trick and the new accessor together (the
   opt-in is default-branch only: hdr/README.md).
3. 0x0042A8E0 (NAME_FIXES): respell as the pair's ??_G? It would also move its ~pair binding off @CAssoc_dtor@4 (0x00429E16 2 -> 1).
4. 0x0041D0A8 / 0x0042EBB6 need header claims (STATUS); the ?g_ line stays until they and 0x0059A238 move.

## Self-check
- recheck_final.log: one vland --recheck-landed run over the 11 changed rows + 0x0042BE50 (test only): 12/12, 1 MATCH + 11
  RELOCATION_MATCH, all PASS, no RELOC_CONFLICT (warnings: STANDIN_LEAF / RELOC_NAME = the catalog / table lines RENAMES / TABLE_CHANGES move;
  SHARED_HEADER_AVAILABLE = HEAD debt listed above; RELOC_ALIAS wcscmp import thunk).
- Link (work/link_check.txt): the 11 changed rows + binders 0x0042C02F 0x0042ACC3 0x0042AC76 0x0042B76C 0x0042B7AD 0x0042B7EE 0x0042D246
  0x0042BE7B + co-definers 0x00429E16 0x004291DE 0x00429C53 0x0042A8E0 0x0041E641 0x0041E1CD 0x0042CE63 0x0042A1B6 0x0041CFB2 (28 objects,
  overlay on): 0 LNK4006 / LNK2005; the same set with the 11 HEAD objects: 2 LNK4006 (?BuildTreeArray@Tree@@QAEXPAUNode@@@Z,
  0x0042A1B6 / 0x0042A264 / 0x0042A237; work/link_check_head.txt).
- Header proof: pin_asserts 0 pinned; assert_names_check 0 mismatches; equiv_check over the 26 includers 0 differing.
