// NScript::CQ_RansomVictimScript::CVictim::Init() -- WaitingForThingsToDie false, ToldHeroOfThingsAttackingHim true, GotOutOfCave false; make the victim not pushable by the hero
// Leaf: ?Init@CVictim@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE1B60, 61 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012DD1AC, which the entity Alloc 0x00DE1A50 (`NScript::CQ_RansomVictimScript::CVictim::Alloc`) stores; ?Init@CVictim@CQ_RansomVictimScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00de1b60" is a gap-scan placeholder.
// Callees: interface slot 844 SetIsPushableByHero(CScriptThing, bool) (by value: the copy is built in the argument slot).
// Data: vtables CBaseClass 0x01231710, CScriptThing 0x01238C8C.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CVictim: WaitingForThingsToDie +0x1c, ToldHeroOfThingsAttackingHim +0x1d, GotOutOfCave +0x1e.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
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
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_RansomVictimScript {
public:
    class CVictim;
};
class CQ_RansomVictimScript::CVictim : public CEntityScriptBase {
public:
    CQ_RansomVictimScript*    ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      WaitingForThingsToDie;      // +0x1c
    bool                      ToldHeroOfThingsAttackingHim; // +0x1d
    bool                      GotOutOfCave;               // +0x1e
private:
    virtual void Init();
};
}

void NScript::CQ_RansomVictimScript::CVictim::Init()
{
    WaitingForThingsToDie = false;
    ToldHeroOfThingsAttackingHim = true;
    GotOutOfCave = false;
    Game->SetIsPushableByHero(Me, false);
}