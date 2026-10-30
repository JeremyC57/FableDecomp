// NScript::CQ_OrchardFarmRaidScript::CArtefact::Init() -- count one more crate in the parent, add Me to its crate list and give it the quest-core mini-map orb
// Leaf: ?Init@CArtefact@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCFA10, 76 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012DB348, which the entity Alloc 0x00DCDB80 (`NScript::CQ_OrchardFarmRaidScript::CArtefact::Alloc`) stores; ?Init@CArtefact@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00dcfa10" is a gap-scan placeholder.
// Callees: std::vector<CScriptThing>::push_back(const CScriptThing&) 0x008ADF90, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slot 350 MiniMapAddMarker.
// Data: "HUD_ORB_QUEST_CORE" @ 0x01258AB0.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. Parent: CrateCount +0x48, CrateList +0x54 (STLport vector, 12 bytes; Ego_r.pdb +0x68).
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
namespace std {
template <class T> class allocator { public: allocator() { } };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x);                           // 0x008ADF90 for vector<CScriptThing>
};
}
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript {
public:
    class CArtefact;
    unsigned char _pad000[0x48];
    long CrateCount;                                    // +0x48 (retail = Ego_r.pdb - 0x14)
    unsigned char _pad04c[0x8];
    std::vector<CScriptThing> CrateList;                // +0x54
};
class CQ_OrchardFarmRaidScript::CArtefact : public CEntityScriptBase {
public:
    CQ_OrchardFarmRaidScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void Init();
};
}

void NScript::CQ_OrchardFarmRaidScript::CArtefact::Init()
{
    ++ParentClass->CrateCount;
    ParentClass->CrateList.push_back(Me);
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
}