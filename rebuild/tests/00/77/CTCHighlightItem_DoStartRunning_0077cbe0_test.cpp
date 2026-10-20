// Behaviour test for CTCHighlightItem::DoStartRunning (0x0077CBE0): with an emitter type in the def the effect is
// created at the thing's position raised by DistanceFromGround and kept; the flags/timer are reset either way and
// the def reference taken by GetSubDef is released.
#include <cstdio>

struct CDefBase {
    long m_count; int destroyed;
    void Destroy() { ++destroyed; }
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CHighlightItemDef : CDefBase { long ParticleEmitter; float DistanceFromGround; };
struct CHighlightItemDefPointer {
    CHighlightItemDef* m_p;
    CHighlightItemDefPointer() : m_p(0) {}
    ~CHighlightItemDefPointer() { if (m_p) m_p->Release(); }
    CHighlightItemDef* operator->() const { return m_p; }
};
static CHighlightItemDef* g_def = 0;
struct CDefClassBase { bool GetSubDef(CHighlightItemDefPointer& out) { out.m_p = g_def; ++g_def->m_count; return true; } };
struct C3DVector { float X, Y, Z; };
struct CThing {
    CDefClassBase* PDef; C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
};
static CThing g_effect; static long g_type = 0; static C3DVector g_pos; static bool g_flag = true; static int g_creates = 0;
CThing* CreateEffectThing(long type, const C3DVector* position, bool flag) { g_type = type; g_pos = *position; g_flag = flag; ++g_creates; return &g_effect; }
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() : CBaseIntelligentPointer() {} };
struct CTCHighlightItem {
    CThing* ParentThing; bool StartRunning; bool Running; long LifeTimer; C3DVector StartPos;
    CBaseIntelligentPointer PParticleEmitter;
    void DoStartRunning();
};
void CTCHighlightItem::DoStartRunning()
{
    CHighlightItemDefPointer def;
    ParentThing->PDef->GetSubDef(def);
    if (def->ParticleEmitter) {
        C3DVector position = ParentThing->GetPosition();
        position.Z += def->DistanceFromGround;
        CThing* emitter = CreateEffectThing(def->ParticleEmitter, &position, false);
        CIntelligentPointer pointer;
        pointer.SetPItem(emitter);
        PParticleEmitter.SetPItem(pointer.GetPItem());
    }
    LifeTimer = 0;
    StartRunning = false;
    Running = true;
}
int main()
{
    CHighlightItemDef def; def.m_count = 1; def.destroyed = 0; def.ParticleEmitter = 42; def.DistanceFromGround = 0.5f;
    g_def = &def;
    CDefClassBase thingDef; CThing thing; thing.PDef = &thingDef; thing.Pos.X = 1.0f; thing.Pos.Y = 2.0f; thing.Pos.Z = 3.0f;
    CTCHighlightItem h; h.ParentThing = &thing; h.StartRunning = true; h.Running = false; h.LifeTimer = 7;
    h.DoStartRunning();
    if (g_creates != 1 || g_type != 42 || g_pos.X != 1.0f || g_pos.Y != 2.0f || g_pos.Z != 3.5f || g_flag ||
        h.PParticleEmitter.GetPItem() != &g_effect || h.LifeTimer != 0 || h.StartRunning || !h.Running ||
        def.m_count != 1 || def.destroyed != 0 || thing.Pos.Z != 3.0f) { std::printf("0077cbe0_TEST FAIL\n"); return 1; }
    def.ParticleEmitter = 0; h.StartRunning = true; h.Running = false; h.LifeTimer = 3;
    h.DoStartRunning();
    if (g_creates != 1 || h.LifeTimer != 0 || h.StartRunning || !h.Running || def.m_count != 1) { std::printf("0077cbe0_TEST FAIL\n"); return 1; }
    std::printf("0077cbe0_TEST PASS\n");
    return 0;
}