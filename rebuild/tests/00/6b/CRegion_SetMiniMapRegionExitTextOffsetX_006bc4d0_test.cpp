// Behaviour test for CRegion::SetMiniMapRegionExitTextOffsetX @ 0x006bc4d0: the named exit's offset X is set, the entry created on demand.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* m_text; };

class C2DVector { public: float X, Y; };

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

class CRegion { public:
    CRegionExitTextOffsetMap MiniMapRegionExitTextOffsets;
    void SetMiniMapRegionExitTextOffsetX(const CCharString& exitName, float x)
    {
        MiniMapRegionExitTextOffsets[exitName].X = x;
    }
};

int main()
{
    CRegion r;
    CCharString north = { "North" }, south = { "South" };
    r.SetMiniMapRegionExitTextOffsetX(north, 12.5f);
    r.SetMiniMapRegionExitTextOffsetX(south, -3.0f);
    r.SetMiniMapRegionExitTextOffsetX(north, 7.0f);
    if (r.MiniMapRegionExitTextOffsets.m_count != 2 || r.MiniMapRegionExitTextOffsets[north].X != 7.0f ||
        r.MiniMapRegionExitTextOffsets[south].X != -3.0f) { std::printf("006bc4d0_TEST FAIL\n"); return 1; }
    std::printf("006bc4d0_TEST PASS\n");
    return 0;
}