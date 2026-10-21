// CAIStateGroup_Thunder::Update() -- mark the target in danger, and in the idle combat state cast the lightning storm when the target is within the ability range
// Leaf: Update
// Grade: DIFFER(506v495) -- retail loads the CDefPointer target once into esi after GetDef(index,&def) and reuses it for the range test and the final release across later calls (ours reloads the escaped local, like the list sentinel in 0x0091B570 / 0x00966AF0); string/def frame slots swapped
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096C4F0; best of the probes graded under VC7.1 RTM (probe/0096c4f0c.cpp).
struct CTCBase;
struct C3DVector { float x, y, z; };
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
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
#pragma pack(pop)
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CSpecialAbilityDef {
    virtual void v00();
    virtual void Destroy();
    long m_refs;                                // +0x04
    unsigned char _pad[0x44 - 8];
    float Range;                                // +0x44
};
struct CDefPointer {
    CSpecialAbilityDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (--m_p->m_refs == 0) m_p->Destroy(); }
    CSpecialAbilityDef* operator->() const { return m_p; }
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& Get();       // 0x0044C6B0
    long GetDefGlobalIndex(const CCharString& name, bool b);   // 0x009AD390
    bool GetDef(long index, CDefPointer* out);  // 0x008E31D0
};
struct CThingCreatureBase;
struct CTCStealth {                             // interface 0x36
    void SetAsInDanger(CThing* creature, long level);   // 0x00747B80
};
struct CTCThunderCombat {                       // interface 0xad
    long GetState();                            // 0x007AED10
    void Reset();                               // 0x007AED50
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
};
struct CCreatureAction_ThunderLightningStorm : CCreatureActionBase {
    CCreatureAction_ThunderLightningStorm(CThingCreatureBase* creature);   // 0x0096C6E0
    ~CCreatureAction_ThunderLightningStorm();   // 0x0062E900
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
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
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
};
__forceinline float Distance2D(CThing* target, CThing* me)
{
    const C3DVector& from = me->GetPosition();
    const C3DVector& to = target->GetPosition();
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    return (float)sqrt(dx * dx + dy * dy);
}
#pragma pack(push, 1)
struct CAIStateGroup_Thunder : CAIStateGroupBase {
    CBaseIntelligentPointer PArenaCentre;       // +0x14
    bool bIsRangeAttack;                        // +0x1c
    CBaseIntelligentPointer PTargetThing;       // +0x1d
    unsigned char _pad_0x25[3];
    long HitCount;                              // +0x28
    virtual void Update();
};
#pragma pack(pop)

void CAIStateGroup_Thunder::Update()
{
    CTCThunderCombat* combat;
    PBrain->PCreature->GetInterface(&combat, 0xad);
    if (IS_THING_ALIVE(PTargetThing)) {
        CTCStealth* stealth;
        if (PTargetThing.GetPItem()->GetInterface(&stealth, 0x36))
            stealth->SetAsInDanger(PBrain->PCreature, 3);
    }
    switch (combat->GetState()) {
    case 0:
        if (IS_THING_ALIVE(PTargetThing)) {
            long index = CGameDefinitionManager::Get().GetDefGlobalIndex("SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF", true);
            CDefPointer def;
            CGameDefinitionManager::Get().GetDef(index, &def);
            float dist = Distance2D(PTargetThing.GetPItem(), PBrain->PCreature);
            if (dist < def->Range && dist > 1.0f) {
                long result = AIAction_PerformActionAndWait(&CCreatureAction_ThunderLightningStorm(GetCreatureBase()), false);
                if (result != 0) {
                    HitCount = 0;
                    combat->Reset();
                }
            }
        }
        Terminate();
        break;
    case 1:
        Terminate();
        break;
    }
}
