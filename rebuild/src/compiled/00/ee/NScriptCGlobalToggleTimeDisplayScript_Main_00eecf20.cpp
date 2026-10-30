// NScript::CGlobal_ToggleTimeDisplayScript::Main() -- toggle the time display and deactivate the active quest (this toggle script) at once
// Leaf: ?Main@CGlobal_ToggleTimeDisplayScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EECF20, 69 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the level-script vtable 0x012F57A0 (slot 1 = `NScript::CGlobal_ToggleTimeDisplayScript::RegisterMain`), stored by 0x00EECEF0; ?Main@CGlobal_ToggleTimeDisplayScript@NScript@@UAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00eecf20" is a gap-scan placeholder.
// Callees: interface slots 340 DisplayTime(bool), 655 GetActiveQuestName() (CCharString by value), 281 DeactivateQuestLater(const CCharString&, unsigned long); ~CCharString 0x0099EAE0.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 (and the master data +0x44 where the ctor takes one); the script's own members sit 0x14 below their Ego_r.pdb offsets: no master data (the ctor takes only the interface), TimeDisplayActive +0x44 (Ego_r.pdb +0x58).
// Shape: the DeactivateQuestLater vtable (edi) is read once, before the GetActiveQuestName argument call.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
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
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv)
public:
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5..17 simplified: not called here)
    virtual void GetPos() const;                          // slot 6
    virtual void GetHomePos() const;                      // slot 7
    virtual void GetCurrentMapName() const;               // slot 8
    virtual void GetHomeMapName() const;                  // slot 9
    virtual float GetAngleXY() const;                     // slot 10
    virtual void GetPThing() const;                       // slot 11
    virtual void GetPThingUniqueID() const;               // slot 12
    virtual void GetCurrentStateGroupType() const;        // slot 13
    virtual void GetCurrentScriptPriority() const;        // slot 14
    virtual void IsSneaking() const;                      // slot 15
    virtual void IsAwareOfHero() const;                   // slot 16
    virtual void MsgIsTriggeredBy() const;                // slot 17
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGlobal_ToggleTimeDisplayScript : public CScriptBase {
public:
    virtual void Main();
    CGameScriptInterfaceBase* Game;                     // +0x40
    bool TimeDisplayActive;                             // +0x44
};
}

void NScript::CGlobal_ToggleTimeDisplayScript::Main()
{
    TimeDisplayActive = !TimeDisplayActive;
    Game->DisplayTime(TimeDisplayActive);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}