# Chunk 669 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 2026-11-01 (chunk 669) The inline-call register choice inside an inherited member's dtor follows the caller's expression shape:
  in ~CScriptGameResourceObjectMovie (0x006E7550) the PImp CCountedPointer release reloads Info into eax only when the preceding
  call reads the player index through the inline PDB accessor (`GetPlayer(ScriptInterface->GetCurrentPlayer())`); a direct
  `CurrentPlayer` member read gives edx. A named `CPlayer&` local for the GetPlayer result puts the outer call's constant push
  after the inner call (one chained expression pushes it first).
- 2026-11-01 (chunk 669) A dtor body that equals an out-of-line sibling's body (Flush 0x00994520 inside
  ~CBufferedFileDataOutputStream 0x009947A0; SetDefaultThemeWeight 0x00886430 inside CEnvironmentThemePriorityStack::
  ProcessTransitions 0x00887B00, with its float clamp folded for the 0.0f constant) is that member called by name and defined
  plain above the caller in the TU; VC7.1 inlines it on its own, first probe.
- 2026-11-01 (chunk 669) Same-family destructor bodies are not always the setter: ~CNavQuadTreeNode tests `p != 0` before
  `p != &CNavQuadTree::BlockedNode` while the out-of-line SetTLNode family tests the blocked node first; write the dtor's own
  `if (p && p != &BlockedNode) delete p; p = 0;` in the byte order (TL, TR, BR, BL), not SetXXNode(0) calls.
- 2026-11-01 (chunk 669) Destructor labels in this band: 5 of 9 right (CTCTrophy, CTCOpinionSource, CTCMultiStrike,
  CTCElectrocutedEffect were wrong). The slot-0 ??_G's vtable in rtti-vtables.tsv settled every one in a single lookup
  (`xref.py <row>` -> ??_G at site-3 -> `grep <??_G> rtti-vtables.tsv`).

Review at landing:
- NAME_FIXES.tsv (25 rows): the four wrong-label own rows; the nine landed slot-0 ??_G stand-ins (free __fastcall
  `*_scalar_deleting_destructor` / `ScalarDeletingDestructor` members) and their `*_dtor` / `Dtor()` callee spellings, which name
  this chunk's rows; 0x00771360 (??0CTCOpinionSource takes CThing&, landed CThing*); 0x00510D40 (STLport _List_base<
  CIntelligentPointer<CThingAICreature> >::clear; respell callers 0x006DDA20, 0x006D4D40, 0x006C0FD0; 11 call sites in all
  (0x00504859, 0x0050626B, 0x00512893, 0x005142E3, 0x006C1128, 0x006D4D4E, 0x006DDA36, 0x006DDB94/BAD, 0x006DDE1F, 0x006DE0A4) --
  the other callers were not re-checked here). The landed 0x00512890 "CActiveFile::OnReadFinished" is `clear(); if (node)
  free(node);` = the out-of-line ~_List_base of the same list type by shape (unproven; not in NAME_FIXES); 0x008EDFF0 (CWillButtonHelperTimer ctor W4EHeroAbility / CThing&); 0x00A75CA0..0x00A75D60 (Set?Node are protected,
  IAE; landed QAE, caller 0x00A76060).
- 0x00634190 CPlayer::RemoveMode: the landed row declares `typedef long EPlayerMode` (mangles QAEXJ@Z); both PDBs list
  RemoveMode(EPlayerMode) and RemoveMode(long). The body walks list<EPlayerMode> PlayerModes, so this chunk binds
  ?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z; vland may warn on the landed spelling. Confirm which overload 0x00634190 is
  before respelling (not put in NAME_FIXES: the (long) overload's address is unknown).
- 0x00887A30 (landed CEnvironmentThemePriorityStackEntry::ProcessTransitions) uses an invented `__forceinline SetBaseWeight`
  helper; its body is SetDefaultThemeWeight(weight, true) expanded (the same plain-member shape this chunk used).
- 0x00771A90's TU defines the CTCOpinionSource ctor with the landed row's initialisers (not graded; only the implicit dtor is).
- vland WARN RELOC_NAME on 0x00A75C20 / 0x009947A0: ??1CBaseClassNonCopyable@@MAE@XZ -> 0x0099A300 whose catalog label is still
  `CBaseClass::~CBaseClass` (the established binding, engine/CBaseClassNonCopyable.h; ~CBaseClass is 0x0099A2E0) -- a catalog fix
  already raised by earlier chunks, not a source problem.
- Every row matched within 5 probes; none needed more than 4 shape changes after the first.
