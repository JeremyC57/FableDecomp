#include <cstdio>
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
#define __fastcall
#endif
struct C3DVector { float X, Y, Z; };
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
struct CDefPointeeBase {                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> struct CDefPointer {          // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
};
struct CCreatureDef : CDefPointeeBase {
    unsigned char _pad08[0xca - 0x08];
    bool PickPocketable;                        // +0xca
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CCreatureDef>* out);         // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}

struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing;
struct CPlayer {
    CThing* GetPlayerCreature();            // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;               // 0x00449960
};
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;            // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;                // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_statusFlags;           // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;   // 0x004C73D0
    CPlayerManager* GetPlayerManager();     // 0x004C79F0
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
struct CTCPerceiveThing {
    CThing* GetPerceivedThing() const;          // 0x007446A0
    bool CanSeeOrSmellPerceivedThing() const;   // 0x00661600
};
struct CThingCreatureBase : CThing {
    bool IsPickPocketable();
};

bool CThingCreatureBase::IsPickPocketable()
{
    CDefPointer<CCreatureDef> out;
    PBaseDef->GetSubDef(&out);
    CCreatureDef* def = out.m_pObject;
    if (def) {
        if (def->PickPocketable) {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetPlayerCreature();
            CTCPerceiveThing* perceive;
            if (hero && !(hero->m_statusFlags & 1) && GetInterface(&perceive, 0x34)) {
                if (perceive->GetPerceivedThing() != hero || !perceive->CanSeeOrSmellPerceivedThing()) {
                    if (GFWorld3DDistanceSmallerThan(GetPosition(), hero->GetPosition(), 2.0f)) {
                        def->ReleaseDef();
                        return true;
                    }
                }
            }
        }
        def->ReleaseDef();
    }
    return false;
}
// ---- test doubles ----
void CDefPointeeBase::v000() {}
void CDefPointeeBase::DestroyDef() {}
struct CTestDef : CCreatureDef { int destroyed; virtual void DestroyDef() { ++destroyed; } };
static CTestDef* g_def;
void* FindDef_CDefClassBase_GetSubDef_0() { if (g_def) ++g_def->m_refCount; return g_def; }
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float d)
{
    float x = a.X - b.X, y = a.Y - b.Y, z = a.Z - b.Z; return x * x + y * y + z * z < d * d;
}
static C3DVector g_pos[2]; static CThing* g_thingForPos[2];
const C3DVector& CThing::GetPosition() const { return this == g_thingForPos[0] ? g_pos[0] : g_pos[1]; }
static CThing* g_hero; static CPlayer g_player; static CPlayerManager g_pm;
CPlayerManager* CThing::GetPlayerManager() { return &g_pm; }
CPlayer* CPlayerManager::GetMainPlayer() const { return &g_player; }
CThing* CPlayer::GetPlayerCreature() { return g_hero; }
static CTCPerceiveThing g_perceive; static CThing* g_perceived; static bool g_canSee;
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { return *type == 0x34 ? &g_entries[0] : last; }
CThing* CTCPerceiveThing::GetPerceivedThing() const { return g_perceived; }
bool CTCPerceiveThing::CanSeeOrSmellPerceivedThing() const { return g_canSee; }
int main()
{
    CTestDef def; def.m_refCount = 1; def.destroyed = 0; def.PickPocketable = true; g_def = &def;
    CDefClassBase base;
    CThingCreatureBase c; for (int i = 0; i < 9; ++i) c.m_presentBits[i] = 0; c.m_statusFlags = 0; c.PBaseDef = &base;
    c.m_presentBits[1] = 1u << 20;              // has CTCPerceiveThing (type 0x34)
    g_entries[0].type = 0x34; g_entries[0].component = (CTCBase*)&g_perceive;
    c.m_interfaces.first = g_entries; c.m_interfaces.last = g_entries + 1;
    CThing hero; hero.m_statusFlags = 0; g_hero = &hero;
    g_thingForPos[0] = &c; g_pos[0].X = 0; g_pos[0].Y = 0; g_pos[0].Z = 0;
    g_pos[1].X = 1.0f; g_pos[1].Y = 1.0f; g_pos[1].Z = 0;     // hero 1.41 away
    g_perceived = &hero; g_canSee = false;
    bool ok = c.IsPickPocketable() && def.m_refCount == 1;          // perceives hero but cannot see/smell him
    g_canSee = true;
    ok = ok && !c.IsPickPocketable();                                // watching the hero
    g_perceived = 0;
    ok = ok && c.IsPickPocketable();                                 // perceiving someone else
    g_pos[1].X = 3.0f;
    ok = ok && !c.IsPickPocketable();                                // too far
    g_pos[1].X = 1.0f; hero.m_statusFlags = 1;
    ok = ok && !c.IsPickPocketable();                                // dead hero
    hero.m_statusFlags = 0; g_hero = 0;
    ok = ok && !c.IsPickPocketable();                                // no hero
    g_hero = &hero; c.m_presentBits[1] = 0;
    ok = ok && !c.IsPickPocketable();                                // no perception component
    c.m_presentBits[1] = 1u << 20; def.PickPocketable = false;
    ok = ok && !c.IsPickPocketable() && def.m_refCount == 1 && def.destroyed == 0;
    def.PickPocketable = true; def.m_refCount = 0;
    ok = ok && c.IsPickPocketable() && def.destroyed == 1;           // last reference released
    g_def = 0;
    ok = ok && !c.IsPickPocketable();                                // no creature def
    if (!ok) { std::printf("00665e90_TEST FAIL\n"); return 1; }
    std::printf("00665e90_TEST PASS\n");
    return 0;
}