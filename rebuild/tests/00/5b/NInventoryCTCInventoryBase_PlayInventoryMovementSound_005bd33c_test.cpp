// behaviour test for 005bd33c: CTCInventoryBase::PlayInventoryMovementSound -- game-state guard, CSoundInit flag 0x20, def add-ref/release
#include <cstdio>
class CASoundBank { public: int id; };
class CSoundInit { public:
    float Volume, Pitch; int Delay, LoopCount, Priority, Flags, Pan, Id;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Delay(0), LoopCount(1), Priority(1), Flags(0), Pan(0), Id(-1) {}
};
struct CSoundPlayer {
    int calls; CASoundBank* bank; int sample; bool loop; int flags; bool attached; int refDuringCall;
    CSoundPlayer() : calls(0), bank(0), sample(0), loop(false), flags(0), attached(true), refDuringCall(0) {}
};
class CInventoryDef { public:
    int RefCount; int destroyed; int MovementSound; int NoMovementSound;
    void Destroy() const { ++((CInventoryDef*)this)->destroyed; }
};
static CSoundPlayer g_player;
static CInventoryDef* g_seenDef = 0;
static void PlaySample(CSoundPlayer* p, CASoundBank& bank, int sample, bool loop, const CSoundInit& init, bool attached)
{
    ++p->calls; p->bank = &bank; p->sample = sample; p->loop = loop; p->flags = init.Flags; p->attached = attached;
    p->refDuringCall = g_seenDef ? g_seenDef->RefCount : -1;
}
struct CGameState { CSoundPlayer* GetSoundPlayer() { return &g_player; } };
static CGameState* g_pGameState = 0;
static CASoundBank g_bank;
class CThing { public: CASoundBank& GetSampleBank() const { return g_bank; } };
struct CTCInventoryBase {
    CThing* ParentThing;
    CInventoryDef* PDef;
    void PlayInventoryMovementSound() const
    {
        if (g_pGameState) {
            CASoundBank& bank = ParentThing->GetSampleBank();
            CSoundInit init;
            init.Flags = 0x20;
            CSoundPlayer* player = g_pGameState->GetSoundPlayer();
            CInventoryDef* def = PDef;
            if (def)
                ++def->RefCount;
            PlaySample(player, bank, def->MovementSound, true, init, false);
            if (--def->RefCount == 0)
                def->Destroy();
        }
    }
};
int main()
{
    int bad = 0;
    CThing thing; CInventoryDef def; def.RefCount = 1; def.destroyed = 0; def.MovementSound = 41; def.NoMovementSound = 42;
    g_seenDef = &def;
    CTCInventoryBase inv; inv.ParentThing = &thing; inv.PDef = &def;
    inv.PlayInventoryMovementSound();                  /* no game state: nothing */
    if (g_player.calls != 0 || def.RefCount != 1) ++bad;
    CGameState state; g_pGameState = &state;
    inv.PlayInventoryMovementSound();
    if (g_player.calls != 1 || g_player.sample != 41 || !g_player.loop || g_player.attached || g_player.flags != 0x20) ++bad;
    if (g_player.bank != &g_bank || g_player.refDuringCall != 2 || def.RefCount != 1 || def.destroyed != 0) ++bad;
    def.RefCount = 0;                                  /* the copy held the last reference: released through Destroy */
    inv.PlayInventoryMovementSound();
    if (def.destroyed != 1 || def.RefCount != 0) ++bad;
    if (bad) { std::printf("005bd33c_TEST FAIL\n"); return 1; }
    std::printf("005bd33c_TEST PASS\n");
    return 0;
}