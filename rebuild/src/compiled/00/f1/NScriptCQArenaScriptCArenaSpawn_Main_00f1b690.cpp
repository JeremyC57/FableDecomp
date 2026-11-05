// NScript::CQ_ArenaScript::CArenaSpawn::Main() -- arena spawn point: its data string is the spawn-slot index; while the script runs, wait for the parent script to flag that slot, play the SUMMON_ARENA effect at the spawn (unless the arena is in state 3), clear the flag and wait a frame
// Leaf: ?Main@CArenaSpawn@CQ_ArenaScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F1B690, length 361 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract).
// Callees: CScriptThing slot 3 GetDataString (CCharString by hidden pointer) and slot 6 GetPos, GFCharStringToInt
//          @ 0x0099E7F0 (__fastcall), ~CCharString @ 0x0099EAE0, CCharString(const char*, long = -1) @ 0x0099EBF0,
//          ~CScriptThing @ 0x004AA840, CEntityScriptBase::IsDone @ 0x00F35B30, the game interface's slots 7 NewScriptFrame,
//          87 RegisterTimer, 88 DeregisterTimer, 100 CreateEffect (cgsi_slots_aligned.tsv); data pDSTGame
//          @ 0x0143E8F8; literals "SUMMON_ARENA" 0x012F9D00 and "" 0x0122D70E.
// Shape: the timer handle is registered on the global interface; the inner wait loop's IsDone exit and the ArenaState
//          branch's IsDone exit both `goto stop` (one shared DeregisterTimer tail, retail copy 2 with eax), while the
//          after-wait IsDone check deregisters and returns inline and is tail-merged with the loop fallthrough (copy 1,
//          edx). Explicit returns at all three sites (or break at the middle one) put the inner exit into copy 1 or swap
//          the copies' registers; this goto/return mix is the only one of 36 exit-form combinations that matches. The slot
//          index stays in ebp, the timer in ebx with a frame spill across CreateEffect (which loads its vtable into ebx);
//          the CScriptThing result is a named local (destroyed after the two string temporaries).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14; the parent script's arena state +0xa8
//          and per-slot spawn flags (bytes) at +0xe2 (descriptive names). engine/CScriptThing.h and
//          engine/CGameScriptInterface.h flatten the vptr into a data member, but this row calls their virtual slots
//          (GetDataString/GetPos, NewScriptFrame/RegisterTimer/DeregisterTimer/CreateEffect), so polymorphic local
//          declarations are used.
// Naming wave 35: identity reg 00f1005d "ArenaSpawn" in 00f0fb70; Alloc 00f1b5b0; vtable 012f9ce4 slot 1; PDB ?Main@CArenaSpawn@CQ_ArenaScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CArenaSpawn::Main (wave3/labels_alloc.tsv); PDB ?Main@CArenaSpawn@CQ_ArenaScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CArenaSpawn@@UAEXXZ`; this source now defines the PDB member (outer stand-in CQ_ArenaScript moved into NScript); same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
#include "engine/C3DVector.h"

#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
long __fastcall GFCharStringToInt(const CCharString& text);                                                   // 0x0099E7F0
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

namespace NScript {
class CQ_ArenaScript {
public:
    unsigned char _pad[0xa8];
    long ArenaState;                                     // +0xa8
    unsigned char _pad2[0xe2 - 0xac];
    bool SpawnFlags[1];                                  // +0xe2 [slot]
public:
    class CArenaSpawn;               // the entity class below (naming wave 35)
};
}
using NScript::CQ_ArenaScript;

class CEntityScriptBase { public:   // the PDB class the accessor belongs to
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
};
namespace NScript {
class CQ_ArenaScript::CArenaSpawn {
public:
    virtual ~CArenaSpawn();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    CQ_ArenaScript* ParentClass;                         // +0x14
private:
    virtual void Main();                          // PDB ?Main@CArenaSpawn@CQ_ArenaScript@NScript@@EAEXXZ
};
}

void NScript::CQ_ArenaScript::CArenaSpawn::Main()
{
    long slot = GFCharStringToInt(Me.GetDataString());
    long timer = pDSTGame->RegisterTimer();
    while (!((const CEntityScriptBase*)this)->IsActiveThreadTerminating()) {
        while (!ParentClass->SpawnFlags[slot]) {
            Game->NewScriptFrame();
            if (((const CEntityScriptBase*)this)->IsActiveThreadTerminating()) goto stop;
        }
        if (((const CEntityScriptBase*)this)->IsActiveThreadTerminating()) {
            pDSTGame->DeregisterTimer(timer);
            return;
        }
        if (ParentClass->ArenaState != 3) {
            if (((const CEntityScriptBase*)this)->IsActiveThreadTerminating()) goto stop;
            CScriptThing effect = Game->CreateEffect("SUMMON_ARENA", Me.GetPos(), "", 0.0f, false, false);
        }
        ParentClass->SpawnFlags[slot] = false;
        Game->NewScriptFrame();
    }
    pDSTGame->DeregisterTimer(timer);
    return;
stop:
    pDSTGame->DeregisterTimer(timer);
}