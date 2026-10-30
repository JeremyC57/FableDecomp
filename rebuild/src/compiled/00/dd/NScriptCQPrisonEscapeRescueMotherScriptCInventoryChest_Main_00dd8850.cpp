// NScript::CQ_PrisonEscapeRescueMotherScript::CInventoryChest::Main() -- until the hero has found his weapons, close the inventory chest and put the hero's equipment bag in it
// Leaf: ?Main@CInventoryChest@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD8850, 87 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the RTTI-less vtable 0x012DC44C, which the entity Alloc 0x00DD8790 (`NScript::CQ_PrisonEscapeRescueMotherScript::CInventoryChest::Alloc`) stores; ?Main@CInventoryChest@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00dd8850" is a gap-scan placeholder.
// Callees: CEntityScriptBase::IsActiveThreadTerminating() const 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 436 CloseChest, 585 AddItemToContainer.
// Data: "OBJECT_HERO_EQUIPMENT_BAG" @ 0x012DBC1C.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. Parent: HeroHasFoundWeapons +0x65 (Ego_r.pdb +0x79).
// Shape: two guard returns (parent flag, then the terminating test) share retail's epilogue.
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonEscapeRescueMotherScript {
public:
    class CInventoryChest;
    unsigned char _pad000[0x65];
    bool HeroHasFoundWeapons;                           // +0x65 (retail = Ego_r.pdb - 0x14)
};
class CQ_PrisonEscapeRescueMotherScript::CInventoryChest : public CEntityScriptBase {
public:
    CQ_PrisonEscapeRescueMotherScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void Main();
};
}

void NScript::CQ_PrisonEscapeRescueMotherScript::CInventoryChest::Main()
{
    if (ParentClass->HeroHasFoundWeapons)
        return;
    if (IsActiveThreadTerminating())
        return;
    Game->CloseChest(Me);
    Game->AddItemToContainer(Me, "OBJECT_HERO_EQUIPMENT_BAG");
}