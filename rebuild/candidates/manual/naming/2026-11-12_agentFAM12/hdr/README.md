# agentFAM12 header overlay

Starts as FAM11's overlay (hdr_base_FAM11/, unchanged). Every header added here is a full copy of the CURRENT repo header with
`#ifdef` blocks added and nothing else changed. Grade with FABLE_HDR_OVERLAY pointing at this folder.

## Headers changed beyond FAM11's copy (land these)

| Header | Opt-in macro | Adds | Used by row |
|---|---|---|---|
| stlport/fable_map.h | FABLE_RETAIL_STLP_TREE_ERASE_BODY (with FABLE_RETAIL_STLP_TREE_ERASE) | `_Rb_global::_Rebalance_for_erase` declaration (out of line 0x0042A34E) and the STLport 4.6 erase(iterator) body (rebalance, _Destroy, deallocate, --count) in place of the declaration | 0x004C3F90, 0x004C6810, 0x00638EE0, 0x007775B0, 0x007E99F0, 0x009D7760 (erase(first,last), with FABLE_RETAIL_STLP_TREE_ERASE_KEY + FABLE_RETAIL_STLP_TREE_POSTINC) |
| engine/CTCTavernGame.h | FABLE_RETAIL_CTCTAVERNGAME_REGISTERGAMEMASTER | `void RegisterGameMaster(CThingAICreature*);` (Ego_r/FableWin ?RegisterGameMaster@CTCTavernGame@@QAEXPAVCThingAICreature@@@Z) | 0x0094D430 |
| engine/CAInputProcess.h | FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE | `void RemoveFromInterface();` (Ego_r ?RemoveFromInterface@CAInputProcess@@QAEXXZ, 0x00A0D2D0) | 0x006336E0, 0x006336F0, 0x00633780, 0x006337B0, 0x00633BD0 |

FAM11's overlay headers (README below) are kept as they were.

---

# agentFAM10 header overlay

Every header here is a full copy of the CURRENT repo header (rebuild/include) with `#ifdef` blocks added and nothing else changed,
except NUISystem_CText.h, whose default slot order is fixed on purpose (queue entry land1011dl). Every row was graded with
FABLE_HDR_OVERLAY pointing at this folder. NF26's ten headers (CDefinitionManager, CLevelLoader, CMainGameComponent, CScriptThing,
CSymbolMap, CTCCreatureModeManager, CTCVillage, CTCWeapon, CTCWife, CWeaponDef) are now identical to the repo (NF26 landed): they are
plain copies, nothing to land for them.

NPlayerGui_CDrawBase.h was re-based on 2026-11-12 onto the repo copy that gained Q22's FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
block; the 12 CDraw rows were regraded on the re-based copy (all RELOCATION_MATCH, tests PASS).

## Headers changed beyond NF26 (land these)

| Header | Opt-in macro | Adds | Used by row |
|---|---|---|---|
| engine/NUISystem_CText.h | (default changed) | slots 147-150 in retail order unconditionally: SetTextWindow 0x0054FFB0, GetTextWindow 0x0054FFE0, ForceWidth 0x0054FB10, GetAlignment 0x0054FFF0. FABLE_RETAIL_CTEXT_WINDOW_SLOTS no longer changes anything | all 34 landed includers graded, see below |
| engine/CPlayer.h | FABLE_RETAIL_PLAYER_FREEZECONTROLS (inside FABLE_RETAIL_PLAYER) | `UnfreezeControls()` 0x00487120, `FreezeControls()` 0x00487140, `SpecialAbilityUnfreezeControls()` 0x00487150, `SpecialAbilityFreezeControls()` 0x00487170 (declarations) | 0x00487120, 0x00487150, 0x007A75F0, 0x007AA370 |
| engine/NPlayerGui_CDrawBase.h | FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_IMPLICIT_DTOR | omits the `~CDrawBase();` declaration (`#else` keeps it): the implicit destructor, no vptr store | 0x00443580..0x00443A60 (12 CDraw DeleteData rows) |
| engine/CArray.h | FABLE_RETAIL_CARRAY_SIZE_CTOR_FORCEINLINE | `FABLE_FORCEINLINE explicit CArray(unsigned int n)` (and `#include "fable_compiler.h"`); per-TU inline-budget opt-in | 0x004538EB |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_VECTOR_FILL_OUTOFLINE | the vector fill ctor `vector(size_type, const _Tp&, const _Alloc&)` as a declaration only (out of line, as retail calls 0x004516E7) | 0x004538EB |
| engine/CFontManager.h | FABLE_RETAIL_CFONTMANAGER_VIRTUAL_DTOR | `virtual ~CFontManager();` | 0x009A6C10 |
| engine/CUserProfileManager.h | FABLE_RETAIL_CUSERPROFILEMANAGER_VIRTUAL_DTOR | `virtual ~CUserProfileManager();` (declaration only; do not combine with the VOLUME / SCOREBOARD layout opt-ins) | 0x0040D830 |
| engine/CGameJoystickManager.h | FABLE_RETAIL_CGAMEJOYSTICKMANAGER_VIRTUAL_DTOR | `virtual ~CGameJoystickManager();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x0049BC20 |
| engine/CWADBank.h | FABLE_RETAIL_CWADBANK_VIRTUAL_DTOR | `virtual ~CWADBank();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x0050BCC0 |
| engine/CCreatureStatsDef.h | FABLE_RETAIL_CCREATURESTATSDEF_VIRTUAL_DTOR | `virtual ~CCreatureStatsDef();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00672AA0 |
| engine/CMeshBankManager.h | FABLE_RETAIL_CMESHBANKMANAGER_VIRTUAL_DTOR | `virtual ~CMeshBankManager();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x009A6BF0 |
| engine/CFontDataBank.h | FABLE_RETAIL_CFONTDATABANK_VIRTUAL_DTOR | `virtual ~CFontDataBank();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x009E2960 |
| engine/CStreamingFontDataBank.h | FABLE_RETAIL_CSTREAMINGFONTDATABANK_VIRTUAL_DTOR | `virtual ~CStreamingFontDataBank();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x009E2970 |
| engine/CTextureManager.h | FABLE_RETAIL_CTEXTUREMANAGER_VIRTUAL_DTOR | `virtual ~CTextureManager();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x009FFD20 |
| engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManagerRepeatedStaticMeshes();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B36080 |
| engine/CEnginePrimitiveManagerRepeatedZSprites.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDZSPRITES_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManagerRepeatedZSprites();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B36090 |
| engine/CEnginePrimitiveManagerSprites.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSPRITES_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManagerSprites();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B360A0 |
| engine/CEnginePrimitiveManagerFlareSprites.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERFLARESPRITES_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManagerFlareSprites();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B360B0 |
| engine/CEnginePrimitiveOcclusionGroupManager.h | FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONGROUPMANAGER_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveOcclusionGroupManager();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B36100 |
| engine/CEnginePrimitiveManagerParticleLineGroup.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLELINEGROUP_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManagerParticleLineGroup();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B36140 |
| engine/CEnginePrimitiveManagerParticleMeshGroup.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEMESHGROUP_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManagerParticleMeshGroup();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B36150 |
| engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITETRAILGROUP_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManagerParticleSpriteTrailGroup();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B36170 |
| engine/CEnginePrimitiveManagerParticleDecalGroup.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManagerParticleDecalGroup();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B361A0 |
| engine/CEnginePrimitiveManager2DSprites.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DSPRITES_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManager2DSprites();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B4B040 |
| engine/CEnginePrimitiveManager2DParticleGroup.h | FABLE_RETAIL_CENGINEPRIMITIVEMANAGER2DPARTICLEGROUP_VIRTUAL_DTOR | `virtual ~CEnginePrimitiveManager2DParticleGroup();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00B4B060 |
| engine/CParticleSimulationRenderData.h | FABLE_RETAIL_CPARTICLESIMULATIONRENDERDATA_VIRTUAL_DTOR | `virtual ~CParticleSimulationRenderData();` in place of the flattened `void* __vftable;` (same layout; the offsetof assert on the old member is guarded) | 0x00BB6330 |

All VIRTUAL_DTOR blocks: Ego_r.pdb / RTTI give the class a vptr at +0 and the deleter calls vtable slot 0 with flag 1 (scalar
deleting destructor), which a flattened `void* __vftable` member cannot emit.

## NUISystem_CText.h includers graded with the fixed default (all MATCH / RELOCATION_MATCH, tests PASS)

0x0042A2CA 0x0054FF90 0x0054FFA0 0x0054FF50 0x00427216 0x005A8C66 0x005AF64E 0x0058AF10 0x005A5B31 0x0041E6D3 0x0056AD40 0x005F519F 0x005C8459 0x005C9A95 0x005B8743 0x005B690B 0x005B6A66 0x005B76D1 0x005C574E 0x005C5F11 0x005C90FC 0x005CACB9 0x005F077F 0x005F1526 0x005F3F7B 0x005F45B5 0x005F4C8F 0x005A3734 0x005A8175 0x005B0AAC 0x005A29F4 0x0047E651 0x0047E9AE 0x005E4BE0

0x00427216, 0x005F519F and 0x005E4BE0 are BUSY rows: graded read-only from their repo sources, not changed.

# agentNF27 additions (2026-11-12)

The FAM10 overlay above is kept as is, except engine/CTCVillage.h: it was a plain copy that had fallen behind the repo (Q23 added
FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE), so it is now the current repo copy again (nothing to land for it). Every header below is a
full copy of the CURRENT repo header with `#ifdef` blocks added and nothing else changed. All NF27 rows were graded with
FABLE_HDR_OVERLAY pointing at this folder.

## Headers changed beyond FAM10's copy (land these)

| Header | Opt-in macro | Adds | Used by row |
|---|---|---|---|
| engine/CPhysicsMeshObject.h | FABLE_RETAIL_CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS | `IsSpherePartContainedInBoundingBox(const C3DVector&, float) const` 0x00A11F20, plus `IsPointContainedInBoundingBox(const C3DVector&) const` 0x00A12890 only when Q23's FABLE_RETAIL_CPHYSICSMESHOBJECT_NAVIGATION (which also declares it) is not defined. Re-based on the repo copy with Q23's block | 0x00A12890, 0x0063F150, 0x0063F1E0 |
| engine/CPlayer.h | FABLE_RETAIL_PLAYER_INITIALISEIFUNINITIALISED (inside FABLE_RETAIL_PLAYER) | `void InitialiseIfUninitialised();` 0x00488A80 (Ego_r 0x004955E0, FableWin 0x01ACEE50). FAM10's FREEZECONTROLS block kept | 0x006E7630 |
| engine/CPlayerManager.h | FABLE_RETAIL_CPLAYERMANAGER_FREEZEPLAYERCONTROLS (both the retail and the flattened branch) | `FreezePlayerControls(long)` 0x00449A30, `UnfreezePlayerControls(long)` 0x00449A50 | 0x00449A30, 0x00449A50 |
| engine/CTCShapeManager.h | FABLE_RETAIL_CTCSHAPEMANAGER_GETINSTANTIATIONFROMENUM (needs FABLE_RETAIL_CTCSHAPEMANAGER) | includes CCountedPointer.h / C3DVector.h / fable_containers.h; `namespace NShape { class CShape; enum EShape; }`; `CCountedPointer<NShape::CShape> GetInstantiationFromEnum(NShape::EShape, std::vector<C3DVector>&)` 0x007624E0 | 0x007624E0 |
| fable_user_profile.h | FABLE_RETAIL_CUSERPROFILEMANAGER_PCLASS (inside the retail CUserProfileManager) | includes engine/CCountedPointer.h; `private: static CCountedPointer<CUserProfileManager> PClass;` 0x013B7D4C (FableWin "dynamic initializer for 'CUserProfileManager::PClass''") | 0x0040D2A0 |
| candidates/mouse_dx_accessors_compiled.h | FABLE_RETAIL_CMOUSEDX_MOUSECURSOR | defines FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL and includes CCountedPointer.h; CSurface gets its Ego_r fields (`_padding_`, PD3DSurface +4, AllocationSource, PAllocatedMemory); `CCountedPointer<CSurface> MouseCursor;` in place of CCountedSurfacePointer | 0x00AB5390 |

## Comment debt (no code change)

engine/CCountedPointer.h line ~124 names 0x008AB3D0 as `??1?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@XZ`. The RELOC_CONFLICT
resolution (RENAMES section 1) makes 0x008AB3D0 the CModeSitFloor destructor and 0x008AB320 the CModeSitBase one; the comment should
be respelled when the rows land.

## agentFAM11 (2026-11-12): headers changed beyond NF27's copy (land these)

The rest of this folder is NF27's overlay, unchanged (diff -r against hdr_base_NF27 shows only the three files below). All FAM11 rows were
graded with FABLE_HDR_OVERLAY pointing here.

| Header | Opt-in macro | Adds | Used by row |
|---|---|---|---|
| engine/CTCBoastingPosition.h | FABLE_RETAIL_CTCBOASTINGPOSITION_PBOASTINGPODIUM | `CIntelligentPointer<CThing> PBoastingPodium;` (+0x0c, Ego_r.pdb type) in place of the opaque `unsigned char PBoastingPodium[0x8]` (`#else` keeps it); needs engine/CIntelligentPointer.h with FABLE_RETAIL_CINTELLIGENTPOINTER first | 0x008147D0 |
| engine/CThingSearchTools.h | FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA_CONSTPROC | the ForEachInRoughArea declaration with the PDB signature (QBE, `const Proc&`); `#else` keeps the old `Proc&` declaration | 0x0063E590 |
| engine/NTCScriptedControl_CScriptConversation.h | (new header) | `namespace NTCScriptedControl { class CScriptConversation { public: virtual ~CScriptConversation(); }; }` -- the TC class, distinct from ::CScriptConversation (RTTI vtable 0x01265520, slot 0 = 0x00718840; ??1 in Ego_r.pdb and FableWin.pdb) | 0x00718080 |

Not edited (recorded only): the engine/CCountedPointer.h comment near line 124 should name 0x008AB3D0 as
??1?$CCountedPointer@VCModeSitFloor@NCreatureMode@@@@QAE@XZ (CModeSitFloor, not CModeSitBase); a comment fix is not an `#ifdef` addition.
