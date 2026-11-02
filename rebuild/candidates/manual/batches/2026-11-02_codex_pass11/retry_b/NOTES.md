# retry11b notes (Codex pass 11 rows, set b)

**Finished:** 3 rows (vland PASS) in `payload.json`.

The oracle for vland is `lv/oracle_vland.tsv` (scratch). It holds the 3 rows, with 0099dcb0 recut to 1455 bytes from the exe.

| Row | Grade |
|---|---|
| 0x0099DCB0 `_DisplayErrorDialog@8` | RELOCATION_MATCH, 1455 bytes, /GS |
| 0x00C0F440 `?GetFirst_00c0f440@CVtable_012b2308@@UAEPAXXZ` | MATCH, 8 bytes |
| 0x00631C60 `?Transfer@CUIDef@NUISystem@@UAEXAAVCPersistContext@@@Z` | RELOCATION_MATCH, 1524 bytes |

**Parked:** 19 rows (`differ/`). **Not authored:** 0x006286F0.

## Extent fix

**0x0099DCB0 is 1455 bytes, not 1454.** Byte 1455 (0x0099E25E) is the int3 VC7.1 emits after the no-return `exit()` call. 0x0099E25F is the single alignment byte before 0x0099E260.

## 0x00631C60 NUISystem::CUIDef::Transfer: blocker resolved

### Layout

- The retail offsets equal Ego_r's minus the def-base and STLport differences.
- PositionOffset sits at +0x142, so C2DVector must have alignment 1.
- Evidence for the alignment, from independent sources:
  1. Retail `lea [esi+142h]` and `lea [esi+146h]`, each feeding Transfer<float>.
  2. Ego_r layout TSV: CUIDef+0x15E and COldFileFormatThingBrushInfo+0x11.
  3. The packed bit in the C2DVector TPI records (Codex raw TPI).
- **Coordinator decision:**
  - `FABLE_RETAIL_C2DVECTOR` uses an authored `#pragma pack(push, 1)` on a Lionhead class (lib_coord.hpp), not an SDK boundary header.
  - This is the same device as the landed CDefClassBase / CParentDefClassBase / CTCOwnership packs, which also rest on byte evidence.
  - The 2026-11-02 owner note restricts pshpack1.h to SDK structs and says packing needs byte evidence; this pack has it.
  - Retail truth is that C2DVector is packed everywhere. It is opt-in only so that landed includers keep their layout.

### Calls and source shape

- Retail uses the one-argument member templates, the same as the landed 0x00431C30 CUIIconsDef row.
- Vectors go through `TransferVector<T>`, because both PDBs instantiate only `TransferVector<K/M/J>`, never `Transfer<vector>`. This renames three bound addresses (see NAME_FIXES).
- The call order is retail's order, not declaration order.
- TextValue is the only member whose default has an out-of-line constructor. Retail builds `CWideString()` in the dead `persist` slot around that one call.
- The block-scoped default local `{ CWideString defaultTextValue; persist.Transfer(TextValue); }` matches.
- Two comma-expression forms also match (v03, v04). The block form was chosen as the most natural source.

### Identity

Vtable 0x01259F8C, slot 18 is 0x00631C60; RTTI `.?AVCUIDef@NUISystem@@` (read from the exe). The Ego_r name is at 0x006758F0.

## 0x00C0F440 (libpk)

Descriptive names with the address suffix, because libpk has no PDB and no RTTI.

0x00C0F490, slot 4 of vtable 0x012B2308, is that class's deleting destructor. The manifest label CLandscapeBackgroundPatch is propagated and wrong.

## 0x006286F0 GFPlayFullScreenXBoxMovie: not authored

### Naming blocker: resolved by evidence

The function's only sound-manager use is through `PSoundManager` 0x013B8394 (already bound, `?PSoundManager@@3PAVCASoundManager@@A`):

- **Virtual slot 46 (+0xB8):** called as `(mask 8/0x10/0x40/0x80, volume, fade)`. Landed 0x0042E17C already names it `SetGroupVolume(unsigned long, float, float)` (0x00A3E9D0).
- **Three non-virtual bool setters:**
  - 0x00991600 (+0x24): landed binding `?SetEnabled@CSoundManagerLH@@QAEX_N@Z`.
  - 0x009915E0 (+0x25; when cleared, tail-calls slot 18 with 0): no proven name.
  - 0x00991620 (+0x26; on change, tail-calls slot 2): no proven name.
- Ego_r has CASoundManager only as a forward record (size 1, no members). Neither PDB name set contains a CASoundManager or CSoundManagerLH method.
- So the two unbound setters take descriptive names with the address suffix (for example `SetFlag25_009915e0` / `SetFlag26_00991620` on CSoundManagerLH). That is the documented rule, so the blocker is not a real block.
- The manifest labels for both (CWorld::SetMiniMapEnabled, CTCDParticleEmitter::EditFrameUpdate) are propagated and wrong.

### Why not authored

- The body is 1864 bytes with about 90 calls across CVideoSys, CXMVPlayerBuffers, CIEngine, D3D, the display manager and the CRT, and none of those classes has a shared header.
- Codex's drafts (movie_hdr) were never compiled.
- Writing those models is a header project of its own, so no source and no attempts were made.
- **Coordinator decision:** cut it as its own chunk after cons11b lands the engine models, or hand it to the CThing-family header agent's successor.

## Parked rows

Each row has at least 10 own probes (see `probe/` and `STATUS.tsv`).

**Graded on the landed repo headers plus this overlay (f01):**
- 008eec70: 1 scheduler tie, `mov eax,esp`.
- 0087aa10 / 0087aff0: x87 stack allocation in the refinement loop, same template.
- 00e137b0: cons11 wasp TU.

**Graded in Codex's selected context (header debt):**
- 0049124e, 00509a20, 0052f900, 006159a0, 006b97e0, 00762620, 00763060, 00803540, 00828710, 00875fb0, 00879070, 00a7a0a0, 00a8f1b0, 00accbf0, 00d39330.
- Five of them use Codex's banned second CCharString model or per-feature macros: 0049124e, 00762620, 006159a0, 00509a20 and 00763060. These must be rebased on fable_string.h and the landed models before any landing.

**Near misses (equal size, scheduler or x87 ties):**
- 008eec70 (2 lines)
- 00a8f1b0 (2 lines)
- 00803540 (2 lines)
- 00875fb0 (6 lines)
- 0087aa10 (8 lines)

**Lane gaps (inline budget):**
- 00d39330 and 00e137b0: implicit ScriptedThingBase `operator=` expanded by retail.
- 0052f900: CRGBFloatColour conversions.
- 00828710: CVectorMap::Insert.
- 006b97e0: GetDef / lower_bound.
- 00a7a0a0: CNavRegion copy.

## Names: collisions and second spellings (explicit)

- **0x0138E060** `?gApplicationLanguage@@3GA`, **0x013BD6EC** `_hStrings`, **0x013BD6E8** `?pConfig@@3PAVConfigDetection@@A`:
  - Row 0x0099DCB0 uses these spellings.
  - Each gives its address a second or third spelling next to the landed `?g_ModuleLanguage@@3GA` / `?g_LanguageID@@3GA`, `?g_hStringsDll@@...` and `?g_pConfigDetectionInfo@@...`.
  - They rest on one source only (Codex's raw PDB data-symbol reading).
  - Coordinator: pick one spelling per address before landing, and respell either the row or the landed callers.
- **0x00464841, 0x00464CBF, 0x00466A47:** row 0x00631C60 calls them `??$TransferVector@K/M/J@...`. That differs from the landed `??$Transfer@V?$vector@J...` for 0x00466A47 and from several bound spellings for the other two. The PDB evidence is in NAME_FIXES.tsv.

## Other

- `/gotchas_new.txt` (87 KB, at the filesystem root) was created by an early unguarded redirect in this session. Removing it was blocked by the safety check, so it needs manual removal.
