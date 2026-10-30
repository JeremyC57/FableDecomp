# Wave 5 headers: evidence per slot and member, and what is still unproven

The evidence comments are in the headers themselves (`rebuild/include/engine/`). This page summarises them.
Every slot table was checked by `scripts/vtprobe.py`. It compiles a probe TU that instantiates the class under VC7.1 RTM
3077 and compares the emitted vtable's mangled slot names, in order, with the retail labels of the retail vtable's
entries. Five or more landed rows per header were mcheck SAME before any bulk migration.

| header | opt-in | retail vtable | slots proven (vtprobe) | size | landed rows on it (wave 5) |
|---|---|---|---:|---:|---:|
| CBaseObject.h (retail branch) | FABLE_RETAIL_BASE_OBJECT | 0x0129A7D8 | 2 / 2 | 0x8 | 449 (BO) |
| CScriptGameResourceObjectBase.h (new) | FABLE_RETAIL_BASE_OBJECT | 0x0126008C | 2 / 2 | 0x8 | via GRO |
| CScriptGameResourceObjectScriptedThingBase.h (retail branch) | FABLE_RETAIL_BASE_OBJECT | 0x0127094C | 30 / 30 | 0x10 | via GRO |
| CScriptGameResourceObjectMovieBase.h (new) | FABLE_RETAIL_BASE_OBJECT | 0x01260EF4 | 2 / 2 | 0x10 | via GRO |
| CIsThingConscious.h (new) | FABLE_RETAIL_NSCRIPT | 0x012C2FE8 (no RTTI) | 3 / 3 | 0x10 | 1 |
| CSpawnedFunc.h / CSpawnedFunc1.h (new templates) | FABLE_RETAIL_SUSPENDABLE_PROCESS | one per instantiation (e.g. 0x012F55CC) | 7 / 7 | 0x3C / 0x40 | 239 |
| CGameScriptInterface.h (2 non-virtual members added) | FABLE_RETAIL_NSCRIPT | 0x01260F0C (unchanged) | n/a | 0x50 | 334 (CGSI-w4rev 54, -macros 51, -members 229) |

GRO rows: 134 (118 in the first pass, 16 after `gro_cp.py`). CGSIB rows: 144, all on the existing
CGameScriptInterfaceBase.h / CGameScriptInterface.h.

## CBaseObject (retail branch)

- **Chain.** RTTI `.?AVCBaseObject@@ : CBaseClass`.
- **Slots.** Slot 0 is the scalar deleting dtor 0x0099A4D0 (~CBaseObject is 0x0099A430). Slot 1 is `Validate() const`
  0x0099A400, whose body is `ret`. The dword after slot 1 is string data.
- **Spellings.** Ego_r.pdb and FableWin.pdb agree:
  - ctor, copy ctor and operator= are protected (`IAE`);
  - the dtor is protected virtual (`MAE`);
  - Validate is `UBE`, ClearIntelligentPointers is `QAE`, InternalValidate is `IBE`.
- **Bodies.** All in the CBaseClass.cpp TU:
  - ctor 0x0099A380 and copy ctor 0x0099A390 store the vtable and zero +4. The copy ctor does not copy the pointer.
  - operator= 0x0099A3B0 is `mov eax,ecx; ret 4`.
  - ClearIntelligentPointers 0x0099A3C0.
- **Member.** `IntelligentPointer` at +0x04: 0x0099A3C0 reads and zeroes it. Ego_r.pdb gives sizeof 8.
- **Unproven:**
  - InternalValidate has no retail body.
  - The class key of CBaseObjectPointer: no mangled name carries it.
- **Local by design.** The CBaseClass.cpp TU rows 0x0099A2D0-0x0099A4D0 (0x0099A380 and 0x0099A390 were tried and DIFFER:
  same-TU auto-inlining).

## CScriptGameResourceObjectBase (new)

- **Chain and slots.** RTTI `: CBaseObject`. Slot 0 is the scalar deleting dtor 0x006E70A0. Slot 1 is the inherited
  Validate 0x0099A400. No members of its own.
- **User-written dtor.** The dtor is `virtual ~X() { }`, written by hand and inline. Every expansion stores 0x0126008C
  before `jmp ~CBaseObject 0x0099A430`: in 0x006E70A0, 0x006E7B80 and 0x007E74D0.
  - VC7.1 drops that store from an implicit dtor. Tested: the implicit form emitted a bare `jmp 0x0099A430`, and the GRO
    family went from 83 to 97 SAME when the dtor became user-written.
- **Implicit members.**
  - ctor 0x006E7A60 (`call 0x0099A380; mov [esi],0x0126008C`);
  - copy ctor and operator=: the derived 0x00905330 / 0x008ABD10 call CBaseObject's members directly.
- **Spellings.** The PDB `??0 / ??4 / ??1 UAE / ??_G` spellings (Ego_r 0x00710370, FableWin 0x02467C20) are what the
  implicit members mangle to.

## CScriptGameResourceObjectScriptedThingBase (retail branch)

- **Chain.** RTTI `: CScriptGameResourceObjectBase`. 30 slots.
- **Slots.**
  - Slot 0 is the scalar deleting dtor 0x007E7520.
  - Slot 1 is `Validate() const` 0x007E72C0. It is protected (FableWin `MBEXXZ`).
  - Slots 2-29 are the 28 forwarders. Each is `mov ecx,[ecx+8]; test; je; jmp [eax+4*slot]`.
- **Slot spellings.** Every slot spelling is in FableWin.pdb, and slots 2-29 are also in Ego_r.pdb.
  - Slot order follows Ego_r's address order, except the Speak pair. VC7.1 lays out adjacent overloads in reverse
    declaration order, so `Speak(..., unsigned long, ...)` is declared first and slot 13 is the `const char*` overload.
  - vtprobe: 30 / 30.
- **Members.**
  - PImp is `CCountedPointer<CScriptGameResourceObjectScriptedThingBase>` at +0x08 / +0x0C. The slots read [ecx+8], and
    the dtor releases +0xC.
  - IsValid() const 0x00CD23B9 is `cmp [ecx+8],0; setne al` (FableWin `?IsValid@...@@QBE_NXZ`). Landed rows called it
    `IsScripting()`; it is respelled in every migrated row.
  - Release 0x00CD2770.
- **Implicit ctor and dtor.**
  - The COMDAT copies are 0x007E72A0 and 0x007E74D0. 0x007E74D0 has no store of 0x0127094C before the release (the
    implicit-dtor shape).
  - Over 151 rows, the implicit dtor gave 97 SAME and an out-of-line declaration gave 84.
  - Retail inlines at most sites and calls the copies at others. Rows that call them stay local: these are the
    REVERTED.tsv `GRO` / `GRO-cp` "mixed inlining" rows and the 50 out-of-line ctor rows in NEXT.md.
- **SetImplementation** is 0x008AB960 (both PDBs `QAEXPAV1@Z`). It is bound by landed row 0x00903810, and its body is
  `add ecx,8; jmp 0x008AB230` (PImp reset).
- **Unproven.** `EScriptEntityMoveType` is a forward enum: its enumerators are not recovered.
- **Contract.** The row defines `CCountedPointer<T>` before the include.

## CScriptGameResourceObjectMovieBase (new)

- **Chain and slots.** RTTI `: CScriptGameResourceObjectBase`. Slot 0 is the scalar deleting dtor 0x006E7BD0. Slot 1 is
  the inherited Validate. vtprobe: 2 / 2.
- **Member.** PImp at +0x08 / +0x0C: the ctor 0x006E7B60 zeroes both.
- **Release.** 0x00CF8850 is `add ecx,8; call 0x008AB2B0`.
- **Implicit ctor and dtor.** The COMDAT copies are 0x006E7B60 and 0x006E7B80.
  - Label fix: 0x006E7B80 is labelled ~CScriptGameResourceObjectBase but is `??1CScriptGameResourceObjectMovieBase@@UAE@XZ`
    (NAME_FIXES.tsv).
- **Unproven.** SetImplementation (both PDBs). The candidate is 0x008AB970 `add ecx,8; jmp 0x008AB2B0`, the twin of the
  STB one, which jumps to the reset Release uses. It is unbound, so the header does not claim the address.

## NScript::CIsThingConscious (new)

- **Shape.** The same shape as CIsThingAlive.h. No RTTI. The vtable 0x012C2FE8 is stored by Clone 0x00CDDAE0.
- **Slots.**
  - Slot 0 is `operator() const` 0x00CDDAB0. It is true when the Thing's slot 75 IsAlive is true and its slot 61
    IsUnconscious is false.
  - Slot 1 is Clone 0x00CDDAE0.
  - Slot 2 is the scalar deleting dtor 0x00CDDB10. ~ is 0x00EF7AB0.
- **Spellings.** Ego_r and FableWin agree on `??R`, Clone and `??1`. The ctor and copy ctor are FableWin-only, and retail
  expands them inline: a row that needs them defines them out of class after the include (00eea020 does).
- **Member.** Thing at +4. Size 0x10.
- **Wave 6.** The other 48 rows need the CScriptThing family first: their local CScriptThing models clash.

## CSpawnedFunc<T> / CSpawnedFunc1<T,P1> (new)

- **Chain.** `: CSpawnedFuncBase`. No RTTI.
- **Slots.** There are 7: slot 0 is the deleting dtor, slots 1-3 are CSuspendableProcessMicrothread's, slot 4 is
  `SuspendableProcess` (`(PClass->*Func)()` / `(Par1)`), and slots 5-6 are CSpawnedFuncBase's.
- **Layout.** Ego_r.pdb CSpawnedFunc<NScript::CQ_BountyHuntScript>, 0x3C: Func at +0x34, PClass at +0x38. CSpawnedFunc1
  adds Par1 at +0x3C.
- **Spellings.** The ctor (name, id, class, func) is in both PDBs.
- **The ctor is `__forceinline` under MSVC** (`FABLE_SPAWNEDFUNC_INLINE`; plain `inline` for g++).
  - Over 241 rows, plain inline reproduces 224 and forceinline 238.
  - The other 2 rows are CSpawnedFunc1 rows with a CScriptThing argument. They are reverted.
  - **Unproven:** whether retail declared it forced, or plain inline under another inlining heuristic.

## CGameScriptInterface (existing; two non-virtual members added)

- `protected: long GetMinWorldFrameForMessages() const; // 0x006E7510` and `long GetMaxWorldFrameForMessages() const;
  // 0x006E7530`.
  - Both PDBs list `?GetMinWorldFrameForMessages@CGameScriptInterface@@IBEJXZ` and then `...GetMax...@@IBEJXZ`, 0x30
    apart (Ego_r 0x00710770 / 0x007107A0, FableWin 0x024674E0 / 0x024676B0).
  - Retail has the same twin pair 0x30 apart: 0x006E7510 reads the +0x18 frame word, 0x006E7530 reads +0x1c.
  - Max is bound under `IBE` (protected) already.
  - **Unproven:** the Min name at 0x006E7510 is assigned by PDB order. Landed rows call it `GetCurrentWorldFrameForMessages`
    (`QBE`); that is a NAME_FIXES entry, and those rows stay local until the coordinator respells them.
  - The only landed row that includes the header and uses these names is 00893ec0: mcheck SAME on the new header.
- `GetTextIDFromSymbol` (0x006E77F0, 38 rows) is **not** added: it is in neither PDB and has no label.
