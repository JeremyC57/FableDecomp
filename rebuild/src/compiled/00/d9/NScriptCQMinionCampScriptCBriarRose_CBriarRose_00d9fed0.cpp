// NScript::CQ_MinionCampScript::CBriarRose::~CBriarRose() -- implicit entity-script destructor: deregister SpeechTimer, destroy Target and MyDestination (CScriptThing) and seh_me in reverse order, then ~CEntityScriptBase
// Leaf: ??1CBriarRose@CQ_MinionCampScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9FED0, 203 bytes, tail jump to ~CEntityScriptBase 0x00F35B40; sole caller the scalar
//   deleting dtor 0x00D9FEB0 (slot 0 of the RTTI-less entity vtable 0x012D6D0C).
// Identity (proven): vtable 0x012D6D0C is stored only by the ctor 0x00D9E930, whose only caller is the factory 0x00D9E8B0
//   (`new` 0x50 = sizeof Ego_r CQ_MinionCampScript::CBriarRose; the CQ_HeroSoulsBriarScript / CV_IntroductionToTrophiesScript
//   nests are 0x20); CQ_MinionCampScript::Main 0x00D9E5E0 (slot 2 of the quest vtable 0x012D6C60, whose ctor 0x00D9E210 is
//   reached only from the Alloc 0x00DA2F40 registered after `push "S_QMC"` / "Q_MinionCamp") stores that factory in the
//   CEntityScriptBinding record named "BriarRose" (string 0x012D42EC, site 0x00D9E656). Ego_r.pdb / FableWin.pdb carry
//   ??1CBriarRose@CQ_MinionCampScript@NScript@@UAE@XZ; no vtable store in the body = compiler-generated dtor.
// Layout: Ego_r.pdb CQ_MinionCampScript::CBriarRose (0x50, same as retail): CEntityScriptBase 0x14, ParentClass +0x14,
//   PMasterData +0x18, seh_me +0x1c, MyDestination +0x2c, Target +0x38, NumHitsBetweenForcePushes +0x44, SpeechTimer +0x48,
//   IsPlayingChantAnim +0x4c.
// Callees: CTimer::~CTimer inline (engine/CTimer.h: interface slot 88 DeregisterTimer through g_pGameScriptInterface 0x0143E8F8),
//   ~CScriptThing inline (vtable 0x01238C8C, CCountedPointer release, ~CBaseClass 0x0099A2E0), seh_me's implicit dtor inline
//   (PImp release, vtable 0x0126008C, ~CBaseObject 0x0099A430), operator delete 0x00BFE9BC, ~CEntityScriptBase 0x00F35B40.
// TU: the PDB ctor is defined (retail keeps it out of line at 0x00D9E930), which emits the vtable, ??_G and this implicit dtor.
void operator delete(void* p);                          // 0x00BFE9BC
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_MinionCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CBriarRose : public CEntityScriptBase {
    public:
        CQ_MinionCampScript*    ParentClass;                            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                            // +0x18
        CScriptGameResourceObjectScriptedThingBase seh_me;              // +0x1c
        CScriptThing            MyDestination;                          // +0x2c
        CScriptThing            Target;                                 // +0x38
        long                    NumHitsBetweenForcePushes;              // +0x44
        CTimer                  SpeechTimer;                            // +0x48
        bool                    IsPlayingChantAnim;                     // +0x4c
        CBriarRose(CQ_MinionCampScript& parent, CScriptDataBase* data, const CScriptThing& me);
    private:
        virtual void Main();
    public:
        virtual CScriptBase* GetParentScript() const;
    };
};
CQ_MinionCampScript::CBriarRose::CBriarRose(CQ_MinionCampScript& parent, CScriptDataBase* data, const CScriptThing& me)   // 0x00D9E930
    : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}