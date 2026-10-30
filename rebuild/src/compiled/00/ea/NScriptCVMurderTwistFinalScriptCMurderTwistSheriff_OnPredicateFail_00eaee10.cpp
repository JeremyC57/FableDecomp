// NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::OnPredicateFail() -- when the sheriff was killed, give the hero the sheriff-kill morality and renown and move the quest to sheriff state 4
// Leaf: ?OnPredicateFail@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAEE10, 112 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012EFA0C, which the landed
//   NScript::CV_MurderTwistFinalScript::CMurderTwistSheriff::CMurderTwistSheriff 0x00EAED90 (factory 0x00EAED10) stores (site 0x00EAEDBB); Ego_r.pdb has ?OnPredicateFail@CMurderTwistSheriff@CV_MurderTwistFinalScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMurderTwistSheriff (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("" 0x0122D70E); CScriptThing::MsgIsKilledBy (slot 18); interface slots
//   156 GiveHeroMorality, 161 GiveHeroRenownPoints; pGlobalScriptDef 0x0143E90C.
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptDef {                                      // Ego_r.pdb CScriptDef; retail offsets (Ego_r - 0x5c: 23 vectors of 0x10 are 0xC)
public:
    unsigned char _pad0[0x708];
    float HeroKillsSheriffMorality;                     // +0x708 (Ego_r +0x764)
    unsigned char _pad70c[0x71c - 0x70c];
    long  HeroKillsSheriffRenown;                       // +0x71c (Ego_r +0x778)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
enum ESheriffState { };                                 // NScript::CV_MurderTwistFinalScript::ESheriffState in Ego_r.pdb (value 4 unnamed here)
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
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
class CV_MurderTwistFinalScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    ESheriffState SheriffState;                         // +0x48
    class CMurderTwistSheriff;
};
class CV_MurderTwistFinalScript::CMurderTwistSheriff : public CEntityScriptBase {
public:
    CV_MurderTwistFinalScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    unsigned char MyEntityHandle[0x10];                 // +0x1c CScriptGameResourceObjectScriptedThingBase
    long          AttractTimer;                         // +0x2c CTimer
    unsigned long NextLine;                             // +0x30
private:
    virtual void OnPredicateFail();
};

void CV_MurderTwistFinalScript::CMurderTwistSheriff::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        Game->GiveHeroMorality(pGlobalScriptDef->HeroKillsSheriffMorality);
        Game->GiveHeroRenownPoints(pGlobalScriptDef->HeroKillsSheriffRenown);
        ParentClass->SheriffState = (ESheriffState)4;
    }
}
}