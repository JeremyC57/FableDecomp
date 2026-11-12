// CAIStateGroup_MeleeCombatNearbyBase::RequestMoney() -- when the enemy is the living hero and its fine dialogue is allowed, show the fine for the creature's village's current crimes (with the hero's interface-4 amount), then yield the brain, refresh the best enemy and terminate unless still runnable
// Leaf: ?RequestMoney@CAIStateGroup_MeleeCombatNearbyBase@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D580, length 408 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
// CPlayer::GetPControlledCreature @ 0x00487DC0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy +0x14), CVectorMap::LowerBound
// @ 0x0040F020 (inlined lookups: 0xF3 = CTCFineDialogue on the hero, 0x23 = CTCVillager, interface 4 on the hero,
// 8 = CTCTargetingAI), CTCFineDialogue::IsAllowed @ 0x007E2790, GFGetPVillageInterface @ 0x007EE540 (twice; manifest
// "IS_THING_ALIVE<..>" propagated), CTCVillage::GetFineAmountForCurrentCrimes @ 0x0065A41C, CTCFineDialogue::Show @ 0x007E27F0,
// the brain's virtual slot 2 (SuspendSuspendableProcess), CTCTargetingAI::UpdatePBestEnemyCreatureForAttack @ 0x007F6200, own virtual slot 14
// IsStillRunnable(bool), Terminate @ 0x008FCF40.
// Shape: the fine dialogue, interface-4 and targeting lookups are the out-parameter GetInterface (component kept in eax;
// the fine dialogue's out word in the frame), the villager lookup is HasTC + the ternary GetTC; the callee-saved pushes
// of edi (hero map) and ebp (fine amount) sink into the branches that need them.
// Layout: engine/CAIStateGroup_MeleeCombatNearbyBase.h flattens the base; local model with its PEnemy (+0x14).
class CTCBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

class CTCVillage { public:
    long GetFineAmountForCurrentCrimes();       // 0x0065A41C
};
struct CTCVillager;
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

class CTCFineDialogue { public:                        // interface 0xF3
    bool IsAllowed();                           // 0x007E2790
    void Show(long fine, long amount, CTCVillage* village);     // 0x007E27F0
};
struct CTCHeroInterface4 {                      // interface 4 on the hero
    unsigned char _pad[0x3c];
    long m_amount;                              // +0x3c
};
class CTCTargetingAI { public:                         // interface 8
    void UpdatePBestEnemyCreatureForAttack(bool force);     // 0x007F6200
};

class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual bool SuspendSuspendableProcess();                       // slot 2 (+0x08) 0x00A44840 ?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ (Ego_r.pdb + FableWin.pdb, inherited; was the stand-in Yield; name-fix 2026-11-12)
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsStillRunnable(bool checkEnemy);  // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    protected:
    CPlayerManager& GetPlayerManager() const;        // 0x008FCF90
    public:
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;             // +0x14
    void RequestMoney();
};

void CAIStateGroup_MeleeCombatNearbyBase::RequestMoney()
{
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCFineDialogue* fine;
    if (hero != 0 && !hero->IsDead() && hero == PEnemy.Get() && hero->GetTC(TCI_FINE_DIALOGUE, &fine)) {
        if (fine->IsAllowed()) {
            CThing* creature = PBrain->PCreature;
            if (creature->HasTC(TCI_VILLAGE_MEMBER)) {
                CTCVillager* villager = ((CTCVillager*)&creature->GetTC(TCI_VILLAGE_MEMBER));
                if (((CTCVillageMember*)villager)->GetPVillage() != 0) {
                    CTCVillage* village = ((CTCVillageMember*)villager)->GetPVillage();
                    long amount = village->GetFineAmountForCurrentCrimes();
                    if (amount > 0) {
                        long heroAmount = 0;
                        CTCHeroInterface4* tc4;
                        if (hero->GetTC(TCI_HERO_STATS, &tc4))
                            heroAmount = tc4->m_amount;
                        fine->Show(amount, heroAmount, village);
                        PBrain->SuspendSuspendableProcess();
                        CTCTargetingAI* targeting;
                        if (PBrain->PCreature->GetTC(TCI_TARGETING, &targeting))
                            targeting->UpdatePBestEnemyCreatureForAttack(true);
                        if (!IsStillRunnable(true))
                            Terminate();
                    }
                }
            }
        }
    }
}