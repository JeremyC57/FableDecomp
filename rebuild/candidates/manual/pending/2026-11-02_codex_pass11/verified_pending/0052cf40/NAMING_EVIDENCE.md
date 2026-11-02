# 0052cf40 — identity, extent and headers

Exact public virtual member `?ChangeState@CChangingStateComponent@NUISystem@@UAEXK@Z`: original Ego_r/FableWin name sets and class-V CChangingStateComponent TPI prove the namespace, receiver, unsigned-long parameter and access. Retail RTTI CSprite0122f5d4 slot48 and derived ChangeState callers independently bind the function. `objdump -t` emitted this exact unique leaf in both attempts.

Extent0052cf40..0052d390: code ends0052d365, first jump table0052d368..0052d374, byte map0052d374..0052d37b, second jump table0052d37c..0052d390. These are this function's own tables, explicitly allowed by the pass11 policy; no second function is included.

PDB module ui_changing_state_component.cpp201–308 supplies `state`, `ptarget` and four separate `count` scopes. Full class/member/enum layout and vtable provenance: evidence/math_identity/UI.md,UI_PDB.json,UI_DEPENDENCIES_PDB.json,UI_CONTAINER_PDB.json,UI_VTABLES.json,UI_LAYOUTS.json,UI_SOURCE_PROVENANCE.json. Complete CChangingStateComponent0x15c uses actual CComponent0x134 and nested EChangingStateComponentState/list at+13c. Complete CUIState0x24 is in its own real header; native uses StateChangeType+4 and UpdateTime+1c. Original enum names/values are used, with the simultaneous bitwise combination3 explicitly cast because no original enum member names3.

Virtual methods are the exact original GetState(unsigned long) slot135,HasState slot44,OnActivate140,OnDeactivate141,GetParent52,GetType65 and ChangeState(unsigned long,float)47. GetParent/GetType have const receivers; the parent hooks are protected. No placeholder vtable access is used.

The only named direct callee beyond CRT is0052d740:
`?push_back@?$list@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@V?$allocator@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@@std@@@std@@QAEXABW4EChangingStateComponentState@CChangingStateComponent@NUISystem@@@Z`.
The actual STLport4.6.2 `_list.h` has the const-reference push_back and insert implementation; raw PDB list specialization/enum and native45B malloc12/link/ret4 shape bind it independently. No existing reloc_bindings entry for0052d740 was found, and no conflicting name was imported. The emitted symbol is checked in the attempt's symbol table. The emitted code does not use the donor's later rvalue overload.

`_malloc` binds00bfea0e and `_free` binds00bfea14, both genuine CRT thunks. The only absolute external operand is compiler literal0.0f at0122dedc, not a named game global. All other absolute operands are the original local switch tables/labels. Independent21relocation audit: evidence/math_identity/UI_CHANGE_STATE_RELOCATION_REVIEW.json.
