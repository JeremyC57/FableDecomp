# NF184 notes (2026-11-20, round 10)

## q4190 (row 0x0083EEE0 LHHttp2::GetUntilCRLF, NF180 held draft)
- The brief prescribes declaration-only _M_fill_insert. That does not work. Hiding the header body through a trial opt-in gives
  DIFFER(255v255). The resize(1) char() temporary moves from its own frame byte (retail 0x0083EF14 `mov [esp+13h],bl`) into the
  dead `offset` argument slot. NF180's form 1 found the same.
- An `extern template` explicit-instantiation declaration of the member suppresses the copy, but it also gives DIFFER: VC7.1 must
  compile the body.
- Fix with no header change: two existing fable_containers.h opt-ins, FABLE_RETAIL_STLP_FILL_CHAR (STLport's char fill / fill_n memset
  overloads) and FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22 (STLport's guarded __copy_trivial_backward).
  - The emitted _M_fill_insert<char> COMDAT is then 289 bytes and grades RELOCATION_MATCH at its owner 0x004110D0. Without them it
    is 266 bytes. With FILL_CHAR alone it is 289v289 DIFFER (register swap, and the backward memmove 4 bytes early).
  - The row stays RELOCATION_MATCH. vland: RELOCATION_MATCH + PASS (work/b1.log).
- Pick-any check (q.py of the row source at each owner address, /Gr). Every newly defined symbol that reloc_bindings maps elsewhere
  grades RELOCATION_MATCH there: _M_fill_insert 0x004110D0, __copy_trivial 0x00405BA0, __copy_trivial_backward 0x00405BC0,
  resize(n) 0x00412880, resize(n, x) 0x004122D0.
- Debt (not touched): landed 0x0055F710 emits a 293-byte _M_fill_insert<char> copy. The callee of the copy, vector<char>::_M_insert_overflow
  0x0040F6B0, is bound by its owner 0x004110D0 as the stand-in `?_M_insert_overflow@?$vector@D@std@@QAEXPADABDABU__true_type@@I_N@Z`
  (reloc_bindings); the shared header's COMDAT copies (0x0083EEE0 here, already 0x0055F710 / 0x00563480) bind the STLport spelling
  `?_M_insert_overflow@?$vector@DV?$allocator@D@std@@@std@@IAEXPADABDABU__true_type@2@I_N@Z`. Neither is in a PDB name set; no retail
  body other than 0x004110D0 calls 0x0040F6B0, so the gate sees no new split. Queue item: port 0x004110D0 / 0x0040F6B0 onto the header.
- Lesson for the queue: a body that is visible but kept out of line cannot be declaration-only when the row depends on its
  visibility. Fix the copy's bytes with the header's STLport-faithful opt-ins instead.

## q4391 (row 0x009A80B0 CBankFileManager::BankExists, NF177 held draft)
- Declaration-only std::less<CCharString>::operator() (trial opt-in in fable_containers.h, reverted) removes the copy but gives
  DIFFER(140v145).
  - With less visible, VC7.1 knows the noinline _M_find copies write no memory, so it keeps the bank-list head in ebx across them
    (retail 0x009A80DF).
  - Declaration-only reloads the head from [edi+0x10] every pass.
- Form 2: the explicit specialization under optimize("s") (new opt-in FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY, stlport/fable_map.h).
  The row matches, but the copy is 16 bytes: CCharString::operator< is called, not expanded.
- Form 3 = form 2 + the existing FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS. The row is RELOCATION_MATCH and the copy grades
  RELOCATION_MATCH at 0x0042AAB3 (41 bytes, as the owner's size TU). vland RELOCATION_MATCH + PASS (work/b2.log).
- COORDINATOR DECISION: the opt-in is scaffolding (`#pragma optimize("s")` around an explicit specialization in a shared header,
  marked `Fidelity: scaffolding, not retail`).
  - Retail's BankExists TU surely emitted its own /O2 copy, which the linker discarded.
  - The pick-any rule allows a byte-identical copy, and this is the only form found that keeps the row and the copy both right.
  - If not accepted, the row stays held.
  - No FABLE_FORCEINLINE was added to a header: FORCEINLINE_LESS is an existing opt-in.
- Pick-any check: every newly defined symbol that reloc_bindings maps elsewhere grades MATCH / RELOCATION_MATCH at its owner:
  0x00429950, 0x0042AAB3, 0x0041C780, 0x0041C7A0, 0x0041C7C0, 0x0041C830, 0x009AB4F0, 0x009AB560, 0x009AB5D0.

## q4393 / q12124 (0x009AB5D0 map<CCharString,CCharString>::_M_find)
- Census (HEAD probe of every REFS row, work/probe_q4393.tsv). Already on the D1 spelling: 0x009A7CA0, 0x009A7E20, 0x009A7ED0,
  0x009A7F80, 0x009C4440, 0x009C46B0, 0x009EF360. Wrong: 0x009A80B0 (fixed under q4391) and 0x009A7D40.
- 0x009A7D40 CBankFileManager::GetRetailBankFilePath ported onto fable_string.h, stlport/fable_map.h and fable_retail_banks.h.
  - Two new opt-ins in fable_retail_banks.h: CREATEDRETAILBANKS (the PDB member type) and CREATERETAILBANK (private declaration).
  - One new opt-in in fable_map.h: STLP_TREE_INSERT_UNIQUE_DECL_ONLY.
  - vland RELOCATION_MATCH + PASS (work/b3.log). The landed standalone test is kept: it embeds no row source.
- Callee spellings it now binds:
  - 0x009AB5D0 and 0x009AB560: their D1 self Leafs. This narrows both addresses.
  - 0x009AA4C0: unchanged.
  - 0x009AAEB0 set<CWideString> insert_unique: the STLport header spelling
    `?insert_unique@?$_Rb_tree@VCWideString@@V1@U?$_Identity@VCWideString@@@std@@U?$less@VCWideString@@@3@V?$allocator@VCWideString@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@VCWideString@@U?$_Nonconst_traits@VCWideString@@@std@@@std@@_N@2@ABVCWideString@@@Z`
    replaces the stand-in `?insert_unique@?$set@VCWideString@@@std@@...`.
    - No PDB has this instance (Dinkumware PDBs), so the spelling is descriptive-by-library.
    - The address's spelling count stays 3: self row 0x009AAEB0 4-argument local model, 0x009A7A20's
      `?insert_unique@?$_Rb_tree@VCWideString@@@std@@...`, and this row. The exe scan finds only these two referencers.
    - The split pre-exists. The family to close it is {0x009AAEB0 self, 0x009A7A20}. Both need a CWideString::operator< (0x0099AF70)
      declaration in fable_string.h first.
- 0x009A7D40 pick-any check:
  - f1 (insert_unique body visible) emitted an insert_unique<CWideString> copy whose less compares converted text pointers.
    fable_string.h has no CWideString::operator<. It also emitted _M_decrement 0x004294E2, DIFFER(64v57).
  - f2 (declaration-only) removes both. The row bytes are the same in both forms.
  - The remaining mapped copies, iterator == / != 0x0041C820 / 0x0041C830, grade MATCH.
- The row is an int3-after-noreturn N+1 row: q.py shows DIFFER(223v222) on the trailing int3 only; vland grades it RELOCATION_MATCH.

## q4394 (0x009D2DD0 map<CCharString,unsigned long>::_M_find): open (census only; rule slip: skipped ahead, port not attempted)
- HEAD probe (work/probe_q4394.tsv): 0x009CCDC0, 0x009CE5E0, 0x009F0500, 0x009F0570 and 0x009F05A0 bind the claim.
- 0x009F03B0 binds `?find@CStringSectionMap@CodeSections@@...`. It is also self-misnamed: catalog RemoveSection, but both PDBs have
  ?DeregisterSection@CCodeSectionManager@@QAEXVCCharString@@@Z, and the row models CCharString (which has a shared header) locally.
- The port moves six callee spellings together:
  - 0x009D2DD0 (two calls);
  - 0x0042A34E, the _Rb_global _Rebalance_for_erase header spelling;
  - 0x009F0240, the PDB private GetSectionDesc;
  - 0x009F0E30, map<CCharString,ulong>::erase(iterator);
  - 0x009F0D90, map<ulong,CSectionDesc>::find (iterator returned through a hidden pointer);
  - 0x009F0E80, its erase.
- It also needs a CSectionDesc layout, and has an inline-budget split: ShortNameToHandle.erase is expanded while the other erases are
  called. Not attempted in this slice (context). The Ego_r layout: ShortNameToHandle +4, LongNameToHandle +0x10, SectionDesc +0x1C
  (retail 12-byte maps).

## q5226 / q5234: rejected; q6141: done-already; q6143: done
- The s6 w1 destructor claims conflict with the header's ReleaseData / destructor pair analysis (engine/CCountedPointer.h, FAM01
  2026-11-11, NF104 REJECTED q4777). In each case the 44-byte body (0x0042918C, 0x0065D5A0) is ReleaseData, and a 5-byte jmp thunk
  (0x00429BF9, 0x0065F92A) is the destructor. See REJECTED.tsv and NAME_FIXES.tsv. The fix is a 9-row family per address. It is not
  done here.
- The q6141 copy constructor 0x00428905: all four REFS rows bind the claim (probe).
- The q6143 copy constructor 0x0065D89B:
  - 0x0065F96D binds the claim.
  - 0x0065F37A (an STLport list node factory under the stand-in CRetailList_node0x10::Buynode) now takes
    CCountedPointer<CVillageTaskHorn> from engine/CCountedPointer.h, with the existing COPY_DECL_ONLY.
  - Its own leaf follows the parameter type (leaf-only self rename, RENAMES section 1). No landed row binds the old or the new leaf.
    The one referencer 0x0065FD39 binds its own stand-in; that split pre-exists (count 2 -> 2).
  - Descriptive: the true name is std::list<CCountedPointer<CVillageTaskHorn> >::_M_create_node (STLport, protected). It is not
    applied: emitting a protected header member as the row needs the explicit-instantiation scaffolding. Queue item.

## q6391 / q6462 (0x0070C050 CAnimInit::Init)
- HEAD probe of all 40 REFS rows (work/probe_q6391.tsv): 39 bind the Init spelling. 0x007502D0 (reverted by NF173b) bound Set.
- 0x007502D0 is ported onto engine/CAnimInit.h. Its local CAnimFrameInfo / CAnimInit models are gone; the update mode is
  ANIM_UPDATE_FROM_SPEED (value 1, engine/EAnimUpdateMode.h).
- The ctor COMDAT problem that held NF173b: the NF184 scaffolding opt-in FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY compiles the inline
  constructor under the owner TU's optimize("y", off) + ("s", on). The emitted copy grades RELOCATION_MATCH at 0x005B33F4, and the row
  stays RELOCATION_MATCH (vland PASS, work/b5.log).
- COORDINATOR DECISION (same as q4391): acceptance of `#pragma optimize` scaffolding in a shared-header opt-in.
- The census probe compiled 0x00861A80 read-only. It is locked by NF185 and was not changed.

## q11760 / q11873 (0x006D41F0): deferred (the brief reserves the row for NF181)

## Descriptive names
- 0x0065F37A keeps the stand-in CRetailList_node0x10::Buynode (descriptive; see q6143).
- The set<CWideString> insert_unique STLport spelling bound by 0x009A7D40 is descriptive-by-library.

## Locks
- Claimed beyond rows.txt; nothing changed in any of them (free them unless a resume needs them): 009a7ca0 009a7e20 009a7ed0 009c4440
  009c46b0 009ef360 009a7f80 009aaeb0 009a7a20 009ab560 009ccdc0 009ce5e0 009f0500 009f0570 009f05a0 0041d198 004291b8 00429bf9
  0042dd28 0042f50b 0042fc86 005958f5 0041d1b4 0042e98f 0065f37a 0065f96d 007502d0.
- Changed under those claims: 009a7d40, 0065f37a and 007502d0. The other changed rows (0083eee0, 009a80b0) were in rows.txt.
- In rows.txt and not touched: the q5234 / q11655 / q11656 / q11718 / q11954 / q11956 / q12018 / q12134 / q12135 / q12139 /
  q12140 / q12142 rows (entries open or rejected).

## Funclets
- `grep` of rebuild/src/compiled/01/ for the five changed rows' file names: no EH funclet includes any of them. Nothing to regrade.
  The 0x0083EEE0 self rename moves no funclet.

## Recheck
- recheck_final.log: 5 rows, all RELOCATION_MATCH + PASS (WINS: 5). The 19 SHARED_HEADER_AVAILABLE warnings are all on 0x007502D0.
  Its other local models (CCharString, CAnimationSet, CDefPointeeBase ...) were there at HEAD; this hand-back removed two of them
  (CAnimInit, CAnimFrameInfo) and added none.
