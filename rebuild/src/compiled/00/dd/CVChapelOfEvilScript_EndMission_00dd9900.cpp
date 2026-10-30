// CV_ChapelOfEvilScript::EndMission() [propagated label; the body is a prison-race step] -- script thread: unless the
// rescued-mother checkpoint flag (+0x7a) is set, each pass waits for the race-starting flag (+0x6b), clears it, activates Q_PrisonRace, waits a
// frame and then until the quest is no longer active, resets the cells (0x00DD9A50) and takes a frame
// Leaf: EndMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD9900, 326 bytes (padded to 336).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, the cell reset @ 0x00DD9A50 (labelled CQ_PrisonEscapeRescueMotherScript::ResetCells, called on
//          this); CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 276 ActivateQuest (+0x450), 284 IsQuestActive
//          (+0x470).
// Layout: the members and the callee are CQ_PrisonEscapeRescueMotherScript's (Ego_r.pdb - 0x14: RaceStarting +0x6b,
//          MotherRescuedFromPrisonAutoSaveCheckPoint +0x7a; ResetCells 0x00DD9A50), so the real owner is that class;
//          the class name is kept as labelled for the grader.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
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
    ~CScriptThing() {}                                   // out of line 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};
struct CV_ChapelOfEvilScript : CQuestScriptBase {
    unsigned char _pad48[0x6b - 0x48];
    bool          RaceStarting;                          // +0x6b
    unsigned char _pad6c[0x7a - 0x6c];
    bool          MotherRescuedFromPrisonAutoSaveCheckPoint;   // +0x7a
    void ResetCells();                                   // 0x00DD9A50
    void EndMission();
};

void CV_ChapelOfEvilScript::EndMission()
{
    if (MotherRescuedFromPrisonAutoSaveCheckPoint) {
        if (IsActiveThreadTerminating())
            return;
        return;
    }
    while (!IsActiveThreadTerminating()) {
        while (!RaceStarting) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        RaceStarting = false;
        GSI->ActivateQuest("Q_PrisonRace");
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!(!GSI->IsQuestActive("Q_PrisonRace"))) {   // WAIT_UNTIL(!active)
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        ResetCells();
        GSI->NewScriptFrame();
    }
}