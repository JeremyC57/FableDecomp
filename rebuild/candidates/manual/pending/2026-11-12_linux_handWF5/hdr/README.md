## 0x009ADAE0

- engine/CDefinitionManager.h: `FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATESUBDEF` (row 0x009ADAE0) -- declares InstantiateSubDef 0x009ADAE0 and LoadDef(unsigned long) const 0x009AD6E0, and gives the protected non-const GetPDefFromGlobalIndex(long) its inline body (LoadDef + copy of InstantiatedDefs[index]->PDef). Default path unchanged.
- engine/CDefClassBase.h: `FABLE_RETAIL_CDEFCLASSBASE_SETASTEMPLATE` (row 0x009ADAE0; inside the FABLE_RETAIL_DEF_CLASS_BASE layout) -- inline `void SetAsTemplate(bool is_template) { Template = is_template; }` (FableWin.pdb ?SetAsTemplate@CDefClassBase@@QAEX_N@Z 0x03060C40). Default path unchanged.
- (repair 2026-11-12) engine/CDefinitionManager.h rebased onto the current repo header (keeps NF28's two FABLE_RETAIL_CDEFINITIONMANAGER_CINSTANTIATEDEFINFO_DECL blocks); the overlay now differs from the repo only by the FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATESUBDEF additions.

## 0x00499AA0

# Header overlay for row 0x00499AA0 (CProgressDisplay::StartProgress)
Empty: the row matches on the current repo headers (no new opt-in; a trial CCountedPointer operator= noinline opt-in was not needed and was removed).

## 0x00EF4320

No header overlays: this row uses only opt-ins that already exist in the repo headers.

## 0x00A11D10

# Header overlay for row 0x00A11D10 (wf5)
- engine/CMatrix3x4.h: `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS` -- the two-argument `TransformPoint(const C3DVector&, C3DVector&) const` (FableWin.pdb ?TransformPoint@CMatrix3x4@@QBEXABVC3DVector@@AAV2@@Z) as FABLE_FORCEINLINE with three plain `E1k*x + E2k*y + E3k*z + E4k` rows; default body unchanged in the `#else`.
- engine/CPhysicsMesh.h: `FABLE_RETAIL_CPHYSICSMESH_NAVIGATION` -- NavigationContainmentVolumes typed as its Ego_r.pdb `CVectorMap<long,C3DMeshVolume,CKeyPairCompareLess<long,C3DMeshVolume> >` (same offset 0x54 and size 0x10; C3DMeshVolume forward-declared), inline `GetNavigationContainmentVolumes() const` (FableWin.pdb) and the declaration `long GetNavigationLevelAt(const C3DVector&) const` (0x00A73D10); default layout unchanged.

## 0x007F53B0

stlport/fable_map.h: FABLE_RETAIL_STLP_MULTIMAP_CONSTFIND -- multimap gains the STLport inline `const_iterator find(const key_type&) const` / `end() const` (+ const_iterator typedef); default path unchanged (row 0x007F53B0).
stlport/fable_map.h: (repair 2026-11-12) overlay rebased onto the current repo header -- it is the repo file plus only the FABLE_RETAIL_STLP_MULTIMAP_CONSTFIND block (diff vs repo: 0 removed lines), so it keeps row 0x004B3CE0's FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE opt-in.

## 0x009692C0

# wf5 row 009692c0 header overlay (full copies of the current repo headers, additions only)
- engine/CAIStateGroupBase.h: `FABLE_RETAIL_CAISTATEGROUPBASE_SETPUSHABLEBYHERO` (inside _MEMBERS, protected) declares `void SetPushableByHero(bool)` 0x0097E330 (?SetPushableByHero@CAIStateGroupBase@@IAEX_N@Z, Ego_r + FableWin).
- engine/CAIStateGroup_TavernKeeper.h: `FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER_RECEIVEPAYMENT` (inside FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER, public) declares `bool WalkToPoint(const C3DVector&, const CRightHandedSet&)` 0x00968230, `void FreeBarPoint()` 0x00968950, `bool ReceivePayment()` 0x009692C0.

## 0x0094D780

# wf5 0094d780 header overlay
- engine/CAIStateGroup_TrollBase.h: `FABLE_RETAIL_CAISTATEGROUP_TROLLBASE` -- the polymorphic class on its real base CAIStateGroupBase (RTTI; vtable 0x0128C008 slot 1), Ego_r members at the same offsets; default flat struct unchanged.
- engine/CExplosionDef.h: `FABLE_RETAIL_CEXPLOSIONDEF_MEMBERS` -- Damage +0x28 / ExplosionRadius +0x2C / FireDamage +0x30 and the inline StatGetTypeName; default padding unchanged.

## 0x0064C869

# wf5 0064c869 header overlay (full copies of the current repo headers + opt-ins; default paths unchanged)
- engine/NPlayerGui_CDrawSpiritTargetingCursor.h -- NEW shared header: NPlayerGui::CDrawSpiritTargetingCursor : CDrawBase, Draw 0x0064C869, PTargetedThings (vector<CIntelligentPointer<CThing>>) +0x28, sizeof 0x34; needs FABLE_RETAIL_CINTELLIGENTPOINTER.
- engine/CDisplayEngine.h -- FABLE_RETAIL_CDISPLAYENGINE_ISLETTERBOXMODE: inline `bool IsLetterBoxMode() const` (FableWin ?IsLetterBoxMode@CDisplayEngine@@QBE_NXZ), reads the first byte of LetterBox +0x70.
- engine/CPlayer.h -- FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE: declaration `bool IsMainPlayer() const;` (0x00487520) for callers of the out-of-line copy (inactive when FABLE_RETAIL_PLAYER_SWITCHCHARACTERS already declares it).
- engine/CPlayerManager.h -- FABLE_RETAIL_CPLAYERMANAGER_MULTIPLAYERCOLOUR: declaration `CRGBColour GetMultiplayerColour(long) const;` (0x00449B60) in both branches of the header.

## 0x0095E0B0

# Header overlay for row 0x0095E0B0 (full copies of the current repo headers plus opt-ins; default paths unchanged)
- engine/CAIStateGroupBase.h: `FABLE_RETAIL_CAISTATEGROUPBASE_PROJECTILESTRAFETHING` (with _MEMBERS + _AIACTIONS) declares the protected `AIAction_ProjectileAttackAndStrafeToThingProximity` (0x0097EF60, both PDBs).
- engine/CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets.h: `FABLE_RETAIL_CAISTATEGROUP_CREATURERANGEDCOMBATFIREATSPECIFICTARGETS` = the polymorphic retail class over CAIStateGroupBase with `CIntelligentPointer<const CThing> PTarget` (+0x14) and the Ego_r protected virtuals.
- engine/CCreatureAction_AimMissileWeapon.h: `FABLE_RETAIL_CCREATUREACTION_AIMMISSILEWEAPON` = the retail class over CCreatureActionBase (ctor 0x00718650, Clone, PAmmo +0xa8, implicit dtor).

## 0x00A11F20

engine/CPhysicsMesh.h: FABLE_RETAIL_CPHYSICSMESH_BOUNDINGBOXINTERSECTED -- declares CPhysicsMesh::HasBoundingBoxIntersectedFromTo(float,const C3DVector&,const C3DVector&) const (0x00A17690, Ego_r + FableWin); declaration only, default path unchanged.
engine/CMatrix3x4.h: FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_SUMS -- two-argument TransformPoint(pSource,destination) as FABLE_FORCEINLINE plain left-to-right sums (row 0x00A11F20); body only, default (accumulated, plain inline) path unchanged.

