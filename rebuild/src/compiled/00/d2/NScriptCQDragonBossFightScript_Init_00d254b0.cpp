// CQ_DragonBossFightScript::Init() -- reset the dragon state and the spawn bookkeeping, take the fly-by count between summoner spawns from the quest globals, and register the boss-fight region with the quest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D254B0, length 110 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (twice), ~CCharString @ 0x0099EAE0 (twice), and
// AddQuestRegion (slot 271, +0x43c) through the quest's game script interface (+0x40). Strings 0x0126CE40 and
// 0x012C45B4 (masked); the quest-globals record @ 0x0143E90C holds the count at +0xbe0.
// Layout: Ego_r.pdb CQ_DragonBossFightScript - 0x14 with retail 12-byte vectors: DragonState +0x48,
// NextSummonerSpawnPoint +0x64, NumMinions +0x70, NumSummoners +0x74, NumFlyBysBetweenSummonerSpawns +0x78.
// Shape: both strings are NAMED locals built before the interface is read (temporaries in the call let VC fetch the
// vtable first); the region (second argument) is declared first and sits in the higher frame word.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0xbe0];
    long          NumFlyBysBetweenSummonerSpawns;           // +0xbe0
};
extern CScriptDef* pGlobalScriptDef;                      // 0x0143E90C

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

struct CScriptThingVector { void* First; void* Last; void* End; };   // STLport vector<CScriptThing> (12 bytes)

namespace NScript {
class CQ_DragonBossFightScript : public CQuestScriptBase {
public:
    long               DragonState;                         // +0x48
    CScriptThingVector MinionSpawnPoints;                   // +0x4c
    CScriptThingVector SummonerSpawnPoints;                 // +0x58
    long               NextSummonerSpawnPoint;              // +0x64
    long               TargetNumMinions;                    // +0x68
    long               TargetNumSummoners;                  // +0x6c
    long               NumMinions;                          // +0x70
    long               NumSummoners;                        // +0x74
    long               NumFlyBysBetweenSummonerSpawns;      // +0x78
    void Init();
};
} // namespace NScript
using NScript::CQ_DragonBossFightScript;

void NScript::CQ_DragonBossFightScript::Init()
{
    DragonState = 0;
    NextSummonerSpawnPoint = 0;
    NumMinions = 0;
    NumSummoners = 0;
    NumFlyBysBetweenSummonerSpawns = pGlobalScriptDef->NumFlyBysBetweenSummonerSpawns;
    CCharString region("DragonCliff2", -1);
    CCharString quest("Q_DragonBossFight", -1);
    GSI->AddQuestRegion(quest, region);
}