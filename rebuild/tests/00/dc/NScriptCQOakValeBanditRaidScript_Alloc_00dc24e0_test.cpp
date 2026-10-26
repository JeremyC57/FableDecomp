// Behaviour test for NScript::CQ_OakValeBanditRaidScript::Alloc (0x00DC24E0): a new raid script with the game
// interface and master data stored, the HerosFollowers vector empty and BanditInRange an empty script thing.
#include <cstdio>

class CGameScriptInterfaceBase { public: int id; };
class CScriptDataBase { public: int id; };
static int g_rootCtors = 0;
class CBaseClass {
public:
    CBaseClass() { ++g_rootCtors; }
    virtual ~CBaseClass() { }
};
template <class T> class CCountedPointer {
public:
    T* Data; void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass {
public:
    CScriptThing() { }
    CCountedPointer<CScriptThing> PImp;
};
class CScriptBase { public: virtual ~CScriptBase() { } };
template <class TData> struct CLevelScriptBase : CScriptBase {
    CGameScriptInterfaceBase* Game;
    TData* MasterScriptData;
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((TData*)data) { }
};
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
class CQ_OakValeBanditRaidScript : public CLevelScriptBase<CScriptDataBase> {
public:
    bool BanditsAwareOfPlayer, BanditsAwareOfPlayerAgain, GateClosed;
    CVector<CScriptThing> HerosFollowers;
    bool OpenGates, FirstSetup;
    long SoundPlayed;
    bool StopWhistling, BanditsKilled, IsBanditInRange;
    CScriptThing BanditInRange;
    long BanditsStopChasing;
    bool NoPersistence;
    CQ_OakValeBanditRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CLevelScriptBase<CScriptDataBase>(data, game) { }
    static CScriptBase* Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) { return new CQ_OakValeBanditRaidScript(data, game); }
};

int main()
{
    CGameScriptInterfaceBase game; game.id = 1;
    CScriptDataBase data; data.id = 2;
    CQ_OakValeBanditRaidScript* s = (CQ_OakValeBanditRaidScript*)CQ_OakValeBanditRaidScript::Alloc(&data, game);
    if (s == 0 || s->Game != &game || s->MasterScriptData != &data || g_rootCtors != 1
        || s->HerosFollowers._M_start || s->HerosFollowers._M_finish || s->HerosFollowers._M_end_of_storage
        || s->BanditInRange.PImp.Data != 0 || s->BanditInRange.PImp.Info != 0) {
        std::printf("00dc24e0_TEST FAIL\n");
        return 1;
    }
    delete s;
    std::printf("00dc24e0_TEST PASS\n");
    return 0;
}