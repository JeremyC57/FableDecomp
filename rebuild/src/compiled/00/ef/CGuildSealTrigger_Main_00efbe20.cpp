// CGuildSealTrigger::Main() -- guild seal trigger: register a conscious filter on Me, then each script frame wait until the hero is within 2 units and, when the hero carries OBJECT_GUILD_SEAL_1, switch this trigger's activation on for ten seconds and off again
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFBE20, length 337 bytes.
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 into the CIsThingConscious filter (vtable 0x012C2FE8, its
//   destructor @ 0x00EF7AB0 out of line), the filter registration @ 0x00F35B10, IsDone @ 0x00F35B30,
//   IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`), CCharString(const char*, long) @ 0x0099EBF0 /
//   ~CCharString @ 0x0099EAE0 ("OBJECT_GUILD_SEAL_1" @ 0x01251B4C); CScriptThing slot 69 SetActivationTriggerStatus
//   (0x004AB0A0); CGameScriptInterface (vtable 0x01260F0C) slots 7 NewScriptFrame, 70 GetHero, 184
//   IsObjectInThingsPossession, 376 Pause.
// Layout: the entity-script base {vptr, Game +4, CScriptThing Me +8} as the landed CUndeadRisingZombieAttacker::Main
//   0x00E0CFF0 (same filter class) and CHTG_AmbushPoint::Main 0x00D73610.
// Shape: the filter lives in its own `{}` block; the outer `while (!IsDone())` is rotated (its test repeated after the
//   bottom NewScriptFrame); the possession result survives the string temporary's destructor in bl.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
struct CIsThingConscious {                         // vtable 0x012C2FE8: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingConscious();                  // 0x00EF7AB0
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CGuildSealTrigger {
public:
    void*                 __vftable;               // +0x00
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    void RegisterFilter(CIsThingConscious& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void CGuildSealTrigger::Main()
{
    {
        CIsThingConscious filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDone()) {
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f)) {
            Game->NewScriptFrame();
            if (IsDone())
                return;
        }
        if (IsDone())
            return;
        if (Game->IsObjectInThingsPossession("OBJECT_GUILD_SEAL_1", Game->GetHero())) {
            if (IsDone())
                return;
            Me.SetActivationTriggerStatus(true);
            Game->Pause(10.0f);
            Me.SetActivationTriggerStatus(false);
        }
        Game->NewScriptFrame();
    }
}