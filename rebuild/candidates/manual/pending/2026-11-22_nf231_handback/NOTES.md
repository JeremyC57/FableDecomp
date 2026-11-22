# NF231 notes (2026-11-22, round 10): the nf230 follow-ups (q12625, q12627, q12628)

Base: the repo tree with nf230 landed. Locks NF231 (pre-claimed for the 10 entry rows). No AUTH20 lock (0x00B4D370 0x00BD3070 0x00502E90), Codex
pass 27 row or 0x0042BEC0 touched. Tools: NF230's (tools/, re-pathed). hdr/: two add-only opt-ins (hdr/README.md). Flags lines: none. EH
funclets: none. Descriptive names added: none (every new spelling is a PDB one or the established STLport binder spelling).

## Entry 1 (q12625): 0x00430340 SetQuit QAE -> UAE -- done
- Census: callers.py no E8/E9 binder; absrefs.py one absolute reference, 0x01230CB0 = slot 4 of the retail vtable
  ??_7CNewFrontendGameComponent@@6BCBaseClassNonCopyable@@@ (0x01230CA0, table line witness 0x0042F50B): data, not a landed row. Compiled
  vtable census (vtsyms.py, work/vt_e1.txt) of the rows that emit CNewFrontendGameComponent vtable COMDATs: 0x0042EA8F (ctor) and 0x0042F50B
  (dtor) model one slot each (slot 0 ??_E); 0x004330D2 / 0x0049DCC0 / 0x00446BE0 emit none. So no landed object binds a SetQuit spelling at
  slot 4; callers (0x0059A238, 0x00599E3F StartGame) reach it with `call [eax+10h]`. No table line names either spelling.
- Row: `#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT` + the header; `void CNewFrontendGameComponent::SetQuit() { Quit = true; }`
  (CGameComponent::Quit +0x08, protected). MATCH; symbol diff HEAD v new: only the leaf. 1 spelling after (UAE).
- Test: new header test (raw storage, qualified non-virtual call; a never-constructed subclass names the protected Quit / Running as
  pointers-to-member): Quit raised, only byte +0x08 written, already-set stays set. PassPattern PASS_00430340 unchanged.

- Vtable consequence: without the SETQUIT opt-in a header-built ??_7CNewFrontendGameComponent would bind slot 4 to the base's
  ?SetQuit@CGameComponent@@MAEXXZ (0x00415930), not 0x00430340. Any future header port of the ctor / dtor rows (0x0042EA8F, 0x0042F50B) that
  emits the full vtable must define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT.
- Finding (not fixed; existing split): the two landed vtable emitters disagree on the first vtable's name: 0x0042EA8F emits
  ??_7CNewFrontendGameComponent@@6BCBase@@@, 0x0042F50B ??_7CNewFrontendGameComponent@@6BCBaseClassNonCopyable@@@ (the table line
  0x01230CA0, witness 0x0042F50B); both emit the second as ...@@6BCGameComponentInterface@@@ (table 0x01230C94), where engine/CGameComponent.h
  (retail RTTI) has CDeviceResetCallback as the second base, so a header build would emit ...@@6BCDeviceResetCallback@@@ (work/vt_e1.txt).

## Entry 2 (q12627): 0x0042EBB6 Shutdown full header port -- done
- FileNames / FileTimes: Ego_r.pdb (struct_layouts_egor.tsv) and Ego_d.pdb (field list 0x57888, types 0x7D2B / 0x1D9E0, resolved with
  egod_type.py) type them std::vector<CWideString> / std::vector<CDateAndTime> (private). New opt-in FILENAMES_FILETIMES inside the RUN branch.
- m_pFrontEndDef: CDefPointer<CFrontEndDef const> in Ego_d (type 0x57882) and Ego_r; `= 0` is Reset(0) under the landed CDEFPOINTER_RESET (retail
  release block then the second `mov [esi+3Ch],ebx` at 0x0042EBD6).
- The two header-less stand-ins, named:
  1. CFrontendGlobal_013b838c (`->Flag38 = true` on PFontManager 0x013B838C) = CFontManager::SetLoadFontGlyphsAsynchronously(true): Ego_r.pdb
     ?SetLoadFontGlyphsAsynchronously@CFontManager@@QAEX_N@Z, inline, byte +0x38 (engine/CFontManager.h FABLE_RETAIL_CFONTMANAGER_ASYNC, the
     Run row 0x0042EC7C writes the same member with `false`). No header change.
  2. SetActiveFrontend_004336b0 = CStartupDefinitionManager::Set(0): Ego_r.pdb + FableWin.pdb ?Set@CStartupDefinitionManager@@SIXPAV1@@Z (public
     static __fastcall); the body stores into the counted pointer PClass 0x013B8760; 0x0042F5A9 binds the same spelling. New opt-in
     CSTARTUPDEFINITIONMANAGER_SET (declaration only).
- The five local models (CWideString, CDateAndTime, CRGBColour, CDisplayManager, CFrontEndDef) plus the local CNewFrontendGameComponent /
  CDefPointer / vector are gone: the row includes engine/CNewFrontendGameComponent.h, CStartupDefinitionManager.h, CFrontEndDef.h,
  CRGBColour.h (default 4-channel ctor, R G B A store order as retail), IDirect3DDevice9.h + CDisplayManager.h (FABLE_RETAIL_CDISPLAYMANAGER +
  FABLE_RETAIL_CPIXELFORMAT, the Direct3D9 types), CFontManager.h, CASoundManager.h. The only row-local declaration left is
  `extern CDisplayManager* GPDisplayManager;` (no header declares it; same as HEAD). recheck_final shows no SHARED_HEADER_AVAILABLE warning.
- Grade: RELOCATION_MATCH (same as the landed in-place version), pragmas unchanged (`("y", off)` + `("s", on)`), no Flags line.
- Callee spellings: three move, each onto what the other landed binders emit (compiled census work/bind_00412450.tsv, bind_00411b00.tsv,
  bind_004336b0.tsv): no new spelling at any address, each address goes down one; 3 table lines lose their only witness (TABLE_CHANGES).
- Relocation / symbol diff HEAD v new, pinned overlay: work/sc_fin.log. pick-any (work/pk_fin.tsv): every DEF COMDAT of the new object is owner-less (inline members with no retail out-of-line copy).
- Link (work/link_check.txt): 0x00430340 + 0x0042EBB6 + co-definers 0x0042EC7C 0x0042F50B 0x00495830 0x00495780 0x0042F5A9 0x004336B0 0x00412450
  0x00411B00 0x0042EA8F: 1 LNK4006 (??1CNewFrontendGameComponent 0x0042EA8F v 0x0042F50B), the same with the two HEAD objects
  (work/link_check_head.txt). LNK2005 0. None new.
- equiv_check.py (the standard tool) creates empty stub headers under $S/stubinc_equiv (shared, outside this folder; it existed since
  2026-11-15).
- Test: new header test (q12628): the body on the shared headers; component / font manager / display manager on raw storage; the def a test
  pointee on CDefPointeeBase (CResource / CIVCountedPointeeBase out-of-line members stubbed); the sound manager the 0x009A5A50 test's
  CASoundManager subclass; erase stubbed as explicit specializations of the declaration-only STLport member. `#define private public`
  first (test only, as 190 landed tests do) for the private members. Three cases: full teardown (def deleted on its last reference), a game
  being created without a display (no sound teardown, def kept, no clear / swap), no sound manager and no def.

## Entry 3 (q12628): tests -- 2 of 9
- Done: 0x0042EBB6 (above), 0x0041D0A8 (STATUS). Both define the private static `CNewFrontendGameComponent::m_pNewFrontendGameComponent`
  in the test (NF230's idea), so no ?g_ stand-in is modelled.
- Open (context): 0x0041D98A, 0x00446720, 0x00599D5C, 0x00542140, 0x0059A238, 0x0054F410, 0x0054F370. 0x0054F410 / 0x0054F370: see Decision 3.

## Decisions for the coordinator
1. Test access: 0x0042EBB6's test puts `#define private public` before all its includes (CDefPointer::Object, CFontManager's byte, the
   component's private members); 0x0041D0A8's narrows it to the one class header. OK for header tests (190 landed tests use it)?
2. Self rows 0x00412450 / 0x00411B00 / 0x004336B0 still emit stand-in leaves (NAME_FIXES). For the two erase rows the PDBs spell the member
   with Dinkumware _Vector_iterator (debug builds); the STLport pointer-iterator std spelling the landed binders emit is proposed. Agree?
3. 0x0054F410 / 0x0054F370 tests: the rows themselves keep local CCharString / CWideString / NGameText::CDataBank / CMainGameComponent /
   NUISystem::CText (98 placeholder virtuals) models, so a header test needs the row port first. Queue the row port (not a test item)?
4. The vtable-name split of the CNewFrontendGameComponent vtables (0x0042EA8F `6BCBase` v 0x0042F50B `6BCBaseClassNonCopyable`, and the
   `6BCGameComponentInterface` second vtable v the header's CDeviceResetCallback base): queue a census / port of the two vtable emitters?

## Self-check
- recheck_final.log: one vland --recheck-landed run, overlay on, over 0x00430340 / 0x0042EBB6 (source + test) and 0x0041D0A8 (test only):
  3/3 PASS, 1 MATCH + 2 RELOCATION_MATCH, no RELOC_CONFLICT (warnings: RELOC_NAME = catalog labels, 0x0041D0A8's SHARED_HEADER_AVAILABLE =
  its unchanged HEAD local models).
- Header proof: pin_asserts (CNewFrontendGameComponent.h 0, CStartupDefinitionManager.h 3 pinned), assert_names_check 0 mismatches,
  equiv_check over the 43 includers 0 differing.
