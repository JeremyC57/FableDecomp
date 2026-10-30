# Wave 2 class-model migration: shared headers added or changed (2026-10-30)

All paths are under `rebuild/include/engine/` in the worktree `$S/mig2/repo`. Every header below compiles under VC7.1 (the
lane's `cl /O2 /W3`) and under `g++ -m32 -w -std=c++98 -fsyntax-only`. The probe source `$S/mig2/work/t/g_all2.cpp` is wave
1's `g_all.cpp` plus the wave 2 headers. All three are opt-in: each needs `FABLE_RETAIL_DEF_CLASS_BASE` (CDefPointer.h has an
`#error` without it). The flattened `#else` layouts stay the default, so existing includers are unchanged.

## CParentDefClassBase.h: new retail branch (FABLE_RETAIL_DEF_CLASS_BASE)

The generated flattened layout stays in the `#else` branch. The header's first line now carries RETAIL_MARK wording, so
`gen_class_headers.py` keeps the branch.

- **Chain:** RTTI `.?AVCParentDefClassBase@@ : CDefClassBase` (ghidra_out/rtti_bases.tsv).
- **Vtable:** 0x01230CBC, 25 slots.
- **Probe:** `$S/mig2/work/t/probe_pdcb.cpp`, a CProbeDef deriving from the header, compared slot by slot with
  `scripts/vtprobe.py`. All 25 emitted slot symbols match the retail slot names (slots 18-20 are the probe's own overrides).

| slot | retail | declaration | evidence |
|---|---|---|---|
| 0 | 0x004303F0 | `virtual ~CParentDefClassBase()` | scalar deleting dtor; frees `[esi+0x29]`, stores CDefPointeeBase vtable 0x01230BA0, calls ~CResource 0x009FC550 |
| 10 | 0x004303B0 | `CDefString GetInstantiationName() const` | `mov ecx,[ecx+0x25]`; ?GetInstantiationName@CParentDefClassBase@@UBE?AVCDefString@@XZ (Ego_r + FableWin) |
| 11 | 0x004303C0 | `void SetInstantiationName(CDefString)` | `mov [ecx+0x25],eax`; both PDBs |
| 12 | 0x009B00D0 | `void AddSubDef(const CCharString&, long, long)` | both PDBs |
| 13 | 0x009B0170 | `void RemoveSubDef(const CCharString&)` | both PDBs |
| 14 | 0x009AD4A0 | `const CSubDefInfo* GetPSubDefInfo(const CCharString&) const` | UBE spelling in both PDBs |
| 15 | 0x009AD510 | `CSubDefInfo* GetPSubDefInfo(const CCharString&)` | UAE spelling; declared first (VC7.1 reverses overloads) |
| 16 | 0x004303D0 | `bool IsParentDef() const` | `mov al,1; ret` |
| 21 | 0x009B0200 | `void InternalSaveBinary(CDataOutputStream&)` | both PDBs |
| 22 | 0x009AED70 | `void InternalLoadBinary(CDataInputStream&)` | both PDBs |
| 24 | 0x004303E0 | `CVectorMap<..>* GetPSubDefInfoMap()` | `lea eax,[ecx+0x29]` |

- The other slots are inherited unchanged: 1-9, 17 and 23 (Validate stays CDefClassBase's 0x0042DA90), and 18-20 are
  `_purecall`. The brief said "slots 10-16 and 21-23"; the retail vtable has slot 23 inherited and slot 24 overridden.
- **Members:**
  - InstantiationName (CDefString, a 4-byte TablePos) is at +0x25.
  - SubDefInfoMap (CVectorMap, 0x10 bytes: CArray 0xC, Compare +0xC, Dirty +0xD) is at +0x29.
  - Size 0x39, under `#pragma pack(1)` like its base.
  - Evidence: the slot bodies above, and the ctor 0x00430370, which stores -1 at +0x25, zeroes +0x29/+0x2D/+0x31 and stores 0
    in the Dirty byte at +0x36. Ego_r.pdb gives 0x3D because its CVectorMap is a 0x14 Dinkumware vector.
- **Typed or opaque members:** there is no shared CDefString or CVectorMap header. An includer that defines both types
  completely defines `FABLE_PDCB_TYPED_MEMBERS` and gets typed members. Otherwise the two members are opaque at the same
  offsets (`long InstantiationName_TablePos`, `unsigned char SubDefInfoMap[0x10]`).
- **Out-of-line members:**
  - **Ctor 0x00430370** (`??0CParentDefClassBase@@QAE@XZ`, both PDBs, bound). CDefClassBase's private ctor and the CDefString
    and CVectorMap ctors are inlined into it.
  - **~CParentDefClassBase 0x00430300** (`??1..UAE@XZ`, both PDBs, bound). This is out of line, not inline. The derived
    deleting dtors call 0x00430300. The class's own deleting dtor 0x004303F0 inlines the same body, because it is compiled
    in the TU that defines the dtor (the same-TU auto-inline rule in LESSONS.md).
  - **Copy ctor 0x0046EE98** (`??0..QAE@ABV0@@Z`) and **operator= 0x00431ED0** (`??4..QAEAAV0@ABV0@@Z`). Both are in both
    PDBs and bound.
- **Unproven:** whether the copy ctor and operator= are user-declared or implicit.
  - 64 of the 67 landed out-of-line callers of 0x00431ED0 are size-optimised TUs.
  - 5 /O2 rows expand the operator= inline (0x00432C70, 0x00432E60, 0x00434450, 0x004345A0, 0x0067C980).
  - That split looks like an implicit, inline-able operator= (GOTCHAS line 1072). The header declares the two members out
    of line because every migrated row needs that, and an opaque member can't give the implicit body. See NEXT.md item 3.

## CIVCountedPointeeBase.h (retail branch): reference-count accessors

- **Added:**
  - `void AddRef() const { ++IVRefCount; }`
  - `long Release() const { return --IVRefCount; }`
  - `long GetRefCount() const { return IVRefCount; }`
  - `IVRefCount` is now `mutable unsigned long`.
- **Spellings:** FableWin.pdb `?AddRef@CIVCountedPointeeBase@@QBEXXZ`, `?Release@..@@QBEJXZ` (returns long) and
  `?GetRefCount@..@@QBEJXZ`. Ego_r and retail inline them everywhere, so there is no retail body.
- **Retail shapes:**
  - the GetSubDef family gives `inc [eax+4]`, `dec [ecx+4]; jne` and, for the manager's temporary,
    `mov edi,[eax+4]; dec edi; mov [eax],edi`;
  - all three come out of these accessors through CDefPointer.h;
  - 151/151 rows give the same bytes.
- **`mutable` or `const_cast`:** this can't be told from the bytes, because the two compile to the same code. `mutable`
  follows the landed local models; the header comment says so.

## CDefPointer.h (new; FABLE_RETAIL_DEF_CLASS_BASE)

- **Layout:** Ego_r.pdb CDefPointer<T> is 4 bytes, with `CDefPointeeBase* Object` at +0 (struct_layouts_egor.tsv, every
  instantiation).
- **Member set:** FableWin.pdb (a debug build, so nothing is inlined there). Instantiation counts:
  - `??0(ENullDef)` 332;
  - copy ctor 52;
  - the template converting ctor `??$?0` (for example `CDefPointer<const CDefClassBase>::CDefPointer<CDefClassBase>`);
  - `??1` 249;
  - `??4` 31;
  - the template `??$?4` operator=<U>;
  - Release 249;
  - InternalGetObject 245 (const, returns CDefPointeeBase*);
  - `SetDynamicCast<const CDefClassBase>` 246;
  - `operator->` 229;
  - `operator*` 42;
  - Get 73;
  - IsNull 42;
  - Reset(const T*) 18;
  - GetRefCount 1.
- **GetSubDef<CAreaOfEffectAttackDef> (FableWin):** its symbol set is exactly ctor(ENullDef), dtor, operator->, Release,
  SetDynamicCast<const CDefClassBase>, InternalGetObject, `GFDebugPDynamicCast<CDefPointeeBase*, const T*>` and
  `T::StatGetTypeName()`. The migrated rows follow that set: `GetPSubDefInfo(T::StatGetTypeName())`, then
  `CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(..)` (the template ctor), then
  `out->SetDynamicCast(def)`.
- **Inline bodies:** one template gives both retail release shapes. `Release()` must hold the pointee in a local:
  reloading `Object` before `Delete()` gives `mov ecx,[esp+8]` where retail reuses ecx.
- **`__forceinline`** (`FABLE_DEFPTR_INLINE`, which is plain `inline` under g++) on the copy and template ctors, the dtor,
  both operator= forms, SetDynamicCast, Release and the private Assign:
  - no relocation binding names any CDefPointer member;
  - the four out-of-line operator= bodies (0x00592844, 0x005C07AE, 0x005F7E99, 0x006551FF) are unreferenced COMDATs;
  - size TUs inline them too. With plain `inline`, rows 0x007BE88C and 0x007BE92E call them (112 vs 162 bytes).
- **Unproven or not declared:**
  - the ENullDef ctor: the enumerator name is in neither PDB name list, and the landed rows spell it three ways;
  - Reset's body (declared only);
  - the exact Lionhead spelling of Release (the local-pointer form is what the bytes need).
