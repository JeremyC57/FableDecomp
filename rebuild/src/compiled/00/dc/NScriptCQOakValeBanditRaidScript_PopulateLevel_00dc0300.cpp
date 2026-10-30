// CQ_OakValeBanditRaidScript::PopulateLevel() -- on a restart top the bandits up at the free restart points, otherwise clear the old bandits and spawn one at every bandit spawn point; attach each new bandit to the village, then refresh the hero's follower list
// Leaf: PopulateLevel
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC0300, 1328 bytes.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0 / default ctor 0x0099E4B0 / Format 0x0099F1F0, free 0x00BFEA14
//   (inline vector destructors), operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0 (inline ~CScriptThing),
//   vector<CScriptThing>::erase 0x008AC9D0, CleanupBandits 0x00DC0830; CScriptThing::GetPos (slot 6); through the
//   interface (+0x40): GetHero (70), GetThingWithScriptName (72), GetAllThingsWithScriptName (75), CreateCreature (91),
//   GetFollowingEntityList (231), EntityAttachToVillage (492).
// Layout (retail = Ego_r.pdb - 0x14, the vector 12 bytes so -0x18 after it): BanditsAwareOfPlayer +0x48,
//   HerosFollowers +0x4c, OpenGates +0x58, FirstSetup +0x59, BanditsStopChasing +0x70, NoPersistence +0x74.
// Shape: vector/CScriptThing model of the landed CQ_HangingTreeEvilScript::CleanUp 0x00D6B720.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(_M_start, _M_finish); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CCharStringFmt : CCharString {
    CCharStringFmt();                                   // 0x0099E4B0
    void __cdecl Format(const char* fmt, ...);          // 0x0099F1F0
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_OakValeBanditRaidScript : public CScriptBase {
public:
    CGameScriptInterface*       Game;                   // +0x40
    void*                       PMasterData;            // +0x44
    bool                        BanditsAwareOfPlayer;   // +0x48
    std::vector<CScriptThing>   HerosFollowers;         // +0x4c
    bool                        OpenGates;              // +0x58
    bool                        FirstSetup;             // +0x59
    unsigned char               _pad5a[0x70 - 0x5a];
    long                        BanditsStopChasing;     // +0x70
    bool                        NoPersistence;          // +0x74
    bool PopulateLevel();
    void CleanupBandits();                              // 0x00DC0830
};
} // namespace NScript
using NScript::CQ_OakValeBanditRaidScript;

bool NScript::CQ_OakValeBanditRaidScript::PopulateLevel()
{
    bool spawned = false;
    BanditsAwareOfPlayer = false;
    if (!FirstSetup && OpenGates) {
        std::vector<CScriptThing> restarts;
        Game->GetAllThingsWithScriptName("OakValeRestart", restarts);
        CScriptThing village = Game->GetThingWithScriptName("OVBR_Village");
        std::vector<CScriptThing> bandits;
        Game->GetAllThingsWithScriptName("OakValeBanditRaidBandit", bandits);
        if (!NoPersistence)
            BanditsStopChasing = bandits.size();
        NoPersistence = false;
        for (unsigned int i = 0; i < restarts.size() - bandits.size(); ++i) {
            spawned = true;
            CScriptThing bandit = Game->CreateCreature("CREATURE_BANDIT_GRUNT", restarts[i].GetPos(), "OakValeBanditRaidBandit", false);
            Game->EntityAttachToVillage(bandit, village);
        }
    } else {
        CleanupBandits();
        std::vector<CScriptThing> spawns;
        Game->GetAllThingsWithScriptName("M_BanditSpawn", spawns);
        CScriptThing village = Game->GetThingWithScriptName("OVBR_Village");
        FirstSetup = false;
        for (unsigned int i = 0; i < spawns.size(); ++i) {
            CScriptThing bandit = Game->CreateCreature("CREATURE_BANDIT_GRUNT", spawns[i].GetPos(), "OakValeBanditRaidBandit", false);
            Game->EntityAttachToVillage(bandit, village);
        }
    }
    HerosFollowers.clear();
    Game->GetFollowingEntityList(Game->GetHero(), HerosFollowers);
    CCharStringFmt msg;
    msg.Format("Found %d followers", HerosFollowers.size());
    return spawned;
}