# NF222 notes (2026-11-22, round 10): the CCountedPointer<T>::ReleaseData family, handed back whole

Base: NF221's hand-back (`pending/2026-11-22_nf221_handback/`). Its 38 family rows are re-diffed against today's tree (the landed sources of
all 40 rows are older than NF221's run; landing nf221 touched only the four q12403 rows, which are not in this hand-back). rows/ = those 38
(6 changed again here, below) + 0x00A5F1C0 / 0x00A634F0 (the two open rows) + their callers 0x009E2C10 / 0x009E2BA0 = **42 rows**, plus
NF221's rows/0048e78e_test.cpp.
- Recheck: one `vland --recheck-landed` run (recheck_final.log, FABLE_HDR_OVERLAY=hdr/) over 69 rows: the 42 rows, the 4 q12403 rows landed at
  nf221, and the 23 unchanged landed referencers of family addresses (work/outside.txt). WINS 69, every row RELOCATION_MATCH + PASS.
- One RELOC_CONFLICT warning, also present at HEAD: 0x0041AFA0 binds ??1<CGraphicDataBank> at the thunk 0x004194B5, while the table line
  (32706, witness 0x00416C8A) says 0x00419108. TABLE_CHANGES moves that line to 0x004194B5, witness 0x0041AFA0, which clears it.
- Link check (tools/lnk.py: /NODEFAULTLIB /FORCE, LINK unset), work/link_check.txt:
  - the 69 objects give 1 LNK4006, between two unchanged non-family rows (0x00A09F20 / 0x009F83D0 both define CBasicString<char>::Compare
    strongly; pre-existing). The 42 family objects alone gave 0 in the first run.
  - Control: one object linked twice reports its leaf duplicate, so the check works.
Locks: the 40 rows.txt rows plus 0x009E2C10 / 0x009E2BA0 (claimed), all NF222.

## Entry 1: the AcquireFont rows (Reading A: every referencer of 0x0041947A on the new spelling)
- 0x00A5F1C0 CStreamingFontDataBankEntry::AcquireFont: the local `CCountedPointer_00a5f1c0` stand-in -> engine/CCountedPointer.h
  (`RELEASEDATA_DECL_ONLY` + `RESET_CAMERA_DECL`, the declaration-only Reset). Member `mutable CCountedPointer<CFontBank> Font` (Ego_r.pdb
  CStreamingFontDataBankEntry +0x04 Font; was m_font). Self leaf `?AcquireFont@CStreamingFontDataBankEntry@@QBE?AV?$CCountedPointer@VCFontBank@@@@XZ`.
  One form.
- 0x00A634F0 CFontDataBankEntry::AcquireFont: the same port. The local `struct CStaticFontBank(long, long)` model is replaced by
  engine/CStaticFontBank.h `_CTOR`, `??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z`. That is the self leaf of the landed row 0x00AB8E10,
  whose only landed binder is this row (census: callers.py; 0x00A6336C is not landed). Bank / Index retyped CFontDataBank* / unsigned long
  as engine/CFontDataBankEntry.h lays them out. One form.
- QBE: all three PDBs spell both AcquireFonts `QBE` (const). The brief's condition was "both PDBs + the callers censused and respelled in the
  same hand-back". The census finds one landed caller each (callers.py + absrefs.py: 0x009E2C10 -> 0x00A5F1C0, 0x009E2BA0 -> 0x00A634F0,
  no absolute references). Both callers are respelled in place (`AcquireFont() const` in their HEAD local models), one form each. So each
  AcquireFont address ends on one spelling, the QBE one: at HEAD it had two, the self stand-in return type and the callers' QAE.
- `mutable` is unproven: the PDB layout cannot show it. It is the smallest spelling that lets the const member re-seat Font; no byte depends on it.
- 0x00A634F0's local helper `CreateBank` (a static inline member of the HEAD local model, no PDB name): with the header argument types it
  defined a new external pick-any ?CreateBank@CFontDataBankEntry@@SAPAVCStaticFontBank@@PAVCFontDataBank@@K@Z (form 1). Form 2 moves it to a
  file-scope `static inline` function (internal linkage): RELOCATION_MATCH, no external symbol. The defined-symbol diff v HEAD of all 42 objects
  now shows only leaves and CCountedPointer / CCPPointerInfo header copies (work/sc_all.log). 0x00A5F1C0's CreateBank member is unchanged
  from HEAD (same mangled name) and stays.
- Both rows still carry their HEAD local CStreamingFontDataBankEntry / CFontDataBankEntry models. The generated headers exist but flatten
  Font into Font_Data / Font_Info and declare no methods, so this is HEAD debt and is not made worse (in-place respell, same class token).
- Tests: unchanged. All four test files are self-contained host models and pass as they did at HEAD.

## Entry 2: 0x005BFF07 (ruling 1: ported onto engine/CCountedPointer.h)
- The HEAD local `template <class T> class CCountedPointer` model and its `class CCPPointerInfo;` are removed. The row uses
  engine/CCountedPointer.h with `RELEASEDATA_DECL_ONLY`, the shape of the landed sibling ~CTCInventoryBase row 0x005C00A0.
- The SerialisedItems pointees come first, from the Ego_r.pdb CTCInventoryBase record: SerialisedItems is
  CCountedPointer<vector<NInventory::CItem>> (+0x78), SerialisedSelectedItems is CCountedPointer<vector<CCharString>> (+0x80). They are declared
  with forward-declared std::allocator / std::vector exactly as 0x005C00A0 does. The local GetPItem() accessor (no PDB spelling) is gone: the
  temporary's Data is read.
- One form, RELOCATION_MATCH. The temporary's release `??1<CGraphicDataBank>` becomes ReleaseData<CGraphicDataBank> at 0x00419108.
- Pick-any copies naming CCountedPointer, all from the shared header:
  - ??1<vector<CItem>> = owner 0x005C07DF and ??1<vector<CCharString>> = owner 0x005C07FB, both 5-byte `jmp` (bytes + relocations EQ);
  - ??1<CGraphicDataBank> = owner 0x004194B5 (EQ);
  - the two default ctors ??0<vector<...>>: 10 bytes, NO-OWNER (always expanded in retail).
  NF221's ??1<void> copy, built from a local-model body, is gone.
- The implicit ~CTCInventoryBase / ??_G / ??_7 come from the HEAD local CTCInventoryBase model. They are defined at HEAD and now; the
  ~CTCInventoryBase copy is NEQ v 0x005C00A0 at HEAD and now, 30 v 129 bytes. HEAD debt: the class has engine/NInventory_CTCInventoryBase.h,
  but its default path keeps the serialised pointers opaque. Moving the row onto that header is a separate port.
- Still a stand-in, outside the family: `?CreateAndInitialize@CGraphicBankManager@@...` at 0x009F83D0 (NAME_FIXES). The test is unchanged
  (a self-contained host model).

## Entry 3: whole-family recheck
- Grades: tools/pkrun.sh compiled every row at HEAD and in rows/ (overlay on). All 42 rows are RELOCATION_MATCH at both (work/sc_all.log).
- Census: every landed referencer of the 39 family addresses. That is callers.py + absrefs.py over Fable.exe; the q12403 CComponent targets
  are excluded, since landing nf221 censused them. Result: the 42 rows, plus 23 unchanged rows compiled as landed (work/hb_out.tsv, all
  RELOCATION_MATCH). Census file: work/census_final.txt.
  - Every family address ends on ONE spelling, 0x0041947A included (self + 0x00A5F1C0 + 0x00A634F0 on Reset<CFontBank>).
  - The exceptions are splits that this family did not create and does not grow:
    - 0x00419036 / 0x00428AE7: self leaf @DeleteIfSet@4 v the binders' DeleteData<T>. 0x00419036 had 3 spellings at HEAD and has 2; 0x00428AE7
      had 2 and has 2 (NAME_FIXES).
    - 0x0041E641 (~CManager: self, 0x0042FB40 ??1CManager@NUISystem@@IAE@XZ, 0x0042DB20 ?Dtor@CManager@@QAEXXZ) and 0x005BFF07 (the 10
      CTCInventory* ctors bind 4 spellings of ??0CTCInventoryBase): binder rows unchanged.
- Callees outside the family:
  - 0x009E2C80 GetFont: 8 spellings at HEAD, 5 now;
  - 0x009F83D0 OpenGraphicBank: 4 at HEAD, 3 now;
  - 0x00AB8E10 ??0CStaticFontBank: 2 at HEAD, 1 now (work/gf_out.tsv).
- Pick-any copies v owners: tools/pk.py. It reads every DEF function COMDAT of each object except the leaf, at HEAD and now, against
  work/owners.tsv. That map holds the post-family spellings: rows' Leaf lines, landed Leaf lines, the ReleaseData / thunk / DeleteData /
  ??4<CMeshDataBank> 0x00429BFE / vector ??1 owners. It is not today's reloc_bindings, whose ??1<T> lines still name the ReleaseData bodies.
  A copy is EQ when its bytes (relocation fields zeroed) and relocation targets equal the owner's. Result: work/pk_summary.tsv.
  - Measured on the NF221 forms, 6 new copies were NEQ, so 6 rows were changed again here:
    - the five Reset rows: their DeleteData<T> copy was a plain `operator delete`, NEQ v the virtual-delete owners. Fix:
      `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` (overlay), which declares it as at HEAD. No copy now; each row still RELOCATION_MATCH.
    - 0x0042DDB3: its `??$?4VCFontBank` copy had `push ebp` from the frame-pointer TU, NEQ v 0x0042FE34. Fix:
      `FABLE_RETAIL_CCOUNTEDPOINTER_CONVERTING_ASSIGN_DECL_ONLY` (overlay). No copy now.
    - Making T complete would also fix the Reset copies: CFontBank.h `FABLE_RETAIL_CFONTBANK` gives the virtual delete. It is not possible
      for CLUA, which has no header, so the one declaration-only opt-in was used for all five.
  - Final, NEW objects: 38 EQ (8 of them new symbols), 0 new NEQ, 14 NEQ that were NEQ at HEAD too (debt, nf203), 142 NO-OWNER
    (14 of them new symbols).
  - NEQ at HEAD and now, CCountedPointer ones:
    - 0x00416C8A ShareData<CGraphicDataBank> v 0x00419134 and ??4<CGraphicDataBank> v 0x004194BA (both 40 / 26 bytes, same length; could be
      closed with SHAREDATA_ / ASSIGN_DECL_ONLY, not tried);
    - 0x0056F177 copy ctor <CGraphicDataBank> v 0x0041BA30;
    - 0x005958F5 copy ctor <CMeshDataBank> v 0x00428905.
  - NEQ at HEAD and now, others: ??_GCGameComponent (0x00418B7A, 0x0042F50B), the 0x0048E78E CFadeInFadeOutBase / sprite / IsTextReadyForClickPast
    copies, 0x0065431D vector<CEnginePrimitiveHandle> dtor, 0x005BFF07 ~CTCInventoryBase.
  - Improved: 15 ??1<T> copies in 0x00418B7A / 0x0042DD28 / 0x0042F50B / 0x0056F177 / 0x005958F5 / 0x00416C8A were 44-byte release bodies at
    HEAD (NEQ v the 5-byte thunks). They are now the 5-byte jmp, EQ v 0x004194B5 / 0x00429BF9 / 0x00419423 / 0x0042FBCB / 0x0042FC0E / 0x0042FCFF.
  - New NO-OWNER symbols (coordinator decision; none is referenced by code, since every row is RELOCATION_MATCH):
    - ??1<CFontBank>: 5-byte jmp ReleaseData. New in 0x004168DC, 0x0042DDB3, 0x0064560D, 0x0065431D, 0x00A5F1C0, 0x00A634F0. Retail has no
      out-of-line ??1<CFontBank>: no E9 into 0x004057A0 anywhere in Fable.exe, and the same holds for ??1<NGameText::CDataBank> / 0x00403750.
      At HEAD 0x00418B7A / 0x0048E78E already defined it (44 bytes), and default-path includers such as 0x009E2C10 / 0x009E2BA0 define it
      with 53 bytes, so the symbol already had differing pick-any bodies.
    - ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z, the header's FableWin spelling: new in 4 Reset rows, replacing the local `JP6I` spelling.
    - copy ctor ??0<CFontBank>, 25 bytes: in the 2 AcquireFont rows, identical to the 0x009E2C10 / 0x009E2BA0 copies at HEAD.
    - the two ??0<vector> default ctors in 0x005BFF07.

## Entry 4: table lines (TABLE_CHANGES.tsv: 42 delete, 21 add)
- Deletes are the reloc_bindings lines at family targets whose spelling no landed row binds after this lands. The full census of the
  referencers backs them, not only the witness rows. They include line 14686 `??1<CFontBank> -> 004057a0`, the ??1<T>-at-ReleaseData lines,
  every stand-in line, the AcquireFont QAE lines, `??0CStaticFontBank@@QAE@JJ@Z`, and the four GetFont / CreateAndInitialize stand-in lines
  whose only binder was a family row.
- Adds are the new spellings with a witness. Two adds are optional: the table simply had no line for these unchanged bindings
  (`??1<CGraphicDataBank> 004194b5` witness 0x0041AFA0; `?Dtor@CManager@@QAEXXZ 0041e641` witness 0x0042DB20).
- Lines for anonymous-namespace data (`?BANK_HANDLE@?A0x...`, `?DEFAULT_FRONT_END_FONT@?A0x...`) carry a per-compile hash and are not listed.
- q12403's `??1<CComponent>` move already landed (nf221).

## RENAMES
- Section 1: 25 self renames (NF221's 23 + the two AcquireFonts).
- Section 2: 66 caller respellings (NF221's lines minus the four landed q12403 rows, + 5 here: the two Reset stand-ins, ??0CStaticFontBank, and
  the two AcquireFont callers).
- 0x005BFF07's line (??1 -> ReleaseData<CGraphicDataBank> at 0x00419108) is unchanged by the full port.

## Other
- Flags lines: none added. Descriptive names: none new (the made-up `CreateBank` helper predates this pass). EH funclets / includers: a grep of rebuild/src
  and rebuild/tests for the file names of the 11 rows changed by NF222 finds no includer (NF221 checked its own).
- Not touched: AUTH9 rows, Codex pass 27 reservations (none in the family).
- NAME_FIXES: the two AcquireFont QBE fixes and the 0x00429C53 move are applied/landed and dropped. The three DeleteData self rows remain, as
  do the GetFont / OpenGraphicBank binders still on stand-ins (lists refreshed).
