// CFleeingWoman::Main() -- fleeing-woman entity script
// Leaf: ?Main@CFleeingWoman@@
namespace NScript { class CPredicateBase; }
class C3DVector { public: float X, Y, Z; };
extern C3DVector g_ZeroVector;                         // 0x0143E8E0
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CScriptThingImp {
    virtual void i0(); virtual void i1(); virtual void i2(); virtual void i3(); virtual void i4(); virtual void i5();
    virtual const C3DVector& GetPosition() const;      // slot 6 (+0x18)
};
class CScriptThing { public:
    virtual void t000();
    CScriptThingImp* PImpData;
    void* PImpInfo;
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum EScriptEntityMoveType { SCRIPT_MOVE_DEFAULT };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    void* PImpData;
    void* PImpInfo;
    CScriptGameResourceObjectScriptedThingBase() { PImpData = 0; PImpInfo = 0; }
    virtual ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    virtual void s001();
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    void MoveToPosition(const C3DVector& pos, float speed, EScriptEntityMoveType type, bool a, bool b);   // 0x007E72F0
    bool IsPerformingScriptTask() const;               // 0x007E7450
};
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);   // 0x00CBE716
class CScriptDef { public:
    unsigned char _pad[0xe44];
    long  WomanFleeTriggerDistance;                    // +0xe44
    long  WomanFleeTalkDistance;                       // +0xe48
    float WomanFadeTime;                               // +0xe4c
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
class CPredicateBase
{
public:
    virtual void f00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;
    CScriptThing          Me;
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CFleeingWoman : CEntityScriptBase {
    unsigned char _pad14[8];
    bool Talked;                                       // +0x1c
    void Main();
};

void CFleeingWoman::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->WomanFleeTriggerDistance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing target = Game->GetThingWithScriptName("FleeingWomanEscapePos");
    if (!IsActiveThreadTerminating()) {
        for (;;) {
            if (!Talked && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->WomanFleeTalkDistance)) {
                if (IsActiveThreadTerminating())
                    return;
                Talked = true;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_072_WOMAN_FLEES_10", false, Me, Game->GetHero());
            }
            if (!scripted.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating())
                    return;
                scripted.MoveToPosition(!target.PImpData ? g_ZeroVector : target.PImpData->GetPosition(), 1.0f, (EScriptEntityMoveType)1, false, true);
            }
            if (IsDistanceBetweenThingsUnder(Me, target, 2.0f))
                break;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        FadeOutAndKillEntity(Me, true, pGlobalScriptDef->WomanFadeTime, true);
    }
}