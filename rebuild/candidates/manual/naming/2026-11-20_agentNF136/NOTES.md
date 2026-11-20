# NF136 notes (round 10, PRIORITY slice 11 remainder: 18 queue lines)

Early hand-back (context). Counts from APPLIED.tsv / STATUS.tsv / recheck_final.log:
- Entries (18): 1 done (q4290), 3 blocked (q3887: rows 0x009E4830 / 0x009E48F0, identity; q5199 / q5220: row 0x006D41F0, port not attempted:
  context; its self identity is now two-sourced in NAME_FIXES), 14 open (q4056, q4197, q4198, q4291, q4292, q4295, q4296, q4299, q4659, q4942, q4943, q6168, q6305, q6308;
  not started, each APPLIED line carries the census / resume note). 0 done-already, 0 rejected, 0 deferred.
- Rows: 52 ported (every one RELOCATION_MATCH + PASS in one recheck_final.log run; no HARD / RELOC_CONFLICT / HEADER_POLICY), 22 not-a-binder
  (REFS rows of the three terminal entries that already bind the claims, incl. the self row 0x009E2590; 0x00499880, 0x009ACEE0, 0x00B4F510 re-probed at HEAD), 3 blocked (0x006D41F0, 0x009E4830, 0x009E48F0). 43 rows.txt rows not reached (STATUS
  "not reached"). One test rewritten (0x009BE420); every other test unchanged and standalone.
- RENAMES: 25 section-1 self renames (0x009BE420, 0x009E4710, 21 CPersistContext_TransferableComponent<T>::TransferIn rows,
  0x005D8167, 0x00679910), 52 section-2 caller respellings. NAME_FIXES: 17 lines (found on the way, outside my rows).
- Header opt-ins (hdr/README.md): CPixelFormat.h FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX (add-only static declaration);
  CPersistTraits.h partial mode FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY (token-identical without the macro). No shared-header member renamed.
- selfcheck.log: 0 problems. The SHARED_HEADER_AVAILABLE warnings in recheck_final.log are the rows' pre-existing local models (none
  added; 0x009E4420 lost its local CInputEvent model).
- Every ported row's object defines only its leaf except 0x009E4420 (inline STLport list COMDATs, see q3887) and 0x009E4710
  (?deallocate@?$allocator@U?$_List_node@VCMaintainedEvent pick-any COMDAT).
- Locks: 98 rows hold owner NF136 in $S/w22/nflocks (the 60 rows.txt rows plus claims for the ported rows outside it; 0x00686240 was
  `released`, claimed). None is `handed off` / `reserved (acctB` in UNMATCHED_ROWS.tsv. No EH funclet includes any changed row (grep).
- Coordinator decisions asked: (1) the reduce-only self rename 0x009E4710 (rows/009e4710.cpp can be dropped alone: the six q3887 binders
  do not depend on it); (2) vtable 0x01239378 now has slot 1 0x004B8660 as CPersistContext_TransferableComponent<int> (PDB) against the
  landed slot 0 0x004B8680 <ENumAcceptedBoasts> (NAME_FIXES); (3) the q4056 family reaches rows outside my locks (APPLIED census).
Order taken (brief "NF136 specifics" list read as the order): q4290, q5199 / q5220, then the not-started list in queue order,
then q4292 / q6168. Stale rows: git was not run (mtime scan instead; nf131 / nf133 landed mid-session); rows modified since 2026-11-20 00:00 (file mtimes) that are in REFS.tsv
(0x004299A8 0x00499880 0x00643CDA 0x00646762 0x00686240 0x009ACEE0 0x009C8700 0x009C8FB0 0x009FDE80 0x00A62120 0x00F37170) are
re-probed when their entry is worked; everything else uses NF133's work/NF133_round1/probe.tsv census.

## q4290 (0x009BE420 CDisplayManager::ClearRenderTarget)
- Self row moved off the CDisplayManagerClearRenderTargetView stand-in onto engine/CDisplayManager.h; RELOCATION_MATCH first grade.
- Callees: 0x009E3620 now binds ?FindFormatInfoIndexFromD3DFormat@CPixelFormat@@SIJW4_D3DFORMAT@@@Z (NF136 opt-in
  FABLE_RETAIL_CPIXELFORMAT_FINDFORMATINFOINDEX; Ego_r 0x00B35470 / FableWin 0x0308CAB0) instead of the stand-in
  FableFindFormatInfoIndexFromD3DFormat_009E3620: a PDB spelling replacing a stand-in, the address's spelling count does not grow (its self
  row keeps ?FindFormatInfoIndexFromD3DFormat@@YIJH@Z; NAME_FIXES). 0x0129BA40 now binds ?GD3DFormats@@3QBVCD3DFormatInfo@@B (already bound
  by other rows; the g_FablePixelFormatInfoTable_0129BA40 spelling disappears).
- Test rewritten on the shared headers (zeroed manager block, D3DDevice +0x58 and DepthBufferFormat +0x1AC written in place, a fake device
  whose slot 43 is the Clear fixture; GD3DFormats and CPixelFormat::FindFormatInfoIndexFromD3DFormat defined by the test). PassPattern unchanged.
- 0x00B4F510 binds the claim at HEAD (nf133 landed mid-session; re-probed): the entry is closed.

## q5199 / q5220 (0x009E2590 CPersistTraitsSLONG::TransferIn): 44 of 45 binders ported, 0x006D41F0 blocked
- 21 CPersistContext_TransferableComponent<T>::TransferIn rows (stand-ins CTransferableByte/Long_009e2590, two non-template local
  models, one public-virtual template model, one free __fastcall stand-in) re-authored as the explicit instantiation of the member on
  engine/CPersistContext_TransferableComponent.h + engine/CPersistTraits.h (the 0x00E1E0D0 body, but with the header's CPersistTraitsSLONG
  instead of a local copy). Self renames (RENAMES s1). T for each from two sources: the FableWin.pdb TransferIn (and TransferOut) instance
  name, and the vtable's owner (the CPersistContext::Transfer<T> that stores it, catalog / landed binders' spelling). The exe scan finds no
  landed row calling any of them (vtable-only).
  - 0x004B8660: catalog <int>, Ego_r.pdb ??$Transfer@H@CPersistContext@@QAEXPBDAAHABH@Z (0x004BCB00) = the landed owner 0x004B8D70; the
    landed slot-0 sibling 0x004B8680 says ENumAcceptedBoasts, a name in no PDB (NAME_FIXES).
  - 0x00CDCBE0 / 0x00CDCD90 / 0x00CDCF40 / 0x00E9E3E0 / 0x00F2D640: T = EGameflowPosition, CQ_SunnyvaleMasterData::ArcheryOpenState,
    EPlaceInHeroSouls, CV_IntroductionToTrophiesScript::BriarRoseOpinion, EFeatAttackType: the spellings the landed callers of the owning
    Transfer<T> rows bind (0x00CEF8E0, 0x00CDC070, 0x00E9E110, 0x00F2D330) plus the FableWin instance pairs. The Transfer<T> rows themselves
    still model descriptive enums (EPostSavePosition ...): existing splits, NAME_FIXES.
  - The forms that differed: the CPersistTraits<T> traits-object form also matched but emitted CPersistTraits<T>::TransferIn as an extra
    COMDAT; the explicit form (00e1e0d0 shape) emits only the leaf, so it was used.
- 0x005D8167 / 0x00679910 (both catalogued CPersistTraits<EInputKey>::TransferIn, propagated) re-authored as the explicit instantiations
  CPersistTraits<long> / CPersistTraits<EOpinion>::TransferIn: the spellings their only callers 0x009E8C10 / 0x00678760 already bind
  (closes two existing self-vs-binder splits).
- 21 rows that model CCharString / CStringParser / CPersistContext / CPersistTraits locally keep those pre-existing models and drop only
  the free stand-in declaration (and its inline helper): they include engine/CPersistTraits.h in the NF136 partial mode
  FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY (hdr/README.md) and call CPersistTraitsSLONG::TransferIn. Their own leaves are unchanged (several
  are wrong: 0x00580194 / 0x00686240 / 0x00755F40 define CPersistTraits<X>::TransferIn with pointer parameters; not in my entries).
- 0x006D41F0 blocked (port, not attempted: context): self identity CPersistTraits<std::pair<CCharString,long> >::TransferIn from Ego_r.pdb
  0x00708E10 + FableWin.pdb + its caller 0x006D4180 (NAME_FIXES); the port moves it and 0x006D4180 off the N006d41f0 stand-ins together.
- Tests: none changed (all 44 tests are standalone and PASS).

## q3887 (0x009E4710 STLport _List_base<CMaintainedEvent>::clear): self + 6 binders ported, 2 rows blocked (identity)
- Self row re-authored as `template void std::_List_base<CMaintainedEvent, std::allocator<CMaintainedEvent> >::clear();` over
  stlport/fable_containers.h + engine/CMaintainedEvent.h (RELOCATION_MATCH first grade). Descriptive-by-library (STLport names are in no PDB).
- Binders moved in place from stand-in member types (SubBuffer / MemberBuffer / CMaintainedEventList) to std::list<CMaintainedEvent>
  with FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE; 0x009E4420 also dropped its local CInputEvent model for engine/CInputEvent.h. Its object
  now also emits the pick-any COMDATs ~_List_base / ~list<CMaintainedEvent>, ~_STLP_alloc_proxy, ~allocator, deallocate and ??_I (all
  inline library code; no landed row defines them strongly).
- Blocked: 0x009E4830 / 0x009E48F0 (std::list_T_<addr>::~list stand-ins): identical 25-byte bodies that are the out-of-line
  ~_List_base<CMaintainedEvent> and ~list<CMaintainedEvent>; nothing in the exe calls or addresses either, so which is which cannot be proven.
- Leads (one source each, not in NAME_FIXES): 0x009E42C0 / 0x00A66860 / 0x00A66C00 are catalogued CMemoryDataOutputStream::Clear (propagated)
  but clear the MaintainedEventsList of CJoystick (+0xD0C) / CKeyboard or CMouse (+0x340C) and zero the next counter; Ego_r.pdb has
  ?ClearAllMaintainedEvents@CJoystick / CKeyboard / CMouse@@QAEXXZ. The three rows also define the same leaf ?Clear@CMemoryDataOutputStream@@QAEXXZ.
- Coordinator decision asked: the self rename is reduce-only (the claim was already bound at 0x009E4710 by 0x00A66990 / 0x00A66F80; the
  old self stand-in spelling disappears, the two blocked rows keep their stand-ins; the spelling set at 0x009E4710 goes from 7 to 3).

## Descriptive names
No new made-up names. STLport spellings (0x009E4710 _List_base<CMaintainedEvent>::clear; the std::list<CMaintainedEvent> members) are
descriptive-by-library. The template arguments of the 23 component / traits rows are PDB spellings (two sources each, RENAMES evidence).

## Flags lines
None added: every row grades with its build entry's flags; 0x005D8167 / 0x0057F93E keep their size-TU `#pragma optimize` lines.

## Rule slips
- One sed edit of work/q5199/genA.py did not apply (escaping); caught by the next precheck, fixed with a python edit (one extra failed compile
  on 0x00F2D640, counted in grades_used).
- Two generated comments were wrong before hand-back and were corrected (0x00CDCBE0 "CQ_SunnyvaleMasterData::OnPersist 0x00CEF8E0";
  0x007AD7B0 / 0x004AA3F0 / 0x004CBD00 stand-in descriptions).
- The section-1 old leaves of the stand-in rows were derived from the HEAD sources by script, not pasted from HEAD compiles.
