// NScript::CQ_HeroSoulsArenaScript::CSummonerToKill::Main() -- the arena summoner entity script: register an alive filter over Me, update once; unless the arena cut-scene has played, take Me under scripted control (priority 4) and wait frame by frame for the first wave to attack; then keep running frames until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7DD80, 253 bytes (+3 int3 padding), `ret`. The PDB has three CSummonerToKill
//   classes (Mother 0x00D83DC0 and Nostro 0x00D88540 are landed); this one's parent flags at retail +0x4C / +0x51 are
//   CQ_HeroSoulsArenaScript::PlayedArenaCS / FirstWaveAttack (Ego_r.pdb +0x60 / +0x65, the landed -0x14 shift), and the
//   body sits beside the Arena script's CArenaHeroGate::Main 0x00D7DF30.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90, ~CScriptThing 0x004AA840, the filter registration 0x00F35B10
//   and IsDone 0x00F35B30 (on `this`), CBaseObject::CBaseObject 0x0099A380, ~CScriptGameResourceObjectScriptedThingBase
//   0x007E74D0 (labelled ~CScriptGameResourceObjectMovieBase), CGameScriptInterface slots 7 NewScriptFrame (+0x1C) and
//   8 StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) (+0x20)
//   (ghidra_out/cgsi_slots_aligned.tsv). Vtables: CIsThingAlive 0x012C32F0 (inline destructor here: the vtable store
//   then ~CScriptThing), CScriptGameResourceObjectScriptedThingBase 0x0127094C.
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: the landed CSummonerToKill / CFishermanWasp prelude (block-scoped filter); the scripted-thing handle is
//   block-local in the `if` (it shares the filter's frame words) and its out-of-line destructor runs on the normal path
//   and on the merged early returns of the wait loop.
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    virtual ~CScriptThing();                               // 0x004AA840
};
class CScriptGameResourceObjectScriptedThingBase;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CIsThingAlive { public:                             // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
class CBaseObject { public:                               // CBaseIntelligentPointer root
    virtual void v00();
    void* m_pIntelligentPointer;                   // +0x04
protected:
    CBaseObject();                                 // 0x0099A380
public:
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00();
    virtual ~CScriptGameResourceObjectScriptedThingBase(); // 0x007E74D0
};
namespace NScript {
class CQ_HeroSoulsArenaScript { public:
    unsigned char _body[0x4c];
    bool          PlayedArenaCS;                   // +0x4C (PDB +0x60)
    bool          PlayerHitSummoner1;              // +0x4D
    bool          PlayerHitSummoner2;              // +0x4E
    bool          PlayerAttacksGuard;              // +0x4F
    bool          JackTauntNearHOH;                // +0x50
    bool          FirstWaveAttack;                 // +0x51 (PDB +0x65)
    class CSummonerToKill;
};
}
class NScript::CQ_HeroSoulsArenaScript::CSummonerToKill {
public:
    void*                             __vftable;   // +0x00
    CGameScriptInterface*             Game;        // +0x04
    CScriptThing                      Me;          // +0x08
    NScript::CQ_HeroSoulsArenaScript* ParentClass; // +0x14
    void*                             PMasterData; // +0x18
    void RegisterFilter(CIsThingAlive& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void NScript::CQ_HeroSoulsArenaScript::CSummonerToKill::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    if (!ParentClass->PlayedArenaCS) {
        if (IsDone())
            return;
        CScriptGameResourceObjectScriptedThingBase scripted;
        Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4);
        while (!ParentClass->FirstWaveAttack) {
            Game->NewScriptFrame();
            if (IsDone())
                return;
        }
        if (IsDone())
            return;
    }
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}