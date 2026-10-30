// CQ_RansomVictimScript::CQ_RansomVictimScript(CScriptDataBase*, CGameScriptInterface&) -- quest-script ctor: script base, interface/data pair, own vtable, the enemy-name string, the NPC comment timer and five empty scripted-thing handles
// Leaf: ??0CQ_RansomVictimScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE0320, length 182 bytes, `ret 8`; vtable 0x012DCE70; built by the factory
// 0x00DEBC60 (registered as "S_QRV", `push 0BCh` = Ego_r.pdb 0xd0 - 0x14).
// Callees: CScriptBase() 0x00CB8110, CCharString() 0x0099E4B0, CBaseObject() 0x0099A380 (each handle's root), and
// g_pGameScriptInterface (0x0143E8F8) RegisterTimer (slot 87) for the inline CTimer ctor.
// Layout (Ego_r.pdb - 0x14): ActiveEnemyName +0x64, NPCCommentTimer +0x68, seh_Guard +0x6c, seh_Father +0x7c, seh_Victim
// +0x8c, seh_Brother +0x9c, seh_Ransommer +0xac (CScriptGameResourceObjectScriptedThingBase, vtable 0x0127094C, 16 bytes).
// Shape: the two argument words come from the inline `__declspec(novtable)` intermediate base and are stored before the
// derived vtable; the handles' inline ctors run on `lea edi,[esi+N]` and zero their counted pointers from one ebx.
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
class CScriptDataBase;

class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers), vtable 0x012C1648
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    CScriptBase();                                       // 0x00CB8110
};

struct __declspec(novtable) CQuestScriptBase : CScriptBase {   // the argument pair stored before the derived vtable
    CGameScriptInterface* GSI;                           // +0x40
    CScriptDataBase*      Data;                          // +0x44
    CQuestScriptBase(CScriptDataBase* data, CGameScriptInterface* gsi) : CScriptBase(), GSI(gsi), Data(data) {}
};

extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

class CTimer { public:                                          // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) {}
};

namespace NScript {
class CQ_RansomVictimScript : public CQuestScriptBase {
public:        // vtable 0x012DCE70
    bool QuestStartScreened;                             // +0x48
    bool MissionSucceeded;                               // +0x49
    bool MissionFailed;                                  // +0x4a
    bool RescuedVictim;                                  // +0x4b
    bool RansommerDied;                                  // +0x4c
    bool RansomHelperStarted;                            // +0x4d
    bool RansomHelperSpoken;                             // +0x4e
    bool ExtraAssistanceGiven;                           // +0x4f
    bool RansomHelpersAttacked;                          // +0x50
    bool ToldGuardToWaitOutside;                         // +0x51
    bool EnteredNewRegion;                               // +0x52
    bool ThanksCutsceneStarting;                         // +0x53
    long HitChief;                                       // +0x54
    long HitEvilBrotherAndProtection;                    // +0x58
    bool SomeoneDealingWithBeingHit;                     // +0x5c
    long KidnappersKilled;                               // +0x60
    CCharString ActiveEnemyName;                         // +0x64
    CTimer NPCCommentTimer;                              // +0x68
    CScriptGameResourceObjectScriptedThingBase seh_Guard;      // +0x6c
    CScriptGameResourceObjectScriptedThingBase seh_Father;     // +0x7c
    CScriptGameResourceObjectScriptedThingBase seh_Victim;     // +0x8c
    CScriptGameResourceObjectScriptedThingBase seh_Brother;    // +0x9c
    CScriptGameResourceObjectScriptedThingBase seh_Ransommer;  // +0xac
    virtual void v00();
    CQ_RansomVictimScript(CScriptDataBase* data, CGameScriptInterface& gsi);
};
} // namespace NScript
using NScript::CQ_RansomVictimScript;

NScript::CQ_RansomVictimScript::CQ_RansomVictimScript(CScriptDataBase* data, CGameScriptInterface& gsi)
    : CQuestScriptBase(data, &gsi)
{
}