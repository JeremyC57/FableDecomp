# Chunk 709: notes

## Results
- **Matched:** 9 of 10 rows, all RELOCATION_MATCH, and every behaviour test passes under g++ and VC7.1.
  - The read-only vland run (FABLE_HDR_OVERLAY set, `--recheck-landed`) has 9 WINS: RELOCATION_MATCH + PASS, with no
    RELOC_IDENTITY and no DIFFER. The log is in tmp/vland.log, and lv/ was deleted afterwards.
- **Parked:** none.
- **Skipped:** 0x00BFEF20. It is a mid-function fragment, so it was not authored (probe/00bfef20.log).
  - The 24 bytes are the `__finally` block (the `__ArrayUnwind` 0x00BFEE7A path) of
    `` `eh vector destructor iterator' `` `??_M@YGXPAXIHP6EX0@Z@Z` at 0x00BFEED8.
  - That function is 96 bytes, runs from 0xBFEED8 to 0xBFEF37, and is already landed with that extent.
  - Proof: compiling the landed 0x00BFEED8 source and grading it against this row gives DIFFER(96v24), and the object's last
    24 bytes equal the row's bytes.
  - The catalog name "CFrameData::ReleaseTexture" belongs to no function at this address.

## Names and evidence (each name has two sources and the exact PDB spelling)

| Row | Name | Evidence |
|---|---|---|
| 0x0049D8C0 | `?EventIsWorldEvent@CWorld@@QBE_NPBVCGameEvent@@@Z` | Ego_r and FableWin CWorld member. The caller is CMainGameComponent::AddGameEvent 0x0041649C. The dispatch table global is `?GameEventDispatchTable@@3PAVCGameEventDispatch@@A` (0x013B9288). |
| 0x0049E270 | `?Unload@CWorld@@QAEXXZ` | Ego_r order puts it after LoadWorldFromFile. It calls `GetThingManager` through vtable slot 3. Callees: GetMainPlayer 0x00449970, CPlayer::ResetModes 0x00634870, CPlayer::GetPControlledCreature 0x00487DC0, CThing::Kill 0x004C9B80, CThingManager::UpdateDeaths 0x0051F070. |
| 0x0071CD30 | `?FrameUpdate@CTCHeroMorph@@UAEXXZ` | Slot 9 of vtable 0x01265CF4. Ego_r order is UpdateIdleParticles then FrameUpdate. Slot 28 is DoMorphUpdate. Members proven from ctor 0x0071E180; sizeof 0x64 from `push 64h` at 0x004D28BB. |
| 0x0072D390 | `??1CCreatureAction_SheatheItemToInventory_ForEnflame@@UAE@XZ` | ctor 0x0069F060 stores vtable 0x0125C95C, whose RTTI is ForEnflame. The FableWin `??1` exists. Local use in SetupEnflameSpell 0x0072C790. Twin of the landed ForDivineWrath 0x0072D3E0. |
| 0x00826380 | `??1CCachedFile@@UAE@XZ` | Stores vtable 0x012724D4 (CCachedFile). Ego_r `??1`. Close is slot 2 (0x00A5AFA0). The catalog's "CChunkedFile" is wrong. |
| 0x00873DB0 | `?OnCameraInsideObservedThing@CPCMouseLook@NCameraModes@@UAEXXZ` | Slot 3 of vtable 0x01277784, a slot whose base body is an empty `ret`. Ego_r order matches. It tail-calls `?FrameUpdate@CBase@NCameraModes@@QAEXXZ` 0x0084DC10. |
| 0x00953470 | `??1CCreatureAction_DragonTakeOffIntoFlying@@UAE@XZ` | `??_G` 0x00953960 is slot 0 of 0x01293A7C. Ego_r `??1` is UAE. Local use in 0x00951B20. |
| 0x009CAE60 | `??1CGroupEntry@NGameText@@UAE@XZ` | `??_G` 0x009CAE40 is slot 0 of 0x0129B478. Ego_r `??1`. |
| 0x009EEA50 | `??1?$pair@VCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@QAE@XZ` | The console TU is /Gr. FableWin lists this non-const pair type. A lane experiment of the AddInput 0x009EC5E0 shape reproduces the retail emission order of the four bodies 0x009EEA50, 0x009EEA90, 0x009EEDC0 and 0x009EF260. |

New spellings with no reloc_bindings entry before this chunk, both present in fablewin_pdb_names.tsv and the Ego_r signatures:
- `?Close@CCachedFile@@UAEXXZ` 0x00A5AFA0
- `?UpdateIdleParticles@CTCHeroMorph@@AAEXXZ` 0x0071C9F0

## Label corrections (all in NAME_FIXES.tsv)
- **Bound stand-ins in landed callers:**
  - 0x0041649C binds `CWorld_Methods::EventIsWorldEvent`.
  - 0x0072C790 binds `??1CCombatAction_GenericStrikeResponse@@QAE@XZ` for 0x0072D390.
  - 0x00951B20 binds the QAE (should be UAE) spelling for 0x00953470.
  - 0x0084DC10 is bound as `CCameraModeBase::FrameUpdate` and as `CCameraBase::FrameUpdate`.
  - The four 0x009EEA50-set addresses carry descriptive stand-ins. Three are landed: 0x009EEA90, 0x009EEDC0 and 0x009EF260.
- **Catalog labels:**
  - The `_Dest_val<..C3DSkeletalMorphTarget..>` label was propagated to four unrelated destructors: 0x0072D390, 0x00953470,
    0x009CAE60 and 0x009EEA50.
  - CChunkedFile should be CCachedFile.
  - vfunc_9 and vfunc_3 at 0x0071CD30 and 0x00873DB0.
  - CPlayer::GetHeroCreature at 0x00487DC0 (vland WARN RELOC_NAME). The PDB name is GetPControlledCreature.
- **Extra spellings for one address:**
  - 0x00449970 GetMainPlayer.
  - 0x004C9B80 CThing::Kill.
  - The data address 0x013B9288 has two spellings: `GameEventDispatchTable`, which this chunk used, and `g_GameEventHandlers`.
- **Collisions:** none of my names collides with another landed row's spelling. 0x009EEA50 is a byte twin of 0x009EEA90 and
  0x009EF260, but those are different symbols, each named from its own role, so all three bind.

## Header corrections (hdr/, see hdr/README.md)
- **engine/CWorld.h:** new opt-in `FABLE_RETAIL_CWORLD` branch for the polymorphic retail class. Bases are CBaseClassNonCopyable
  and CIDrawWorld. There are 14 primary slots and 3 CIDrawWorld overrides. sizeof is 0x198. The default branch is unchanged.
- **engine/CPlayer.h:** ResetModes and GetPControlledCreature added inside the existing `FABLE_RETAIL_PLAYER` branch.
- **Recheck, read-only with the overlay:** the landed FABLE_RETAIL_PLAYER user 0x00802610 and four landed CWorld.h default-path
  includers (0x0041649C, 0x0062B7C0, 0x008A7F60, 0x00972490) are all RELOCATION_MATCH.
- **Overlay proof:** with the overlay off, 0x0049D8C0 and 0x0049E270 fail to compile (SRC_FAIL).

## Header debt
- **engine/CCreatureActionBase.h** is a flattened, non-polymorphic `#pragma pack(1)` struct with `void* __vftable`, so it cannot
  be a base class.
  - 0x0072D390 and 0x00953470 model the base locally: a virtual dtor plus opaque storage up to the retail sizeof 0xA8. This is
    the model of the landed twin 0x0072D3E0, and 849 landed sources define a local `class CCreatureActionBase {`.
  - vland warns SHARED_HEADER_AVAILABLE for 0x00953470. 0x0072D390 escapes the warning only because its comment names the
    header. LANDVERIFY_STRICT_HEADERS=1 would reject 0x00953470.
  - The fix is a `FABLE_RETAIL_CCREATUREACTIONBASE` branch: a polymorphic class with the full slot list of vtable 0x01259D24
    and the generated members at their natural alignment, unpacked.
  - That slot list is not in rebuild/manifest/rtti-vtables.tsv, which lists only slot 0, so the fix was too large for this chunk.
  - CCombatActionBase (0x114, vtable 0x01259E44) needs the same treatment.
- **No engine header exists** for CThing (quarantined), CThingPhysical, CThingGameObject, CThingCreatureBase,
  CThingPlayerCreature, CAFile/CCachedFile, NCameraModes::CBase/CPCMouseLook, CTCAppearanceMorphBase/CTCHeroMorph,
  CCreatureAction_SheatheItemToInventory(_ForEnflame), CGameEventDispatch and CConsoleInputBase.
  - Each row restates its class locally with every member at its retail offset, or as opaque storage up to the retail
    sizeof.
  - In 0x0049E270 the intermediate CThing-chain sizes (0xC0, 0xD0, 0x160) are opaque and not byte-verified. Only CThing
    (0x98) and CThingPlayerCreature (0x208, from `mov ecx,208h` before ctor 0x0052AB20) are proven.

## Decisions for the coordinator
1. Apply NAME_FIXES.tsv: respell the bound callers (0x0041649C, 0x0072C790, 0x00951B20 and the 0x0084DC10 binders), and
   rename the three landed 0x009EE* / 0x009EF260 stand-ins to the STLport pair symbols.
2. Copy hdr/engine/CWorld.h and hdr/engine/CPlayer.h into the repo. Both changes are opt-in.
3. Either accept the local CCreatureActionBase model, as for 0x0072D3E0, or schedule the retail CCreatureActionBase header
   branch first.
4. Retire catalog row 0x00BFEF20 as a fragment of landed 0x00BFEED8.
