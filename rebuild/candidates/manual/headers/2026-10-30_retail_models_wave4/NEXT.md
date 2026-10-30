# Wave 5: remaining class-model debt and the plan (counts from the wave 4 worktree, 2026-10-30)

A row counts when it defines `class|struct <X> {` (template prefix allowed) under `rebuild/src/compiled`.

| class | after wave 3 | after wave 4 | shared header |
|---|---:|---:|---|
| CGameScriptInterface | 1284 | **700** | CGameScriptInterface.h |
| CBaseClass | 612 | **21** | CBaseClass.h |
| CParentDefClassBase | 108 | **67** | CParentDefClassBase.h |
| CDefClassBase | 689 | **647** | CDefClassBase.h |
| CResource / CDefPointeeBase | 353 / 501 | 330 / 477 | def-chain headers |
| CIVCountedPointeeBase / CDefPointer / CSubDefClassBase | 380 / 1241 / 75 | same | def-chain headers |
| CSpawnedFuncBase | 16 | 16 | CSpawnedFuncBase.h |
| CScriptBase / CScriptThing | 1704 / 2503 | same | NScript headers |
| CGameScriptInterfaceBase | 202 | same | NScript headers |
| CEntityScriptBase / CLevelScriptBase / CTimer | 1122 / 951 / 200 | same | NScript headers |
| CBaseObject (no header) | 652 | 652 | none: new header, brief item 5 |
| CScriptGameResourceObjectBase / ...ScriptedThingBase / ...MovieBase (no header) | 199 / 390 / 97 | same | none |
| NScript::CIsThingConscious (no header) | 49 | 49 | none |
| CSpawnedFunc / CSpawnedFunc1 (no header) | 232 / 21 | same | none |

Wave 4 edited and regraded 1,228 rows (ROWS.tsv): CGSI 584, BC 591 (52 of them also took the CGSI header), PDCB 41, DCB 12.
REVERTED.tsv lists the 124 rows that were tried and backed out, each with its residue.

## Ready next (in order)

1. **CGSI, 82 compile failures:** REVERTED.tsv rows with family `CGSI` / `CGSI (BC kept)`. `mig_cgsi.py --rename` accepts
   them, and they fail only on the row's own types:
   - **33 local vector models.** Give the row a minimal STLport `namespace std { template <class T> class allocator;
     template <class T, class A = allocator<T> > class vector {...}; }` in place of its `vector<T>`. The 0045767e
     template in this wave works: 12 bytes, `_M_start / _M_finish / _M_end_of_storage`. Watch the forward declaration in
     fable_nscript_forward.h (`template <class T, class A> class vector;` has no default argument, so put the default on
     the row's definition only if the forward declaration came first. Test.)
   - **26 int-to-enum arguments.** Replace the int with the real enumerator where the enum is known, or a cast. Bytes are
     unchanged, but mcheck it.
   - **11 local CScriptThing stand-ins (CScriptThingRuntime / Local / Ref).** These belong with the CScriptThing family.
   - **9 local CCountedPointer** (struct vs the forward header's class template). **2 local enum typedefs. 1 other.**
2. **CGSI, 460 rows with non-virtual items in the local block.** The block carries members or helpers besides slots, so
   `mig_cgsi.py` skips it. Survey by cluster: most declare `HeroSThing` or a member function next to the slots.
   **35 rows with a named slot at a different index:** check each against the header (invented names to respell, or a
   real overload order question). **40 rows that already include the header.**
3. **BC remainder (21):**
   - 9 rows whose inline local `CBaseClass()` is used outside a copy ctor, in code that reaches the leaf (REVERTED.tsv,
     `BC-other`). Retail constructs the base inline there, so the out-of-line header ctor DIFFERs. These need a look at
     what retail actually builds: probably an inline copy, or a construct that never calls the ctor.
   - 00d9c2b0 (converting ctor from CScriptThing): with the CScriptThing family.
   - 008f7bd0 (`__declspec(noinline)` local model), and 00cfd6c0 / 00f1a990, whose local CBaseClass is EMPTY (the vptr is
     flattened elsewhere).
   - 00a10dc0 (CRenderTarget ctor): its behaviour harness needs dtor stubs first (LESSONS item 6).
   - 2 other shapes.
   - The 5 CBaseClass.cpp TU rows (0x0099A310-0x0099A4D0) stay local by design.
4. **PDCB remainder (67):**
   - 16 rows from this wave's typed pass are in REVERTED.tsv: the 5 /O2 expanders stay local; 0x00430370, 0x004304A0 and
     the 0067c7xx copy family stay local by design.
   - The fixable ones: 006250b0, 0067ca70, 007d79c0, 0067ce50 (non-template CKeyPairCompareLess / CVectorMap: rewrite
     them as the 3-parameter templates), 004303b0 (needs a local CVectorMap for FABLE_PDCB_TYPED_MEMBERS), 007a9d60
     (local CDefPointer: needs engine/CDefPointer.h), 004f9df8 (dtor spelling; see REVERTED).
   - 9 rows whose bytes inline the chain ctor stay local.
   - 004585bd is CArmourDef::~CArmourDef (NAME_FIXES.tsv): rename it, then use the novtable model.
   - The rest are the `shape(item)` rows (local `virtual Transfer`, flattened `__vftable`, `long InstantiationName`).
5. **DCB clusters (647):**
   - 012a455e (24) / fe4a0baa (7): local CDefBase + GetSubDef template.
   - 728549f7 (21) / 28cee7d7 (18) / 4714b782 (8): data-only CDefClassBase copies on a local CDefPointeeBase. mig_dcb.py
     rejects them because they also define CResource / CDefPointeeBase: extend it the way mig_pdcb_typed.py removes the
     whole chain.
   - 83101c63 (18), 46f7e374 (15), 662692ff (14); the Destroy / RefCount stand-ins (e90794eb 11, 27b1d4c5 6, 1e69bd22 6).
   - 6 DCB rows from this wave in REVERTED.tsv.
6. **New headers (brief item 5), not started:**
   - CBaseObject (652 rows) is the largest family left without a header.
   - Then CScriptGameResourceObjectScriptedThingBase (390), CSpawnedFunc<T> (232), CScriptGameResourceObjectBase (199),
     ...MovieBase (97), CIsThingConscious (49).
   - Prove chain, size, every slot and 5 rows each first.

## Bindings and naming (coordinator)

- `rebuild/oracles/reloc_bindings.tsv` is unchanged. `bindings_multi_check.py` prints 0.
- The relocation renames in this wave all go to spellings that are already bound:
  - `??1CBaseClass@@UAE@XZ`, `??1CScriptThing@@UAE@XZ`, `??1CParentDefClassBase@@UAE@XZ` (dtors that are now virtual);
  - `??4CParentDefClassBase@@QAEAAV0@ABV0@@Z` (0046ef87);
  - `$L` jump-table labels.
- The old spellings are still bound because other rows use them. `?Copy@CParentDefClassBase@@UAEXPBVCDefClassBase@@@Z`
  is an invented name for 0x00431ED0.
- NAME_FIXES.tsv adds 0x004585BD (`??1CArmourDef@@UAE@XZ`). Waves 2-3 entries stand. 0x0143E8F8 untouched.
