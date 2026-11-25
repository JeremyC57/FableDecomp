# NF454 hand-back (2026-11-25): best-guess registration (owner-approved speed-up B1)

NF454 reviewed every line of the research dry run that passed `descriptive_check`: 3,672 `agree_unverified` addresses whose spelling is consistent everywhere but appears in no PDB name set. `NOTES.md` is its own notes.

| place | ACCEPT | PROVISIONAL | REJECT | FOUND_REAL | total |
|---|---|---|---|---|---|
| libpk | 746 | 0 | 168 | 54 | 968 |
| data | 756 | 0 | 138 | 96 | 990 |
| game | 366 | 115 | 490 | 447 | 1,418 |
| 0xCA0000-0xCD0000 | 3 | 0 | 62 | 231 | 296 |
| **all** | **1,871** | **115** | **858** | **828** | **3,672** |

## Registered: 1,821 lines in `rebuild/oracles/descriptive_names.tsv` (`REGISTER_ADD.tsv`)
- **1,653 ACCEPT lines.** The basis is `style` (light, readable libpk and data names in retail's style) or `one-source:caller-binding` (game names whose class is in a PDB set or RTTI).
- **137 libpk ACCEPT lines whose spelling is a GameCODA SDK 2.10 symbol.** These are re-based to `one-source:gamecoda-sdk`, checked against NF441's SDK dump of CODAApiDbg.lib and CODAApi.lib.
- **31 `REACTION_MANAGER_DEF_INSTANTIATION_NAME` lines** from FOUND_REGISTRABLE, basis `one-source:egor-pdb-initializer` (r454a).
- **Checks** against a fresh tracker record:
  - every line was `agree_unverified` with that one spelling;
  - no line shares its `?name@scope@@` with a name in any PDB set;
  - `descriptive_check` gives 2,011 lines, 0 FAIL;
  - the tracker diff is exactly 1,821 moves from `agree_unverified` to `agree_descriptive` and nothing else.

## Rulings (advisor decision, owner delegated)
- **Held: the GameCODA class-only hits.** The SDK has no symbol list in the repo, so NF454 could not check the CODA-block names. The coordinator split the 746 libpk lines against NF441's SDK dump: 137 are exact SDK symbols (registered above), and 81 belong to an SDK class but use a method name the SDK does not have.
  - The 81 are held in `acc_sdk_classonly.tsv` for an agent with the SDK dump, because a one-source SDK name ranks above a made-up one.
- **r454a, FOUND_REGISTRABLE (222).**
  - The 31 Ego_r-initializer lines are registered.
  - The 191 `one-source:vendor-object` lines are NOT registered. 166 of them equal the symbol `libsig` proved at that address. **Owner item:** should the tracker count a `?` name as proven when it equals `lib_symbol_at(address)`? That is A1's own proof standard, extended past `is_std`. It is a counting-rule change.
- **r454b, the 444 PDB same-name lines in FOUND_REAL: respell route, never the registry.**
  - A guess that a PDB name contradicts breaks the rule.
  - Do the destructors first.
  - Keep Ego_r / FableWin hits apart from Ego_d-only hits.
  - Each "candidate" needs a per-row retail argument-byte tie.
- **r454c, needs_vendor_source (190 lines in REJECT): not registered; owner item.** These are vorbisfile / Intel ivec / DXUT ImeUi / strmbase / zlib / CRT names that need the vendor source trees.
- **r454d, the 34 EH-funclet addresses at 0x0121xxxx that carry a function's name: not registered.** How the tracker classifies funclets is a counting question for the owner. It needs a measurement of how every funclet row is classified first.
- **ACCEPT_PROVISIONAL (115): held.** Their classes are in no PDB set and no RTTI, so a class may itself be an invented stand-in. Review them class by class.

## Queued
- **FOUND_REAL vendor respells:**
  - 217 strmbase BaseClasses;
  - about 90 relocation names in vendor objects;
  - 10 libvorbis smallft;
  - the `__real@` float literals landed as `g_zero` / `kMaxPitch`.

  These are in `FOUND_REAL.tsv`, waiting for a respell agent (T3 nf454 family line).
- **RENAME_PROPOSALS.tsv (119):** better descriptive names for rejected placeholders, for example CMatrix3x4 RotateB/A/C → RotateX/RotateY/RotateZ. These are row and caller respells, not registry lines.
