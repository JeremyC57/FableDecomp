// CArenaSpawn::Main() -- arena spawn point: its data string is the spawn-slot index; while the script runs, wait for the parent script to flag that slot, play the SUMMON_ARENA effect at the spawn (unless the arena is in state 3), clear the flag and wait a frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F1B690, length 361 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract).
// Callees: CScriptThing slot 3 GetDataString (CCharString by hidden pointer) and slot 6 GetPos, GFCharStringToInt
//          @ 0x0099E7F0 (__fastcall), ~CCharString @ 0x0099EAE0, CCharString(const char*, long = -1) @ 0x0099EBF0,
//          ~CScriptThing @ 0x004AA840, CEntityScriptBase::IsDone @ 0x00F35B30, the game interface's slots 7 NewScriptFrame,
//          87 RegisterTimer, 88 DeregisterTimer, 100 CreateEffect (cgsi_slots_aligned.tsv); data g_pGameScriptInterface
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
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };

class CCPPointerInfo;                                   // CCountedPointer's shared block
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
    virtual void v01() const; virtual void v02() const;
    virtual CCharString GetDataString() const;           // slot 3 (+0x0c)
    virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
};
long __fastcall GFCharStringToInt(const CCharString& text);                                                   // 0x0099E7F0
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

struct CQ_ArenaScript {
    unsigned char _pad[0xa8];
    long ArenaState;                                     // +0xa8
    unsigned char _pad2[0xe2 - 0xac];
    bool SpawnFlags[1];                                  // +0xe2 [slot]
};

struct CArenaSpawn {
    virtual ~CArenaSpawn();
    virtual void Main();                                 // slot 1
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    CQ_ArenaScript* ParentClass;                         // +0x14
    bool IsDone();                                       // 0x00F35B30
};

void CArenaSpawn::Main()
{
    long slot = GFCharStringToInt(Me.GetDataString());
    long timer = g_pGameScriptInterface->RegisterTimer();
    while (!IsDone()) {
        while (!ParentClass->SpawnFlags[slot]) {
            Game->NewScriptFrame();
            if (IsDone()) goto stop;
        }
        if (IsDone()) {
            g_pGameScriptInterface->DeregisterTimer(timer);
            return;
        }
        if (ParentClass->ArenaState != 3) {
            if (IsDone()) goto stop;
            CScriptThing effect = Game->CreateEffect("SUMMON_ARENA", Me.GetPos(), "", 0.0f, false, false);
        }
        ParentClass->SpawnFlags[slot] = false;
        Game->NewScriptFrame();
    }
    g_pGameScriptInterface->DeregisterTimer(timer);
    return;
stop:
    g_pGameScriptInterface->DeregisterTimer(timer);
}