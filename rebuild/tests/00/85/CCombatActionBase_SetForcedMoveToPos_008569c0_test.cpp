#include <cstdio>

// Model of CCombatActionBase::SetForcedMoveToPos @ 0x008569C0
class C3DVector { public: float X, Y, Z; };
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { C3DVector r; r.X = a.X - b.X; r.Y = a.Y - b.Y; r.Z = a.Z - b.Z; return r; }
class CThingCreatureBase { public: C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
class CCombatActionBase { public:
    virtual void v00() {} unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xdd - 0x10]; bool PreferredFinalPosSet; unsigned char _padde[0x108 - 0xde]; C3DVector PreferredMovementVector;
    void SetForcedMoveToPos(const C3DVector& pos) { PreferredFinalPosSet = true; PreferredMovementVector = pos - PCreature.GetPItem()->GetPosition(); }
};
int main()
{
    CThingCreatureBase c; c.pos.X = 1.0f; c.pos.Y = 2.0f; c.pos.Z = 3.0f;
    CCombatActionBase a; a.PCreature.m_pItem = &c; a.PreferredFinalPosSet = false;
    C3DVector p; p.X = 10.0f; p.Y = 20.0f; p.Z = 30.0f;
    a.SetForcedMoveToPos(p);
    if (!a.PreferredFinalPosSet || a.PreferredMovementVector.X != 9.0f || a.PreferredMovementVector.Y != 18.0f || a.PreferredMovementVector.Z != 27.0f
        || (sizeof(void*) == 4 && (char*)&a.PreferredMovementVector - (char*)&a != 0x108) || (sizeof(void*) == 4 && (char*)&a.PreferredFinalPosSet - (char*)&a != 0xdd)) { std::printf("008569c0_TEST FAIL\n"); return 1; }
    std::printf("008569c0_TEST PASS\n"); return 0;
}