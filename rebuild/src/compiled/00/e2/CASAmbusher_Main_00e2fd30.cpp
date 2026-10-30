// CAS_Ambusher::Main() -- ambush scam ambusher: ally with the trickster (if it lives), target the hero, wait until the hero is within the ambush distance, play a random ambush sound on itself, then idle until the script is done
// Leaf: ?Main@CAS_Ambusher@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2FD30, length 360 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract; the landed CAS_Ambusher::Init is 0x00E2BC10).
// Callees: CV_AmbushScamScript::GetTrickster @ 0x00E2EC30 (CScriptThing by value through the hidden pointer, on ParentClass),
//          CScriptThing::IsAlive inline (slot 75 on the game-side thing), CEntityScriptBase::IsDone @ 0x00F35B30 (manifest
//          "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated), the game interface's slots 599
//          EntitySetThingAsAllyOfThing, 70 GetHero, 451 GiveThingBestEnemyTarget, 7 NewScriptFrame, 690 PlaySoundOnThing
//          (cgsi_slots_aligned.tsv), IsDistanceBetweenThingsUnder @ 0x00CBE2FF (__fastcall: the two things in ecx/edx, the
//          float on the stack), rand @ 0x00BFEB16 (MSVCR71), CDefString::operator CCharString @ 0x00415D70, ~CCharString
//          @ 0x0099EAE0, ~CScriptThing @ 0x004AA840; data: pGlobalScriptDef @ 0x0143E90C (sound-name vector +0x2c0/+0x2c4,
//          ambush distance +0x2cc).
// Shape: the trickster is a frame local destroyed at both exits; the early `if (IsDone()) return;` sits before the rest of
//          the body, which lives in a __forceinline helper so VC7.1 pushes/pops ebx (the address of Me) inside that region
//          only; the two wait loops are rotated whiles (`while (!near) { frame; if done return; }`, then
//          `do frame while (!done)`); the sound index is `rand() % size()` with the vector size as a pointer difference.
// Layout: Ego_r.pdb CAS_Ambusher (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//          PMasterData +0x18. CScriptThing per the landed script model (CBaseClass + CCountedPointer, 12 bytes).
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
class CDefString { public:                                      // 4 bytes: a def-side string
    void* PStringData;
    operator CCharString() const;                        // 0x00415D70
};
extern "C" int __cdecl rand();                           // 0x00BFEB16 (MSVCR71)

class CCPPointerInfo;                                   // CCountedPointer's shared block
#include "engine/CCountedPointer.h"
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
    unsigned char _pad[0x2c0];
    CDefString* AS_AmbushSoundsBegin;                    // +0x2c0 vector<CDefString> (retail STLport, 12 bytes)
    CDefString* AS_AmbushSoundsEnd;                      // +0x2c4
    CDefString* AS_AmbushSoundsEndOfStorage;             // +0x2c8
    float       AS_AmbushDistance;                       // +0x2cc
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CV_AmbushScamScript {
    CScriptThing GetTrickster();                         // 0x00E2EC30
};
struct CQ_SunnyvaleMasterData;

struct CAS_Ambusher {
    virtual ~CAS_Ambusher();
    virtual void Main();                                 // slot 1
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    CV_AmbushScamScript* ParentClass;                    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                 // +0x18
    bool IsDone();                                       // 0x00F35B30
    __forceinline void RunAmbush();
};

// Target the hero, wait for it to come near, shout, then idle until the script ends.
__forceinline void CAS_Ambusher::RunAmbush()
{
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), pGlobalScriptDef->AS_AmbushDistance)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    unsigned long count = pGlobalScriptDef->AS_AmbushSoundsEnd - pGlobalScriptDef->AS_AmbushSoundsBegin;
    unsigned long index = rand() % count;
    {
        const CCharString& sound = pGlobalScriptDef->AS_AmbushSoundsBegin[index];
        Game->PlaySoundOnThing(Me, sound);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}

void CAS_Ambusher::Main()
{
    CScriptThing trickster = ParentClass->GetTrickster();
    if (trickster.IsAlive()) {
        if (IsDone())
            return;
        Game->EntitySetThingAsAllyOfThing(Me, trickster);
        Game->EntitySetThingAsAllyOfThing(trickster, Me);
    }
    RunAmbush();
}