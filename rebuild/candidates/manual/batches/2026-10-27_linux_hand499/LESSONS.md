# Chunk 499 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Entity script vtable (RTTI-less, 7 slots): 0 dtor, 1 Main, 2 Init, 3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted. Name rows from the slot that the landed Alloc's stored vtable puts them in.
- Entity layout: vptr; Game at +4; Me (12-byte CScriptThing) at +8; ParentClass at +0x14; PMasterData at +0x18; members from +0x1c. Ego_r offsets equal retail.
- Level script vtable: 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist. Retail CScriptBase is 0x40, so level members sit at Ego_r offset - 0x14.
- STLport vector<bool> in retail is a plain byte vector of 0xC (TransferVector<bool> at 0x00CDCF80; operator= at 0x00462E78).
- Reuse landed models: 33 of 36 rows matched on the first grade.
- A 9-byte `xor eax,eax; cmp [ecx+8],eax; setne al` body needs `#pragma optimize("s", on)`. The default speed TU emits `mov edx,[ecx+8]; test edx,edx`.
- An enum declared inside `namespace NScript` mangles with `@NScript@@`. Declare it at global scope when the retail Transfer<> symbol has no namespace.
- Transfer(name, v, default) temporaries live in the pushed-ecx slot or the dead argument slot. Let the compiler place them; do not hand-order locals.
- A Copy body that inlines an implicit operator= matches. An implicit copy ctor used in Clone stores only the derived vtable.
- CBaseList::AddAfterI: read `pAfter->Next()` after SetData, as in the landed AddBeforeI. An early read keeps pBefore in ebx (+3 bytes).
- Test on g++: `long` is 8 bytes, so typedef HRESULT as int. Otherwise E_FAIL compares as positive.
- Oracle rows that pack several int3-separated functions are SKIP (4 in this chunk). They need re-splitting, not authoring.

Review at landing:
- 00cd23b9 and 00cf8850 are Probable names. Confirm IsValid and Release against a second source before applying labels.
- Apply the label corrections in NOTES.md, especially the propagated C3DMeshInfo::HasPhysicsMesh and the invented IsScripting on 00cd23b9.
- The header debt for 00d154a0 and 00d1fce0 (local CGameScriptInterface model) goes away once the shared header models the vptr.
- Re-split the four SKIP rows (00caf430, 00cafc40, 00cdf120, 00ce1090) into per-function rows.
