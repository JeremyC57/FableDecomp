# Chunk 708 lessons (GOTCHAS one-liners)

- VC7.1 lays out overloaded virtuals in **reverse declaration order**: declare `GetRandomNavigablePos(bool...)` before the C2DVector overload to get CNavigatorAStar slot 6 = C2DVector (0x00A47180, test-compiled).
- A plain private member defined above its caller in the same TU keeps its out-of-line copy and is expanded inline in each caller; this gives the 0x009FEB80 / 0x009FECE0 / 0x009FEBC0 trio.
- `~Derived() { }` with an empty body is what stores the vtable. A class with no user dtor gets a compiler-generated one that drops the store (CClass ??_G 0x00A379E0 has no CClass store). The retail headers need a declared `virtual ~X();` for the 0x00A66990 / 0x00A66F80 / 0x00A47180 shapes.
- Reading the same member pointer for two calls (`PCreature->A(); PCreature->B()->C()`) reloads it. A named local (`CThingAICreature* creature = ...`) keeps it in edi (0x008D6690: 72v70 -> match).
- When a destructor deletes two pointers and nulls them, the null stores come after both deletes (0x00A47180). Nulling each pointer right after its delete moves the first store.
- `delete p` on a polymorphic pointer whose dtor is inline (CIndexBuffer `{ Uninitialise(); }`) becomes `test; je; push 1; call [vtbl]`. The repo header (no virtual dtor) gives 73v74 (0x00BE2B20).
- The NUISystem::CComponent primary vtable reaches RemoveChild at slot 64 (+0x100). Retail RTTI puts CObserver at +4 and CActionParam at +0x18. With STLport set at 12 bytes, CObserver is 0x14.
- vland's SHARED_HEADER_AVAILABLE warning is a substring test. A layout-rename include (`#define X XFields` / include / `#undef X`) plus a one-line `FABLE_STATIC_ASSERT(sizeof(X) == sizeof(XFields))` satisfies it with no byte change.
- Proving an overlay header is in use: a repo header with no user dtor gives C2600 when `X::~X() {}` is defined. That is a safe use-check, unlike `#error`.

## Review at landing
- GetPItem spelling: PDB `QBEPAVCBaseObject@@XZ` vs the brief house style `QBEPAVCThing@@XZ` (NOTES.md).
- hdr/ overlays: six opt-in branches. Merge them with any parallel CClass.h edit from 706 or 707.
- The CComponent v01..v63 virtual placeholders in 0x005567B0 are descriptive; no symbols are emitted.
