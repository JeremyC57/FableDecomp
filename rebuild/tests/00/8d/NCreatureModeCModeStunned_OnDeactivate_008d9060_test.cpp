#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/008d9060.cpp) ----
// NCreatureMode::CModeStunned::OnDeactivate() -- vtable slot 6: the stunned creature gets its default targeting flags back, then the out-of-action base deactivation runs
// Leaf: ?OnDeactivate@CModeStunned@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9060, 72 bytes (+8 int3 padding).
// Name: FableWin.pdb ?OnDeactivate@CModeStunned@NCreatureMode@@EAEXXZ; slot 6 of the vtable 0x01267C74 (rtti-vtables.tsv;
// CTCCreatureModeManager::CModeBase declares OnDeactivate; slot 5 of the sibling modes is the landed OnActivate).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const long*) @ 0x0040F020 (the inlined GetInterface,
// type 9 = CTCTargeted, bit 9 of the +0x20 bitset), CTCTargeted::ReturnToDefaultTargetingFlags @ 0x007EA2B0,
// CModeOutOfActionBase::OnDeactivate @ 0x008D8F30.
// Layout: CModeBase {vptr, CThing* +4}; the thing's interface bitset +0x20 and {type, component} map +0x44 (as the
// landed CModeCreatureIsProjectile::Recover 0x008E08C0).
// Shape: the out-parameter GetInterface template (the key homed in the `push ecx` slot, `it == last || 9 < it->type`
// picks last) used as the condition; the base call keeps `this` in edi. Plain `ret`.
class CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

class CTCTargeted { public:                       // interface type 9
    void ReturnToDefaultTargetingFlags();  // 0x007EA2B0
};

class CThingCreatureBase { public:
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
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

namespace NCreatureMode {

struct CModeBase {                         // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThingCreatureBase* m_pThing;          // +0x04
};

struct CModeOutOfActionBase : CModeBase {
    void OnDeactivate();                   // 0x008D8F30
};

struct CModeStunned : CModeOutOfActionBase {
    virtual void OnDeactivate();
};

void CModeStunned::OnDeactivate()
{
    CTCTargeted* targeted;
    if (m_pThing->GetInterface(&targeted, 9))
        targeted->ReturnToDefaultTargetingFlags();
    CModeOutOfActionBase::OnDeactivate();
}

} // namespace NCreatureMode

// ---- stubs for the external callees ----

CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}

static int g_order = 0;
static int g_returned = 0;
static int g_baseDeactivated = 0;
static CTCTargeted* g_targetedThis = 0;
void CTCTargeted::ReturnToDefaultTargetingFlags() { g_returned = ++g_order; g_targetedThis = this; }
NCreatureMode::CModeBase::~CModeBase() {}
void NCreatureMode::CModeOutOfActionBase::OnDeactivate() { g_baseDeactivated = ++g_order; }

static void run()
{

    CTCTargeted targeted;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 3; entries[0].component = 0;
    entries[1].type = 9; entries[1].component = reinterpret_cast<CTCBase*>(&targeted);
    CThingCreatureBase creature;
    int w = 0;
    while (w < 9) { creature.m_presentBits[w] = 0; ++w; }
    creature.m_interfaces.first = entries;
    creature.m_interfaces.last = entries + 2;
    NCreatureMode::CModeStunned mode;
    mode.m_pThing = &creature;
    mode.OnDeactivate();                      // no targeted interface: only the base runs
    CHECK(g_returned == 0 && g_baseDeactivated == 1);
    creature.m_presentBits[0] |= 1UL << 9;
    mode.OnDeactivate();
    CHECK(g_targetedThis == &targeted && g_returned == 2 && g_baseDeactivated == 3);
}

int main()
{
    run();
    if (!g_ok) { std::printf("008d9060_TEST FAIL\n"); return 1; }
    std::printf("008d9060_TEST PASS\n");
    return 0;
}