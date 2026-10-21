#include <cstdio>
#include <cstring>
// Behaviour test for CTCJackShield::EnableShield (0x00731680): same body as src/, with the owner's graphic object, the
// navigation component, the emitter factory, the intelligent pointer and Kill stubbed and recorded.
static int g_names = 0;
struct CCharString {
    const char* s;
    CCharString(const char* text, long length = -1) : s(text) { (void)length; ++g_names; }
    ~CCharString() { --g_names; }
};
struct C3DVector { float X, Y, Z; };
class CTCBaseComponent { };
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CInterfaceBits {
    unsigned long _M_w[9];
    bool test(unsigned long pos) const { return (_M_w[pos / 32] & (1ul << (pos % 32))) != 0; }
};
static int g_order = 0;
struct CThingGraphic {
    int shown, slot43, slot44, last43, shownAt, slotAt;
    void SetShieldShown(bool s) { shown = s; shownAt = ++g_order; }
    void Slot43(long v) { ++slot43; last43 = v; slotAt = ++g_order; }
    void Slot44() { ++slot44; slotAt = ++g_order; }
};
struct CThing {
    CInterfaceBits present;
    CInterfaceMap m_interfaces;
    CThingGraphic* PGraphic;
    unsigned char Flags91;
    C3DVector pos;
    int kills; bool killSilent;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const { return pos; }
    void Kill(bool silent) { ++kills; killSilent = silent; Flags91 |= 1; }
};
struct CTCSwitchableNavigation : CTCBaseComponent {
    int calls; bool on;
    void SetSwitchedOn(bool o) { ++calls; on = o; }
};
static CThing g_fxThings[4]; static int g_fxCount = 0; static bool g_failCreate = false;
static const C3DVector* g_createdAt = 0; static const char* g_createdName = 0; static bool g_createdAttached = true;
struct CTCDParticleEmitter {
    static CThing* Create(const CCharString& name, const C3DVector& position, bool attached)
    {
        g_createdName = name.s; g_createdAt = &position; g_createdAttached = attached;
        if (g_failCreate) return 0;
        CThing* t = &g_fxThings[g_fxCount++]; std::memset(t, 0, sizeof *t); return t;
    }
};
static int g_ptrs = 0;
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ptrs; }
    ~CBaseIntelligentPointer() { --g_ptrs; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    bool IsAlive() const { return GetPItem() && !(GetPItem()->Flags91 & 1); }
};
struct CTCBase { void* vt; CThing* ParentThing; };
struct CTCJackShield : CTCBase {
    CIntelligentPointer<CThing> PEmitter;
    void EnableShield(bool enable);
};

void CTCJackShield::EnableShield(bool enable)
{
    ParentThing->PGraphic->SetShieldShown(enable);
    if (enable)
        ParentThing->PGraphic->Slot44();
    else
        ParentThing->PGraphic->Slot43(0);
    CTCSwitchableNavigation* navigation;
    if (ParentThing->GetInterface(&navigation, 0x79))
        navigation->SetSwitchedOn(enable);
    if (enable) {
        if (!PEmitter.IsAlive()) {
            CThing* emitter = CTCDParticleEmitter::Create("JACK_OF_BLADES_SHIELD_01",
                                                          ParentThing->GetPosition(), false);
            if (emitter)
                PEmitter = CIntelligentPointer<CThing>(emitter);
        }
    } else if (PEmitter.IsAlive())
        PEmitter.GetPItem()->Kill(true);
}

int main()
{
    int fails = 0;
    CThingGraphic graphic; std::memset(&graphic, 0, sizeof graphic);
    CTCSwitchableNavigation nav; nav.calls = 0; nav.on = false;
    CInterfaceMap::SEntry entries[2] = { { 0x40, 0 }, { 0x79, &nav } };
    CThing owner; std::memset(&owner, 0, sizeof owner);
    owner.PGraphic = &graphic; owner.m_interfaces.first = entries; owner.m_interfaces.last = entries + 2;
    owner.present._M_w[0x79 / 32] |= 1ul << (0x79 % 32);
    CTCJackShield tc; tc.ParentThing = &owner;
    int base = g_ptrs;
    tc.EnableShield(true);
    if (graphic.shown != 1 || graphic.slot44 != 1 || graphic.slot43 != 0 || graphic.shownAt > graphic.slotAt) ++fails;
    if (nav.calls != 1 || !nav.on) ++fails;
    if (g_fxCount != 1 || tc.PEmitter.GetPItem() != &g_fxThings[0] || g_createdAt != &owner.pos || g_createdAttached) ++fails;
    if (std::strcmp(g_createdName, "JACK_OF_BLADES_SHIELD_01") != 0 || g_names != 0 || g_ptrs != base) ++fails;
    tc.EnableShield(true);                                   // live emitter: not recreated
    if (g_fxCount != 1 || graphic.slot44 != 2) ++fails;
    tc.EnableShield(false);                                  // kill the live emitter
    if (graphic.shown != 0 || graphic.slot43 != 1 || graphic.last43 != 0 || nav.on || nav.calls != 3) ++fails;
    if (g_fxThings[0].kills != 1 || !g_fxThings[0].killSilent) ++fails;
    tc.EnableShield(false);                                  // already dead: no second kill
    if (g_fxThings[0].kills != 1) ++fails;
    owner.present._M_w[0x79 / 32] = 0;                        // no navigation component
    g_failCreate = true;
    tc.EnableShield(true);                                   // dead emitter, creation fails: pointer kept
    if (nav.calls != 4 || tc.PEmitter.GetPItem() != &g_fxThings[0] || g_fxCount != 1) ++fails;
    g_failCreate = false;
    tc.EnableShield(true);
    if (tc.PEmitter.GetPItem() != &g_fxThings[1]) ++fails;
    if (fails) { std::printf("00731680_TEST FAIL %d\n", fails); return 1; }
    std::printf("00731680_TEST PASS\n");
    return 0;
}