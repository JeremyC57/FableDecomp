// CHKC_ScreamerPoint::Main() -- a screamer spawn point: hold an alive filter over Me, wait until the quest arms the point, then until the point is switched off spawn a "CREATURE_SCREAMER_01" at Me whenever the hero comes within 20, set it on the hero and wait for it to die
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1B750, 512 bytes; entity-script vtable slot 1 (Main), plain `ret`.
// Callees: CScriptThing copy ctor @ 0x004ABE90 / ~CScriptThing @ 0x004AA840, CEntityScriptBase::SetPredicate @ 0x00F35B10,
//          CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, CBaseObject::CBaseObject @ 0x0099A380, the scripted-thing
//          handle destructor @ 0x007E74D0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0
//          ("CREATURE_SCREAMER_01" @ 0x012E1034, "SpawnedScreamer" @ 0x012E104C), IsDistanceBetweenThingsUnder @ 0x00CBE2FF
//          (__fastcall, distance on the stack); CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 91 CreateCreature
//          (by value), 451 GiveThingBestEnemyTarget; CScriptThing slots 6 GetPos and 75 IsAlive.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} and the scripted-thing handle (vtable 0x0127094C) as in the landed
//         CChasingScreamer::Main 0x00E1AF30 (same TU); this class adds the quest master data pointer +0x14 (armed flag +0x48,
//         stop flag +0x4f) and a dword +0x1c cleared after every spawn.
// Shape: the handle is declared (never used) at function scope after the first terminating test; the two creature-name
//        strings are implicit conversion temporaries built before the CreateCreature pushes (Me.GetPos() is evaluated
//        between the pushes); the spawned creature is a block-scoped by-value local.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };

class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030();
    virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046();
    virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070();
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CCountedPointer.h"
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

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
    virtual ~CIsThingAlive() { }
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

inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }

struct CHKCMasterData {
    unsigned char _pad00[0x48];
    bool          ScreamersArmed;                      // +0x48
    unsigned char _pad49[0x4f - 0x49];
    bool          ScreamersStopped;                    // +0x4f
};

struct CHKC_ScreamerPoint : CEntityScriptBase {
    CHKCMasterData* PMasterData;                       // +0x14
    unsigned char   _pad18[4];                         // +0x18
    long            SpawnTimer;                        // +0x1c
    void Main();
};

void CHKC_ScreamerPoint::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    while (!PMasterData->ScreamersArmed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!PMasterData->ScreamersStopped) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 20.0f)) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing screamer = Game->CreateCreature("CREATURE_SCREAMER_01", Me.GetPos(), "SpawnedScreamer", true);
            SpawnTimer = 0;
            Game->GiveThingBestEnemyTarget(screamer, Game->GetHero());
            while (reinterpret_cast<const CScriptThingView&>(screamer).Thing().IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    IsActiveThreadTerminating();
}