// Behaviour test for 008718b0: src/008718b0.cpp copied verbatim; the definition lookups, object creation, carrying
// component and AddTC stubbed so the carry gate, the hand slot lookup, the object created for the main player at the
// thing's position and placed in the slot, and the optional destroy-on-finish component are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CActionEventAddObject::Dispatch(const CCreatureActionBase&) -- when the creature can carry: resolve the hand slot and the object def by name, create the object at the creature's position for the main player and put it in that carry slot, optionally tagging it to be destroyed when the action finishes
// Retry 10b (combined accessor sweeps, FIXES B-4/B-15): the player manager and the script thing are read through inline getters (`g_pMainGameComponent->GetPlayerManager()`, `GetThing()`); plain member reads give the parked register difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008718B0, length 339 bytes (`ret 4`; the action argument is unused).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 70 = bit 6 of the bitset dword at +0x28 =
//          CTCCarrying), CPlayerManager::GetMainPlayerNumber @ 0x00449700 (manifest "CWorld::DrawGetEnvironment" is
//          propagated; g_pMainGameComponent @ 0x013B86A0 +0x1c), CThing::GetPosition @ 0x004C73D0, CGameDefinitionManager::Get
//          @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410 (x2), CCharString(const char*, long)
//          @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("" @ 0x0122D70E, "CTCDestroyThingOnActionFinish" @ 0x0123E144), the
//          thing-object creator @ 0x00703210 (__fastcall: def in ecx, position in edx; player, orientation, flags, name on the
//          stack), CTCCarrying::PutThingInCarrySlot @ 0x006A1F80, CThing::AddTC @ 0x004C9D60.
// Shape: everything sits under the carrying-component hit (the out pointer is spilled and reloaded for the carry call);
//          the position is copied through the C3DVector user copy constructor and the orientation is the default
//          CRightHandedSet (0,0,1 / 0,1,0) stored after the position; the hand-slot index is tested first, and the rest of
//          the body is a __forceinline helper so VC7.1 pushes ebp inside that branch; both strings are implicit conversions
//          (built before the pushes, `lea`-addressed); the created thing's alive test is `p && !(p->flags & 1)`.
// Layout: Ego_r.pdb CActionEventAddObject (32 bytes): CActionEventBase {vptr, Finished +4, ActivationFrame +8, PThing +0xc,
//          PCameraManager +0x10}, Object +0x14, Hand +0x18, DestroyOnExit +0x1c.
class CTCBase;
class CCreatureActionBase;
class CGameCameraManager;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};
class CRightHandedSet { public:
    C3DVector First;
    C3DVector Second;
    CRightHandedSet() : First(0.0f, 0.0f, 1.0f), Second(0.0f, 1.0f, 0.0f) { }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool activate, void* params);   // 0x004C9D60
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
class CTCCarrying { public:                            // interface 70
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();          // 0x0044C6B0
};
class CPlayerManager { public:
    long GetMainPlayerNumber();                 // 0x00449700
};
class CMainGameComponent { public:
    unsigned char _pad[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
    CPlayerManager* GetPlayerManager() const { return PlayerManager; }
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

CThing* __fastcall CreateThingObject(long defIndex, C3DVector* position, long player, CRightHandedSet* orientation,
    unsigned long flags, const CCharString& scriptName);   // 0x00703210

struct CActionEventAddObject {
    virtual ~CActionEventAddObject();
    bool Finished;                              // +0x04
    long ActivationFrame;                       // +0x08
    CThing* PThing;                             // +0x0c
    CThing* GetThing() const { return PThing; }
    CGameCameraManager* PCameraManager;         // +0x10
    CCharString Object;                         // +0x14
    CCharString Hand;                           // +0x18
    bool DestroyOnExit;                         // +0x1c
    __forceinline void CreateObjectInHand(CTCCarrying* carrying, long player, C3DVector& position,
        CRightHandedSet& orientation, long slot);
    virtual void Dispatch(const CCreatureActionBase& action);
};

// Resolve the object def, create it for the player at the creature's position and hand it over.
__forceinline void CActionEventAddObject::CreateObjectInHand(CTCCarrying* carrying, long player, C3DVector& position,
    CRightHandedSet& orientation, long slot)
{
    long objectDef = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(Object);
    if (objectDef > 0) {
        CThing* thing = CreateThingObject(objectDef, &position, player, &orientation, 0, "");
        if (thing != 0 && !(thing->m_flags91 & 1)) {
            carrying->PutThingInCarrySlot(thing, slot, true);
            if (DestroyOnExit)
                thing->AddTC("CTCDestroyThingOnActionFinish", false, 0);
        }
    }
}

void CActionEventAddObject::Dispatch(const CCreatureActionBase& action)
{
    CTCCarrying* carrying;
    if (GetThing()->GetInterface(&carrying, 70)) {
        long player = g_pMainGameComponent->GetPlayerManager()->GetMainPlayerNumber();
        C3DVector position = GetThing()->GetPosition();
        CRightHandedSet orientation;
        long slot = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(Hand);
        if (slot > 0)
            CreateObjectInHand(carrying, player, position, orientation, slot);
    }
}
// ---- end source ----

CCharString::CCharString(const char* t, long) : PStringData((void*)t) {}
CCharString::~CCharString() {}
CActionEventAddObject::~CActionEventAddObject() {}
static C3DVector g_pos(4, 5, 6);
const C3DVector& CThing::GetPosition() { return g_pos; }
static int g_addTC = 0; static char g_addTCName[64];
CTCBase* CThing::AddTC(const CCharString& name, bool activate, void*) { ++g_addTC; strcpy(g_addTCName, (const char*)name.PStringData); return 0; }
static int g_puts = 0; static CThing* g_putThing = 0; static long g_putSlot = 0;
void CTCCarrying::PutThingInCarrySlot(CThing* t, long slot, bool imm) { if (imm) { ++g_puts; g_putThing = t; g_putSlot = slot; } }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& n)
{ const char* s = (const char*)n.PStringData; if (strcmp(s, "HAND_LEFT") == 0) return 2; if (strcmp(s, "OBJECT_TORCH") == 0) return 17; return 0; }
static CGameDefinitionManager g_gdm;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return g_gdm; }
long CPlayerManager::GetMainPlayerNumber() { return 1; }
static CPlayerManager g_pm; static CMainGameComponent g_mgc; CMainGameComponent* g_pMainGameComponent = &g_mgc;
static CThing g_created; static long g_createDef = 0, g_createPlayer = -1; static C3DVector g_createPos;
CThing* __fastcall CreateThingObject(long def, C3DVector* pos, long player, CRightHandedSet* o, unsigned long flags, const CCharString&)
{ g_createDef = def; g_createPos = *pos; g_createPlayer = player; return (o && o->First.Z == 1.0f && flags == 0) ? &g_created : 0; }
static CTCCarrying g_carrying;
static CInterfaceMap::SEntry g_entries[1] = { { 70, (CTCBase*)&g_carrying } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= 70 ? first : last; }
int main()
{
    g_mgc.PlayerManager = &g_pm;
    static CThing thing; memset(&thing, 0, sizeof(thing)); thing.m_interfaces.first = g_entries; thing.m_interfaces.last = g_entries + 1;
    memset(&g_created, 0, sizeof(g_created));
    static unsigned char raw[sizeof(CActionEventAddObject)]; memset(raw, 0, sizeof(raw));
    CActionEventAddObject* e = (CActionEventAddObject*)raw; e->PThing = &thing;
    e->Object.PStringData = (void*)"OBJECT_TORCH"; e->Hand.PStringData = (void*)"HAND_LEFT";
    const CCreatureActionBase& action = *(const CCreatureActionBase*)raw;
    e->CActionEventAddObject::Dispatch(action); CHECK(g_puts == 0);              // cannot carry
    thing.m_presentBits[70 >> 5] |= 1UL << (70 & 31);
    e->CActionEventAddObject::Dispatch(action);
    CHECK(g_puts == 1 && g_putThing == &g_created && g_putSlot == 2 && g_createDef == 17 && g_createPlayer == 1);
    CHECK(g_createPos.X == 4 && g_createPos.Y == 5 && g_createPos.Z == 6 && g_addTC == 0);
    e->DestroyOnExit = true;
    e->CActionEventAddObject::Dispatch(action);
    CHECK(g_puts == 2 && g_addTC == 1 && strcmp(g_addTCName, "CTCDestroyThingOnActionFinish") == 0);
    e->Hand.PStringData = (void*)"HAND_NONE";
    e->CActionEventAddObject::Dispatch(action); CHECK(g_puts == 2);              // no slot
    if (fails == 0) printf("008718b0_TEST PASS\n"); else printf("008718b0_TEST FAIL\n");
    return fails;
}