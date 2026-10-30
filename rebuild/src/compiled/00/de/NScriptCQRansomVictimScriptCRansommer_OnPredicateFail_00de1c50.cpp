// NScript::CQ_RansomVictimScript::CRansommer::OnPredicateFail() -- the ransommer was killed: flag RescuedVictim and RansommerDied and count one more kidnapper killed
// Leaf: ?OnPredicateFail@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE1C50, 74 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012DD1C8, which the entity Alloc 0x00DE1BA0 (`NScript::CQ_RansomVictimScript::CRansommer::Alloc`) stores; ?OnPredicateFail@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00de1c50" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::MsgIsKilledBy (slot 18, +0x48; CScriptThing vtable 0x01238C8C).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. Parent: RescuedVictim +0x4b, RansommerDied +0x4c, KidnappersKilled +0x60.
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
class CGameScriptInterfaceBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_RansomVictimScript {
public:
    class CRansommer;
    unsigned char _pad000[0x4b];
    bool RescuedVictim;                                 // +0x4b (retail = Ego_r.pdb - 0x14)
    bool RansommerDied;                                 // +0x4c
    unsigned char _pad04d[0x13];
    long KidnappersKilled;                              // +0x60
};
class CQ_RansomVictimScript::CRansommer : public CEntityScriptBase {
public:
    CQ_RansomVictimScript*    ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_RansomVictimScript::CRansommer::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->RescuedVictim = true;
        ParentClass->RansommerDied = true;
        ++ParentClass->KidnappersKilled;
    }
}