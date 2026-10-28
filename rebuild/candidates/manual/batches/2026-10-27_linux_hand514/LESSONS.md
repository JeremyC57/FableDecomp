# Chunk 514 lessons (GOTCHAS one-liners)

- (2026-10-28) Chunk 514 (40 re-queued rows, 33/40 matched incl. 1 retail_asm): most re-queued labels were propagated. Real names came from Ego_r donor order and sizes (a 97-byte vs 51-byte pair settles which of two neighbours is which), RTTI at vtable-4, and single callers.
- A deleting destructor with one `test [esp+8],1` is `??_G` (scalar), never `vector_deleting_destructor`. Its identity is the vtable it sits in (slot 0), read through the RTTI locator at vtable-4.
- VC7.1 resets the vptr only in user-written destructors. A def's `??_G` whose only vptr store is CDefPointeeBase's needs implicit derived destructors, but VC7.1 then calls `??1` instead of inlining it, even with the qualified-destructor device, /O1 or /Ob2 (0x00625160, 0x0067C520: parked).
- A stand-in GetDef/GetSubDef body under `auto_inline(off)` that only writes the out pointer, plus a by-value CDefPointer return, gives retail's merged add-ref/release (`inc ; mov ; dec ; mov ; mov`). The def index read before the def local is declared fixes the register choice (0x00663340).
- In the GUI /O1 TU, STLport and helper inlines need `__forceinline`. A named local object (`CIntelligentPointer<T> none; v.resize(4, none);`) gives retail's `lea eax,[ebp-8] ; push eax` where a temporary pushes the ctor's returned eax (0x0064DD4D, 0x0064A2DB).
- A by-reference `const CPlayerGuiDef& def = PeekGuiDef();` then `def.Map.find()` gives `lea esi,[eax+off]`; a reference to the member itself gives `mov esi,eax ; add esi,off` (0x0064A37B).
- A colour copy whose four byte loads all come before the four stores is `CRGBColour(const CRGBColour& c) { Set(c.R, c.G, c.B, c.A); }` with a forceinline Set; a member-wise copy interleaves them (0x00654F80, 0x0065564B).
- An out-of-line STLport `find_if` with a by-value predicate: hidden result in ecx, the tag `&[ebp+0Fh]` in edx (it shares the `last` slot), and the callee destroys the predicate. A same-TU noinline `__find_if` body reproduces it on the first grade (0x0065FFCA).
- `x = a->b.Get()->f()` through a CCountedPointer temporary: `operator->` (not `.Data->`) gives retail's `mov eax,[eax] ; push ; mov ecx,eax` order (0x006F90D0).
- The inline-asm ROUND helper (`fstp [param] ; fld [param] ; fistp [local]`) in a /O1 GUI row is retail_asm. Compute the rounded long into a local before the other struct fields to get retail's fistp-then-ftol2 order (0x006472B2).
- `1.0f / def->x` compiles to `fld 1.0 ; fdiv [x]`; retail's `fld [x] ; fdivr 1.0` form was not reached with a named helper or a CDefPointer operator-> (0x006D63A0, parked).

# Review at landing
See NOTES.md "Review at landing". Two items need a rename or a re-bind:
- Landed 0x00644526 is OnEventUnsheatheWeapon, but it carries ?OnEventAttack@CDrawTargetInfo@NPlayerGui@@QAEXXZ, which this batch gives to 0x00644587.
- Invented landed spellings are reused by this batch's callees: 0x00655EB4 (push_front), 0x013BA89C/0x006870C0 (RegisteredTCs), 0x0042B467 (_M_find), 0x00656D20, 0x004433F0, 0x004A9730, 0x004C7990, 0x00438570, 0x005937E1, 0x006BCC10, 0x006B2660, 0x013B8394. Callers that bind invented names: 0x00710B80, 0x00744D00, the callers of 0x0070C4E0, and 0x007363A0 for 0x00663340.
