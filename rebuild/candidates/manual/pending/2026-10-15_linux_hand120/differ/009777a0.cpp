// Grade: DIFFER(ours v retail) -- 128v128 scheduling only: 'lea edi,[esi+18h]' (&PEnemy, reused for the later GetPItem) is hoisted above the temporary's GetPItem call (retail: after it)
// CAIStateGroup_Guildmaster::IsRunnable() -- target the main player's character as the enemy, register the fight with the combat manager and always run
// Leaf: ?IsRunnable@CAIStateGroup_Guildmaster@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009777A0, length 128 bytes.
// Callees: the state group's player-manager accessor @ 0x008FCF90 (PBrain->+0x20), CPlayerManager::GetMainPlayer
// @ 0x00449970 (landed name), CPlayer::GetPCharacter @ 0x00487DC0 (`add ecx,2Ch ; jmp GetPItem`),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10, the
// world accessor @ 0x008FCFC0 (PBrain->+0x1c->+0x24), CCombatManager::RegisterCombatant(CThingCreatureBase*,
// CThingCreatureBase*, bool) @ 0x006EDC50. Vtable 0x0123E7D8 = CIntelligentPointer<CThingCreatureBase>.
//
// Tried: operator= from the temporary (this), optimize("s") (frame pointer, 98 bytes), explicit
// PEnemy.SetPItem(temp.GetPItem()), a scoped named temporary -- all hoist the lea.
// Shape: `PEnemy = CIntelligentPointer<CThingCreatureBase>(player character)` (the stack temporary: base
// ctor, push, vtable, SetPItem; copied with SetPItem(GetPItem()), base destructor; the member address kept
// in edi for the later GetPItem). The creature (ebx) and the world's combat manager at +0x4c (esi) are
// named locals read before the argument pushes (the lane evaluates a chained object expression after its
// arguments). PDB layout (sizeof 0x40): AttackNum +0x14, PEnemy +0x18, PLastTeleportMarker +0x20,
// TeleportMarkers +0x28 (12-byte retail vector), LastWillFrame +0x38, LastTeleportFrame +0x3c. The engine
// header CAIStateGroup_Guildmaster.h is a flattened data-only layout without the vptr chain: local model.
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                         // one vtable per instantiation
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

struct CPlayer {
    CThingCreatureBase* GetPCharacter();        // 0x00487DC0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};

struct CCombatManager {
    void RegisterCombatant(CThingCreatureBase* creature, CThingCreatureBase* enemy, bool fight);   // 0x006EDC50
};

struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* m_pCombatManager;           // +0x4c
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    CWorld* GetPWorld();                        // 0x008FCFC0
protected:
    virtual bool IsRunnable();
};

struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    long AttackNum;                                         // +0x14
    CIntelligentPointer<CThingCreatureBase> PEnemy;         // +0x18
protected:
    virtual bool IsRunnable();
};

bool CAIStateGroup_Guildmaster::IsRunnable()
{
    PEnemy = CIntelligentPointer<CThingCreatureBase>(GetPPlayerManager()->GetMainPlayer()->GetPCharacter());
    CThingCreatureBase* creature = PBrain->PCreature;
    CCombatManager* combat = GetPWorld()->m_pCombatManager;
    combat->RegisterCombatant(creature, PEnemy.GetPItem(), true);
    return true;
}
