// NScript::CQ_FireHeartScript::CProphetCage::Main() -- a Fire Heart prophet cage: under an alive filter on Me, wait for this cage's prophet to be released or killed; then wait for the OPENCAGE cutscene flag, burst the cage with the crystal implode (
// Leaf: ?Main@CProphetCage@CQ_FireHeartScript@NScript@@EAEXXZ
// Revisit: inline-depth -- byte-exact, but reached with #pragma inline_depth regions retail most likely never wrote; redo pragma-free once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38F70, length 955 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing 0x004AA840),
// SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0 ("OPENCAGE"
// 0x012CCC28, "FOO" 0x012CD1E0, "CRYSTAL_IMPLODE_01" 0x012CD1CC, "CRYSTAL_OPEN_01" 0x012CD1BC), the STLport map<CCharString, bool>
// operator[] 0x008ADF10 (NF85), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, FadeOutAndKillEntity 0x00CBE716 (__fastcall);
// CScriptThing slot 6 GetPos; CGameScriptInterfaceBase slots 7 NewScriptFrame, 100 CreateEffect.
// Layout: engine/CQ_FireHeartScript.h (CutsceneFlags +0x4c, ProphetsKilled +0xc8, ProphetsReleased +0xd4) and engine/CEntityScriptBase.h;
//   the entity's own members (Ego_r.pdb): ParentClass +0x14, PMasterData +0x18, IWasActivated +0x1c, MyID +0x20.
// Shape: the CMagicBarrierGraveyard::Main 0x00CE0600 model (inline ~CScriptThing, inline_depth(0) around the filter's inline destructor);
//   the discarded CreateEffect result is destroyed inline.
// NF87: moved off the row-local CCharString / CGameScriptInterface (placeholder vtable) / CQ_FireHeartScript / CEntityScriptBase /
//   CIsThingAlive / CCutsceneFlagMap models onto the shared headers, so 0x008ADF10 binds the STLport map<CCharString, bool> spelling.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() {}
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#pragma inline_depth(0)
inline NScript::CIsThingAlive::~CIsThingAlive() { }
#pragma inline_depth()
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_fade.h"

namespace NScript {
class CQ_FireHeartScript::CProphetCage : public CEntityScriptBase {
public:
    CQ_FireHeartScript*     ParentClass;            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    bool                    IWasActivated;          // +0x1c
    long                    MyID;                   // +0x20
private:
    virtual void Main();
};
}

void NScript::CQ_FireHeartScript::CProphetCage::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IWasActivated) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->ProphetsReleased[MyID]) {
            if (IsActiveThreadTerminating())
                return;
            while (ParentClass->CutsceneFlags["OPENCAGE"] != true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->CreateEffect("CRYSTAL_IMPLODE_01", Me.GetPos(), "FOO", 0.0f, false, false);
            FadeOutAndKillEntity(Me, 0.2f, true, true);
            ParentClass->CutsceneFlags["OPENCAGE"] = false;
            IWasActivated = true;
        } else if (ParentClass->ProphetsKilled[MyID]) {
            if (IsActiveThreadTerminating())
                return;
            while (ParentClass->CutsceneFlags["OPENCAGE"] != true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->CreateEffect("CRYSTAL_OPEN_01", Me.GetPos(), "FOO", 0.0f, false, false);
            FadeOutAndKillEntity(Me, 0.2f, true, true);
            ParentClass->CutsceneFlags["OPENCAGE"] = false;
            IWasActivated = true;
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}
