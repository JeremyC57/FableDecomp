#include <cstdio>
struct CConstraintRepeat;
struct CRegistry {
    CConstraintRepeat* added[4]; int n;
    CRegistry() : n(0) { }
    void Add(CConstraintRepeat* c) { added[n++] = c; }
};
struct CProgramOwner { CRegistry m_constraints; };
struct CProgram { CProgramOwner* m_pOwner; CRegistry m_updates; };
struct CInterpolatedFloat {
    float m_current; float m_target; bool m_isAngle;
    CInterpolatedFloat() : m_isAngle(false) { }
    void Reset(float value) { m_current = value; m_target = value; }
};
static int g_order = 0;
struct CConstraintSlot { int built; CConstraintSlot() : built(++g_order) { } };
struct CConstraintRepeat {
    CProgram* m_pProgram; void* m_pTarget; CInterpolatedFloat m_value; CConstraintSlot m_slot;
    CConstraintRepeat(CProgram* program, void* target, float value)
        : m_pProgram(program), m_pTarget(target)
    {
        m_value.Reset(value);
        m_pProgram->m_updates.Add(this);
        m_pProgram->m_pOwner->m_constraints.Add(this);
    }
};
int main()
{
    CProgramOwner owner; CProgram prog; prog.m_pOwner = &owner;
    int target = 0;
    CConstraintRepeat c(&prog, &target, 2.5f);
    int bad = (c.m_pProgram != &prog || c.m_pTarget != &target || c.m_value.m_isAngle || c.m_value.m_current != 2.5f || c.m_value.m_target != 2.5f || c.m_slot.built != 1);
    if (prog.m_updates.n != 1 || prog.m_updates.added[0] != &c || owner.m_constraints.n != 1 || owner.m_constraints.added[0] != &c) bad = 1;
    if (bad) { std::printf("00c38570_TEST FAIL\n"); return 1; }
    std::printf("00c38570_TEST PASS\n");
    return 0;
}