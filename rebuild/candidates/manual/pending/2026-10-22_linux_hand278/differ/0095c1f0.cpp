// Grade: DIFFER(572v595) -- boundary over-capture: code + 8-entry jump table are RELOCATION_MATCH against the first 0x23C bytes (jt.py); the row also swallows 0x0095C430 (mov eax,ecx; mov [eax],0; ret 4) and 0x0095C440 (mov eax,[ecx]; ret). Land after a catalog split with verify_land_jumptable.py
// CAIStateGroup_HoverHornetQueen::Update() -- run the hornet queen boss phase: 0 go to the spawn point, 1/4 spawn babies, 2/5 leave combat while the babies fight then re-engage the target, 3/6 attack until the boss is ready for the phase change, 7 dive-bomb near the arena centre or attack until done; terminate when no longer runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095C1F0, length 540 bytes + an 8-entry jump table at 0x0095C40C.
// Callees: IsStillRunnable @ 0x0095A420, Terminate @ 0x008FCF40, CVectorMap::LowerBound @ 0x0040F020, CTCBoss::GetBossPhase
// @ 0x007AED10 / SetNextBossPhase @ 0x007AEDA0 / IsReadyForPhaseChange @ 0x007AEE90 / IncrementBossPhase @ 0x007AED20,
// GoToSpawnPoint @ 0x0095B020, Spawn @ 0x0095B820, CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CCombatManager::
// DeregisterCombatant @ 0x006ED9C0 / RegisterCombatant @ 0x006EDC50, WaitWhileBabiesFight @ 0x0095C0B0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, Attack @ 0x0095BCF0, IsKnockedDown @ 0x0085F010 (`__fastcall`), PlayIdle
// @ 0x0095B680, CThing::GetPosition @ 0x004C73D0, DiveBomb @ 0x0095B9E0.
// Layout: Ego_r.pdb CAIStateGroup_HoverHornetQueen PTarget +0x14, ArenaCentrePos +0x28; CWorld +0x4c combat manager.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetXYMagnitude() const { return (float)sqrt(X * X + Y * Y); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
extern float g_HornetDiveBombRange;             // 0x0138D2D8 (15.0f)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CThingCreatureBase : CThing { };
bool __fastcall IsKnockedDown(CThing* thing);   // 0x0085F010
struct CTCBoss {                                // interface 0xad
    long GetBossPhase();                        // 0x007AED10
    void IncrementBossPhase();                  // 0x007AED20
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0
    bool IsReadyForPhaseChange();               // 0x007AEE90
};
struct CCombatManager {
    void DeregisterCombatant(CThingCreatureBase* creature);   // 0x006ED9C0
    void RegisterCombatant(CThing* creature, CThing* target, bool flag);   // 0x006EDC50
};
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool IsStillRunnable();                     // 0x0095A420
    bool GoToSpawnPoint();                      // 0x0095B020
    void PlayIdle();                            // 0x0095B680
    void Spawn();                               // 0x0095B820
    void DiveBomb();                            // 0x0095B9E0
    void Attack();                              // 0x0095BCF0
    long WaitWhileBabiesFight();                // 0x0095C0B0
    void Update();
};

void CAIStateGroup_HoverHornetQueen::Update()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CTCBoss* boss;
    GetCreature()->GetInterface(&boss, 0xad);
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
            CWorld* world = GetPWorld();
            world->PCombatManager->DeregisterCombatant(creature);
        }
        if (WaitWhileBabiesFight() == 1 || !IsStillRunnable()) {
            Terminate();
            return;
        }
        boss->IsReadyForPhaseChange();
        {
            CThingCreatureBase* creature = GetCreature();
            CCombatManager* combat = GetPWorld()->PCombatManager;
            combat->RegisterCombatant(creature, PTarget.GetPItem(), true);
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
                if (IsKnockedDown(PTarget.GetPItem()))
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
            if ((PTarget.GetPItem()->GetPosition() - ArenaCentrePos).GetXYMagnitude() > g_HornetDiveBombRange)
                Attack();
            else
                DiveBomb();
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if (IsKnockedDown(PTarget.GetPItem()))
                PlayIdle();
        } while (IsStillRunnable());
        break;
    }
}
