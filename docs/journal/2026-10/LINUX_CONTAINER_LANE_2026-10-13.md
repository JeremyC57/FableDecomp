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
Result: **53 of 71 real functions landed** (9 of the 80 picks are mis-trimmed fragments, skipped);
18 DIFFERs are ledgered, all instruction-order misses (`rebuild/candidates/manual/batches/2026-10-13_linux_b5/README.md`).

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

Coverage: catalog 18,631 → 18,749 → 18,802 byte-exact entries (37.6% → 37.9% of 49,568); the compile-gate
ledger still needs the canonical refresh on the Windows box to count them.

## Resume

1. Done in-container: all four batches graded and landed (118/122); the four DIFFERs are in
   `tools/decomp_pipeline/crawl/gen_tried.txt` and described in each batch README.
2. To regenerate the container-side artefacts: Ghidra project lives only in the container scratchpad
   (not committed); rebuild it with the headless import in `docs/BUILDING.md` and
   `LabelApply.java <tsv from functions.tsv>`.
