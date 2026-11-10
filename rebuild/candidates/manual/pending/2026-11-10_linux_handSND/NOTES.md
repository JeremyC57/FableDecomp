# agentSND (w22, 2026-11-10): CASoundManager / CSoundManagerLH slot names + the six held sound rows

**Grading setup.** Every grade, `gall.sh` run and compile check ran with `FABLE_HDR_OVERLAY=$S/w22/agentSND/hdr` exported. `gr.sh`
and `cc.sh` export it themselves. `cc.sh` is a compile-only check, not a grade: it was used only to catch compile errors. The
overlay is described in `hdr/README.md`.

## Results

| row | name | result | grades |
|---|---|---|---|
| 0x0056F30B | CTCInventoryExperience::CloseInventory | RELOCATION_MATCH | 1 |
| 0x0058F649 | CTCInventory::OpenPCInventory | RELOCATION_MATCH | 1 |
| 0x005F64DD | CTCInventoryAbilities::CycleAssignedAbilities | DIFFER(512v508) | 9 |
| 0x005F6793 | CTCInventoryAbilities::CycleAssignedExpressions | DIFFER(445v442) | 5 |
| 0x005A94BD | CTCInventoryTrade::OpenInventory | DIFFER(1488v1482) | 3 |
| 0x005ACEDA | CTCInventoryTrade::ConstructActualInventoryPC | DIFFER(1113v1067) | 2 |

**Respelled rows (`rows/`, `RENAMES.tsv`).** There are 40 in total and every one grades MATCH or RELOCATION_MATCH under the overlay.
- **The 10 relanded rows.** These use SetGainFader for the old SetGroupVolume. 0x0041E6D3 uses `Get2DEmitter().StopSpecificSample` and no longer has a local CASoundSampleOwner_Unproven. 0x006286F0 uses the ENABLE_NAMES opt-in. 0x00991670 is now `CASoundManager::Update`.
- **The self rows.** These are 22 CSoundManagerLH rows and 8 CASoundManager rows (base slot bodies and the three Enable setters). They are the vfuncNN and address-suffix rows, plus AddBank (now NewAtmos), AddSound (now NewChannel), the catalog's CCreatureActionBase::GetAnimStartFrame (now CanAllocateEmitter) and CWorld::SetMiniMapEnabled x2 (now EnableMusic / EnableSound).
- **Tests.** None of the tests needed to change, because they print the pass line on their own local models.

## Step 1: sources checked for the slot names

| source | result |
|---|---|
| Ego_r.pdb, FableWin.pdb, Ego_d.pdb name sets (`ghidra_out/egor_pdb_names.tsv`, `_fablewin_api.tsv`) | **Miss** for class members: neither PDB has a CASoundManager or CSoundManagerLH method, because the audio library was linked without them. **Hit**: 15 `NGlobalConsole::ConsoleAudio_*` console stubs (SetOutputPCMBufferCount .. SetHeadroomBits, SendLibCommand). Their order follows slots 19..35, 43, 52 and 53. |
| `ghidra_out/struct_layouts_egor.tsv` | **Hit**, types only: CUserProfileManager::UsingHeadPhones +0x30 (slot 7 caller); CSoundDef AtmosListenerDampingMin / Range (InitAtmos arguments, which fix slots 27 / 28); CFadeTimes FadeOutMS / FadeInMS and CRegisteredMusicEntry Gain / Offset (PlayMusic arguments); CReverbParameters, 84 bytes (slot 29); CSoundInit (0x20 bytes). |
| FSE manifest `refs/fse_api_manifest.json` | **Hit** for script callers only (Play2DSound, StopSound, IsSoundPlaying, EnableSounds, SetAllSoundsAsMuted). No engine names. |
| RTTI (`ghidra_out/rtti_bases.tsv`, the exe's COLs) | **Hit**: CASoundManager (vtable 0x0129A574), CSoundManagerLH (0x0129D2C4), CASoundEmitter (0x0129E684), CSoundEmitterLH (0x0129E6EC), CASoundReceiver / CSoundReceiverLH (0x0129E76C), and the nested `.?AVCAtmosMap@CASoundManager@@`. |
| Exe strings | **Partial**: the cutscene verbs PlayMusic / StopMusic / CacheMusic / MuteSounds / EnableSounds. |
| PDB-named TLC callers of each slot (`tools/slotcallers.out`) | **Hit**. These are CWorld::RenderUpdate, CThingSoundEmitter::OnCreate / ~, CGameScriptInterface::StopSound / IsSoundPlaying / EnableSounds, CMusicManager::PlayMusic, CAtmosProcess::*, SoundTheme_LoadDefinitions, CUserProfileManager::ApplyValues and the 119 SetGainFader sites. |
| Fable Anniversary Xbox 360 PDB index (`$S/anniv_pdb/index`, publics) | **Hit**: CASoundManager / CSoundManagerUN / CSoundEmitterUN method names with parameter types, and EnableMusic / EnableSound / EnableAtmos. **Miss**: the TPI has no Fable class records. It is a reading aid, so it is never the sole source for a name. |
| Fable Anniversary PC exe RTTI vtables (CASoundManager 0x02E5895C, CSoundManagerUN 0x02E58CA4, CSoundEmitterUN 0x02E5889C) | **Hit**: these give the slot alignment. TLC slot t maps to Anniversary slot a as follows: 1 to 1; Anniversary a2 (CreateAtmosSoundBank) has no TLC slot; t2..18 to a3..19; a20 / a21 are inserted; t19..49 to a22..52; a53 / a54 are inserted; t50..53 to a55..58. |
| `docs/journal/FINDINGS_LOG.md`, `docs/pipeline/GOTCHAS.md`, catalog labels | **Miss** for real names. Catalog labels for these bodies are propagated or garbage; see NAME_FIXES. |
| EgoCore, FableForge, ForgeFSE-retail-shadow, Fable2RE | **Not on this machine**, so not checked. |
| Aeon Lua ports | Not consulted: they are script level and do not reach the audio classes. |

**Class at CSoundManagerLH+0x44.** It is a `CSoundEmitterLH`, whose RTTI is `CSoundEmitterLH : CASoundEmitter : CBaseClass` with vtable 0x0129E6EC.
- The manager constructor 0x00A3EF00 builds it with 0x00AAC530, `CSoundEmitterLH::CSoundEmitterLH(CSoundManagerLH*)`, the non-positional emitter.
- +0x48 is a CSoundReceiverLH (0x00AAD020).
- +0x40 is the libpk sound system (0x00C096E0).
- The abstract class now has its own shared header, `hdr/engine/CASoundEmitter.h`. Slot 9 of the manager returns `CASoundEmitter&`.

## Step 2: names (SLOTS.tsv)

- **Real names.** All 54 manager slots except two have a real name from the Anniversary publics, each with a TLC second source: a PDB-named caller, a ConsoleAudio_* stub, a body or a shared base body.
- **Descriptive names (owner rule 2026-11-10).**
  - **Slot 9, `Get2DEmitter`.** It returns the manager's own non-positional emitter. Its 25 callers play UI samples on it. The Anniversary slot is a folded body with no public name.
  - **Slot 51, `ExtractMetaData`.** It passes a bank file name to the libpk system slot 1, which reads the .lug/.lut meta data. There is no Anniversary public and no TLC caller.
- **Emitter.** 23 of the 25 CASoundEmitter slots have real names. The other two are descriptive: 18 `Is3D` (returns the +0x18 flag the positional constructor sets) and 24 `GetActiveSampleInfo`.

## Step 5: held rows

### 0x0056F30B / 0x0058F649
Both matched on the first grade once the slot names existed. I removed their old `// Flags: none` lines, because gall/q pass a literal flag.

### 0x005F64DD / 0x005F6793
Both are rebuilt on the shared headers: the CTCInventoryAbilities retail opt-in plus ASSIGN / BUTTONS / PRIORITISED, CSoundInit.h, CASoundBank SOUNDINDEX and CThing MOREACCESS.
- **Name corrections from the PDBs.**
  - 0x005EDCF0 is `GetNumAssignableSpecialMoveButtons`; the prior draft called it GetNumAbilitiesPerRow.
  - 0x005F6423 is `SetAbilityToButton`; the prior called it SetAssignedAbility.
  - 0x005F66D9 is `SetExpressionToButton(long, long)`.
  - +0x144 / +0x1B8 are `CurrentAssignedAbilityLevel` / `CurrentAssignedExpressionLevel`.
  - CSoundInit +0x14 is `GainFaderBits` (0x20).
- **Sound tail.** It is `PSoundManager->Get2DEmitter().PlaySample(bank, sample, true, init, false)`.
- **0x005F64DD lever.** A same-TU stand-in body for GetPrioritisedAbilityDefs that keeps no pointer to the vector made VC7.1 keep the vector end in edi across the later calls, as retail does. That matches the whole head. The remaining residue is the assign loop: retail holds the button counter in edi and the page iterator in memory; ours does the reverse.
- **0x005F6793.** The same lever moved the zero register to esi, as in retail. The `this` register and the button/count/page web stay swapped (retail edi / ebx, ours ebx / edi).

### 0x005A94BD / 0x005ACEDA: identity settled from the hand1009 / 1009b NOTES, two sources each
- **0x005A94BD is `CTCInventoryTrade::OpenInventory`.** It is the slot-30 override in vtable 0x01253034 and has the Ego_r / FableWin name.
- **0x005ACEDA is `ConstructActualInventoryPC`.** FableWin.pdb names it, and its only caller is OpenInventoryPC 0x005AE86B.
- **Remaining OpenInventory callees.** I found these in this chunk:
  - OnOpenMenus.
  - SetPlayerThing of GetPlayer().GetPControlledCreature().
  - ItemSellLists / ItemBuyLists clears.
  - IsInventoryScreen with GetPHeroInfoScreensManager / SelectScreenOfType(INVENTORY_TRADE_SCREEN).
  - The base OpenInventory.
  - `SetInventoryAnimRenderBox(C2DBoxF(GetPDef()->MeshAreaTLPos, GetPDef()->MeshAreaBRPos))`.
  - The console list builders ConstructSpecialShopList 0x005A29F4 / ConstructNormalShopList 0x005A04F7.
  - RefreshEntriesAmountsAndAvailabilities for the BUY / SELL / WANTED entries.
- **Shared-header gaps.** These needed new overlay opt-ins: PDEF / CInventoryDef, OPENINVENTORY / ACTUALINVENTORYPC, and C2DBOXF_CORNERS_FORCEINLINE.
- **0x005ACEDA residue.** The prior's local model and the shared headers give the same 1113 bytes. Retail shrink-wraps the ebx/esi/edi pushes past the sound block; ours zeroes ebx at entry. The closing block's `this` reload enables retail's tail cross-jump, and the first normal-shop loop has a pre-test in ours.
- **0x005A94BD residue.** Only esi/edi naming and 4 bytes of frame packing remain; the C2DBoxF temporary shares the later vectors' slots in retail.

## Header debt / next steps
- **CASoundManager members.** The class is still opaque from +0x04 to +0x4C, and the LH members start at +0x40: the system, the 2-D emitter and the receiver. The Anniversary members (registered banks CVectorMap, enable flags +0x24..+0x26, exclusive mode +0x20, emitter limit +0x18) are the next model to write.
- **Landed rows with local stand-ins.** About 90 landed rows still model PSoundManager with local stand-ins: CSoundSystem::RefreshCategory (e.g. CTCInventoryTrade::CloseInventory 0x0059F304), Sound::SetGroupVolume, g_pGameState->GetSoundPlayer, and so on. A follow-up wave should move them to `engine/CASoundManager.h` and `engine/CASoundEmitter.h`.
- **Emitter self rows.** The CSoundEmitterLH self rows that are still named vfuncN were outside this brief. Map them with the CASoundEmitter.h slot comments, e.g. vfunc9 to StopSpecificSample, vfunc5 to PlayAtmos, vfunc14 to Update.
- **NAME_FIXES proposals.** GetPSoundBank (0x00991840) and UpdatePlayingSounds (0x00AABCA0) have one source only, the Anniversary publics.
- **Next for 0x005F64DD / 0x005F6793.** Try the c2prio colouring tool of hand1009b on the assign-loop webs.
- **Next for 0x005ACEDA / 0x005A94BD.** The residue is allocator and packing only. Try a nested-scope experiment for the vectors (frame packing), and work out why retail homes `this` in [ebp-8] and reloads it.
