// CreateRelevantPersonAndUpdateStats(float, float, long, long, long&, long&, const C3DVector&, const CThingAICreatureInit&, CTCVillage*, vector<CThingAICreature*>&, vector<CThingAICreature*>&, bool) -- create a villager of the under-represented sex (falling back to the other def), count it, optionally list it, mark it as allowed to follow the hero (also through teleporters), add it to the village and, for a non-listed villager, roll its sexuality from the village people def
// Leaf: ?CreateRelevantPersonAndUpdateStats@@YIPAVCThingAICreature@@MMJJAAJ0ABVC3DVector@@ABVCThingAICreatureInit@@PAVCTCVillage@@AAV?$vector@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@4_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FF750, 336 bytes, __fastcall (the two def indices in ecx / edx; the floats and the
//   rest on the stack), `ret 28h`.
// Identity: Ego_r.pdb / FableWin.pdb ?CreateRelevantPersonAndUpdateStats@@YIPAVCThingAICreature@@MMJJAAJ0ABVC3DVector@@...4_N@Z
//   (Ego_r 0x004D1FB0, a free __fastcall of the CWorldMap TU); its two callers are CWorldMap::PopulateRegionWithVillagers 0x00502E90.
// Callees: CThingAICreature::Create 0x00833800, vector<CThingAICreature*>::push_back(const T&) 0x00517A10 (out of line: three sites),
//   CTCVillage::AddThingToVillage(CThing&) 0x0065B854, CDefClassBase::GetSubDef<CVillagePeopleDef> 0x00512FE0 (const),
//   CThingAICreature::AutoSetupSexuality(float) 0x008330D0, the def's Delete (slot 1).
// Layout: CThingAICreature +0x1C9 bit 0 AllowedToFollowHero (FableWin inline SetAsAllowedToFollowHero), CThingCreatureBase +0x15C bit 6
//   FollowHeroThroughTeleporters; CTCVillage : CTCBase (ParentThing +4); CThing::PBaseDef +0x70; CVillagePeopleDef : CDefClassBase,
//   ProbabilityOfHomsexualityPerDialogueEnabledMan +0x34 (Ego_r = retail: no container before it).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CTCVILLAGE
#include "engine/CTCVillage.h"                                       // retail branch: CTCVillage : CTCBase + AddThingToVillage (hdr/)

#include "engine/CThingBaseDef.h"                                   // CThingBaseDef + the FABLE_RETAIL_CTHING inline CThing::PeekBaseDef
class CVillagePeopleDef : public CDefClassBase {                     // Ego_r.pdb members (retail = Ego_r up to here)
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CVillagePeopleDef", -1); }   // ?StatGetTypeName@CVillagePeopleDef@@SI?AVCCharString@@XZ (inline)
    bool  Enable;                                                    // +0x25
    float DesiredMaleWeight;                                         // +0x28
    float DesiredFemaleWeight;                                       // +0x2c
    float DesiredChildWeight;                                        // +0x30
    float ProbabilityOfHomsexualityPerDialogueEnabledMan;            // +0x34
};
// The member template CDefClassBase::GetSubDef<T> (header text; retail instantiates it in this TU: 0x00512FE0, the
// CVillagePeopleDef COMDAT between the CWorldMap bodies). Its body in the TU lets VC7.1 see the out pointer is not kept.
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

CThingAICreature* __fastcall CreateRelevantPersonAndUpdateStats(float maleWeight, float femaleWeight, long maleDefIndex,
    long femaleDefIndex, long& noMales, long& noFemales, const C3DVector& pos, const CThingAICreatureInit& init,
    CTCVillage* village, std::vector<CThingAICreature*>& males, std::vector<CThingAICreature*>& females, bool listCreatures)
{
    CThingAICreature* creature = 0;
    if (noMales * femaleWeight < noFemales * maleWeight)
    {
        if (maleDefIndex)
        {
            creature = CThingAICreature::Create(maleDefIndex, pos, 4, init);
            if (listCreatures) males.push_back(creature);
            ++noMales;
        }
        else if (femaleDefIndex)
        {
            creature = CThingAICreature::Create(femaleDefIndex, pos, 4, init);
            if (listCreatures) females.push_back(creature);
            ++noFemales;
        }
    }
    else
    {
        if (femaleDefIndex)
        {
            creature = CThingAICreature::Create(femaleDefIndex, pos, 4, init);
            if (listCreatures) females.push_back(creature);
            ++noFemales;
        }
        else if (maleDefIndex)
        {
            creature = CThingAICreature::Create(maleDefIndex, pos, 4, init);
            if (listCreatures) males.push_back(creature);
            ++noMales;
        }
    }
    if (creature)
    {
        creature->SetAsAllowedToFollowHero(true);
        creature->FollowHeroThroughTeleporters = true;
        village->AddThingToVillage(*creature);
        if (!listCreatures)
        {
            CDefPointer<const CVillagePeopleDef> peopleDef;
            if (village->ParentThing->PeekBaseDef().GetSubDef(&peopleDef))
                creature->AutoSetupSexuality(peopleDef->ProbabilityOfHomsexualityPerDialogueEnabledMan);
        }
    }
    return creature;
}
