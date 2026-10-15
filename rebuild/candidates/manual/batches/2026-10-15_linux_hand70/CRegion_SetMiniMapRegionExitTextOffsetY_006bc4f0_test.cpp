// Behaviour test for CRegion::SetMiniMapRegionExitTextOffsetY @ 0x006bc4f0: the named exit's offset Y is set, the entry created on demand.
#include <cstdio>
#include <cstring>

struct CCharString { const char* m_text; };

struct C2DVector { float X, Y; };

struct CRegionExitTextOffsetMap {
    const char* m_keys[8]; C2DVector m_values[8]; int m_count;
    CRegionExitTextOffsetMap() : m_count(0) {}
    C2DVector& operator[](const CCharString& key)
    {
        for (int i = 0; i < m_count; ++i)
            if (std::strcmp(m_keys[i], key.m_text) == 0) return m_values[i];
        m_keys[m_count] = key.m_text; m_values[m_count].X = 0.0f; m_values[m_count].Y = 0.0f;
        return m_values[m_count++];
    }
};

struct CRegion {
    CRegionExitTextOffsetMap MiniMapRegionExitTextOffsets;
    void SetMiniMapRegionExitTextOffsetY(const CCharString& exitName, float y)
    {
        MiniMapRegionExitTextOffsets[exitName].Y = y;
    }
};

int main()
{
    CRegion r;
    CCharString north = { "North" }, south = { "South" };
    r.SetMiniMapRegionExitTextOffsetY(north, 12.5f);
    r.SetMiniMapRegionExitTextOffsetY(south, -3.0f);
    r.SetMiniMapRegionExitTextOffsetY(north, 7.0f);
    if (r.MiniMapRegionExitTextOffsets.m_count != 2 || r.MiniMapRegionExitTextOffsets[north].Y != 7.0f ||
        r.MiniMapRegionExitTextOffsets[south].Y != -3.0f) { std::printf("006bc4f0_TEST FAIL\n"); return 1; }
    std::printf("006bc4f0_TEST PASS\n");
    return 0;
}
