// NScript::CV_LostTraderScript::CLostTraderBrother::OnPredicateFail() -- when the brother was killed (by anyone), the quest fails for reason 1 and the brother is flagged killed
// Leaf: ?OnPredicateFail@CLostTraderBrother@CV_LostTraderScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA50E0, 71 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012EE034, which the landed
//   NScript::CV_LostTraderScript::CLostTraderBrother Alloc 0x00EA4FF0 (new 0x2c; the catalog folds it into the landed
//   CLostTraderTrader::MakeRandomComment span, which really ends at 0x00EA4FF0); slot 2 of the vtable is the landed
//   CLostTraderBrother::Init 0x00EA50A0 stores (site 0x00EA5024); Ego_r.pdb has ?OnPredicateFail@CLostTraderBrother@CV_LostTraderScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CLostTraderBrother (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("" 0x0122D70E); CScriptThing::MsgIsKilledBy (slot 18, 0x004AAAF0).
// Shape (family L137): the bool result is kept in bl across the temporary's destructor.
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
    virtual ~CScriptThing();                            // slot 0
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08();
    virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16();
    virtual void t17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48) 0x004AAAF0
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
class CV_LostTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool g_bLostTraderKilled;                           // +0x48
    bool g_bTraderBrotherKilled;                        // +0x49
    bool g_bQuestFailed;                                // +0x4a
    bool g_bTraderHit;                                  // +0x4b
    bool ReachedTrader;                                 // +0x4c
    long DistanceForBrotherToReunite;                   // +0x50
    long ReasonForFailure;                              // +0x54
    class CLostTraderBrother;
};
class CV_LostTraderScript::CLostTraderBrother : public CEntityScriptBase {
public:
    CV_LostTraderScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long stateBrain;                                    // +0x1c
    bool HasToldHeroWhereBrotherIs;                     // +0x20
    long TalkCounter;                                   // +0x24
    bool DoneIntro;                                     // +0x28
private:
    virtual void OnPredicateFail();
};

void CV_LostTraderScript::CLostTraderBrother::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->ReasonForFailure = 1;
        ParentClass->g_bTraderBrotherKilled = true;
    }
}
}