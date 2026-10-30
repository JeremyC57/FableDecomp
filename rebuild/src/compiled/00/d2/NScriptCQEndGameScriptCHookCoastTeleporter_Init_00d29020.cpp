// NScript::CQ_EndGameScript::CHookCoastTeleporter::Init() -- while the end-game undead bar is below the def's bar decrement, the Hook Coast teleporter is made persistent and marked with the core-quest minimap orb
// Leaf: ?Init@CHookCoastTeleporter@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D29020, 93 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012CBC3C, which the landed
//   NScript::CQ_EndGameScript::CHookCoastTeleporter::Alloc 0x00D28F70 stores (site 0x00D28FA6); Ego_r.pdb has
//   ?Init@CHookCoastTeleporter@CQ_EndGameScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: script interface slots 477 SetThingPersistent (+0x774) and 350 MiniMapAddMarker (+0x578); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0
//   ("HUD_ORB_QUEST_CORE" 0x01258AB0); pGlobalScriptDef 0x0143E90C.
// Layout: Ego_r.pdb CHookCoastTeleporter: ParentClass +0x14, PMasterData +0x18; CQ_EndGameScript::UndeadBar (float) Ego_r +0x5c = retail +0x48.
// Shape: `fld [UndeadBar] ; fcomp [def] ; test ah,5 ; jp` is VC7.1's `<`.
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the virtual calls and the by-value copy need
//   a polymorphic local model (as the landed entity-script rows use).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CScriptDef {                                     // the global script def (TLC member order, landed CScriptDef ctor 0x00F28F90)
public:
    unsigned char _pad000[0xbe8];
    float         EndGame_BarDecrementAmount;           // +0xbe8
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing              Me;                       // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float UndeadBar;                                    // +0x48
    class CHookCoastTeleporter;
};
class CQ_EndGameScript::CHookCoastTeleporter : public CEntityScriptBase {
public:
    CQ_EndGameScript*              ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
private:
    virtual void Init();
};

void CQ_EndGameScript::CHookCoastTeleporter::Init()
{
    if (ParentClass->UndeadBar < pGlobalScriptDef->EndGame_BarDecrementAmount)
    {
        Game->SetThingPersistent(Me, true);
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    }
}
}