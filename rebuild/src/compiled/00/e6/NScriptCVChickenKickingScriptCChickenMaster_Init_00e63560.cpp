// NScript::CV_ChickenKickingScript::CChickenMaster::Init() -- clear the chicken master's talk / played / kicking / ghost-chat flags
// Leaf: ?Init@CChickenMaster@CV_ChickenKickingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E63560, 15 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E747C, which the landed
//   NScript::CV_ChickenKickingScript::CChickenMaster::Alloc 0x00E634B0 stores (site 0x00E634E6); Ego_r.pdb has ?Init@CChickenMaster@CV_ChickenKickingScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CChickenMaster (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
class CBaseClass {                                      // vtable 0x01231710
public:
    CBaseClass() { }
    virtual ~CBaseClass();
};
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
class CV_ChickenKickingScript {
public:
    class CChickenMaster;
};
class CV_ChickenKickingScript::CChickenMaster : public CEntityScriptBase {
public:
    CV_ChickenKickingScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    bool HaveTalked;                                    // +0x1c
    bool HeroHasPlayed;                                 // +0x1d
    bool KickingChickens;                               // +0x1e
    bool GhostChat;                                     // +0x1f
private:
    virtual void Init();
};

void CV_ChickenKickingScript::CChickenMaster::Init()
{
    HeroHasPlayed = false;
    HaveTalked = false;
    KickingChickens = false;
    GhostChat = false;
}
}