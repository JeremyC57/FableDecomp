// CTCLightningNode::FrameUpdate() -- per-frame update of a lightning node: expire it (remove its own TC) when the def lifetime has run out or the parent died or lost its node, else spread new nodes and branches to things in range and keep each branch's particle beam fed and its target electrocuted
// Leaf: ?FrameUpdate@CTCLightningNode@@UAEXXZ
// Inline-budget: header opt-in FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE (CVectorMap<unsigned long,C3DVector,...>::Insert): retail expands both inserts (push_back end test + Dirty store, 0x007F3312..0x007F33BE); the c2 inline trace (tools/inl.sh) scores the plain inline Insert 191 (c1 72, f 0.026) -> called. Ego_r.pdb holds no out-of-line Insert (FableWin.pdb, unoptimised, does), as for the existing FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE forced Insert.
// Retail: Fable.exe 0x007F2DC0, 1942 bytes (`push ebp` .. `ret` at 0x007F3555; the bytes up to the reservation estimate are this TU's unreferenced inline COMDATs).
//   Ego_r.pdb / FableWin.pdb ?FrameUpdate@CTCLightningNode@@UAEXXZ; rtti-vtables CTCLightningNode 0x0124337C slot 9 = 0x007F2DC0.
// Callees: CBaseIntelligentPointer GetPItem 0x00A01B50 / ~ 0x00A01C10; CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760;
//   IS_THING_ALIVE<CIntelligentPointer<const CThing>> 0x0069CE20 / <CIntelligentPointer<CThing>> 0x004CC340; CThing::GetPos 0x004C73D0,
//   AddTC 0x004C9D60, RemoveTC 0x004C9840; __ftol2 0x00BFEA70; C2DBoxI(const C2DCoordI&, long) 0x00434740; CTCBase::PeekThingSearchTools
//   0x00686D90, GetDefinitionManager 0x006869D0; CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, vector<CThingGameObject*>,
//   CFilter_Bind3<In3DRange, unary_negate<HasTC>, EnemyCreature>> 0x007F4020, <..., CFilter_Bind2<In3DRange, unary_negate<HasTC>>> 0x007F42A0,
//   <CThingObject, ..., CFilter_Bind3<..., (anon)::CThingFilter_IsElectrocutable>> 0x007F4500; CDefinitionManager::GetDef<CLightningDef>
//   0x007F3BB0; CCharString 0x0099EBF0 / ~ 0x0099EAE0 ("CTCLightningNode" 0x0123E650, "LIGHTNING_ORB_BRANCH" 0x01271D90,
//   "CTCElectrocutedEffect" 0x0123D394); CTCLightningNode::SetPParent 0x007F39A0 / SetPSourceOfFaction 0x007F39F0 / SetPSourceOfHit
//   0x007F3A40 / SetPDef 0x007F3A90 (inline, called here); operator new 0x00BFEA1A; CLightningBranch(CThing&) 0x007F3AD0;
//   CCountedPointer<CLightningBranch>(T*) 0x007F3F60 / ~ (via 0x004DA6F7); vector<CCountedPointer<CLightningBranch>>::push_back 0x007F48D0 /
//   erase 0x007F3E90; free 0x00BFEA14; CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580; CSpecialEffectsDef::GetEffectFromString
//   0x00669D30; NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) 0x004C1060; CTCDParticleEmitter::Create 0x006E0880,
//   SetEmitterUpdateParameters 0x006E11A0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the inline GetTCs); CCRC::Calc
//   0x004014A0 ("LIGHTNING_START" 0x0126C810 / 15, "LIGHTNING_END" 0x0126C800 / 13); NParticleEngine::CParticleUpdateParameters<unsigned long,
//   unsigned long> ctor 0x006E24D0 / Update 0x0076C750 / ~ 0x006E2500; vector<pair<unsigned long,C3DVector>>::_M_insert_overflow 0x0076ADA0;
//   CHitParameters() 0x007A3BC0 / SetThingHitting 0x008685F0 / AddDamageWithAttribute 0x00868CA0 / ~ 0x00789358; CThingGameObject::OnHit
//   (vtable +0x104); CTCElectrocutedEffect::Reset 0x007CE7A0.
// Shape (c2 inline trace + call pairing: all 75 calls in retail order): the parent test is spelled out (`PParent.Get() == 0 ||
//   PParent->IsDead() || !HasTC`): the four-term || halves the static frequency of everything after it, which is what makes VC7.1 CALL
//   IS_THING_ALIVE(branch effect) (score 117), CIntelligentPointer(T*) (150) and operator= (112) inside the branch loop as retail does
//   (with IS_THING_ALIVE(PParent) they score 72 / 85 / 64 and two of them expand); the area centre is a direct C2DCoordI((long)X,(long)Y)
//   temporary (X, Y, then the radius, as retail); the things loop is `if (!things.empty())` (FableWin lists vector::empty in this TU;
//   retail's unreferenced COMDAT 0x007F35D0) with the iterator taken before GetDef and a do-while (retail's aligned loop head, no
//   re-test); the AddTC result goes through GFDebugPDynamicCast (FableWin lists ??$GFDebugPDynamicCast@PAVCTCBase@@PAVCTCLightningNode@@
//   in this TU; code-identical to static_cast); the branch loop is a `for` with `continue` after each erase (two separate erase calls as
//   retail; an `||` target test cross-jumps them); the branch effect is a named reference (retail `mov esi,[edi]; add esi,8` once); the
//   range test squares a named difference vector (X, Z, Y as retail); the two LIGHTNING params are named pairs built before the
//   parameter block (the landed CCreatureAction_SpiritAttack::FrameUpdate 0x00854B90 shape).
// Lifetime test: retail reads Timer after the PDef null test and keeps a frame copy (0x007F2DE6 `mov eax,[ebx+28h]`, 0x007F2DEF
//   `mov [esp+18h],eax`, 0x007F2DFA `fild [esp+18h]`). VC7.1 forwards a single-use copy of a member into its use unless a control-flow
//   join lies between them (measured in fragments; landed twin of the effect: CTCHeroExperience::IsToAutoBlock 0x006D6CB0, where a
//   clamp's join keeps `level` in [esp+10h]). `long timer = Timer;` followed by the materialised `const bool has_life_time` (branch +
//   join, threaded away later) gives retail's copy, its scheduling between `fld` and `fcomp`, and the whole frame; the `const` keeps the
//   range test's X, Z, Y squaring order (a plain `bool` gives Z, Y, X).
// Area radius: `float range = PDef->Range;` (forwarded to its use) lets `fld [ecx+3Ch]` issue before the C2DCoordI temporary's Y store
//   as retail does (0x007F2E97 / 0x007F2E9A); read through the `PDef->` expression the load keeps a may-alias edge to that store (the
//   temporary's address goes to the out-of-line C2DBoxI ctor) and issues after it (c2 scheduler DAG, tools/sched.sh).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_UPDATEPARAMETERS
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_CTHINGFILTER_ENEMYCREATURE_CTOR
#define FABLE_RETAIL_C2DBOXI_CENTRE_RADIUS_OUTOFLINE
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_ADDDAMAGE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCLIGHTNINGNODE_FRAMEUPDATE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_In3DRange.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CHitParameters.h"
#include "engine/CTCElectrocutedEffect.h"
#include "engine/CLightningDef.h"
#include "engine/CTCLightningNode.h"
class CThingCreatureBase;
class CThingObject;

namespace {
// FableWin.pdb ??RCThingFilter_IsElectrocutable@?A0xc5a89a9e@@QBE_NABVCThingObject@@@Z: this TU's anonymous-namespace filter (out of
// line at 0x007F3D60, landed).
class CThingFilter_IsElectrocutable {
public:
    bool operator()(const CThingObject& thing) const;                   // 0x007F3D60
};
}

void CTCLightningNode::FrameUpdate()
{
    if (PDef.IsNull())
        return;
    long timer = Timer;
    const bool has_life_time = PDef->LifeTime > 0.0f;
    if ((has_life_time && timer > PDef->LifeTime * CMainGameComponent::GetConstantFPS())
        || PParent.Get() == 0 || PParent->IsDead() || !PParent->HasTC(TCI_LIGHTNING_NODE)) {
        ParentThing->RemoveTC(TCI_LIGHTNING_NODE);
    } else {
        if (PDef->NodeDefIndex > 0 && PDef->MaxNumberOfBranches > 0) {
            C3DVector pos = ParentThing->GetPos();
            float range = PDef->Range;
            C2DBoxI area(C2DCoordI((long)pos.X, (long)pos.Y), (long)range);
            std::vector<CThingGameObject*> things;
            if (IS_THING_ALIVE(PSourceOfFaction))
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(area,
                    GFFilter_Bind3(CThingFilter_In3DRange(pos, PDef->Range), std::not1(CThingFilter_HasTC(TCI_LIGHTNING_NODE)),
                                   CThingFilter_EnemyCreature(*PSourceOfFaction.Get())), things);
            else
                PeekThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(area,
                    GFFilter_Bind2(CThingFilter_In3DRange(pos, PDef->Range), std::not1(CThingFilter_HasTC(TCI_LIGHTNING_NODE))),
                    things);
            PeekThingSearchTools().GetAllThingsInRoughArea<CThingObject>(area,
                GFFilter_Bind3(CThingFilter_In3DRange(pos, PDef->Range), std::not1(CThingFilter_HasTC(TCI_LIGHTNING_NODE)),
                               CThingFilter_IsElectrocutable()), things);
            if (!things.empty()) {
                std::vector<CThingGameObject*>::iterator it = things.begin();
                CDefPointer<const CLightningDef> node_def;
                GetDefinitionManager().GetDef(PDef->NodeDefIndex, &node_def);
                do {
                    CThingGameObject* thing = *it;
                    CTCLightningNode* node = GFDebugPDynamicCast(thing->AddTC("CTCLightningNode", false, 0), (CTCLightningNode*)0);
                    node->SetPParent(*ParentThing);
                    node->SetPSourceOfFaction(PSourceOfFaction.Get());
                    node->SetPSourceOfHit(PSourceOfHit.Get());
                    node->SetPDef(node_def);
                    LightningBranches.push_back(CCountedPointer<CLightningBranch>(new CLightningBranch(*thing)));
                    ++it;
                } while (it != things.end());
            }
        }
        CDefPointer<const CSpecialEffectsDef> special_effects;
        long effect = -1;
        if (PDef->GetSubDef(&special_effects) && special_effects->GetEffectFromString("LIGHTNING_ORB_BRANCH", effect)) {
            C3DVector start = ParentThing->GetTCGraphicAppearance()->GetFocalPos(1.0f);
            for (std::vector<CCountedPointer<CLightningBranch> >::iterator it = LightningBranches.begin();
                 it != LightningBranches.end(); ) {
                CThing* target = (*it)->PTarget.Get();
                if (!IS_THING_ALIVE(target)) {
                    it = LightningBranches.erase(it);
                    continue;
                }
                C3DVector end = target->GetTCGraphicAppearance()->GetFocalPos(1.0f);
                C3DVector difference = start - end;
                if (difference.GetSquaredMagnitude() > PDef->Range * PDef->Range) {
                    it = LightningBranches.erase(it);
                    continue;
                }
                CIntelligentPointer<CThing>& branch_effect = (*it)->PEffect;
                if (!IS_THING_ALIVE(branch_effect))
                    branch_effect = CTCDParticleEmitter::Create(effect, ParentThing->GetPos(), false);
                if (IS_THING_ALIVE(branch_effect)) {
                    CTCDParticleEmitter* emitter;
                    branch_effect->GetTC(&emitter);
                    std::pair<unsigned long, C3DVector> start_param(CCRC::Calc(0, "LIGHTNING_START", 15), start);
                    std::pair<unsigned long, C3DVector> end_param(CCRC::Calc(0, "LIGHTNING_END", 13), end);
                    NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> parameters;
                    parameters.VectorParams.Insert(start_param);
                    parameters.VectorParams.Insert(end_param);
                    parameters.Update();
                    emitter->SetEmitterUpdateParameters(parameters);
                    if (PDef->DamagePerCycle > DELTA_NOTIONAL_ZERO && (target->GetTypeFlags() & 4)) {
                        if ((*it)->Timer++ % (long)(CMainGameComponent::GetConstantFPS() * PDef->DamagePeriod) == 0) {
                            CHitParameters hit;
                            hit.SetThingHitting(*(IS_THING_ALIVE(PSourceOfHit) ? PSourceOfHit : PParent).Get());
                            hit.AddDamageWithAttribute(DAMAGE_LIGHTNING, PDef->DamagePerCycle);
                            hit.SetPlayHitResponse(false);
                            static_cast<CThingGameObject*>(target)->OnHit(hit);
                            CTCElectrocutedEffect* electrocuted;
                            if (!target->GetTC(&electrocuted))
                                electrocuted = static_cast<CTCElectrocutedEffect*>(target->AddTC("CTCElectrocutedEffect", true, 0));
                            electrocuted->Reset();
                        }
                    }
                }
                ++it;
            }
        }
    }
    ++Timer;
}