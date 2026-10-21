// CCreatureAction_ReleaseTimeSpell::FinishAction() -- base FinishAction; release the camera's scripted control; re-equip
// Grade: DIFFER(430v430) -- frame layout: ours lets the haste (key, multiplier) pair temporary share the unsheathe-name slot (frame 0xD0) where retail gives it its own slot after the camera out-pointer (frame 0xD4), and this lands in edi instead of ebx; tried if/else, if-return, named pair, make_pair, temp name
// the item taken out (fast unsheathe carrying the haste key) when it lives, else register the creature's air resistance as
// a haste multiplier under HasteKey (adding a CTCHaste when missing) and recompute its speed
// Leaf: ?FinishAction@CCreatureAction_ReleaseTimeSpell
struct CTCBase;
struct CWorld;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
    const SEntry* LowerBound(const long* type) const; // 0x004365B0 (the const copy)
};
struct CPlayer {
    unsigned char _pad[0x20a];
    unsigned char m_autoFaceTarget;                   // +0x20a
};
class CThingCreatureBase;
struct CCreatureActionBase;
struct CCharString;
struct CTCParamsBase;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    CPlayer* GetPlayer();                             // 0x004C7A10
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0 (CThingCreatureBase)
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CBaseIntelligentPointer PItemRemoved;             // +0x2c
    unsigned char _pad34[0xa8 - 0x34];
    CWorld* GetPWorld();                              // 0x00692940
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    unsigned char _body[0x10];
    ~CCreatureAction_UnsheatheItemFromInventory();    // 0x005CC091
};
struct CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : CCreatureAction_UnsheatheItemFromInventory {
    CCreatureActionBase* PHasteAction;                // +0xb8
    CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(CCreatureActionBase* hasteAction, CThing* owner,
        const CCharString& animation, CThing* item, long priority, long timeout, CCreatureActionBase* next);   // 0x0069F1F0
};
struct CCombatActionBase : CCreatureActionBase {      // retail sizeof 0x114
    unsigned char _pad0a8[0x114 - 0xa8];
    void FinishAction();                              // 0x00859A20
};
struct CTCPhysicsBase {                               // interface 0x92
    float GetAirResistance();                         // 0x0079C740
};
struct CHasteMultiplier {                             // std::pair<void* const, float>
    void* first;
    float second;
    CHasteMultiplier(void* key, float value) : first(key), second(value) {}
};
struct CHasteMap {                                    // map<void*, float> (Dinkumware tree)
    struct iterator { void* node; };
    struct pairib { iterator first; bool second; };
    pairib insert(const CHasteMultiplier& value);     // 0x00580D21
};
struct CTCHaste {                                     // interface 0x8C
    unsigned char _pad0[0x18];
    CHasteMap Multipliers;                            // +0x18
    void RecalculateSpeed();                          // 0x00800020
};
struct CGameCameraManager {
    void ReleaseScriptedControl();                    // 0x00698950
};
struct CCameraHolder {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char _pad[0x34];
    CCameraHolder* PCurrentGameCamera;                // +0x34
    CCameraHolder* GetCurrentGameCamera() { return PCurrentGameCamera; }
};
struct CCreatureAction_ReleaseTimeSpell : CCombatActionBase {
    void* HasteKey;                                   // +0x114 (PDB +0x12C)
    void FinishAction();
};

void CCreatureAction_ReleaseTimeSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CGameCameraManager* camera;
    if (GetPWorld()->GetCurrentGameCamera()->GetCameraManager(&camera))
        camera->ReleaseScriptedControl();
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString name("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        GetCreature()->SetCurrentAction(&CCreatureAction_UnsheatheItemFromInventoryWithHasteAction((CCreatureActionBase*)HasteKey, GetCreature(), name, PItemRemoved.GetPItem(), 100, 250, 0));
    } else {
        CTCPhysicsBase* physics;
        const CThing* creature = PCreature.GetPItem();
        if (creature->GetInterface(&physics, 0x92)) {
            CTCHaste* haste;
            if (!PCreature.GetPItem()->GetInterface(&haste, 0x8c))
                haste = (CTCHaste*)PCreature.GetPItem()->AddTC("CTCHaste", true, 0);
            haste->Multipliers.insert(CHasteMultiplier(HasteKey, physics->GetAirResistance()));
            haste->RecalculateSpeed();
        }
    }
}
