// NScript::CQ_HangingTreeGoodScript::CHTG_Executioner::Init() -- set Me's combat ability to 100
// Leaf: ?Init@CHTG_Executioner@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D73280, 23 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D29B8, which the entity Alloc 0x00D731A0 (`NScript::CQ_HangingTreeGoodScript::CHTG_Executioner::Alloc`) stores; ?Init@CHTG_Executioner@CQ_HangingTreeGoodScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d73280" is a gap-scan placeholder.
// The vtable is stored by the CHTG_Executioner ctor 0x00D73220 (called from the Alloc 0x00D731A0); label correction: the manifest names that ctor "CACompressor::CACompressor" (propagated). Slot 1 is the landed CHTG_Executioner::Main 0x00D73F40.
// Callees: interface slot 466 EntitySetCombatAbility(const CScriptThing&, float) (100.0f = 0x42C80000).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets.
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
class CQ_HangingTreeGoodScript {
public:
    class CHTG_Executioner;
};
class CQ_HangingTreeGoodScript::CHTG_Executioner : public CEntityScriptBase {
public:
    CQ_HangingTreeGoodScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void Init();
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_Executioner::Init()
{
    Game->EntitySetCombatAbility(Me, 100.0f);
}