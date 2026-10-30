// NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::OnPredicateFail() -- a freedom fighter died: one bandit fewer, flag HangingTreeBanditKilled in the master data and BanditKilled in the parent
// Leaf: ?OnPredicateFail@CHTE_FreedomFighter@CQ_HangingTreeEvilScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6A4F0, 77 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012D2170, which the entity Alloc 0x00D6A3D0 (`NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Alloc`) stores; ?OnPredicateFail@CHTE_FreedomFighter@CQ_HangingTreeEvilScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d6a4f0" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::MsgIsKilledBy (slot 18, +0x48; CScriptThing vtable 0x01238C8C).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. Parent: BanditKilled +0x4a, CurrentBandits +0x64; CQ_SunnyvaleMasterData::HangingTreeBanditKilled +0x124.
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
class CQ_HangingTreeEvilScript {
public:
    class CHTE_FreedomFighter;
    unsigned char _pad000[0x4a];
    bool BanditKilled;                                  // +0x4a (retail = Ego_r.pdb - 0x14)
    unsigned char _pad04b[0x19];
    long CurrentBandits;                                // +0x64
};
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x124];
    bool HangingTreeBanditKilled;                       // +0x124 (Ego_r.pdb offset, same in retail)
};
class CQ_HangingTreeEvilScript::CHTE_FreedomFighter : public CEntityScriptBase {
public:
    CQ_HangingTreeEvilScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      HeroHitMe;                  // +0x1c
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_HangingTreeEvilScript::CHTE_FreedomFighter::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        --ParentClass->CurrentBandits;
        PMasterData->HangingTreeBanditKilled = true;
        ParentClass->BanditKilled = true;
    }
}