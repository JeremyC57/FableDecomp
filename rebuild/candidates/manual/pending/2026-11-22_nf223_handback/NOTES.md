# NF223 notes (2026-11-22, round 10): the CCountedPointer tail and two split follow-ups

Base: the repo tree after landing nf222. Locks: claimed with plain mkdir (or `released` reclaimed), owner NF223 (list in rows.txt).

## Entry 1 (q12551-q12553): the three DeleteData self rows -- done
- Census (callers.py + absrefs.py over Fable.exe): no E8/E9 into any of the three; absolute stores only:
  - 0x00419036: Reset<CGraphicDataBank> 0x004194D1 (site 0x004194FA), OpenGraphicBank 0x009F83D0 (0x009F8491), unlanded 0x00429BA1;
  - 0x00428AE7: Reset<CMeshDataBank> 0x0042FC86 (0x0042FCAF), OpenMeshBank 0x00A09F20 (0x00A09FA9), unlanded 0x00429BDF;
  - 0x0042F864: Reset<const CFontBank> 0x0042FC13 (0x0042FC3C), unlanded 0x0042FBF4.
- All five landed binders already bind the FableWin spelling (graded at HEAD, RELOCATION_MATCH; work/dd_hb.tsv). So only the self leaves
  move; no binder respelling. Each address goes from 2 spellings (self `@DeleteIfSet@4` + binders' DeleteData<T>) to 1.
- Names: FableWin.pdb, Ego_r.pdb and Ego_d.pdb all carry DeleteData<CGraphicDataBank> / <CMeshDataBank>; the const-T
  `?DeleteData@?$CCountedPointer@$$CBVCFontBank@@@@SIXPAX@Z` is in FableWin.pdb and Ego_d.pdb (Ego_r.pdb has only the non-const one). Second
  source: the binders store exactly these spellings, and the 11-byte body is the virtual delete.
- Rows: the landed DeleteData recipe (0x0056C0F0 / 0x00BB6330): include the pointee header with its polymorphic opt-in, then
  engine/CCountedPointer.h, then the explicit instantiation. Each object defines only its leaf (objdump -t).
  - 0x00419036: `FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS` + `FABLE_RETAIL_CRESOURCELIST` + engine/CGraphicDataBank.h. Form 1 (without
    CRESOURCELIST) failed to compile (CResourceBank.h GetCurrentFrame); form 2 MATCH.
  - 0x00428AE7: the same two + `FABLE_RETAIL_CMESHDATABANK_BASES` + engine/CMeshDataBank.h. Form 1 (BASES alone: non-polymorphic
    CBankFile) DIFFER 8 v 11; form 2 MATCH.
  - 0x0042F864: `FABLE_RETAIL_CFONTBANK` + engine/CFontBank.h, `CCountedPointer<const CFontBank>`. Form 1 MATCH.
- Recheck: vland --recheck-landed, 3 MATCH + PASS (work/e1_recheck.log). Tests unchanged (print-only).
- Pick-any: the self objects define no copies. Binder objects 0x009F83D0 / 0x00A09F20 define DeleteData copies: both EQ v the owners
  0x00419036 / 0x00428AE7 (pk.py; unchanged rows).
- Files: the coordinator moves `global_DeleteData_00419036.cpp` / `global_DeleteData_00428ae7.cpp` / `gapscan_sub0042f864_0042f864.cpp`
  to the CCountedPointer_DeleteData_<addr> names and the catalog names to CCountedPointer<T>::DeleteData (RENAMES section 1).
- Table: no change. reloc_bindings already has the three DeleteData lines (witnesses 0x009F83D0, 0x0042FC86, 0x0042FC13); there is no
  `@DeleteIfSet@4` line.

## Entry 2 (q12554 GetFont 0x009E2C80, q12555 OpenGraphicBank 0x009F83D0)
Census (callers.py + absrefs.py; hb.sh grades, work/e2_hb.tsv): 0x009E2C80 has 16 landed callers, 4 on stand-ins (0x00405020, 0x00438AA0,
0x00494840, 0x0054ED90); 0x009F83D0 has 4, 3 on stand-ins (0x00416C8A, 0x0041E3F6, 0x005BFF07). No absolute references.
- **q12555 done: 0x009F83D0 3 spellings -> 1.**
  - 0x00416C8A: the `CGraphicDataBankManager` stand-in plus cast becomes a local `class CGraphicBankManager { OpenGraphicBank }`.
    New-local-model rule, all three conditions: no header for CGraphicBankManager anywhere under rebuild/include (CSystemManager.h only
    forward-declares it); the landed sibling 0x0042DDB3 has the same shape; no base. One form.
  - 0x0041E3F6 / 0x005BFF07: the local `CGraphicBankParams` and `class CPixelFormat { long Format; }` stand-ins are ported onto
    engine/CGraphicDataBankInit.h (and CPixelFormat.h). Members are mapped by offset to the Ego_r names (+0 NonAlphaFormat ... +0x28
    AllowDither). So 0x00415B80 moves too: ??0CGraphicBankParams -> ??0CGraphicDataBankInit@@QAE@XZ, 2 spellings -> 1 (its 4 landed callers).
    - Form 1 (default header) was RELOCATION_MATCH, but the TU emitted a NEW ??0CGraphicDataBankInit copy, 48 B, NEQ v 0x00415B80 (constants
      stored directly; retail uses dl / cl).
    - Form 2: the overlay opt-in `FABLE_RETAIL_CGRAPHICDATABANKINIT_CTOR_DECL_ONLY` (hdr/README.md). No copy; RELOCATION_MATCH.
    - Remaining NEQ copies are identical at HEAD (??1<CGraphicDataBank> / ??1<CComponent> 44 B in 0x0041E3F6; ~CTCInventoryBase in 0x005BFF07).
    - 0x005BFF07's ??1 copies stay EQ.
- **q12554 blocked: 2 of 4 moved, 0x009E2C80 5 spellings -> 3.**
  - Moved: 0x00438AA0 (its local CCountedPointer / CCPPointerInfo model also goes, onto engine/CCountedPointer.h through CFontManager.h;
    defined symbols unchanged) and 0x00405020 (HEAD-local CSystemManager +0x84 pointer retyped CFontManager* in place; its local GetFontBank
    accessor COMDAT changes return-type spelling; nothing binds it). One form each.
  - Held, coordinator decision (drafts work/e2/00494840.cpp, work/e2/0054ed90.cpp, both RELOCATION_MATCH):
    - 0x00494840: Ego_r.pdb types MainFont as CCountedPointer<const CFontBank>, so the real GetFont result goes through the converting
      operator=. The TU emits a NEW copy `??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@QAEAAV0@ABV?$CCountedPointer@VCFontBank@@@@@Z`
      (72 B, ShareData expanded). It is NEQ v its owner 0x0042FE34 (18 B: push both words, call ShareData 0x0042F9F2, ret 4).
      CONVERTING_ASSIGN_ / SHAREDATA_DECL_ONLY would put calls where retail InitGraphics expands them.
    - 0x0054ED90: the row included engine/CCountedPointer.h INSIDE namespace NUISystem, so all its counted-pointer COMDATs were
      NUISystem::CCountedPointer<NUISystem::CFontBank> and had no owner. The draft includes it at global scope. The same bodies then get
      owners and read NEQ: ReleaseData<CFontBank> 53 B v 0x004057A0 (44 B), ShareData<CFontBank> v 0x004190E2, ??4<CFontBank> v 0x00419463.
      Retail's TU expanded these inline, as the draft does.
    - NF222's pk data (pk_all.tsv) has no HEAD copy of these CFontBank symbols, NEQ or otherwise, so there is no precedent to point to.
    - Question for the coordinator: may a copy that was NO-OWNER at HEAD only through a stand-in spelling count as the HEAD NEQ debt
      (nf203)? If yes, both drafts are ready to hand back as they are.

## Entry 3 (q12556): 0x0041E641 ~CManager -- done, 2 spellings -> 1
- Referencers: 0x0042FB40 Clear (??1CManager@NUISystem@@IAE@XZ) and 0x0042DB20 (?Dtor@CManager@@QAEXXZ). 0x0042DB20 itself has no referencer.
- 0x0042DB20 is the compiler's ??_GCManager@NUISystem@@IAEPAXI@Z (Ego_r.pdb + FableWin.pdb, protected like the dtor). The landed Clear TU
  0x0042FB40 emits the same COMDAT, EQ v 0x0042DB20 (pk.py).
- The row is rewritten so that its leaf is that ??_G:
  - the HEAD-local struct moves into namespace NUISystem in place (same class token), with a protected ~CManager as the header has it;
  - a TU-internal friend scaffold calls `manager->~CManager()`; VC7.1 emits ??_G for an unqualified explicit destructor call;
  - the defined symbols are ??_G and the anonymous-namespace scaffold (hashed, the 0x00BB1740 precedent).
- Forms:
  1. the made-up member moved into NUISystem: RELOCATION_MATCH, but a fabricated external member of the real class;
  2. a qualified call from a derived scaffold: no ??_G emitted;
  3. as landed here.
- The made-up self leaf `?ScalarDeletingDestructor@CManager@@QAEPAXI@Z` is gone.

## Entry 4 (q12557): 0x00416C8A pick-any debt -- done
- The existing opt-in `FABLE_RETAIL_CCOUNTEDPOINTER_ASSIGN_DECL_ONLY` alone does it. Retail calls the out-of-line operator= 0x004194BA, and
  the TU kept that call; only the inline body went. The row stays RELOCATION_MATCH with unchanged relocations.
- Both HEAD NEQ copies are no longer emitted: ShareData<CGraphicDataBank> (40 B v 0x00419134) and ??4<CGraphicDataBank> (26 B v 0x004194BA).
  The remaining CCountedPointer copy, ??1<CGraphicDataBank>, is EQ v 0x004194B5. SHAREDATA_DECL_ONLY is not needed, since ShareData was only
  reached through the inline ??4. Same row as entry 2 (rows/00416c8a.cpp carries both changes).

## Entry 5 (q12558): `mutable` on the AcquireFont Font member -- keep, report only (no byte change)
- The PDB member record cannot settle it. In the Ego_d.pdb TPI dump (`$S/dl_debug/types.txt`, made 2026-11-17 when Ego_d.pdb was the only PDB on the lane), the field list 0x706AF of CStreamingFontDataBankEntry has
  `LF_MEMBER Font, Type 0xA08F, offset 4, attrs private`. 0xA08F is a plain LF_CLASS CCountedPointer<CFontBank>, with no LF_MODIFIER.
  CodeView member attributes (access, mprop, pseudo, noinherit, noconstruct, compgenx, sealed) have no mutable bit.
- The body is the other source. AcquireFont is const (LF_MFUNCTION 0x706A7, const this type 0x706A4; the QBE leaf), and it re-seats Font
  (Reset on +4). That takes `mutable` or a const_cast, and both give the same bytes.
- CFontDataBankEntry (field list 0x70523) has the same record: Font, Type 0xA08F, offset 4, private.
- `mutable` stays as the smallest spelling. Nothing changed in either row.
- Side note, not applied: the PDB makes Font private; the HEAD-local models keep it public. That is in-place HEAD debt, and access changes
  no bytes for a data member.

## Entry 6 (q12548): 0x00765B70 vector<vector<C3DVector> >::_M_insert_overflow -- done
- Referencers: 0x00763060 (x3) and 0x007634E0, both on the STLport spelling; 0x00765C80, on the July placeholder `?InsertGrow@CRetailVector_CElement12_006f0810@@...`.
- The landed self row 0x00765B70 already defines the STLport spelling (its Leaf line is a prefix of it), so the self row is unchanged.
- 0x00765C80 (push_back) is ported onto stlport/fable_containers.h as an explicit instantiation (the 0x00568850 recipe). It uses the existing
  opt-ins `FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY` (copy ctor 0x006F0810 out of line) and `FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY`.
  One form, RELOCATION_MATCH.
- Three addresses move with it:
  - its self leaf moves from the placeholder onto STLport push_back, the spelling 0x00763060 binds it as: 2 spellings -> 1;
  - 0x00765B70: 2 -> 1;
  - 0x006F0810 drops `??0CElement12_006f0810@@QAE@ABU0@@Z`, which only this row used (grep). The address keeps its other, pre-existing
    spellings (a large split, not grown).
- Pick-any: the new ??0__false_type (3 B) and _Construct<vector<C3DVector>> (19 B) copies are NO-OWNER inline header helpers; the HEAD object
  had ??0SScalarTag instead.
- The test is an abstract host model (no class that has a header), unchanged.
- Table: delete line 2849 and add the `_M_insert_overflow` line (witness 0x00763060).

## Entry 7 (q12550): 0x00B2FD60 label -- done (LABELS.tsv)
- The self row is not landed. The only referencer, 0x00B30B50 SetCameraState, already binds ?SetupGamut@CEngineCamera@@AAEXXZ (RELOCATION_MATCH,
  table line 39430), and both PDBs carry that spelling.
- labels_forge_autoname4.tsv line 1598 still says CPlane_InitializeFromCorners, and so does the derived ghidra_out/engine_api.tsv line 37056.
  labels_landing_2026-11-07t.tsv line 2 already has CEngineCamera::SetupGamut. Hand-back: LABELS.tsv.
- 0x00765C80's push_back spelling is in all three PDBs (grep -a). The _M_insert_overflow spelling is in none of them; it is the landed
  self leaf of 0x00765B70 and is not renamed here.

## Recheck, link, self-check
- recheck_final.log: one vland --recheck-landed run (FABLE_HDR_OVERLAY=hdr/), 20 rows: the 10 changed rows plus the 10 unchanged landed
  referencers of the touched addresses (0x004194D1 0x009F83D0 0x0042FC86 0x00A09F20 0x0042FC13 0x0042DDB3 0x0042FB40 0x00763060 0x007634E0
  0x00B30B50). WINS 20: 3 MATCH + 17 RELOCATION_MATCH, all PASS, no RELOC_CONFLICT.
  - The 3 rows remain as WARN SHARED_HEADER_AVAILABLE for HEAD-local models, e.g. 0x00416C8A's local CGraphicDataBankInit. That is HEAD
    debt, not grown.
- Link (tools/lnk.py, /NODEFAULTLIB /FORCE, the same 20 objects): 6 LNK4006, all between unchanged rows and all pre-existing:
  - 5 reported in 0x007634E0 for NShape helpers it shares with 0x00763060, the only other NShape object (lnk.py cuts each line at 260 chars, which hides the second object);
  - 1 between 0x00A09F20 and 0x009F83D0 (CBasicString::Compare, as at nf222).
  None names a changed row. File: work/link_check.txt.
- Defined-symbol diff v HEAD, from the per-row sc.sh runs in the notes above. Only these change:
  - the leaves;
  - shared-header copies: the CCountedPointer / vector helpers, ??_G (EQ v its owner), and ??0CPixelFormat (gone with CTOR_DECL_ONLY);
  - the HEAD-local GetFontBank accessor's return-type spelling in 0x00405020.
- Census: work/census_final.txt. Every touched address drops or keeps its spelling count; none grows.
- Pick-any: every new copy with an owner is EQ, except in the two held rows (0x00494840, 0x0054ED90).
- Flags lines: none added. Descriptive names: the scaffold EmitScalarDeletingDestructor (anonymous namespace, internal) in 0x0042DB20.
- EH funclets / includers: none of the changed files is included by another source (grep of the file names over rebuild/src and rebuild/tests).
- Not touched: AUTH10 locks, Codex pass 27 reservations (none in this slice).
