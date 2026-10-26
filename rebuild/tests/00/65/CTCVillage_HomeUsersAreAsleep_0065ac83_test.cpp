// Behaviour test for 0065ac83: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCVillage::HomeUsersAreAsleep(CThingBuilding* home) -- true when the building has at least one living villager calling it home and every such villager is in creature mode 13 (asleep)
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
class CTCBase;
class CBaseObject;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
class CThingBuilding : public CThing { public: };

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    void* __vftable;
    CBaseObject* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing);   // 0x004AB8F0

class CThingAICreature : public CThing { public:
    unsigned char _pad_0x4c[0x168 - 0x4c];
    CBaseIntelligentPointer HomeBuilding;       // +0x168 CIntelligentPointer<CThingBuilding>
    CThingBuilding* GetHomeBuilding() const { return (CThingBuilding*)HomeBuilding.GetPItem(); }
};

class CTCCreatureModeManager { public:
    class CModeBase { public:
        virtual void v00();
        virtual long GetModeType() const;       // slot 1
    };
    CModeBase& GetCurrentMode() const;          // 0x0073A6E0
};

struct SPeopleNode {                            // STLport list node of CIntelligentPointer<CThingAICreature>
    SPeopleNode*            _M_next;
    SPeopleNode*            _M_prev;
    CBaseIntelligentPointer _M_data;            // +0x08
};
struct CPeopleList {                            // retail STLport list: the head node pointer
    SPeopleNode* _M_node;
    SPeopleNode* begin() const { return _M_node->_M_next; }
    SPeopleNode* end() const { return _M_node; }
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x2c];
    CPeopleList   VillagePeople;                // +0x2c
    bool HomeUsersAreAsleep(CThingBuilding* home);
};

bool CTCVillage::HomeUsersAreAsleep(CThingBuilding* home)
{
    bool usersFound = false;
    for (SPeopleNode* it = VillagePeople.begin(); it != VillagePeople.end(); it = it->_M_next) {
        CBaseIntelligentPointer& person = it->_M_data;
        if (IS_THING_ALIVE(person)) {
            if (((CThingAICreature*)person.GetPItem())->GetHomeBuilding() == home) {
                usersFound = true;
                CTCCreatureModeManager* modes;
                person.GetPItem()->GetInterface(&modes, 0x31);
                if (modes->GetCurrentMode().GetModeType() != 0xd)
                    return false;
            }
        }
    }
    return usersFound;
}

// ---- stubs and driver ----
bool CBitset::test(unsigned long position) const { return position == 0x31; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)PData; }
static CThing* g_dead;
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing) { return thing.PData != 0 && (CThing*)thing.PData != g_dead; }
struct CTestMode : CTCCreatureModeManager::CModeBase { long type; };
void CTCCreatureModeManager::CModeBase::v00() {}
long CTCCreatureModeManager::CModeBase::GetModeType() const { return static_cast<const CTestMode*>(this)->type; }
static CTestMode g_modes[3];
static CTCCreatureModeManager g_managers[3];
CTCCreatureModeManager::CModeBase& CTCCreatureModeManager::GetCurrentMode() const { return g_modes[this - g_managers]; }

static CThingAICreature g_people[3];
static CInterfaceMap::SEntry g_entries[3];
static SPeopleNode g_head, g_nodes[3];

int main()
{
    static CThingBuilding home, otherHome;
    static CTCVillage village;
    g_head._M_next = &g_nodes[0];
    for (int i = 0; i < 3; ++i) {
        g_nodes[i]._M_next = i < 2 ? &g_nodes[i + 1] : &g_head;
        g_nodes[i]._M_data.PData = (CBaseObject*)&g_people[i];
        g_entries[i].type = 0x31; g_entries[i].component = (CTCBase*)&g_managers[i];
        g_people[i].m_interfaces.first = &g_entries[i]; g_people[i].m_interfaces.last = &g_entries[i] + 1;
        g_modes[i].type = 0xd;
    }
    village.VillagePeople._M_node = &g_head;
    g_people[0].HomeBuilding.PData = (CBaseObject*)&home;
    g_people[1].HomeBuilding.PData = (CBaseObject*)&otherHome;
    g_people[2].HomeBuilding.PData = (CBaseObject*)&home;
    if (!village.HomeUsersAreAsleep(&home)) { std::printf("0065ac83_TEST FAIL (asleep)\n"); return 1; }
    g_modes[1].type = 3;                                               // awake, but lives elsewhere
    if (!village.HomeUsersAreAsleep(&home)) { std::printf("0065ac83_TEST FAIL (other home)\n"); return 1; }
    g_modes[2].type = 3;                                               // a home user is awake
    if (village.HomeUsersAreAsleep(&home)) { std::printf("0065ac83_TEST FAIL (awake)\n"); return 1; }
    g_dead = (CThing*)&g_people[2];                                     // ... but dead
    if (!village.HomeUsersAreAsleep(&home)) { std::printf("0065ac83_TEST FAIL (dead)\n"); return 1; }
    static CThingBuilding empty;
    if (village.HomeUsersAreAsleep(&empty)) { std::printf("0065ac83_TEST FAIL (nobody)\n"); return 1; }
    std::printf("0065ac83_TEST PASS\n");
    return 0;
}