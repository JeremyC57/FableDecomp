# agent283 lessons (GOTCHAS candidates, one line each)

- If a callee takes a local's address (an out-param or `const&`), write a same-TU stand-in for it (`#pragma auto_inline(off)` + `__declspec(noinline)` body that only writes the out-param). This fixes frame-word sharing and register allocation; it closed 008c64f0 (GetWeaponHand / GetSubDef / GetDef) and 00846100 (list::remove).
- q.py strips trailing 0x90/0xCC, so a function that ends in `jmp rel8` = `eb 90` grades DIFFER(n v n-1). Check it with `V.parity_of` on the full row bytes before trying more variants.
- Inline `CCharString::operator==(const char*)` (strcmp(text,"") when the data is null, else Compare 0x411570) needs `__forceinline` when it is used more than once; otherwise only the first use inlines. The same applies to the counted-pointer operator=.
- When retail's inline counted-pointer assign loads the source's Info and Data first (esi/edi) before comparing with the old Info, write it with locals: `info = other.Info; data = other.Data; if (Info != info) ...`.
- Write the CScriptThing inline accessors with the null arm first: `PThing == 0 ? g_ZeroVector : *PThing->GetPosition()`, `PThing == 0 ? 0.0f : PThing->GetAngleXY()`.
- The CRGBColour ctor stores R,G,B,A in that order (memory layout is B,G,R,A).
- A named `CThing* x = GetCreature();` before a def lookup makes GetPItem run before the `&def` push.
- A switch factory doing `new T(...)` per case needs /EHsc. The EH state numbers give the source case order (00c1efc0).
- A `ret 8` ctor fed by `push *src->V1(); call src->V2(); push eax` is `T(src->V2(), *src->V1())`. Arguments are evaluated right to left, and V2 takes no argument.
- Default `const CCharString&` arguments are built right to left (the last parameter's temp first), and the copies into members follow member order.
- A user-defined `C3DVector(const C3DVector&)` member-wise copy ctor changes the order of the indexed-element copy loads, to match retail's `mov edx,[ecx+eax*4]` first.
- When a free call's arguments include `GetPWorld()->vcall()` and `GetPWorld()->field`, but retail evaluates both GetPWorld calls before the right-hand arguments, use named locals (`CWorld* world = GetPWorld(); tools = GetPWorld()->SearchTools;`).
- A first `push_back` inlined and a second one called out of line (0x76B120) in the same function: write the first as a `__forceinline` STLport body (placement new plus `_M_insert_overflow` 0x76ADA0 with `(pos, x, __false_type(), 1, true)`).
- A STL-using behaviour test can TCC_FAIL under the VC lane. Keep tests to plain arrays plus `<cstring>`/`<cstdio>`.
