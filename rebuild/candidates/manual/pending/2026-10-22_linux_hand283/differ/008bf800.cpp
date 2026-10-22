// CCreatureAction_GenericLeaderCommand::Initialise() -- pick the leader's command animation (target knocked down / an ally knocked down / a 20% random taunt), stamp the leader component and say the matching combat speech, then the base Initialise
// Grade: DIFFER(570v570) -- loop entry loads header/begin into eax/ecx swapped (retail ecx=header, eax=begin) and the arm tail merge pairs arms 1+3 (retail 2+3)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BF800, length 570 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CVectorMap::LowerBound @ 0x0040F020, CCombatActionBase::GetCombatManager @ 0x00856B10 (manifest label is
//          propagated STL noise), CCombatManager group lookup GetValueForThing @ 0x006E9360, IsKnockedDown(CThing*)
//          @ 0x0085F010 (`__fastcall`), STLport tree iterator _M_increment @ 0x006EF790, CCharString::operator=(const
//          char*) @ 0x0099EFE0, CCreatureActionBase::GetPWorld @ 0x00692940, CWorld::GetWorldFrame @ 0x0049D870
//          (static), CWorld random seed @ 0x0049D810, GFROR13 @ 0x00497890, CombatDialogueManager::SaySpeech
//          @ 0x008AFC40, CCombatActionBase::SetAnimationName @ 0x008574D0, CCombatActionBase::Initialise @ 0x00857520,
//          CCharString::~CCharString @ 0x0099EAE0.
// Layout: CCombatActionBase (retail 0x114) with PCreature at +8 and PTarget at +0xa8; the combat manager's dialogue
//   manager at +0x38 with its dialogue def pointer at +0x44 (as the landed CCreatureAction_CombatBoast::Initialise
//   0x008B6E60); the dialogue def's command speeches at +0x60/+0x64/+0x68 (names descriptive).
struct CTCBase;
struct CThingCreatureBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const char* text);          // 0x0099EFE0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[0x24 / 4];             // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CThingCreatureBase : CThing { };

bool __fastcall IsKnockedDown(CThing* thing);          // 0x0085F010

long __fastcall GFROR13(unsigned long value);          // 0x00497890
struct CWorld {
    static long GetWorldFrame();                       // 0x0049D870
    unsigned long* GetRandomSeed();                    // 0x0049D810
};
__forceinline unsigned long GetRandomBelow(CWorld* world, unsigned long n)
{
    unsigned long* seed = world->GetRandomSeed();
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed % n;
}

struct SGroupNode {                                    // STLport _Rb_tree_node of the group's member map
    long        m_colour;
    SGroupNode* m_pParent;
    SGroupNode* m_pLeft;
    SGroupNode* m_pRight;
    unsigned char _key[8];
    CThing*     m_member;                              // +0x18
};
struct SGroupIterator {
    SGroupNode* m_node;
    SGroupIterator(SGroupNode* n) : m_node(n) { }
    bool operator!=(const SGroupIterator& o) const { return m_node != o.m_node; }
    void Increment();                                  // 0x006EF790
};
struct CGroupMembers {                                  // STLport map: header pointer first
    SGroupNode* m_header;
    SGroupNode* begin() const { return m_header->m_pLeft; }
    SGroupNode* end() const { return m_header; }
};
struct SGroupInfo {
    unsigned char _pad[0x34];
    CGroupMembers m_members;                           // +0x34
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long m_refCount;                                   // +4
};
struct CCombatDialogueDef : CDefBase {
    unsigned char _pad08[0x60 - 0x08];
    unsigned long RandomCommandSpeech;                 // +0x60
    unsigned long TargetDownSpeech;                    // +0x64
    unsigned long AllyDownSpeech;                      // +0x68
};
template <class T> struct CDefPointer {
    T* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    ~CDefPointer() { if (m_pDef) { if (--m_pDef->m_refCount == 0) m_pDef->Destroy(); } }
    T* operator->() const { return m_pDef; }
};
struct CombatDialogueManager {
    unsigned char SpeechGroupInfoMap[0x0c];
    CDefPointer<CCombatDialogueDef> PDialogueDef;   // +0x0c
    bool SaySpeech(CThingCreatureBase& creature, unsigned long speech);   // 0x008AFC40
};
struct CCombatManager {
    unsigned char _pad00[0x38];
    CombatDialogueManager DialogueManager;             // +0x38
    SGroupInfo* GetValueForThing(CThing* thing);       // 0x006E9360
};

struct CTCLeaderCommand {                              // interface type 0xaf component
    unsigned char _pad[0x0c];
    long m_commandFrame;                               // +0x0c
    long m_commandType;                                // +0x10
};

struct CBaseIntelligentPointer {
    void*               __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00(); virtual void v01();
    virtual void Initialise();                         // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                   // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    CCombatManager* GetCombatManager();                // 0x00856B10
    void SetAnimationName(const CCharString& name);    // 0x008574D0
    CWorld* GetPWorld() const;                         // 0x00692940
};

struct CCreatureAction_GenericLeaderCommand : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_GenericLeaderCommand::Initialise()
{
    bool targetDown = false;
    bool allyDown = false;
    CCharString animation("LEADER_IDLE", -1);
    CThingCreatureBase* leader = PCreature.GetPItem();
    if (leader->HasTC(0xaf)) {
        CTCLeaderCommand* command = (CTCLeaderCommand*)leader->FindTC(0xaf);
        if (PTarget.GetPItem() != 0 && !(PTarget.GetPItem()->m_flags91 & 1) && PTarget.GetPItem()->HasTC(0x263)) {
            CThing* target = PTarget.GetPItem();
            CCombatManager* combat = GetCombatManager();
            CGroupMembers* members = &combat->GetValueForThing(target)->m_members;
            for (SGroupIterator it(members->begin()); it != SGroupIterator(members->end()); it.Increment()) {
                if (IsKnockedDown(it.m_node->m_member))
                    allyDown = true;
            }
            if (IsKnockedDown(PTarget.GetPItem()))
                targetDown = true;
        }
        CCombatManager* manager = GetCombatManager();
        CDefPointer<CCombatDialogueDef> def = manager->DialogueManager.PDialogueDef;
        if (targetDown) {
            animation = "LEADER_CHEER";
            command->m_commandFrame = GetPWorld()->GetWorldFrame();
            command->m_commandType = 0;
            CThingCreatureBase* creature = PCreature.GetPItem();
            CCombatManager* speaker = GetCombatManager();
            speaker->DialogueManager.SaySpeech(*creature, def->TargetDownSpeech);
        } else if (allyDown) {
            animation = "LEADER_DISAPPOINTED";
            command->m_commandFrame = GetPWorld()->GetWorldFrame();
            command->m_commandType = 1;
            CThingCreatureBase* creature = PCreature.GetPItem();
            CCombatManager* speaker = GetCombatManager();
            speaker->DialogueManager.SaySpeech(*creature, def->AllyDownSpeech);
        } else if (GetRandomBelow(GetPWorld(), 100) < 20) {
            animation = "LEADER_GIVE_ORDERS";
            command->m_commandFrame = GetPWorld()->GetWorldFrame();
            command->m_commandType = 2;
            CThingCreatureBase* creature = PCreature.GetPItem();
            CCombatManager* speaker = GetCombatManager();
            speaker->DialogueManager.SaySpeech(*creature, def->RandomCommandSpeech);
        }
    }
    SetAnimationName(animation);
    CCombatActionBase::Initialise();
}
