#include <cstdio>
#include <cstring>

struct C3DVector { float X, Y, Z; };
static int g_strings;
struct CCharString {
    const char* p;
    CCharString(const char* text, long = -1) : p(text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CTCBase { int dummy; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
static int g_kills;
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    C3DVector m_pos;
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    void Kill(bool immediately) { if (immediately) { ++g_kills; m_flags91 |= 1; } }
    const C3DVector& GetPosition() const { return m_pos; }
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
static CThing g_spawned;
static int g_creates; static C3DVector g_createdAt; static char g_createdName[32];
static CThing* Create(const CCharString& defName, const C3DVector& position, bool flag)
{
    ++g_creates; g_createdAt = position; std::strcpy(g_createdName, defName.p);
    return flag ? 0 : &g_spawned;
}
static int g_attached; static CThing* g_attachThing; static long g_attachType, g_attachIndex; static const char* g_attachName;
struct CTCDParticleEmitter {
    int id;
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float delay)
    { ++g_attached; g_attachThing = thing; g_attachType = attachType; g_attachName = helperName.p; g_attachIndex = helperIndex; if (delay != 0.0f) g_attached = 100; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CFirePoint {
    CCharString HelperName; long HelperIndex; long EffectIndex; long Resistance; bool Burning;
    long FireDamageAfterFrame; CThing* PParentThing; CBaseIntelligentPointer PEffect; CBaseIntelligentPointer PSingeEffect;
    CFirePoint() : HelperName("hand_l") {}
    void Reset()
    {
        if (PEffect.GetPItem() && !PEffect.GetPItem()->IsDead()) PEffect.GetPItem()->Kill(true);
        if (PSingeEffect.GetPItem() && !PSingeEffect.GetPItem()->IsDead()) PSingeEffect.GetPItem()->Kill(true);
        if (Burning) {
            CThing* effect;
            {
                CCharString name("Flaming_Hands_Die", -1);
                effect = Create(name, PParentThing->GetPosition(), false);
            }
            if (effect && !effect->IsDead()) {
                CTCDParticleEmitter* emitter = 0;
                if (effect->HasTC(0x4b)) emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
                emitter->AttachToThing(PParentThing, 0x1c, HelperName, HelperIndex, 0.0f);
            }
        }
        Burning = false;
    }
};
int main()
{
    static CTCDParticleEmitter emitter;
    CInterfaceMap::SEntry entries[2] = { { 3, 0 }, { 0x4b, (CTCBase*)&emitter } };
    std::memset(&g_spawned, 0, sizeof(g_spawned));
    g_spawned.m_interfaces.first = entries; g_spawned.m_interfaces.last = entries + 2;
    g_spawned.m_presentBits[2] = 0x800;
    CThing parent; std::memset(&parent, 0, sizeof(parent)); parent.m_pos.X = 5.0f; parent.m_pos.Y = 6.0f;
    CThing fire; std::memset(&fire, 0, sizeof(fire));
    CThing singe; std::memset(&singe, 0, sizeof(singe)); singe.m_flags91 = 1;   // already dead: not killed again
    CFirePoint p; p.HelperIndex = 3; p.Burning = true; p.PParentThing = &parent; p.PEffect.m_pItem = &fire; p.PSingeEffect.m_pItem = &singe;
    p.Reset();
    if (g_kills != 1 || !(fire.m_flags91 & 1) || p.Burning || g_creates != 1 || std::strcmp(g_createdName, "Flaming_Hands_Die") != 0 ||
        g_createdAt.X != 5.0f || g_createdAt.Y != 6.0f || g_attached != 1 || g_attachThing != &parent || g_attachType != 0x1c ||
        std::strcmp(g_attachName, "hand_l") != 0 || g_attachIndex != 3 || g_strings != 1) {
        std::printf("00780700_TEST FAIL\n"); return 1;
    }
    p.Reset();                                         // not burning: no new effect
    if (g_creates != 1 || g_kills != 1) { std::printf("00780700_TEST FAIL\n"); return 1; }
    p.Burning = true; g_spawned.m_flags91 = 1;          // the spawned effect is already dead: no attach
    p.PEffect.m_pItem = 0;
    p.Reset();
    if (g_creates != 2 || g_attached != 1 || p.Burning) { std::printf("00780700_TEST FAIL\n"); return 1; }
    std::printf("00780700_TEST PASS\n");
    return 0;
}