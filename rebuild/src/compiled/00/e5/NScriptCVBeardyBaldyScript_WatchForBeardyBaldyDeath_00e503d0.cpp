// CV_BeardyBaldyScript::WatchForBeardyBaldyDeath() -- look up BB_BeardyBaldyMan, wait until he has been killed (by anyone), then (unless the thread is terminating) record BeardyBaldyKilled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E503D0, length 232 bytes (240 with padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CBaseClass::~CBaseClass
// @ 0x0099A2E0, CScriptThing::~CScriptThing @ 0x004AA840 (the out-of-line copy), operator delete @ 0x00BFE9BC,
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed); through the interface (+0x40)
// GetThingWithScriptName (slot 72, +0x120, CScriptThing by hidden pointer) and NewScriptFrame (slot 7, +0x1c); the
// script thing's implementation slot 18 (+0x48, MsgIsKilledBy(const CCharString&)) behind the inlined
// CScriptThing::MsgIsKilledBy (0x004AAAF0: null PImp -> false). Literal "" @ 0x0122D70E.
// Layout (retail = Ego_r.pdb CV_BeardyBaldyScript - 0x14): BeardyBaldyKilled +0x68.
// Shape: both strings are IMPLICIT CCharString conversions (built before the interface/PImp are read); the handle has
// THREE destructor sites (the terminating return inside the wait loop, a second `if (IsActiveThreadTerminating()) return;`
// after it, and the end): VC7.1 expands the inline destructor at the first site only and calls the out-of-line
// ~CScriptThing (0x004AA840) at the other two, which tail-merge (the two-site `if (!...)` form expands both).
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
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer);  // slot 18 (+0x48)
};
class CScriptThing : public CBaseClass { public:                               // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);
    ~CScriptThing() {}                                      // out of line 0x004AA840
    bool MsgIsKilledBy(const CCharString& killer)           // 0x004AAAF0
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

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

namespace NScript {
class CV_BeardyBaldyScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x20];                             // +0x48 quest flags, phase, required hair strings
    bool BeardyBaldyKilled;                                 // +0x68
    void WatchForBeardyBaldyDeath();
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;

void NScript::CV_BeardyBaldyScript::WatchForBeardyBaldyDeath()
{
    CScriptThing beardy = GSI->GetThingWithScriptName("BB_BeardyBaldyMan");
    while (!beardy.MsgIsKilledBy("")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    BeardyBaldyKilled = true;
}