## 0x00881A40

## 0x00881A40 NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags -- RELOCATION_MATCH (13 grades)
- Matched after porting onto shared headers (FULL stack branch, CThing, CTCGraphicAppearance MOVEMENT, CIntelligentPointer ASSIGN, fable_map set/tree opt-ins) plus: set find result in a block-scoped local; loop-1 restore = inline CThingGraphicByPassFlagState::Reset (PDB); loop 2 tests the thing via one `const CIntelligentPointer<CThing>&`; last 2 hunks (node in eax vs edi) closed by giving the map's tree find its body visible but noinline (FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE) -- a Q32-style callee-visibility fact.
- Needs overlay hdr/ (3 opt-ins, see hdr/README.md). No flags. NAME_FIX hint: landed 0x00883860 `_Rb_tree<CUnprovenKey_00883860,...>::find` is map<CThing*, CScreenFilterEntry::CThingGraphicByPassFlagState>::find (key/value proven by this row).
- Repair (verifier findings): the block-scoped set-find iterator is load-bearing and now cited in the source (retail 0x00881A49 lea esi,[ecx+0A0h]; 0x00881A54 lea ecx,[esp+10h]/push/mov ecx,esi; 0x00881A64 mov edx,[esp+0Ch]; cmp edx,[esi]); spelled inline = 4 hunks, function-scope local = 22 hunks (ATTEMPTS.log r1/r2). NAME_FIXES 0x00883860 now cites FableWin.pdb _Tree::find (_fablewin_api 0x01817856/0x02A5B6F0) + Ego_r map::operator[] (egor 0x008C43B0 = retail 0x00884C70). Regraded RELOCATION_MATCH (also strict); vt PASS.
- Repair 3 (verifier findings, still RELOCATION_MATCH incl. LANDVERIFY_STRICT_HEADERS=1; vt PASS):
  (1) hdr/README.md now documents that overlay fable_map.h line 201 `#ifdef FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE` is rewritten into the `_DECL` / `_NOINLINE && _MSC_VER` / `FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE` `#if/#elif` chain, and that the existing opt-in arm and the default `#else` are token-identical to the repo. The overlay was also resynced to the CURRENT repo fable_map.h (it had dropped the newer FABLE_RETAIL_STLP_MULTIMAP_EQUAL_RANGE block); diff vs repo is now only line 201.
  (2) noinline evidence cited in the source and overlay: retail calls 0x00883860 out of line at 0x00881ABB (inline default spelling = 609 bytes / 32 hunks); with the find declaration-only (_DECL) the node lands in eax (0x00881AC0 mov eax,[esp+10h]; 0x00881AC4 cmp eax,[ebx]; 0x00881AC8 lea edi,[eax+14h]) where retail has mov edi,[esp+10h] / cmp edi,[ebx] / add edi,14h (2 hunks). Graded x_inl / x_decl / x_def (ATTEMPTS.log).
  (3) NAME_FIXES.tsv: full STLport mangled + demangled spelling of 0x00883860 (_Rb_tree<CThing*, pair<CThing* const, ...CThingGraphicByPassFlagState>, _Select1st<...>, less<CThing*>, allocator<...> >::find<CThing*>(CThing* const&)), taken from the matched object's external, plus the full FableWin.pdb _Tree::find and Ego_r.pdb map::operator[] mangled names. Both PDBs mangle CThingGraphicByPassFlagState with class key V, so the overlay now declares it `class ... { public:` (bytes unchanged; the symbol now matches the PDB key).

## 0x00492973 (not landed)

## 0x00492973 CSubtitleRenderer::SetText (full 4-caption wide overload) -- RELOCATION_MATCH (15 grades)
- What made it match: the tail (found list in the dead text slot +0xC, filter +0x14, unrotated removal loop with the sentinel in edi)
  is callee-body visibility: the TU's COMDATs _List_base<const CMessageEvent*> ctor 0x00493BC2 / dtor 0x00493BDD, RemoveMessage
  0x00493B5B and FindAllMessages<IsOfType> 0x00493EAB get their real (STLport / landed) bodies in the row under auto_inline(off).
  RemoveMessage + FindAllMessages bodies alone did nothing; adding the list base ctor/dtor bodies matched at once.
- Then ported onto shared headers (no local class models); strict-header regrade RELOCATION_MATCH. Pragmas y-off + s-on, no flags.
- Overlay opt-ins (hdr/README.md): CSUBTITLERENDERER_SETTEXTINFO, CMESSAGEEVENTMANAGER_FINDALL, CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR,
  CTCHERORECEIVEITEMS_DISPLAYINGITEMS, STLP_DEQUE_PUSH_BACK_OUTOFLINE, STLP_QUEUE.
- Names: message type 0x11 is spelled with the shared header's enumerator MESSAGE_EVENT_CONVERSATION_CLICK_PAST
  (engine/EMessageEventType.h, generated from the Ego_r enum, value 17). No rename of landed code is handed back (see below). Hero component byte +0x26 is CTCHeroReceiveItems::DisplayingItems
  (Ego_r +0x32). Callee names from the dossier: GetPControlledCreature, InitPlayerMode, CFontBank::Preload.
- The 0x00493BC2 / 0x00493BDD COMDATs are modelled here by the STLport _List_base<const CMessageEvent*> ctor/dtor bodies
  (shape only: one allocator& arg, `and [out],0`, allocate + self-link the sentinel). Their retail symbol names are NOT proven.

## Verifier findings (repair pass)
- Withdrawn: the claim that landed global_ListNodeCreateCircularSentinel_00493bc2 / std_list_T_00493bdd_dtor_00493bdd are
  ??0/??1?$_List_base@PBVCMessageEvent@@...  Neither symbol is in egor_pdb_names.tsv or fablewin_pdb_names.tsv (both PDBs are
  Dinkumware builds listing only ??0/??1?$list@PBVCMessageEvent@@...); the only support is STLport code shape, one source.
  No NAME_FIXES row is handed back for them; the bodies in this row are the STLport header's own, so the byte match stands.
- Withdrawn: NAME_FIXES row 0x11 -> MESSAGE_EVENT_CONVERSATION_CLICK_PAST (replacing Clear 0x00492BAB's
  MESSAGE_EVENT_TEXT_FINISHED_2). The name has one source only (rebuild/include/engine/EMessageEventType.h, from the Ego_r enum);
  a repo-wide grep finds it nowhere else (no FableWin extract, fable_types.h, FSE). This row still uses the shared header's
  enumerator (that is the header's name, not a new one); the coordinator may reconcile Clear when a second source appears.
  NAME_FIXES.tsv now holds the header line only.

## Repair 3 (verifier findings 1-4) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), vt.py PASS, addmatch rerun
- RemoveMessage 0x00493B5B: comment no longer calls it a template; defined `inline __declspec(noinline)` (header-inline member that
  VC7.1 emitted out of line, Ego_r COMDAT 0x0049CCAB in SetText's TU), body = landed CMessageEventManager_RemoveMessage_00493b5b.cpp
  verbatim except the node typedef (shared STLport list has no `_Node`; std::_List_node<CMessageEvent> named directly). First grade matched.
- Provenance comment per carried body: _List_base ctor/dtor = STLport header #else bodies (NOT the landed made-up models
  global_ListNodeCreateCircularSentinel_00493bc2 / std_list_T_00493bdd_dtor_00493bdd); FindAllMessages = landed
  CMessageEventManager_FindAllMessages_00493eab.cpp loop re-spelled on shared headers (iterators, push_back, size()).
- Names: callee line marks the _List_base spelling UNPROVEN by PDB and cites Ego_r ??0/??1?$list@PBVCMessageEvent@@ (0x0049C87F /
  0x0049CB20). New evidence: retail 0x00493CCA (list(const allocator&)) calls 0x00493BC2 and 0x00493CDC (list dtor) is
  `jmp 0x00493BDD` => both are base-class members. NAME_FIXES.tsv proposes the _List_base<const CMessageEvent*> symbols for the two
  landed rows, flagged as not in any PDB.
- Pragmas kept (every landed sibling of the TU uses optimize("y", off) + ("s", on), none uses addmatch flags); justified in the source
  header by 0x00492973 push ebp/mov ebp,esp, 0x00492BA7 leave, 0x004929C5 `or [esi+2Ch],-1`, 0x00492A77 fldz/fstp, 0x00492A85
  `and [esi+24h],0`; auto_inline(off) by the four out-of-line call sites 0x00492B66/7C/91/9F.

