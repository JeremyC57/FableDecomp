// Grade: DIFFER(1367v1367) -- same length; retail keeps the own gossip component in a frame word (ours ebp) and a zero register differs (ours xor ebx), the message query's locals are laid out in another order (filter ctor result stored straight into the query), and the 5-bit kind test is xor/test (ours and/cmp)
// CAIStateGroup_BanditGossip::Update() -- find (or advertise for) a gossip partner, walk to it, sheathe weapons, speak one of three random bandit lines, unsheathe and terminate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090E5D0, 1367 bytes.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator=(const char* text);            // 0x0099EFE0
    CCharString& operator=(const CCharString& other);    // 0x0099EFB0
    bool operator!=(const char* text) const;             // 0x0099E960
};
struct CThingGraphic;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingGraphic* PGraphic;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
long GFGetCurrentFrame();                       // 0x0049D870
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
    CTCBase* AddTC(const CCharString& name, long a, long b);   // 0x004C9D60
    void RemoveTC(long interfaceType);          // 0x004C9840
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7E0
    virtual void v00();
    __forceinline CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
struct CIntelligentPointerSpeaker : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointerSpeaker(const CIntelligentPointerCreature& other);   // 0x0050F410 (out of line)
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
bool __fastcall IsThingCarryingWeapon(const CThing* thing);   // 0x006A1540
bool __fastcall IsThingDead(CBaseIntelligentPointer& pointer);   // 0x007304C0
struct CTCBanditGossip {                        // interface 0xC1
    unsigned char _pad00[0x0c];
    CIntelligentPointerCreature Partner;        // +0x0c
    bool Initiator;                             // +0x14
};
struct CTCNotSolid {                            // interface 0x4D
    void AddThingNotSolidTo(CThing* thing);     // 0x00770FA0
};
struct CThingFilter_GossipPartner {
    CThing* Creature;
    unsigned char _pad[0x10];
    CThingFilter_GossipPartner(CThing* creature);   // 0x0090C870
};
struct CSphere { C3DVector Centre; float Radius; };
struct CMessageInSphere { const CSphere* Sphere; };
struct CMessageFrom { CThingFilter_GossipPartner* Filter; };
struct CMessageQuery {
    const long* Type;
    CMessageInSphere* InSphere;
    CMessageFrom* From;
};
struct CMessageEvent {
    unsigned char _pad00[0x48];
    CBaseIntelligentPointer Sender;             // +0x48
};
struct CMessageManager {
    CMessageEvent* FindMessage(const CMessageQuery& query);   // 0x0090EBE0
    void RemoveMessage(const CMessageEvent* message);         // 0x00493B5B
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2,
                              float a, float b, long priority);   // 0x00687540
};
struct CWorld {
    unsigned char _pad00[0x60];
    CMessageManager* PMessageManager;           // +0x60
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];
    ~CCreatureActionBase();                     // 0x0062E900 (~CCombatActionBase)
};
struct CCreatureAction_SheatheWeapons : CCreatureActionBase {
    CCreatureAction_SheatheWeapons(CThingCreatureBase& creature);   // 0x008CC370
};
struct CCreatureAction_UnsheatheWeapons : CCreatureActionBase {
    CCreatureAction_UnsheatheWeapons(CThingCreatureBase& creature); // 0x008B8590
};
extern long g_FramesPerSecond;                  // 0x01375550
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float proximity, bool& moving,
                                       long anim, bool lookAround, bool b7, bool b8, bool b10, float speedScale);   // 0x0097E380
    long AIAction_SpeakToTarget(long repeat, CIntelligentPointerSpeaker target, CCharString text, long priority, bool wait);   // 0x00981410
    __forceinline unsigned long Random()
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        return *seed = GFROR13(*seed);
    }
};
struct CAIStateGroup_BanditGossip : CAIStateGroupBase {
    unsigned char _pad14[0x18 - 0x14];
    bool Speaking;                              // +0x18
    unsigned char _pad19[0x24 - 0x19];
    bool WeaponsSheathed;                       // +0x24
    void Update();
};

void CAIStateGroup_BanditGossip::Update()
{
    WeaponsSheathed = false;
    Speaking = false;
    CIntelligentPointerCreature partner(0);
    CTCBanditGossip* gossip = 0;
    if (GetCreature()->TestTC(0xc1))
        gossip = (CTCBanditGossip*)GetCreature()->GetTC(0xc1);
    bool initiator;
    CThingFilter_GossipPartner filter(GetCreature());
    CMessageFrom from = { &filter };
    CSphere sphere;
    sphere.Centre = GetCreature()->CThing::GetPosition();
    sphere.Radius = 16.0f;
    CMessageInSphere inSphere = { &sphere };
    long type = 0x2c;
    CMessageQuery query = { &type, &inSphere, &from };
    CMessageEvent* message = GetPWorld()->PMessageManager->FindMessage(query);
    if (message) {
        CThing* sender = 0;
        if (message->Sender.GetPItem() && (*(unsigned long*)((char*)message->Sender.GetPItem() + 0x6c) & 0x1f) == 0x1f)
            sender = message->Sender.GetPItem();
        partner = CIntelligentPointerCreature(sender);
        GetPWorld()->PMessageManager->RemoveMessage(message);
        if (!IS_THING_ALIVE(partner)) {
            Terminate();
            return;
        }
        CTCBanditGossip* partnerGossip = 0;
        if (partner.GetPItem()->TestTC(0xc1))
            partnerGossip = (CTCBanditGossip*)partner.GetPItem()->GetTC(0xc1);
        gossip->Partner.SetPItem(partner.GetPItem());
        gossip->Initiator = true;
        partnerGossip->Partner = CIntelligentPointerCreature(GetCreature());
        partnerGossip->Initiator = false;
        initiator = false;
    } else {
        CThingCreatureBase* creature = GetCreature();
        GetPWorld()->PMessageManager->AddMessage(0x2c, g_FramesPerSecond * 4, &creature->GetPosition(), creature, 0, 16.0f, 16.0f, 0);
        long endFrame = GFGetCurrentFrame() + g_FramesPerSecond * 4;
        for (;;) {
            if (IS_THING_ALIVE(gossip->Partner)) {
                partner = gossip->Partner;
                partner.GetPItem();
                initiator = true;
                break;
            }
            if (AIAction_DoNothing() == 1 || GFGetCurrentFrame() > endFrame) {
                Terminate();
                return;
            }
        }
    }
    if (!partner.GetPItem()) {
        Terminate();
        return;
    }
    if (AIAction_LookAtThing(1, CIntelligentPointerConstThing(partner), 0x11, 0.0027777778f) != 2) {
        Terminate();
        return;
    }
    long moved;
    bool moving;
    do
        moved = AIAction_MoveToThingProximity(2, CIntelligentPointerConstThing(partner), 2.0f, moving, 3, true, false, true, true, 0.0f);
    while (moved == 4);
    if (moved != 2) {
        Terminate();
        return;
    }
    if (IsThingCarryingWeapon(GetCreature())) {
        if (AIAction_PerformActionAndWait(&CCreatureAction_SheatheWeapons(*GetCreature()), false) != 2) {
            Terminate();
            return;
        }
        WeaponsSheathed = true;
    }
    if (IsThingDead(partner)) {
        Terminate();
        return;
    }
    CTCNotSolid* notSolid;
    if (GetCreature()->HasTC(0x4d))
        notSolid = (CTCNotSolid*)GetCreature()->GetTC(0x4d);
    else
        notSolid = (CTCNotSolid*)GetCreature()->AddTC(CCharString("CTCSolidToThing"), 0, 0);
    notSolid->AddThingNotSolidTo(partner.GetPItem());
    const char* lines[3] = { "VILLAGER_INTERACTION_BANDIT_PUSHING", "VILLAGER_INTERACTION_BANDIT_HOLDING", "VILLAGER_INTERACTION_BANDIT_TELLING_OFF" };
    CCharString line(lines[Random() % 3]);
    Speaking = true;
    AIAction_SpeakToTarget(1, CIntelligentPointerSpeaker(partner), line, 100, initiator);
    Speaking = false;
    GetCreature()->RemoveTC(0x4d);
    if (WeaponsSheathed && AIAction_PerformActionAndWait(&CCreatureAction_UnsheatheWeapons(*GetCreature()), false) == 2)
        WeaponsSheathed = false;
    Terminate();
}
