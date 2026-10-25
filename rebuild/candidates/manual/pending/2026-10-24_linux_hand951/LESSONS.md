# Chunk 951 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Identity

- **Retail has no ICF.** Byte-identical template bodies are separate COMDATs, so each address is exactly one instantiation.
  - Never stamp a sibling's name. Two family members never share a T.
  - When only one T is left for a body, uniqueness against the other families settles it: G070's seven Ts versus G015's nine.
  - 604 rows in 73 families all followed this rule. In G001 (0x00440F00) the binary keeps 12 identical copies.
- **CThingSearchTools masks.** The thing-type mask tested by the inlined `CThingSearchTools` / `GetAllThingsInRoughArea` caller identifies `CThingCompare_Nearest::operator()<T>` (G015 0x0048B220, G070 0x0048B160):

  | T | Mask |
  |---|---|
  | CThingPhysical | 0x3 |
  | CThingGameObject | 0x7 |
  | CThingCreatureBase | 0xF |
  | CThingAICreature | 0x1F |
  | CThingDeadCreature | 0x87 |
  | CThingVillage | 0x407 |
  | CThingObject | 0x807 |
  | CThingMarker | 0x8007 |
  | CThingTrackNode | 0x10007 |

  CThing itself is folded (no mask test).
- **Element types from the caller chain.**
  - `gen/chain.py` caller trees, plus the vtable each `TransferVectorOfSubComponents` caller stores, give the element types of def sub-component vectors (G067 0x00476672).
  - Unreferenced list ctor / dtor COMDAT pairs give the node size: `malloc(8 + sizeof(T))` (G071, G073).
- **Retail STLport container sizes differ from Ego_r** (Dinkumware). Use these to map PDB member offsets to retail:

  | Container | Retail | Ego_r |
  |---|---|---|
  | list | 4 | 12 |
  | vector | 12 | 16 |
  | map / set | 12 | 16 |
  | deque | 40 | 24 |

  - Retail parent defs sit at Ego_r - 4, and each vector before a member takes 4 more (G057 0x00452CDD).
  - NScript quest-script members sit at Ego_r - 0x14 (G050 0x00D00A90).
  - Retail CCreatureActionBase is 0xA8 bytes (Ego_r 0xB4).
- **NScript script classes have no RTTI.** Identify them from vtable slot neighbours and from Ego_r member names matched to the `Transfer` string literals; OnPersist is slot 4 (G050).
- **CPredicate_And instantiations** come from three things (G051 0x00E23F00):
  - the Clone allocation size;
  - the sub-predicate vtables: CIsThingAlive 0x012C32F0, CIsThingConscious 0x012C2FE8, CIsVariableFalse 0x012EA43C;
  - the Predicate2 offset.
- **Struct spellings in the PDB name sets.** These are structs (U), not classes (V): CScreenEffectPriorityStack::CStackEntry, CRadialBlurEntry, CDisplacementMonochromeEffectColourEntry and CScreenFilterEntry (G073 0x00882B80 / 0x00882C00).
  - A V/U-wrong leaf names no symbol, and grading falls back to the largest function.
- **CBaseClass destructors.**
  - 0x0099A2E0 is `??1CBaseClass@@UAE@XZ`: CBaseClass's own deleting destructor 0x004371F0 calls it, and so do all 40 directly derived classes.
  - 0x0099A300 is `??1CBaseClassNonCopyable@@MAE@XZ` (protected).
  - 0x0099A2F0 is `??0CBaseClassNonCopyable@@IAE@XZ`.
- **Index lookups.**
  - `xr.fstart` labels are often wrong; use `gen/site.py` to find the function that contains a call site.
  - The PDB name-set TSVs are keyed by donor address, so use them only as name sets.

## Codegen: destructors

- **Polymorphic implicit `??_G`: define the implicit destructor first.** Write `void* MakeOne() { return new C; }` and `void KillOne(C* p) { p->C::~C(); }`.
  - The qualified explicit call makes VC7.1 define the implicit destructor before it emits the vtable's `??_G`, which then inlines it.
  - Without it the `??_G` calls an out-of-line `??1`: G032 0x009C6F40 went from 30 to 48 bytes.
  - With two implicit levels, add one Kill per level: G025 0x006DF8C0 needs `KillTrophy` for CCreatureAction_ShowTrophy and `KillOne` for the class (30 -> 54).
- **Non-virtual `??_G...QAEPAXI@Z`** comes from a plain `delete p` on a class with a non-virtual destructor; its own COMDAT is the row (G042 0x006E26C0).
- **Size-optimised implicit virtual destructors** (G030 0x004E32C2) run in this order:
  - the members in reverse order;
  - the base's inline user-written destructor, which stores its vtable;
  - a tail `jmp` to the next base's destructor.

## Codegen: STLport containers

- **The `_Rb_tree` copy constructor inside set/map copy constructors needs `__forceinline`** (G031 0x00513700, G033). Under plain `inline` VC7.1 keeps it out of line as a 19-byte forwarder.
- **In /Os TUs VC7.1 does not inline STLport helpers that retail inlines.** Put `__forceinline` on:
  - `allocate`;
  - `__copy` / `__copy_ptrs` / `copy_backward`;
  - `__uninitialized_copy` / `__uninitialized_fill_n` / `fill`.

  Examples: G063 0x0045C71C went from DIFFER(53v63) to MATCH with `__forceinline` allocate; also G060 0x00456263, G064 0x00464100 and G065.
- **Tag homing on a LIVE argument slot.** When retail homes STLport's empty tag temporaries on a live slot (`atend`, `first`), give the out-of-line callees same-TU `__declspec(noinline)` bodies under `#pragma auto_inline(off)` that never touch the tag (G021 0x00440D70, G026, G055 0x00449130).
  - G072 0x004940AC is the same G067 vector erase with `__copy` also out of line: both `__copy` and `__destroy_aux` are same-TU noinline `__fastcall` bodies, so both tags home at [ebp+0Bh].
- **`stosb` zero-stores.** A `__false_type` without a user constructor is value-initialised with a `stosb` zero-store (G064's overflow call 0x00464100). With a user constructor nothing is stored. Match whichever retail does.
- **/Os `_Rb_tree::erase(const key&)`** (G053 0x0042CDE0):
  - `__distance(const It&, const It&, const input_iterator_tag&)` is an out-of-line `__cdecl` COMDAT.
  - The bidirectional tag lives in a byte of the dead key slot.
  - The caller pops only 8 bytes and reuses the third slot for erase's first by-value iterator.
- **Speed-TU `erase(key)`** (G005 0x004C4430) keeps a two-instruction scheduling swap. It persists with a noinline `__distance` body and with QFE-4035, so park it.
- **STLport list `erase(iterator)`** in a size TU without a frame pointer: `#pragma optimize("s", on)` gives retail's `pop ecx` cleanup, where /O2 gives `add esp,4` (G071 0x004937CB, `ret 8`, hidden iterator result).
  - The element is destroyed through vtable slot 0 with `push 0`.
  - The node is freed with no null test.
- **STLport `_List_base<T>::clear()`** (list.c) is its own out-of-line /O2 COMDAT (G073 0x004A9CF0). The unreferenced `~list` / `~_List_base` pairs call it.
- **Signed versus unsigned keys** in `_Rb_tree::insert_unique(iterator, v)` change only the jcc / setcc forms. G069 0x0047C8C0 reuses G028's template with the key type swapped.
- **The /Os hinted `insert_unique`** (G004 0x0042C1C1, 319 bytes) is unsolved.
  - Retail keeps `this` in ebx and the key in edi across the static `_Rb_global` step calls.
  - Every same-TU stand-in tried keeps the swap.
  - The /O2 twin (G028) matches.

## Codegen: defs, persistence and other bodies

- **Def `Transfer` passes the field only.** Use the single-argument `CPersistContext::Transfer(unsigned long& / long& / float& / bool&)` overloads (e.g. 0x00431020). Enums and vectors go through the member template `Transfer<T>(T&)` (G057, G061).
- **`CPersistContext::Transfer<T>`** for a def sub-object (G066 0x00464B9A, /Os with a frame pointer):
  - the `CPersistTraits<T>` local inlines its traits wrappers;
  - T's own constructor and destructor are called out of line;
  - binary load / save go to T's own TransferBinaryIn / Out.
- **Def compiler-generated `operator=`** is reached by a `jmp` from the virtual `Copy` (vtable slot 19) (G022 0x0046296E, G058 0x004537A5, G059 0x00454088).
  - In a size-optimised def TU the base `operator=` is out of line and the members are copied with integer moves, floats included.
  - Name each row from its Copy's RTTI.
  - `#pragma optimize("s", on)` gives retail's one-register member copies; /O2 rotates eax / ecx / edx.
- **Model a map member with typed fields** (`void*`, `unsigned int`, `_Compare`), not `unsigned char _M_t[12]`. A byte-array map is 1-aligned and shifts every later member: in G030 0x004E32C2, CRumbleDef's maps fell to an unaligned +0x25.
- **Inline reference clamp.** `GFLimitReturn` (FableWin `??$GFLimitReturn@M@@YIABMABM00@Z`, `__fastcall`, returns `const T&`) is the inlined clamp of CTCHeroMorph's setters (G043 0x0071BE20). STLport min / max gave DIFFER(78v93).
- **Captured out-parameters.** The `pMS` out-parameter of `GetPeerSeeking(&pMS)` needs a same-TU noinline stand-in body for the callee, or VC7.1 reloads it from the stack (G049 0x00CAD4C0, 52 against 50 bytes).
- **vector `_M_fill_insert` for a 4-byte POD** (G001 0x00440F00):
  - `fill` is a same-TU noinline `__fastcall` body under auto_inline(off);
  - x_copy, elems_after and n * 4 are homed in the dead argument slots.
- **libpk float setters** (G018 0x00C3D360, G019): `if (field != v) { field = v; flags |= mask; }` gives `fld value ; fld field ; fucompp` and a dword copy of the float.
- **No-/GX TUs.** Without /GX, temporaries' destructors run without EH frames. The CCharString temporary of a literal argument is built before any push and destroyed after the call (G013 0x0062DAD0).

## Leaves and tests

- **Leaf back-references.**
  - An allocator inside a pair element back-references `@2@`.
  - A non-nested struct pointer is `PAU1@`.
  - A wrong back-reference (`PAU12@` for 0x00B7AF60) silently graded the wrong function.
  - Where the tail cannot be predicted (enum arguments `W4...@@`), use a mangled prefix leaf (G064).
  - Confirm every leaf with `gen/bindchk.py` (NOLEAF) or `objdump -t`.
- **Grade variants one at a time.** A parallel batch of variants overran the 4-grade budget on 0x009C6F40 (6 grades).
- **Tests:**
  - `unsigned long` is 8 bytes under g++ x64, so use `unsigned int` in sizeof checks.
  - Never write `__cdecl` in a test.
  - Return temporaries, not named counted locals: VC7.1 has no NRVO.

Review at landing:
- **0x009C6F40** (G032, CACompressor `??_G`) needed 6 grades, over the 4-grade cap; the KillOne variants were graded in one parallel batch. All other rows are within 4.
- **Catalog binding conflicts** that bindchk flags:
  - `??$?RVCThing@@@CThingCompare_Nearest@@QBEPAVCThing@@PAV1@0@Z` is bound to 0x0091F740, which is operator()<const CThingObject>. The true operator()<CThing> is our 0x004A8D80.
  - `??4CBalverineBattleDef@@QAEAAV0@ABV0@@Z` is bound to 0x00478876, which is CAbilityTrackLevelDef::operator=. The true one is our 0x004E485C. The landed 0x00478C2D is vector<CAbilityTrackLevelDef>::erase.
  - The landed 0x0042C058 equal_range is modelled as `_Rb_tree<unsigned long, pair<const unsigned long, long> >`, but its tree is the CTCInventoryTrade map<CComponent*, CComponent*>. Its `_M_lower_bound` / `_M_upper_bound` bindings (0x0042AE90 / 0x0042AEB5) collide with the real ones that our 0x005D8676 calls (0x005D84F2 / 0x005D8446).
  - `??1CBaseClass@@UAE@XZ` is bound to 0x0099A300 (landed 0x00418B7A, 0x008385B0), but 0x0099A300 is `??1CBaseClassNonCopyable@@MAE@XZ`. `??1CBaseClass@@QAE@XZ` is bound to 0x0099A2E0 (landed 0x004C4160). Our 0x009C6F40 / 0x00A5E670 call the true `??1CBaseClass@@UAE@XZ` 0x0099A2E0.
- **Wrong declarations or names in already-landed sources** (details under NOTES.md Label corrections):
  - Landed 0x00913F30 CAIStateGroupBirdFlyToFood::IsRunnable declares 0x00914330 with a CThingPhysical** out, which the mask contradicts.
  - 0x009F1470 spells CSectionDesc as global; Ego_r nests it in CCodeSectionManager.
  - The 0x006924B0 animation lookup has a descriptive name.
  - CCreatureActionBase constructor bindings have spelling differences.
  - `??0CEventTalkedToBy@NEntityEvents@@` is catalog-bound to 0x0084B100, whose vtable is CEventTalkedToAnyone's; no CEventTalkedToBy class exists in either PDB. The catalog also spells the CEventTalkedTo / CEventTriggered parameter as `CThing*`, where FableWin.pdb has `CThing&` (G038).
  - 0x006E2500 is catalogued as `??1CParticleUpdateParameters@NParticleEngine@@UAE@XZ` / `??1SUpdateParameters` / `?Cleanup@Sub28`. FableWin.pdb names it `??1?$CParticleUpdateParameters@KK@NParticleEngine@@UAE@XZ` (G042).
  - Landed copy helpers carry invented names, and 0x00AA8360 is catalogued as CShopItemDef; it is `CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*)`.
  - The landed 0x00A80E40 file says CRegionConnectionNode but is the CNavConnectorNode pair destroy (its binding spells pair `V?$pair`).
  - Destroy helpers bound under invented names: 0x004488B0, 0x004AD9E0, 0x00825AE0, 0x00B8F470.
  - Seven landed `_Uninit_copy` helpers name the wrong element (CActiveEntityScriptBase).
  - These base constructors are bound with PDB-inexact names: 0x00771200, 0x004D3A03, 0x004DC991, 0x0044C3EE. 0x00686800 is also bound as CEngineSubPrimitive2DClockSprite.
  - The G022 / G058 / G059 Copy thunks are bound under invented extern "C" names. They are the virtual `<Def>::Copy` (`*this = *(const <Def>*)other`).
  - 0x004595A3 is bound with the `struct CRGBColour` (U) spelling; CRGBColour is a class.
  - The landed "CArray<pair<long,..>>::push_back" rows (0x00464093 ..) are vector<T>::resize(n, x).
  - 0x0045CFAD is CSoundMap::operator=.
  - 0x0067C490 is CBlurredAttitudesDef::Copy (filed as CCardDef_operator).
  - The G030 landed `??_G` rows model their callees as extern `<Class>_Destruct` functions; retype them to `??1<Class>@@UAE@XZ`.
  - G029 and G031 / G033-G036 bindings use non-template or struct-spelled aliases of the same functions (listed in Label corrections).
  - The landed 0x00493B5B binds 0x004937CB under an invented `CMessageEventList::erase`.
  - The landed 0x00882C40 uses an invented CRetailList; it is `_List_base<CScreenFilterEntry>::clear`.
- **Local base models next to existing engine headers.**
  - Most class-specific rows model `this` locally: the generated `engine/<Class>.h` headers carry the flattened Ego_r layouts, whose STLport members are 4 bytes too large, or data-only vptrs that cannot carry a virtual destructor chain.
  - Each source says so in its `// Model:` / `// Layout:` line (for example G013, G025, G030 and G044).
  - The headers need retail-size STL members before these rows can include them.
- **Probable and descriptive identities** are listed at the end of NOTES.md:
  - 12 probable, including 0x00617D50, 0x0042FD6F, 0x009D3670, 0x009D83E0 and 0x00A81D60.
  - 34 descriptive, including the libpk G018 / G019 setters, the `CUnprovenElement_*` element types of G001 / G017, 0x00CDBD70 and 0x007C0FB0.
- **Catalog rows that hold several functions:** none found. Every row in this chunk is one function; the int3-after-noreturn exception did not occur.
