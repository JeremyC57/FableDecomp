# Chunk 2017 header overlay (`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2017/hdr`)

Every change is an opt-in (`#ifdef FABLE_RETAIL_...`) or a new header only this chunk's rows include; each copied repo header's default
path is unchanged. Two sources of evidence each (PDB name set / TPI layout + retail bytes / donor order / RTTI).

## Copied unchanged from the chunk 2009 hand-back (`pending/2026-11-11_acctB_c2009/hdr/engine/`, see its README)
CAIStateGroup_JackDragon.h (then extended below), CJackDragonDef.h, CCreatureAction_SnapDragon*.h, CCreatureAction_DragonRearUpFull.h,
CCreatureAction_DragonRoar.h (new there), CPlayerGui.h, CTCBoss.h (GENERICCOUNTER, then extended below), CTCCombat.h, CTCStealth.h,
CThingFilter_*.h. Only CAIStateGroup_JackDragon.h and CTCBoss.h are used by this chunk's rows.

## New headers
| header | content | evidence |
|---|---|---|
| engine/CCombatAction_WillForcePushInto.h | : CCombatActionBase, no members, inline ctor (owner, 150, GetSoundPair(owner, "SWORD_SWING")) | Ego_r ??0CCombatAction_WillForcePushInto@@QAE@AAVCThingCreatureBase@@@Z + RTTI vtable 0x012804FC; Summon expands it (0x009745FE..), the landed out-of-line COMDAT 0x008C38B0 has the same body |
| engine/CCreatureGenerationFamilyDef.h | : CParentDefClassBase (RTTI bases), Creatures vector<long> +0x3C, DifficultyLevel, GeneratorObject, GenerationTypes | Ego_r TPI member names/order; Summon reads Creatures start/finish at +0x3C/+0x40 (0x009741BB) |

## Opt-ins added to copies of repo headers (HEAD 2026-11-11, or the c2009 copy)
| header | opt-in | change |
|---|---|---|
| engine/CAIStateGroup_JackDragon.h | FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON_INTOGROUND (nested in c2009's FABLE_RETAIL_CAISTATEGROUP_JACKDRAGON) | private ChooseHoverOrStrafeState() -> EDragonState (0x0094FFF0), StartFlying(CThingCreatureBase&, vector<C3DVector>&, C3DVector&, CCountedPointer<CCreatureActionBase>) (0x00950C30), IntoGroundSwoop() (0x00951430): Ego_r.pdb manglings + donor order; includes stlport/fable_containers.h and engine/CCountedPointer.h |
| engine/CCreatureAction_DragonBurninateInto.h | FABLE_RETAIL_CCREATUREACTION_DRAGONBURNINATEINTO | retail class : CCombatActionBase, Ego_r members PreviousPoint +0x114, MovementPoints vector<C3DVector> +0x120, BurnStart +0x12C, BurnStop +0x130, FlameStartPos +0x134, FlameEndPos +0x140 (sizeof 0x14C, the landed ctor 0x009537B0 writes them there); ctor decl with the Ego_r/FableWin signature (non-const refs) |
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT | protected AIAction_PerformAction(const CCreatureActionBase&) -> EAIActionReturn (Ego_r ?AIAction_PerformAction@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@@Z, donor 0x00848680 before PerformActionAndWait as retail 0x0097D8B0 / 0x0097DB70) |
| engine/CAIStateGroup_JackOfBladesBase.h | FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE_SUMMON (nested in the existing FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE) | protected Summon(long, bool) (Ego_r ?Summon@..@@IAEXJ_N@Z, donor 0x009992F0) |
| engine/CTCBoss.h | FABLE_RETAIL_CTCBOSS_SETHELPERCREATURE | SetHelperCreature(CThingAICreature&) decl (Ego_r 0x007B0CE0; retail 0x007AF310 already bound by that spelling) |
| engine/CTCTargetingAI.h | FABLE_RETAIL_CTCTARGETINGAI_SETSCRIPTTARGETOVERRIDE (default branch) | inline SetScriptTargetOverride(CThingCreatureBase*) { PScriptTargetOverride = target; } (Ego_r/FableWin name; Summon expands it: temporary CIntelligentPointer<CThingCreatureBase>, SetPItem on +0x40, 0x00974564..0x009745A3); needs FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN |
| engine/CCreatureAction_OnGenerate.h | FABLE_RETAIL_CCREATUREACTION_ONGENERATE | retail class : CCombatActionBase with the inline ctor (owner, 250, GetSoundPair(owner, (const char*)0)) (Ego_r ctor name + vtable 0x01268DEC; landed COMDAT 0x0074DBB0 has the same body) |
| stlport/position_algorithms.h | FABLE_RETAIL_STLP_REMOVE_IF_INLINE | STLport 4.6 _algo.h remove_if / find_if as inline bodies, __find_if(random_access) declared only (retail 0x008AAC50); default keeps the out-of-line remove_if declaration |

Landing note: re-copy HEAD headers and re-apply the blocks (other chunks may change the same headers). The CTCBoss.h copy carries
c2009's GENERICCOUNTER block too; CAIStateGroup_JackDragon.h carries c2009's retail class. Rows must include
engine/CAIStateGroupBase.h before any other header (an indirect include from rebuild/include would bypass the overlay copy).
