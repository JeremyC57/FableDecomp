// Behaviour test for 00835bd0: body copied from src/00835bd0.cpp (CThingDeadCreature::PreloadDeadAnimation), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CMeshDataBank { int id; };
struct SAnimationSetEntry { int id; };
static int g_destroyed = 0;
struct CDefPointeeBase {
    virtual void v000() {}
    virtual void DestroyDef() { ++g_destroyed; }
    long m_refCount;
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> struct CDefPointer {
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer() { if (m_pObject) m_pObject->ReleaseDef(); }
};
struct CAnimationSet {
    SAnimationSetEntry* m_first;
    SAnimationSetEntry* m_last;
    SAnimationSetEntry* m_end;
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char       _pad08[0x2c];
    CAnimationSet       m_anims;
};
static CAppearanceDef* g_appearance = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    CAppearanceDef* FindAppearanceDef() { ++g_getSubDef; return g_appearance; }
    bool GetSubDef(CDefPointer<CAppearanceDef>& out);
};
bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>& out)
{
    CAppearanceDef* def = FindAppearanceDef();
    if (!def)
        return false;
    ++def->m_refCount;
    out.m_pObject = def;
    return true;
}
struct CWorld {
    unsigned char  _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;
};
struct CPreloadDeadAnimation {
    CMeshDataBank* m_bank;
    CPreloadDeadAnimation(CMeshDataBank* bank) : m_bank(bank) { }
};
static int g_preloaded = 0; static CMeshDataBank* g_preloadBank = 0; static SAnimationSetEntry* g_lastEntry = 0;
static void PreloadEntry(CMeshDataBank* bank, SAnimationSetEntry* entry) { ++g_preloaded; g_preloadBank = bank; g_lastEntry = entry; }
static SAnimationSetEntry* NextEntry(SAnimationSetEntry* entry) { return entry + 1; }
CPreloadDeadAnimation __fastcall ForEachAnimation(SAnimationSetEntry* first, SAnimationSetEntry* last, CPreloadDeadAnimation preload)
{
    for (; first != last; first = NextEntry(first))
        PreloadEntry(preload.m_bank, first);
    return preload;
}
static CWorld g_world;
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;
    const CWorld& PeekWorld() const { return g_world; }
};
struct CThingDeadCreature : CThing {
    void PreloadDeadAnimation();
};
void CThingDeadCreature::PreloadDeadAnimation()
{
    CMeshDataBank* bank = PeekWorld().PMainMeshBank;
    CDefPointer<CAppearanceDef> def;
    PBaseDef->GetSubDef(def);
    const CAnimationSet& anims = def.m_pObject->m_anims;
    ForEachAnimation(anims.m_first, anims.m_last, CPreloadDeadAnimation(bank));
}
int main()
{
    static CMeshDataBank bank; static CAppearanceDef appearance; static SAnimationSetEntry entries[3]; static CDefClassBase baseDef;
    g_world.PMainMeshBank = &bank; appearance.m_refCount = 1; appearance.m_anims.m_first = entries; appearance.m_anims.m_last = entries + 3; appearance.m_anims.m_end = entries + 3;
    g_appearance = &appearance;
    CThingDeadCreature creature; memset(&creature, 0, sizeof(creature)); creature.PBaseDef = &baseDef;
    // 1: three entries preloaded with the main mesh bank; the temporary reference is released, the def survives
    creature.PreloadDeadAnimation();
    CHECK(g_getSubDef == 1); CHECK(g_preloaded == 3 && g_preloadBank == &bank && g_lastEntry == entries + 2); CHECK(appearance.m_refCount == 1 && g_destroyed == 0);
    // 2: a def held only by the temporary is destroyed on release
    appearance.m_refCount = 0; g_preloaded = 0;
    creature.PreloadDeadAnimation();
    CHECK(g_preloaded == 3 && g_destroyed == 1 && appearance.m_refCount == 0);
    // 3: an empty animation set preloads nothing
    appearance.m_refCount = 1; appearance.m_anims.m_last = entries; g_preloaded = 0;
    creature.PreloadDeadAnimation();
    CHECK(g_preloaded == 0 && appearance.m_refCount == 1);
    if (fails == 0) printf("00835bd0_TEST PASS\n"); else printf("00835bd0_TEST FAIL\n");
    return fails;
}