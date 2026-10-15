#include <cstdio>
struct CCharString { const char* m_pData; };
struct CAnimationEntry { long m_anim; };
struct CThingPlayerCreature {
    virtual void vfunc_68(long anim, long mode) { m_anim = anim; m_mode = mode; }
    virtual void vfunc_69(const CCharString& name, long mode);
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index)
    { m_lastName = name.m_pData; m_lastIndex = index; return &m_entry; }
    CAnimationEntry m_entry; long m_anim, m_mode, m_lastIndex; const char* m_lastName;
};
void CThingPlayerCreature::vfunc_69(const CCharString& name, long mode)
{
    vfunc_68(GetAnimFromName(name, 0)->m_anim, mode);
}
int main()
{
    CThingPlayerCreature c; c.m_entry.m_anim = 77; c.m_anim = 0; c.m_mode = 0; c.m_lastIndex = -1;
    CCharString n; n.m_pData = "WALK";
    c.vfunc_69(n, 5);
    if (c.m_anim != 77 || c.m_mode != 5 || c.m_lastIndex != 0 || c.m_lastName != n.m_pData) { std::printf("006635d0_TEST FAIL\n"); return 1; }
    std::printf("006635d0_TEST PASS\n");
    return 0;
}