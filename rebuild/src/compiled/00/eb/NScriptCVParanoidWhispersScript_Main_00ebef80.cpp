// NScript::CV_ParanoidWhispersScript::Main() -- the Paranoid Whispers quest main: bind the SecretWhisperingMan / SecretWhisperingFriend / PW_NecklaceDiggingSpot entity scripts and spawn the CheckForNecklaceAndQuit and WatchForAngryTraders threads
// Leaf: ?Main@CV_ParanoidWhispersScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBEF80, 588 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the RTTI-less level-script vtable 0x012F0F38 [dtor 0x00EC11F0, RegisterMain 0x00EBEEE0
//   (landed NScript::CV_ParanoidWhispersScript::RegisterMain), Main 0x00EBEF80, Init 0x00EBEEC0 (landed
//   CV_ParanoidWhispersScript::Init), OnPersist 0x00EBEED0]; the row is referenced only from 0x012F0F40. Ego_r.pdb
//   ?Main@CV_ParanoidWhispersScript@NScript@@UAEXXZ. The binding factories are the landed CSecretWhisperingMan::Alloc 0x00EBFB00 /
//   CSecretWhisperingFriend::Alloc 0x00EC0A50 / CPW_NecklaceDiggingSpot::Alloc 0x00EC0900 and the spawned threads the PDB members
//   CheckForNecklaceAndQuit 0x00EBF1E0 (landed) / WatchForAngryTraders 0x00EBF390 (each paired with its own name string).
//   Catalog label sub_00ebef80.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50,
//   CSpawnedFuncBase ctor 0x00CDD450; vtables CEntityScriptBinding<CV_ParanoidWhispersScript> 0x012F0FC8,
//   CSpawnedFunc<CV_ParanoidWhispersScript> 0x012F0F4C; Game slot 64 PostAddScriptedEntities.
// Literals: "SecretWhisperingMan" 0x012F0FD0, "SecretWhisperingFriend" 0x012F0FB0, "PW_NecklaceDiggingSpot" 0x012F0F98,
//   "CheckForNecklaceAndQuit" 0x012F0F80, "WatchForAngryTraders" 0x012F0F68, "" 0x0122D70E.
// Layout: retail NScript headers (CLevelScriptBase 0x48); no CV_ParanoidWhispersScript member is touched.
// Shape: the landed CV_IntroductionToTrophiesScript::Main 0x00E9C4B0 form (bindings with conditional name temporaries, the
//   __forceinline SpawnFunction helper); the last spawn's name temporary is destroyed after the epilogue pops.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CGameScriptInterfaceBase.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ParanoidWhispersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CSecretWhisperingMan {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ParanoidWhispersScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EBFB00
    };
    class CSecretWhisperingFriend {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ParanoidWhispersScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EC0A50
    };
    class CPW_NecklaceDiggingSpot {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ParanoidWhispersScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EC0900
    };
    virtual void Main();
    void CheckForNecklaceAndQuit();                     // 0x00EBF1E0
    void WatchForAngryTraders();                        // 0x00EBF390
    // Source-level spawn helper of the landed quest rows (no retail symbol).
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};

void CV_ParanoidWhispersScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ParanoidWhispersScript>("SecretWhisperingMan", this, &CSecretWhisperingMan::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ParanoidWhispersScript>("SecretWhisperingFriend", this, &CSecretWhisperingFriend::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ParanoidWhispersScript>("PW_NecklaceDiggingSpot", this, &CPW_NecklaceDiggingSpot::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_ParanoidWhispersScript>("CheckForNecklaceAndQuit", 0, this, &CV_ParanoidWhispersScript::CheckForNecklaceAndQuit));
    SpawnFunction(new CSpawnedFunc<CV_ParanoidWhispersScript>("WatchForAngryTraders", 0, this, &CV_ParanoidWhispersScript::WatchForAngryTraders));
}
}   // namespace NScript