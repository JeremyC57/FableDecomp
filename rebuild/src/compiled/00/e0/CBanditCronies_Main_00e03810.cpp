// CBanditCronies::Main() -- bandit crony: register itself with the parent script, wait a frame, then wait until the hero is within 13m, shout a random crony sound, target the hero and idle until the script is done
// Leaf: ?Main@CBanditCronies@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E03810, length 339 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract).
// Callees: CScriptThing copy ctor @ 0x004ABE90 and ~CScriptThing @ 0x004AA840 (the CIsThingAlive filter's member; the
//          filter vtable 0x012C32F0 is stored inline), CEntityScriptBase::SetPredicate @ 0x00F35B10 (manifest
//          "CWorldMap::AllocateMapThingUID" is propagated; landed CHTEPrisoner_Main / CSUMMONEDCREATURE_Main model),
//          CEntityScriptBase::IsDone @ 0x00F35B30, the game interface's slots 7 NewScriptFrame, 70 GetHero,
//          451 GiveThingBestEnemyTarget, 690 PlaySoundOnThing (cgsi_slots_aligned.tsv), IsDistanceBetweenThingsUnder
//          @ 0x00CBE2FF (__fastcall), rand @ 0x00BFEB16, CDefString::operator CCharString @ 0x00415D70, CCharString copy
//          ctor @ 0x0099EC30, ~CCharString @ 0x0099EAE0; data: pGlobalScriptDef @ 0x0143E90C (sound-name vector +0x3ac/+0x3b0).
// Shape: the alive filter is a scoped 16-byte frame object (landed CHTEPrisoner::Main model); `NewScriptFrame; if (IsDone())
//          return;` precedes the rotated `while (!near) { frame; if done return; }`; the sound is a CCharString LOCAL (operator
//          CCharString temp + copy ctor), destroyed after the final `do frame while (!done)` loop; ebx = &Me for the whole body.
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; CScriptThing per the landed script model (12 bytes).
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
class CDefString { public:                                      // 4 bytes: a def-side string
    void* PStringData;
    operator CCharString() const;                        // 0x00415D70
};
extern "C" int __cdecl rand();                           // 0x00BFEB16 (MSVCR71)

class CCPPointerInfo;                                   // CCountedPointer's shared block
template <class T> class CCountedPointer { public:               // destroyed by the out-of-line ~CScriptThing here
    T*              Data;
    CCPPointerInfo* Info;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0x3ac];
    CDefString* BC_ShoutSoundsBegin;                     // +0x3ac vector<CDefString> (retail STLport, 12 bytes)
    CDefString* BC_ShoutSoundsEnd;                       // +0x3b0
    CDefString* BC_ShoutSoundsEndOfStorage;              // +0x3b4
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
using NScript::CIsThingAlive;

struct CBanditCronies {
    virtual ~CBanditCronies();
    virtual void Main();                                 // slot 1
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    void RegisterFilter(CIsThingAlive& filter);          // 0x00F35B10 (CEntityScriptBase::SetPredicate)
    bool IsDone();                                       // 0x00F35B30
};

void CBanditCronies::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 13.0f)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    unsigned long count = pGlobalScriptDef->BC_ShoutSoundsEnd - pGlobalScriptDef->BC_ShoutSoundsBegin;
    unsigned long index = rand() % count;
    CCharString sound = pGlobalScriptDef->BC_ShoutSoundsBegin[index];
    Game->PlaySoundOnThing(Me, sound);
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}