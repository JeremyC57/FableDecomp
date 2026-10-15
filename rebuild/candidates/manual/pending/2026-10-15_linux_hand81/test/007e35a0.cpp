// Behaviour test for 0x007E35A0 CTCAtmosPlayer::CTCAtmosPlayer: the base gets the thing, the sound
// emitter and the atmos def index start at zero.
#include <cstdio>
#include <cstring>
#include <new>
struct CThing { long m_id; };

static int g_baseCtor;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { ++g_baseCtor; }
};

static double g_storage[64];
static void* Fresh() { std::memset(g_storage, 0xAB, sizeof g_storage); return g_storage; }
struct CASoundEmitter;
struct CTCAtmosPlayer : CTCBase_Calls {
    CASoundEmitter* PSoundEmitter;
    long            AtmosDefIndex;
    CTCAtmosPlayer(CThing* thing) : CTCBase_Calls(thing), PSoundEmitter(0), AtmosDefIndex(0) { }
};

int main()
{
    CThing thing;
    CTCAtmosPlayer* p = new (Fresh()) CTCAtmosPlayer(&thing);
    if (g_baseCtor != 1 || p->ParentThing != &thing || p->PSoundEmitter != 0 || p->AtmosDefIndex != 0) { std::printf("007e35a0_TEST FAIL\n"); return 1; }
    std::printf("007e35a0_TEST PASS\n");
    return 0;
}
