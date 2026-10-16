// Behaviour test for 0x008F3FD0 CCreatureAction_DragonHoverInto::Initialise.
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CTCHover { bool m_bFlag10; };
struct CThingCreatureBase {
    C3DVector pos, reset; int facingQueries; CTCHover hover;
    const C3DVector& GetPosition() const { return pos; }
    void ResetPosition(const C3DVector& p) { reset = p; }
    C3DVector GetFacing() { ++facingQueries; return pos; }
    bool GetInterface(CTCHover** pp, long t) { if (t == 9) { *pp = &hover; return true; } return false; }
};
struct CBaseIntelligentPointer { CThingCreatureBase* PItem; CThingCreatureBase* GetPItem() const { return PItem; } };
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CCreatureAction_DragonHoverInto {
    CBaseIntelligentPointer PCreature; const char* anim; int baseInit;
    void SetAnimationName(const CCharString& n) { anim = n.t; }
    void Initialise()
    {
        { CCharString name("HOVER_INTO"); SetAnimationName(name); }
        ++baseInit;
        const C3DVector& pos = PCreature.GetPItem()->GetPosition();
        C3DVector lowered; lowered.X = pos.X; lowered.Y = pos.Y; lowered.Z = pos.Z - 30.148378f;
        PCreature.GetPItem()->ResetPosition(lowered);
        PCreature.GetPItem()->GetFacing();
        CTCHover* hover;
        PCreature.GetPItem()->GetInterface(&hover, 9);
        hover->m_bFlag10 = true;
    }
};
int main()
{
    CThingCreatureBase c; c.pos.X = 1; c.pos.Y = 2; c.pos.Z = 40.148378f; c.facingQueries = 0; c.hover.m_bFlag10 = false;
    CCreatureAction_DragonHoverInto a; a.PCreature.PItem = &c; a.baseInit = 0;
    a.Initialise();
    float dz = c.reset.Z - 10.0f; if (dz < 0) dz = -dz;
    if (c.reset.X != 1 || c.reset.Y != 2 || dz > 1e-4f || c.facingQueries != 1 || !c.hover.m_bFlag10 || std::strcmp(a.anim, "HOVER_INTO") || a.baseInit != 1) {
        std::printf("008f3fd0_TEST FAIL\n"); return 1;
    }
    std::printf("008f3fd0_TEST PASS\n");
    return 0;
}
