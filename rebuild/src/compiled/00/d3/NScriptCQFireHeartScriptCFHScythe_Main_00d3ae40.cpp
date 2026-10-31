// NScript::CQ_FireHeartScript::CFH_Scythe::Main() -- take scripting control of the scythe entity (releasing a stale handle, retrying a script frame at a time), fade in unless the hero is within 50 units, then play the scythe cutscene through the parent quest, mark it played, re-enable boasting and remove the scythe for good
// Leaf: ?Main@CFH_Scythe@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D3AE40, 264 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?Main@CFH_Scythe@CQ_FireHeartScript@NScript@@EAEXXZ (private virtual; the PDB also lists ?Init@ / ?Alloc@
//   of CFH_Scythe); callers: the entity vtable slot 1 (CEntityScriptBase::Main).
// Callees: CEntityScriptBase::IsActiveThreadTerminating() const 0x00F35B30; the handle's IsValid() const 0x00CD23B9 / Release
//   0x00CD2770 (CScriptGameResourceObjectScriptedThingBase, reloc bindings + engine header); IsDistanceBetweenThingsUnder(const
//   CScriptThing&, const CScriptThing&, float) 0x00CBE2FF (Ego_r.pdb ?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z,
//   __fastcall, the float on the stack); CCharString::CCharString(const char*, long) 0x0099EBF0 ("CS_FIREHEART_SCYTHE"
//   0x012CD42C); CQ_FireHeartScript::PlayCutscene(CCharString) 0x00D39330 (Ego_r.pdb ?PlayCutscene@CQ_FireHeartScript@NScript@@
//   QAEXVCCharString@@@Z); CGameScriptInterfaceBase slots 5 DeactivateBoastUi, 7 NewScriptFrame, 8 StartScriptingEntity, 70
//   GetHero, 108 RemoveThing, 374 FadeScreenIn, 477 SetThingPersistent, 882 SetBoastingEnabled (engine header).
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} (engine/CEntityScriptBase.h), Ego_r.pdb CFH_Scythe ParentClass +0x14,
//   PMasterData +0x18. CQ_FireHeartScript (no shared header; retail is 0x64 below Ego_r.pdb): seh_Scythe +0x164 (PDB +0x1c8),
//   HasScytheCutscenePlayed +0x1a1 (PDB +0x205), the landed CFH_BriarRose::Main 0x00D37330 offsets.
// Shape: the stale-handle release goes through a named PARENT pointer local and the member twice
//   (`parent->seh_Scythe.IsValid()` / `.Release()`): VC7.1 loads the parent into eax ahead of the edi push and forms the
//   handle with `lea edi,[eax+164h]` (a handle-pointer local, a reference, `&ParentClass->seh_Scythe` and an inline accessor
//   all give `push edi ; mov edi,[esi+14h] ; add edi,164h`); the StartScriptingEntity loop is rotated with &Me in edi; the
//   distance test's 50.0f is pushed before the GetHero call (right to left); the cutscene name is a by-value CCharString with
//   a user copy constructor built in its argument slot.
class CThing;
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#include "engine/CGameScriptInterface.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_FireHeartScript {                             // retail offsets (Ego_r.pdb less 0x64)
public:
    unsigned char _pad0[0x164];
    CScriptGameResourceObjectScriptedThingBase seh_Scythe;   // +0x164
    unsigned char _pad174[0x1a1 - 0x174];
    bool          HasScytheCutscenePlayed;              // +0x1a1
    void PlayCutscene(CCharString name);                // 0x00D39330

    class CFH_Scythe : public CEntityScriptBase {
    private:
        virtual void Main();
    public:
        CQ_FireHeartScript*     ParentClass;            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    };
};

void CQ_FireHeartScript::CFH_Scythe::Main()
{
    CQ_FireHeartScript* parent = ParentClass;
    if (parent->seh_Scythe.IsValid())
        parent->seh_Scythe.Release();
    while (!Game->StartScriptingEntity(Me, ParentClass->seh_Scythe, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 50.0f)) {
        if (IsActiveThreadTerminating())
            return;
        Game->FadeScreenIn();
    }
    Game->DeactivateBoastUi();
    ParentClass->PlayCutscene(CCharString("CS_FIREHEART_SCYTHE", -1));
    ParentClass->HasScytheCutscenePlayed = true;
    Game->SetBoastingEnabled(true);
    Game->SetThingPersistent(Me, true);
    Game->RemoveThing(Me, false, true);
}
}