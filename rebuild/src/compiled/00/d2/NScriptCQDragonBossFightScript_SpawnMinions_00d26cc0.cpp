// CQ_DragonBossFightScript::SpawnMinions() -- top the minions up to the target: for each missing minion play the evil summon effect at its spawn point and create a war-dog minion there with the DBMinion entity script
// Leaf: SpawnMinions
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D26CC0, 401 bytes (416 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("SUMMON_CREATURE_SUMMON_EVIL"
//   0x012CBA0C, "CREATURE_MINION_WARDOG" 0x012CB9F4, "DBMinion" 0x012CB754, "" 0x0122D70E), CScriptBase::
//   IsActiveThreadTerminating 0x00CB7940, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CGameScriptInterface
//   (+0x40) slots 91 CreateCreature (+0x16c) and 100 CreateEffect (+0x190), both returning a CScriptThing that is
//   dropped at once; the spawn point's GetPos (slot 6, +0x18) called virtually through the vector element.
// Layout (retail = Ego_r.pdb - 0x14, 12-byte STLport vectors): MinionSpawnPoints +0x4c, TargetNumMinions +0x68,
//   NumMinions +0x70.
// Shape: the count and the index live in frame words, the element offset in edi; the trailing terminating test
//   is a bare call (both outcomes return).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
template <class T> struct vector {                          // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
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
    void SpawnMinions();
};
} // namespace NScript
using NScript::CQ_DragonBossFightScript;

void NScript::CQ_DragonBossFightScript::SpawnMinions()
{
    long count = TargetNumMinions - NumMinions;
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating())
            return;
        GSI->CreateEffect("SUMMON_CREATURE_SUMMON_EVIL", MinionSpawnPoints[i].GetPos(), "", 0.0f, false, false);
        GSI->CreateCreature("CREATURE_MINION_WARDOG", MinionSpawnPoints[i].GetPos(), "DBMinion", false);
    }
    if (IsActiveThreadTerminating())
        return;
}