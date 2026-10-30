// NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Init() -- Maze at the tavern starts with no talks, no wave and no hits
// Leaf: ?Init@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25D20, 12 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E1E28, which the landed
//   NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Alloc 0x00E25C70 stores (site 0x00E25CA6); Ego_r.pdb has ?Init@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMazeAtTavern (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
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
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterface;
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
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianSisterInfoScript {
public:
    class CMazeAtTavern;
};
class CQS_GuardianSisterInfoScript::CMazeAtTavern : public CEntityScriptBase {
public:
    CQS_GuardianSisterInfoScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long TalkCounter;                                   // +0x1c
    bool WavedOver;                                     // +0x20
    long HitCount;                                      // +0x24
private:
    virtual void Init();
};

void CQS_GuardianSisterInfoScript::CMazeAtTavern::Init()
{
    TalkCounter = 0;
    WavedOver = false;
    HitCount = 0;
}
}