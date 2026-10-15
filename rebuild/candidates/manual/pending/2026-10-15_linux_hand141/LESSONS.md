# Chunk 141 lessons (CGameScriptInterface)
- A component accessor that returns an UNSET local when the presence bit is clear (`CTC* tc; if (HasTC(k)) tc = GetTC(k); return tc;`) reproduces retail's dead-slot read (`mov eax,[esp+N]`) and the duplicated call on the absent path.
- Component lookup shape picks the register: `GetTC(t,k)->F()` = `mov ecx,[eax+4]`; out-parameter `GetTCOut(t,k,&p)` = `mov eax,[eax+4] ; push ; mov ecx,eax`; a named local used after other work = `mov esi,[eax+4]`.
- Retail's second `GetThing()` before the mask test marks an inline `IsThingAlive(thing)` helper (`t && !t->dead`) evaluated before a fresh GetThing; a single GetThing with the dead test means the null/dead test is inline in the body.
- A CCharString local shares the dead argument slot with the inlined lookup key only as a named local in its own `{}` block; when its destructor runs BEFORE a null test of the call result, close the block before the test (`CTCBase* added; { CCharString n(..); added = AddTC(n,0,0); } if (!added) return;`).
- A CScriptThing returned by value with an inline null result: `CScriptThing() {}` over an out-of-line `CBase::CBase` (0x0099A2D0) with a counted-pointer member whose inline constructor zeroes both words (stores after the vtable store); `return t->GetScriptThing();` forwards the hidden pointer.
- `__fastcall` factories with floats: the floats go on the stack and a trailing `bool` still takes dl (`xor dl,dl` before the call).
- A static getter used as `Get().Method(arg)` pushes `arg` before calling the getter (no named local needed).
