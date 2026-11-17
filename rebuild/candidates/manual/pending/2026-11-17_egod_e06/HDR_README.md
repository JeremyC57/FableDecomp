# e06 overlay headers (copies of today's repo headers plus additive opt-ins)
- engine/CTCSkeletalMorph.h -- NEW header (no header in engine/ or _quarantine/): CTCSkeletalMorph : CTCBase, TCI_SKELETAL_MORPH, members
  Changed +0x0C / Morphs +0x10 / PSkeletalMorph +0x1C (retail sizeof 0x24). Evidence: Ego_d class record 0x2C6DD (names, order, access),
  landed 0x00786870 / 0x00786700 / 0x00787490 (offsets, interface 0x78), Ego_r method spellings. Row 0x0070E2A0.
- engine/CAnimateThing.h FABLE_RETAIL_CANIMATETHING_CALCCACHEDANIMS -- private const CalcInterpolatedMeshAnim / CalcCachedAnims declarations,
  inline GetParentThing, the four cached blend states mutable. Evidence: Ego_r ?CalcCachedAnims@CAnimateThing@@ABEXXZ (const) +
  ?CalcInterpolatedMeshAnim@..@ABE.., Ego_d params/this type. FABLE_STATIC_ASSERT(sizeof==0xbc) moved from line 107 to line 123 (no AT pin; pin_asserts.py at landing).
- engine/C3DAnimationBlendState.h FABLE_RETAIL_C3DANIMATIONBLENDSTATE_SETSKELETALMORPH -- SetSkeletalMorph(const CCountedPointer<const
  C3DSkeletalMorph>&) declaration (Ego_r name; retail 0x00A3C350 call sites in 0x0070E2A0). FABLE_STATIC_ASSERT_AT(82, ..) moved down (AT-pinned).
- stlport/fable_containers.h FABLE_RETAIL_STLP_LIST_CONST_FRONT -- STLport 4.6 `const _Tp& front() const` for list (const member reads
  DelayedAnimQueue.front(), 0x0070E2A7).
- engine/CTCWeapon.h FABLE_RETAIL_CTCWEAPON_GETCREATEDSHOTDEFGLOBALINDEX -- `long GetCreatedShotDefGlobalIndex() const` declaration (Ego_r
  ?GetCreatedShotDefGlobalIndex@CTCWeapon@@QBEJXZ; retail 0x005DA750 bound). Row 0x008BF280. No line moves of asserts.
- engine/NEntityEvents_CEventFiredShot.h -- NEW header: NEntityEvents::CEventFiredShot : CEventBase (HitBase header's), PTarget +8,
  ctor (CThing&) 0x008BFE20 declaration (FableWin ??0CEventFiredShot@NEntityEvents@@QAE@AAVCThing@@@Z, landed ctor row, Ego_r layout).
- engine/CTCTargetingPlayer.h FABLE_RETAIL_CTCTARGETINGPLAYER_FRAMEUPDATE -- virtual FrameUpdate, UpdateTargetThreatening (public), protected
  PostProcessTargetedThing declarations (Ego_r spellings; Ego_d order). Row 0x00805600. Inserted above `protected:` (asserts move down; AT-pinned ones keep their ids).
- engine/CTCTargeted.h FABLE_RETAIL_CTCTARGETED_ISZTARGETABLE_DECL -- IsZTargetable() const declared out of line (Ego_r out-of-line member
  0x007E44D0 = retail 0x007EA290; called at 0x008056F0). The default inline body is the #else (unchanged text).
- engine/CMainGameComponent.h FABLE_RETAIL_CMAINGAMECOMPONENT_GETPPLAYERINTERFACE -- GetPPlayerInterface() 0x00416359 (Ego_r + FableWin).
- engine/CPlayerGui.h FABLE_RETAIL_CPLAYERGUI_NPCNAMEDISPLAY -- SetNPCNameDisplay(const CWideString&) 0x004379D0 (Ego_r + FableWin).
- engine/CNavigatorAStar.h FABLE_RETAIL_CNAVIGATORASTAR_RANDOMPOS -- both GetRandomNavigablePos overrides (bool first, CNavigatorBase order) + private GetPosInDirection 0x00A1AD40 (Ego_r spellings, Ego_d param names). Row 0x00A1BA10 (open).
- engine/CEngineLightingManager.h FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETUPSHADERSPOTLIGHT -- both SetupShaderSpotLight overload declarations (Ego_r / FableWin). Row 0x00B47390.
- Assert-line check (non-AT FABLE_STATIC_ASSERT lines v the repo copies): CTCTargetingPlayer.h lines 94-99 -> 104-109 (inside
  CheckRetailLayout; the FRAMEUPDATE opt-in is inserted above them), CAnimateThing.h 107 -> 123; every other overlay header unchanged
  (fable_containers.h, CPlayerGui.h, CNavigatorAStar.h, CEngineLightingManager.h, CTCWeapon.h, CMainGameComponent.h,
  C3DAnimationBlendState.h, CTCTargeted.h). pin_asserts.py needed for the two moved files at landing.
