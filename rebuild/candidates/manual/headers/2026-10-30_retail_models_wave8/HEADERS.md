# Wave 8 headers: what changed and the evidence

Two new NScript headers (CActiveEntityScriptBase, CActiveEntityScript<T>), one member type fixed in
CEntityScriptBinding<T>, and three comment-only additions (CTimer, CScriptGameResourceObjectMovieBase, the
CActiveEntityScript count). No layout or slot of an existing header changed.

| header | change | includers checked |
|---|---|---|
| engine/CActiveEntityScriptBase.h | **new**: `class CActiveEntityScriptBase : public CSpawnedFuncBase` (0x58); opt-in FABLE_RETAIL_NSCRIPT + FABLE_RETAIL_SUSPENDABLE_PROCESS | 205 rows (AES 120 + CAES 85) mcheck SAME / leafsym; regraded PASS |
| engine/CActiveEntityScript.h | **new**: `template <class T> class CActiveEntityScript : public CActiveEntityScriptBase` (0x60) | as above |
| engine/CEntityScriptBinding.h | `Func`: `void (T::*)()` -> `void (T::*)(CScriptThing)` | all 6 HEAD includers `mcheck --all` SAME (logs/mc_hdr_binding.txt); 85 CAES rows SAME |
| engine/CTimer.h | comment: the TIMER family rule (inline bodies after the include; SetTime / Set are operator=) | comment only |
| engine/CScriptGameResourceObjectMovieBase.h | comment: the per-site expansion finding (below) | comment only |

Both new headers pass `g++ -m32 -w -std=c++98 -fsyntax-only` and a VC7.1 test TU (work/hdrtest_aes.cpp). That TU explicitly instantiates
`CActiveEntityScript<NScript::CQ_Test>::SuspendableProcess` and checks `sizeof == 0x60`; the 85 CAES rows exercise both ctors.

## CActiveEntityScriptBase

- **Retail vtable.** 0x012C3594 has no RTTI (vt[-1] == 0). The ctor 0x00CE1110 stores it after the inlined
  CSpawnedFuncBase ctor (vtable 0x012C2F9C). There are 7 slots:
  - 0: scalar deleting dtor 0x00CE1200.
  - 1-3: the microthread's slots, 0x00A44880 / 0x00A44840 / 0x00A447D0.
  - 4: SuspendableProcess 0x00A44600, not overridden here.
  - 5: OnPredicateFail 0x00CE1090. It forwards to the entity script's slot 5:
    `mov ecx,[ecx+34h]; test; je; mov eax,[ecx]; jmp [eax+14h]`.
  - 6: OnInterrupted 0x00CE10A0.
- **PDB names.** Ego_r.pdb has:
  - `?OnPredicateFail@CActiveEntityScriptBase@@UAEXXZ` and `?OnInterrupted@..@@UAEXXZ`, in the same 5-before-6 order as
    CSpawnedFuncBase.h;
  - `??1CActiveEntityScriptBase@@UAE@XZ`;
  - the ctor `??0CActiveEntityScriptBase@@QAE@ABVCScriptThing@@_KK@Z`. Retail 0x00CE1110 is already bound to it in
    reloc_bindings.tsv.

  FableWin.pdb adds GetFlags, GetEntity and GetPEntityScript.
- **Layout.** Ego_r.pdb gives 0x58, and the retail ctor writes the same offsets:
  - PEntityScript +0x34: a CCountedPointer<CEntityScriptBase>, zeroed.
  - Flags +0x3C: the 4th argument.
  - EntityUID +0x40: an unsigned __int64 made from the 2nd and 3rd argument dwords.
  - Entity +0x48: the inlined CScriptThing copy (vtable 0x01238C8C, PImp addref).
- **The dtor is implicit.** 0x00CE1200 does three things, with no own vtable store first (the wave 5 rule):
  1. destroys Entity (0x004AA840);
  2. releases PEntityScript (0x00CE1000);
  3. tail-calls ~CSpawnedFuncBase 0x00CDD4C0.
- **Size assert.** `sizeof == 0x58` is guarded by `_MSC_VER`. VC7.1 aligns the u64 to 8; g++ -m32 aligns it to 4 and gives 0x54.

## CActiveEntityScript<T>: the SuspendableProcess T recovery (120 of 120)

- **The vtables.** Every retail `CActiveEntityScript<T>::SuspendableProcess` body is slot 4 of an RTTI-less 7-slot vtable:
  - slots 1-3 are the microthread's;
  - slots 5-6 are 0x00CE1090 / 0x00CE10A0;
  - only one function stores the vtable: CEntityScriptBinding<T>::CreateActiveEntityScript, which expands the
    CActiveEntityScript<T> ctor inline.
- **How T was found.** `scripts/aes_T.py` follows each vtable to its binding, then follows the binding's bound or landed
  name to T (work/aes_T.tsv).
- **PDB check.** All 120 T agree with the FableWin.pdb
  `?SuspendableProcess@?$CActiveEntityScript@V<T>@NScript@@@@UAEXXZ` list and with Ego_r's.
- **What the rows got.** The 120 rows (family AES) now define:
  ```cpp
  template <class T> void CActiveEntityScript<T>::SuspendableProcess()
  { if (PEntityScript.Data) PEntityScript.Data->Main(); else (PParentScript->*Func)(Entity); StopRunning = true; }
  ```
  plus the explicit instantiation for their T. Their leaf symbol is now the PDB-exact mangled name.
- **Members.** Func is `void (T::*)(CScriptThing)` at +0x58, a 4-byte single-inheritance member pointer. The FableWin ctor
  spells it `P823@AEXV1@@Z`, and retail calls it as `mov ecx,[esi+5Ch]; call [esi+58h]` with the Entity copy by value.
  Rows declare `class __single_inheritance T;` when T is incomplete. PParentScript is at +0x5C.
- **The ctors.** Both ctors (Func, or an already allocated script) are inline (`__forceinline` under VC7.1). Retail expands
  them inside every CreateActiveEntityScript, for example 0x00D7FAB0 with vtable 0x012D3FE0. The dtor is implicit:
  0x00D7FC10 has no own vtable store.

## CEntityScriptBinding<T>::Func

- Ego_r.pdb types the member `pmf_b463565b`. That is the same type as CActiveEntityScript<T>::Func, which FableWin.pdb
  spells `P823@AEXV1@@Z`, i.e. `void (T::*)(CScriptThing)`. The old `void (T::*)()` had no evidence.
- The binding copies its Func straight into the active script: 0x00D7FAB0's Func path does `mov ebx,[esi+0Ch]` and then, after the base ctor and the vtable store, `mov [edi+58h],ebx`.

## MovieBase (time-boxed finding, no header change beyond the comment)

- **Three destruction forms.** `scripts/mbsites.py` classifies every landed row that touches the MovieBase dtor in retail
  (120 rows). Retail destroys a MovieBase in one of three ways, often mixed inside one function:
  - INL: fully expanded;
  - AB0: expanded, with a call to the PImp CCountedPointer dtor 0x006E7AB0;
  - B80: a call to the ~MovieBase COMDAT 0x006E7B80.
- **Patterns seen.** The most common are AB0 alone (32), INL alone (25), B80 alone (11) and `INL B80 B80` (8), plus 44
  rows with other mixes.
- **What VC7.1 does by itself.** Take the 24 rows that force their per-site choice with `#pragma inline_depth` scaffolding
  and give them two things:
  - the implicit MovieBase dtor, as the header says;
  - a plain `inline` (not `__forceinline`) CCountedPointer release.

  VC7.1 then gives the identical leaf bytes in 15 of the 24 (`scripts/mbexp.py`, logs/mbexp_prag.txt). Retail's mix is
  VC7.1's own per-site inliner decision, not a different model. 14 of those rows were edited this way (family MBX); one
  changed a callee (B80 became ~CBaseObject) and was reverted.
- **unproven.** Single-site rows that call 0x006E7AB0, such as 0x00DF2D30 (349 bytes, one MovieBase), still expand fully
  under VC7.1 (385 v 349) with the real inline release. Two things did not change that:
  - the same release placed in a function compiled before Main in the TU;
  - the same release placed in a function compiled after Main.

  So no per-TU budget explains it. Ego_r's out-of-line `??1CScriptGameResourceObjectMovieBase@@UAE@XZ` (0x007E2EA0) cannot
  be disassembled here: the Ego_r binary is not on disk.
