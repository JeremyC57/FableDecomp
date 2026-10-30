// NScript::CV_SickChildScript::CIngredientOwner::Init() -- clear BoughtSpecialStuff/StoleSpecialStuff; if the parent's ingredient owner is dead, remove Me
// Leaf: ?Init@CIngredientOwner@CV_SickChildScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECB050, 39 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012F2000, stored by the entity Alloc 0x00ECAFA0 (`NScript::CV_SickChildScript::CIngredientOwner::Alloc`); ?Init@CIngredientOwner@CV_SickChildScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00ecb050" is a gap-scan placeholder.
// Callees: interface slot 108 RemoveThing(const CScriptThing&, bool, bool).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets: BoughtSpecialStuff +0x1c, StoleSpecialStuff +0x1d; parent CV_SickChildScript::OwnerAlive +0x70 (retail = Ego_r.pdb - 0x14).
// Shape: the RemoveThing arguments are (Me, false, true): `push 1 ; push edx(0) ; push &Me`.
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_SickChildScript {
public:
    class CIngredientOwner;
    unsigned char _pad000[0x70];
    bool OwnerAlive;                                    // +0x70 (Ego_r.pdb +0x84)
};
class CV_SickChildScript::CIngredientOwner : public CEntityScriptBase {
public:
    CV_SickChildScript*     ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    bool                    BoughtSpecialStuff;         // +0x1c
    bool                    StoleSpecialStuff;          // +0x1d
private:
    virtual void Init();
};
}

void NScript::CV_SickChildScript::CIngredientOwner::Init()
{
    BoughtSpecialStuff = false;
    StoleSpecialStuff = false;
    if (!ParentClass->OwnerAlive)
        Game->RemoveThing(Me, false, true);
}