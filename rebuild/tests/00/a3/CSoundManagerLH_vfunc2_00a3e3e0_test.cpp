#include <cstdio>
struct CSoundOutput {
    virtual void v000() {}
    virtual void SetEnabled(bool enabled) { m_enabled = enabled; ++m_calls; }
    bool m_enabled; int m_calls;
};
struct CSoundSystem {
    virtual void v000() {}
    virtual void v001() {}
    virtual void v002() {}
    virtual CSoundOutput* GetOutput() { return m_out; }
    CSoundOutput* m_out;
};
struct CSoundManagerLH {
    virtual void v000() {}
    virtual void v001() {}
    virtual void vfunc_2(bool disabled);
    CSoundSystem* m_pSoundSystem;
};
void CSoundManagerLH::vfunc_2(bool disabled)
{
    if (m_pSoundSystem && m_pSoundSystem->GetOutput())
        m_pSoundSystem->GetOutput()->SetEnabled(!disabled);
}
int main()
{
    int bad = 0;
    CSoundManagerLH m; m.m_pSoundSystem = 0;
    m.vfunc_2(true);
    CSoundSystem sys; sys.m_out = 0; m.m_pSoundSystem = &sys;
    m.vfunc_2(true);
    CSoundOutput out; out.m_enabled = true; out.m_calls = 0; sys.m_out = &out;
    m.vfunc_2(true);
    if (out.m_enabled || out.m_calls != 1) bad = 1;
    m.vfunc_2(false);
    if (!out.m_enabled || out.m_calls != 2) bad = 2;
    if (bad) { std::printf("00a3e3e0_TEST FAIL\n"); return 1; }
    std::printf("00a3e3e0_TEST PASS\n");
    return 0;
}