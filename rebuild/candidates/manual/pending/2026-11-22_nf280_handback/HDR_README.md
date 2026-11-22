# NF280 header overlay (entry 1, row 0x00416E78 CMainGameComponent::GetInputs)

Base copies: `orig/engine/` (copied from the repo working tree at the start of entry 1; compared equal to the repo again when the
equiv_check tree was built). Every change is add-only under a NEW macro (each name had 0 hits in rebuild/ before); no existing line
was edited. With none of the new macros defined, each header's token stream is HEAD's apart from the pinned assert spellings.

| Header | Macro | What |
|---|---|---|
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_PROCESSEVENT` | `class CGameEvent;` forward, and in the class `protected: void ProcessEvent(const CGameEvent* event); public:` (0x00415FE0; Ego_r.pdb 0x00415194 / FableWin.pdb 0x018A872E `?ProcessEvent@CMainGameComponent@@IAEXPBVCGameEvent@@@Z`). Declaration only, placed before the ctor declaration. |
| engine/CInputManager.h | `FABLE_RETAIL_CINPUTMANAGER_UPDATECHECKSUM` | `class CChecksum;` forward, and `void UpdateChecksum(long time_stamp, const CChecksum& checksum);` (0x009F4A90; Ego_r.pdb / FableWin.pdb `?UpdateChecksum@CInputManager@@QAEXJABVCChecksum@@@Z`). Declaration only. |
| engine/CChecksum.h | `FABLE_RETAIL_CCHECKSUM_CTOR_THREE` | the inline `CChecksum(long, long, long)` (FableWin.pdb `??0CChecksum@@QAE@JJJ@Z`): Checksum1..3 from the arguments, Checksum4 = 0 (retail GetInputs 0x00416F72..0x00416F83). Parameter names descriptive. |

Pins: `pin_asserts.py rebuild/include hdr <header>`: CChecksum.h 5 pinned (`FABLE_STATIC_ASSERT_AT(26..30, ...)`, the old lines);
CMainGameComponent.h and CInputManager.h already spell every assert `_AT` (0 pinned). No assert added or removed.

Header users (direct includers in rebuild/src/compiled): CMainGameComponent.h 496, CInputManager.h 20, CChecksum.h 0 (it is reached
only through other headers, e.g. CInputEventPackage.h). None defines a new macro; the whole-tree equiv_check (every landed source and test, repo tree v repo + these three
files) is the proof that the opt-ins change none of them (NOTES.md, evidence/equiv.log).

NF279's overlay (`$S/w22/agentNF279/hdr/`) holds only engine/CEngineWaterBackgroundSubPatch.h: no overlap.
