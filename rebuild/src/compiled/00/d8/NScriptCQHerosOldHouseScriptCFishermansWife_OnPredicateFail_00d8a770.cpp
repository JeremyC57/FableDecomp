// NScript::CQ_HerosOldHouseScript::CFishermansWife::OnPredicateFail() -- the hero killed the fisherman's wife: take HOH_MoralityLoss morality and flag WifeAttackedAndLeft
// Leaf: ?OnPredicateFail@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8A770, 85 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012D50DC, which the entity Alloc 0x00D8A650 (`NScript::CQ_HerosOldHouseScript::CFishermansWife::Alloc`) stores; ?OnPredicateFail@CFishermansWife@CQ_HerosOldHouseScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d8a770" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::MsgIsKilledBy (slot 18, +0x48; CScriptThing vtable 0x01238C8C); interface slot 156 GiveHeroMorality(float).
// Data: "SCRIPT_NAME_HERO" @ 0x0125D1C8; pGlobalScriptDef @ 0x0143E90C, HOH_MoralityLoss +0xF64 (Ego_r.pdb CScriptDef +0x1020; retail = PDB - 0xBC here, as HangingTreeProtectBanditsBoastReward +0xF50 / PDB +0x100C).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. Parent: WifeAttackedAndLeft +0x4c (Ego_r.pdb +0x60).
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
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv)
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5..9, 11..17 simplified: not called here)
    virtual void GetPos() const;                          // slot 6
    virtual void GetHomePos() const;                      // slot 7
    virtual void GetCurrentMapName() const;               // slot 8
    virtual void GetHomeMapName() const;                  // slot 9
    virtual float GetAngleXY() const;                     // slot 10 (+0x28)
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
class CScriptDef {
public:
    unsigned char _pad000[0xf64];
    float         HOH_MoralityLoss;                       // +0xf64 (Ego_r.pdb +0x1020)
};
extern CScriptDef* pGlobalScriptDef;                     // 0x0143E90C
class CEntityScriptBase {                             // Ego_r.pdb, 20 bytes
public:
    virtual ~CEntityScriptBase();
private:
    virtual void OnPredicateFail();                 // slot 5 (+0x14)
public:
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HerosOldHouseScript {
public:
    class CFishermansWife;
    unsigned char _pad000[0x4c];
    bool WifeAttackedAndLeft;                           // +0x4c (retail = Ego_r.pdb - 0x14)
};
class CQ_HerosOldHouseScript::CFishermansWife : public CEntityScriptBase {
public:
    CQ_HerosOldHouseScript*   ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      TalkedTo;                   // +0x1c
    bool                      LeavingHappy;               // +0x1d
    float                     initial_angle;              // +0x20
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_HerosOldHouseScript::CFishermansWife::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO")) {
        Game->GiveHeroMorality(pGlobalScriptDef->HOH_MoralityLoss);
        ParentClass->WifeAttackedAndLeft = true;
    }
}