# Chunk 452 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Control flow and return values
- `if (a) { if (call()) return true; } else { if (call2()) return true; } return false;`, one early return per arm -> loads common to both arms are hoisted and the early returns use `mov al,1 / xor al,al`. A ternary of the two calls gives `setne` (0x00CBEB7E).
- Script-translator guard `if (IsActiveThreadTerminating()) return false; return true;` -> a byte-wide `neg al ; sbb al,al ; inc al`. `return !IsActiveThreadTerminating();` gives a full-width `sbb eax,eax` (0x00D9F070).
- Float `x == -1.0f` -> an integer compare of the float's bits against 0xBF800000, not an x87 compare (0x00DA0D90).

## Locals and temporaries
- Element-reference local `const CElement& element = data.Elements[i];` -> the strength-reduced `ebx + esi` element address in an index loop. Indexing on every use re-derives it (0x00CD4FF0, 0x00CD5090).
- Zero-reusing inline `GetLength() { return data == 0 ? 0 : data->length; }` -> the null register doubles as the zero, and the guard folds into two `je` (0x00CBF050, 0x00CBFACA).
- A separate `for` index starting at the found position -> retail's ebx/edi/esi assignment. Reusing the search-result variable as the index rotates the registers (0x00CBF050).
- An `&&` whose right operand builds a class temporary -> VC7.1 keeps a "constructed" flag word (`and [ebp-4],0` ... `mov [ebp-4],1` ... `test byte [ebp-4],1`) (0x00CBEED2).

## STLport 4.6 instantiations
- `_Rb_tree::_M_erase` with `#pragma inline_recursion(on)` and the full allocator chain (`_STLP_alloc_proxy` -> `allocator` -> `__malloc_alloc` -> free) -> exactly two inlined recursion levels. A direct free() inlines deeper (0x00CD2919, 0x00CD29F0, 0x00CD344A).
- Free STLport template helpers called out of line (`__uninitialized_copy`) -> they need an explicit `__fastcall` on the template, because the lane has no /Gr. Otherwise they compile cdecl (0x00D34F00).
- `__copy_trivial` written as STLport does it, `(last == first) ? result : (T*)memmove(result, first, n) + n` -> `cmp edx,ecx ; je`. A count-first `n ? ... : result` gives `sub ; je` (0x00D34F40).
- `__copy` called by `__copy_ptrs` -> needs `__forceinline` or it stays out of line (25 v 74) (0x00D34FD0).
- Retail calls an element's copy ctor out of line from push_back -> declare that copy ctor (no body). With the implicit one, VC inlines it into `_Construct` and then keeps `_Construct` out of line (70 v 67) (0x00D18C00).
- `uninitialized_fill_n(p, n, 0L)` over a 4-byte POD in a speed TU -> `rep stosd` (0x00D38F30).
- Fable's STLport node allocator is malloc 0xBFEA0E for list nodes too, not ::operator new 0xBFEA1A. The scratch grader masks the call target, so an `::operator new` model grades RELOCATION_MATCH. Check every call target against retail: `bin/calls.py` pairs each object relocation with the retail target at the same offset (0x00DA0200).
- Template function and member-template leaves are `??$name@`; a `?name@` leaf matches nothing and silently grades the largest function (0x00D34F40). Free functions need the `?Name@` prefix, not the bare name (0x00CDEB40).

## Quest-script classes
- Quest-script classes -> model `CLevelScriptBase<T>` as a `__declspec(novtable)` intermediate holding Game +0x40 and MasterScriptData +0x44 over a 0x40-byte CScriptBase whose ctor is out of line (0xCB8110). Retail offsets = Ego_r.pdb offsets − 0x14 for the CQ_* members (0x00CEF950, 0x00CFAD30, 0x00DC24E0).

Review at landing:
- 00cd344a needed all 4 grades (none needed more than 4).
- Local base models sit next to existing engine headers:
  - Several rows carry a local 12-byte CScriptThing (vtable 0x01238C8C, PImp Data/Info) next to engine/CScriptThing.h.
  - The local `NScript::CScriptBase { vptr; _body[0x3c]; }` / CQ_* layouts are hand-derived (PDB − 0x14).
  - Check them against the generated headers before retyping.
- Already-landed sources whose names look wrong:
  - CTCCarryable_OnKill_00cd2d8b / _00cd2ddc are `_Rb_tree::clear` bodies.
  - CActiveFile_OnReadFinished_00cd31d2 / _00cd3211 are tree destructors.
- Catalog rows propagated from a mislabelled base:
  - The CScriptBase ctor 0xCB8110 is labelled "CHeroMorphDef", so every inlined quest-script ctor or Alloc inherits a CHeroMorphDef_Create* name. The same applies to the generic "Alloc" rows.
  - Fix the root label rather than each row.
- 0xDA0200: the earlier grade used `::operator new`. The landed source must call malloc (0xBFEA0E), as src/ now does.
