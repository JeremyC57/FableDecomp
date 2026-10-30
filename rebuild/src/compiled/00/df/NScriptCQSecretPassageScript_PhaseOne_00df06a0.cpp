// CQ_SecretPassageScript::PhaseOne() -- run the mother's torture sequence, then wait (one script frame at a time) until the mother has been found
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF06A0, length 162 bytes; called from DoMission (0x00DEFF60).
// Callees: CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430 and the out-of-line ~CScriptGameResourceObjectScriptedThingBase
// 0x007E74D0 (the handle local), operator delete 0x00BFE9BC, MotherTortureThread 0x00DF0750 (manifest label),
// CScriptBase::IsActiveThreadTerminating 0x00CB7940, NewScriptFrame (slot 7) through the interface (+0x40).
// Layout: MotherFound +0x4b (Ego_r.pdb +0x5f - 0x14).
// Shape: an unused scripted-thing handle local (inline ctor: CBaseObject call, vtable 0x0127094C, zeroed counted pointer
// from ebx); the early return in the wait loop gets the inlined destructor, the common exit calls it out of line; the
// final thread test's result is dropped (its `return` merges with the fall-through).
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

class CScriptThing { public:                                    // 12 bytes, vtable 0x01238C8C
    unsigned char _body[0xc];
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    ~CScriptThing();                                     // 0x004AA840
};

struct CIsThingConscious {                               // NScript predicate, vtable 0x012C2FE8: Thing alive and not unconscious
    CScriptThing Thing;                                  // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) {}
    ~CIsThingConscious() {}
    virtual bool operator()() const;                     // slot 0 (0x00CDDAB0)
};

class CBaseObject { public:                                     // vtable 0x0129A7D8 {vptr, intelligent-pointer link}
    void* IntelligentPointer;                            // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};

class CScriptGameResourceObjectBase : public CBaseObject { public:     // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};

class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // 16 bytes, vtable 0x0127094C
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    virtual void v01();                                  // slot 1 (0x007E72C0)
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

namespace NScript {
class CQ_SecretPassageScript : public CQuestScriptBase {
public:
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool MissionOver;                                    // +0x4a
    bool MotherFound;                                    // +0x4b
    void MotherTortureThread();                          // 0x00DF0750
    void PhaseOne();
};
} // namespace NScript
using NScript::CQ_SecretPassageScript;

void NScript::CQ_SecretPassageScript::PhaseOne()
{
    CScriptGameResourceObjectScriptedThingBase mother;
    MotherTortureThread();
    while (!MotherFound) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}