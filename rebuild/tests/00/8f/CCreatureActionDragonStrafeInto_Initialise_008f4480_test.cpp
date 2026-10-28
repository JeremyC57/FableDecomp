// CCreatureAction_DragonStrafeInto::Initialise() -- animation "STRAFE_INTO", base Initialise, then (mode manager present) push a new CModeDragonStrafe(owner, Timer, Speed, Timer * 0.125, Timer * 0.75) onto the owner's mode manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4480, length 233 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("STRAFE_INTO" @ 0x01288514), SetCombatAnimation 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager), operator new 0x00BFEA1A (0x58 bytes),
// CModeDragonStrafe::CModeDragonStrafe(CThing*, float, float, float, float) 0x008DDFB0, CCountedPointer<CModeBase>::CCountedPointer(CModeBase*)
// 0x00718A00, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0.
// Shape: as CCreatureAction_DragonBurninateInto::Initialise 0x008F4320 (in-chunk): the owner argument goes through the inline
// `operator CThing*()`, so its GetPItem call runs before the members are loaded and pushed; the two Timer fractions (0.125 @
// 0x0123095C, 0.75 @ 0x01238174) are named float locals computed before the allocation; the manager stays in ebx.
// Layout: Timer / Speed at retail +0x114 / +0x118 (PDB +0x12c / +0x130).
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public: };

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*    m_pObject;                            // +0
    void* m_pCount;                             // +4
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeDragonStrafe : CModeBase {
    unsigned char _pad[0x58];
    CModeDragonStrafe(CThing* owner, float timer, float speed, float rampUp, float rampDown);   // 0x008DDFB0
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_DragonStrafeInto : public CCombatActionBase {
public:
    float Timer;                                // +0x114 (PDB +0x12c)
    float Speed;                                // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_DragonStrafeInto::Initialise()
{
    {
        CCharString name("STRAFE_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31)) {
        float rampUp = Timer * 0.125f;
        float rampDown = Timer * 0.75f;
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeDragonStrafe(PCreature, Timer, Speed, rampUp, rampDown)));
    }
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static char g_anim[32]; static int g_strings = 0, g_baseInits = 0, g_adds = 0, g_pitems = 0;
static CModeBase* g_added = 0; static const void* g_managerThis = 0;
static CThing* g_owner = 0; static float g_args[4];
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; ++g_strings; (void)length; }
CCharString::~CCharString() { --g_strings; }
void CCombatActionBase::SetCombatAnimation(const CCharString& name) { strcpy(g_anim, (const char*)name.PStringData); }
void CCombatActionBase::Initialise() { ++g_baseInits; }
CThing* CBaseIntelligentPointer::GetPItem() const { ++g_pitems; return m_pItem; }
template<> CCountedPointer<CModeBase>::CCountedPointer(CModeBase* object) { m_pObject = object; m_pCount = 0; }
template<> CCountedPointer<CModeBase>::CCountedPointer(const CCountedPointer<CModeBase>& other) { m_pObject = other.m_pObject; m_pCount = other.m_pCount; }
CModeDragonStrafe::CModeDragonStrafe(CThing* owner, float timer, float speed, float rampUp, float rampDown)
{ g_owner = owner; g_args[0] = timer; g_args[1] = speed; g_args[2] = rampUp; g_args[3] = rampDown; }
bool CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase> mode) { ++g_adds; g_added = mode.m_pObject; g_managerThis = this; return true; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    CInterfaceMap::SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
static CThing g_dragon;
static CTCCreatureModeManager g_modes;
static CInterfaceMap::SEntry g_entries[2];
int main()
{
    g_entries[0].type = 0x31; g_entries[0].component = (CTCBase*)&g_modes;
    g_entries[1].type = 0x40; g_entries[1].component = 0;
    g_dragon.m_interfaces.first = g_entries; g_dragon.m_interfaces.last = g_entries + 2;
    g_dragon.m_presentBits[0x31 >> 5] = 1UL << (0x31 & 31);
    CCreatureAction_DragonStrafeInto a;
    a.PCreature.m_pItem = &g_dragon;
    a.Timer = 8.0f; a.Speed = 3.5f;
    a.Initialise();
    CHECK(strcmp(g_anim, "STRAFE_INTO") == 0 && g_strings == 0 && g_baseInits == 1);
    CHECK(g_adds == 1 && g_added != 0 && g_managerThis == &g_modes && g_owner == &g_dragon && g_pitems == 2);
    CHECK(g_args[0] == 8.0f && g_args[1] == 3.5f && g_args[2] == 1.0f && g_args[3] == 6.0f);
    delete (CModeDragonStrafe*)g_added;
    g_dragon.m_presentBits[0x31 >> 5] = 0;          // no mode manager: nothing added
    a.Initialise();
    CHECK(g_adds == 1 && g_baseInits == 2 && g_pitems == 3);
    if (fails == 0) printf("008f4480_TEST PASS\n");
    return fails;
}