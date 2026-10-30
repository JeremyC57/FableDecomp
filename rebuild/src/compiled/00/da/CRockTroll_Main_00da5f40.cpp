// CRockTroll::Main() -- minion clifftop chase rock-troll script: attach the troll to the active quest, wait until the hero is within 20 units, then show a green quest-info health bar for it
// Leaf: Main
// Retail 0x00DA5F40, 337 bytes. Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (the
// CIsThingAlive filter, vtable 0x012C32F0, inline user dtor), RegisterFilter 0x00F35B10, IsDone 0x00F35B30,
// CBaseObject() 0x0099A380 + ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0 (an unused handle, vtable
// 0x0127094C), IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0 ("HUD_QUEST_ICON_ROCK_TROLL" 0x012D75D4); CGameScriptInterface slots 7 NewScriptFrame,
// 70 GetHero, 321 DisplayQuestInfo, 325 AddQuestInfoBarHealth, 465 EntityAttachToScript, 655 GetActiveQuestName.
// Layout: Ego_r.pdb CRockTroll (0x20): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
// PMasterData +0x18, HealthBarID +0x1C. Filter as the landed CHTE_Prisoner::Main 0x00D69F50, the scripted-thing
// handle as the landed quest-script ctor 0x00D2F6B0.
// Shape: the quest name is the nested call's temporary (the interface vtable cached in ebp across both calls); the
// icon is a named local in its OWN block (at function scope VC packs it into the quest-name temporary's slot); the
// green CRGBColour's ctor assigns R, G, B, A (stores grouped by value in that order).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    virtual ~CScriptThing();                               // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
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
class CIsThingAlive : public NScript::CPredicateBase { public:                             // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
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
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
extern const char g_rockTrollIcon[];               // 0x012D75D4 "HUD_QUEST_ICON_ROCK_TROLL"
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CRockTroll : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    void*                 ParentClass;             // +0x14
    void*                 PMasterData;             // +0x18
    long                  HealthBarID;             // +0x1c
    void Main();
};

void CRockTroll::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 20.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DisplayQuestInfo(true);
    {
        CCharString icon(g_rockTrollIcon, -1);
        HealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), icon, 1.0f);
    }
}