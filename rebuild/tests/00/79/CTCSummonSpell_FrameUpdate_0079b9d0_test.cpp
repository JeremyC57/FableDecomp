// Behaviour test for CTCSummonSpell::FrameUpdate (0x0079b9d0): pending create spawns at position+offset, live minion gets the hero faction and fades in with its carried things, dead/missing minion removes TC 0x91.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static const char* g_faction; static int g_strings;
struct CCharString {
    const char* m_data;
    CCharString(const char* text, long length = -1) : m_data(text) { (void)length; ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type) { SEntry* it = first; while (it != last && it->type < type) ++it; return it; }
};
struct CFrameSource {
    C3DVector Offset;
    void GetSummonOffset(C3DVector* out) { *out = Offset; }
};
struct CTCGraphicAppearance { int alpha; int sets; void SetAlpha(unsigned char a) { alpha = a; ++sets; } };
static long g_removed;
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    CFrameSource* Frame;
    CTCGraphicAppearance* Appearance;
    unsigned char Dead : 1;
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
    void RemoveTC(long type) { g_removed = type; }
    template<class T> __forceinline void GetInterface(long interfaceType, T** out)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long key = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CThingVector {
    CThing** First;
    CThing** Last;
    CThing** EndOfStorage;
    CThingVector() : First(0), Last(0), EndOfStorage(0) { }
    ~CThingVector() { if (First) free(First); }
};
static int g_factionCalls;
struct CTCEnemy { void SetFaction(const CCharString& faction) { g_faction = faction.m_data; ++g_factionCalls; } };
static CThing* g_carry[2];
struct CTCCarrying {
    void CollectCarrySlots(CThingVector* things)
    {
        things->First = (CThing**)malloc(2 * sizeof(CThing*));
        things->First[0] = g_carry[0]; things->First[1] = g_carry[1];
        things->Last = things->EndOfStorage = things->First + 2;
    }
};
static C3DVector g_createdAt; static int g_creates;
struct CTCSummonSpell {
    CThing* ParentThing;
    bool PendingCreate;
    CBaseIntelligentPointer Minion;
    long FadeTotal;
    long FadeFrames;
    long FrameCount;
    void CreateMinion(const C3DVector& pos, const C3DVector* facing) { g_createdAt = pos; ++g_creates; (void)facing; }
    void FrameUpdate();
};

void CTCSummonSpell::FrameUpdate()
{
    CThing* thing = ParentThing;
    if (PendingCreate) {
        C3DVector offset;
        thing->Frame->GetSummonOffset(&offset);
        CreateMinion(offset + thing->GetPosition(), 0);
        PendingCreate = false;
        return;
    }
    ++FrameCount;
    if (Minion.GetPItem()) {
        if (!Minion.GetPItem()->Dead) {
            CTCEnemy* enemy;
            Minion.GetPItem()->GetInterface(0x49, &enemy);
            enemy->SetFaction("FACTION_HERO");
            if (FadeFrames > 0) {
                --FadeFrames;
                float alpha = (1.0f - (float)FadeFrames / FadeTotal) * 255.0f;
                CTCGraphicAppearance* appearance = Minion.GetPItem()->Appearance;
                if (appearance)
                    appearance->SetAlpha((unsigned char)alpha);
                CTCCarrying* carrying;
                CThing* minion = Minion.GetPItem();
                if (minion->m_presentBits[0x46 >> 5] & (1UL << (0x46 & 31))) {
                    minion->GetInterface(0x46, &carrying);
                    CThingVector carried;
                    carrying->CollectCarrySlots(&carried);
                    CThing** it = carried.First;
                    CThing** end = carried.Last;
                    for (; it != end; ++it) {
                        CTCGraphicAppearance* a = (*it)->Appearance;
                        if (a)
                            a->SetAlpha((unsigned char)alpha);
                    }
                }
            }
            return;
        }
    }
    thing->RemoveTC(0x91);
}

int main()
{
    int fails = 0;
    static CThing owner, minion, sword, shield;
    memset(&owner, 0, sizeof(owner)); memset(&minion, 0, sizeof(minion)); memset(&sword, 0, sizeof(sword)); memset(&shield, 0, sizeof(shield));
    static CFrameSource frame; frame.Offset = C3DVector(1, 2, 3);
    owner.Frame = &frame; owner.Pos = C3DVector(10, 20, 30);
    static CTCEnemy enemy; static CTCCarrying carrying;
    CInterfaceMap::SEntry entries[2] = { { 0x46, (CTCBase*)&carrying }, { 0x49, (CTCBase*)&enemy } };
    minion.m_interfaces.first = entries; minion.m_interfaces.last = entries + 2;
    minion.m_presentBits[0x49 >> 5] |= 1UL << (0x49 & 31);
    minion.m_presentBits[0x46 >> 5] |= 1UL << (0x46 & 31);
    static CTCGraphicAppearance minionApp, swordApp; memset(&minionApp, 0, sizeof(minionApp)); memset(&swordApp, 0, sizeof(swordApp));
    minion.Appearance = &minionApp; sword.Appearance = &swordApp; shield.Appearance = 0;
    g_carry[0] = &sword; g_carry[1] = &shield;
    CTCSummonSpell spell; spell.ParentThing = &owner; spell.PendingCreate = true; spell.Minion.m_pItem = 0;
    spell.FadeTotal = 4; spell.FadeFrames = 4; spell.FrameCount = 0;
    spell.FrameUpdate();
    if (g_creates != 1 || g_createdAt.X != 11 || g_createdAt.Y != 22 || g_createdAt.Z != 33 || spell.PendingCreate || spell.FrameCount != 0) ++fails;
    spell.Minion.m_pItem = &minion;
    spell.FrameUpdate();                                        // fade 4 -> 3: alpha (1 - 3/4) * 255 = 63
    if (spell.FrameCount != 1 || spell.FadeFrames != 3 || g_factionCalls != 1 || strcmp(g_faction, "FACTION_HERO") != 0) ++fails;
    if (minionApp.alpha != 63 || swordApp.alpha != 63 || swordApp.sets != 1 || g_strings != 0) ++fails;
    spell.FadeFrames = 0; spell.FrameUpdate();                  // faded in: faction only
    if (g_factionCalls != 2 || minionApp.sets != 1 || g_removed != 0) ++fails;
    minion.Dead = 1; spell.FrameUpdate();                       // dead minion: remove the component
    if (g_removed != 0x91 || spell.FrameCount != 3) ++fails;
    g_removed = 0; spell.Minion.m_pItem = 0; spell.FrameUpdate();
    if (g_removed != 0x91) ++fails;
    if (fails) { printf("0079b9d0_TEST FAIL %d\n", fails); return 1; }
    printf("0079b9d0_TEST PASS\n");
    return 0;
}