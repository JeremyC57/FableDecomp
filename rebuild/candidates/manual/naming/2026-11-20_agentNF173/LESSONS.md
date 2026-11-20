# NF173 lessons (one line each)
- Probe binders first and record what the self rows define (selfsym): 0x00A76320, 0x00C2DA00 and 0x009F1F00 turned out to be stand-ins / placeholders at addresses whose catalog names were already right.
- An `add ecx,N ; jmp` forwarder whose target is a static __fastcall taking a reference is a member wrapper passing `&this->member`, not a getter (0x00644644 family to 0x006445BA ClearEffects).
- Interface ids name the class: GetInterface(&p, 0x34) is TCI_PERCEIVE_THING, so 0x007446A0 was CTCPerceiveThing::GetPerceivedThing, not a CTCCreatureOpinionOfHero getter (check engine/ETCInterfaceType.h before trusting a local model's class).
- A bool callee parameter declared `long` in a local model is a hidden widening: the caller's own bool parameter only matched once the callee took bool too (0x00AEB6D0: 193v192 -> RELOCATION_MATCH).
- `return f() ? true : false;` reproduces retail's `test al,al ; setne al` after a bool call (0x00AE0050); the plain return drops it.
- Adding `virtual` to a local model whose base carries an explicit `__vftable` word shifts the layout; replace the word with a virtual in the base instead (0x00753C70).
- q.py cannot grade an EH funclet (it grades the largest function); stage it in a vland payload with funclet_offset 0 and leaf Unwind@<addr>, including the new parent by absolute path.
- Standalone behaviour tests that embed their own model do not link the row, so a callee respell does not break them; sync them anyway when they copy the row's declarations.
