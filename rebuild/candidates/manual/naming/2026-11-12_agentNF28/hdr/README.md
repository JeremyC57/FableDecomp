# Header overlays (agentNF28)
Each file is a FULL copy of the current repo header under rebuild/include/, with `#ifdef` additions only (default builds unchanged).
Grade with FABLE_HDR_OVERLAY=$S/w22/agentNF28/hdr.

| header | opt-in | adds | used by |
|---|---|---|---|
| engine/CTCPhysicsBase.h | FABLE_RETAIL_CTCPHYSICSBASE_SETPHYSICSMESHINFOFROMGRAPHIC | `public: void SetPhysicsMeshInfoFromGraphic(const class CEngineGraphic&);` (0x006B08E0; both PDBs) after the CREATEPPHYSICSMESHINFO block | 006b08e0 (binders 008ec9c0 / 007a69f0 respell their existing local CTCPhysicsBase declarations) |
| engine/CInputProcessTargetLockRightStickTargetSelect.h | FABLE_RETAIL_CINPUTPROCESSTARGETLOCKRIGHTSTICKTARGETSELECT_ADDTOINTERFACE together with FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE | `virtual bool AddToInterface(CInputProcessManager& manager);` | 00688a60 |
| engine/CGameCameraManager.h | FABLE_RETAIL_CGAMECAMERAMANAGER_GETRENDERCAMERA | `virtual CCamera GetRenderCamera(float interpolation) const;` before GetCameraTools | 00697f30 |
| engine/CDefinitionManager.h | FABLE_RETAIL_CDEFINITIONMANAGER_CINSTANTIATEDEFINFO_DECL (retail branch: only without FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF) | nested `class CInstantiatedDefInfo;` declaration (Ego_r.pdb), no layout change | 009b4bd0 (009b4ff0 can follow) |
| engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h | FABLE_RETAIL_CINPUTPROCESSWATCHFORWILLCHARGEUPTHIRDPERSONMODETERMINATION_ADDTOINTERFACE together with FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE (under FABLE_RETAIL_INPUTPROCESS) | `virtual bool AddToInterface(CInputProcessManager& manager);` (0x0068EAF0; Ego_r 0x006BAD70 / FableWin 0x021E9A10) | 0068eaf0 |
| engine/CUserProfileManager.h | FABLE_RETAIL_CUSERPROFILEMANAGER_AUDIODEFAULTS (with FABLE_RETAIL_CUSERPROFILEMANAGER_SETTERS for the SetDefaultValuesAudio declaration) | layout block: pad 0x24, SoundVolume +0x24, MusicVolume +0x28, DialogueVolume +0x2C, UsingHeadPhones +0x30, SoundsMuted +0x31 (Ego_r.pdb names/offsets) and the protected ApplyValues() 0x00407370 unless PASSIVEINPUTS already declares it; not combinable with the other layout blocks | 004083b0 (row + test) |
