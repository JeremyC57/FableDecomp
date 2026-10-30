// CV_ChapelOfEvilScript::WatchForLeaving() [propagated label; the body is the Main of an entity script that follows
// WaspChaseWoman, beside CWaspChaseWoman::Main 0x00E10E60] -- register an alive filter over Me, take Me under scripted
// control (priority 4), follow WaspChaseWoman at distance 1 while she lives, then release the scripting
// Leaf: WatchForLeaving
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E10BF0, 384 bytes.
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840, CCharString(const char*, long)
//          @ 0x0099EBF0 / ~ @ 0x0099EAE0, the filter registration @ 0x00F35B10 and IsDone @ 0x00F35B30 (on this),
//          CBaseObject() @ 0x0099A380, the scripted-thing handle's IsValid @ 0x00CD23B9 / Release @ 0x00CD2770 /
//          FollowThing @ 0x007E7320 / destructor @ 0x007E74D0 (vtable 0x0127094C); CGameScriptInterface slots 7
//          NewScriptFrame (+0x1c), 8 StartScriptingEntity (+0x20), 72 GetThingWithScriptName (+0x120); CScriptThing
//          slot 75 IsAlive (+0x12c, through the reference).
// Layout: the entity-script base {vptr, Game +4, Me +8} as the landed CUndeadRisingZombieAttacker::Main 0x00E0CFF0;
//          the handle model as there (16 bytes).
// Shape: the woman is a const reference bound to the returned temporary (IsAlive dispatches through the vtable); the
//        follow wait is `while (!IsActiveThreadTerminating())` with the not-alive exit inside (a plain `while (alive)` wait rotates).
namespace NScript { class CPredicateBase; }
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptGameResourceObjectScriptedThingBase;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
struct CCountedPointerWords {                            // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                           // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                                  // 0x00CD23B9
    void Release();                                      // 0x00CD2770
    virtual void FollowThing(const CScriptThing& thing, float distance, bool run);   // 0x007E7320
};
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

struct CV_ChapelOfEvilScript : public CEntityScriptBase {
        void*                 ParentClass;                   // +0x14
    void*                 PMasterData;                   // +0x18
    void WatchForLeaving();
};

void CV_ChapelOfEvilScript::WatchForLeaving()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    const CScriptThing& woman = Game->GetThingWithScriptName("WaspChaseWoman");
    handle.FollowThing(woman, 1.0f, true);
    while (!IsActiveThreadTerminating()) {
        if (!woman.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            if (handle.IsValid())
                handle.Release();
            return;
        }
        Game->NewScriptFrame();
    }
}