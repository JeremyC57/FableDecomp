// CM_RestartMarker::Main() -- restart-marker entity script: alive filter on Me; take Me under scripted control (priority 4, retried each frame), look up "Marker1" / "Marker2", then until the script is done walk to Marker1 at a 1.5 max walking speed, reset the speed, walk to Marker2 and wait a frame
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF34A0, length 441 bytes (+ int3 padding).
// Callees: CScriptThing copy ctor @ 0x004ABE90 / ~CScriptThing @ 0x004AA840, CEntityScriptBase::SetPredicate @ 0x00F35B10,
//          ~CIsThingAlive @ 0x00E241B0, CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, CCharString(const char*, long)
//          @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CBaseObject::CBaseObject @ 0x0099A380, the scripted-thing handle's
//          IsValid @ 0x00CD23B9 / Release @ 0x00CD2770, MoveToThing @ 0x007E7300 (direct on the local) and its destructor
//          @ 0x007E74D0; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 72 GetThingWithScriptName,
//          489 EntitySetMaxWalkingSpeed (+0x7a4), 491 EntityResetMaxWalkingSpeed (+0x7ac). Literals "Marker1" @ 0x012F63AC,
//          "Marker2" @ 0x012F63A4.
// Layout / model: the landed CCham::Main 0x00EF3AA0 (same module, same handle and entity-script base).
// Shape: the handle is declared before the two markers (it is destroyed last); the walk loop is a rotated
//          `while (!IsActiveThreadTerminating())`; the 1.5f speed is pushed as its bit pattern.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};

enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CCountedPointer.h"
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

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
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                                  // 0x00E241B0 (out of line in this module)
};
}
using NScript::CIsThingAlive;

class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CM_RestartMarker : CEntityScriptBase {
    void Main();
};

void CM_RestartMarker::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing marker1 = Game->GetThingWithScriptName("Marker1");
    CScriptThing marker2 = Game->GetThingWithScriptName("Marker2");
    while (!IsActiveThreadTerminating()) {
        Game->EntitySetMaxWalkingSpeed(Me, 1.5f);
        scripted.MoveToThing(marker1, 0.0f, (EScriptEntityMoveType)0, true, true, false, true);
        Game->EntityResetMaxWalkingSpeed(Me);
        scripted.MoveToThing(marker2, 0.0f, (EScriptEntityMoveType)0, true, true, false, true);
        Game->NewScriptFrame();
    }
}