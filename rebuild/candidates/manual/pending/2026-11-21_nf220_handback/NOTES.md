# NF220 notes (2026-11-21, round 10)

Lane: q.py grades in `S=$D/pS` (HEAD objects in `$D/pH`); vland payloads from tools/pay.py (NF219's, repointed). Probe sources in work/.

## q12226-q12229: CCountedPointer<T>::ReleaseData (CLUA / CGraphicDataBank / CIEngine / CASoundBank): BLOCKED (family larger than the slice)
Claim holds for all four (FableWin.pdb ReleaseData names, the 44-byte /Os release bodies, 5-byte jmp thunks 0x00419423 / 0x004194B5 /
0x0042FCFF / 0x0042FBCB = the destructors, `~CCountedPointer() { ReleaseData(); }`). No row changed; every probe is in work/.

### The three gates (advisor), all measured
- Gate A, 0x00403750 (NGameText::CDataBank, 53-byte /O2-shaped body, no jmp thunk, callers 0x004175E5 / 0x00418B7A / 0x0042F50B only,
  absrefs none): `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` + `template void CCountedPointer<NGameText::CDataBank>::ReleaseData();`
  with NO size pragma grades RELOCATION_MATCH (work/00403750_g1.cpp). So 0x00403750 = ReleaseData<NGameText::CDataBank> as compiled in a
  speed TU (the /O2 out-of-line dtor would be the same bytes; the /Os callers, which call the other five T's ReleaseData bodies directly
  in the same member sequence, settle the binding as ReleaseData). Self rename outside the queue lines, needed by the family.
- Gate B, the multi-type binders: 0x0042F50B with `#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY` before its
  CCountedPointer.h include: RELOCATION_MATCH, all six sites bind ?ReleaseData@?$CCountedPointer@<T>@@@@QAEXXZ (CDataBank, CIEngine,
  CASoundBank, CMeshDataBank, CGraphicDataBank, $$CBVCFontBank); the object also emits pick-any ??1<T> = 5-byte `jmp ReleaseData<T>`
  copies (the thunk shape). Same one-line change: 0x00416C8A, 0x004175E5 (drop its DTOR_DECL_ONLY), 0x00418B7A, 0x0042DD28, 0x0056F177,
  0x005958F5, 0x0042FD04, 0x0042FD6F all grade RELOCATION_MATCH (work/<addr>_g1.cpp; leaf-level relocs listed by objdump).
- Gate C, the closure (self leaves that must change and pull their callers in): see the census. It does not close at the four
  entry addresses: 0x00418B7A's two CFontBank sites go to 0x004057A0 (`??1?$CCountedPointer@VCFontBank@@@@QAE@XZ` self leaf, /O2-shaped
  like 0x00403750), which reloc_bindings binds under 7 spellings (CCPtr::ReleaseCur, CPolymorphicData_0041947A Release, ??1CFontPointer,
  ??1<CFontBank>, ??1<CFont>, ??1?$CCountedPointer_0042fe34<CFont>, CCountedPointer4057a0::Release) from 10 landed caller rows, with a
  CFont-v-CFontBank pointee question of its own.

### Census (callers.py + absrefs.py over Fable.exe; work/c2_callers.tsv, work/c3_callers.tsv, NF219 work/c4_callers.tsv)
ReleaseData self rows (leaf ??1 -> ReleaseData, the 0x0065D574 recipe): 0x004190B6 CLUA, 0x00419108 CGraphicDataBank, 0x0042F974 CIEngine,
  0x0042F9A0 CASoundBank, 0x0042918C CMeshDataBank (q12183), 0x0042F948 (`CUnprovenPointee_0042f948` -> `const CFontBank`: callers
  0x0042DD28 / 0x0042F50B bind ??1?$CCountedPointer@$$CBVCFontBank), 0x00403750 NGameText::CDataBank (Gate A, no size pragma),
  and next 0x004057A0 CFontBank (not yet censused beyond its 10 caller rows).
Thunk rows (self -> ??1<T> jmp COMDAT): 0x004194B5 (landed as `_Forward`; its one landed caller 0x0041AFA0 already binds
  ??1?$CCountedPointer@VCGraphicDataBank@@@@QAE@XZ there: not-a-binder), 0x00429BF9 (landed as `_GFGetPreMainMemoryUsed`; no landed
  caller). 0x00419423 / 0x0042FCFF / 0x0042FBCB / 0x0042FC0E are not landed.
Callers on the shared header (one-line DECL_ONLY change, measured): 0x00416C8A, 0x004175E5*, 0x00418B7A*, 0x0042DD28, 0x0042F50B,
  0x0056F177, 0x005958F5; already right: 0x0042FD04, 0x0042FD6F (not-a-binder). (* also binds local stand-ins: 0x004175E5 keeps
  `?Release@CCountedPointer4190b6` / `CCountedPointer419108` / `CCountedPointer4057a0` local models for three sites, which must be ported.)
Callers on local models (port onto the header; self leaf changes, pulling their callers in):
  - ShareData stand-ins `CCPtr` (recipe proven: work/00419134_g1.cpp, RELEASEDATA_DECL_ONLY + /Os + explicit ShareData instantiation,
    RELOCATION_MATCH binding ReleaseData<CGraphicDataBank>): 0x00419134 (<CGraphicDataBank>; callers 0x004194BA, 0x0042A9B7),
    0x004291B8 (<CMeshDataBank>; caller 0x0041D198 binds it as ShareData<CGraphicDataBank> PAUCCPPointerInfo: wrong T and class key),
    0x0042F9F2 (<const CFontBank>; caller 0x0042FE34).
  - operator= stand-ins `CCPtr::AssignFrom`: 0x004194BA (<CGraphicDataBank>; callers 0x00416C8A on ??4<CGraphicDataBank>, 0x0042DDB3 on
    ??4?$CCountedPointer_0042fe34<CGraphicDataBank>), 0x0042FE34 (<const CFontBank>; caller 0x0042DDB3 on ??4?$CCountedPointer_0042fe34<CFont>).
  - Reset stand-ins (T = `CPolymorphicData_<addr>`): 0x00419428 (<CLUA>), 0x004194D1 (<CGraphicDataBank>), 0x0042FC13 (<const CFontBank>),
    no landed callers; 0x0042FC86 Reset<CMeshDataBank> (right leaf, local model).
  - 0x0041E641 ~CManager (defines RELEASEDATA but binds ??1SMember10 / ??1SMember18 wrappers; also binds 0x0041D198 / 0x0042A9B7),
    0x0042A9B7 SetGraphicsBank (local `CCountedPointer_CGraphicDataBank`; caller 0x0041E3F6 already binds the real
    ?SetGraphicsBank@CManager@NUISystem@@QAEXV?$CCountedPointer@VCGraphicDataBank@@@@@Z), 0x0041D198 SetMeshBank (labelled
    SetGraphicsBank; callers 0x0041E641, 0x004A6AB0, 0x004A6E30, 0x005958F5 (real SetMeshBank spelling)), 0x0042DDB3 InitialiseGraphics
    (`CCountedPointer_0042fe34` stand-ins), 0x005BFF07 (local CCPPointerInfo).
Totals: 7 self rows (+ 0x004057A0 next), 2 landed thunk rows, 21 landed caller rows of the ReleaseData addresses, 2 operator= rows pulled
in by ShareData, plus 0x004057A0's 10 caller rows (overlap: 0x004175E5, 0x00418B7A, 0x0042DDB3, 0x004190E2, 0x0041947A, 0x004168DC,
0x0064560D ...): about 40 rows, a third of them local-model ports. One component: 0x0042F50B / 0x0042DD28 / 0x00418B7A / 0x004175E5
couple every address, so it hands back whole.
### Proposed plan (coordinator)
One dedicated family agent, rows locked up front from this census: (1) self + thunk recipes (trivial, scriptable); (2) the eight header
callers (one define each, measured above); (3) the CCPtr / Reset / AssignFrom stand-in ports (ShareData recipe proven); (4) 0x004057A0
pointee identity (CFont v CFontBank) before its 10 callers; (5) ~CManager / SetGraphicsBank / SetMeshBank / InitialiseGraphics /
CTCInventoryBase ports. Table actions at landing: the ??1<T> lines at the ReleaseData addresses go stale (??1 lives at the thunks).
Locks: the 26 family rows I claimed are released again (unchanged); rows.txt rows untouched.

## q12401: 0x0099C120 operator+(const CWideString&, wchar_t), /Gr: ported onto fable_string.h (done)
- Form 1 (header only: FABLE_RETAIL_CWIDESTRING + _SPRINTF_ORIGINAL as 0x0099B4D0 / 0x0099BA70, copy ctor / dtor inline through two
  new opt-ins, operator+=(wchar_t) declared): DIFFER 184 v 177 (work/0099c120_f1.cpp): the local's data pointer is reloaded after the
  operator+= and UnassignString calls; retail keeps it in esi. Diagnostic probe (work/0099c120_probe.cpp, NOT a form: access opened
  with a macro): visible noinline bodies of operator+=(wchar_t), MakeOwnCopyOfData and AllocStringData give RELOCATION_MATCH even with a
  dummy AllocStringData, so c2 only needs those bodies in the TU (retail: all live in CWideString.cpp).
- Form 2 (handed back, rows/0099c120.cpp): the header class plus the HEAD row's three same-TU strong definitions, kept and marked
  `// Fidelity: same-TU strong definition of 0x... (retail TU-mate; HEAD debt)`, reaching CWideStringData only through PDB-attested
  members, all declaration-only opt-ins (hdr/README.md): ??0CWideStringData@@QAE@PB_WJ@Z, ?Append@CWideStringData@@QAEX_W@Z,
  ?MakeOwnCopyOfData@CWideString@@IAEXXZ, plus the existing FABLE_RETAIL_CWIDESTRING_TOCHARSTRING_BODY (PeekStringData, inline in retail).
  RELOCATION_MATCH; vland (work/b1.log) RELOCATION_MATCH PASS with no warning (the test is the HEAD standalone one: debt, as NF219 left
  0x0099B4D0's).
- Goal measured: ?UnassignString@CWideString@@IAEXXZ is now COMDAT selection 2 (was 1), 62 = 62 v 0x0099B4D0, bytes and relocs equal
  (cmpcopy; HEAD 59 v 62). lnk.py (LINK unset, /NODEFAULTLIB /FORCE) 0x0099C120 + 0x0099B4D0: HEAD 1 duplicate (UnassignString) -> 0.
- Other copies: the three strong copies keep HEAD's leaves and selection 1 and differed from their owners at HEAD (AllocStringData
  83 v 121, MakeOwnCopyOfData 42 v 128, operator+= 76 v 203; now 64 / 42 / 74): debt under the nf203 rule, still one LNK2005 each with
  their owners, as at HEAD. ??0CWideString@@QAE@ABV0@@Z (66 = 66, equal at HEAD and now) and ??1CWideString@@QAE@XZ (65 v 68 at HEAD and
  now) are pick-any as at HEAD. New pick-any header COMDATs (STLport _Destroy / __destroy / __destroy_aux / __true_type ctor /
  _STLP_alloc_proxy and allocator dtors / _M_deallocate_block / deallocate<unsigned short>, CWideStringData::PeekStringData): no
  reloc_bindings line and no landed Leaf owner. Gone: the HEAD local STLport models' symbols (??0?$allocator@G, ??0CWideStringData@@QAE@PBGK@Z,
  the undefined basic_string ctor / append(size_type, G)). free / operator delete / NoStrings spellings unchanged.
- Self leaf unchanged (??H@YI?AVCWideString@@ABV0@G@Z): no rename, no binder change. The 0x0099B4D0 binder spellings stay as NF219 listed.

## q12383: the ??8?$CBasicString@D@@QBE_NPBD@Z copies in the size TUs: 2 of 5 rows done, 3 open (context)
- Header (hdr/fable_string.h, add-only, after the CBasicString class): FABLE_RETAIL_CBASICSTRING_TEXT_EQ_OWNER_BODY (with
  FABLE_RETAIL_CBASICSTRING_COMPARE_BODY): an explicit specialization `template<> inline __declspec(noinline) bool
  CBasicString<char>::operator==(const char*) const { return Compare(PData,text)==0; }` between `#pragma optimize("ty", on)` (the owner
  0x004115A0's command-line mode: catalog default /O2 /Oy /W3; EXTRA_RULES 2026-11-21 condition 2) and `#pragma optimize("", on)`; the
  SIZE_BODY comment wording (Fidelity: scaffolding, not retail; ends with optimize("", on), so no row-head optimize pragma before the
  include). NF216's FINDING is why it is an explicit specialization: an implicit instantiation takes the END-of-TU mode.
- Row recipe (tools/eqowner.py): the row-head `#pragma optimize("s", on)` moves to just after an explicit `#include "fable_string.h"`
  placed before the first engine include (nf189 placement), and the row-local out-of-class Compare / operator== template definitions go
  (Compare is the header's COMPARE_BODY in-class body).
- 0x005A4CDD: RELOCATION_MATCH (no catalog flags; also with /Gr), vland PASS. == copy 19 -> 88 = owner (cmpcopy bytes and relocs equal; COMDAT
  selection 2). comdiff v HEAD (278 functions): 3 changed: == (to the owner), ??8CCharString@@QBE_NPBD@Z 35 -> 39 (no table line, no
  Leaf owner) and ?__copy_trivial@std@@YIPAXPBX0PAX@Z 35 -> 30, which DIFFERED from owner 0x00405BA0 at HEAD and now equals it (bytes and
  relocs). Both side changes come from fable_string.h's inlines now compiling in the command-line mode (the pragma move), not from the
  opt-in. Compare copy 37 v owner 40: unchanged HEAD debt.
- 0x005A5B31: RELOCATION_MATCH, vland PASS; comdiff (150): == 19 -> 88 = owner (bytes and relocs), ??8CCharString@@QBE_NPBD@Z 35 -> 39
  (ownerless). Link (lnk.py) of both rows + owner 0x004115A0: 0 duplicates (HEAD 0).
- Condition 1 (measured on 0x005A4CDD): the same visible body without the pragma scaffolding (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
  in place of _OWNER_BODY, work/005a4cdd_c1.cpp) grades RELOCATION_MATCH and comdiff against the handed-back form shows exactly one
  change: == 19 -> 88. So the scaffolding changes only that COMDAT. (With no == body at all the leaf DIFFERs 3672 v 3668: the leaf needs
  the visible body, as at HEAD.) Conditions 2 and 3: the opt-in comment cites the owner's catalog flags and ends with optimize("", on).
- Open: 0x005A04F7 (form 1 SRC_FAIL: `CCharString(...)` not found at the first CCharString use once fable_string.h is included ahead
  of the engine headers; cause not settled: an engine header must set a macro before fable_string.h; next form: include after the
  macro-setting header), 0x00CBFB7D and 0x0065A075 (/O1 /Oy-: "ty" restores the owner mode; not attempted, context).

## Entry 4 (measure only, nothing changed): ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ, 0x00429C53 v 0x004291DE
- Spellings (two sources each): 0x004291DE = ?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ (FableWin.pdb name +
  the 44-byte release body, Data zeroed before Info; called on live pointers: ClearBastardChild 0x0041DEED, Reset 0x00429C72,
  ShareData 0x00429217). 0x00429C53 = ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ (Ego_r.pdb ??1 at 0x00438DDC is a 5-byte
  function, and retail 0x00429C53 is the 5-byte `jmp 0x004291DE`; called at local scope ends 0x0054522E / 0x0056BBB9). Same pair as
  the q12226-q12229 family above (`~CCountedPointer() { ReleaseData(); }`).
- Census (callers.py over Fable.exe, work/c4_callers.tsv; source classification work/c4_class.tsv; absrefs.py: no absolute
  reference to either address). Every row named below was compiled at HEAD (q.py, RELOCATION_MATCH) and its relocations read with xref.py
  unless marked "source-classified".
  - 0x004291DE: 576 E8/E9 sites = 553 in 108 landed rows + 23 in no landed row. ??1 bound there (the conflict) by 4 rows, 47 sites, all
    confirmed: 0x0042D45D map<CCharString,CCountedPointer<CComponent>>::operator[] (2, default header path), 0x005C8459 (18, default),
    0x005C9A95 (16, default; grade it with no flags: its catalog CompilerFlags `/Os /Oy-` DIFFERs, the bare-flags lane caveat),
    0x0058F016 (11, DTOR_DECL_ONLY). The other 104 rows bind ReleaseData: confirmed 0x0041DED2 (2), 0x0041E6D3 (38, local model),
    0x00485524 (1, local model), 0x00596A68 (3; source-classified "default" but it binds ReleaseData), 0x00570332 (9); the remaining 99
    are source-classified (RELEASEDATA / _DECL_ONLY / _NOINLINE defined).
  - 0x00429C53: 26 sites, all in 6 landed rows, ??1 (right): 0x00544D20 (1, confirmed), 0x00563920 (15, confirmed; a RELEASEDATA row:
    its out-of-line destructor calls bind ??1), 0x0056AD40 (7), 0x00626110 (1), 0x00626750 (1), 0x00601310 (1) source-classified.
  - reloc_bindings lines with target 0x004291DE or 0x00429C53 (all of them): ReleaseData<CComponent> -> 0x004291DE (witness 0x0041DED2)
    and ??1<CComponent> -> 0x004291DE (witness 0x00570332). The second is STALE: 0x00570332 today binds ReleaseData at all 9 of its sites
    (its ??1 is only the pick-any 5-byte copy it defines). No line for 0x00429C53.
- Proposed plan: move the 4 rows above (47 sites) onto FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY (the one-line change q12226's
  Gate B measured on eight rows), grading each; their out-of-line destructor calls (if any) then bind ??1 at the thunk, as 0x00563920's do.
  Table: replace the stale ??1 -> 0x004291DE line by ??1 -> 0x00429C53 (witness 0x00563920). Overlap with the q12226 family: one row,
  0x0041E641 ~CManager (1 site, already binds ReleaseData here; q12226 changes it for its SMember wrappers), so the two passes touch
  that row together but neither moves its CComponent binding. Locks: none taken.

## Summary for the coordinator
- Entries: q12226-q12229 blocked (family of about 40 rows; gates A / B measured, census + plan above); q12401 done; q12383 partial
  (blocked: 2 of 5 rows handed back, 3 open); entry 4 done (measure only). Rejected / deferred: none.
- Rows handed back (rows/, sources only; tests unchanged HEAD files): 0x0099C120, 0x005A4CDD, 0x005A5B31. recheck_final.log: vland
  --recheck-landed, 3 RELOCATION_MATCH + PASS (warnings: 0x005A4CDD / 0x005A5B31 RELOC_NAME on 0x004DD1C4's label and 0x005A5B31
  SHARED_HEADER_AVAILABLE CThingPlayerCreature, both from text the rows had at HEAD).
- EH funclets: no landed row or test includes any of the three changed row files (grep of rebuild/src and rebuild/tests).
- fable_string.h asserts: all four are FABLE_STATIC_ASSERT_AT(565..568, ...) with explicit line numbers, so the 52 inserted lines
  move no assert key (pin_asserts.py: pinned 0).
- Header: hdr/fable_string.h, 7 add-only opt-ins (hdr/README.md); default path token-identical; pinned 0; no landed includer defines them.
- No renames (RENAMES.tsv empty), no binder respelled, no split created or grown. NAME_FIXES.tsv: the family findings (not applied).
- Locks: rows.txt = the brief's 11 rows (changed: 0x0099C120, 0x005A4CDD, 0x005A5B31; the other 8 untouched, still NF220: release or
  hand on). The 26 q12226 family rows claimed for the census were released again (owner `released`).
- Flags lines: none added. 0x005A4CDD has no CompilerFlags in its catalog block (an earlier grep -A14 read the next block's /Gr); it grades RELOCATION_MATCH with no flags (vland) and with /Gr. Descriptive names: none.
- Rule slips: none known. The 0x0099C120 diagnostic probe used `#define protected public` in work/ only (never handed back).
