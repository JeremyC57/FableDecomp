## 0x00808E40

FABLE_RETAIL_CTCSTEALTH_ISINDANGER (engine/CTCStealth.h): inline IsInDanger() { return InDangerCoolOffTimer >= 0; } (row 0x00808E40)

## 0x0070CE10

engine/CAnimSet.h: FABLE_RETAIL_CANIMSET_UPDATEDISTANCE -- declares private UpdateDistanceMoved / inline PeekAnimInfo / inline UpdatePercentageCompleteTo (Ego_r + FableWin).
engine/C3DAnimationInfo.h: FABLE_RETAIL_C3DANIMATIONINFO_MOVEMENT -- inline GetMovementVector / GetMovementDistance in the retail (FABLE_RETAIL_C3DANIMATIONINFO) branch (FableWin).
engine/CAnimateThing.h: FABLE_RETAIL_CANIMATETHING_GETTHINGSCALE -- inline GetThingScale() returning Scale (FableWin).
stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE -- the vector fill ctor body visible but __declspec(noinline) (COMDAT call).

## 0x00949380

FABLE_RETAIL_CTCAISCRATCHPAD_GETTHANKINGPHRASE (engine/CTCAIScratchpad.h): inline GetThankingPhrase() const (FableWin.pdb ?GetThankingPhrase@CTCAIScratchpad@@QBEKXZ); row 0x00949380.

## 0x006E3580

engine/CTCTalk.h: FABLE_RETAIL_CTCTALK_PLAYSPEECH -- declares PlayLipMovement 0x6E29A0, SetAsUsingNarrator 0x6E2CF0, private GetSpeechDurationInFrames 0x6E2940 and StopCurrentSpeech(CTCSoundPlayer*) 0x6E28D0 (Ego_r/FableWin names).
engine/CTCTalk.h: FABLE_RETAIL_CTCTALK_PLAYSPEECH_INLINES -- inline bodies of IsTalking (0x6E28A0) and SetAsUsingNarrator (0x6E2CF0) after the class (retail expands both in PlaySpeech).
engine/CMainGameComponent.h: FABLE_RETAIL_CMAINGAMECOMPONENT_SERVERFRAME_OUTOFLINE -- out-of-line GetCurrentServerFrame() const 0x416225 when EVENTPACKAGES is off.
engine/CTCSoundPlayer.h: FABLE_RETAIL_CTCSOUNDPLAYER_BANKSAMPLE -- PlayAttachedSample(CASoundBank&, const CSoundPair&, const CSoundInit&, bool,bool,bool) 0x66B300 and SetSpecificSamplePitch(unsigned long,float) 0x66AFA0.
engine/NSpeechGainManager.h: FABLE_RETAIL_NSPEECHGAINMANAGER_ONDIALOGUESTART -- __fastcall OnDialogueStart(const CWorld&, CThing&, unsigned long, EDialogueLayer) 0x6E31E0.

## 0x00966AF0

engine/CAIStateGroup_TavernServer.h: FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_GETBAR declares bool GetBar() (0x00966AF0, Ego_r ?GetBar@CAIStateGroup_TavernServer@@QAE_NXZ)

## 0x006992A0

- engine/CGameCameraManager.h: FABLE_RETAIL_CGAMECAMERAMANAGER_DEBUGPOINTS -- DebugPoints +0x15C typed std::list<std::pair<C3DVector,CRGBColour> > (Ego_r.pdb) and the virtual const PreparePrimitivesForRendering (slot 2, 0x006992A0) declared; use with _ANGLES (vptr replaces __vftable). No layout change.
- engine/NCameraModes_CBase.h: FABLE_RETAIL_NCAMERAMODES_CBASE_GETPOSITION_INLINE (CYLINDER branch) -- GetPosition() const { return GoTo.Position; } inline (retail reads mode +0x54 in place at 0x0069938C).

## 0x00929340

- engine/CAIStateGroup_BanditPlayDice.h: FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUALS -- declares the protected virtual slots 12..16 (Update, InitStateGroup, GetDiceGame, GetMarkerType, PutWeaponAway; Ego_r.pdb) ahead of Act (row 0x00929340).
- engine/CCreatureAction_UnsheatheWeapons.h: FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS -- polymorphic retail class (: CCombatActionBase, WeaponType +0x114, sizeof 0x118, ctor(CThingCreatureBase&) 0x008B8590) (row 0x00929340).

## 0x009F4390

engine/CPixelFormat.h (copy of current repo header) + FABLE_RETAIL_CPIXELFORMAT_SIGNED: decls IsEquivalent(const CPixelFormat&) const 0x009E37F0, IsSigned() const 0x009E3970, inline GetAsD3DPixelFormat() (FableWin.pdb, debug-only body) -- row 0x009F4390.
engine/CSurface.h (copy of current repo header) + FABLE_RETAIL_CSURFACE_DIMENSIONS_INLINE: inline GetDimensions() body (GetDesc + C2DExtentsI(Width,Height)) inside the _BITMAP block -- row 0x009F4390.
engine/CSurface.h + FABLE_RETAIL_CSURFACE_COPYFROMMEMORY_BODY: inline GetPixelFormat() body, decls CopyFromMemory(4-arg) 0x009F3B00, private CanDoProperSignedConversion 0x009F35D0 / CopyFromUnsignedDataToSignedSurface 0x009F36E0 (elif before _PIXELFORMAT) -- row 0x009F4390.

