## 0x008DA410

- engine/NCreatureMode_CModeScreamerAggressive.h: NEW header (no repo header existed): NCreatureMode::CModeScreamerAggressive : CModeStandBase, Ego_r layout (Filter +0x68, PEffect +0x74, PTarget +0x7C, sizeof 0x84); vtable 0x012681EC. Row 0x008DA410.
- engine/CModeStandBase.h: FABLE_RETAIL_CMODESTANDBASE_ACTIVATE -- declares virtual OnActivate() (0x008621A0, ?OnActivate@CModeStandBase@@UAEXXZ) for the qualified base call. Row 0x008DA410.
- engine/CTCDParticleEmitter.h: FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE -- declares static __fastcall Create(long, const C3DVector&, bool) (0x006E0880, Ego_r ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z). Row 0x008DA410.
- engine/CTCCombat.h: FABLE_RETAIL_CTCCOMBAT_READYFORMELEE -- public inline SetIsReadyForMelee(bool) (Ego_r/FableWin ?SetIsReadyForMelee@CTCCombat@@QAEX_N@Z). Row 0x008DA410.

## 0x0079CC80

engine/CTCBulletTime.h: FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT -- PDef typed CDefPointer<const CSpecialAbilitiesBulletTimeDef> at +0x20 (in place of PDef_Object) + private PlayEffect declaration (row 0x0079CC80)

## 0x00AABD50

engine/LHA_CEventPlayOptions.h: FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_SENDFADERS declares the out-of-line send-fader setters 0x00C0A8A0 / 0x00C0A8C0 (landed as CBitMaskPair_00c0a8c0::SetFlag16 / SetFlag).
  The two setter names are descriptive (no PDB method names for libpk/LHA); their class is LHA::CEventPlayOptions by Ego_r offsets -- NAME_FIXES.tsv respells the landed CBitMaskPair_00c0a8c0 rows.

## 0x009810B0

CAIStateGroupBase.h: FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD -- nested CAIFilterBase + AIAction_FleeToThingProximity / AIAction_FindAGuardAndGiveMessage declarations
CThingSearchTools.h: FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF_DECL -- GetPBestLeafThing template declared only (out-of-line COMDAT 0x009823D0)
CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE -- inline converting ctor SetPItem(other.GetPItem())
CCreatureActionBase.h: FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR -- ctor (CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) 0x00693B30 + protected static __fastcall GetRandomAnimFromName 0x006924B0, declarations only
CCreatureAction_ShowFear.h: FABLE_RETAIL_CCREATUREACTION_SHOWFEAR_DERIVED -- polymorphic class deriving the retail CCreatureActionBase, ctor (CThingAICreature&) 0x00842270, PerformAction/Clone overrides declared
(CDefPointer.h experiments were NOT needed; moved to unused/, not part of the overlay)

## 0x00704860

- engine/CGameScriptThing.h: FABLE_RETAIL_CGAMESCRIPTTHING_MANAGER_FRIEND -- friend class CGameScriptThingManager (private ctors; row 0x00704860).

## 0x00E36750

engine/CV_ArcheryCompetitionScript.h: FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_DOCOUNTDOWN declares `void DoCountDown(long);` (Ego_r + FableWin ?DoCountDown@CV_ArcheryCompetitionScript@NScript@@QAEXJ@Z).

