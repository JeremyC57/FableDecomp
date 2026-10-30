// CMinionSpawner::Main() -- minion-spawner marker: read the trigger distance from Me's data string (10 when empty or zero), wait until the hero is that close, then play the evil summon effect at Me, pause 0.2s, create a war-dog minion ("MinionEscort") at Me facing the hero and remove the spawner
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA5980, length 446 bytes (+ int3 padding).
// Callees: GFCharStringToInt @ 0x0099E7F0 (`__fastcall`), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0
//   ("" 0x0122D70E, "SUMMON_CREATURE_SUMMON_EVIL" 0x012CBA0C, "MinionEscort" 0x012D7300, "CREATURE_MINION_WARDOG" 0x012CB9F4),
//   IsDone @ 0x00F35B30, IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`), ~CScriptThing @ 0x004AA840; CScriptThing slots 3
//   GetDataString (+0x0c), 6 GetPos (+0x18); CGameScriptInterface (vtable 0x01260F0C) slots 7 NewScriptFrame, 70 GetHero,
//   91 CreateCreature, 100 CreateEffect (+0x190), 108 RemoveThing, 376 Pause (+0x5e0), 475 EntitySetFacingAngleTowardsThing (+0x76c).
// Layout: the entity-script base {vptr, Game +4, Me +8} of the landed entity Mains.
// Shape: the distance is converted once to a float local (kept in ebx as its bit pattern for every loop push).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
class C3DVector { public: float x, y, z; };
class CCPPointerInfo;
class CScriptThing { public:                              // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                       // slot 0; complete destructor 0x004AA840
    virtual void v001() const; virtual void v002() const;
    virtual CCharString GetDataString() const;     // slot 3 (+0x0c)
    virtual void v004() const; virtual void v005() const;
    virtual const C3DVector& GetPos() const;       // slot 6 (+0x18)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CMinionSpawner : public CEntityScriptBase {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    void Main();
};

void CMinionSpawner::Main()
{
    long distance = GFCharStringToInt(Me.GetDataString());
    if (distance == 0) {
        if (IsActiveThreadTerminating())
            return;
        distance = 10;
    }
    float range = (float)distance;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), range)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->CreateEffect("SUMMON_CREATURE_SUMMON_EVIL", Me.GetPos(), "", 0.0f, false, false);
    Game->Pause(0.2f);
    CScriptThing minion = Game->CreateCreature("CREATURE_MINION_WARDOG", Me.GetPos(), "MinionEscort", true);
    Game->EntitySetFacingAngleTowardsThing(minion, Game->GetHero(), false);
    Game->RemoveThing(Me, false, true);
}