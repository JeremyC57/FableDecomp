// NScript::CV_TrophyDealerScript::CTrophyDealerInCave::Init() -- clear Me's information flags
// Leaf: ?Init@CTrophyDealerInCave@CV_TrophyDealerScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE73F0, 24 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012F52C0, stored by the entity Alloc 0x00EE7340 (`NScript::CV_TrophyDealerScript::CTrophyDealerInCave::Alloc`); ?Init@CTrophyDealerInCave@CV_TrophyDealerScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00ee73f0" is a gap-scan placeholder.
// Callees: interface slot 360 SetThingHasInformation(const CScriptThing&, bool, bool, bool).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
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
class CV_TrophyDealerScript {
public:
    class CTrophyDealerInCave;
};
class CV_TrophyDealerScript::CTrophyDealerInCave : public CEntityScriptBase {
public:
    CV_TrophyDealerScript*  ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void Init();
};
}

void NScript::CV_TrophyDealerScript::CTrophyDealerInCave::Init()
{
    Game->SetThingHasInformation(Me, false, false, false);
}