// NCreatureMode::CModeJackOfBladesFlash::FrameUpdate() -- the Jack of Blades flash: count the timer down (flashing below a threshold); at the glow start frame kill the old emitter, fade the environment to the glow theme and spawn the sword glow and the spot light at the creature's weapon dummy; at zero fade back, kill both, put the flames out, play the strike-out animation and drop the mode; otherwise aim the spot light at the camera; finally drop the mode when the boss is ready for its phase change
// Grade: DIFFER(1647v1665) -- VC7.1 calls CIntelligentPointer<CThing>::operator= (??4 COMDAT) out of line where retail expands it (the documented inline-budget blocker), plus frame-slot assignment (retail keeps the GetTC key / type temp at frame 8); 108 masked diff lines
// Next idea: find a landed row where retail expands CIntelligentPointer<T>::operator= inside a large function and copy its spelling; then retry the frame slots with the GetTC key / type temp declared in the glow-start block
// Leaf: ?FrameUpdate@CModeJackOfBladesFlash@NCreatureMode@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DEC80, 1665 bytes, thiscall, plain `ret` (three exits), no EH frame, no /GS.
//   Literals "ENVIRONMENT_JACK_OF_BLADES_GLOW" 0x01287308, "FLASH_SWORD_GLOW" 0x012872F4, "WEAPON_FOCUS_01" 0x0126515C,
//   "MARKER_SPOT_LIGHT_JACK_OF_BLADES" 0x012872D0, "" 0x0122D70E, "ENVIRONMENT_JACK_OF_BLADES_FADE" 0x012857F0,
//   "ST_STRIKE_LIGHT_OUTOF" 0x012872B8; floats 1/30 0x012856DC, 3.0 0x0122DED4, 0.5 0x0122F59C, 0.0 0x0122DEDC, 1.0 0x0122DED8,
//   -1.0 0x0122DEE0, 1/(2 pi) 0x01237F08, 0.75 (immediate).
// Identity (two sources): slot 3 of the RTTI vtable 0x012857A8 (NCreatureMode::CModeJackOfBladesFlash : CModeStandBase : CModeBase;
//   slot 0 the landed scalar dtor 0x008E3700, slot 6 the landed OnDeactivate 0x008DF310); slot 3 is FrameUpdate in every creature
//   mode vtable (CModeBlock 0x01267C3C slot 3 = landed FrameUpdate 0x008D8EA0, CModeDragonStrafe slot 3 = landed 0x008E07F0, the
//   CModeBase declaration order in engine/CTCCreatureModeManager.h); Ego_r.pdb / FableWin.pdb ?FrameUpdate@CModeJackOfBladesFlash@NCreatureMode@@UAEXXZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CMODESTANDBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/NCreatureMode.h"
#include <math.h>
class CBaseObject;
class CBaseObjectPointer;
#include "engine/CIntelligentPointer.h"
template <> CIntelligentPointer<CThing>::CIntelligentPointer(CThing* pointer);   // 0x004A96F0 (out of line in retail)

class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();            // 0x0044C6B0
};
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
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet() { }
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void ResetRHSet(const CRightHandedSet& set);        // slot 32 (engine/CTCPhysicsBase.h order: SetPosition 30, ResetPosition 31, ResetRHSet 32)
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& orientation, float scale) const;   // 0x004C08C0
};
}
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long type, const C3DVector& pos, bool flag);   // 0x006E0880
};
CThing* __fastcall CreateMarker(long type, const C3DVector& pos, const CCharString& name);   // 0x0083D460
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();             // 0x0062E900
    unsigned char _body[0x114];                 // retail sizeof CCreatureAction_PlayCombatAnimation 0x118 (generated header; Ego_r 0x130)
};
class CThingCreatureBase;
class CCreatureAction_PlayCombatAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase& creature, const CCharString& anim, bool flag);   // 0x008B7FF0
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CTCBoss { public:
    bool IsReadyForPhaseChange() const;         // 0x007AEE90
};
class CTCSpotLight { public:
    void SetAngle(float angle);                 // 0x00731BE0
};
class CEnvironment { public:
    void TransitionToThemeAllChannels(long channel, long theme, float time);   // 0x006B38D0
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual const C3DVector PeekCurrentPos() const;   // slot 11 0x0069C1B0 (engine/CAIGameCameraBase.h)
};
class CWorld { public:
    CEnvironment& GetEnvironment() { return *PEnvironment_ptr; }                       // FableWin ?GetEnvironment@CWorld@@QAEAAVCEnvironment@@XZ
    const CAIGameCameraBase* PeekGameCamera() const { return PCurrentGameCamera; }
    unsigned char      _pad00[0x18];
    CEnvironment*      PEnvironment_ptr;        // +0x18
    unsigned char      _pad1c[0x34 - 0x1c];
    CAIGameCameraBase* PCurrentGameCamera;      // +0x34
};
extern long g_JackFlashFlashFrames;             // 0x01388A7C
extern long g_JackFlashGlowFadeFrames;          // 0x01388A84
extern long JACK_GLOW_START_FRAME;              // 0x013BAFDC

namespace NCreatureMode {
class CModeJackOfBladesFlash : public CModeStandBase {
public:
    virtual void FrameUpdate();
private:
    void ExtinguishFlames();                    // 0x008DCB60
    void Flash();                               // 0x008DCC80
public:
    long                                    Timer;          // +0x68
    long                                    BurnCounter;    // +0x6C
    unsigned char                           Filter[0xc];    // +0x70
    CThingCreatureBase*                     PCreature;      // +0x7C
    CIntelligentPointer<CThingCreatureBase> PTarget;        // +0x80
    CIntelligentPointer<CThing>             PEmitter;       // +0x88
    CIntelligentPointer<CThing>             PSpotLight;     // +0x90
};

void CModeJackOfBladesFlash::FrameUpdate()
{
    CTCCreatureModeManager::CModeBase::FrameUpdate();
    long glow = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_GLOW");
    if (--Timer < g_JackFlashFlashFrames)
        Flash();
    if (Timer == JACK_GLOW_START_FRAME) {
        if (PEmitter.Get() && !PEmitter.Get()->IsDead())
            PEmitter.Get()->Kill(true);
        CWorld& world = GetWorld();
        world.GetEnvironment().TransitionToThemeAllChannels(3, glow, (float)g_JackFlashGlowFadeFrames * (1.0f / 30.0f));
        CDefPointer<const CSpecialEffectsDef> effects;
        if (ParentThing.PeekBaseDef().GetSubDef(&effects)) {
            long type;
            if (effects->GetEffectFromString("FLASH_SWORD_GLOW", type)) {
                C3DVector pos = PCreature->GetPos();
                CRightHandedSet orientation;
                PCreature->GetTCGraphicAppearance()->GetDummyObject("WEAPON_FOCUS_01", 0, 0, pos, orientation, 1.0f);
                CThing* emitter = CTCDParticleEmitter::Create(type, pos, false);
                if (emitter && !emitter->IsDead())
                    PEmitter = CIntelligentPointer<CThing>(emitter);
                long marker_type = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_SPOT_LIGHT_JACK_OF_BLADES");
                CThing* marker = CreateMarker(marker_type, pos, "");
                if (marker && !marker->IsDead())
                    PSpotLight = CIntelligentPointer<CThing>(marker);
            }
        }
    } else if (Timer == 0) {
        long fade = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_FADE");
        CWorld& world = GetWorld();
        world.GetEnvironment().TransitionToThemeAllChannels(3, fade, 0.75f);
        if (PEmitter.Get() && !PEmitter.Get()->IsDead())
            PEmitter.Get()->Kill(true);
        if (PSpotLight.Get() && !PSpotLight.Get()->IsDead())
            PSpotLight.Get()->Kill(true);
        ExtinguishFlames();
        PCreature->SetCurrentAction(CCreatureAction_PlayCombatAnimation(*PCreature, "ST_STRIKE_LIGHT_OUTOF", true));
        CTCCreatureModeManager* modes;
        PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        modes->RemoveMode(MODE_JACK_OF_BLADES_FLASH);
        return;
    }
    if (PSpotLight.Get() && !PSpotLight.Get()->IsDead()) {
        C3DVector camera_pos = GetWorld().PeekGameCamera()->PeekCurrentPos();
        C3DVector direction = camera_pos - ParentThing.GetPos();
        direction.Normalise();
        C3DVector flat = direction;
        flat.Z = 0.0f;
        flat.Normalise();
        PSpotLight.Get()->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), flat));
        CTCSpotLight* spot = 0;
        PSpotLight.Get()->GetTC(TCI_SPOT_LIGHT, &spot);
        float cos_angle = direction.Dot(C3DVector(0.0f, 0.0f, -1.0f));
        if (cos_angle < -1.0f)
            cos_angle = -1.0f;
        else if (cos_angle > 1.0f)
            cos_angle = 1.0f;
        spot->SetAngle(acos(cos_angle) * 0.15915494f);
    }
    CTCBoss* boss = 0;
    PCreature->GetTC(TCI_BOSS, &boss);
    if (boss->IsReadyForPhaseChange()) {
        CTCCreatureModeManager* modes = 0;
        PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        modes->RemoveMode(MODE_JACK_OF_BLADES_FLASH);
    }
}
}
