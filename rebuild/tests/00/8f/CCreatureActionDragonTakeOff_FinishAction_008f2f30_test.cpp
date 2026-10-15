// Behaviour test for 0x008F2F30 CCreatureAction_DragonTakeOff::FinishAction: the physics TC is switched airborne; the physics TC is reset to the
// creature's position raised by 30.148378f on Z (X/Y unchanged), interface 0x4d is removed and the base FinishAction runs.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    C3DVector m_reset; int m_resets; int m_airborneCalls; bool m_airborne;
    void ResetPosition(const C3DVector& p) { m_reset = p; ++m_resets; }
    void SetAirborne(bool a) { m_airborne = a; ++m_airborneCalls; }
};
struct CThing {
    CTCPhysicsBase* PhysicsTC; C3DVector m_pos; long m_removed;
    const C3DVector& GetPosition() const { return m_pos; }
    void RemoveTC(long t) { m_removed = t; }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCreatureAction_DragonTakeOff {
    CBaseIntelligentPointer PCreature; int m_baseFinishes;
    void FinishAction()
    {
        { CThing* creature = PCreature.GetPItem(); creature->PhysicsTC->SetAirborne(true); }
        const C3DVector& current = PCreature.GetPItem()->GetPosition();
        C3DVector position;
        position.X = current.X; position.Y = current.Y; position.Z = current.Z + 30.148378f;
        CThing* creature = PCreature.GetPItem();
        creature->PhysicsTC->ResetPosition(position);
        PCreature.GetPItem()->RemoveTC(0x4d);
        ++m_baseFinishes;
    }
};
int main()
{
    CTCPhysicsBase phys; phys.m_resets = 0; phys.m_airborneCalls = 0; phys.m_airborne = false;
    CThing dragon; dragon.PhysicsTC = &phys; dragon.m_pos.X = 1.0f; dragon.m_pos.Y = 2.0f; dragon.m_pos.Z = 3.0f; dragon.m_removed = 0;
    CCreatureAction_DragonTakeOff a; a.PCreature.m_pItem = &dragon; a.m_baseFinishes = 0;
    a.FinishAction();
    float dz = phys.m_reset.Z - (3.0f + 30.148378f);
    if (!(phys.m_resets == 1 && phys.m_reset.X == 1.0f && phys.m_reset.Y == 2.0f && dz < 1e-4f && dz > -1e-4f &&
          dragon.m_pos.Z == 3.0f && dragon.m_removed == 0x4d && a.m_baseFinishes == 1 && phys.m_airborneCalls == 1 && phys.m_airborne)) {
        std::printf("008f2f30_TEST FAIL\n"); return 1;
    }
    std::printf("008f2f30_TEST PASS\n");
    return 0;
}