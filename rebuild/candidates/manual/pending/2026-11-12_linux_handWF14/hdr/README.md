## 0x00959800

- engine/CAIStateGroup_HoverWanderAroundThing.h: FABLE_RETAIL_CAISTATEGROUP_HOVERWANDERAROUNDTHING -- polymorphic class on CAIStateGroup_HoverMovementBase with PThing CIntelligentPointer<const CThingGameObject> +0x1c and virtual IsRunnable (row 0x00959800); default path unchanged.

## 0x004C9920

- engine/CThingManager.h: FABLE_RETAIL_CTHINGMANAGER_PEEKPLAYERMANAGER -- inline PeekPlayerManager() const / GetPlayerManager() (FableWin.pdb), row 0x004C9920.
- engine/CTCDParticleEmitter.h: FABLE_RETAIL_CTCDPARTICLEEMITTER_ATTACHED_INLINE -- inline IsAlwaysUpdating() const / PeekPThingAttachedTo() const (FableWin.pdb), row 0x004C9920.

## 0x006AC430

- engine/CThingPlayerCreature.h: FABLE_RETAIL_CTHINGPLAYERCREATURE_EXPRESSIONS declares IsExpressionAvailable (QBE, 0x006AC390) / LearnExpression (0x006AC430).
- engine/CTCInventoryItem.h: FABLE_RETAIL_CTCINVENTORYITEM_TUTORIALCATEGORY declares GetTutorialCategory() const (0x005D92B0).
- engine/CTCInventory.h: FABLE_RETAIL_CTCINVENTORY_NEXTFREESLOT declares GetQuickAccessNextFreeSlot(long) const (0x00582027).
- engine/CTCInventoryAbilities.h: FABLE_RETAIL_CTCINVENTORYABILITIES_LEARNEDEXPRESSION declares AddLearnedExpression(long) (0x005EEDCF).
- engine/CExpressionDef.h: FABLE_RETAIL_CEXPRESSIONDEF_DEFINDEX types DummyObject (+0x70) as its Ego_r.pdb type CDefIndex (engine/CDefIndex.h).

## 0x0076C9A0

- engine/CDefString.h: FABLE_RETAIL_CDEFSTRING_NE_CHARSTRING declares CDefString::operator!=(const CCharString&) const (0x0076D640, Ego_r ??9CDefString@@QBE_NABVCCharString@@@Z).

## 0x00AF5430

engine/CDecal.h: FABLE_RETAIL_CDECAL_CLEAR -- inline FableWin.pdb accessors GetParentHandle / GetPrimID / ClearParentHandle (FABLE_FORCEINLINE) / ClearAll (row 0x00AF5430).
engine/CParticleSystem.h: FABLE_RETAIL_CPARTICLESYSTEM_CLEARDECAL -- ClearDecal(list<CDecal*>::iterator&) declaration (row 0x00AF5430).

## 0x00B68DA0

engine/CEngineLandscapeRenderer.h: FABLE_RETAIL_CENGINELANDSCAPERENDERER_RENDERSORTED declares private SetupCommonStates/PostRenderClearRenderStates/RenderSortedPatches (Ego_r/FableWin AAEXXZ)
engine/CEngineLightingManager.h: FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD (needs _MEMBERS) gives the PDB inline PeekGlobalShadowMode() and SetEnableFadedShadowFactor with its `if (PeekGlobalShadowMode() != 1) enable = false;` guard (FableWin; out-of-line copy 0x00B66EB0); also corrects the 'no out-of-line copy' comment

## 0x00941690

engine/CCreatureAction_PlayHopscotch.h: FABLE_RETAIL_CCREATUREACTION_PLAYHOPSCOTCH -- the class derived from CCreatureActionBase with its ctor 0x00941B20 (CThingCreatureBase&).

## 0x009C4440

- (coordinator, at landing) No header change: the row uses the landed opt-in FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE (wf13, 0x009CCEF0); 0x0099ED10 is ?GetAsLowerCase@CCharString@@QBE?AV1@XZ in both PDBs, so the ToLower opt-in was dropped (one address, one name).

