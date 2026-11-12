// CTCSummonSpell::ResetMinionPos() -- re-place the summoned minion in front of the hero: play the summon-appear sound, take the
//   hero's facing vector plus position as the target, and when the minion is alive and has a physics component move it there, then
//   to the nearest valid teleport position, turn it to face away from the hero (up Z, forward = -direction), and spawn the spell's
//   CREATURE_APPEARING effect at the final position
// Leaf: ?ResetMinionPos@CTCSummonSpell@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079B6F0, 732 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb ?ResetMinionPos@CTCSummonSpell@@QAEXXZ (Ego_r 0x007A13F0; FableWin.pdb debug copy 0x026A3B50); the catalog name.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the expanded GetTC<CTCSoundPlayer>), CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, the hero's CTCPhysicsBase slot 70
//   (+0x118 GetFacingVector), CThing::GetPos 0x004C73D0, CBaseIntelligentPointer::GetPItem 0x00A01B50 (Minion.Get), the minion's
//   CTCPhysicsBase SetPosition (slot 30, +0x78) and SetFacingAnglesFromRHSet (slot 74, +0x128), CTCSummonSpell::GetValidTeleportPos
//   0x0079AA90, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30,
//   CTCSpecialAbilities::CreateParticleEffect 0x0072AB70 (__fastcall); the def's slot 1 on the last release.
// Shape: the same TU as the landed CreateMinion 0x0079ACC0 (its define set; the same-TU noinline GetSubDef / GetEffectFromString
//   bodies, the COMDAT copies kept at 0x00669580 / 0x00669D30); the direction's Normalise in the default X, Z, Y squared-magnitude
//   spelling (retail loads Y, Z, X); the effect lookup and the def's release sit inside the minion-alive block (a dead or missing
//   minion returns straight away: 0x0079B7B7); the Newton step in the NEWTON_SQUARE_PAREN spelling (two no-code PAREN tuples:
//   NEWTON_PAREN cuts the scheduling region one tuple short, NEWTON_GROUPED one too far).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_BULLETTIMEOVERRIDE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_COMBOMULTIPLIER
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_SETNEWBRAIN
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT
#define FABLE_RETAIL_CTCCARRYING_ALLCARRIED
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_SQUARE_PAREN
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSUMMONSPELL
#define FABLE_RETAIL_CTCSUMMONSPELL_MINION
#define FABLE_RETAIL_CTCSUMMONSPELL_TELEPORT
#define FABLE_RETAIL_CTCSUMMONSPELL_RESETMINIONPOS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCarrying.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CRGBColour.h"
#include "fable_game.h"
#include "engine/CTCSpecialAbilities.h"

template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSubDefInfo* index;
    { CCharString name("CSpecialEffectsDef",-1); index=GetPSubDefInfo(name); }
    if(index) {
        CDefPointer<const CSpecialEffectsDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name,long& result) const
{
    unsigned long key=name.GetCRC();
    std::map<unsigned long,long>::const_iterator item=SpecialEffectsMap.ContainmentMap.find(key);
    if(item!=SpecialEffectsMap.ContainmentMap.end()) { result=item->second;return true; }
    return false;
}


class CSpecialAbilitiesSummonSpellDef : public CDefClassBase {
    unsigned char _pad_0x25[0x3c - 0x25];
public:
    std::vector<float> TimeOfSummon;                    // +0x3c
    unsigned char BaseSummonCreatureName[4];            // +0x48 CDefString
    float FadeInTime;                                   // +0x4c
    float FadeOutTime;                                  // +0x50
    long ResetCreatureManaCost;                         // +0x54
    std::vector<long> SummonTintColour;                 // +0x58
    CCharString EyeLeftDummyPointName;                  // +0x64
    CCharString EyeRightDummyPointName;                 // +0x68
};
#include "engine/CTCSummonSpell.h"

void CTCSummonSpell::ResetMinionPos()
{
    CTCSoundPlayer* sound_player;
    if (GetParentThing().GetTC(&sound_player))
        sound_player->PlayAttachedCriteria("HERO_ABILITY_SUMMON_CREATURE_APPEAR", true, false, false, -1);
    C3DVector offset = GetParentThing().GetPhysicsTC()->GetFacingVector();
    C3DVector target = offset + GetParentThing().GetPos();
    C3DVector pos = target;
    if (Minion.Get() && !Minion->IsDead()) {
        CTCPhysicsBase* physics = Minion->GetPhysicsTC();
        if (physics) {
            physics->SetPosition(target);
            pos = GetValidTeleportPos(Minion.Get());
            physics->SetPosition(pos);
            offset = pos - GetParentThing().GetPos();
            offset.Normalise();
            physics->SetFacingAnglesFromRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), -offset));
        }
        CDefPointer<const CSpecialEffectsDef> effects;
        long effect = 0;
        if (PDef->GetSubDef(&effects) && effects->GetEffectFromString("CREATURE_APPEARING", effect))
            CTCSpecialAbilities::CreateParticleEffect(effect, pos);
    }
}