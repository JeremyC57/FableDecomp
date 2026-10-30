// NScript::CQ_HerosOldHouseScript::CGhostFisherman::Init() -- set up the ghost fisherman: flags cleared, walks through solid objects, cannot be damaged, force-pushed or killed, targeting type 2, an evil opinion source with information, and a vignette marker unless the hero is already helping
// Leaf: ?Init@CGhostFisherman@CQ_HerosOldHouseScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D8A560, 204 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D50C0, which the entity Alloc 0x00D8A4B0 (`NScript::CQ_HerosOldHouseScript::CGhostFisherman::Alloc`) stores; ?Init@CGhostFisherman@CQ_HerosOldHouseScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d8a560" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 514 EntitySetAsAbleToWalkThroughSolidObjects, 516 EntitySetAsDamageable, 745 SetIsThingForcePushable, 517 EntitySetAsKillable, 470 EntitySetTargetingType, 622 EntitySetAsOpinionSource, 360 SetThingHasInformation, 350 MiniMapAddMarker.
// Data: "OPINION_SOURCE_INANIMATE_EVIL_HIGH" @ 0x012CB260, "HUD_ORB_QUEST_VIGNETTE" @ 0x01258A64.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). CGhostFisherman: GhostGoing +0x1c, HaveTalked +0x1d; parent Helping +0x48 (Ego_r.pdb +0x5c). The targeting value 2 has no recovered enumerator name.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
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
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv / Ego_r.pdb)
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5, 7..17, 19 simplified: not called here)
    virtual const C3DVector& GetPos() const;              // slot 6 (+0x18)
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
    virtual void t019() const;                            // slot 19
    virtual bool MsgIsUsedBy(const CCharString& user) const;       // slot 20 (+0x50)
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};
enum ETargetingType { };                                 // Ego_r.pdb enum (global scope); value 2 unnamed here
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CEntityScriptBase {                             // Ego_r.pdb, 20 bytes
public:
    virtual ~CEntityScriptBase();
private:
    virtual void Init();                            // slot 2 (+0x8)
public:
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HerosOldHouseScript {
public:
    unsigned char _pad000[0x48];
    bool Helping;                                       // +0x48 (Ego_r.pdb +0x5c)
    class CGhostFisherman;
};
class CQ_HerosOldHouseScript::CGhostFisherman : public CEntityScriptBase {
public:
    CQ_HerosOldHouseScript*   ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      GhostGoing;                 // +0x1c
    bool                      HaveTalked;                 // +0x1d
private:
    virtual void Init();
};
}

void NScript::CQ_HerosOldHouseScript::CGhostFisherman::Init()
{
    HaveTalked = false;
    GhostGoing = false;
    Game->EntitySetAsAbleToWalkThroughSolidObjects(Me, true);
    Game->EntitySetAsDamageable(Me, false);
    Game->SetIsThingForcePushable(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetTargetingType(Me, (ETargetingType)2);
    Game->EntitySetAsOpinionSource(Me, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
    Game->SetThingHasInformation(Me, false, true, false);
    if (!ParentClass->Helping)
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_VIGNETTE");
}