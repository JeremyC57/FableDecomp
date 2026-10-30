// NScript::CQ_HeroSoulsMotherScript::CHS_Screamer::Init() -- one more screamer alive in the parent; SuperScreamer = false
// Leaf: ?Init@CHS_Screamer@CQ_HeroSoulsMotherScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D83F10, 11 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D477C, which the entity Alloc 0x00D83E60 (`NScript::CQ_HeroSoulsMotherScript::CHS_Screamer::Alloc`) stores; ?Init@CHS_Screamer@CQ_HeroSoulsMotherScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d83f10" is a gap-scan placeholder.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CHS_Screamer: SuperScreamer +0x1c; parent ScreamersAlive +0x58 (Ego_r.pdb +0x70; the csargs map is 0xC in retail), as the landed CHS_Screamer::OnPredicateFail 0x00D83F20.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
class CGameScriptInterfaceBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsMotherScript {
public:
    class CHS_Screamer;
    unsigned char _pad000[0x58];
    long ScreamersAlive;                                // +0x58 (Ego_r.pdb - 0x18)
};
class CQ_HeroSoulsMotherScript::CHS_Screamer : public CEntityScriptBase {
public:
    CQ_HeroSoulsMotherScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    bool                      SuperScreamer;              // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_HeroSoulsMotherScript::CHS_Screamer::Init()
{
    ++ParentClass->ScreamersAlive;
    SuperScreamer = false;
}