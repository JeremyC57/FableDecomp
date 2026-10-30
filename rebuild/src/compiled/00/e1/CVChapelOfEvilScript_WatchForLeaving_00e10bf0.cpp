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
class CScriptGameResourceObjectScriptedThingBase;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                   // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
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
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CV_ChapelOfEvilScript : public CEntityScriptBase {
    void*                 __vftable;                     // +0x00
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
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