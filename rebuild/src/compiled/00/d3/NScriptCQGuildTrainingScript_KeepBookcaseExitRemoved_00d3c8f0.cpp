// CQ_GuildTrainingScript::KeepBookcaseExitRemoved() -- until the guild training reaches state 9, every time the hero leaves and re-enters HeroGuildComplex switch the region exit next to the secret bookcase off again
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D3C8F0, length 728 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line)
//   / ~CBaseClass 0x0099A2E0 / operator delete 0x00BFE9BC, CScriptBase::IsActiveThreadTerminating 0x00CB7940; through
//   the interface (+0x40): NewScriptFrame (7), IsLevelLoaded (11), GetThingWithScriptName (72), GetNearestWithDefName
//   (81), SetRegionExitAsActive (567).
// Layout: GameState +0x48 (retail = Ego_r.pdb CQ_GuildTrainingScript - 0x14).
// Shape: the CScriptThing model of the landed CV_ChapelOfEvilScript::WatchForWeaponUnsheathed 0x00E6A340 (inline
//   destructors expanded at the in-loop returns, the out-of-line ~CScriptThing on the shared tail); the "still loaded"
//   wait is written `!= false` (test/setne), the "not loaded" wait `!f()` (neg/sbb/inc).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_GuildTrainingScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
    long                  GameState;                        // +0x48
    void KeepBookcaseExitRemoved();
};
void CQ_GuildTrainingScript::KeepBookcaseExitRemoved()
{
    CScriptThing bookcase = Game->GetThingWithScriptName("SecretBookcase");
    CScriptThing exitPoint = Game->GetNearestWithDefName(bookcase, "REGION_EXIT_POINT");
    while (GameState != 9) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (Game->IsLevelLoaded("HeroGuildComplex") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("HeroGuildComplex")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (GameState != 9) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetRegionExitAsActive(exitPoint, false);
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}