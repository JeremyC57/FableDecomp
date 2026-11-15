// 00779ef0 CTCDExperienceOrb::OnInitialActivate
// Initialise orb immunity/lifetime and play its ambient sound.
// Retail-asm: 0x00779F2C..0x00779F49 uses the original GFFloatToLongCeil helper (`fadd ; fistp dword ptr [ebp-4] ; fild`); per-site C++/intrinsic evidence in ROUNDING_ATTEMPTS.md.
// Leaf: ?OnInitialActivate@CTCDExperienceOrb@@UAEXXZ
// Retail: 0x00779EF0,567 bytes,thiscall.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CEXPERIENCEDEF_PASS22
#define FABLE_RETAIL_CTCDEXPERIENCEORB_PASS22_INITIALACTIVATE
#define FABLE_RETAIL_CENTITYSOUNDDEF_PASS22_SOUNDMAP
#define FABLE_RETAIL_CSOUNDMAP
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSAMPLE
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR_TYPE_LAST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_STLP_MAP_REVERSE
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCDExperienceOrb.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CEntitySoundDef.h"
#include "engine/CSoundInit.h"
#include "engine/GFFloatToLongCeil.h"
// This existing sound-definition COMDAT comes from the size-mode sound TU (native0x0065F604,162 bytes).
// The orb-specific instance0x0077AC20 uses the speed-mode template body already visible above.
#ifdef _MSC_VER
#pragma optimize("s", on)
#pragma optimize("y", off)
#endif
template<> bool CDefClassBase::GetSubDef<CEntitySoundDef>(CDefPointer<const CEntitySoundDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CEntitySoundDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#ifdef _MSC_VER
#pragma optimize("t", on)
#pragma optimize("y", on)
#endif
void CTCDExperienceOrb::OnInitialActivate()
{
    CTCBase::OnInitialActivate();
    GetParentThing().PeekBaseDef().GetSubDef(&PDef);
    CombinationImmunityTimer = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * 0.6f);
    CThingPlayerCreature* creature = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    // Original FableWin01F57870 calls rend(), as does retail; preserve that boundary read.
    LifeTimer = (long)(CMainGameComponent::GetConstantFPS() * GetDef().LifeTimeForDistanceThresholds.rend()->second);
    if (creature && !creature->IsDead()) {
        long distance = (long)(creature->GetPos() - GetParentThing().GetPos()).GetAccurateMagnitude();
        for (std::map<long,float>::const_iterator it = GetDef().LifeTimeForDistanceThresholds.begin(); it != GetDef().LifeTimeForDistanceThresholds.end(); ++it) {
            long threshold = it->first;
            float lifetime = it->second;
            if (distance <= threshold) {
                LifeTimer = (long)(CMainGameComponent::GetConstantFPS() * lifetime);
                break;
            }
        }
    }
    CDefPointer<const CEntitySoundDef> soundDef;
    CTCSoundPlayer* player;
    if (GetParentThing().GetTC(&player)) {
        if (GetParentThing().PeekBaseDef().GetSubDef(&soundDef)) {
            const CSoundPair* sound = soundDef->SoundMap.GetPSoundPair("AMBIENT_SOUND");
            if (sound) {
                CSoundInit init;
                init.GainFaderBits = 0x8010;
                init.Volume = 140.0f;
                player->PlayAttachedSample(*sound, init, false, false, false);
            }
        }
    }
}