# Wave 9 headers: what changed and the evidence

One new header, `engine/CCountedPointer.h` (CCPPointerInfo + CCountedPointer<T>). No existing header changed: the NScript
headers keep their contract ("the includer defines CCountedPointer<T> before the include"), and a row now satisfies it with
`#include "engine/CCountedPointer.h"`.

| header | change | includers checked |
|---|---|---|
| engine/CCountedPointer.h | **new** (no opt-in macro: nothing included it before) | 1,553 kept rows (CP 1,548 + CPS 5) mcheck SAME / same-target and regraded PASS; 2,889 rows tried in total (below); g++ `-m32 -std=c++98 -fsyntax-only` and a VC7.1 TU (`work/hdr_cp.cpp`: const T, converting assignment, explicit instantiation) compile |

## CCPPointerInfo (12 bytes)

- Layout from Ego_r.pdb (`struct_layouts_egor.tsv`): RefCount +0x0 (ulong), DeleteFunc +0x4, Data +0x8 (void*).
- DeleteFunc is `void (__fastcall*)(void*)`:
  - every retail release passes the object in ecx: `mov ecx,[eax+8]; call [eax+4]` (0x006E7AB0, 0x00CE1000, 0x00CE1040,
    0x008AB2B0);
  - the functions stored there are FableWin.pdb `?DeleteData@?$CCountedPointer@..@@SIXPAX@Z` (static, `I` = __fastcall),
    343 instantiations. Ego_r's `_func___cdecl_void_void_ptr` is its generic function-type spelling.
- Ctor: FableWin.pdb `??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z` = `(unsigned long, void (__fastcall*)(void*), void*)`. Retail
  Reset 0x008AB2B0 expands it after `new(0xc)`: `[eax]=1; [eax+4]=deleter; [eax+8]=Data` (member order). The rows' invented
  `CCPPointerInfo(void* data, deleter)` ctor is gone with their local blocks; the header's (T*) ctor and Reset call the PDB one.

## CCountedPointer<T> (8 bytes for every T)

- Layout: Data +0x0 (T*), Info +0x4 (CCPPointerInfo*): Ego_r.pdb, 40+ instantiations, all 8 bytes.
- Member set = FableWin.pdb (the debug build emits them all; Ego_r keeps out of line only ShareData 3 / ReleaseData 1 /
  Reset 65 / DeleteData 86 and two ??1 / ??4 copies):
  ??0 (), (T*), (const&), member template ??$?0 (363 FableWin instantiations); ??1; ??4 (const&), member template ??$?4 (4);
  ??C operator-> QBE; ??D operator* QBE; DeleteData SIXPAX@Z; Get / get QBE; GetReferenceCount QBEKXZ; IsNull QBE_NXZ;
  ReleaseData QAEXXZ; Reset / reset QAEXPAV; ShareData QAEXPAV..PAVCCPPointerInfo@@@Z; Validate QBEXXZ (declared only);
  member template ??$SetStaticCast (63 FableWin instantiations) = `ShareData(static_cast<T*>(other.Data), other.Info)`, the
  body the landed 0x00A32620 expands (93 rows model it alike; 5 moved onto the header SAME + PASS, family CPS). Not
  modelled: the free __fastcall ??$?8 / ??$?9 templates (operator== / != of two pointers).
- Bodies (retail bytes):
  - **~CCountedPointer**: 0x006E7AB0 (MovieBase), 0x00CE1000 (CEntityScriptBase), and 730 more /O2 COMDATs found by a byte
    scan of .text (retail has no ICF: one COMDAT per T). `if (Info) { dec [Info]; reload Info; cmp [Info],0; jne;
    ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }`, then **Data = 0, Info = 0** in that order.
    41 /Os copies have the same shape (`and [esi],0; and [esi+4],0`).
  - **ReleaseData**: the same release, then **Info = 0, Data = 0** (Info first). Expanded in 0x00A45330 / 0x00A5F250 /
    0x006633A0 (`mov [x+4],edi; mov [x],edi`). The scan finds no out-of-line COMDAT with the Info-first tail, so retail
    always expands it. The landed rows that call a `ReleaseData` COMDAT name addresses whose bytes have the Data-first tail
    (0x0042F974, 0x005C04BF): those are dtor COMDATs (NAME_FIXES.tsv).
  - **Reset(T*)** 0x008AB2B0 (and 197 more, the scan's `8b442408 85c0 c746..` tail): ReleaseData expanded leaves only
    `mov [esi+4],0` (its Data store is dead), then `Data = data; if (data) Info = new CCPPointerInfo(1, DeleteData, Data)`.
  - **ShareData** 0x00CE1040 (Ego_r.pdb `?ShareData@?$CCountedPointer@VCScriptThing@@@@..`): `if (Info != info) {
    <ReleaseData>; Data = data; Info = info; if (info) ++info->RefCount; }` (both zero stores dead).
  - **operator=** 0x00704580 (CScriptThing): loads o.Data / o.Info, then the ShareData body, returns *this.
  - **(T*) ctor**: the Reset allocation without the release (out of line at 0x008C58F0 / 0x008C5980, `ret 4`).
  - **copy ctor**: copy both words, `if (Info) ++Info->RefCount` (expanded in every CScriptThing copy 0x004ABE90).
  - **DeleteData** = `delete static_cast<T*>(data)`: 0x00CDEE00 (polymorphic T: `test ecx,ecx; je; mov eax,[ecx]; push 1;
    call [eax]`) and 0x00674E80 (CExpressionSet: the inline implicit dtor, then operator delete). The landed DeleteData rows
    (e.g. 0x00674E80, 0x00436BA0, 0x004ADC40) instantiate it with `template class CCountedPointer<T>;`.
- **Model test** (955 landed rows: every non-SAME row of the first pass plus 300 random rows with dtor / copy / assign /
  ShareData members; logs/mc_cpvar_*.txt):
  - the split above (dtor with its own body, ReleaseData zeroing Info first): 15 DIFFER;
  - `~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }`: 20 DIFFER;
  - `~CCountedPointer() { ReleaseData(); }` with a Data-first ReleaseData (first pilot, logs/mc_cp_pilotA.txt): the leaf
    bytes match at 3 more out-of-line-dtor rows (0x00418B7A, 0x0042DD28, 0x0064FA0F), but VC7.1 then calls a ReleaseData
    COMDAT where retail calls the Data-first dtor COMDAT: a wrong callee name. Rejected.
  - operator= with its own copy of the ShareData body (logs/mc_cp_pilot_opfull.txt): 4 more DIFFER. Rejected.
- **Inlining**: every member is plain in-class `inline`. On the true bodies VC7.1 reproduces the out-of-line dtor calls of
  the rows that declared `~CCountedPointer();` (0x00418B7A calls ??1 at 3 sites; SAME), and the forced rows too.
- unproven: the source order of Reset's two stores (the dead Data store hides it); the bodies of GetReferenceCount / reset /
  Get / get / IsNull (FableWin names only; no retail out-of-line copy was found).

## What the header reproduces, by the row's old local model (2,889 rows tried, logs/mc_cp_final.txt)

| local model shape | SAME | RELOC_RENAMED (same target) | DIFFER | SRC_FAIL |
|---|---:|---:|---:|---:|
| data members only | 927 | 1 | 0 | 0 |
| T* ctor with a row deleter (`Destroy_<addr>`) | 7 | 591 | 1 | 0 |
| inline dtor (any) | 449 | 7 | 0 | 0 |
| ShareData body | 60 | 0 | 0 | 0 |
| forced members (`__forceinline` dtor / copy) | 79 | 0 | 0 | 2 (T = void) |
| out-of-line declared members (dtor / copy / operator=) | 166 | 0 | 9 | 6 |
| noinline dtor | 0 | 0 | 1 | 0 |
| other inline members | 579 | 0 | 4 | 0 |

- The 591 renames are `?Destroy_<addr>@@YIXPAX@Z -> ?DeleteData@?$CCountedPointer@V<T>@@@@SIXPAX@Z`; reloc_bindings.tsv
  already binds both spellings to the same address (0x00CDEE00 for the entity Allocs), so no binding was added.
- **Shapes the header cannot reproduce** (REVERTED.tsv family CP / CPA):
  1. **A called operator=** (`operator=` declared out of line; retail calls 0x00704580 where VC7.1 expands it): 0x00CF8970,
     0x00CF8A70, 0x00D36620, 0x00D374A0.
  2. **The CreateActiveEntityScript shape** (all 85 CAES rows, family CPA): retail calls the (T*) ctor 0x00CE10B0, ShareData
     0x00CE1040 and the dtor 0x00CE1000 out of line inside one function; on the header VC7.1 expands them (347 v 272 in
     every row). The rows keep their local `__declspec(noinline)` / declared-only model.
  3. **A called Reset**: CScriptGameResourceObjectMovieBase::Release 0x00CF8850 (`add ecx,8; call 0x008AB2B0`): 49 v 11.
  4. **Per-site mixes the header expands differently**: 0x00545B10, 0x008AE2A0, 0x0093B1D0, 0x00A156D0, 0x00A157C0,
     0x00A6A540, 0x00CE0600, 0x00D643A0, 0x00D67270, 0x00E12330.
  5. **Models the PDB refutes** (compile failures): converting constructors between unrelated row-local types
     (CVillageTask* -> CVillageTaskBase, NUISystem::CComponent -> CText / CList: the rows' local classes lack the real
     bases), `CCountedPointer<void>` (0x00D37530, 0x00DD6440), an invented `operator bool` (0x0064440B).

## MovieBase (time-boxed, residue recorded)

- `mig_mb.py` moved the 20 rows whose local model has the header's shape (GROBase base, PImp only, no declared dtor) onto
  engine/CScriptGameResourceObjectMovieBase.h over the shared pointer. 1 is SAME (0x00ED0B10); 19 DIFFER and were reverted.
- 0x00DF2D30 (single AB0 site) still expands fully, 385 v 349, under every variant tried with the shared header
  (work/mbv/): either dtor split, explicit instantiation of the pointer before or after Main, an earlier `delete p` /
  local destruction, user-declared GROBase / MovieBase ctors, extra inline call layers in Main.
- **New clue for wave 10** (from wave 8 logs/mbsites.txt): the destruction form follows whether the same function expands
  the MovieBase **ctor**.
  - All 35 rows whose only form is AB0 (expanded, PImp dtor 0x006E7AB0 called) expand the MovieBase ctor in the same
    function (`ctor1` / `ctor2`: an inline 0x01260EF4 store).
  - All 26 rows whose only form is INL (fully expanded) call the ctor or construct no MovieBase there (`ctor0`).
  - The 59 mixed rows split 21 ctor0 / 38 ctor1-2.
  - So the expansion budget looks per function: an expanded ctor leaves no room for the third level (the PImp dtor).
    VC7.1 expands both here, so the caller-side cost it sees is smaller than retail's. unproven.
