# NF179 notes (2026-11-20, round 10: returned script / TC / player GUI / UI entries)

## q11932 0x00800540 / 0x008006D0 (done)
- 0x008006D0 SaveRoute: the local CNavStepVector / CNavStep stand-ins and the `reinterpret_cast` to the PDB `vector&` are gone. The route is the
  shared STLport `std::vector<std::pair<C2DVector, unsigned char> >` over the shared byte-packed C2DVector (engine/C2DVector.h
  FABLE_RETAIL_C2DVECTOR; 9-byte pairs, retail `imul 0x38E38E39 ; sar edx,1` = /9). The row-local unpacked C2DVector model is dropped.
  Retail releases with a null test (0x0080086E `cmp eax,edi ; je`), the default ~_Vector_base branch. RELOCATION_MATCH form 1.
- One definition: the new object emits the vector / pair / allocator COMDATs pick-any. Compared (bytes with relocation fields masked) against
  the HEAD objects of all 14 landed rows that name `pair<C2DVector, unsigned char>`: every shared symbol SAME (work/q11932_comdat.txt;
  tools/comdat.py). The headers that name the type (CNavigatorBase.h, CNavigatorFlyer.h, CTCCreatureNavigation.h) only use it in
  reference parameters (no instantiation).
- The row keeps its other pre-existing local models (CBaseIntelligentPointer / CIntelligentPointer / CWorldMap / CPersistContext / CTCBase /
  CTCPreCalculatedNavigationRoute), unchanged; vland warns SHARED_HEADER_AVAILABLE for four of them, as at HEAD.

## q11945 / q11946 NUISystem::CDeletion ctors (done)
- NF174's census (agentNF174/work/probe_ui.tsv) covers every REFS row of both targets; only 0x0059DB82 / 0x0059E6BD bound the
  `N_0042bed4::CAction` stand-in. Both ported here in one form each (the 0x005A887D recipe): local NUISystem::CComponent vfunc model,
  CChildVector / SChildPointer, the CAction stand-in and the local CCharString / CManager / CList replaced by engine/NUISystem_CComponent.h,
  NUISystem_CDeletion.h (FABLE_RETAIL_CDELETION), NUISystem_CList.h, NUISystem_CManager.h, fable_string.h. Virtual slots map by offset to the
  header names (+0x20 FindChildNonRecursive, +0xD4 GetChildren, +0xF0 AddChildDynamically, +0xF8 SetDeletion, +0xFC GetDeletion, +0x104
  GetType, +0x174 MakeUnavailableDynamically, +0x17C MakeAvailableDynamically); the old `*GetAmountLeft() == 0` is
  `GetDeletion().Method == UI_DO_NOT_DELETE`.
- Binding diff HEAD vs new (all pairs): only 0x0042BED4 / 0x0042CD84. Defined-symbol diff: the implicit ??1CDeletion (8 bytes) and the
  shared vector<CCountedPointer<CComponent> > operator[] / begin / size, CCountedPointer operator-> (pick-any). Each compared against 7
  sampled landed CDeletion users (0x005A887D 0x005A9A87 0x005729D8 0x0053B321 0x00627400 0x005AF64E 0x00573792): SAME. (Older pick-any
  list / tree helper COMDATs differ between /Os and /O2 TUs at HEAD already; not changed here.)
- Both rows keep their pre-existing local CTCInventoryTrade model (in place; SHARED_HEADER_AVAILABLE warning as at HEAD).

## q11950 GPreviousSelected 0x013B8A98 (blocked: header claim + port)
- Census re-run (work/probe_q11950.txt): 1 row binds the protected PDB `1V` (0x0054AC90), 16 bind the public `2V` from
  engine/NUISystem_CList.h's default branch (needs header claim: flip to protected + the public fastcall GetPreviousSelected accessor,
  FableWin.pdb), 17 bind stand-ins. Each stand-in row also reaches the list's clear 0x004855A3 (or other callees) through more
  stand-ins (?clear@CNodeList@@ / ?clear@CListRegistry@@ beside the PDB _List_base clear), so each port is a whole-row port with its own
  callee census. Not attempted for context. 0x00555CF0 is NF176's (not compiled).

## q11975 0x00437570 CPlayerGui::ChangeGraphicsBankScene (done)
- Self row re-authored on engine/CPlayerGui.h (it was the gapscan `T::Run`); both landed referencers already bind the PDB spelling
  (leaf-only self rename, narrows a self-vs-callers split). RENAMES s1 carries the catalog fix.
- **Coordinator: one const_cast.** Ego_r.pdb types `CPlayerGui::Graphics` (+0x14) as `CGraphicDataBank*` (struct_layouts_egor.tsv), but
  engine/CPlayerGui.h (FABLE_RETAIL_CPLAYERGUI_CTOR) declares `const CGraphicDataBank*`. ChangeScene is non-const (both PDBs QAE), so the
  row calls through `const_cast<CGraphicDataBank*>(Graphics)` (marked `// Fidelity: cast, header debt`). Retyping the member is a
  coordinated header item (needs header claim; it changes PeekGraphicBank's body and the ctor row 0x0043B570's store, not their bytes).

## q11981 0x00687B30 IsEventButtonReleased (done)
- Census (work/probe_q11981.txt): all 17 REFS rows compiled; 0x00689DF0 / 0x00691D70 are multi-block TUs (binds.py cannot map them), read
  from their objects' symbol tables: both reference the IBE spelling. 0x00689050 (extra lock, was unclaimed) and 0x00689C40 bound the
  public QAE form; both respelled in place on their pre-existing local CInputProcessGameBase models (`protected:` + `const`).

## q11990 0x008EF190 CTCTavernGameCoinBase::EndRound (done)
- Self row re-authored on engine/CTCTavernGameCoinBase.h (the extern "C" forwarder stand-in is gone), the NewGame / NewRound sibling shape.
  HEAD compile: old leaf `_CTCTavernGameCoinBase_EndRound`, callee `_ForwardTarget_005e9aa0`.
- **New PDB spelling on a callee (count unchanged):** 0x005E9AA0 CTCTavernGame::EndRound moves from the stand-in `_ForwardTarget_005e9aa0`
  to Ego_r / FableWin `?EndRound@CTCTavernGame@@UAEXXZ` (the qualified call of the header's virtual). Exe scan (work/r_5e9aa0.tsv): its
  landed referencers are 0x007D7BA0 / 0x008F1720 (bind the non-virtual QAE form, local models: existing split, NAME_FIXES) and this row.
  Spellings before: QAE + stand-in; after: QAE + UAE.

## q12007 0x00E7FD50 GenerateSignText (done: catalog only)
- Parked row; both landed referencers bind the PDB spelling (HEAD compiles). NAME_FIXES names the files that still say RefreshCompetition.

## q12008 0x00E9E350 CPersistTraits<BriarRoseOpinion>::TransferBinaryIn (done)
- Self row moved from the free stand-in to the FableWin.pdb member specialisation (two sources for the signature: the PDB symbol + the
  body: `this` unused, ecx reloaded from [esp+4], both arguments by reference, `ret 8`). Form 1 SRC_FAIL (CPersistTraits.h needs
  engine/CDataOutputStream.h first, as the sibling 0x007AD7F0 does); form 2 RELOCATION_MATCH. The row keeps its pre-existing local
  CDataInputStream / CMemoryDataInputStream models (vland SHARED_HEADER_AVAILABLE, as at HEAD and in the 31-body sibling family).
- Uses FABLE_RETAIL_CV_INTRODUCTIONTOTROPHIESSCRIPT_ORIGINAL_OPINION_ENUM (existing opt-in; same mangled enum as the caller's empty enum).

## Definer census of the self renames (no one-definition risk)
- The new leaves of 0x00437570, 0x008EF190 and 0x00E9E350 are UNDEF in every referencer object (HEAD compiles of 0x00498490, 0x00499D90,
  0x007CF4C0, 0x007D2920, 0x00E9E270), and no other body exists in rebuild/src/compiled or rebuild/include (grep; the generic
  CPersistTraits<T>::TransferBinaryIn is declaration-only).

## Locks claimed outside rows.txt
- 0x008006D0 (was released), 0x0059E6BD (was released), 0x00689050 (new), 0x00E7F410 / 0x00E80F10 (were released; claimed for
  HEAD compiles only, untouched).

## Header opt-ins
- None. No overlay (hdr/ is empty); every row uses existing repo opt-ins (0x0071A7C0 uses the 0x0071A790 opt-ins).

## Rule slips
- One Bash call exited non-zero (an `ls` of the missing agentNF178/hdr directory, early orientation).
- One g.sh call used a relative row path (CPP precheck failed, no grade); rerun with the absolute path.
- One xdis.py call ran with `$S` unset (no output); rerun with the preamble.
- The first q11981 APPLIED note miscounted (13 for 15); corrected from work/probe_q11981.txt.

## q12011..q12014 CTeamSpawn family (done: 5 rows)
- One preamble for the ctor 0x00DCD160, the ??_G 0x00DCF6E0 (NF163 held text) and the ??1 0x00DCF700: the PDB ctor defined `inline` on
  engine/CQ_OrchardFarmRaidScript.h (CTEAMSPAWN_MAIN) and referenced from an anonymous-namespace placement-new helper (`// Fidelity:
  scaffolding`), plus the HEAD rows' inline CScriptThing ctor / dtor / operator= (form 1 without them: ctor DIFFER(80v96), ??1 DIFFER(35v94),
  CScriptThing() called instead of expanded). So ??0 / ??_7 / ??_G / ??1 of CTeamSpawn are pick-any COMDATs with identical bytes in all
  three objects (tools/comdat.py, work/ts/); no row defines the ctor strongly any more.
- Link proof (LINK= cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED; link/): ctl.log = HEAD 0x00DCF700 (strong ctor) + new 0x00DCD160:
  LNK2005 + LNK1169 (rc 145); new.log = the three new objects: rc 0 (LNK4088 only); fam.log = the five new objects + HEAD Main 0x00DCD350:
  rc 0, no LNK2005, every CTeamSpawn symbol resolves.
- Vtable 0x012DB2D4 (only referencer 0x00DCD160): ??_7CTeamSpawn@@6B@ -> ??_7CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@6B@ (RENAMES s2).
  The old 0x00DCD160 object also emitted a local-model ??_7CEntityScriptBase@@6B@ with different contents; the new one is the header's
  (SAME bytes as HEAD 0x00DCF700's).
- 0x00DCD1D0 Init ported onto the header with Main 0x00DCD350's defines (binding pairs unchanged); 0x00DCD1C0 GetParentScript re-authored
  with a Leaf line (MATCH). No landed row calls any of the five addresses (exe scan; REFS empty).
- The `(CQ_SunnyvaleMasterData*)data` conversion is the landed 0x00DCF700 / 0x00E016F0 text (both PDB-declared types), unchanged.
- Locks claimed outside rows.txt: 0x00DCF700 (unclaimed; changed), 0x00DCD350 (unclaimed; HEAD compile for the link only, untouched).

## q12037 / q12038 CMainGameComponent ForceRender / Render (done)
- 0x004164E0 respelled QAE -> IAE (protected, Ego_r + FableWin) with its only referencer 0x00417001 (RENAMES s2); 0x00417001's own object
  now defines the protected virtual ?Render@CMainGameComponent@@MAEXXZ its Leaf line already named (RENAMES s1). No other definer or binder.

## q12039 IsTimeBetween / q12044 / q12045 (catalog only / done-already)
- 0x006BB390: all 15 REFS rows already bind the PDB spelling (work/probe_q12039.txt); catalog fix in NAME_FIXES.
- q12044 / q12045: both REFS rows of each bind the private static spelling at HEAD; nothing to change.

## q12048 GetWorldMap 0x00686D30 (blocked: one port left)
- Census work/probe_q12048.txt: 30 of 33 REFS rows bind ?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ.
- 0x0071A7C0 (also q12077's referencer) re-authored onto the 0x0071A790 shared-header preamble as CTCMapwho::OnDie (UAE, the vtable slot
  0x0071A790 already binds); `// Inferred:` note on the RemoveFromMapwho text. vland warns RELOC_NAME for 0x00637A10 because the catalog
  label is DestroyMapwhoNode while the header / PDB call is CThingMapManager::RemoveThing (q12077 owns that catalog question; not reached).
- 0x007E00D0: the local CTCBase model's GetWorldMap redeclared to the PDB form (protected, CWorldMap&); only binding change is the target.
  The local CTCBase / CTCTeleporter models remain; moving the row to engine/CTCBase.h is separate work.
- 0x007DE030: blocked: port (not attempted: context). It already binds the PDB spellings of its five FlyInCircles callees; a full move onto
  engine/CTCCreatureFlyInCircles.h is needed (local models listed in STATUS).
- NAME_FIXES: seven landed rows define a non-virtual ?OnDie@CTCMapwho@@QAEXXZ for bodies at other addresses (propagated label). No link
  clash with the UAE symbol today, but they misname seven functions.
- 0x00637A10 split check (reloc_bindings oracle): before, three spellings (self leaf DestroyMapwhoNode, 0x0071A790 RemoveThing, 0x0071A7C0
  Helper2::Notify); after, two (DestroyMapwhoNode self, RemoveThing x2). Narrowed; RemoveThing is q12077's queue target (Ego_r 0x006784C0 +
  FableWin 0x02221100 per the queue line). No landed row binds 0x0071A7C0 under the old free ?CTCMapwho_OnDie spelling (oracle: none).
- Other rows still define the free ?CTCMapwho_OnDie stand-in (fastcall 0x0048D890 / 0x00444D70 / 0x00A47040 / 0x00AECBF0, stdcall
  0x00C0EAD0): pre-existing duplicate strong symbols among themselves; 0x0071A7C0 no longer joins them. Listed in NAME_FIXES (do not respell to UAE).
- 0x00417001's two RELOC_NAME warnings (GetTimeSinceLastRenderFrameStart / IsReadyToRender) are HEAD bindings (oracle), not from this change.
- Locks: 0x0071A7C0 claimed (changed); 0x007DE030 / 0x007E00D0 are rows.txt. Still held as NF179 but untouched: 0x007DE030, 0x00E7F410,
  0x00E80F10, 0x00DCD350 (HEAD compiles only); the coordinator may release them.

## Not reached
- q12049 .. q12108 (14 entries) left `open`: handed back at the q12048 entry boundary as context was heavy. Hints for the next agent:
  q12077 starts from the 0x0071A7C0 binding above (RemoveThing vs the DestroyMapwhoNode catalog label); q12094's 0x005C2118 is acctB;
  q12106 had NF174 catalog work, recheck the current catalog first.

## Descriptive names
- None introduced: every new spelling is a PDB symbol.
