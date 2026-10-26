// Behaviour test for CTCVillage::~CTCVillage (0x0065B598): every live villager with a village-member component gets
// SetPVillage(0), dead villagers are skipped, the village def is released (destroyed at refcount 0, pointer cleared),
// the member containers are destroyed in reverse declaration order and the CTCBase destructor runs last.
#include <cstdio>

class CTCBase;
class CTCVillage;
static int g_order[32];
static int g_n = 0;
static int g_setCalls = 0, g_setNonNull = 0, g_destroyed = 0;

class CThing {
public:
    bool alive;
    bool hasMember;
    CTCBase* member;
    bool HasTC(long type) const { return type == 35 && hasMember; }
    CTCBase* GetTC(long) { return member; }
};
class CThingAICreature : public CThing {};
bool IS_THING_ALIVE(CThing* const& thing) { return thing != 0 && thing->alive; }

class CTCVillageMember { public:
    CTCVillage* village;
    void SetPVillage(CTCVillage* v) { ++g_setCalls; if (v) ++g_setNonNull; village = v; }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct SPersonNode { SPersonNode* _M_next; SPersonNode* _M_prev; CBaseIntelligentPointer _M_data; };

template <int ID> struct CMemberList { void* _M_node; ~CMemberList() { g_order[g_n++] = ID; } };
struct CCreaturePointerList { SPersonNode* _M_node; ~CCreaturePointerList() { g_order[g_n++] = 2; } };

struct CDefPointee {
    long RefCount;
    virtual void Destroy() { ++g_destroyed; }
    virtual ~CDefPointee() {}
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CVillageDefPointer {
    CDefPointee* Object;
    ~CVillageDefPointer() { if (Object) { Object->Release(); Object = 0; } g_order[g_n++] = 100; }
};

class CTCBase {
public:
    virtual ~CTCBase() { g_order[g_n++] = 999; }
};

class CTCVillage : public CTCBase { public:
    CMemberList<1>  VillageBuildings;
    CCreaturePointerList VillagePeople;
    CMemberList<3>  VillageTorches;
    CMemberList<16> PlayersCrimes;
    CMemberList<18> VillageDummies;
    CVillageDefPointer PDef;
    virtual ~CTCVillage();
};

CTCVillage::~CTCVillage()
{
    for (SPersonNode* node = VillagePeople._M_node->_M_next; node != VillagePeople._M_node; node = node->_M_next) {
        CThingAICreature* person = (CThingAICreature*)node->_M_data.GetPItem();
        if (IS_THING_ALIVE(person)) {
            CTCVillageMember* member = 0;
            if (person->HasTC(35))
                member = (CTCVillageMember*)person->GetTC(35);
            member->SetPVillage(0);
        }
    }
}

int main()
{
    CTCVillageMember m1, m2;
    m1.village = (CTCVillage*)&m1; m2.village = (CTCVillage*)&m2;
    CThingAICreature p1, p2, p3;
    p1.alive = true;  p1.hasMember = true; p1.member = (CTCBase*)&m1;
    p2.alive = false; p2.hasMember = true; p2.member = (CTCBase*)&m2;
    p3.alive = true;  p3.hasMember = true; p3.member = (CTCBase*)&m2;
    SPersonNode head, n1, n2, n3;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n1._M_data.m_pItem = &p1; n2._M_data.m_pItem = &p2; n3._M_data.m_pItem = &p3;
    CDefPointee* def = new CDefPointee;
    def->RefCount = 1;
    {
        CTCVillage* v = new CTCVillage;
        v->VillagePeople._M_node = &head;
        v->PDef.Object = def;
        CTCBase* base = v;
        delete base;
    }
    int ok = g_setCalls == 2 && g_setNonNull == 0 && m1.village == 0 && m2.village == 0 && g_destroyed == 1;
    static const int expect[] = { 100, 18, 16, 3, 2, 1, 999 };
    ok = ok && g_n == 7;
    for (int i = 0; ok && i < 7; ++i)
        if (g_order[i] != expect[i]) ok = 0;
    delete def;
    if (!ok) { std::printf("0065b598_TEST FAIL\n"); return 1; }
    std::printf("0065b598_TEST PASS\n");
    return 0;
}