# Chunk 2009 header overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2009/hdr`)

Every change is an opt-in (`#ifdef FABLE_RETAIL_...`) or a new header only this chunk's rows include; each copied repo header's
default path is unchanged (copied from HEAD 2026-11-11, then the block below added). Evidence: two sources each (PDB name set /
TPI layout + retail bytes / donor order / RTTI).

## New headers
| header | content | evidence |
|---|---|---|
| engine/CThingFilter_HasPhysicsMesh.h | empty filter (Ego_r size 1), user ctor (FableWin ??0..@@QAE@XZ), operator()(const CThing&) const decl (0x006A6F50) | Ego_r TPI + both name sets; retail never writes its frame byte (0x007454E5) |
| engine/CThingFilter_RadiusIntersectsInfiniteLine.h | C3DVector Start / End (Ego_r 24 bytes), inline ctor (FableWin ??0..@@QAE@ABVC3DVector@@0@Z), operator() decl (0x006A7050) | Ego_r TPI + the six dword copies 0x00745493..0x007454CC |
| engine/CCreatureAction_DragonRoar.h | : CCombatActionBase (0x114), ctor (CThingCreatureBase&, CThing&) decl (0x00953300) | Ego_r / FableWin ctor name + Ego_r size 300 = Ego_r CCombatActionBase; UpdateGround destroys it with ~CCombatActionBase |

## Opt-ins added to copies of repo headers
| header | opt-in | change |
|---|---|---|
| engine/CPlayerGui.h | FABLE_RETAIL_CPLAYERGUI_PERCEIVERS | AddPerceivingThing(CThing&) 0x004378F0 / RemovePerceivingThing(CThing&) 0x00437900 decls (Ego_r.pdb 0x0044A590 / 0x0044A5A0, CThing& spelling) |
| engine/CTCStealth.h | FABLE_RETAIL_CTCSTEALTH_STATTYPE | inline StatGetInterfaceType() returning TCI_STEALTH (the PeekTC<const CTCStealth> lookups key 0x36 in place, 0x0074515B) |
| engine/CTCCombat.h | FABLE_RETAIL_CTCCOMBAT_ISINMELEE | IsInMelee() const decl (Ego_r 0x0074B330, retail 0x00733190 by donor order) |
| engine/CThingFilter_IsInFOV.h | FABLE_RETAIL_CTHINGFILTER_ISINFOV_CTOR | ctor (const C3DVector&, const C3DVector&, float, float, bool) decl (both PDBs, retail 0x006A7150 landed) |
| engine/CTCBoss.h | FABLE_RETAIL_CTCBOSS_GENERICCOUNTER | inline GetGenericCounter() const (FableWin ?GetGenericCounter@CTCBoss@@QBEJXZ; retail reads +0x10 in place, 0x00952CE4) |
| engine/CJackDragonDef.h | FABLE_RETAIL_CJACKDRAGONDEF | retail class : CSubDefClassBase with the 20 Ego_r members (+0x28..+0x74, sizeof 0x78), inline StatGetTypeName ('CJackDragonDef', the landed GetSubDef 0x008E3300 literal) |
| engine/CCreatureAction_SnapDragon.h, engine/CCreatureAction_SnapDragonFast.h | FABLE_RETAIL_CCREATUREACTION_SNAPDRAGON / _SNAPDRAGONFAST | : CCombatActionBase, ctor (CThingCreatureBase&, CThing&) decl (0x009531C0 / 0x00953260), implicit dtor |
| engine/CCreatureAction_DragonRearUpFull.h | FABLE_RETAIL_CCREATUREACTION_DRAGONREARUPFULL | : CCombatActionBase + CIntelligentPointer<CThing> PShot +0x114 (sizeof 0x11C), ctor decl (0x00953120), implicit dtor (retail 0x009531A0) |
| engine/CAIStateGroup_JackDragon.h | FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON | retail class : CAIStateGroupBase with the Ego_r members (PEnemy +0x14 .. DragonPosStrafeFlameLeft +0x132; needs FABLE_RETAIL_C3DVECTOR_PACKED), the private methods UpdateGround calls (IsStillRunnable 0x0094FDF0, StayIdle(float) 0x0094FE30, FindHitEvent 0x00950B20, WaitForAnimLoop(float) 0x00950950, SetHeadTurning(bool) 0x00950BB0, FlyOutOfGround 0x00951B20) and the protected Update |

Landing note: the blocks are additive; re-copy HEAD headers and re-apply them before merging (other chunks may have changed the same
headers). The landed JackDragon / CTCPerceiveThing rows use local models, so no landed includer changes.
