// CQ_AmbushTradersScript::WatchForArcherSpawnConditions() -- once the spy has made contact and the hero is in DesertedFarm, create a crossbow guard "AT_Phase3Archer" on every live M_AT_ArcherSpawn point, attach it to the quest and, if M_AT_ArcherSpawnFaceMe lives, turn the archer to face it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0F910, length 599 bytes. Spawned as "WatchForArcherSpawnConditions"
//   (0x012F8450) by CQ_AmbushTradersScript::ProcessInitialSpyExtraction 0x00F0F680; the manifest label
//   "CQ_GuildTrainingScript::WatchForSparrowKilled" is propagated.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (out of line),
//   CScriptThing::IsAlive 0x004AB130 (slot 75; called directly on the local), ~vector<CScriptThing> 0x008AC970 (out of
//   line), CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40): NewScriptFrame (7),
//   IsRegionLoaded (12), GetThingWithScriptName (72), GetAllThingsWithScriptName (75), CreateCreatureOnEntity (93),
//   EntityAttachToScript (465), EntitySetFacingAngleTowardsThing (475), GetActiveQuestName (655).
// Layout: SpyContactMade +0x5f (retail = Ego_r.pdb CQ_AmbushTradersScript - 0x14).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                     // 0x008AC970 for CScriptThing
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x5f - 0x48];
    bool                  SpyContactMade;               // +0x5f
    void WatchForArcherSpawnConditions();
};
void CQ_AmbushTradersScript::WatchForArcherSpawnConditions()
{
    while (!SpyContactMade) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("DesertedFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    std::vector<CScriptThing> spawnPoints;
    long count = Game->GetAllThingsWithScriptName("M_AT_ArcherSpawn", spawnPoints);
    CScriptThing faceMe = Game->GetThingWithScriptName("M_AT_ArcherSpawnFaceMe");
    for (long i = 0; i < count; ++i) {
        if (spawnPoints[i].IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing archer = Game->CreateCreatureOnEntity("CREATURE_BS_GUARD_CROSSBOW", spawnPoints[i], "AT_Phase3Archer");
            Game->EntityAttachToScript(archer, Game->GetActiveQuestName());
            if (faceMe.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(archer, faceMe, false);
            }
        }
    }
}
}