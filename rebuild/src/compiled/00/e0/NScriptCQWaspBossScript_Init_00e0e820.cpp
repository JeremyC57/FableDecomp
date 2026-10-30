// CQ_WaspBossScript::Init() -- register the Wasp Boss quest region and map offset, clear the quest state and reset the wasp-helper timer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0E820, length 192 bytes; vtable 0x012E00E4 slot 3 (factory 0x00E0E9F0,
// `push 5Ch` = Ego_r.pdb 0x70 - 0x14).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the quest's interface (+0x40) AddQuestRegion
// (slot 271) and SetQuestWorldMapOffset (slot 272); g_pGameScriptInterface (0x0143E8F8) SetTimer (slot 89) via CTimer.
// Layout (Ego_r.pdb - 0x14): the eight flags +0x48..+0x4f, SavedVillagerCount +0x50, QuestStartScreened +0x54,
// ReachedWaspHelper (CTimer) +0x58.
// Shape: the quest-name temporary is built before the C2DCoordI(10, 10) temporary (an implicit conversion goes first),
// whose two words are stored after its address is pushed; the clears share one zero register in source order.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing() {}                                    // the null thing
    ~CScriptThing() {}                                   // out of line 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);  // 0x008AB980
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const;
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48) 0x004AAAF0
    virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const; virtual void v25() const; virtual void v26() const;
    virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const;
    virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const;
    virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const; virtual void v49() const; virtual void v50() const;
    virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const;
    virtual void v59() const; virtual void v60() const;
    virtual bool IsUnconscious() const;                 // slot 61 (+0xf4) 0x004AAFA0
    virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const;
    virtual void v70() const; virtual void v71() const; virtual void v72() const; virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
class C2DCoordI { public:                                       // integer 2D coordinate
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

class CTimer { public:                                          // 4 bytes: an index into the global script timers
    long TimerIndex;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};

namespace NScript {
class CQ_WaspBossScript : public CQuestScriptBase {
public:
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool DronesCreated;                                  // +0x4a
    bool QueenHornetAttacks;                             // +0x4b
    bool CutsceneFinished;                               // +0x4c
    bool SaidLineOnce;                                   // +0x4d
    bool PanickedVillagersScene;                         // +0x4e
    bool StartChase;                                     // +0x4f
    long SavedVillagerCount;                             // +0x50
    bool QuestStartScreened;                             // +0x54
    CTimer ReachedWaspHelper;                            // +0x58
    void Init();
};
} // namespace NScript
using NScript::CQ_WaspBossScript;

void NScript::CQ_WaspBossScript::Init()
{
    GSI->AddQuestRegion("Q_WaspBoss", "PicnicArea");
    GSI->SetQuestWorldMapOffset("Q_WaspBoss", C2DCoordI(10, 10));
    PanickedVillagersScene = false;
    MissionSucceeded = false;
    MissionFailed = false;
    DronesCreated = false;
    SaidLineOnce = false;
    QueenHornetAttacks = false;
    CutsceneFinished = false;
    StartChase = false;
    QuestStartScreened = false;
    SavedVillagerCount = 0;
    ReachedWaspHelper.SetTime(0);
}