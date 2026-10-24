// Behaviour test for 0x00761740 NShape::CTrack::IsPosInShape
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTrack {
    long m_points;
    float m_lineY;                                 // the track is the line y = m_lineY
    bool GetMinDistanceToTrack(const C3DVector& pos, float& distance) const
    {
        if (m_points < 2) return false;
        float d = pos.Y - m_lineY;
        distance = d < 0.0f ? -d : d;
        return true;
    }
    bool IsPosInShape(const C3DVector& pos) const
    {
        float distance;
        if (GetMinDistanceToTrack(pos, distance))
            return distance < 0.25f;
        else
            return false;
    }
};
int main()
{
    CTrack t; t.m_points = 3; t.m_lineY = 1.0f;
    C3DVector near = { 5.0f, 1.125f, 0.0f };
    C3DVector edge = { 5.0f, 1.25f, 0.0f };
    C3DVector far = { 0.0f, -2.0f, 0.0f };
    int ok = t.IsPosInShape(near) && !t.IsPosInShape(edge) && !t.IsPosInShape(far);
    t.m_points = 1;
    ok = ok && !t.IsPosInShape(near);
    if (!ok) { std::printf("00761740_TEST FAIL\n"); return 1; }
    std::printf("00761740_TEST PASS\n");
    return 0;
}