// CStatueMasterStatue::Main() -- the rotating statue entity script: every frame face the statue along the parent's time-of-day angle and, whenever the position it points at changes, retag its readable text (Bowerstone / Greatwood / nowhere)
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED41D0, length 505 bytes (padded to 512).
// Callees: CV_StatueMasterScript::GetStatueAngle @ 0x00ED43D0 (float; manifest "CGameCameraManager::HasCameraMode" is
//   propagated) and GetStatuePointingPosition @ 0x00ED4420; IsDone @ 0x00F35B30 (on `this`); `_CIfmod` @ 0x00BFEB28;
//   CCharString() @ 0x0099E4B0, CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0; CBaseClass::~CBaseClass
//   @ 0x0099A2E0 and operator delete @ 0x00BFE9BC (the inlined ~CScriptThing); the game interface's slots 7
//   NewScriptFrame, 72 GetThingWithScriptName (+0x120), 474 EntitySetFacingAngle (+0x768), 512
//   EntitySetCutsceneBehaviour (+0x800), 714 SetReadableObjectTextTag (+0xb28).
// Constants: 0.5f 0x0122F59C, 1.0 (double) 0x01237F00.
// Layout: the CEntityScriptBase family: Game +4, CScriptThing Me +8, ParentClass +0x14.
// Shape: the altar handle and an unused CCharString are function-scope locals (their destructors are expanded at the
//   loop exit and again at the in-loop `return`); the angle is a named float (stored, reloaded for `(double)` fmod);
//   the position switch lays out case 1, case 3 and the default in retail's order with the `push -1` hoisted. The loop is
//   `for (;;) { if (IsActiveThreadTerminating()) break; ... }` (a `while (!IsActiveThreadTerminating())` swaps eax/ecx in the two inlined ~CScriptThing
//   delete pushes) and the previous position is read before the call and overwritten before the compare.
#include <math.h>
#pragma intrinsic(fmod)
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
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
    float GetStatueAngle();                                 // 0x00ED43D0
    long GetStatuePointingPosition();                       // 0x00ED4420
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CStatueMasterStatue : public CEntityScriptBase {
    void*                  __vftable;                       // +0x00
    CGameScriptInterface*  Game;                            // +0x04
    CScriptThing           Me;                              // +0x08
    CV_StatueMasterScript* ParentClass;                     // +0x14
    void Main();
};

void CStatueMasterStatue::Main()
{
    Game->EntitySetCutsceneBehaviour(Me, (ECutsceneBehaviour)2);
    CScriptThing altar = Game->GetThingWithScriptName("StatueAltar");
    CCharString text;
    long lastPosition = 0;
    for (;;) {
        if (IsActiveThreadTerminating())
            break;
        float angle = ParentClass->GetStatueAngle() + 0.5f;
        Game->EntitySetFacingAngle(Me, (float)fmod((double)angle, 1.0), true);
        long previous = lastPosition;
        long position = ParentClass->GetStatuePointingPosition();
        lastPosition = position;
        if (position != previous) {
            if (IsActiveThreadTerminating())
                return;
            switch (position) {
            case 1:
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_061_STATUE_BOWERSTONE");
                break;
            case 3:
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_061_STATUE_GREATWOOD");
                break;
            default:
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_061_STATUE_NOWHERE");
                break;
            }
        }
        Game->NewScriptFrame();
    }
}