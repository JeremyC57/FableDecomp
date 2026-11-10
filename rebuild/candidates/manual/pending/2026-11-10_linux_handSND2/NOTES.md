# agentSND2 NOTES (2026-11-10)

**Every grade in this directory ran with `FABLE_HDR_OVERLAY=$A/hdr` exported** (`$A = $S/w22/agentSND2`). That covers `rg.sh`, `q.py` and the c2prio probes. `gall.sh` now exports it too.

The overlay holds three headers, each change behind a new opt-in: CASoundManager.h, CThingSoundEmitter.h and CSoundInit.h. `hdr/README.md` describes them.

## Result

| Job | Rows | Result |
|---|---|---|
| 1. Emitter self rows | 15 of 15 | All MATCH / RELOCATION_MATCH on the slot names from CASoundEmitter.h, on the shared headers. In `rows/` and `RENAMES.tsv`. |
| 2. Local sound-manager models | 79 of 81, in file order | All RELOCATION_MATCH on `engine/CASoundManager.h`. In `rows/`. |
| 3. Parked rows | 0 of 4 matched | All 4 in `differ/`, each with a measured residue. |

Job 2 skipped two rows:
- **0x009A6610** CSystemManager::Initialise. Its local `CP*` counted-pointer wrappers are invented. A coherent move needs `CCountedPointer<CASoundManager>::Reset` and the CSoundManagerLH ctor spelled together, which is header work outside this brief.
- **0x00A0B9A0** ~CSoundBankLH. It never uses PSoundManager. Its local model is a CASoundBank (PSymbolMap +4), not a sound manager.

Every `rows/<addr>_test.cpp` passes the verifier's VC7.1 `/Od` behaviour test (`tall_rows.py`, through `V.behaviour_of`):
- 0x004022B0 links its row object (sobj), as landed. I added the standard `004022b0_TEST PASS` line next to its old pass line.
- 0x0088F660 got a new self-contained host-model test. The landed one linked the row's old local-class symbol.

Under plain g++ (`-std=c++03`), 83 of the 94 tests pass as they stand. These do not:
- They need `-I rebuild/include` plus empty `__fastcall`-style macros, as landed: 0x0042DED5, 0x0056FAD7, 0x00596763, 0x00629DE0, 0x0066BA80, 0x0066BDF0, 0x006E2A50.
- 0x00587847 and 0x0066BD00 used `__declspec(noinline)` and `__fastcall` with no g++ guard. They now carry an `#if !defined(_MSC_VER)` shim and pass under g++ with `-I rebuild/include`; under VC7.1 the shim is inactive and the behaviour test still passes.
- 0x004022B0 still fails under 64-bit g++, because `rebuild_abi.h` asserts 32-bit pointers (pre-existing; debt).
- 0x0088F660 failed with the landed test; its new test passes under g++.

## Job 1: emitter self rows

- Each row is a `class CSoundEmitterLH : public CASoundEmitter` override with the header signature, under the SIZE40 opt-in.
- CActiveSound is a nested `class` model (members State, SampleID, PBank, StartTime, Started, Stopping, Sample, Name, PAtmos; descriptive names, no PDB).
- The list is the real STLport `std::list<CCountedPointer<CActiveSound> >`.
- Every row graded MATCH or RELOCATION_MATCH on its first grade.
- Tests are new host models; the old ones linked the `vfuncN` symbols.

## Job 2: callee spellings changed (for reloc_bindings)

`RELOC_CHANGES.tsv` lists every relocation of every returned row against `rebuild/oracles/reloc_bindings.tsv`:
- **`changed`**: the binding exists with another symbol. These 19 need respelling.
- **`new`**: the target has no binding for that row. Mostly `?PSoundManager@@3PAVCASoundManager@@A` and the header's sound callees.

The 19 changed bindings (row: target, old → new):

| Row | Target | Old spelling | New spelling |
|---|---|---|---|
| 0x004175E5 | 0x00991750 | `?StopMusic@CSoundManager@@QAEXXZ` | `?UnloadAllBanks@CASoundManager@@QAEXXZ` |
| 0x004175E5 | 0x009918F0 | `?Shutdown@CSoundManager@@QAEXXZ` | `?CloseAllBanks@CASoundManager@@QAEXXZ` |
| 0x0042E17C | 0x00991600 | `?SetEnabled@CSoundManagerLH@@QAEX_N@Z` | `?EnableSound@CASoundManager@@QAEX_N@Z` |
| 0x0042EBB6 | 0x00991750 | `?Restore_00991750@CGameStateOwner_013b8394@@QAEXXZ` | UnloadAllBanks |
| 0x0049DCC0 | 0x00991620 | `?func_00991620@CFlaggedObject_Retail@@QAEX_N@Z` | `?EnableAtmos@CASoundManager@@QAEX_N@Z` |
| 0x00629DE0 | 0x00438570 | `?PeekGuiDef@@YIPAVCPlayerGuiDef@@XZ` | `?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ` (Ego_r + FableWin; from engine/CPlayerGui.h) |
| 0x0065C781, 0x0066BA80, 0x009BD910 | 0x009917E0 | CSymbolMap::FindFirstSymbolWithVal (QAE / QBE), CASoundManager::FindSoundBank | `?GetPSoundBank@CASoundManager@@QBEPAVCASoundBank@@ABVCCharString@@@Z` |
| 0x006B1960 | 0x0125D47C | `??_7CMyAtmosMap@@6B@` | `??_7CMyAtmosMap@CAtmosProcess@@6B@` (RTTI) |
| 0x006E2A50 | 0x00991690 | `?FindSymbolForValue@CSymbolMap@@QAEPAXJ@Z` | `?GetPSoundBank@CASoundManager@@QBEPAVCASoundBank@@ABVCLipSyncDataBank@@@Z` |
| 0x006E2A50 | 0x009916F0 | `?FindValueForSymbol@CSymbolMap@@QAEPAXJ@Z` | `?GetPLipSyncBank@CASoundManager@@QBEPBVCLipSyncDataBank@@ABVCASoundBank@@@Z` |
| 0x006E2A50 | 0x00991720 | `?FindFirstValue@CSymbolMap@@QAEPAXXZ` | `?GetAnyPLipSyncBank@CASoundManager@@QBEPBVCLipSyncDataBank@@XZ` |
| 0x00836740 | 0x004C9560 | `?OnCreate@CThing@@QAE_NXZ` | `?OnCreate@CThing@@UAE_NXZ` |
| 0x00AAC1D0 | 0x00A3ED30 | FindSample@CSoundBankLH | `?FindByName@CSoundManagerLH@@QAEPAVCSoundBankLH@@ABVCCharString@@@Z` |
| 0x00AAC1D0 | 0x00AAB910 | the CActiveSound ctor taking `PAUCSoundEmitterSource` | the ctor taking `PAVCSoundBankLH` |
| 0x00AAC1D0 | 0x00AAC820 | the U-key CCountedPointer<CActiveSound> ctor | the V-key ctor |
| 0x00AAC1D0 | 0x00AACCC0 | `CActiveSoundList::push_back` | the real `std::list<...>::push_back` |
| 0x005AE86B | 0x005ACEDA | `?ConstructTradeScreenPC@CTCInventoryTrade@@QAEXXZ` | `?ConstructActualInventoryPC@CTCInventoryTrade@@AAEXXZ` (FableWin.pdb, private; respelled in rows/005ae86b.cpp) |

Virtual-slot respellings leave no relocation, but they now carry the header names:
- `CSoundSystem::RefreshCategory`, `Sound::SetGroupVolume`, `Stop` and `PlaySound` are now SetGainFader (slot 46).
- `MixAtmos` (the manager slot) is now SetChannelGain (slot 40).
- Slots 14, 15 and 36 are StopSound, IsSoundPlaying and InitAtmos.
- `GetSoundPlayer` / `GetBank` now go through Get2DEmitter (slot 9) to PlaySample / PlaySoundEvent (CASoundEmitter slots 3 / 4).

## Fixes made in this pass, after the first grades

- **0x006B1900.** The migration script had renamed the row itself to `CAtmosProcess::SetChannelGain`. It is back to its Ego_r name `CAtmosProcess::MixAtmos` (`?MixAtmos@CAtmosProcess@@QAEXABVCCharString@@0MM@Z`). Only the manager call is SetChannelGain. Regraded RELOCATION_MATCH.
- **0x00557D20.** The include had landed inside `namespace NUISystem {`, which referenced `?PSoundManager@NUISystem@@...`. The include now sits at file scope. Regraded RELOCATION_MATCH, and the object references the global.
- **0x0088F660.** Rewritten as the PDB member `?StopSound@CGameScriptInterface@@UBEXK@Z` on engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT), with a proper line 1 and a new self-contained test. RELOCATION_MATCH.
- **Leaf lines.** 17 landed rows had no `// Leaf:` line: 00407370, 004175e5, 0042ebb6, 0047d78b, 00557000, 00557d20, 00587847, 005ae86b, 005fce60, 006116e0, 00621720, 00621c80, 0066aeb0, 0066afa0, 0066aff0, 006e2a50, 0088f570. Each now has the symbol of the function its object grades, and each regraded RELOCATION_MATCH. Some of those manglings carry the landed local models' debt; see Header debt.
- **Test identifiers.** Tests 0x0066A920, 0x0066ACE0, 0x0066B080 and 0x0066B120 used identifiers containing `_emit` (`g_emitter`). They are respelled `g_soundEmitter`.
- **Header forward declarations.** `CLipSyncDataBank` and `C2DCoordI` moved under their opt-ins, and the 15 rows that use them were regraded RELOCATION_MATCH.

## Job 3: parked rows

All four rows are in `differ/`, with the measured residue on line 2. Every variant was measured first with c2prio `--obj`; probes are free and their object is byte-identical to a grade's. A grade was spent only to record the best version.

### 0x005A94BD CTCInventoryTrade::OpenInventory
- **Grades:** 2 used, 5 of 10 left. **Result:** DIFFER(1488v1482).
- **Lever that worked:** the frame. The vectors after `sellableItems` go in a nested block. That gives retail's frame 0x40 and every [ebp-x] slot as retail; the C2DBoxF temporary now packs over the buy / all-items vectors.
- **Left:**
  - **Registers.** this=edi / menu=esi where retail has this=esi / menu=edi. c2prio colours pMenu at priority 74 before `this` at 72; equal priorities would still lose to pMenu's key.
  - **Loop entry.** The first normal-shop erase loop has a duplicated pre-test.
  - **Const-0 piece.** The const-0 ebx piece starts at the `highlighted > 0` compare.
- **Tried with no change:** while-loop, iterator outside the loop, swapped loops, `0 < h`, `h >= 1`, a block-scoped `highlighted`, `pMenu` declared first.
- **Note:** the same-TU STLport `push_back` body is kept as landed precedent (0x00574576).

### 0x005ACEDA CTCInventoryTrade::ConstructActualInventoryPC
- **Grades:** 1 used (a re-measure under this overlay), 7 left. **Result:** DIFFER(1113v1067), unchanged.
- **Left:**
  - **Prologue.** Retail homes `this` at entry and shrink-wraps the pushes below the SetGainFader block. Ours starts the `this` esi piece and the const-0 ebx piece at entry.
  - **Closing block.** Retail reloads `this` into ebx there. Ours reloads it into esi at the end of every arm, which stops the special arms cross-jumping.
  - **Loop entry.** The same pre-test residue as 0x005A94BD.
- **Tried with no change:** the sound-test spelling, `pShop` hoisted, `optimize("t")`, `while`.
- **Next:** read c2's split-piece ranges for `this` (#7) and const 0 (#1). The probe shows the pieces but not their block ranges.

### 0x005F6793 CTCInventoryAbilities::CycleAssignedExpressions
- **Grades:** 2 used (the second re-ran the same file to list addresses), 3 left. **Result:** DIFFER(447v442).
- **Levers that worked:**
  - **No invented stand-in.** The invented `FindPrioritisedExpressionDefs_s5eed8c` stand-in is gone. The real GetPrioritisedExpressionDefs body (0x005EED8C, landed) gives the same bytes.
  - **Two-call assign arm.** It fixed `this`=edi, the late ebx push and the button register.
- **Left:**
  - **Search iterator.** It is homed before the `checked < buttons` pre-test; retail keeps it in eax and enters with `jmp`.
  - **min / max temporaries.** They pack into the iterator's slot; retail puts them in `original`'s and `checked`'s slots.
  - **ebx restore.** Ours restores ebx after the final free, so the early-return free does not cross-jump.

### 0x005F64DD CTCInventoryAbilities::CycleAssignedAbilities
- **Grades:** 1 used, the last of the 10. **Result:** DIFFER(508v508), the same length as retail.
- **Lever that worked:** the two-call assign arm (`if (def.IsNull()) F(NULL, b, false); else F(def->Ability, b, false);`) shares the def copy's AddRef null test as retail does.
- **Left:**
  - **Wrap value.** The `count - buttons` wrap is computed in edi; retail uses ecx.
  - **Iterator copy.** The search iterator copy sits above the pre-test.
  - **Shared pushes.** Retail hoists the shared `push 0 ; mov ecx,ebx ; push edi` above the null test.
- **Debt:** it still uses the agentSND stand-in `FindPrioritisedAbilityDefs_s5f4b30` for GetPrioritisedAbilityDefs. The real body (0x005F4B30) needs a retail CThingCreatureBase::IsChild declaration and `GetAbilityPDef` (MAGIC opt-in) before it can replace it.

## Header debt
- **CTCSoundPlayer local models.** The CTCSoundPlayer rows (0x0066A920 … 0x0066BDF0, 0x0066E742) still declare local CTCSoundPlayer / CThingSoundEmitter-side models; only the sound manager moved. CTCSoundPlayer needs a shared header.
- **CMyAtmosMap.** Its slot names GetBlockValue / GetBlockHeight are descriptive.
- **The +0x64 map's value type.** The landed rows and 0x00AAC1D0 spell it `CSoundBankLH*`, but RTTI CSoundBankLH's slot 2 is 0x00A0B6C0 (UnloadCompiledBank), while this object's slot 2 starts an atmos event. It is probably an atmos object; the Anniversary build has CAtmosUN / CChannelUN.
- **libpk.** The libpk classes (CSoundSystem vtable 0x012B1E24, CSoundEventManager vtable 0x012B2748) have no PDB or RTTI, so the event-manager slot names in the emitter rows are descriptive.
- **0x0129D25C, the invalid sound handle.** It has two manglings in the returned rows: `?g_InvalidSoundHandle@@3KA` (8 rows) and `@@3JA` (0x0088F570). The landed tree has more spellings. The Anniversary candidate `?SOUND_RESULT_FAIL@NSoundSystem@@3KB` has one source only, so it is not in NAME_FIXES.
- **One-source proposals.** UpdatePlayingSounds for 0x00AABCA0 (landed RemoveInactiveSounds) and GetPSoundBank(long) are Anniversary-only names; they were left as landed.
- **0x00891220 / 0x008912A0.** OverrideMusic and StopOverrideMusic still define their members on invented derived classes (`CGameScriptInterface_OverrideMusicMethods` / `CGameScriptInterface_Methods`), with local CUserProfileManager / CWorld event models and no line-1 header (as landed). The PDB names are `?OverrideMusic@CGameScriptInterface@@UBEXW4EMusicSetType@@_N1@Z` and `?StopOverrideMusic@CGameScriptInterface@@UBEX_N@Z`, slots 696 / 697. Only their sound-manager call moved.
- **Landed local models in some Leaf manglings.** For example, 0x00621C80 CTCInventoryQuests::CloseInventory and 0x006116E0 OpenInventory are `QAE` where the vtable slots are virtual. 0x005FCE60 is still `CTCInventoryMap::vfunc_31`. 0x004175E5 uses invented `CCountedPointer403750`-style wrappers.
- **Landed test shims.** Tests 0x00629DE0, 0x0066BA80 and 0x0066BDF0 carry a landed `#define __forceinline inline` portability shim for g++.

## Files
- `rows/`: 94 sources and 94 tests (jobs 1 and 2).
- `differ/`: the four job-3 rows.
- `src/`, `test/`: empty; nothing in job 3 matched.
- `RENAMES.tsv`, `RELOC_CHANGES.tsv`, `STATUS.tsv`, `ATTEMPTS.log`, `NAME_FIXES.tsv`, `LABELS.tsv`, `LESSONS.md`, `hdr/` + `hdr/README.md`.
- Tools: `pr.sh` + `objcmp.py` (c2prio probe with object diff), `rdis.py` (retail disassembly from the oracle), `tall_rows.py`.
