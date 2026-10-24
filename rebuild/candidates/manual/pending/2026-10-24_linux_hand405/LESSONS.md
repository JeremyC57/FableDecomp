# Chunk 405 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU template algorithm bodies:** a same-TU template algorithm BODY (STLport `__find_if`) tells VC7.1 the call
  writes no memory. The caller then caches `end()` across it (CTCContainer::RemoveItem 0x008ED040). With a declaration only,
  `end()` is re-read.
- **STLport 4.6 list iterators:** the iterator shape `_List_iterator_base` (`_M_incr`, `==`/`!=`) under `_List_iterator`
  with a user copy ctor keeps the ebp/edi pushes above the empty-range test of `transform` into a back_insert_iterator
  (0x008FCA40). A plain node-pointer iterator sinks them (191v193).
- **CBaseClass is polymorphic:** model it with a virtual dtor, so its vptr is at +0. An empty non-polymorphic base under a
  polymorphic derived class is placed at +4, which gives `lea ecx,[obj+4]` for the base ctor call (CreateEffect
  0x008F7E70 / 0x008F7F50). The landed 0x007C25A0 model has this error.
- **Implicit copy ctor out of line:** a compiler-generated copy ctor is emitted out of line when a member type (C3DVector)
  declares a user copy ctor and `Clone()` does `new T(*this)` (DragonSwoop 0x008F57F0). Its array members are copied by a
  3-iteration element-ctor loop.
- **Named-result operators:** a named-result `operator-` (`C3DVector r; r.X = ...; return r;`) stores X first. The
  `return C3DVector(...)` form computes Z first (0x008F5B50, 0x008E8FF0).
- **Two-dimensional dots:** build the vector in a dot product with sequenced member statements (`v.X = ...; v.Y = ...;`).
  An inline `operator/` returning a temporary flips that vector's dots to X-term first, where retail is Y-term first
  (CRadialArea::IsPosInArea 0x008EA3F0, the load-creation rule).
- **Shared false exit:** `if (!check || x < limit) { ...; if (a && b) return true; } return false;` shares retail's single
  false exit. `if (check && !(x < limit)) return false;` gets its own `xor al,al ; add esp ; ret` block (0x008EA3F0).
- **`__forceinline` filter operators:** in a `CFilter_BindN::operator()` instance, a filter `operator()` holding a call
  plus an x87 compare must be `__forceinline`. A plain in-class inline stays an out-of-line call (0x008E6030, as in the
  landed Bind4 0x0085B160).
- **`push_back` of a hoisted local:** a loop of `push_back(entry)` over one local built before the loop, together with the
  same-TU noinline `_M_insert_overflow` stand-in (reads x, never writes it), gives retail's `mov [eax],ebx`. The zero
  first word is forwarded from a register and the bool+padding dword is reloaded (CMap::InitThemePalette 0x008F71A0).
  STLport `clear()` is `erase(begin(), end())` through `__copy_ptrs(__false_type)`, so retail keeps the unfolded
  `mov ecx,eax ; sub ecx,eax ; sar ecx,3` copy loop.
- **No NRVO:** VC7.1 has no NRVO. `T handle; handle.Set(new Impl(x)); return handle;` builds the local, calls the out-of-line
  setter, copy-constructs the result (base copy ctor call, vtable, AddRef) and destroys the local inline
  (CreateNullHandle 0x00903810). The CCountedPointer dtor is `if (Info) { if (--RefCount == 0) {...} } Data = 0; Info = 0;`.
- **Dead constructor vptr store:** the intermediate base vptr store of an inline base ctor is dropped. The one in a base's
  user (inline, virtual) dtor stays, as `mov [esp+..],0x0126008C` before `CBaseObject::~CBaseObject` (0x00903810).
- **`2*pi` pool constant:** the double 2pi pool constant 0x0124F2B8 is `angle * (double)TWO_PI` with
  `static const float TWO_PI = 6.2831855f;`. Each `fsin` / `fcos` pair over one angle needs explicit `v.X = (float)sin(a);
  v.Y = (float)cos(a);` statements (0x008EA3F0).

Review at landing:
- **0x008F5BA0** used all 4 grades and matched on grades 3 and 4. No row needed more than 4.
- **Local models next to engine headers:**
  - CCreatureAction_DragonSwoop.h carries PDB offsets (0x150 vs retail 0x138).
  - CScriptGameResourceObjectScriptedThingBase.h / CScriptGameResourceObjectScriptedThing.h are flat pad structs with no
    base classes.
  - The CMap model (+0x1c DefinitionManager, palette +0x1a6c) is local because Ego_r's palette is at +0x1a78.
  - The `_M_insert_overflow` stand-in bodies in 0x008ED3B0 / 0x008ED440 / 0x008F71A0 are same-TU stand-ins: only their
    call matters.
- **Errors in already-landed sources:**
  - The landed 0x008ED010 "CTCBuyableHouse::MovedInNotification/ContainsThing" is really CTCContainer::ContainsItem(long)
    const over `pair<long, CCharString>` entries. It models them as {CThing*, long}.
  - The landed 0x007C25A0 InitAttachedEffects models CBaseClass as non-polymorphic.
  - The landed CMap ctor 0x00820CC0 names the 0x008F71A0 callee "InitialisePalette". The PDB name is `InitThemePalette`.
- **Catalog row holding two functions:** the 0x008ECF60 row contains the unreferenced STLport find_if wrapper at
  0x008ECFD0.
- **Renamed predicate:** the functor of 0x008ECEF0 / 0x008ED040 was renamed CIsItemWithDefIndex (PDB) after grading. Both
  files regraded MATCH / RELOCATION_MATCH in the final gall.sh run.
