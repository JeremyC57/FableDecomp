// NScript::CQ_AmbushTradersScript::CAT_BanditAlly::OnPredicateFail() -- count the kill in the master data and remove the bandit's health bar if it has one
// Leaf: ?OnPredicateFail@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F03850, 29 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F7D88, stored by the entity Alloc 0x00F037A0 (`NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Alloc`); ?OnPredicateFail@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00f03850" is a gap-scan placeholder.
// Callees: interface slot 338 RemoveQuestInfoElement(long).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets: BanditHealthBarID +0x24; CQ_SunnyvaleMasterData::AmbushTradersKillCount +0xcc (Ego_r.pdb offset, same in retail).
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
class CEntityScriptBase {                             // Ego_r.pdb, 20 bytes: {vptr, Game +4, CScriptThing Me +8}
public:
    virtual ~CEntityScriptBase();
private:
    virtual void OnPredicateFail();                          // slot 5 (+0x14)
public:
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0xcc];
    long AmbushTradersKillCount;                        // +0xcc (Ego_r.pdb offset, same in retail)
};
class CQ_AmbushTradersScript {
public:
    class CAT_BanditAlly;
};
class CQ_AmbushTradersScript::CAT_BanditAlly : public CEntityScriptBase {
public:
    CQ_AmbushTradersScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    bool                    BanditHired;                // +0x1c
    bool                    BanditFollowing;            // +0x1d
    bool                    BanditTeleported;           // +0x1e
    bool                    BanditSwitchedAllegience;   // +0x1f
    bool                    BanditRemovedFromBowerstonePosh; // +0x20
    bool                    ResetInvalidRegionTimer;    // +0x21
    long                    BanditHealthBarID;          // +0x24
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_AmbushTradersScript::CAT_BanditAlly::OnPredicateFail()
{
    ++PMasterData->AmbushTradersKillCount;
    if (BanditHealthBarID > 0)
        Game->RemoveQuestInfoElement(BanditHealthBarID);
}