// CPreMeleeDummy::Main() -- the guild training dummy (nested entity script of CQ_GuildTrainingPreMeleeScript): untargetable until pre-melee mode 1, then wobble when the hero hits it (counting hits); in mode 2 spin when hit with the training stick, wobble on any other hero hit
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D51FC0, length 592 bytes, plain ret.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0; Me's CScriptThing vtable (0x01238C8C) slots 21 MsgIsHitBy (+0x54), 24 MsgIsHitByWithWeapon
// (+0x60); CGameScriptInterface slots 7 NewScriptFrame, 469 EntitySetTargetable, 487 EntityPlayObjectAnimation,
// 517 EntitySetAsKillable.
// Layout: Ego_r.pdb CPreMeleeDummy (0x1c): CEntityScriptBase vptr, Game +4, Me +8, ParentClass +0x14; the parent
// CQ_GuildTrainingPreMeleeScript is PDB - 0x14 (PreMeleeMode +0x4c, DummyHits +0x50).
// Shape: the calls on the member Me stay virtual; ebx is saved only after the first wait loop (retail shrink-wrap).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009(); virtual void s010(); virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015();
    virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;   // slot 21 (+0x54) 0x004AAB30
    virtual void s022(); virtual void s023();
    virtual bool MsgIsHitByWithWeapon(const CCharString& attacker, const CCharString& weapon) const;   // slot 24 (+0x60) 0x004AAB90
    void* PImp_Data;
    void* PImp_Info;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CQ_GuildTrainingPreMeleeScript {
    unsigned char _body[0x48];                         // CQuestScriptBase, Game +0x40, Data +0x44
    bool MazeArrived;                                  // +0x48
    bool HeroSleeps;                                   // +0x49
    bool WhisperCutsceneFinished;                      // +0x4a
    long PreMeleeMode;                                 // +0x4c
    long DummyHits;                                    // +0x50
};
class CEntityScriptBase { public:
    virtual ~CEntityScriptBase();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};
struct CPreMeleeDummy : CEntityScriptBase {
    CQ_GuildTrainingPreMeleeScript* ParentClass;       // +0x14
    void*                           PMasterData;       // +0x18
    void Main();
};

void CPreMeleeDummy::Main()
{
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetTargetable(Me, false);
    while (ParentClass->PreMeleeMode != 1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetTargetable(Me, true);
    while (ParentClass->PreMeleeMode == 1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            Game->EntityPlayObjectAnimation(Me, "WOBBLE", false);
            ++ParentClass->DummyHits;
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (ParentClass->PreMeleeMode == 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsHitByWithWeapon("SCRIPT_NAME_HERO", "OBJECT_HERO_STICK")) {
            if (IsActiveThreadTerminating()) return;
            ++ParentClass->DummyHits;
            Game->EntityPlayObjectAnimation(Me, "GET_HIT_SPIN", false);
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            Game->EntityPlayObjectAnimation(Me, "WOBBLE", false);
        }
    }
    if (IsActiveThreadTerminating()) return;
}