// NScript::CQ_BanditCampBossBattleScript::CBanditKing::OnPredicateFail() -- when the bandit king was killed, record it (TwinBladeKilled, BanditCampTwinbladeKilled); then hide the quest info and remove the king's health bar
// Leaf: ?OnPredicateFail@CBanditKing@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0A7B0, 96 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012C9474, which the landed
//   NScript::CQ_BanditCampBossBattleScript::CBanditKing::Alloc 0x00D0A700 stores (site 0x00D0A736); Ego_r.pdb has
//   ?OnPredicateFail@CBanditKing@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (the literal "" 0x0122D70E), CScriptThing::MsgIsKilledBy (virtual slot 18, +0x48),
//   script interface slots 321 DisplayQuestInfo (+0x504) and 338 RemoveQuestInfoElement (+0x548).
// Layout: Ego_r.pdb CBanditKing: ParentClass +0x14, PMasterData +0x18, HealthBarIndex +0x1c; CQ_BanditCampBossBattleScript::TwinBladeKilled
//   Ego_r +0x78 = retail +0x64; CQ_SunnyvaleMasterData::BanditCampTwinbladeKilled +0x3a (master-data offsets equal retail).
// Shape: the H035 OnPredicateFail shape (landed 0x00D1DD80): the CCharString temporary dies before the result (bl) is tested.
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
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48) 0x004AAAF0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
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
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing              Me;                       // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x3a];
    bool          BanditCampTwinbladeKilled;            // +0x3a
};
class CQ_BanditCampBossBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _pad48[0x64 - 0x48];
    bool          TwinBladeKilled;                      // +0x64
    class CBanditKing;
};
class CQ_BanditCampBossBattleScript::CBanditKing : public CEntityScriptBase {
public:
    CQ_BanditCampBossBattleScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
    long                          HealthBarIndex;   // +0x1c
private:
    virtual void OnPredicateFail();
};

void CQ_BanditCampBossBattleScript::CBanditKing::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
    {
        ParentClass->TwinBladeKilled = true;
        PMasterData->BanditCampTwinbladeKilled = true;
    }
    Game->DisplayQuestInfo(false);
    Game->RemoveQuestInfoElement(HealthBarIndex);
}
}