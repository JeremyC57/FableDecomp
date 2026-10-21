// CAIStateGroup_SeekScriptTarget::Update() -- finish any action, battle-cry once, mark the enemy in danger, unsheathe and move to the script target until reached
// Leaf: Update
// Grade: DIFFER(505v505) -- register/scheduling only after 4 grades: this in esi (ours edi), the inlined STLport lower_bound loads first into eax and copies it to edi (ours loads edi directly, shifting the loop-alignment nop), mov ecx,this hoisted above the cmp
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093C030; best of the probes graded under VC7.1 RTM (probe/0093c030d.cpp).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
__forceinline CInterfaceMap::SEntry* LowerBoundLen(CInterfaceMap::SEntry* first, long key, long len)
{
    while (len > 0) {
        long half = len / 2;
        CInterfaceMap::SEntry* middle = first + half;
        if (middle->type < key) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}
__forceinline CInterfaceMap::SEntry* LowerBoundInline(CInterfaceMap::SEntry* first, CInterfaceMap::SEntry* last, long key)
{
    return LowerBoundLen(first, key, last - first);
}
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterfaceInline(T** ppOut, long interfaceType)
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            CInterfaceMap::SEntry* last = m_interfaces.last;
            CInterfaceMap::SEntry* it = LowerBoundInline(m_interfaces.first, last, interfaceType);
            *ppOut = (T*)((it == last || interfaceType < it->type) ? last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
struct CRefCountedDef {
    virtual void v00();
    virtual void Destroy();
    long m_refs;                                // +0x04
    unsigned char _pad[0x48 - 8];
    long BattleSpeech;                          // +0x48
};
struct CCountedDefPtr {
    CRefCountedDef* m_p;
    CCountedDefPtr(CRefCountedDef* p) : m_p(p) { if (m_p) ++m_p->m_refs; }
    ~CCountedDefPtr() { if (--m_p->m_refs == 0) m_p->Destroy(); }
    CRefCountedDef* operator->() const { return m_p; }
};
struct CThingCreature;
struct CombatDialogueManager {                  // +0x38 of the combat system
    unsigned char _pad[0xc];
    CRefCountedDef* PDef;                       // +0x0c (+0x44 of the combat system), CCountedPtr
    bool SaySpeech(CThingCreature* creature, long speech);    // 0x008AFC40
    bool PlayBattleCry(CThingCreature* creature);            // 0x008AF850
};
struct CCombatSystem {
    unsigned char _pad[0x38];
    CombatDialogueManager Dialogue;             // +0x38
};
struct CWorld {
    unsigned char _pad[0x4c];
    CCombatSystem* PCombatSystem;               // +0x4c
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
struct CTCStealth {                             // interface 0x36
    void SetAsInDanger(CThingCreature* creature, long level);   // 0x00747B80
};
struct CCombatManager {
    void CreatureUnsheathWeapon(CThingCreature* creature, long how);   // 0x006EBDD0
};
struct CThingCreature : CThing {
    bool IsPerformingAnyAction();               // 0x006622A0
    bool IsFreeToFinishAction();                // 0x006621C0
    void FinishCurrentAction();                 // 0x00662650
    CCombatManager* GetCombatManager();         // 0x00661ED0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerThing target, float proximity, bool* reached,
        long movement, bool a, bool b, bool c, bool d, bool e);    // 0x0097E380
};
struct CAIStateGroup_SeekScriptTarget : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;            // +0x14
    bool IsStillRunnable();                     // 0x0093BFA0
    virtual void Update();
};

void CAIStateGroup_SeekScriptTarget::Update()
{
    if (!IsStillRunnable())
        goto stop;
    if (PBrain->PCreature->IsPerformingAnyAction() && PBrain->PCreature->IsFreeToFinishAction())
        PBrain->PCreature->FinishCurrentAction();
    {
        bool reached = false;
        bool spoken = false;
        long result;
        do {
            if (PEnemy.GetPItem() == 0 || (PEnemy.GetPItem()->m_flags91 & 1))
                goto stop;
            if (!spoken) {
                if (WorldRandomRange(*GetPWorld()->GetPRandomSeed(), 100) > 50) {
                    long speech = CCountedDefPtr(GetPWorld()->PCombatSystem->Dialogue.PDef)->BattleSpeech;
                    CThingCreature* creature = PBrain->PCreature;
                    CCombatSystem* combat = GetPWorld()->PCombatSystem;
                    combat->Dialogue.SaySpeech(creature, speech);
                    spoken = true;
                } else {
                    CThingCreature* creature = PBrain->PCreature;
                    CCombatSystem* combat = GetPWorld()->PCombatSystem;
                    spoken = combat->Dialogue.PlayBattleCry(creature);
                }
            }
            CTCStealth* stealth;
            if (PEnemy.GetPItem()->GetInterfaceInline(&stealth, 0x36))
                stealth->SetAsInDanger(PBrain->PCreature, 2);
            PBrain->PCreature->GetCombatManager()->CreatureUnsheathWeapon(PBrain->PCreature, 4);
            result = AIAction_MoveToThingProximity(2, PEnemy, 1.0f, &reached,
                5, true, false, true, true, false);
            if (!IsStillRunnable())
                goto stop;
        } while (result == 4);
        if (result == 2)
            return;
    }
stop:
    Terminate();
}
