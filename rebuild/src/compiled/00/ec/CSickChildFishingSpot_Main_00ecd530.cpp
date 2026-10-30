// CSickChildFishingSpot::Main() -- sick-child quest fishing-spot entity: register an alive-filter over Me, start a frame, then wait frame by frame while the fishing spot stays enabled; once it is disabled (and the script is not done) set the parent's GotFishingSpotMushroom flag
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ECD530, length 145 bytes (slot of the entity vtable at 0x012F2424 references it).
// Callees: CScriptThing copy ctor @ 0x004ABE90, the filter registration @ 0x00F35B10, ~CIsThingAlive @ 0x00E241B0 (out of line in
// this module), IsDone @ 0x00F35B30; CGameScriptInterface slots 7 NewScriptFrame and 243 IsFishingSpotEnabled (0x008958F0).
// Layout = Ego_r.pdb CSickChildFishingSpot (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18;
// the parent's flag byte is at +0x69 in retail (PDB CV_SickChildScript::GotFishingSpotMushroom +0x7d).
// Shape: the landed CHHUndead::Main 0x00E95E80 prelude; the first IsDone test is explicit and the second is the rotated
// `while (!IsActiveThreadTerminating())` entry test; the disabled-spot exit re-tests IsDone before the flag store and returns.
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
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
    virtual ~CIsThingAlive();                              // 0x00E241B0 (out of line in this module)
};
}
using NScript::CIsThingAlive;
struct CV_SickChildScript {
    unsigned char _pad[0x69];
    bool          GotFishingSpotMushroom;          // +0x69 (PDB +0x7d; retail script base is 0x14 smaller)
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CSickChildFishingSpot : public CEntityScriptBase {
public:
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   Game;                  // +0x04
    CScriptThing            Me;                    // +0x08
    CV_SickChildScript*     ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    void Main();
};

void CSickChildFishingSpot::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        if (!Game->IsFishingSpotEnabled(Me)) {
            if (!IsActiveThreadTerminating())
                ParentClass->GotFishingSpotMushroom = true;
            return;
        }
        Game->NewScriptFrame();
    }
}