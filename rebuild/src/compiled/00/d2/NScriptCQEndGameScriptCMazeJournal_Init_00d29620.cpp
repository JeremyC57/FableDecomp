// NScript::CQ_EndGameScript::CMazeJournal::Init() -- once the abbey has been seen, the maze journal gets the core-quest minimap orb
// Leaf: ?Init@CMazeJournal@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D29620, 62 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012CBCAC, which the landed
//   NScript::CQ_EndGameScript::CMazeJournal::Alloc 0x00D29570 stores (site 0x00D295A6); Ego_r.pdb has
//   ?Init@CMazeJournal@CQ_EndGameScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: script interface slot 350 MiniMapAddMarker (+0x578); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("HUD_ORB_QUEST_CORE" 0x01258AB0).
// Layout: Ego_r.pdb CMazeJournal: ParentClass +0x14, PMasterData +0x18; CQ_EndGameScript::SeenAbbey Ego_r +0x64 = retail +0x50.
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float         UndeadBar;                            // +0x48
    long          GuiCounter;                           // +0x4c
    bool          SeenAbbey;                            // +0x50
    class CMazeJournal;
};
class CQ_EndGameScript::CMazeJournal : public CEntityScriptBase {
public:
    CQ_EndGameScript*              ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
private:
    virtual void Init();
};

void CQ_EndGameScript::CMazeJournal::Init()
{
    if (ParentClass->SeenAbbey)
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
}
}