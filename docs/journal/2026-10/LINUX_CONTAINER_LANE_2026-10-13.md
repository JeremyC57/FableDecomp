# Linux container lane — 2026-10-13

Session on branch `FullDecomp` (head `8d6ce97c` at start) in a cloud Linux container
(4 CPU, 15 GB, Ubuntu noble, Java 21, Python 3.13, no Windows, no VC7.1).

## What the container can and cannot do

| Capability | Status | Evidence |
|---|---|---|
| Retail `Fable.exe` | yes | extracted from the user's RAR (RAR5; Ubuntu `7zip` lacks the codec, `unrar` works): 16,666,624 bytes, linker 7.10, 6 sections, entry `0x401067`, `.text` 0xE2B8A8 |
| Ghidra 12.1 headless | yes | `ghidra_12.1_PUBLIC_20260513.zip`; full auto-analysis of the 14.8 MB `.text` took ~20 min on 4 cores |
| Reproduce the named DB | yes | `LabelApply.java` fed a 57,097-row TSV generated from `rebuild/manifest/functions.tsv`: 55,540 renamed, 12,660 functions created, 0 failed |
| Byte-match grading | **no** | needs `cl.exe` 13.10.3077 on the Windows box; nothing authored here can be promoted past `candidate` from the container |
| Byte-exact oracle extraction | yes | `pe_oracle.py` logic ported to env-driven paths (scratch); `crawl/rowtrim.py` works unchanged (capstone from pip) |
| Shape templates | yes | opcode-skeleton index over the landed `MATCH`/`RELOCATION_MATCH` set finds byte-exact siblings for ~40% of the smallest unlanded bodies |

## Landed in the tree

- `rebuild/candidates/manual/batches/2026-10-13_linux_b1/` — 29 commented, header-typed
  candidate sources with self-contained VC7.1 behaviour tests, `payload.json` + `oracle.tsv` in the
  exact `verify_and_land.py` input format, and a README with the grading command. All 58 TUs pass
  a host syntax check (`g++ -m32 -std=c++98 -fsyntax-only -I rebuild/include`), the purity gate
  and the shared-header policy check (0 warnings).

- `rebuild/candidates/manual/batches/2026-10-13_linux_b2/` — 42 more candidates, restricted to
  catalog-unique labels on header-backed classes; 84 TUs pass the same three checks. Its README
  tabulates seven retail-vs-PDB layout shifts (CBankFile, CTCHeroStats, CTCInventoryTrade,
  CTCInventory, CLandscapeBackgroundTreeNode, CNavigatorManager, CQuestManager) that the generated
  headers do not yet reflect, and one relabel (0x00CACBE0 is a `__stdcall` COM AddRef helper).

- `rebuild/candidates/manual/batches/2026-10-13_linux_b3/` — 23 more (same selection rule); 46 TUs
  pass the three checks.
- `rebuild/candidates/manual/batches/2026-10-13_linux_b4/` — 28 more, including five CTCVillage
  functions placed by the refined container rule; 56 TUs pass the three checks.

## Findings worth keeping

- Seven of the smallest unlanded "families" are not what their labels say. The 747 bodies labelled
  `CActionDoCreatureAction::GetActionName` are constructors: each builds a `CCharString` at `this`
  from a distinct literal (`"CTCTargetingPlayer"`, `"SG_PREPARE_MEAL"`, ...) with length `-1` and
  returns `this`. The strings are thing-component class names and AI state-group names.
- `CUserProfileManager` "NewProfile" fields sit 0xBC bytes after their live counterparts in retail
  (Ego_r.pdb says 0xC0): the shipped build dropped one 4-byte field before that block. Getters
  labelled `GetCameraSensitivity` (0x0054A530/0x0054A590) are `TextureDetail`/`MeshDetail`; the one
  labelled `IsCameraResetting` at 0x00548710 is `VerticalSyncEnabled`.
- `CTCWife::ClearAppearanceChangedNoticed` writes `FrameNoticedChangeInAppearance[category - 2]`
  (array at +0xAC per PDB, store at +0xA4 + 4*category).
- `CEngineSceneGridCell::InvalidateAll` clears bits 0..2 of the +0x44 bitfield (BoundingBoxValid,
  TotalCountValid, RenderMaskValid) up the `Parent` chain, stopping at the first already-invalid cell.
- `CThing` +0x48 is `CVectorMap<ETCInterfaceType,CTCBase*> Components` (PDB), so the body labelled
  `CThing::GetPBaseDef` at 0x007334B0 (reads +0x48 as a ref-counted pointer) is mislabelled; skipped.
- **Container-size rule.** Relative to Ego_r.pdb, a retail `std::vector` / `CArray` / `std::map` /
  `std::set` / `CVectorMap` is 4 bytes smaller and a retail `std::list` is 8 bytes smaller (head
  pointer only); a retail vector is `{first, last, end}` with `first` at +0. Members move earlier by
  4 per vector-like and 8 per list that precede them. Proof: CTCVillage (17 lists + 1 set before
  `Initialised` = 0x8C; retail reads AmountOfFineAlreadyPaid +0xA0, HeroEnemyOfGuardsUntil +0xD4,
  CurrentIsHeroCriminal +0xE4, CrateTasks +0x60 exactly there), CPlayer (InitialPlayerModeSet
  +0x216 after the PlayerModes list), CTCHeroStats (Marriages +0x1D8 after six vectors/maps),
  CBankFile (-0x10 / -0x24), CTCInventory (-0x28), CTCInventoryTrade (-0x2C), CQuestManager (-8),
  CTCCombat (-8), CClassFactory (-4), CTCTurncoat (-4), CTCCreatureGroupBoastingCrowd (-4).
  `gen_class_headers.py` can derive retail layouts mechanically from the PDB member types; until
  then the batch TUs carry `<Class>_Retail` views.
- Callee labels reached from these bodies are mostly propagated garbage (`CPixelShader::CountReferences`
  at 0x00662000 is called on a creature, `CWorld::DrawGetEnvironment` at 0x00449700 on a
  CPlayerManager). Only landed `PASS` identities were used for naming.

## Grading and landing (same day, after the Wine lane came up)

All four batches were graded in the container with the DDK 13.10.4035 `cl.exe` under wine32
(`tools/decomp_pipeline/linux_lane/`, `docs/BUILDING.md` "Linux verification lane"). First pass:
25/29, 30/42, 14/23, 18/28 byte-exact. Every miss was then diffed against the retail bytes with a
capstone side-by-side (scratch `q.py` harness: `parity_of()` + both disassemblies) and re-authored
until it matched; the landed files carry the reason in a `Lane feedback:` comment.

| Batch | Rows | Landed | Still DIFFER |
|---|---|---|---|
| b1 | 29 | 29 | — |
| b2 | 42 | 41 | 0049d8c0 `CWorld::EventIsWorldEvent` (23v23, entry pointer materialised) |
| b3 | 23 | 21 | 00a371c0 `CClassFactory::AllocateClass` (40v42, `pop esi` placement), 005d8e90 `CTCInventoryItem::GetInventoryOwner` (42v43, sibling-call `jmp` vs `call;ret`) |
| b4 | 28 | 27 | 009959f0 `CDataOutputStream::WriteString` (46v46, `mov edx,offset ""` hoisted) |
| **total** | **122** | **118** | 4 (ledgered in `crawl/gen_tried.txt`) |

Compiler rules the misses taught (all now one line each in `docs/pipeline/GOTCHAS.md`):

- bool results: `return cond ? true : false;` is the retail `sete al`; `return cond;` is 4-6 bytes longer.
- a value read before a call stays before the call only as a named local (IsMainPlayer, GetUnusedSpace, WantsNiceGift).
- retail STL access goes through iterator objects: `*(begin()+n)` / `*begin()` / `end()` re-read from
  `this` each iteration and never CSE with `size()`; `CRetailVector<T>` / `CRetailList<T>` (12-byte
  vector, 4-byte list) reproduce it (GetRegisteredQuestFromIndex, GetContextSensitiveFadeAlpha,
  IncrementProjectileWeaponMode, GetPMeleeFlourishAbility, DeleteAllClasses, IsThereACratePutDownTask).
- a store the retail bytes re-read immediately = inlined helper with a `long&` parameter or a
  const-ref `max` template (SetMaxStamina, ForceRenownLevelTo, ForceSpendableExperienceTo).
- `#pragma optimize("s",on)` TUs: shared epilogue, `and [mem],0`, pass-through `push [esp+n]`
  (SetCropTaskAsAcceptedByThing, GetContextSensitiveFadeAlpha, SetMaxStamina, ForceRenownLevelTo,
  IsConflicting, IsThereACratePutDownTask) — the lander sweep selects it and records `["s"]`.
- `dec eax ; jne` = one-case `switch`; `cmp 4;jl / cmp 5;jle / cmp 7;jne` = `switch` with cases {4,5,7}.
- `test ah,5 ; jp` is ordered `<`; the SetTurncoatTimeLast draft had the sense inverted.
- callee-saved registers are assigned in reverse declaration order of the locals.
- `wchar_t` is a typedef (no `/Zc:wchar_t`): include `<stddef.h>`; by-value class return with no
  `add esp,4` = `__stdcall`.

## Batch 5: compile-as-you-go (same day)

With the lane at ~0.5 s per compile (persistent `wineserver -p`), the next 80 smallest un-landed
functions were authored straight against the compiler: generator `gen/batch5.py` → `payload.json` →
`gp.py` (parity + sweep + side-by-side disassembly for every miss) → reshape → land.
Result: **58 of 71 real functions landed** (9 of the 80 picks are mis-trimmed fragments, skipped);
13 DIFFERs are ledgered, all instruction-order misses (`rebuild/candidates/manual/batches/2026-10-13_linux_b5/README.md`).

New codegen rules from this batch (also in GOTCHAS): a named `float` local holding a call result
blocks the sibling-call `jmp` (pointer/long locals do not); a `(long, float)` constructor reproduces
the x87/register mixed copy of an 8-byte by-value return; a POD 8-byte struct comes back in
`edx:eax`, a non-POD (any user ctor) through the hidden pointer; to get only the plain `??1`
destructor body out of a polymorphic class give it a virtual function and a NON-virtual destructor
(a virtual destructor makes the extractor see the deleting `??_G`); `x * <literal>` loads x first
and multiplies by the constant pool, `x * <extern>` loads the extern first; a two-word
`Helper(a, b)` that keeps `this` in ecx is a __thiscall member call; `inc dword ptr [mem]` pairs come
from an inlined per-object helper, not from four statements on the owner; `ret` vs `ret N` fixes the
real parameter count regardless of the manifest prototype.

Coverage: catalog 18,631 → 18,749 → 18,807 byte-exact entries (37.6% → 37.9% of 49,568). The five
`__stdcall` thunks needed `#pragma optimize("gsy",off)` + `("s",on)` in-source (size, no global
optimisation, frame pointers kept): a TU setting the lander sweep does not yet try.; the compile-gate
ledger still needs the canonical refresh on the Windows box to count them.

## Family landing (batches `2026-10-13_linux_b6n` and `_f<N>`)

Hand-authoring ~80 rows per batch could not keep up with the catalog, so the un-landed bodies of
≤ 64 bytes were grouped by opcode skeleton (`tools/decomp_pipeline/families/famscan.py`: 12,574
bodies, 4,848 families) and one parametrised, commented source was written per shape
(`fam.py`, one template per family index, aliases where the lander sweep covers a scheduling
difference such as `pop ecx` vs `add esp,4`). Every template was graded on 3–5 samples with
`gp.py` before the whole family went through `verify_and_land.py --land`.

| wave | families | rows | landed |
|---|---|---|---|
| b6n: `CCharString("<lit>", -1)` returners | 1 | 179 | 179 |
| 1: factories, ctor shells, Clone, list/block/tree ctors, `??_G` | 13 | 1,548 | 1,516 |
| 2: Alloc(a,b), embedded members, Initialise+animation name, persist, static initialisers, copy ctors, … | 20 | 849 | 838 |
| 3: ctor/dtor variants, vfunc getters/setters, allocators, static list/map initialisers | 32 | 445 | 425 |
| 4: flag setters, root ctors, predicates, global getters, re-initialisers | 11 | 69 | 63 |

Total 2,832 family rows → 2,800 landed (`famfinal.py` wrote the per-batch READMEs and headers, and
ledgered the 24 DIFFERs). Catalog 18,807 → **21,786 byte-exact entries (44.0% of 49,568)**.

Rules learned (all in GOTCHAS): by-value enum = `long`; `rtti-vtables.tsv` names classes, no-RTTI
vtables (0x012C2F64.. module) become `CVtable_<va>`; base/member constructor labels are used only
when unique in the manifest; a CCharString temporary handed to a setter is a scoped named local;
`xor eax,eax ; cmp ; sete al` is an int result; a vtable store before zero stores = an embedded
member with an inline ctor, a vtable store after member destructors = an inlined empty
intermediate-base destructor with a `__declspec(novtable)` derived class; `if (p) delete p;`
keeps the null test; a member store before the vtable store in a root class = an inlined base
constructor whose own vtable store is dead. Lander change: `.text$yc`/`.text$yd` sections (dynamic
initialisers and atexit thunks) are now read, so `CCharString g_x("lit", -1);` lands the retail
`??__E` body (288 rows across F7/F25/F27/F28/F106/F153/F154/F71).

Open after ten-plus variants each: F15 (76 rows), F73 (14), F148 (9) — `docs/journal/FINDINGS_LOG.md`.

## Family waves 5–7 (128-byte rescan, batches `2026-10-13_linux_g<N>`)

`famscan.py 128` over the post-wave-4 catalog (11,111 families, 19,632 bodies); `fam.py` now
dispatches on a template id separate from the family index (`FAMFILE`/5th argument), so one
template serves every family of a shape across scans. 42 families → 1466/1474 rows landed
(wave 5: 906, wave 6: 338, wave 7: 207). Catalog 23,237 byte-exact entries (46.9% of 49,568).

Shapes landed: `CCountedPointer<T>::Reset`/constructor (195 + 36; the count block is rebuilt from
`m_pObject`, not the argument), retail red-black tree destructor (227), iterator `--`/`++` (109 + 48,
Dinkumware `_Dec`/`_Inc`), `find` with long keys (56, single-return form), `_Uninit_copy` (42,
`T* d = dest;` + `++first, ++d`), `~vector` of polymorphic elements (19, `last` cached), `_Buynode`
(28, placement new), persist read hooks (21 + 29), combat-action constructors with a zeroed
`CSoundPair` and an animation name (54), copy constructors with an inlined smart-pointer copy (30),
`SuspendableProcess` with a by-value `CScriptThing` (120), `Release()` of a static counted pointer
(25), non-virtual `??_G` of non-polymorphic classes (15 + 36: `delete p` in a helper makes VC7.1
emit it), EH-framed destructor/constructor/call shells (42 + 41 + 30, `/EHsc` added to the lander
sweep), adjustor thunks (18: both bases must declare the virtual), `GetInterface_<type>` lookups
(47, `optimize("y",off)` + `("s",on)` and a named map pointer), static `cos`/`sin` float
initialisers (54, `#pragma intrinsic`). Rules are in GOTCHAS; the DDK STL headers are a different
Dinkumware revision from retail (FINDINGS_LOG: F14 `std::copy` front ends), so STL internals are
written out by hand.

### Wave 8 (EH module + STL helpers)

Seven more EH-framed shapes of the 0x00C09000 module (parameter objects passed to virtual slots,
constructors with out-of-line members, pair destructors with states 0/-1, a novtable destructor, a
guarded virtual query: 54 rows), container removal helpers (38), the Dinkumware four-way unrolled
`_Find` (22) and `_Uninit_fill_n` returning the advanced pointer (18): 132 rows. Running total for
the 128-byte rescan: 1598/1609 rows from 53 families; catalog 23,369 (47.1%). Two shapes
parked with their evidence in FINDINGS_LOG: F467/F624 (`this` survives a call in edx — a custom
convention, probably /GL) and `GetDef<T>` (the temporary's release sequence).

### Wave 9 (256-byte rescan)

`famscan.py 256` surfaced one huge family: the script module's `CCountedPointer<CX>` factories
(557 + 33 bodies, `__fastcall`, hidden result in ecx) — `return CCountedPointer(new CX(owner,
thing, arg));` with a three-level constructor chain inlined — all landed. The VC6-era Dinkumware
`_Tree::insert` / `_Insert` pairs (signed and unsigned keys, frame-pointer TUs) landed 98 more.
Running total for the rescans: 2271/2282 rows from 59 families; catalog 24,042 (48.5%).
Parked with evidence in FINDINGS_LOG: the script `Main` shape (163, temporary slot order), the
default-optimisation `insert` (56, one extra local), `GetSubDef<T>` (151, the release sequence).

### Wave 10 (256-byte rescan, small STL/engine shapes)

Twenty-six more families of the 256-byte scan, 428 rows: the Dinkumware `_Uninit_fill_n`
family (`__fastcall`, placement copies through out-of-line copy constructors, pair variants with
one scalar member, the 8-byte trivially-copyable pair: 78), retail vector `push_back` (60: the
reallocating insert's third argument is a const reference to an EMPTY TAG object — give the tag a
user constructor so the 1-byte temporary is not zero-stored; VC7.1 homes it in the dead argument
slot, which is the address retail pushes), `pair<unsigned long, X>` copy constructors (13),
implicit destructors of non-polymorphic aggregates (31: declare the destructor out of line, a
`p->~T()` helper inlines it), shared-pointer-member destructors (12), non-virtual `??_G` with a
base destructor (15), persist read/write hooks (31) and the by-value `CIntelligentPointer` setter
(13), `LessByKeyThenAddress` (10), recursive tree frees (61), zeroed-member constructors (37),
list `Buynode` for scalars (10), and `CPersistTraits<T>::TransferBinaryIn` (70: the inlined
`CMemoryDataInputStream::Read` with its `__int64` range check — `m_position += size` keeps the
old position live and yields retail's `lea`; a struct-typed local makes the 4-byte memcpy a
`movsd`, a scalar one forwards straight into the output; frame-pointer + size TU).
Running total for the rescans: 2699/2714 rows from 84 families; catalog 24,470 (49.4%).

Parked with evidence in FINDINGS_LOG: the vector grow-and-insert F17/F23 (72 rows; every source
form reproduces the body except which of `count`/`buffer` gets the callee-saved register), the
persist forwarder F33 (24; retail re-reads the returned argument instead of caching it), and the
non-frame-pointer `TransferBinaryIn` F24 (31; identical instructions, two range-check jumps target
the other duplicated tail).

### Wave 11 (256-byte rescan, tree copy and counted-pointer containers)

Eleven more families, 251 rows: tree iterator increment and the unsigned-key `find` through the
existing templates (28), the frame-pointer `push_back` (12) and a `PushBackConstant` wrapper (15),
derived-class `operator=` over an out-of-line base assignment (14), `list<CCountedPointer>::clear`
(13: advance the cursor first, then destroy the old node) and `_Move` of counted pointers (14: the
inlined operator= reads the source's fields before the release), the VC6-era `_Tree::_Copy` /
`operator=` pair (68: `parent` re-used as the running predecessor, a named head local across the
Copy call, `and dword ptr [..],0` size TUs), the EH-module collection transfer (41) and the
hashed-name constructors (46: `m_name("LIT")` -- an IMPLICIT CCharString conversion temporary is
re-addressed with `lea`, an explicit `CCharString("LIT", -1)` reuses the constructor's returned
pointer). Running total for the rescans: 2950/2966 rows from 95 families; catalog 24,721 (49.9%).
Parked: the script callback `Fire` (F30, 26 rows; the `this` copy lands between the two vtable
stores of the by-value CScriptThing argument here, after them in retail).

### Wave 12 (SGI algorithm instantiations)

The retail algorithms are SGI-STL, not Dinkumware: `sort` is `__introsort_loop(first, last,
(T*)0, __lg(last - first) * 2, comp)` followed by `__final_insertion_sort` (threshold 16, inlined
in some TUs and out of line in others), the copies are SGI's counted `__copy(first, last, result,
random_access_iterator_tag, Distance*)` (`rep movsd` for larger PODs, an out-of-line operator=
otherwise), and every free function is `__fastcall`, i.e. those TUs were built with /Gr. Nine
families, 133 rows. Running total for the rescans: 3083/3105 rows from 107 families;
catalog 24,854 (50.1%).

### Wave 13 (512-byte rescan, batches `2026-10-13_linux_i<N>`)

`famscan.py 512` over the post-wave-12 catalog (19,558 families, 27,878 bodies; 168 families of
8+). Twenty-one families, 249 rows: Dinkumware `_Uninit_copy` / `_Fill` / `_Destroy_range` /
`_Uninit_fill_n` loops (a >8-byte POD needs an explicit memberwise copy constructor to get
direct dword moves instead of a pointer-register block copy), counted-pointer vector and pair
destructors, `fill` through an out-of-line operator=, `distance` over tree iterators (SGI
`__distance`: advance, then count), the unsigned-key `find` and the 0x1c-byte tree constructor
through old templates, `erase(first, last)` with the SGI counted copy inlined (the compiler folds
the source into an offset from the destination), `resize` with Erase/Insert out of line, STLport
`__insertion_sort` (`__linear_insert(first, i, T(*i), comp)`: the by-value element has a user copy
constructor), and `CPersistContext::TransferBinary` (mode 2 = load; the two-way switch lowers to
`dec/dec` under the size pragma). Running total for the rescans: 3332/3354 rows from 128
families; catalog 25,103 (50.6%).

### Wave 14 (512-byte rescan, second pass)

Twenty-one more families, 218 rows: four through older templates (`_Move`, `_Uninit_copy`
with a copy constructor, `__copy` through operator=, derived operator=), SGI `__copy_backward`
for 12-byte elements (POD block copy and operator= forms, two tags), vector destructors whose
elements hold two out-of-line-destructor members, list clear with an out-of-line value
destructor, derived copy constructors with trailing dword members, two-argument `__fastcall`
factories, the two-argument combat-action constructors (zeroed CSoundPair temporary, optional
scoped CCharString setter), persist `Transfer` hooks that hand each member to a typed context
helper, `_Tree::operator=` with the clear inlined, resize for reciprocal-multiply element sizes,
smart-pointer copy constructors and AssignTarget through old templates, and `_Uninit_copy` of
{long, X} / {X, long} pairs. Running total for the rescans: 3550/3585 rows from 150
families; catalog 25,321 (51.1%).

### Wave 15 (512-byte rescan, third pass)

Thirteen more families, 152 rows. The big one is the `CGameScriptInterface` script API family
(Set/Reset/Get over a by-value `CScriptThing`, 65 bodies in four shapes): the 12-byte parameter
is `CBase`-derived with an embedded counted pointer, its destructor is inlined on both the early
return and the main path (vtable store, count-block release, `CBase::~CBase`), the thing is
resolved through virtual slot 11 of the pointee, and the TC lookup is an inlined `CVectorMap`
find whose key must be an enum converting to the `LowerBound` reference (a frame temporary) so
the compare keeps the immediate; the "map absent" path reads an uninitialised local that VC homes
in a dead argument slot, and the source reproduces that. Also landed: `CCombatSequence_X::OnCreate`
(counted temporary default-constructed, then assigned through `Reset`), gap-scan CCharString
literal callers (virtual slot 350 with a member address; implicit conversion for a bool query
with the result in `bl`), Dinkumware-style tree `Erase` returning the decremented size, two-member
persist hooks with dword/byte defaults in one frame slot, STLport list clear through a virtual
destructor, `_Uninit_fill_n` of counted pointers, and non-virtual `??_G` over one member.
Parked with evidence: the lazily created NUISystem list push_back (24 bodies, a zeroed dword the
compiler keeps that no allocator/tag model reproduces), `AddEngineComponent<T>` (20 identical
bodies, the conversion temporary lands in the first argument slot in retail and the second here),
the AI state-group TC hooks (`mov ecx,eax` after the lookup) and the two-level CVectorMap lookup
(one key temporary shared by two `LowerBound` calls with immediate compares). Running total for
the rescans: 3716/3752 rows from 165 families; catalog 25,473 (51.4%).

### Wave 16 (512-byte rescan, fourth pass)

Nine more families, 130 rows: the frame-pointer forwarders (STLport-style string range
constructors with by-value iterator classes stored into reserved slots and a `__fastcall` helper
taking the dispatch tag in edx; the 46-body two-argument forwarders whose tag is homed in the
dead second slot; the hidden-pointer forwarders with a 4-byte handle copied by value), the
combat-action constructors whose CSoundPair comes back through the hidden pointer from
`GetSoundPair` over an implicitly converted CCharString literal (with and without a scoped setter
string), the `__fastcall` tree serialisers (count, then two fields per node from the leftmost
to the head through the `__cdecl` successor, dword and float variants) and the scalar deleting
destructors of one-list-member classes (the base padded to the member offset; the translation
unit must construct the class so its vtable -- and with it `??_G` -- is emitted). Parked with
evidence: the 119-body script-entity factory (everything reproduces but the lookup-call
register naming), `_Insert_n` for 8-byte elements (spill slot), tree erase-by-key (one load
hoisted across a store) and persist `TransferFloat` (the zeroed traits temporary's store sits
between the mode test and its branch in retail). Running total for the rescans: 3846/3877
rows from 170 families; catalog 25,603 (51.7%).

### Wave 17 (512-byte rescan, fifth pass)

Six more families, 102 rows, all STLport containers: the tree `erase(first, last)` (clear
through the recursive release when the range is everything, else the inline successor, a
`__cdecl` unlink and a free per node), the retail vector `resize(n, value)` for 2- and 4-byte
scalars (the copy-down helper inlined as a memmove when shrinking, InsertN when growing; its
`if (src == srcEnd) return dest;` early-out is what reproduces the `cmp eax,eax ; jne`), the
59-body `insert_unique` over unsigned, signed and byte keys (descent, out-of-line decrement,
`_M_insert` through an explicit result pointer homed in the dead value slot, `Insert(y, y, v, 0)`
in the begin case and no comparator local), and `_M_insert` itself (node malloc'd and
placement-copied, linked left or right of y, header links fixed, `__cdecl` rebalance, the
iterator written through the result pointer). Parked with evidence: the 42-body
`CDefinitionManager::GetDef` family (by name, by index, size-optimised) -- the by-value
CDefPointer temporary, the out pointer's assignment through `m_p->Release()` and the fail path
at the end all reproduce, but the temporary's own release (`mov esi,[eax+4] ; mov ecx,eax ;
add eax,4 ; dec esi ; mov edx,esi ; test edx,edx ; mov [eax],esi`, `dec [eax] ; cmp [eax],0`
in the size TU) resists eighteen helper shapes (volatile, reference, out-copy, sub-object `this`);
StdSet insert over string keys (comparator inlining, 217 vs 254). Running total for the rescans:
3948/3979 rows from 176 families; catalog 25,705 (51.9%).

### Wave 18 (512-byte rescan, sixth pass)

Thirteen batches, 336 rows, led by the definition lookups: the ~100 `CDefinitionManager::GetDef<T>`
bodies (by instantiation name, by global index, by kind and index; speed and frame-pointer size
TUs) and the 151 `CDefClassBase::GetSubDef<T>` bodies (a block-scoped literal CCharString,
the sub-def index through virtual slot 14, the manager's CDefPointer<CDefBase> converting-copied
into the typed CDefPointer<T>) finally reproduce once the temporary's release is written as the
ref-count base's two inline accessors called from one member function on the pointee
(`DecRef(); if (GetRefCount() == 0) Destroy();` -- the adjusted base pointer is materialised
once for both uses, the stored count forwarded to the test, the def kept in ecx for the virtual
call), while the typed pointer's own destructor and assignment use the single-expression
`if (--m_count == 0) Destroy()`; the size TUs need `operator=` and the temporary's destructor
forced inline. Also landed: the speed-TU `CPersistTraits<float>::TransferBinaryIn` (31) once the
64-bit range check goes through an inline bool predicate (its exits then share the duplicated
return tails the retail way), the static tree atexit destructors (15), the frame-pointer range
constructor forwarders (11, size pragma) and the counted-pointer event dispatchers through a
single-inheritance member function pointer (26). Parked with evidence: the retail vector
grow-and-insert cluster (register assignment differs from the lane's 13.10.4035), map FindNode
over string keys (everything but one jump-threading fold), the lazy list push_back (a zeroed
frame dword), the CTriangleList counted factory (one hoisted argument load) and copy_backward of
8-byte pairs (induction-variable elimination). Running total for the rescans: 4284/4315 rows
from 189 families; catalog 26,041 (52.5%).

### Wave 19 (512-byte rescan, seventh pass: the small Dinkumware shapes)

Twenty-seven batches, 170 rows, almost all 35–80-byte algorithm instantiations: `_Uninit_fill_n`
and `_Uninit_copy` (POD dwords, memberwise-copied classes, `rep movsd` blocks, counted pointers
with addref; the placement null check is VC's own), `std::fill`, `_Destroy_range` (counted
pointers through `Release`, virtual destructors with the deleting flag 0), vector teardown loops,
`_Dest_val` over counted-pointer map values, the tree iterator distance, `sort_heap` over pairs
with a user copy constructor, `CVectorMap::UpperBound` over 12/24/40-byte entries, the
counted-pointer constructor from a raw pointer, and the thirty `IS_THING_ALIVE`/`IS_THING_DEAD`
predicates (nested ifs keep both tests as branches). Lessons: the size TUs show as unaligned
loops and `pop ecx` argument cleanup; a 12/16-byte element copied through one register is a
class with a memberwise user copy constructor, through three registers a POD of dwords, through
`rep movsd` a byte block; `upper_bound` needs the negated comparison so the advance is the
fall-through. Parked after two attempts each (all one scheduling or register detail short, see
the Windows-box note in ROADMAP): make_heap, equal_range, the implicit operator=,
Array_Initialize, the erase copy-down, and the copy/copy_backward shapes whose source pointer
retail eliminates. Running total for the rescans: 4454/4500 rows from 216 families; catalog
26,211 (52.9%).

### Wave 20 (EH unwind funclets of landed parents; FPU clamped setters)

176 rows from two sources. The 1,378 `Unwind@<va>` catalog functions are the unwind funclets
VC7.1 emits after every EH-framed function (`push -1 ; push offset __ehhandler`): one `$L<n>`-headed
`.text` contribution per parent holding its funclets in unwind-map order followed by the
`__ehhandler$` thunk, which the linker placed at 0x0121xxxx. They carry no source of their own, so
`tools/decomp_pipeline/families/funclets.py` walks each landed `/EHsc` parent's retail FuncInfo
(handler thunk → `mov eax, offset FuncInfo` → unwind map actions), checks the block is contiguous
and writes one payload entry per funclet whose source is a stub that `#include`s the parent's
landed file; `verify_and_land.py` grew a `funclet_offset` extractor (the funclet's bytes at that
offset of the object's `$L` block, relocations shifted) and per-entry `flags`, and reuses the
parent's behaviour test. All 167 funclets of the 167 landed parents graded RELOCATION_MATCH/PASS.
The second source is the nine `CAttribute::SetClamped` bodies: the value clamped between two pooled
float literals through one nested ternary (each branch an FPU load, the store + flag tail
duplicated three times by VC) -- if/else chains or an inline setter fold the literals into integer
stores. 527 EH-framed parents (and their ~1,200 funclets) remain; they are mostly one-off bodies.
Running total for the rescans: 4630/4676 rows from 218 batches; catalog 26,387 (53.2%).

### Wave 21 (EH-framed families, their funclets, the ≤16-byte tail)

203 rows from three sources. (1) Six EH-framed families of the 512-byte scan, each a `/EHsc`
parent with a `push -1 ; push offset __ehhandler` frame: the assertion handlers F47 (14; a
CDebugStream over the shared buffer, file/line/message through `__fastcall` `operator<<`, the
report callback when installed, an empty CDebugBreak temporary whose dead frame slot keeps the
retail high byte), the mailbox `ProcessMessage`/`DispatchMessage` pair F163/F328 (8+6; scoped
lock, peek through the mailbox's virtual slot, the assertion stream in a nested scope with the
`CDebugBreak()` inside the `if`), the two-level `CIntelligentPointer` constructors F676 (4), observer
registration F1117 (3) and the pair destructors F1121 (3; the user-declared destructor is the
landed unit -- the implicit one yields only a 5-byte tail-call wrapper). (2) Their unwind funclets
through `families/funclets.py` (batches funclets2-5). The F676 funclets needed two fixes: the
state-0 funclet destroys the root base on `this` with no offset, so the root is a polymorphic
`__declspec(novtable)` class owning the vptr (the parent body is identical under both models;
only the funclet tells them apart), and under `/Gy` the inline base constructor is also emitted
out of line with a `$L` block of its own, so the extractor now tries every `$L` block in section
order. (3) The ≤16-byte tail through a second generic synthesiser, `wtiny2` (~24 instruction-pattern
recognisers: forwarders, through-pointer getters, bit tests, size getters, global setters, small
constructors): 114 of 132 bodies, after three model fixes -- a `mov al ; test ; setne al` predicate
returns `unsigned char` (a `bool` zeroes eax first), a `shr al,n ; and al,1` getter is a one-bit
bitfield of an `unsigned char` storage unit, and the `__stdcall` block-release wrapper is a
delete-expression (`mov eax,[esp+4] ; push eax ; call operator delete ; pop ecx`; `free` pushes
the slot directly). Running total for the rescans: 4833/4925 rows from 230 batches; catalog
26,590 (53.6%). EH-framed parents landed: 205 of 694; funclets landed: 218 of 1,378.

### Wave 22 (the EH 3-body families, their funclets, the ≤16-byte tail synthesisers 3 and 4)

195 rows. (1) The five remaining 3-body EH-framed families: observer notification F1116 (find,
then a virtual notify with `(this, argument)`), the file visitor F1123 (a 12-byte stack visitor
whose inline *empty* destructor is what registers EH state 0 -- nothing is called on the normal
path), the container constructors F1130 and destructors F1131, and the `new CX` factories F1136
(the class's own operator new/delete; the block stays in the frame for the operator-delete
funclet). F1130/F1131 took four lessons: a virtual call written directly in a constructor or
destructor is devirtualised by VC7.1, so retail's `call [eax+N]` there means the call sits in an
inline non-virtual helper; the most-derived class is `__declspec(novtable)` (its destructor body
runs before any vtable store); a member initialised before the EH state-0 store belongs to the
root base (the root's own vtable store is dead and dropped in the constructor); and the inline
helper with a loop needs `__forceinline`. Their 21 funclets followed (funclets6). (2) Two more
generic synthesisers for the ≤16-byte tail: `wtiny3` (22 recognisers over the 3-9-body shapes:
`::operator delete(this)`, unused-argument forwarders, COM-style `__stdcall` interface calls,
`__fastcall` import wrappers, hidden-pointer handle returns, bit counters -- 83 of 88) and
`wtiny4` (40 recognisers over the two-body shapes -- 76 of 83). Lessons: VC loads the right-hand
FPU operand first; a one-slot call whose result is returned is cleaned with `add esp,4` and a
dropped one with `pop ecx`; a byte predicate (`setne al`, `sbb al,al`) returns `unsigned char`.
Parked with evidence: the four `$E` initialisers that store one constant (VC7.1 folds every
inline constructor into static initialisation -- link-time inlining), the list copy constructors
F135 (63 v 63 bytes, one load scheduled differently) and three sibling-call wrappers the DDK
compiler always tail-jumps. Running total for the rescans: 5028/5165 rows from 240 batches;
catalog 26,785 (54.0%). EH-framed parents landed: 220 of 694; funclets landed: 239 of 1,378.

### Wave 23 (hand-authored EH-framed bodies, funclet passes 7–25, delegated hand-authoring agents)

Two lanes ran side by side. (1) Hand EH passes `_eh1..21`: 216 of the one-off EH-framed bodies of
the 0x00C09000.. module (105–170 bytes) written against the compiler one at a time with
`$S/tools/q.py` / `udiff.py`: scoped-lock queries and setters over the global @ 0x0143BB14,
member factories (`new CObject(this, b, c, a)`), container constructors and destructors built from
the root/node pattern (the root keeps the node pointer, the most-derived class is `novtable`,
drain loops through `__forceinline` virtual helpers), class arrays through `__ehvec_ctor`/`_dtor`,
COM-style `__stdcall` slot calls with `this` pushed, one-word critical-section holders with the
Win32 imports, action-request records with slot accessors, event dispatch with `delete` through a
virtual destructor, `std::vector` insert/erase shapes and float results parked in dead argument
slots. Each pass was followed by `families/funclets.py` (277 funclets in `_funclets7..25`).
(2) Delegated hand-authoring: the class-labelled 17–96-byte singleton/pair pool was cut into
40-row chunks (`$S/w22/chunkNN.tsv`, oracles registered for q.py) and handed to background agents
with a brief (`$S/w22/BRIEF.md`: tools, rules, test template, lessons); each agent iterates at most
four times per row and writes `payload.json` / `oracle.tsv` / `differ/` / `NOTES.md`; the
coordinator lands sequentially with `$S/w22/land_agent.sh` (strips the padding the chunk rows
carry, filters already-landed addresses). Chunks 00–14 landed 519 of 522 rows; the agents'
lessons and label corrections are in the batch READMEs. Lessons (also in GOTCHAS): constructor
shells read as embedded members with inline constructors (vtable store before zeroed stores) or
root-base members (store before the derived vtable); a member stored before EH state 0 belongs to
the root; `optimize("y",off)+("s",on)` for frame-pointer bodies; a non-throwing `operator delete`
declaration removes the EH state of an inline vector release; a member destructor declared
`throw()` as the last call drops the trailing state store; reference parameters make downcasts
null-check free; the Dinkumware `insert` temporary and other inlined locals are homed in dead
argument slots. Parked with evidence (FINDINGS): DDK-vs-RTM scheduling nits (adjacent stores,
register roles, a load before the `this` parking), the DDK folding `size() != 0` into a mask
test, EH frames the DDK drops for empty inline destructors, the DDK's STL `string(const char*,
const allocator&)` choice in `_Xlen`, and LTCG register-argument bodies. Catalog 27,809
(56.1%). EH-framed parents landed: 436 of 694; funclets in the catalog: 515 of 1,378.

### Wave 23, parts 7–9 (EH passes 22–26, funclets 26–31, hand chunks 15–23)

The hand EH lane resolved the long-standing "null-object warning" family: an MFC `CString`
(`g_pchNil` default constructor, `Append`/`AppendNumber` `__fastcall` chain, handed to the log
sink at 0x0143BB2C) built in its OWN nested block, followed by an empty-class trace temporary
(0x00C16750) homed on a dead frame byte. Other shapes that now reproduce: a `__stdcall` walk over
a multiply-inherited bank reached through a reference cast; MFC-style position lists with an
inlined `__forceinline Find` returning the caller's `position` (the found path re-tests the
memory slot); init-list members read back through an already-initialised member (`m_pWorld->`)
instead of the parameter; `CNode` containers whose root and container destructors are out of
line beside inline constructors; `__declspec(novtable)` intermediate classes so only the
most-derived constructor stores its vtables; virtual calls on `this` in constructors/destructors
through a non-virtual inline helper (`call [vtable+N]` when the vtable is known, dynamic dispatch
under `novtable`); unsigned-to-float conversions (`fild` + 2^32 fix-up); a Y/Z-swapped vector
copy; 8-byte result records; scalar initialisers assigned in the body after the member
constructors. The delegated chunks 15–23 landed 333 of 360 rows before the user capped delegation
at one agent; their label corrections (propagated creature-action, console and component labels)
and retail-vs-PDB layout facts (CCombatActionBase 0x114, CCreatureActionBase 0xa8, CConsole,
CCompressorZlib, CCompositeSurfaceCache) are in the batch READMEs and sources.

Parked with evidence (DDK 13.10.4035 vs retail RTM, re-grade on the Windows box): the merged
early-return block placed at the last return site (0x00C16E10, 0x00C31F60, 0x00C3CD00); a
constant for two state stores materialised in a register by retail only (0x00C14560,
0x00C14630); the DDK refusing to home an EH-tracked `CString` in an argument slot shared with the
trace temporary (0x00C0E1F0); the final `return 0` `xor` hoisted above the test (0x00CAC4A0);
init-list zero stores sunk past the semaphore pushes (0x00CAC9D0); the empty-string temporary's
data loaded into edi by retail, ecx by the DDK (0x00C101D0); a key materialised for a reg/mem
compare (0x00CAFE90); register roles (0x00C425E0, 0x00CAADB0); hoisted state store (0x00C32060);
indexed search loops strength-reduced by the DDK (0x00C34B80, 0x00C34E60). Catalog 28,291
(57.1%). Parked list: `$S/w23/parked.txt` / `parked_notes.txt`.

### Wave 23, parts 10–12 (EH passes 27–29, funclets 32–34)

Single-session work after delegation was capped: 35 more hand-authored EH-framed bodies of
236–291 bytes from the 0x00C0xxxx module (`_eh27..29/`, 70 funclets). New shapes that reproduce:
MFC `CString::Right` with two inlined null-object checks and the hidden-result flag; the
red-black node (shared or owned sentinel, self-linked link) and morphable node holder (ternary
over a common base) constructors shared by the module's containers; `__cdecl` factories with a
class `operator new`, a 0xa0-byte record assignment and `delete` through a virtual destructor;
action factories with a `__stdcall` error hook and class allocator; constructors whose scalar
initialisers are body assignments after the member constructors; float init-list zeros as
immediates beside integer zeros in the zero register; `novtable` side bases; `~CNavButton` /
`~C3DMesh2` / `~CVolume` / `~CCombatAbilityStrafeDef` destructors with inline container
destructors and per-pointer `if (p) { delete p; p = 0; }`; the mailbox by-value `Take`; the
worker-thread constructor on the landed thread procedure. Behaviour tests must initialise every
pointer the body reads (the VC7.1 heap is not zeroed). Parked (notes in `$S/w23/parked_notes.txt`):
0x00CAB770 / 0x00CAC300 (derived vtable stores above the section push), 0x00C14F90, 0x00CAFE90,
0x00C425E0, 0x00C101D0, 0x00CAF580, 0x00C39130 (state store before a non-throwing member
constructor), 0x00CAACA0, 0x00C3DA60 (tail-merged error block), 0x00C172F0. Catalog 28,387
(57.3%); the open EH set is now 300+ bytes per body.

### Wave 24, part 4 (chunks 30, 32, 34, 35)
Chunk 30 (CDrawMiniMap .. CDynamicObjectHeapBase) to the single sub-agent: 33/40 (`_hand30`; the NPlayerGui drawers and
CDrawQuestInfo list walkers under `optimize("s")`, CCombatManager::ResetDialogue, CDriveManager's file/directory removal,
CDynamicList::Initialise; parked on the call-prep-inside-store-group scheduling gap, FPU-pushed float arguments and the /GS
argument-load gap). Chunks 32 (CEngine .. CEngineInternalPrimitiveMeshBase: 33/40, `_hand32`), 34 (CEngineLightingManager ..
CEnginePrimitiveManagerDecal: 32/40, `_hand34`) and 35 (CEnginePrimitiveManagerDecal .. CEngineResourceManager: 34/40,
`_hand35`) by the coordinator: the internal-primitive container family over one shared vtable + VC6 handle-vector model, the
CreateInternalPrimitive allocator family (plain words before the vtable, member objects after), four manager destructors
over one CVertexShader model (`novtable` bases, a local pointer for the `dec ; jne` release), the engine manager's
component-list walkers, CPersistContext transfers, the 0x00C3xxxx last-error idiom, two `??_G` bodies (one needs `/EHsc`
and an explicit `leaf`: the lander now honours the payload's `leaf`, chunk sources carry `// Flags:` / `// Leaf:` lines).
Parked with evidence (NOTES.md, READMEs): the hidden return pointer kept in edx across a same-TU callee, the allocator
address from a dead flag slot, select-then-store duplication, a sqrt result parked through an integer local, the dead
`lea eax,[eax+edi*8]` of a reference-bound handle (five bodies), `mov ecx,eax` materialisation, a `??_G` without a vtable
store, a dead 16-byte zeroed temporary, zero-register `xor` hoisting (three constructors), index/first register
allocation, the empty-range inlined erase, and two bad family-scan rows (28-byte prefixes of larger functions: 00bd71b0,
00ba1140). Catalog 28,771 -> **28,903 (58.3%)**. Lessons in GOTCHAS waves 24e/24f. Next: chunk 33 is cut for the next
agent (`$S/w22/chunk33.tsv`, `agent33/PROMPT.md`); `mkchunk.py 36 1` for the coordinator (pool ~3,360 rows).

### Wave 24, part 13 (chunks 59, 60)

- Chunk 59 (coordinator, `_hand59/`): 27/40 (CMouseCursor .. CNavQuadTreeNode). Parked 12 (store scheduling, dead-argument-slot homing, the `find` tag over the live key slot, register choice) and skipped one row spanning two functions.
- Chunk 60 (coordinator, `_hand60/`): 36/40 (CNavQuadTreeNode .. CNavigatorQueueManager): the navigator manager / A* / flyer forwarders and lookups, five VC7.1 `list::clear` bodies, the 64-bit switch map lookup, the `/Os` `movsd` record forwarder. Parked 4 (GOTCHAS 24p).
- Catalog 29,669 -> **29,732** (60.0%). Chunk 58 is with the single agent (lands in part 14).

### Wave 24, part 12 (chunks 55, 56, 57)
Chunk 55 (CLocalDetailPrimitiveRepeatedMesh .. CMapFilenames) to the single sub-agent: 32/40 (`_hand55`; the NUISystem::CManager
rows (/Os, retail layout: definition manager +0x64, graphics bank +0x10, layer tree +0x70), the CMap grid getters over the landed
0081f090 model, CThingSwitch and C3DMeshMaterial::CFilenames constructors mislabelled CMapFilenames, CPatchTesselationEdgeStrip,
a CAIBrain method with a stack CBrainOutput; parked: the 00bfac70-style `push edi ; lea` hoist on the two mesh constructors, a
push-before-branches argument, a register choice, a reference-vs-value divide order, a retail null-`this` re-read, the wave-24b mask
narrowing, a `push edi` placement). By the coordinator: chunk 56 (CMapFilenames .. CMemoryDataInputStream: 39/40, `_hand56`; the
CMemoryAllocator / CMemoryAllocatorFixedSize / CMemoryAllocatorVariableSize bodies (sub-allocator sums under the allocator lock, the
fixed-size area chains, the variable-size chunk tree rotation and best-fit search, the aligned header arithmetic with the counters
read into locals), CMemoryAreaChunk::RemoveAreaFromFreeList, CSimpleMemoryPool::CMemoryBlock, CMatrix3x4::operator* (the named-local
copy-constructor return), CMapInfo / CMarkerInfo / CMapsAndRegionsDialog / CMemoryDataInputStream rows; parked: one register choice)
and chunk 57 (CMemoryDataInputStream .. CMessageEventFilter_CanBeSeenOrHeard: 24/40, `_hand57`; the memory stream / console line /
memory file constructors, the NUISystem::CScrollingViewport and CMesh constructors with three vtables, the mesh bank manager and
mesh data bank bodies, ~CMeshData, the by-value counted-pointer setter, the SGI median, the distance filter; fifteen parked on
register choice, branch layout, push hoisting, tag-slot sharing, an `int 3` after `_CxxThrowException`, store scheduling; one cold
block skipped). Catalog 29,574 -> 29,613 -> 29,645 -> **29,669 (59.9%)**. Lessons in GOTCHAS wave 24o. Next: chunk 58 (CMouseCursor ..
CNavQuadTreeNode) to the agent, chunk 59 by hand; `mkchunk.py 60 2` after (pool 2,440 rows).

### Wave 24, part 11 (chunks 52, 54)
Chunk 52 (CItem leftovers .. CLandscapeBackgroundLoadTask) to the single sub-agent: 37/40 (`_hand52`; CKeyboard / CMouseDX /
CKeyboardDX / CJoystickDX input bodies with label corrections (the "CJoystick" event rows are CKeyboard's at +0x3404, the
"UpdateAcquired" rows are CMouseDX / CKeyboardDX / CJoystickDX by slot, "~CJoystickDX" is a DirectSound-buffer Stop+Release), the
CLHFile GetSize/GetMode pair and the CLHFileManager factories, the eight "CLandscapeBackgroundLoadTask ??_G" rows that are a VC6 tree,
POD-vector pairs, delete[] buffers and pool records; parked: a destroy-range tag at the live flags slot, an unoptimised list insert
with three untouched frame dwords, a post-call load order). Chunk 54 (CLandscapeLayerMesh .. CLocalDetailPrimitiveMeshZSpriteBatch) by
the coordinator: 34/40 (`_hand54`; the landscape-memory block owners (OnPostRelocation / ~CLandscapeLayerMesh), the free-area merge,
the tesselator bodies, CLayer::SetAllPolys, the CLevelLoader / CLightningBranch / failure-handler constructors, the counted-pointer
destructors of CEventHitBase and CLHFile, the CLipSyncDataBank insert / VC6 erase-all / resize and CLipSyncPhonemeSample::operator=,
the NUISystem::CList predicates and AddChild variants, the cache-map close / update bodies, the collection-type IsEquivalent / copy /
PeekFadeEnd, the placement-grid releases; parked: a byte-wise copy constructor, three temporaries in one dead slot, two allocator
tags over a live slot, a `push edi ; lea` schedule, an argument load order). Catalog 29,503 -> 29,537 -> **29,574 (59.7%)**. Lessons in
GOTCHAS wave 24n. Next: chunk 55 (CLocalDetailPrimitiveRepeatedMesh .. CMapFilenames) to the agent, chunk 56 (CMapFilenames ..
CMemoryDataInputStream) by hand; `mkchunk.py 58 2` after (pool 2,520 rows).

### Wave 24, part 10 (chunks 50, 51, 53)
Chunk 50 (CInputManager .. CInputProcessManager) to the single sub-agent: 39/40 (`_hand50`; the retail CInputManager layout (0x80
bytes, CDiskFileWin32 at +0x60), event-scan vector bodies, the CInputProcess* Init / ProcessInput / AddToInterface rows, three
mislabelled destructors (~CTCEffectOnDie, ~CThingTrackNode); parked: IsMouseEventInQueue's register pairing). By the coordinator:
chunk 51 (CInputProcessManager .. CItem: 33/40, `_hand51`; the input-process manager list and destructor, two keyed no-RTTI
constructors, the input-process Update/Cleanup/ProcessInput bodies, the CIs* predicates (hopscotch board, hit events, horn task,
projectile weapon, thing state counters, usable icicle point), the CItem def queries / constructor / RemoveItem / GetItemThings /
AddItem and the counted-record copies and assignments, the handle release/place bodies; one LTCG row skipped, six parked) and chunk 53
(CLandscapeBackgroundLoadTask .. CLandscapeGPUUpdateQueue: 38/40, `_hand53`; thirty deleting destructors with propagated labels
reproduced as non-polymorphic `delete` helpers, novtable-over-base classes and virtual destructors with factories, the landscape
memory-manager `operator delete`, the tree-node / split-branch bodies, nine "ClearTextures" bodies; parked: a register choice and a
`lea` schedule). Catalog 29,393 -> 29,426 -> 29,465 -> **29,503 (59.5%)**. Lessons in GOTCHAS wave 24m. Next: chunk 52 (CItem ..
CLandscapeBackgroundLoadTask) to the agent; `mkchunk.py 54 2` (pool 2,680 rows).

### Wave 24, part 9 (chunks 47, 48, 49)
Chunk 47 (CGroundNavigatorExternalInterface .. CHandleBasedMemoryPool) to the single sub-agent: 37/40 (`_hand47`; the navigator
peek accessors with hidden-pointer returns, the four std::list::clear bodies mislabelled "SortTreeRecursively", CTextEntry /
CGroupEntry::UnloadResource, CCombatRing::GetAttackerProngPosition, IntToWideString, the window-list inserts, a COM-style
`__stdcall` cancel; the retail CHandleBasedMemoryPool header is 32 bytes and its CArray 12; parked: the SGI find_if tag slot,
two def-pointer frame-word reuses). By the coordinator: chunk 48 (CHandleBasedMemoryPool .. CIME: 30/40, `_hand48`; the pool's
free-list insert/find/update, the handle chain, CHeightMap::SetSizeZAt, the CHeroExperienceDef / CHeroMarriageDef / CHeroMorphDef
constructors and the CDefPointeeBase destructors, the CHitLocation / CHitParameters destructors, SetPHitMaterialDef, CHoverable's
constructor and Update, CIME; parked: the six `IsDivineWrathStrike` std::find bodies (an empty tag temporary overlaid on the live
value slot), OnHovered's counted temporary, three vector-slot `lea` materialisations) and chunk 49 (CIPhysicsFacesSegmentBase ..
CInputManager: 38/40, `_hand49`; the collision-stamp vector, nine unrelated "CIcicleData" constructors, HasEnoughIcons, the
COM-style DrawText forwarder, CIndexBuffer's Direct3D create/lock, CInitBaseClass's constructor and deleting destructor, the
CInkeyBuffer ring, fifteen CInputEvent setters, two retail vector push_backs; one cold block skipped, one argument-order store
parked). Catalog 29,288 -> 29,318 -> 29,355 -> **29,393 (59.3%)**. Lessons in GOTCHAS wave 24l. Next: chunk 50 (CInputManager ..
CInputProcessManager) is with the agent; chunk 51 (CInputProcessManager .. CItem) by hand; `mkchunk.py 52 2` (pool 2,760 rows).

### Wave 24, part 8 (chunks 43, 44, 45, 46)
Chunk 43 (the CGameScriptInterface script entry points) to the single sub-agent: 38/40 (`_hand43`; every component
lookup matched with the landed OUT-PARAMETER `GetInterface(T** ppOut, long type)` template, which leaves the component in
eax like retail; label corrections for the mini-map, guild-master message, bullet-time, opinion-source and trap bodies;
parked: a `push esi` placement and the CDefPointer re-read). By the coordinator: chunk 44 (the same family with a
pointer-returning `Find()` model: 9/40, `_hand44` -- the README records the eax-load gap that chunk 43 then resolved;
the parked rows want re-authoring with the out-parameter form), chunk 45 (CGameScriptThing / CGameTimeManager
neighbours: 18/40, `_hand45`; `pack(4)` for `__int64` after a vptr, inline setters, `static __stdcall`, `unsigned short`
wide strings) and chunk 46 (CGameScriptThing .. CGroundNavigatorExternalInterface: 34/40, `_hand46`; the inline
`data->~T(); operator delete(data)` chain of CGraphicFrame::UnInitialise x3, the counted-pointer member object in
~CGameScriptThingManager, two non-polymorphic ~CGlobalDef bodies, the 64-bit UID list, the graphic bank list walkers,
the parsed item owning its CCharString in ReadFromParserAsSLONG x3, NotifyEntryUpdated with a volatile vector-first;
parked: IsTimeBetween's duplicated compare, PeekNavigatorRadius' hoisted `fld`, the UpdateAvailableMemory store/load
order x2, two by-value handle inserts). Correction: the 0x0122DEDC constant is 0.0f, not -1.0f -- the two landed rows that
read it (0x004348D0, 0x0089D340) and their tests/READMEs are fixed. Catalog 29,189 -> 29,216 -> 29,254 -> **29,288 (59.1%)**.
Lessons in GOTCHAS wave 24k. Hygiene item found: 620 files under `rebuild/src/compiled` from the old Windows waves
contain `__asm` / `__declspec(naked)` (grade `asm_bake`, e.g. `CGameScriptInterface_RemoveHeroHairstyle_0089b4d0.cpp`);
the lander rejects new ones, the old ones need a sweep (re-author or move to `rebuild/oracles/`). Next: chunk 47
(CGroundNavigatorExternalInterface .. CHandleBasedMemoryPool) is with the agent; chunks 48/49 are cut
(`mkchunk.py 48 2`, pool 2,840 rows); 48 by hand.

### Wave 24, part 7 (chunks 40, 41, 42)
Chunk 41 (CGameCameraManager .. CGameScriptInterface) to the single sub-agent: 35/40 (`_hand41`; the scripted-control
action constructors, CPSCSpline, CProcessedInput, CGameEvent::ReadData8 (a static `__fastcall` with an edx:eax return),
CMap theme camera heights, timer deregistration over an STLport tree; retail CGameCameraManager is 0x18 bytes narrower
than the PDB; parked: two constant-trip loops the DDK unrolls, a stack CDefPointer release, two load-order nits). Chunks 40
(CFileInstaller .. CGameCameraManager: 34/40, `_hand40`; the file-installer locks and buffer erase, the def
constructors with an intermediate novtable base, font bank entries / AcquireFont with the counted copy, CFontManager,
CFrameData, CFrameRateSmoother::AddFrameTime, the camera-manager message and frustum test; parked: two CRT `__ehvec`
SEH funclets, a loop register choice, a C3DVector return copy, a vector-first reload, a member-ctor schedule) and 42 (the
CGameScriptInterface entry points, 34/40, `_hand42`: the IsValid/GetThing script-thing shape over the retail CThing
bit-fields, the world-map/thing-manager/conversation forwarders, the player-mode switch table; parked: a `not cl` bool
test, a register choice, two argument-load orders, the fade colour split, a tail jump) by the coordinator. Lander fix:
`objdump -d -z` so switch tables keep their zero runs. Catalog 29,086 -> 29,120 -> 29,154 -> **29,189 (58.9%)**.
Lessons in GOTCHAS wave 24j. Next: chunk 43 is cut (`$S/w22/chunk43.tsv`, CGameScriptInterface); `mkchunk.py 44 2`
(pool ~3,080 rows).

### Wave 24, part 6 (chunks 38, 39)
Chunk 38 (CEngineVSConstantLayoutLightsBones .. CEnvironment) to the single sub-agent: 38/40 (`_hand38`; CObjectAugmentationsDef and
CEntitySoundDef-style def constructors, the libpk auto-map entry constructors, buffer-pair array destructors, a `__fastcall` delete
helper, the CAudioContext EAXSet/EAXGet wrappers, StartRaining/StartSnowing, CSoundBankLH / CMemoryFile constructors, libpk init
functions; retail CEngineWaterRenderer / CEngineWorldMap offsets differ from the Ego PDB by the 12-byte CArrays; parked: two def
constructors where retail schedules a member-constructor `lea` after a run of register-zero stores). Chunk 39 (CEnvironment ..
CFileInstaller) by the coordinator: 36/40 (`_hand39`; the NEntityEvents event constructors/destructors over CIntelligentPointer /
CIVCountedPointer / CCountedPointer members, CEventHitBase and CLHFile deleting destructors, the spell-effect interface setters and
frame update, theme-stack transitions, the three CFadeinSummoner::Init variants with a 16-byte CIsThingAlive filter, CFileInstaller::
Unpause over `volatile` members, CFileDataInputStream reads, the CDateAndTime / CFadeInFadeOutBase records, CExpressionDef's twelve
member objects; parked: a counted-pointer return without NRVO, a `dec [ecx+4] ; jne` release through an accessor, a register-choice
loop, a 12-byte fill scheduled across call-prep pushes). Catalog 29,012 -> 29,048 -> **29,086 (58.7%)**. Lessons in GOTCHAS wave 24i.
Next: `mkchunk.py 40 2` is cut (chunk 40 CFileInstaller .. CGameCameraManager by hand, chunk 41 to the agent; pool ~3,160 rows).

### Wave 24, part 5 (chunks 33, 36, 37)
Chunk 33 (CEngineInternalPrimitiveMeshBase .. CEngineLightingManager) to the single sub-agent: 37/40 (`_hand33`;
CIVCountedPointer / CCountedPointer releases and assignments, the particle-group primitives over the retail layout,
CParticleSimulation's destructor pair, owner delete loops, a `__fastcall` allocator free; parked: a compare-chain register
choice and the inline by-value getter's frame temporary). Chunks 36 (CEngineResourceManager ..
CEngineScreenEffectOutlineGlow: 39/40, `_hand36`) and 37 (CEngineScreenEffectOutlineGlow .. CEngineTreeFilerBase: 33/40,
`_hand37`) by the coordinator: the free-buffer / list-owner / vector-of-objects destructor families, NShape::CShape and
CBasicShape, CScreenFilterEntry, the engine scene's chain walkers, four list range-insert forwarders, the block-file
manager's set lookups and counted-pointer assignment, CEngineStateBlocks (ten 4-byte members), the sub-primitive
constructors (members in the novtable base before the derived vtable), three NON-POLYMORPHIC deleting destructors whose
polymorphic object at +0 is a member. The test harness compiles behaviour tests with the lane's cl.exe (not g++): the
VC7.1 `std::string` has no `clear()`. The funclet pass cut 20 candidates that all DIFFER 0vN (removed, as funclets39).
Parked with evidence (NOTES / READMEs): zero-register and `lea` scheduling, callee-saved scratch registers around a byte
test, `add esp,4` placement, address-taken-local register allocation, two deleting destructors without a vtable store
(one with a pooled delete called with `this` in ecx). Catalog 28,903 -> **29,012 (58.5%)**. Lessons in GOTCHAS waves
24g/24h. Next: `mkchunk.py 38 1` (pool ~3,280 rows), one chunk to the agent, one by hand.

### Wave 25, part 1 (Opus authoring fleet; Fable 5.1 landing session; chunks 58, 61–91, ledger chunk 901)
The owner ran this wave as a split: Opus 5.5 sub-agents (up to 16, later capped at 8, then 10, then 12) author the class-labelled
chunks in parallel in their own `$S/w22/agentNN/` directories (`mkprompt25.py` builds dir + brief; agents never touch the repo)
and stage them with `stage_pending.sh` into `rebuild/candidates/manual/pending/<batch>/`; the Fable 5.1 session re-grades and
lands each batch with `land_pending.sh` (`verify_and_land.py --land` under the retail flags + pragma sweep, tests under the lane's
cl.exe), keeps the parked sources under `pending/<batch>/differ/`, and regenerates `docs/pipeline/FABLE_LANDING_QUEUE.md`
(`queue_status.py`). A mid-wave pause (owner) and an Opus usage limit cost nothing: every agent kept `PROGRESS.md` and the
relaunched agents resumed from it without redoing matched rows. Landed: chunk 58 (34/40) and 61–91 (35, 37, 40, 38, 35, 36, 37,
36, 37, 38, 37, 36, 38, 35, 34, 33, 34, 38, 37, 40, 36, 33, 32, 36, 31, 34, 36, 37, 34, 35, 34 of 40) -- 1,113 rows, zero lander
refusals. Rows an agent could not author as ONE function are no longer lost: `skipped_ledger.py` keeps them in
`rebuild/candidates/manual/SKIPPED_ROWS.tsv` (kinds multi / prefix / cold / ltcg) and `to-chunk NN` turns the multi/prefix pieces
into an ordinary chunk; chunk 901 (34 bodies from 13 multi rows: CPersistContext_TransferableSubComponent slots 0–2, CTCInventory
ProcessButtonB/XReleased, the CTCDoor/CTCChest slot-10/23 pairs, CTCCreatureModeManager's list walkers, CScriptThing operator==/!=,
CSoundReceiverLH slots 2/4, the STLport `<cmath>` float overloads atan2/log10/pow, a block-header thunk, `Sleep(interval/10000)`)
was hand-authored by the coordinator, 34/34 -- but 21 pieces were refused on the first landing as OUTSIDE_MANIFEST (the manifest
knew only the original rows); their starts are now manifest rows with evidence and `to-chunk` adds them itself (GOTCHAS wave 25).
Catalog 29,732 -> **30,909 (62.4%)**. Open: ~124 parked rows across the pending batches (register colouring, the push_back
empty-tag slot, post-call re-reads, `fistp` asm_bake helpers, LTCG conventions), 6 ledger rows (2 ltcg, 2 cold, 2 ~2 KB prefix
functions), the review items in each batch's LESSONS.md (local models beside headers whose PDB offsets miss retail's 12-byte
containers; wrong propagated labels listed in NOTES.md). In flight: chunks 92–103 (12 agents); 104–108 prepared; pool ~1,040.

### Wave 25, part 2 (chunks 92–111, ledger chunks 902–904, retries 1–3, the 97–160-byte pool)
Fleet rules changed twice at the owner's request (12 Opus agents, then "no launches until fewer than 7 run and hold 7",
then "hold 6 Opus plus one Fable 5.1 agent at medium effort"); the landing session keeps the count by launching only when a
report arrives. Landed: chunks 92–111 (38, 39, 39, 39, 38, 38, 32, 37, 36, 33, 33, 38, 36, 37, 39, 38, 32, 32, 36, 36 of 40)
and the ledger chunks 902 (3: the CTCPerceiveThing pieces; the two ~2 KB RenderPrimitive bodies inline the engine's assembly
float-to-long rounding and are parked as asm_bake shapes with full hand-over notes), 903 (8 of 8: the six CTCTargetingSpirit
vtable bodies of one 48-byte row, CTCInventoryQuests::ProcessInventoryInputUp/Down) and 904 (8 of 9: three propagated
"CThingFilter_IsValid" rows holding slot forwarders, a six-argument record constructor, a word getter, the "MPRT" descriptor
getter and three `return this` accessors; the __stdcall COM forwarder thunk is parked on an argument-slot re-store).
Retries by the landing session: every parked differ source was re-graded as-is and under four pragma variants (1 win: a
sibling-proven form the author left ungraded); the STLport subscript `*(begin() + n)` flipped chunk 91's three CTCInventory
element-access rows; declaring the search cursor before its limit flipped CTCInventoryAbilities::GetButtonAbilityIsSetTo.
Ten forms of the bilinear interpolation (one load placement), ResetTrap, GetCardDef and the def-pointer release rows
(00663260, 0082d870: the hand-written pointer-local release still splits when the pointer is tested right after the load)
stay parked. The class-labelled 17–96-byte pool is spent (80 rows); `mkchunk.py N K LO HI` now cuts a size band and chunks
117–124 come from the 97–160-byte class pool (2,510 rows, then 1,971 at 161–256). Recurring agent lessons this part: VC7.1
uses the register-clobber facts of callees compiled earlier in the same TU (so "LTCG register knowledge" parks are worth
retrying with the callee body above the function); the STLport `_Vector_base` + `_STLP_alloc_proxy` model keeps fixing
"24i" scheduling shapes; `optimize("s")` markers (`or byte ptr [m],imm`, `fld ; push ecx ; fstp [esp]`, shared zero
registers, `lea` address formation); a virtual first declared by a secondary base compiles with `this` at that subobject;
overloaded virtuals sit in reverse declaration order. Catalog 30,909 -> **31,659 (63.9%)**; ledger open: 2 ltcg, 2 cold,
1 single EH tail, the 2 asm-rounding prefix bodies and the COM thunk.

### Wave 25, part 3 (Fable 5.1 session: chunks 113–114, ledger chunk 905, duplicate-source hygiene)

- Landed chunks 113 (38/40: 00c9cb40 parked, LTCG register convention; 00837040 two-function row → ledger) and
  114 (37/40: 00406d90 loop rotation, 00be24d0/00be2540 dead 16-byte stack temporary). Catalog 31,774 (64.1%).
- Ledger chunk 905: row 00837040 split into CThingTrackNode::DrawIsDrawable (vtable 0x01272CE4 slot 27, tail jump
  to CDisplayEngine slot 9) and a dead getter at 0x837050 reading the packed +0xe6 draw record shared with 0x837060.
  Both matched first grade. Ledger: 00837040 done; still open 004a1c8e (EH tail), 005c150b/00c8a520 (ltcg),
  007fa26c/00a446fa (cold), 00ba1140/00bd71b0 (asm rounding, in 902 differ), 00caef70 (in 904 differ).
- Chunk 114 re-authored 0040d2a0 and 0040d400, already landed; the lander wrote a second source for 0040d2a0 under
  a different name (it keys on file name, not address). A repo scan found 23 more addresses with two sources (most
  from the 2026-09-07 import); kept the file whose name matches the catalog row, removed the other and its test.
  Coverage is per catalog address and was never inflated.
- Fleet rule (owner): 5 Opus agents + 1 Fable 5.1 medium agent. In flight: 115 (Fable), 116–120 (Opus); prepared 121–124.
- Review items from chunk 114 LESSONS.md not yet acted on: 0x0040B6D0 is the RefreshRate setter and 0x0040B750 the
  VerticalSyncEnabled setter (landed under wrong labels, pad-based models); from chunk 113: CFileDataOutputStream.h
  +4 is a CCharString, CHandleBasedMemoryPool.h Name is at +0x448.

## Resume

1. Done in-container: all four batches graded and landed (118/122); the four DIFFERs are in
   `tools/decomp_pipeline/crawl/gen_tried.txt` and described in each batch README.
2. To regenerate the container-side artefacts: Ghidra project lives only in the container scratchpad
   (not committed); rebuild it with the headless import in `docs/BUILDING.md` and
   `LabelApply.java <tsv from functions.tsv>`.

### Wave 23, part 13 (EH pass 30, funclets 35)

Fifteen more bodies landed byte-exact (`2026-10-14_linux_eh30/`, commit `0f82791c`): the two
auto-delete-array `Resize` twins, the buffered-reader `Seek`, the two checked `At` twins, the
mailbox `Dispatch`, the map-who `CreateInstance`, the `__fastcall` bitset describer that returns a
string by value, the audio thread-owner `Attach` / `WaitForAsyncCompletion`, the animation-table
`GetMaxDamageInAnimation`, the `~CCreatureActionHeroJump` and `~CTCKraken` destructors, the
master-object `Finalize` (`delete this`) and the hero `ReleaseTarget` sweep, plus the 31 funclets of
those parents (`_funclets35/`). Catalog 28,387 → **28,433 (57.4%)**. Ten near misses parked with
evidence in `$S/w23/parked_notes.txt`: the DDK hoists loads/stores past `push`es (0x00C14E40,
0x00CAB370/0x00CAC070), colours registers differently (0x00C11AC0, 0x00C88780, 0x00C0AD10 needed
the loop index declared before the string), lays the return-0 exit inline (0x00C11230), interleaves
64-bit loads (0x00CAF8F0), uses immediates where retail keeps a zero register (0x00C48E00), and
never shares a frame slot between sibling-scope locals (0x00C1A870, 0x00C1A110 — retail does).
The open EH set of the module is down to ~48 bodies of 298+ bytes.

### Wave 23, part 14 (EH pass 31, funclets 36)

Ten more bodies landed byte-exact (`2026-10-14_linux_eh31/` + `_eh31_b/`, commit `2fac7a40`):
the playback `LogStatus` (string returned by value), the `CTCKraken` constructor, the audio-bank
`LoadAsync` (COM smart pointer), the `CFileManager` constructor and destructor, the
instance-manager `Assign`, the `CAbilitySoundComponent` constructor (sixteen slots, named
strings from temporaries), the `CSBAgents` constructor, the `~CCodaFrameStreamer` destructor and
the animation-group initialiser, plus 40 funclets (`_funclets36/`). Catalog 28,433 → **28,483
(57.5%)**. Eight near misses parked with evidence: register colouring and lazy saves
(0x00CA6450, 0x00CA68F0, 0x00C131C0, 0x00C42DD0), retail's tail-merged epilogues (0x00CA67B0),
the `/GS` lane gap (0x00CAB160, 0x00C3BB50: the lane compiler copies or caches pointer
arguments where retail re-reads the argument slots) and the array-new zero-count special case
(0x00C3F800). The open EH set of the module is down to ~23 bodies of 390+ bytes.

### Wave 23, part 15 (EH pass 32, funclets 37)
Landed `2026-10-14_linux_eh32` (8 bodies: 00c3ee10 CreateNavigationLayer, 00c16310 LoadScript, 00c38b20 ~CActivationReceptor,
the four CEventSubscription ctors 00c23c10 / 00c23e00 / 00c24050 / 00c239b0, 00c4c8b0 LoadScriptInfo) and `_funclets37` (25 of 45).
Catalog 28,483 -> 28,516 (57.5%). The t32 builder/tests were produced by the session's single sub-agent from the matched scratch sources.
Lessons: the CProfileLock is 8 bytes (section + tick) -- a 4-byte model shrinks the frame by a word (c4c8b0); `new T[n]` twins derive from
the malloc template by swapping the entry allocation only. Parked with evidence (`$S/w23/parked_notes.txt`): 00c4cab0 (vtable caching),
00caf230 (DirectShow-style CUnknown/CBaseDispatch ctor: the lane folds the inlined `if (pptinfo == NULL)` on `&local`, retail keeps it),
00c11d00 (CTCHero ctor, store scheduling), 00c1a9c0 (async read thread: lane rotates the loop and shrink-wraps). With these the
0x00C0xxxx EH set is exhausted: every remaining open EH body is parked on a systematic lane-vs-retail codegen gap for the Windows QFE compiler.

### Wave 24, part 1 (hand chunks 24–25, lane tooling saved)
The 0x00C0xxxx EH set being exhausted, the delegated chunk pool resumed under the one-agent rule: chunk 24 (40 CCreatureAction_*
rows) went to the single sub-agent (40/40 matched, `_hand24`), chunk 25 was authored by the coordinator in parallel (36/40,
`_hand25` + `_hand25_b`; 008c6160 / 007a9b10 / 00528930 / 004c41d0 parked with evidence in the README). Catalog 28,516 -> 28,592
(57.7%). Seven "CCreatureAction_*::Clone" rows turned out to be Dinkumware tree-node factories (value at +0x10) and are landed as
`CRetailTree_<addr>::BuyNode`. The scratch-only lane tooling (grader, disassembler, EH helpers, chunk brief/cutter/lander, EH-pass
builder example, parked list) is now in `tools/decomp_pipeline/linux_lane/lane_tools/` so a fresh container can resume; the chunk
cutter trims the int3 padding the family rows carry (the lander compares whole rows). Lessons in GOTCHAS (wave 24a).

### Wave 24, part 2 (hand chunks 26–27)
Fresh chat, same container: the previous scratch lane (game, DDK compiler, Wine prefix, pools) survived under the old
scratchpad directory and was reused as `$S`; the only bootstrap cost was priming Wine (`wine cmd /c exit` with null stdio) so the
service processes do not inherit the grader's pipe. Chunk 26 (40 rows, CCreatureAction_* Sheathe.. WhisperSommersaultBackward)
went to the single sub-agent: 40/40 matched (`_hand26`), with six relabels (BanditKingUnstuck / VillagerAttackKnockdown /
WaspQueenDie constructors, a list BuyNode, an STLport list Insert, a free `__stdcall` setter). Chunk 27 (CCreatureAction_
WhisperSommersaultForward .. CDateAndTime: the CDateAndTime family, the CDataOutputStream writers, data-bank lookups, stream
constructors, CDBMinion, CCriticalSectionLock) was authored by the coordinator: 35/40 (`_hand27` + `_hand27_b`). Catalog
28,592 -> **28,667 (57.8%)**. Parked with evidence (`$S/w22/agent27/NOTES.md`, best sources in `differ/`): 00bf3250 (1.0f
register stores sunk past the zero stores), 00c59720 (mask narrowing), 00995a20 / 00995a80 (null-arm block placement),
00e5dde0 (argument load scheduling). `funclets.py` found 20 new EH parents but the lane emitted no funclet for any of them
(DIFFER 0vN) -- the batch was discarded; the agent's EH-prologue constructors want a look on the Windows box. Lessons in
GOTCHAS (wave 24b). Next cut: `mkchunk.py 28 2` (pool ~3,640 rows).

### Wave 24, part 3 (hand chunks 28–31)
Chunk 28 (CDateAndTime .. CDisplayManager) to the single sub-agent: 38/40 (`_hand28`; SSE constructors with a by-value `__m128i`
in xmm0 under `/GS`, SGI `vector::erase`, `_Uninit_fill_n` over packed def classes, retail-tree `Contains`; 00a31220 / 009ace90
parked on register allocation / tail placement). Chunks 29 (CDisplayManager .. CDrawMiniMap: 34/40, `_hand29`) and 31 (CEditBox ..
CEngine: 32/40, `_hand31`) by the coordinator. Catalog 28,701 -> 28,739 -> **28,771 (58.1%)**. Parked with evidence in the
agents' NOTES.md and the batch READMEs: whole-program `this` retention across a call (00c20cb0), a bare `fistp` rounding (an
inline-asm helper, 006472b2), a stack-imbalanced retail body (00c2d260), `fld x ; fdivr 1.0` operand order (006d63a0),
zero-register / copy-register allocation (0049ba70, 007b52b0, 00b25d20/50), `push` vs `mov ecx` order (00540120), the eax zero
register with `mov al` returns (00643e09), 8 dead frame bytes (00c437c0), `mov ecx,ebx` scheduling (00644334/006443ac). Lessons
in GOTCHAS (waves 24c, 24d). Next cut: chunk 30 is cut (`$S/w22/chunk30.tsv`, CDrawMiniMap .. CDynamicObjectHeapBase, for the
next agent); then `mkchunk.py 32 2` (pool ~3,480 rows).

