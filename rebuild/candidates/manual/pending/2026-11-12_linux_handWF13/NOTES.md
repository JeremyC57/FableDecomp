## 0x00492973

## 0x00492973 CSubtitleRenderer::SetText (full 4-caption wide overload) -- RELOCATION_MATCH (15 grades)
- What made it match: the tail (found list in the dead text slot +0xC, filter +0x14, unrotated removal loop with the sentinel in edi)
  is callee-body visibility: the TU's COMDATs _List_base<const CMessageEvent*> ctor 0x00493BC2 / dtor 0x00493BDD, RemoveMessage
  0x00493B5B and FindAllMessages<IsOfType> 0x00493EAB get their bodies in the row under auto_inline(off). Per body: the list base
  ctor/dtor are the STLport header's own #else bodies; RemoveMessage is the landed row's body; FindAllMessages is the landed row's
  loop on shared headers with STLport size() = distance(begin(),end()) (FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE), byte-proven by
  grading the same source against 0x00493EAB (RELOCATION_MATCH, Repair 4).
  RemoveMessage + FindAllMessages bodies alone did nothing; adding the list base ctor/dtor bodies matched at once.
- Then ported onto shared headers (no local class models); strict-header regrade RELOCATION_MATCH. Pragmas y-off + s-on, no flags.
- Overlay opt-ins (hdr/README.md): CSUBTITLERENDERER_SETTEXTINFO, CMESSAGEEVENTMANAGER_FINDALL, CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR,
  CTCHERORECEIVEITEMS_DISPLAYINGITEMS, STLP_DEQUE_PUSH_BACK_OUTOFLINE, STLP_QUEUE.
- Names: message type 0x11 is spelled with the shared header's enumerator MESSAGE_EVENT_CONVERSATION_CLICK_PAST
  (engine/EMessageEventType.h, generated from the Ego_r enum, value 17). Only the 0x11 enumerator respelling is withheld; NAME_FIXES.tsv hands back two rows (0x00493BC2 / 0x00493BDD -> _List_base<const CMessageEvent*> ctor/dtor, Repair 3), flagged as found in no PDB. Hero component byte +0x26 is CTCHeroReceiveItems::DisplayingItems
  (Ego_r +0x32). Callee names from the dossier: GetPControlledCreature, InitPlayerMode, CFontBank::Preload.
- The 0x00493BC2 / 0x00493BDD COMDATs are modelled here by the STLport _List_base<const CMessageEvent*> ctor/dtor bodies
  (shape only: one allocator& arg, `and [out],0`, allocate + self-link the sentinel). Their retail symbol names are NOT proven.

## Verifier findings (repair pass)
- Withdrawn: the claim that landed global_ListNodeCreateCircularSentinel_00493bc2 / std_list_T_00493bdd_dtor_00493bdd are
  ??0/??1?$_List_base@PBVCMessageEvent@@...  Neither symbol is in egor_pdb_names.tsv or fablewin_pdb_names.tsv (both PDBs are
  Dinkumware builds listing only ??0/??1?$list@PBVCMessageEvent@@...); the only support is STLport code shape, one source.
  [SUPERSEDED by Repair 3: NAME_FIXES.tsv now hands back two rows, 0x00493BC2 / 0x00493BDD -> _List_base<const CMessageEvent*>
  ctor/dtor, flagged as not in any PDB; evidence = retail call graph 0x00493CCA / 0x00493CDC + STLport body shape.]
  The bodies in this row are the STLport header's own, so the byte match stands.
- Withdrawn: NAME_FIXES row 0x11 -> MESSAGE_EVENT_CONVERSATION_CLICK_PAST (replacing Clear 0x00492BAB's
  MESSAGE_EVENT_TEXT_FINISHED_2). The name has one source only (rebuild/include/engine/EMessageEventType.h, from the Ego_r enum);
  a repo-wide grep finds it nowhere else (no FableWin extract, fable_types.h, FSE). This row still uses the shared header's
  enumerator (that is the header's name, not a new one); the coordinator may reconcile Clear when a second source appears.
  [SUPERSEDED by Repair 3: NAME_FIXES.tsv is no longer header-only; it holds the two _List_base rows above. The 0x11 enumerator
  row stays withdrawn.]

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

## Repair 4 (verifier findings 1-2) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), vt.py PASS, addmatch rerun
- Finding 1: the carried FindAllMessages body graded DIFFER(81v81) against 0x00493EAB (ours `mov ecx,[ebp+0Ch] ; mov edx,[ecx]`,
  retail `mov eax,[ebp+0Ch] ; mov edx,[eax]` at 0x00493EE4) because the default shared list::size() is a member for-loop.
  Fix: `#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE` -- an EXISTING opt-in of the repo's stlport/fable_containers.h (chunk 849,
  row 0x0057E2F1): size() = `size_t result = distance(begin(), end()); return result;` with STLport 4.6's free __distance
  input-iterator loop, i.e. the real STLport spelling that the landed row's free Distance(first,last) models. First grade:
  0x00493EAB RELOCATION_MATCH (strict too), 0x00492973 RELOCATION_MATCH (strict too); 0x00493B5B / 0x00493BC2 / 0x00493BDD still
  RELOCATION_MATCH from the same source (grade bc2/bdd with the mangled leaf prefix ??0?$_List_base / ??1?$_List_base; the bare
  `_List_base` leaf picks the wrong symbol and shows a spurious DIFFER). The 3-argument distance(first,last,n) form was not needed.
  No overlay header change (the opt-in is in the repo header already).
- Finding 2: first section now states per-body provenance; the two 'Verifier findings' bullets that said NAME_FIXES is header-only
  are marked superseded by Repair 3; the source comment says the same.

