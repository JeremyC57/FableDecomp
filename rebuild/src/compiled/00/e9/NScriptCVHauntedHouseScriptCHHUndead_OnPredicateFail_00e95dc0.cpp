// NScript::CV_HauntedHouseScript::CHHUndead::OnPredicateFail() -- an undead is gone: when the hero killed it, raise the house's undead bar by four, three, two or one times its amount (bar below 50 / 100 / 200 / above) and count one undead fewer
// Leaf: ?OnPredicateFail@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E95DC0, 147 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012ECD3C, which the landed
//   NScript::CV_HauntedHouseScript::CHHUndead::Alloc 0x00E95CD0 stores (site 0x00E95D06); Ego_r.pdb has ?OnPredicateFail@CHHUndead@CV_HauntedHouseScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CHHUndead (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing slot 18 MsgIsKilledBy ("" 0x0122D70E temporary); CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0. Float pool: 50.0f 0x0124E7F8, 4.0f 0x0123088C, 100.0f 0x01230A08, 3.0f 0x0122DED4, 200.0f
//   0x0125A168 (the doubling is `fadd st(0),st(0)`).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
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
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08();
    virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16();
    virtual void t17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
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
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HauntedHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    float UndeadBar;                                    // +0x48
    long GuiCounter;                                    // +0x4c
    bool CellarDoorOpen;                                // +0x50
    bool HeroBeenToChest;                               // +0x51
    bool QuestCardGiven;                                // +0x52
    bool InHouse;                                       // +0x53
    long UndeadAlive;                                   // +0x54
    long MaxUndead;                                     // +0x58
    class CHHUndead;
};
class CV_HauntedHouseScript::CHHUndead : public CEntityScriptBase {
public:
    CV_HauntedHouseScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    float RaiseUndeadBarAmount;                         // +0x1c
private:
    virtual void OnPredicateFail();
};

void CV_HauntedHouseScript::CHHUndead::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        float amount;
        if (ParentClass->UndeadBar < 50.0f)
            amount = RaiseUndeadBarAmount * 4.0f;
        else if (ParentClass->UndeadBar < 100.0f)
            amount = RaiseUndeadBarAmount * 3.0f;
        else if (ParentClass->UndeadBar < 200.0f)
            amount = RaiseUndeadBarAmount * 2.0f;
        else
            amount = RaiseUndeadBarAmount;
        ParentClass->UndeadBar += amount;
        --ParentClass->UndeadAlive;
    }
}
}