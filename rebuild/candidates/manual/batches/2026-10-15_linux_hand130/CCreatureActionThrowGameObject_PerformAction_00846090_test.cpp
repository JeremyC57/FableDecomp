// Behaviour test for 0x00846090 CCreatureAction_ThrowGameObject::PerformAction.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase { C3DVector m_acc; int m_calls; void ApplyAcceleration(const C3DVector& a) { m_acc = a; ++m_calls; } };
struct CThing {
    unsigned int m_presentBits[9];
    void* m_carrying;
    CTCPhysicsBase* m_pPhysics;
    template<class T> bool GetInterface(T** ppOut, int interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) { *ppOut = (T*)m_carrying; return true; }
        return false;
    }
};
struct CTCCarrying {
    CThing* m_primary; int m_removed;
    CThing* GetThingInPrimarySlot() { return m_primary; }
    void RemoveThing(CThing* t) { if (t == m_primary) m_primary = 0; ++m_removed; }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CAction {
    CBaseIntelligentPointer PCreature; C3DVector Target;
    void PerformAction()
    {
        CTCCarrying* pCarrying = 0;
        PCreature.GetPItem()->GetInterface(&pCarrying, 0x46);
        CThing* thrown = pCarrying->GetThingInPrimarySlot();
        if (thrown) {
            pCarrying->RemoveThing(thrown);
            thrown->m_pPhysics->ApplyAcceleration(Target);
        }
    }
};
int main()
{
    CTCPhysicsBase physics; physics.m_calls = 0;
    CThing rock; rock.m_pPhysics = &physics;
    CTCCarrying carrying; carrying.m_primary = &rock; carrying.m_removed = 0;
    CThing me; for (int i = 0; i < 9; ++i) me.m_presentBits[i] = 0;
    me.m_presentBits[0x46 >> 5] |= 1u << (0x46 & 31); me.m_carrying = &carrying;
    CAction a; a.PCreature.m_pItem = &me; a.Target.X = 0; a.Target.Y = 9; a.Target.Z = 1;
    a.PerformAction();
    if (carrying.m_primary != 0 || carrying.m_removed != 1 || physics.m_calls != 1 || physics.m_acc.Y != 9) { std::printf("00846090_TEST FAIL\n"); return 1; }
    a.PerformAction();   // nothing left to throw
    if (carrying.m_removed != 1 || physics.m_calls != 1) { std::printf("00846090_TEST FAIL\n"); return 1; }
    std::printf("00846090_TEST PASS\n");
    return 0;
}
