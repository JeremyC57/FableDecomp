# Chunk 171 lessons (for GOTCHAS)
- A def pointer that retail loads ONCE from the escaped out slot and keeps in a callee-saved register for the reads and the release (`dec [edi+4] ; jne`) is a holder copy: `CDefPointer out; Get(&out); CDefReference def(out.Object);` with the release in the holder's dtor (fixes the release in 007cf7e0, 007b8680 and 005da6e0, which still park on other diffs).
- IS_THING_ALIVE<Base*>(Base* const&) called with a Derived* binds through a converted temporary in the frame, so the thing stays in esi (no reload after the call); a const-qualification conversion does not give that.
- A GetTC that retail writes as `found = last ... cmp [eax],edi ; jg` with the result in the map register is an inline map member `find(const K* key, K value)` returning `last` on a miss; a `const K&` key is re-read from memory, an open-coded select uses ecx.
- In a /GS SendPauseEvent the flag normalised in the bool argument's own slot (`test ; setne ; mov [arg],cl ; lea [arg]`) is an inline AddBool(bool&) that normalises its referent; every by-value helper form gets its own frame slot.
- `T* p = 0;` before the call whose result is tested places the preloaded `xor ebx,ebx` before that call, not after the test.
- An inline accessor (`GetHome() { return PHome.GetPItem(); }`) turns `mov ecx,eax ; add ecx,N` into `lea ecx,[eax+N]` for a member intelligent pointer of a call result.
- A named def-class local (`CDefClassBase* c = ParentThing->DefHolder; CDefPointer d; c->GetSubDef(&d);`) moves the out slot's zero store after the push (retail placement); the inline accessor form puts it before.
- VC7.1 in this lane has no native wchar_t: declare L"" parameters as `const unsigned short*`.
