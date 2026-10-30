// CStatueMasterCellarDoors::Main() -- the cellar-door entity script: unlocked when the statue points at position 1 and the escorted trader is no longer alive; otherwise locked, telling the hero it is locked whenever he uses it
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED45A0, length 459 bytes (padded to 464).
// Callees: CV_StatueMasterScript::GetStatuePointingPosition @ 0x00ED4420; IsDone @ 0x00F35B30 (on `this`, as the landed
//   CStatueMasterChest::Main 0x00ED4830); CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0;
//   CBaseClass::~CBaseClass @ 0x0099A2E0 and operator delete @ 0x00BFE9BC (the inlined ~CScriptThing); CScriptThing slots 20
//   MsgIsUsedBy (+0x50) and 75 IsAlive (+0x12c); the game interface's slots 7 NewScriptFrame (+0x1c), 40
//   MsgIsGameInfoClickedPast (+0xa0), 72 GetThingWithScriptName (+0x120), 115 DisplayGameInfo (+0x1cc), 518
//   EntitySetAsLocked (+0x818).
// Layout: Ego_r.pdb CStatueMasterCellarDoors (the CEntityScriptBase family): Game +4, CScriptThing Me +8, ParentClass +0x14.
// Shape: the first test is a named bool over `pos == 1 && !GetThingWithScriptName("TraderToEscort").IsAlive()`: the
//   string and the returned CScriptThing are flagged conditional temporaries (ebx bits 1/2) whose destructors run before
//   the bool is tested; IsAlive stays a virtual call on the temporary.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SScriptThingInfo {                                   // the counted block behind CScriptThing::PImp_Info
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};
class CScriptThing : public CBaseClass { public:                               // vtable 0x01238C8C, 12 bytes
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19();
    virtual bool MsgIsUsedBy(const CCharString& user) const;   // slot 20 (+0x50)
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                           // slot 75 (+0x12c)
    void* PImp_Data;                                        // +0x04
    SScriptThingInfo* PImp_Info;                            // +0x08
    CScriptThing(const CScriptThing& other);
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CV_StatueMasterScript {
    long GetStatuePointingPosition();                       // 0x00ED4420
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CStatueMasterCellarDoors : public CEntityScriptBase {
    void*                  __vftable;                       // +0x00
    CGameScriptInterface*  Game;                            // +0x04
    CScriptThing           Me;                              // +0x08
    CV_StatueMasterScript* ParentClass;                     // +0x14
    void Main();
};

void CStatueMasterCellarDoors::Main()
{
    bool unlocked = ParentClass->GetStatuePointingPosition() == 1
                 && !Game->GetThingWithScriptName("TraderToEscort").IsAlive();
    if (unlocked) {
        if (IsActiveThreadTerminating())
            return;
        Game->EntitySetAsLocked(Me, false);
        return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsLocked(Me, true);
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_061_CELLAR_LOCKED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}