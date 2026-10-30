// NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Init() -- note whether the arena is finished (master-data flag or Q_Arena completed), make the guard and the hero allies of each other and give the guard information
// Leaf: ?Init@CBowerstoneIntroGuardToPosh@CQ_BowerstoneTownLifeIntroScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D243E0, 170 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012CB5E0, which the entity Alloc 0x00D24330 (`NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Alloc`) stores; ?Init@CBowerstoneIntroGuardToPosh@CQ_BowerstoneTownLifeIntroScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d243e0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (the Q_Arena temporary is destroyed under the `||` flag in ebx); interface slots 286 IsQuestCompleted, 70 GetHero, 599 EntitySetThingAsAllyOfThing, 360 SetThingHasInformation.
// Data: "Q_Arena" @ 0x012C50A4.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent ArenaFinished +0x48 (Ego_r.pdb +0x5c); CQ_SunnyvaleMasterData ArenaFinished +0x75 (master-data offsets are Ego_r's).
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x75];
    bool ArenaFinished;                                 // +0x75
};
class CQ_BowerstoneTownLifeIntroScript {
public:
    unsigned char _pad000[0x48];
    bool ArenaFinished;                                 // +0x48 (Ego_r.pdb +0x5c)
    class CBowerstoneIntroGuardToPosh;
};
class CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh : public CEntityScriptBase {
public:
    CQ_BowerstoneTownLifeIntroScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void Init();
};
}

void NScript::CQ_BowerstoneTownLifeIntroScript::CBowerstoneIntroGuardToPosh::Init()
{
    ParentClass->ArenaFinished = PMasterData->ArenaFinished || Game->IsQuestCompleted("Q_Arena");
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
    Game->SetThingHasInformation(Me, false, false, false);
}