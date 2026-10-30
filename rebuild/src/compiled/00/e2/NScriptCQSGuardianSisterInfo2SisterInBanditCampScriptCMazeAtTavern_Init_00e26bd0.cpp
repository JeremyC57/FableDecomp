// NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::Init() -- reset the talk/wave/hit state, put Maze on the minimap and make him unpushable
// Leaf: ?Init@CMazeAtTavern@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E26BD0, 118 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012E1F74, which the landed
//   NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::Alloc 0x00E26AF0 stores (site 0x00E26B26); Ego_r.pdb has ?Init@CMazeAtTavern@CQS_GuardianSisterInfo2_SisterInBanditCampScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CMazeAtTavern (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("HUD_ORB_QUEST_CORE" 0x01258AB0); interface
//   slots 350 MiniMapAddMarker, 844 SetIsPushableByHero.
// Shape: the by-value CScriptThing argument of SetIsPushableByHero is built in its stack slot by the inline copy
//   (CBaseClass vtable 0x01231710, then CScriptThing's 0x01238C8C, PImp add-ref).
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQS_GuardianSisterInfo2_SisterInBanditCampScript {
public:
    class CMazeAtTavern;
};
class CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern : public CEntityScriptBase {
public:
    CQS_GuardianSisterInfo2_SisterInBanditCampScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long TalkCounter;                                   // +0x1c
    bool WavedOver;                                     // +0x20
    long HitCount;                                      // +0x24
private:
    virtual void Init();
};

void CQS_GuardianSisterInfo2_SisterInBanditCampScript::CMazeAtTavern::Init()
{
    TalkCounter = 0;
    WavedOver = false;
    HitCount = 0;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->SetIsPushableByHero(Me, false);
}
}