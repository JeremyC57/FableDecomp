// CV_TourGuideScript::WatchForGuideKilled() -- look up the TourGuideGuide, wait until he is alive (spawned), then wait until he has been killed (by anyone) and (unless the thread is terminating) record TourGuideKilled
// Leaf: ?WatchForGuideKilled@CV_TourGuideScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE4A70, 313 bytes (320 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC (MSVCR71), ~CScriptThing 0x004AA840 (the out-of-line copy), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; through the interface (+0x40) GetThingWithScriptName (slot 72, +0x120, CScriptThing by hidden pointer)
//   and NewScriptFrame (slot 7, +0x1c); the script thing's implementation slots 75 IsAlive (+0x12c) and 18
//   MsgIsKilledBy(const CCharString&) (+0x48) behind the inlined null-safe CScriptThing wrappers.
// Strings: "TourGuideGuide" 0x012F510C, "" 0x0122D70E.
// Shape: the landed CV_BeardyBaldyScript::WatchForBeardyBaldyDeath (0x00E503D0) pattern with two wait loops: both
//   strings are IMPLICIT conversions; the handle has four destructor sites (the terminating return inside the first
//   loop, the macro test after it, the return inside the second loop and the end) -- VC7.1 expands the inline
//   destructor at the first only and calls the out-of-line ~CScriptThing at the others (the last two tail-merge).
// Layout: TourGuideKilled +0x120 (landed WatchForClosingTime: TourFinished +0x123, PDB - 0x14).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data (CScriptThing vtable layout)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;    // slot 18 (+0x48)
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72();
    virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                                   // slot 75 (+0x12c)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);
    ~CScriptThing() {}                                      // out of line 0x004AA840
    bool IsAlive() const                                    // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
    bool MsgIsKilledBy(const CCharString& killer) const     // 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_TourGuideScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x120 - 0x48];                     // WaypointInfo +0x48 ..
    bool          TourGuideKilled;                          // +0x120
    void WatchForGuideKilled();
};
} // namespace NScript
using NScript::CV_TourGuideScript;
void NScript::CV_TourGuideScript::WatchForGuideKilled()
{
    CScriptThing guide = Game->GetThingWithScriptName("TourGuideGuide");
    while (!guide.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!guide.MsgIsKilledBy("")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    TourGuideKilled = true;
}