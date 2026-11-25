// Leaf: Update
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Grade: RELOCATION_MATCH 572/572 on the corrected span 0x0095C1F0+572 (code + embedded jump table; bounds audit) -- catalog row 608 B also swallowed 0x0095C430/0x0095C440
// CAIStateGroup_HoverHornetQueen::Update() -- run the hornet queen boss phase: 0 go to the spawn point, 1/4 spawn babies, 2/5 leave combat while the babies fight then re-engage the target, 3/6 attack until the boss is ready for the phase change, 7 dive-bomb near the arena centre or attack until done; terminate when no longer runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095C1F0, length 540 bytes + an 8-entry jump table at 0x0095C40C.
// Callees: IsStillRunnable @ 0x0095A420, Terminate @ 0x008FCF40, CVectorMap::LowerBound @ 0x0040F020, CTCBoss::GetBossPhase
// @ 0x007AED10 / SetNextBossPhase @ 0x007AEDA0 / IsReadyForPhaseChange @ 0x007AEE90 / IncrementBossPhase @ 0x007AED20,
// GoToSpawnPoint @ 0x0095B020, Spawn @ 0x0095B820, CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CCombatManager::
// DeregisterCombatant @ 0x006ED9C0 / RegisterCombatant @ 0x006EDC50, WaitWhileBabiesFight @ 0x0095C0B0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, Attack @ 0x0095BCF0, IsKnockedDown @ 0x0085F010 (`__fastcall`), PlayIdle
// @ 0x0095B680, CThing::GetPos @ 0x004C73D0, DiveBomb @ 0x0095B9E0.
// Layout: Ego_r.pdb CAIStateGroup_HoverHornetQueen PTarget +0x14, ArenaCentrePos +0x28; CWorld +0x4c combat manager.
// Name fix (agentNF24): 0x0095A420 respelled the protected CAIStateGroup_HoverHornetQueen::IsStillRunnable (Ego_r 0x0097ECE0 / FableWin 0x02C0FEC0 ?IsStillRunnable@CAIStateGroup_HoverHornetQueen@@IAE_NXZ; was public)
#include <math.h>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetXYMagnitude() const { return (float)sqrt(X * X + Y * Y); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
extern float g_HornetDiveBombRange;             // 0x0138D2D8 (15.0f)
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public: };
// Naming (NF439 gen, 2026-11-25): callee 0x0085F010 binds its PDB spelling ?IsKnockedDown@NCreatureMode@@YI_NABVCThing@@@Z (T1 target; was the free stand-in ?IsKnockedDown@@YI_NPAVCThing@@@Z).
namespace NCreatureMode { bool __fastcall IsKnockedDown(CThing const& thing); }
class CTCBoss { public:                                // interface 0xad
    long GetBossPhase() const;                        // 0x007AED10
    void IncrementBossPhase();                  // 0x007AED20
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0
public:
    bool IsReadyForPhaseChange() const;   // 0x007AEE90
};
class CCombatManager { public:
    // NF122 (2026-11-20, q4266): Ego_r.pdb 0x007173E0 / FableWin.pdb 0x02412E40 ?DeregisterCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z (by reference; engine/CCombatManager.h); was DeregisterCombatant(CThingCreatureBase*).
    void DeregisterCombatant(CThingCreatureBase& creature);   // 0x006ED9C0
    void RegisterCombatant(CThing* creature, CThing* target, bool flag);   // 0x006EDC50
};
class CWorld { public:
    unsigned char _pad0[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/fable_ai_action_enums.h"   // NF333: EAIActionReturn (WaitWhileBabiesFight's PDB return)

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    CWorld& GetWorld() const;                        // 0x008FCFC0 ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (Ego_r/FableWin, protected; agentNF16: was GetPWorld)
public:
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;            // +0x14
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
protected:
    bool IsStillRunnable();                     // 0x0095A420 (protected: Ego_r/FableWin IAE)
public:
    protected: void GoToSpawnPoint(); public:   // 0x0095B020
    protected:
    void PlayIdle();
    public:
                            // 0x0095B680
    protected: void Spawn(); public:                               // 0x0095B820
    protected: void DiveBomb(); public:                            // 0x0095B9E0
    protected: void Attack(); public:                              // 0x0095BCF0
    protected: EAIActionReturn WaitWhileBabiesFight(); public:   // 0x0095C0B0
    void Update();
};

void CAIStateGroup_HoverHornetQueen::Update()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CTCBoss* boss;
    GetCreature()->GetTC(TCI_BOSS, &boss);
    switch (boss->GetBossPhase()) {
    case 0:
        GoToSpawnPoint();
        boss->SetNextBossPhase(1, false);
        break;
    case 1:
    case 4:
        Spawn();
        if (IsStillRunnable())
            boss->SetNextBossPhase(boss->GetBossPhase() + 1, false);
        break;
    case 2:
    case 5:
        {
            CThingCreatureBase* creature = GetCreature();
            CWorld* world = &GetWorld();
            world->PCombatManager->DeregisterCombatant(*creature);
        }
        if (WaitWhileBabiesFight() == 1 || !IsStillRunnable()) {
            Terminate();
            return;
        }
        boss->IsReadyForPhaseChange();
        {
            CThingCreatureBase* creature = GetCreature();
            CCombatManager* combat = GetWorld().PCombatManager;
            combat->RegisterCombatant(creature, PTarget.Get(), true);
        }
        boss->IncrementBossPhase();
        break;
    case 3:
    case 6:
        if (IsStillRunnable()) {
            do {
                if (boss->IsReadyForPhaseChange())
                    break;
                Attack();
                if (!IsStillRunnable()) {
                    Terminate();
                    return;
                }
                if (NCreatureMode::IsKnockedDown(*(PTarget.Get())))
                    PlayIdle();
            } while (IsStillRunnable());
        }
        if (IsStillRunnable())
            boss->IncrementBossPhase();
        break;
    case 7:
        do {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if ((PTarget.Get()->GetPos() - ArenaCentrePos).GetXYMagnitude() > g_HornetDiveBombRange)
                Attack();
            else
                DiveBomb();
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if (NCreatureMode::IsKnockedDown(*(PTarget.Get())))
                PlayIdle();
        } while (IsStillRunnable());
        break;
    }
}