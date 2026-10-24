# Chunk 327 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Const interface lookups copied into a call (`body->AddAndCopyTC(tc, true)`, or a component used only inside a later
  `if`): use the out-parameter form `const T* p; if (GetConstInterface(&p, type)) ...`. A plain `if (HasTC(t))
  f(GetConstTC(t))` rotates the component register (edx/ecx/eax). It also sinks the component load below the next test;
  retail always uses `mov eax,[eax+4]` before the branch. (0x00663F50)
- A `GetConstTC` written as `(it == end || type < it->type ? end : it)->component` loads the component straight into the
  consumer register (`mov ecx,[eax+4]`). The `it = map->last; return it->component` form adds a `mov ecx,eax`.
  (0x00663F50, CTCCreatureModeManager lookup)
- Arguments that retail reads before a virtual call inside the same argument list (`movsx edi,[esi+90h]` ahead of
  `call [edx+10h]`) are locals in the source, read in that order before the call. VC7.1 does not hoist field loads across
  a call. (0x00663F50, `CThingDeadCreature::Create(def, pos, player, name, mode->Vcall())`)
- `p->q->Virtual(arg)`: when retail keeps the object in a callee-saved register across the evaluation of `arg`, the object
  comes from an inline accessor (`GetPhysicsTC()`). A direct `p->PhysicsTC->` is re-loaded after the inner call.
  (0x00663F50)
- `PeekWorld()->Manager->AddMessage(...)` with PeekWorld called before the pushes and the manager loaded last needs a
  `CWorld* world = PeekWorld();` local. A field read ahead of PeekWorld (`physics->Position`) needs its own local declared
  first. (0x00663F50)
- `events->AddEvent(new CEvent(...))`: when retail fetches the TC before `new`, the TC is a named local first.
  (0x00663F50)
- Do not give GetSubDef / GetEffectFromString same-TU stand-ins when the def slot must stay separate from a later
  CCharString temporary. A stand-in shows VC that the slot is dead and the two share a frame word. (0x00663F50)
- STLport `vector<CCountedPointer<T> >::push_back` inline: give `_M_insert_overflow` a same-TU `__declspec(noinline)` body
  that only reads the value. The count block then stays in a register across the overflow call. The empty `__true_type`
  needs a user default ctor, or `__true_type()` stores a zero byte. (0x00549B20)
- One inline constructor can be expanded at some call sites and called out of line at others in the same function.
  CAction() and CCountedPointer<CAction>(CAction*) as plain in-class inline: VC7.1 called the first four uses
  (0x0042BE50 / 0x0042AA29) and expanded the last, matching retail. Do not force either way. (0x00549B20)
- A CActionParam appended to CAction::UIActionParams must use the real STLport `push_back -> insert(end()) ->
  _M_create_node` chain; a flattened append drops the `push ebp` (shrink-wrap). This confirms the landed 0x00549230 note.
  (0x00549B20)
- Bitfield flag persistence (`Transfer("X", flag, def)` into a bool:1): one block-scoped bool per flag. A single function-
  scope flag shares the default temporary's byte and moves the traits temporaries. (0x00676BE0, 0x00830FE0)
- In a test model, `T::GetDef()` returning a CDefPointer by value must copy from a never-destroyed object. A local plus
  `return p;` is NRVO'd by g++ but copied and destroyed by VC7.1 /Od, which releases the def twice.
- Generated engine headers CThingCreatureBase.h / CThingAICreature.h are PDB-layout flat pad structs (PDef at +0xfc
  v retail +0xe0). They cannot model retail offsets, so these rows use local retail-offset models.

Review at landing:
- 00663f50 (CThingCreatureBase::OnDie) took about 12 grades and 00549b20 (CTextSlider::Initialize) about 6, both more
  than the 4-attempt rule. Both are byte-exact, with tests passing on g++ and VC.
- 00663f50 and 008329b0 / 00830fe0 / 00832260 use local CThing / CThingCreatureBase / CThingAICreature models next to the
  existing engine headers. The headers are PDB-offset flat structs, unusable at retail offsets; consider regenerating them
  from retail evidence.
- 00549b20 defines a same-TU noinline `_M_insert_overflow` body (the 0x0054A2A0 symbol; the landed 0x00547500 idiom).
  CAction() / CCountedPointer<CAction>(CAction*) are emitted as COMDATs that stand for 0x0042BE50 / 0x0042AA29.
- Catalog name collisions: 0x006A15A0 and 0x006A1620 are both labelled IsThingCarryingMeleeWeapon; 0x006A1620 is
  IsThingCarryingRangedWeapon. 0x008E7E10 (Briar Rose mode) is landed under CIconText::HasEnoughIcons.
- 0x00830110 (CThingAICreature vtable slot 74) is `mov al,1 ; ret 8`. The slot's real name is unknown; I called it
  IsNewAttackerMoreImportant in differ/008329b0.cpp.
