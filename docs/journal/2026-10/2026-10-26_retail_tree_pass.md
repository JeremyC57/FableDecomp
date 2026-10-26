# CRetailTree family pass (treepass), 2026-10-26

Scope: replace invented red-black-tree stand-ins in landed code with the real STLport 4.6 `std::_Rb_tree`
(and `std::map`/`std::set` where retail used them), instantiated with each instance's proven key/value types.
Worktree: `repo/` (base ea46ef831). Scripts: `gen/`. Nothing in /home/user/FableDecomp touched; no git run.

## Result

**388 landed rows converted.** Every row regraded MATCH or RELOCATION_MATCH with its behaviour test PASS
through `recheck.py` + `vland --recheck-landed`: 75 MATCH, 313 RELOCATION_MATCH, 0 failures.

| Stage | Rows | What |
|---|---|---|
| s1 | 363 | Components with no landed callers outside the family and no outside callees |
| s2s | 17 (+3 re-emitted) | Unproven instances whose values hold CCharString/CWideString members, proven by the erase bodies' destructor calls |
| p1 | 8 rows + 2 caller edits | Pilot for the caller-edit recipe: `map<ETargetingType, float>` (CTargetingDef::TargetingRanges) |

- **leafscan:** 388/388. The `// Leaf:` symbol is present in each object, and no Leaf is shared by two
  addresses.
- **Bindings:** -63 / +33 rows. After the change, no symbol binds two targets.
  - Every symbol bound to a converted address is that row's Leaf.
  - No converted row calls a landed row under a different name (`XNAME` = 0).
- **Purity:** no `__asm` / `naked` / `_emit` / `volatile` / `_STL` / `windows.h` in any added or changed file.
- **Not converted:** 882 inventory rows, listed with reasons in `KEPT.tsv`.

### Counts per op (converted)

| Op | Rows |
|---|---|
| CTOR8 (`_Rb_tree(const _Compare&, const allocator_type&)`) | 108 |
| CTOR0 (container default ctor) | 108 |
| FIND (`find`, `_M_find` inlined) | 30 |
| DIST (`__distance(first, last, input_iterator_tag)`) | 21 |
| CLEAR (`clear()`; 41-byte /Os and 51-byte speed forms) | 16 |
| DTOR66/tree (`~_Rb_tree`) | 16 (one /Os) |
| MERASE (`_M_erase`) | 16 |
| DTOR66/cont (implicit container dtor) | 15 |
| DEC / INC (iterator `operator--` / `operator++`) | 13 / 11 |
| ERASEIT (`erase(iterator)`) | 9 |
| CREATE (`_M_create_node`) | 7 |
| IUNIQUE (`insert_unique`) | 6 |
| MINSERT (`_M_insert`) | 6 |
| ERASERANGE (`erase(first, last)`) | 4 |
| ASSIGN (`operator=`) | 1 |
| MCOPY (`_M_copy`) | 1 |

TU variants: 371 speed, 8 /Os, 9 /Os + `optimize("y", off)`.

### Type decisions (per instantiation; full table in `EVIDENCE.tsv`)

- **Proven:** 32 instantiations, 48 rows. Every one has at least 2 sources:
  - a PDB owner member (e.g. `CTargetingDef::TargetingRanges`, `CBankFileManager::Banks`,
    `CConsole::KeyBindingMap`);
  - plus a node-size check and a compare-signedness check, or a destructor-callee check (`~CCharString`).
  - Example: `map<ETargetingType, float>`, `set<CCharString>`, `map<CCharString, CBankFileManager::CBankInfo>`,
    `map<unsigned __int64, CThingCreatureBase*>`.
- **Unproven:** 221 instantiations, 320 rows. These use the placeholders
  `CUnprovenKey_/CUnprovenValue_/CUnprovenKeyOfValue_<inst>`, where `<inst>` is the lowest member address.
  - The key is `long` or `unsigned long` depending on the compare branches (jl/jge vs jb/jae).
  - The value size is node size − 0x10.
- **Unproven with proven members:** 12 instantiations, 20 rows. Only the value members that the erase
  bodies destroy are typed (`lea ecx,[node+0x10+off]; call ~CCharString`/`~CWideString`). The rest are
  `unsigned char` gaps.
- **Unproven container classes:** `CUnprovenContainer_<inst>`.
- **Unproven iterator traits:** `CUnprovenTraits_<row>`.
  - `_Nonconst_traits` is used only when the iterator row is called from `insert_unique` or
    `erase(first, last)`.

## Decisions

1. **One placeholder set per instantiation.** Every row of an instantiation shares the same definitions
   (ODR-consistent across TUs).
   - The three stage-1 container constructors of instantiations that stage s2s later typed were re-emitted
     with the member-typed placeholders: 009ac0f0, 0056ddc0, 006f3090.
2. **Destructor identity:**
   - Speed TUs: adjacency to the retail tree constructor (`C8`, ret 8 → `~_Rb_tree`) or to the container
     default constructor (`C0` → `~container`) (`gen/dtorid.py`).
   - /Os, proven by the compiler: an /Os `~map` compiles to a 5-byte `jmp`, so a 23-byte body that calls
     `clear()` and frees the header is `~_Rb_tree`. Used for 00451940.
   - When the role is unknown, the whole component is held.
3. **One address = one name.** Components are converted whole. These are held:
   - two bodies that grade to the same symbol (e.g. `_Rb_tree::clear` vs `map::clear`; 21 rows);
   - a component whose Leaf is already landed elsewhere.
4. **Retail op mapping:**
   - ERASEIT rows other than the 66-byte form are `_M_erase` with value destruction. Exception: rows that
     call `_Rb_tree_rebalance_for_erase` (0x42a34e) are `erase(iterator)`.
   - DIST rows are `__distance`, not `distance`.
   - INC/DEC rows inline `_Rb_global<bool>::_M_increment/_M_decrement`.
5. **Model:** real STLport 4.6 `stl/_tree.h` + `stl/_tree.c` text in `namespace std`. Explicit member
   instantiation emits each member as its own COMDAT.
   - /Os TUs need `__forceinline` on `_Construct` / `_M_clone_node`.
   - Speed `insert_unique` needs a same-TU noinline iterator `operator--`.
6. **Names:**
   - Catalog column 2 is the `llvm-undname` form of the Leaf, e.g.
     `std::_Rb_tree<long, std::pair<long const, long>, ...>::find`.
   - ps1 Module = the full class.
   - File base = `make_base(module, method, addr)`.
   - Line 3 of each source says which stand-in it replaces (`was <old catalog name>`).

## Caller-edit recipe (pilot p1, verified)

A converted row's landed callers must declare its exact symbol. Recipe:

1. In the caller TU, drop the local stand-in struct.
2. Prepend `gen/caller.py:prelude()`: the proven types plus the STLport model plus containers, but without
   the out-of-line `stl/_tree.c` bodies, which stay declared only.
3. Type the member as the real container.

Results:
- `CTargetingDef::operator=` (0045d729): still RELOCATION_MATCH, and now binds
  `??4?$_Rb_tree@W4ETargetingType@...` → 0x0045D7FE.
- `CTargetingDef` scalar deleting dtor (0045470e): uses `member.~map()` (inlined to `~_Rb_tree` @ 0x00451940).
- The pilot also converted a mislabelled `_M_erase` (004507ce, was `LTextTreeWalkThrough::BuildTreeArray`)
  and the /Os `~_Rb_tree` (00451940, was `_global::StdList_Dtor_00451940`), so that every edge is consistently
  named.

## Kept (`KEPT.tsv`, 882 rows, with reason and component id)

| Rows | Reason |
|---|---|
| 556 | **Caller edits needed.** A landed caller outside the family declares a stand-in name. 375 distinct callers: def-class `operator=`, scalar deleting destructors, atexit destructors of global maps, `GFSerialiseMapBinaryIn`, `map::operator[]`, forwarders. Converting these components means applying the p1 recipe to every caller (bespoke per caller). |
| 163 | **Outside callees.** Value types or helpers outside the family (`CBaseIntelligentPointer`, `_M_erase` of nested containers, non-landed helpers). |
| 64 | **User code shaped (OTHER / SINIT / SEXIT).** Not `_Rb_tree` members. |
| 41 | **Held in-family.** 21 duplicate `clear` bodies (role ambiguous), 9 unknown dtor roles, 8 MERASE failures (value members freed through owned pointers), 6 user-class dtors, 5 CTOR0 type skips (doubly nested types), 1 CTOR4, 1 Leaf landed elsewhere. |
| 33 | **Unsupported op shapes.** |
| 25 | **Outside any reference component.** |

## Hidden family members (`OS_FAMILY.tsv`; landed, misnamed, not in the stand-in inventory)

These are retail /Os tree code under invented names (`ListNode_AllocateNode*`, `ListNode_Init*`,
`TextureFormat_Handler_*`, `_global::StdList_Dtor_*`):

- 62 `_Rb_tree_base(const allocator_type&)`: 22 bytes, mallocs the header, node size in the push.
- 62 /Os `_Rb_tree(const _Compare&, const allocator_type&)`: 42 bytes.
- 52 /Os `~_Rb_tree`: 23 bytes, of which 1 was converted in p1.
- 26 look-alike /Os `~list`.

They block many usercaller components. Next step: add them to the inventory and solve them with the same
model.

## Header debt

Converted TUs declare these locally, from tdecl and PDB layouts:

- CCharString / CWideString (copy constructor + destructor + PStringData);
- enums: ETargetingType, EOpinion*, EGameAction, ...;
- C3DVector, CDateAndTime, CObjectAugmentationSoundSet, CBankFileManager::CBankInfo,
  CNavigatorAStar::CAStarState, NThingManager::CEntityRuntimePersistence;
- nested-class shells (`class Outer { public: class Inner; };`).

None of them has a `rebuild/include/engine/<Class>.h` yet. They become includes when those headers exist.
The two edited callers keep their pre-existing local `CDefClassBase` (vland warns SHARED_HEADER_AVAILABLE;
this warning was already there before the edit).

## Files

- **Change records:** `RENAMES.tsv` (388), `CHANGED_FILES.txt` (776 D / 776 A / 5 M), `CALLERS.tsv` (2 edited
  callers).
- **Bindings:** `BINDINGS_DROP.tsv` (63), `BINDINGS_ADD.tsv` (33).
- **Worklists:** `EVIDENCE.tsv`, `KEPT.tsv`, `OS_FAMILY.tsv`.
- **Labels:** `LABELS.tsv` is empty. Every relocation target of a converted row is a landed catalog row, so
  there are no proven non-landed callees and no `ghidra_out/labels_treepass_2026-10-26.tsv` was written.
- **Regrade logs:** `logs/regrade_{s1,s2s,p1,p1c,fix2,p1all}.out.log`.

## Proposals for the coordinator

**GOTCHAS one-liner:**

> STLport trees: an /Os `~map` compiles to a 5-byte `jmp ~_Rb_tree`, so a 23-byte `clear(); free(header)`
> body is `~_Rb_tree`. Two identical speed `clear` bodies of one instantiation (`_Rb_tree::clear` vs
> `map::clear`) cannot be told apart from bytes: hold both. Unproven placeholder types must be defined
> identically in every TU of the instantiation.

**FINDINGS_LOG paragraph:**

> 2026-10-26 treepass. 388 landed rows that carried invented red-black-tree stand-ins (`CRetailTree*`,
> `CTreeOwner_*`, `std::_Rb_tree<T_node0x..>`, `_global::RbTree_Clear_*`) are now real STLport 4.6
> `std::_Rb_tree` / `map` / `set` members. 75 are MATCH and 313 RELOCATION_MATCH, all with behaviour PASS.
>
> - **Proven types:** 32 instantiations, from PDB owner members plus node-size, compare and destructor
>   checks.
> - **Unproven:** 233 instantiations use per-instantiation `CUnproven*_<addr>` placeholders. 12 of them
>   have value members typed from destructor calls.
>
> 882 rows remain, mostly because 375 landed callers outside the family still declare stand-in names.
> The caller-edit recipe was verified on `CTargetingDef::operator=` and its scalar deleting destructor.
> The pass also found about 180 misnamed /Os tree functions (`_Rb_tree_base` ctor, /Os
> `_Rb_tree(cmp, alloc)`, /Os `~_Rb_tree`) under list or texture names (`OS_FAMILY.tsv`).
