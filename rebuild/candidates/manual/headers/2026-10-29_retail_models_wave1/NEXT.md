# Wave 2: remaining class-model debt and the plan (counts taken from the worktree after wave 1, 2026-10-29)

## Remaining row-local definitions, per class

A row counts when it defines `class|struct <X> {` (template prefix allowed) under `rebuild/src/compiled`. Shared headers now
exist for every class in this table.

| class | HEAD | after wave 1 | shared header |
|---|---:|---:|---|
| CSpawnedFuncBase | 347 | 16 | CSpawnedFuncBase.h (FABLE_RETAIL_SUSPENDABLE_PROCESS) |
| CDefClassBase | 1007 | 841 | CDefClassBase.h (FABLE_RETAIL_DEF_CLASS_BASE) |
| CBaseClass | 1821 | 1694 | CBaseClass.h |
| CScriptBase | 1707 | 1702 | CScriptBase.h (FABLE_RETAIL_NSCRIPT) |
| CScriptThing | 2506 | 2501 | CScriptThing.h (NSCRIPT branch) |
| CGameScriptInterfaceBase | 206 | 198 | CGameScriptInterfaceBase.h |
| CGameScriptInterface | 1384 | 1381 | CGameScriptInterface.h (NSCRIPT branch) |
| CEntityScriptBase | 1124 | 1120 | CEntityScriptBase.h |
| CLevelScriptBase | 953 | 950 | CLevelScriptBase.h |
| CTimer | 202 | 200 | CTimer.h |
| CPredicateBase / CIsThingAlive / CPredicate_And | 108 / 244 / 48 | 106 / 242 / 46 | the three predicate headers |
| CEntityScriptBinding / ..Base | 179 / 12 | 178 / 12 | CEntityScriptBinding.h, CEntityScriptBindingBase.h |
| CResource / CDefPointeeBase / CIVCountedPointeeBase | 353 / 501 / 418 | 353 / 501 / 380 | def-chain headers (DEF_CLASS_BASE) |
| CSubDefClassBase | 84 | 75 | CSubDefClassBase.h (new) |
| CParentDefClassBase | 332 | 332 | CParentDefClassBase.h (flattened only: needs a retail branch) |

(The CResource and CDefPointeeBase counts did not move, although DCB rows dropped local copies. Those were header-like
forward stand-ins, and most DCB rows had no local CResource.)

## Families ready for a scripted pass (dry-run counts on the remaining files)

- **BC: `scripts/mig_bc.py`.**
  - Dry run: 1,144 `edit ok`, 364 other shapes, 186 skipped because they name a `v00` stand-in.
  - The 238 remaining rows of cluster cd08cff0 are the exact header shape (`CBaseClass(); virtual ~CBaseClass();`).
  - The b638a797 dtor-only (466) and inline-body forms need mcheck. A row that constructs a derived object through the
    implicit ctor will change from an inline vptr store to a call to 0x0099A2D0. Retail calls the ctor out of line
    (0x0099A2D0, 1,225 callers), so such a row's inline model was wrong: investigate each one and don't keep a local
    inline ctor.
- **DCB: `scripts/mig_dcb.py`.**
  - Fixed after the batch: the nested-CSubDefInfo pattern now runs before `public:`. Dry run on the remaining 991 files:
    - 19 `edit ok`;
    - 35 rows with a local CResource in a shape the script rejects (copy ctors, extra members);
    - 30 rows with a local CParentDefClassBase (needs a retail CParentDefClassBase branch first: vtable 0x01230CBC
      overrides slots 10-16 and 21-23, members InstantiationName and a CVectorMap; ghidra_out has the addresses);
    - 6 rows with a CIVCountedPointeeBase that carries AddRef/Release (add the FableWin inline `AddRef() const`,
      `Release() const` (long) and `GetRefCount() const` to the header first; IVRefCount must then be mutable or the
      methods const_cast);
    - 752 other CDefClassBase shapes;
    - 153 with no block (a `struct` key or a macro).
  - The 7 reverted DCB rows are listed in REVERTED.tsv.
- **dd084063 (151 rows), the GetSubDef<T> family.**
  - Its local CDefClassBase has 14 invented `vNN` slots plus `GetSubDefIndex` at slot 14. Slot 14 is the const
    `GetPSubDefInfo` (proven; see HEADERS.md). Rewrite the call to `const CSubDefInfo* p = GetPSubDefInfo(name);` /
    `p->DefIndex`.
  - The local CDefBase/CRefCounted/CDefPointer model must become a real `CDefPointer<T>` header. The PDB members are:
    - `Object` (CDefPointeeBase*) +0;
    - Release, InternalGetObject, the `SetDynamicCast<U>` template, the `operator=<U>` template, Get, GetRefCount, Reset;
    - CIVCountedPointeeBase AddRef / Release / GetRefCount.
  - Before scripting the 151 rows, prove the inline bodies on 5 rows: the temporary release `mov edi,[eax+4]; dec edi;
    mov [eax+4],edi` versus the member release `dec [ecx+4]`.
- **Other DCB clusters:**
  - 012a455e (24) and fe4a0baa (7) use a local CDefBase;
  - 83101c63 (18) has a full local CResource chain with GetPSubDefInfo naming. It is probably mechanical once the slot
    names line up;
  - 46f7e374 (15) declares `virtual void Copy` alone;
  - 662692ff (14) declares the GetSubDef template only;
  - e90794eb / 1e69bd22 / 27b1d4c5 (11 / 6 / 6) use `Destroy` / `RefCount` stand-ins.
- **SFB:** 16 rows left. The shapes are 577495d5 (5), 0e822918 (4), NOBLOCK (4), f836a710, ccadef22. They need a hand edit.
- **NScript (Part A classes):** these are the bulk of the debt (CScriptBase 1,702, CScriptThing 2,501, CEntityScriptBase
  1,120, CGameScriptInterface 1,381).
  - The headers and their contract are proven on 8 rows.
  - A script needs to:
    - (a) drop the local `CGameScriptInterfaceBase { vNNN...; Named(); }` blocks for the include (their named slots must
      be checked against the header's spelling, because overload resolution and const-ness change the call only if a
      spelling differs);
    - (b) keep the row's own `CCountedPointer<T>` model (the CScriptThing contract);
    - (c) move inline bodies (CTimer::operator=, the ~CScriptThing / copy / wrapper expansions) out of class after the
      include;
    - (d) convert invented helper members on the local CScriptBase (`SpawnFunction`, ...) into free inline helpers, or
      into the real `AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&)` 0x00CB7E50.
  - Do the rows that only use the interface (`Game->X(...)`) first: those are the commonest shape. Then CScriptBase
    plus CLevelScriptBase, then CEntityScriptBase plus CScriptThing.

## Open items carried into wave 2
1. The CScriptThing copy that is inline at one site and out of line at another (row 00f05f30) has no shared-header
   expression. Possibly retail's copy ctor is inline in the header, and VC7.1's inliner declines it inside the
   forceinlined CSpawnedFunc1 ctor. Try the `inline` copy ctor plus a non-forceinline CSpawnedFunc1 ctor across more
   spawn rows before deciding.
2. NAME_FIXES.tsv:
   - the `?Copy@CBlurredAttitudesDef` U-spelling (row 0067c980 stays reverted until it is respelled);
   - the CEntityScriptBinding slot labels;
   - candidate `??0CDefClassBase@@AAE@XZ` (one source).
3. Invented spellings were noticed in rebuild/oracles/reloc_bindings.tsv and are not fixed in wave 1:
   - `??0CScriptThing@@QAE@PAX@Z`;
   - `??1CScriptThing@@QAE@XZ` (0x004AA840: retail is the virtual ??1..UAE@XZ slot-0 dtor);
   - `?IsActiveThreadTerminating@CEntityScriptBase@@QAE_NXZ` (Ego_r has QBE);
   - the `?SetPredicate` variants;
   - `??0CPredicateBase_00c26c10`.
   - 0x0143E8F8 carries 7 names: g_pGameScriptInterface, g_Game, g_pScriptTimers, g_pThingManager*, g_timerMgr_cd4470, ...
     The row sources still reference `g_pGameScriptInterface`. The one-address-one-name cleanup is a coordinator task.
4. CGameScriptInterfaceBase slot 308 is a `long` stand-in for the nested enum ECategory. A shared
   `CHeroLogBook::CHeroLogBookEntry` header would remove it.
