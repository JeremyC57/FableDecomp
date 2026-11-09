# Landing 2026-11-09f: seven quick-win rows (chunk 893's three matches + four parked near misses)

The user asked for the ten easiest rows that could be finished in about an hour, byte-matching, behaving as retail and
portable. Seven were finished in that time: 4 RELOCATION_MATCH + 3 MATCH, every behaviour test PASS under VC7.1 (vland `/Od`)
and under g++ `-std=c++03`; no `__asm` / `naked` / `_emit` / `volatile`, no bare `__forceinline`, no flags, no pragmas.

| Address | Row | Grade | Lever |
|---|---|---|---|
| 0x00C73830 | CRegistryKey::QueryDWord | RELOCATION_MATCH | chunk 893's duplicate-return rule: an else-if chain whose arms all return keeps the FIRST `return 0` block inline |
| 0x0085FBC0 | NCreatureMode::IsAbleToBeEngagedInCombat | RELOCATION_MATCH | same rule, chain ending in `else { ...; return x; }` |
| 0x009D06F0 | CBankFile::OpenReadOnly | RELOCATION_MATCH | same rule, the dev-path tail as the final `else { ...; return ...; }` arm |
| 0x00C6DA40 | MixGainRampUp | MATCH | landed twin 0x00C6E790's spelling (chunk 892): the accumulator indexed through a copy of the loop counter |
| 0x00C6DAD0 | MixGainRampDown | MATCH | landed twin 0x00C6E820's spelling, same lever |
| 0x00BBA3E0 | CEngineInternalPrimitiveDecal::GetBoundingSphereWorldSpace | MATCH | `sqrtf((x * x) + (y * y))`: the two PAREN tuples move `faddp` / `fsqrt` one scheduler slot earlier (playbook); now on the shared C3DVector / C3DBoundingSphere / C2DVector headers |
| 0x00807170 | std::__median<pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting> | RELOCATION_MATCH | the shared CVectorCompare_BestForTargeting header with `_BODIES` (inline COMDAT scorers, as the landed 0x00808AA0 / 0x00807250 of this TU): c2 knows the comparator is not captured |

Sources: the three chunk-893 matches from `pending/2026-11-09_linux_hand893/matched/` (line 2 rewritten from the stale DIFFER note to
the lever); the other four rewritten from their parked drafts (`pending/2026-10-24_linux_hand443/`, `2026-10-27_linux_hand519/`,
`2026-10-24_linux_hand398/`). The tests are new (C++03 models of each row's logic with stub callees).

Tried and left parked this session (each 5-13 variants, none moved the residue):
- 0x009E1960 / 0x009E1A80 GFStringToInt (narrow / wide): retail does not thread the zero-length guard past the sign test; for,
  while, do-while guard, strlen in the condition, `return -result`, ternary, `negative` as char -- all thread (1 line).
- 0x00A17620 CPhysicsMesh::HasBoundingSphereIntersectedFromTo: the commutative `fld [ecx+80h] ; fadd [esp+10h]` order; every
  operand order, named locals, parentheses and a sphere reference keep ours (2 lines).
- 0x00C720C0 CAudioSource::StartExponentialFade: x87 compare operand order; `0.0f != x`, `==` with swapped arms, `!(x == 0)` (2 lines).
- 0x00D259A0 CDragon::OnPredicateFail: parent pointer in eax v retail's esi; rebased on the sibling 0x00DE1F10's shared headers,
  reference / pointer locals, a GetParent accessor, a const-ref setter, an enumerator list, a named bool (2 lines).
- Not tried for time: 0x0079CF30 (old local model needs a shared-header rewrite), 0x00619560 (invented CRetailVector stand-in and a
  bare `__forceinline`: needs a rewrite on the shared STLport vector), 0x00C11520 (CNavEvent ctor 0x00C20390 bound under three
  stand-in names; settle the name first).

Warnings at landing (non-blocking):
- STANDIN_LEAF x5: the catalog still held the old descriptive labels; `ghidra_out/labels_landing_2026-11-09f.tsv` applied the new
  names (catalog + manifest for these seven addresses only; `sync_catalog_names.py`'s other 4,286 renames were not kept, per HANDOFF).
- SHARED_HEADER_AVAILABLE on 0x0085FBC0 (CThing, CTCCreatureModeManager, CCreatureActionBase, CDefString, CDefClassBase) and
  0x009D06F0 (CDataInputStream, CAFile): the chunk-893 sources keep their local models; rebasing them on engine/ headers is header
  debt for a later wave.
- RELOC_NAME 0x009A9D60: this row binds the PDB `CCountedPointer<NBankFileManager::CRetailBank>` destructor; an older landed binder
  spells `CCountedPointer<CRetailBank>` (struct, no namespace). Queued `land0911f` (PRIORITY) next to the existing `c678` line.
- `bindings_multi_check.py` prints 1 (CScriptThing::NullCoord over three addresses), unchanged from HEAD before this landing.
