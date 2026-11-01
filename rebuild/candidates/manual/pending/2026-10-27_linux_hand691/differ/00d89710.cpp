// NScript::CQ_HeroSoulsNostroScript::DealWithCreatureGeneration() -- wait for the quest start screen, spawn Nostro at the undead generator with a green health bar, raise 8 summoned undead at random "HSUndeadHere" points, then while Nostro lives raise one more every 3 timer units while fewer than 8 are alive
// Grade: DIFFER(903v960) -- residue: retail expands ~CScriptThing inline at ONE site (the loop-2 creature temporary: vtable 0x01238C8C cached in ebp, timer index spilled to a frame word, edi reused) and calls 0x004AA840 at every other site; VC7.1 here inlines all six sites (inline dtor) or none (declared dtor). The documented lever (in-body #pragma inline_depth(0), GOTCHAS b18) is banned by the chunk brief; frame 0x38 vs 0x3c and the slot shifts follow from it. Next step: coordinator decision on the inline_depth lever, or the b18 recipe under an owner exception.
// Leaf: ?DealWithCreatureGeneration@CQ_HeroSoulsNostroScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D89710, 960 bytes, plain `ret`. Identity: CQ_HeroSoulsNostroScript's Main-side spawner stores
//   0x00D89710 as the member-function pointer of a CSpawnedFunc (+0x34, vtable 0x012D4D40, owner +0x38) named "DealWithCreatureGeneration"
//   (0x012D4DB4, at 0x00D880C8); Ego_r.pdb and FableWin.pdb both list ?DealWithCreatureGeneration@CQ_HeroSoulsNostroScript@NScript@@QAEXXZ.
// Layout (retail = Ego_r.pdb - 0x14): GSI +0x40, KickedOffQuestStartScreen +0x4d, UndeadAlive +0x50.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString 0x0099EBF0 / ~ 0x0099EAE0; ~CScriptThing 0x004AA840;
//   ~vector<CScriptThing> 0x008AC970; rand 0x00BFEB16; GSI slots 7 NewScriptFrame, 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName,
//   91 CreateCreature, 321 DisplayQuestInfo, 325 AddQuestInfoBarHealth; pDSTGame 0x0143E8F8 timer slots 87..90 (CTimer, Ego_r TimerIndex);
//   CScriptThing::GetPos (0x004AA980) and IsAlive (0x004AB130) expanded inline on locals, GetPos virtual on vector elements; zero vector 0x0143E8E0.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
class C3DVector { public: float X, Y, Z; };
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
extern const C3DVector g_ZeroVector;                        // 0x0143E8E0
inline const C3DVector& CScriptThing::GetPos() const
{
    if (!PImp.Data)
        return g_ZeroVector;
    return PImp.Data->GetPos();
}
inline bool CScriptThing::IsAlive() const
{
    if (PImp.Data && PImp.Data->IsAlive())
        return true;
    return false;
}
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                          // 0x008AC970 for CScriptThing
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}; the setter is operator=(long)).
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
#include "engine/CScriptBase.h"
extern "C" int __cdecl rand();
namespace NScript {
class CQ_HeroSoulsNostroScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Data;                             // +0x44
    long                  SummonerCount;                    // +0x48
    bool                  PlayedNostroCS;                   // +0x4c
    bool                  KickedOffQuestStartScreen;        // +0x4d
    long                  UndeadAlive;                      // +0x50
    CTimer                NostroVulnerableTimer;            // +0x54
    void DealWithCreatureGeneration();
};
} // namespace NScript
void NScript::CQ_HeroSoulsNostroScript::DealWithCreatureGeneration()
{
    while (!KickedOffQuestStartScreen) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing generator = GSI->GetThingWithScriptName("HS_UndeadGenerator");
    CScriptThing nostro = GSI->CreateCreature("CREATURE_NOSTRO_01", generator.GetPos(), "Nostro", true);
    GSI->DisplayQuestInfo(true);
    GSI->AddQuestInfoBarHealth(nostro, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_UNDEAD_GENERAL", 1.0f);
    std::vector<CScriptThing> spawnPoints;
    GSI->GetAllThingsWithScriptName("HSUndeadHere", spawnPoints);
    for (long i = 0; i < 8; ++i) {
        if (IsActiveThreadTerminating())
            return;
        GSI->CreateCreature("CREATURE_UNDEAD_NOSTRO_MINION", spawnPoints[rand() % spawnPoints.size()].GetPos(), "SUMMONED_CREATURE", true);
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    while (nostro.IsAlive()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0 && UndeadAlive < 8) {
            if (IsActiveThreadTerminating())
                return;
            GSI->CreateCreature("CREATURE_UNDEAD_NOSTRO_MINION", spawnPoints[rand() % spawnPoints.size()].GetPos(), "SUMMONED_CREATURE", true);
            timer = 3;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
