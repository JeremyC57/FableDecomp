# Chunk 2026 header overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2026/hdr`)

Every change to a copied repo header (HEAD 2026-11-11) is an opt-in `#ifdef FABLE_RETAIL_...` block (or a retail-class branch in front of
the generated default, `#ifdef ... #else <generated> #endif`), so landed includers keep their bytes; new headers are only included by this
chunk's rows. Evidence: two sources each (PDB name set / TPI layout + retail bytes, RTTI, donor order). Both rows regraded after the last
header change (gall.sh: 0x0092DA20 and 0x0094E7B0 RELOCATION_MATCH).

## Copied unchanged from the chunk 2017 hand-back (prior/hdr)
| header | note |
|---|---|
| engine/CAIStateGroupBase.h | HEAD + chunk 2017's FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT (protected `AIAction_PerformAction(const CCreatureActionBase&)`, retail 0x0097D8B0), used by 0x0094E7B0 |

## New headers
| header | content | evidence |
|---|---|---|
| engine/CTCBed.h | CTCBed : CTCBase, TCI_BED (0x35); Empty +0x0C, GettingIn, GettingOut, Animating, BedSleepAnim +0x10, PDef (CDefPointer<const CBedDef>) +0x14; inline GetDefinition() const (returns PDef by value), StartGetOutOfBedAnimation(long) decl (0x00746A70) | RTTI .?AVCTCBed@@ : CTCBase; Ego_r TPI members/offsets (sizeof 0x18); Ego_r/FableWin ?GetDefinition@CTCBed@@QBE?AV?$CDefPointer@$$CBVCBedDef@@@@XZ, ?StartGetOutOfBedAnimation@CTCBed@@QAEXJ@Z; retail 0x0092DC58 (`mov edi,[tc+14h] ; test ; inc [edi+4]`), landed 0x00746A70 |
| engine/CBedDef.h | CBedDef : CSubDefClassBase; CDefString GetInBedAnimName +0x28, SleepInBedAnimName +0x2C, GetUpFromBedAnimName +0x30, GetUpFromBedFastAnimName +0x34, bool Animating +0x38 | RTTI .?AVCBedDef@@ : CSubDefClassBase; Ego_r TPI (sizeof 0x3C); retail reads of +0x28/+0x2C/+0x30/+0x34 in 0x0092DA20 and +0x38 in 0x00746A70 |
| engine/CCreatureAction_SheatheWeapons.h | : CCombatActionBase; ctor (CThingCreatureBase&) decl 0x008CC370; PlayedSound +0x114 (sizeof 0x118) | RTTI vtable 0x01282CEC; Ego_r/FableWin ??0CCreatureAction_SheatheWeapons@@QAE@AAVCThingCreatureBase@@@Z (already bound); landed ctor stores +0x114 |

## Opt-ins added to copies of repo headers
| header | opt-in | change |
|---|---|---|
| engine/CAIStateGroup_MinionSleep.h | FABLE_RETAIL_CAISTATEGROUP_MINIONSLEEP | retail class : CAIStateGroup_GoToBed (RTTI), public virtuals InitCreaturePosition / IsStillValid / OnReplace / IsRunnable / Update / InitStateGroup (vtable 0x0128B140 slots 3/5/10/11/12/13, Ego_r+FableWin UAE/UBE spellings), public `bool IsInSleepTime() const` (0x0092C520: Ego_r donor order + the body compiled from this TU grades RELOCATION_MATCH against 0x0092C520), protected WasCarryingWeapon +0x60 (Ego_r, sizeof 0x64) |
| engine/CAIStateGroup_TrollBase.h | FABLE_RETAIL_CAISTATEGROUP_TROLLBASE | retail class : CAIStateGroupBase; Ego_r members (PEnemy +0x14, EnemyKnockDownFrame, ShotCounter, the four TROLL_*_DISTANCE floats; sizeof 0x34); public virtual Initialise, public IsStillRunnable, protected GetEnemyPredictedPos / HasEnemyBeenKnockedDown / ForceRiseUp / ThrowRocks / GroundPound (Ego_r spellings; donor order = retail 0x0094D680 .. 0x0094DC40) |
| engine/CAIStateGroup_EarthTroll.h | FABLE_RETAIL_CAISTATEGROUP_EARTHTROLL | retail class : CAIStateGroup_TrollBase; protected virtual GetStateGroupType (slot 8) / IsRunnable (slot 11) / Update (slot 12) (Ego_r/FableWin MAE/MBE spellings, vtable 0x0128C044) |
| engine/CCreatureAction_GetInBed.h | FABLE_RETAIL_CCREATUREACTION_GETINBED | retail class : CCreatureActionBase with the INLINE ctor (body = landed 0x0092E300, the COMDAT this TU emits and calls), PBed / AnimGroup / SleepAnimName / BedGetInAnim / BedSleepAnim at +0xA8..+0xBC, PDB virtual decls; implicit dtor (retail destroys the temporary inline) |
| engine/CCreatureAction_GetOutOfBed.h | FABLE_RETAIL_CCREATUREACTION_GETOUTOFBED | retail class : CCreatureActionBase, ctor (CThingCreatureBase&, CThing&, const CCharString&) decl 0x00860060 (PDB reference spelling), PBed +0xA8, PDB virtual decls; implicit dtor |
| engine/CCreatureAction_UnsheatheWeapons.h | FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS | retail class : CCombatActionBase, ctor (CThingCreatureBase&) decl 0x008B8590, WeaponType +0x114 |
| engine/CCreatureAction_TrollGroundPound.h | FABLE_RETAIL_CCREATUREACTION_TROLLGROUNDPOUND_CTOR | ctor (CThingCreatureBase&, CThingCreatureBase&) decl 0x0094E5F0 (Ego_r/FableWin spelling; landed with it) |
| engine/CCreatureActionBase.h | FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR | ctor (CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) decl 0x00693B30 (Ego_r donor 0x0051B170) for the inline GetInBed ctor |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_BEDSLEEP | SheatheAllWeapons() decl 0x006647F0 (+ HasAnim decl 0x00662C70 unless ANIMQUERY declares it) |
| engine/CThingCreatureBase.h | FABLE_RETAIL_CTHINGCREATUREBASE_SETHANDEDNESS | SetLastActionHandedness(ECombatAnimationHandedness) decl 0x006619F0 (MS forward enum) |
| engine/CThingAICreature.h | FABLE_RETAIL_CTHINGAICREATURE_SLEEPTIMES | `void GetSleepTimes(long&, long&) const` decl 0x00833390 (Ego_r/FableWin ?GetSleepTimes@CThingAICreature@@QBEXAAJ0@Z) |
| engine/CTCSleep.h | FABLE_RETAIL_CTCSLEEP_READY | inline GetIsReadyToSleep() (`mov al,[sleep+0Ch]`), GetFastTrack() decl 0x0092C3B0 (both PDBs, QAE_NXZ) |
| engine/CGameTimeManager.h | FABLE_RETAIL_CGAMETIMEMANAGER_ISTIMEBETWEEN | `bool IsTimeBetween(long, long) const` decl 0x006BB390 (both PDBs ?IsTimeBetween@CGameTimeManager@@QBE_NJJ@Z) in the narrow retail branch |
| engine/CDefString.h | FABLE_RETAIL_CDEFSTRING_NOTEQUAL | `bool operator!=(const char*) const` decl 0x005FA740 (Ego_r ??9CDefString@@QBE_NPBD@Z, already bound) when FABLE_RETAIL_EXPRESSION_FRAME does not declare it |
| engine/CTCCombat.h | FABLE_RETAIL_CTCCOMBAT_RESETCOMBO | ResetComboChainPos() decl 0x00733210 (both PDBs, already bound) |
| engine/CIntelligentPointer.h | FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE_OUTOFLINE | `template<class U> CIntelligentPointer(CIntelligentPointer<U>&)` decl (FableWin ??$?0VCThingCreatureBase@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@AAV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z, out of line at 0x0091F990) |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D | selects the existing two-statement GetMagnitude2D body (and GFFastSqrt) without the rest of FABLE_RETAIL_UPDATE_NEAREST_INTERESTING (which switches CTCTargetingBase to another branch) |

Landing note: re-copy HEAD headers and re-apply the blocks (other chunks may change the same headers). Rows include the overlay headers
first (an indirect include of a repo header from rebuild/include would bypass the overlay copy).
