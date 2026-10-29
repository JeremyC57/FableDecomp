// CAIStateGroup_MazeTutorialBattle::OnActivate() -- register the creature against its enemy as a combatant
// when the enemy is flagged for it, then stamp FrameOfLastShield with the current frame.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954190, length 74 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy at +0x14, called twice: once for the flag
// test, once for the argument), CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (twice: once for the combat manager
// at +0x4c, once with its result DROPPED in front of the frame read -- `GetPWorld()->GetFrameCount()` with a
// static GetFrameCount, the landed CreatureUseCover shape), CCombatManager::RegisterCombatant(creature, enemy,
// true) @ 0x006EDC50, CWorld::GetFrame @ 0x0049D870 (__fastcall, no arguments).
// Shape: the flag is bit 3 of the enemy's byte at +0x6c, tested as a branch; inside the body the enemy, the
// creature and the world are named locals in that order (edi, ebx, then the call) so the pushes follow the
// world call. FrameOfLastShield is +0x20 (engine header name); the header flattens the base, so the layout
// is local.
class CThingCreatureBase;

class CThing { public:
    unsigned char _pad[0x6c];
    unsigned char m_flags6c;                // +0x6c bit 3: wants combat registration
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatManager { public:
    void RegisterCombatant(CThingCreatureBase* creature, CThing* target, bool hostile);   // 0x006EDC50
};


class CWorld { public:
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;       // +0x4c
    static long __fastcall GetFrame();
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    CWorld* GetPWorld();                    // 0x008FCFC0
};

struct CAIStateGroup_MazeTutorialBattle : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;         // +0x14 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad1c[4];                // +0x1c
    long FrameOfLastShield;                 // +0x20
    virtual void OnActivate();
};

void CAIStateGroup_MazeTutorialBattle::OnActivate()
{
    if (PEnemy.GetPItem()->m_flags6c & 8) {
        CThing* enemy = PEnemy.GetPItem();
        CThingCreatureBase* creature = PBrain->Creature;
        CWorld* world = GetPWorld();
        world->m_pCombatManager->RegisterCombatant(creature, enemy, true);
    }
    FrameOfLastShield = GetPWorld()->GetFrame();
}