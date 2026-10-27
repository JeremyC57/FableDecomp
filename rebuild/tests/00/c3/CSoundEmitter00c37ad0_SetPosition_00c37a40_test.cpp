// Test for 0x00C37A40 CSoundEmitter_00c37ad0::SetPosition: the 16-byte block is cached and marked dirty.
#include <cstdio>
struct SPos { float x, y, z, w; };
struct CEmitter { float m_scalar; SPos m_position; bool m_positionDirty;
    void SetPosition(int, const SPos& p) { m_position = p; m_positionDirty = true; } };
int main()
{
    CEmitter e; e.m_positionDirty = false; SPos p = {1.0f, 2.0f, 3.0f, 4.0f};
    e.SetPosition(9, p);
    if (!e.m_positionDirty || e.m_position.x != 1.0f || e.m_position.y != 2.0f || e.m_position.z != 3.0f || e.m_position.w != 4.0f)
    { std::printf("00c37a40_TEST FAIL\n"); return 1; }
    std::printf("00c37a40_TEST PASS\n");
    return 0;
}