// CQ_DragonBossFightScript::SpawnSummoners() -- top the summoners up to the target: for each missing one pick one of two summoner creature defs at random, play the evil summon effect at the next summoner spawn point (round robin) and create the summoner there with the DBSummoner entity script
// Leaf: SpawnSummoners
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D26E60, 496 bytes, plain `ret`.
// Callees: rand 0x00BFEB16, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("CREATURE_SUMMONER_02" 0x012CBA28 /
//   "CREATURE_SUMMONER_01" 0x012CBA40, "SUMMON_CREATURE_SUMMON_EVIL" 0x012CBA0C, "DBSummoner" 0x012CB748, "" 0x0122D70E), CScriptBase::
//   IsActiveThreadTerminating 0x00CB7940, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CGameScriptInterface
//   (+0x40) slots 91 CreateCreature (+0x16c) and 100 CreateEffect (+0x190), both results dropped at once; the spawn
//   point's GetPos (slot 6, +0x18) called virtually through the vector element.
// Layout (retail = Ego_r.pdb - 0x14, 12-byte STLport vectors): SummonerSpawnPoints +0x58, NextSummonerSpawnPoint
//   +0x64, TargetNumSummoners +0x6c, NumSummoners +0x74.
// Shape: the random def is a named CCharString per iteration (`rand() % 2 == 0 ? 01 : 02` for retail's je); the wrap test
//   is an unsigned compare against size() (`jb`), the vtable constant of the dropped results is kept in ebp across the loop.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
int __cdecl rand();                                         // 0x00BFEB16 (MSVCR71)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    virtual void s01() const; virtual void s02() const; virtual void s03() const; virtual void s04() const; virtual void s05() const;
    virtual const C3DVector& GetPos() const;                // slot 6 (+0x18), 0x004AA980
    CCountedPointer<CScriptThing> PImp;                     // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                      // out of line 0x004AA840
};
template <class T> struct vector {                          // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return *(_M_start + n); }
    unsigned int size() const { return _M_finish - _M_start; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
namespace NScript {
class CQ_DragonBossFightScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Data;                             // +0x44
    long                  DragonState;                      // +0x48
    vector<CScriptThing>  MinionSpawnPoints;                // +0x4c
    vector<CScriptThing>  SummonerSpawnPoints;              // +0x58
    long                  NextSummonerSpawnPoint;           // +0x64
    long                  TargetNumMinions;                 // +0x68
    long                  TargetNumSummoners;               // +0x6c
    long                  NumMinions;                       // +0x70
    long                  NumSummoners;                     // +0x74
    void SpawnSummoners();
};
} // namespace NScript
using NScript::CQ_DragonBossFightScript;

void NScript::CQ_DragonBossFightScript::SpawnSummoners()
{
    long count = TargetNumSummoners - NumSummoners;
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        CCharString def(rand() % 2 == 0 ? "CREATURE_SUMMONER_01" : "CREATURE_SUMMONER_02");
        GSI->CreateEffect("SUMMON_CREATURE_SUMMON_EVIL", SummonerSpawnPoints[NextSummonerSpawnPoint].GetPos(), "", 0.0f, false, false);
        GSI->CreateCreature(def, SummonerSpawnPoints[NextSummonerSpawnPoint].GetPos(), "DBSummoner", false);
        ++NextSummonerSpawnPoint;
        if (NextSummonerSpawnPoint >= SummonerSpawnPoints.size()) {
            if (IsActiveThreadTerminating())
                return;
            NextSummonerSpawnPoint = 0;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}