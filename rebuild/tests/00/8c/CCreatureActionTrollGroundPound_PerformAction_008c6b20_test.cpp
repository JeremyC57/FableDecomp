// Behaviour test for 008c6b20: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CCreatureAction_TrollGroundPound::PerformAction()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
#define snprintf _snprintf
#else
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static char g_log[4096];
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n  log=%s\n", __LINE__, #c, g_log); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_TrollGroundPound::PerformAction() -- with the troll's ground-pound sub-def: create its trap object at the
// troll (owned by the troll's player) and make the trap spare the troll and fire at once; create its second object at the
// troll and hand its driver the troll; and, for a positive obstruction radius, create an obstruction there and pass its
// driver the def's +0x48 value
// Leaf: ?PerformAction@CCreatureAction_TrollGroundPound
// Layout: CThing interface bitset +0x20, map +0x44, PTCDriver +0x5c (the cached driver component), def +0x70, player +0x90.
// Shape: GetSubDef 0x008C79B0 sits in this TU (same-TU noinline stand-in), as does the LowerBound stand-in (0x0040F020),
//   so the key word, the def word and the out pointers share frame slots; each position is a block-scoped copy.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
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
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
struct CTrollGroundPoundDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long          RefCount;                           // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long          TrapDefIndex;                       // +0x3c
    long          ObjectDefIndex;                     // +0x40
    float         ObstructionRadius;                  // +0x44
    long          ObstructionValue;                   // +0x48
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CTrollGroundPoundDef>* out);   // 0x008C79B0
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTrollGroundPoundDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
void* FindSubDef(CDefClassBase* def);
#pragma auto_inline(off)
#pragma auto_inline(on)
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                  // +0x20 interface-present bitset
    CInterfaceMap  m_interfaces;                      // +0x44
    unsigned char  _pad4c[0x5c - 0x4c];
    CTCBase*       PTCDriver;                         // +0x5c the cached driver component
    unsigned char  _pad60[0x70 - 0x60];
    CDefClassBase* Def;                               // +0x70
    unsigned char  _pad74[0x90 - 0x74];
    char           PlayerNumber;                      // +0x90
    CDefClassBase* GetDef() const { return Def; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    template <class T> __forceinline bool GetDriver(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = (T*)PTCDriver;
            return true;
        }
        return false;
    }
};
class CRightHandedSet;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210
__forceinline CThing* CreateThingObject(const long& defIndex, const C3DVector& position, const CThing* creature, const CCharString& name)
{
    return CreateThingObject(defIndex, &position, creature->PlayerNumber, 0, 0, name);
}
CThing* __fastcall CreateThing(long defIndex, const C3DVector* position);             // 0x0080C460
CThing* __fastcall CreateObstruction(const C3DVector* a, const C3DVector* b, float radius);   // 0x007FD4B0
class CTCTrapBase { public:                                  // interface 0x7D
    void SetThingAsImmune(CThing* thing);             // 0x0078839A
    void ManualTrigger();                             // 0x00788388
};
struct CTCDriver41 {                                  // driver of interface 0x41
    void SetOwner(CThing* owner);                     // 0x0080CBF0
};
struct CTCDriverED {                                  // driver of interface 0xED
    void SetValue(long value);                        // 0x007FD3D0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_TrollGroundPound : public CCreatureActionBase { public:
    void PerformAction();
};

void CCreatureAction_TrollGroundPound::PerformAction()
{
    CDefPointer<CTrollGroundPoundDef> def;
    if (GetCreature()->GetDef()->GetSubDef(&def)) {
        if (def.Object->TrapDefIndex) {
            C3DVector position = GetCreature()->GetPosition();
            CThing* trap = CreateThingObject(def.Object->TrapDefIndex, position, GetCreature(), "");
            CTCTrapBase* trapBase;
            trap->GetInterface(&trapBase, 0x7d);
            trapBase->SetThingAsImmune(GetCreature());
            trapBase->ManualTrigger();
        }
        if (def.Object->ObjectDefIndex) {
            C3DVector position = GetCreature()->GetPosition();
            CThing* object = CreateThing(def.Object->ObjectDefIndex, &position);
            CTCDriver41* driver;
            object->GetDriver(&driver, 0x41);
            driver->SetOwner(GetCreature());
        }
        if (def.Object->ObstructionRadius > 0.0001f) {
            C3DVector position = GetCreature()->GetPosition();
            CThing* obstruction = CreateObstruction(&position, &position, def.Object->ObstructionRadius);
            if (obstruction) {
                CTCDriverED* driver;
                obstruction->GetDriver(&driver, 0xed);
                driver->SetValue(def.Object->ObstructionValue);
            }
        }
    }
}

// ---- stubs ----
static void logf(const char* fmt, long a = 0, long b = 0, long c = 0) { char t[256]; snprintf(t, sizeof t, fmt, a, b, c); strcat(g_log, t); strcat(g_log, ";"); }
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; }
CCharString::~CCharString() { }
static CTrollGroundPoundDef g_def;
void CTrollGroundPoundDef::v00() { }
void CTrollGroundPoundDef::Destroy() { logf("Destroy"); }
static bool g_hasDef;
void* FindDef_CDefClassBase_GetSubDef_0() { if (!g_hasDef) return 0; ++g_def.RefCount; return &g_def; }
void* FindSubDef(CDefClassBase* def) { return 0; }
static C3DVector g_position = { 1.0f, 2.0f, 3.0f };
const C3DVector& CThing::GetPosition() const { logf("GetPosition"); return g_position; }
static CThing* g_trap; static CThing* g_object; static CThing* g_obstruction; static CThing* g_creature;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set, unsigned long flags, const CCharString& name)
{
    logf("CreateThingObject(%ld,%ld,%ld)", defIndex, player, (long)(position->X == 1.0f && position->Z == 3.0f && set == 0 && flags == 0 && ((const char*)name.PStringData)[0] == 0));
    return g_trap;
}
CThing* __fastcall CreateThing(long defIndex, const C3DVector* position) { logf("CreateThing(%ld,%ld)", defIndex, (long)(position->Y == 2.0f)); return g_object; }
CThing* __fastcall CreateObstruction(const C3DVector* a, const C3DVector* b, float radius) { logf("CreateObstruction(%ld,%ld)", (long)(a == b && a->X == 1.0f), (long)(radius * 100.0f)); return g_obstruction; }
static CTCTrapBase g_trapBase; static CTCDriver41 g_driver41; static CTCDriverED g_driverED;
// the miss paths of the untested lookups call through never-written locals: the stubs do not touch `this`
void CTCTrapBase::SetThingAsImmune(CThing* thing) { logf("SetThingAsImmune(%ld,%ld)", (long)(this == &g_trapBase), (long)(thing == g_creature)); }
void CTCTrapBase::ManualTrigger() { logf("ManualTrigger(%ld)", (long)(this == &g_trapBase)); }
void CTCDriver41::SetOwner(CThing* owner) { logf("SetOwner(%ld,%ld)", (long)(this == &g_driver41), (long)(owner == g_creature)); }
void CTCDriverED::SetValue(long value) { logf("SetValue(%ld,%ld)", (long)(this == &g_driverED), value); }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CCreatureActionBase::v00() { }

int main()
{
    static unsigned char creatureBytes[0x100]; memset(creatureBytes, 0, sizeof creatureBytes); g_creature = (CThing*)creatureBytes;
    static unsigned char trapBytes[0x100]; memset(trapBytes, 0, sizeof trapBytes); g_trap = (CThing*)trapBytes;
    static unsigned char objectBytes[0x100]; memset(objectBytes, 0, sizeof objectBytes); g_object = (CThing*)objectBytes;
    static unsigned char obstructionBytes[0x100]; memset(obstructionBytes, 0, sizeof obstructionBytes); g_obstruction = (CThing*)obstructionBytes;
    static CInterfaceMap::SEntry trapEntries[3] = { {3, (CTCBase*)0x11}, {0x7d, (CTCBase*)&g_trapBase}, {0, (CTCBase*)0} };
    g_trap->m_interfaces.first = trapEntries; g_trap->m_interfaces.last = trapEntries + 2;
    g_object->PTCDriver = (CTCBase*)&g_driver41; g_obstruction->PTCDriver = (CTCBase*)&g_driverED;
    static CDefClassBase defClass; g_creature->Def = &defClass; g_creature->PlayerNumber = -2;
    CCreatureAction_TrollGroundPound* action = (CCreatureAction_TrollGroundPound*)calloc(1, sizeof(CCreatureAction_TrollGroundPound));
    action->PCreature.m_pItem = g_creature;

    // 1. no sub-def: nothing
    g_hasDef = false; g_log[0] = 0;
    action->PerformAction();
    CHECK(strcmp(g_log, "") == 0);

    // 2. def with nothing set: only the def lookup and its release
    g_hasDef = true; g_def.RefCount = 10; g_def.TrapDefIndex = 0; g_def.ObjectDefIndex = 0; g_def.ObstructionRadius = 0.0f; g_log[0] = 0;
    action->PerformAction();
    CHECK(strcmp(g_log, "") == 0);
    CHECK(g_def.RefCount == 10);

    // 3. trap: created at the creature's position for its (signed) player, made immune to the creature, triggered
    g_def.TrapDefIndex = 501; g_trap->m_presentBits[3] = 1UL << 29; g_log[0] = 0;
    action->PerformAction();
    CHECK(strcmp(g_log, "GetPosition;CreateThingObject(501,-2,1);SetThingAsImmune(1,1);ManualTrigger(1);") == 0);

    // 4. object: created and owned by the creature through the driver; obstruction with its value
    g_def.TrapDefIndex = 0; g_def.ObjectDefIndex = 777; g_object->m_presentBits[2] = 1UL << 1;
    g_def.ObstructionRadius = 2.5f; g_def.ObstructionValue = 9; g_obstruction->m_presentBits[7] = 1UL << 13; g_log[0] = 0;
    action->PerformAction();
    CHECK(strcmp(g_log, "GetPosition;CreateThing(777,1);SetOwner(1,1);GetPosition;CreateObstruction(1,250);SetValue(1,9);") == 0);

    // 5. tiny radius: no obstruction; null obstruction: no driver call
    g_def.ObjectDefIndex = 0; g_def.ObstructionRadius = 0.00005f; g_log[0] = 0;
    action->PerformAction();
    CHECK(strcmp(g_log, "") == 0);
    g_def.ObstructionRadius = 1.0f; g_obstruction = 0; g_log[0] = 0;
    action->PerformAction();
    CHECK(strcmp(g_log, "GetPosition;CreateObstruction(1,100);") == 0);

    // 6. the last reference: the def is destroyed when released
    g_def.ObstructionRadius = 0.0f; g_def.RefCount = 0; g_log[0] = 0;
    action->PerformAction();
    CHECK(strcmp(g_log, "Destroy;") == 0);

    if (fails == 0) printf("008c6b20_TEST PASS\n"); else printf("008c6b20_TEST FAIL\n");
    return fails != 0;
}