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
// ---- the candidate model (copied from src/008d9410.cpp) ----
// NCreatureMode::CModeUnderground::FrameUpdate() -- vtable slot 3: the base frame update, then an underground creature whose linked thing (component 0x111) is gone or dead kills itself
// Leaf: ?FrameUpdate@CModeUnderground@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9410, 109 bytes (+3 int3 padding).
// Name: slot 3 of the vtable 0x01267E38 (rtti-vtables.tsv); it opens with the landed CModeBase::FrameUpdate 0x0073AC50
// (CTCCreatureModeManager::CModeBase), so the slot is FrameUpdate; slot 5 is the landed OnActivate 0x008D66E0.
// Callees: CTCCreatureModeManager::CModeBase::FrameUpdate @ 0x0073AC50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// (const) @ 0x004365B0 (the inlined const component lookup, type 0x111 = bit 17 of the bitset word +0x40),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CThing::Kill(bool) @ 0x004C9B80.
// Layout: CModeBase {vptr, creature +4}; the thing's interface bitset +0x20 and map +0x44, Dead = bit 0 of +0x91 (as the
// landed CThingDeadCreature::WorldUpdate 0x00836260); the 0x111 component holds a CIntelligentPointer<CThing> at +0x0C
// (its class is not recovered: descriptive name).
// Shape: the out-parameter `GetConstInterface(&link, 0x111)` template as the condition (it leaves the component in
// eax and forms `lea esi,[eax+0Ch]`; the value-returning HasTC + GetConstTC pair gives `mov esi,[eax+4] ; add esi,0Ch`),
// the pointer's address kept in esi for both GetPItem calls (`!p || dead`), the creature re-read from +4 for the Kill.
// Plain `ret`.
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing {
public:
    void* __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];        // +0x20
    CInterfaceMap m_interfaces;            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                // +0x91 bit 0
    void Kill(bool immediately);           // 0x004C9B80
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetConstInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;              // 0x00A01B50
};

struct CTCLinkedThing {                    // interface type 0x111
    unsigned char           _pad00[0x0c];
    CBaseIntelligentPointer PLinkedThing;  // +0x0c
};

namespace NCreatureMode {

struct CModeBase {                         // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThing* m_pThing;                      // +0x04
    void FrameUpdate();                    // 0x0073AC50
};

struct CModeUnderground : CModeBase {
    virtual void FrameUpdate();
};

void CModeUnderground::FrameUpdate()
{
    CModeBase::FrameUpdate();
    const CTCLinkedThing* link;
    if (m_pThing->GetConstInterface(&link, 0x111)) {
        if (!link->PLinkedThing.GetPItem() || link->PLinkedThing.GetPItem()->Dead)
            m_pThing->Kill(false);
    }
}

} // namespace NCreatureMode

// ---- stubs for the external callees ----

const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}

static int g_order = 0;
static int g_base = 0;
static int g_kills = 0;
static bool g_immediately = true;
void CThing::Kill(bool immediately) { g_kills = ++g_order; g_immediately = immediately; }
void CBaseIntelligentPointer::v00() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
NCreatureMode::CModeBase::~CModeBase() {}
void NCreatureMode::CModeBase::FrameUpdate() { g_base = ++g_order; }

static void run()
{

    CTCLinkedThing link;
    link.PLinkedThing.PItem = 0;
    CInterfaceMap::SEntry entries[1];
    entries[0].type = 0x111;
    entries[0].component = reinterpret_cast<CTCBase*>(&link);
    CThing thing;
    int w = 0;
    while (w < 9) { thing.m_presentBits[w] = 0; ++w; }
    thing.m_interfaces.first = entries;
    thing.m_interfaces.last = entries + 1;
    NCreatureMode::CModeUnderground mode;
    mode.m_pThing = &thing;
    mode.FrameUpdate();                       // no linked-thing interface: base update only
    CHECK(g_base == 1 && g_kills == 0);
    thing.m_presentBits[0x111 >> 5] |= 1UL << (0x111 & 31);
    mode.FrameUpdate();                       // linked to nothing: killed (not immediately)
    CHECK(g_base == 2 && g_kills == 3 && g_immediately == false);
    CThing other;
    other.Dead = 0;
    link.PLinkedThing.PItem = &other;
    mode.FrameUpdate();                       // linked thing alive: kept
    CHECK(g_base == 4 && g_kills == 3);
    other.Dead = 1;
    mode.FrameUpdate();                       // linked thing dead: killed
    CHECK(g_base == 5 && g_kills == 6);
}

int main()
{
    run();
    if (!g_ok) { std::printf("008d9410_TEST FAIL\n"); return 1; }
    std::printf("008d9410_TEST PASS\n");
    return 0;
}