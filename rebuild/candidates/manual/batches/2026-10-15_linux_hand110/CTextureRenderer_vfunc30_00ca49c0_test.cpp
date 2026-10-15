// Behaviour test for 0x00CA49C0 CTextureRenderer::vfunc30
#include <cstdio>
struct CRendererPosPassThru {
    long m_resets;
    long ResetMediaTime() { ++m_resets; return 0; }
};
struct CTextureRenderer {
    long m_slot28Calls;
    CRendererPosPassThru* m_pPosition;
    virtual long vfunc28() { ++m_slot28Calls; return 5; }
    virtual long vfunc30()
    {
        if (m_pPosition)
            m_pPosition->ResetMediaTime();
        vfunc28();
        return 0;
    }
};
int main()
{
    CRendererPosPassThru pos; pos.m_resets = 0;
    CTextureRenderer r; r.m_slot28Calls = 0; r.m_pPosition = 0;
    bool ok = r.vfunc30() == 0 && r.m_slot28Calls == 1 && pos.m_resets == 0;
    r.m_pPosition = &pos;
    if (r.vfunc30() != 0 || r.m_slot28Calls != 2 || pos.m_resets != 1) ok = false;
    if (!ok) { std::printf("00ca49c0_TEST FAIL\n"); return 1; }
    std::printf("00ca49c0_TEST PASS\n");
    return 0;
}
