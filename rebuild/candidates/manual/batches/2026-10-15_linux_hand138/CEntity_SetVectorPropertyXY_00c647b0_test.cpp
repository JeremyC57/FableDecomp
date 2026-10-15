// Behaviour test for 0x00c647b0 CEntity::SetVectorProperty_XY
#include <cstdio>
struct SLevelRatioProperty { long Level; float Ratio; };
struct CEntityOwner { long LastError; };
struct CEntity {
    unsigned long m_dirtyFlags; CEntityOwner* m_pOwner; SLevelRatioProperty m_current; SLevelRatioProperty m_deferred;
    long SetVectorProperty_XY(const SLevelRatioProperty* value)
    {
        long level = value->Level;
        if (level < -10000 || level > 0 || value->Ratio < 0.0f || value->Ratio > 1.0f) { m_pOwner->LastError = -2; return -3; }
        if (level != m_current.Level || value->Ratio != m_current.Ratio) { m_current.Level = m_deferred.Level = level; m_current.Ratio = m_deferred.Ratio = value->Ratio; m_dirtyFlags |= 1; }
        return 0;
    }
};
int main()
{
    CEntityOwner o; o.LastError = 0;
    CEntity e; e.m_dirtyFlags = 0; e.m_pOwner = &o; e.m_current.Level = -500; e.m_current.Ratio = 0.5f; e.m_deferred.Level = 0; e.m_deferred.Ratio = 0;
    SLevelRatioProperty same = { -500, 0.5f }, bad1 = { -10001, 0.5f }, bad2 = { 1, 0.5f }, bad3 = { -1, 1.5f }, bad4 = { -1, -0.25f }, good = { -500, 0.5f };
    bool ok = e.SetVectorProperty_XY(&same) == 0 && e.m_dirtyFlags == 0;                    // unchanged: no dirty bit
    ok = ok && e.SetVectorProperty_XY(&bad1) == -3 && e.SetVectorProperty_XY(&bad2) == -3 && e.SetVectorProperty_XY(&bad3) == -3 && e.SetVectorProperty_XY(&bad4) == -3 && o.LastError == -2;
    e.m_current.Level = -1;
    ok = ok && e.SetVectorProperty_XY(&good) == 0 && e.m_current.Level == -500 && e.m_dirtyFlags == 1 && e.m_deferred.Level == -500 && e.m_deferred.Ratio == 0.5f;
    if (!ok) { std::printf("00c647b0_TEST FAIL\n"); return 1; }
    std::printf("00c647b0_TEST PASS\n");
    return 0;
}
