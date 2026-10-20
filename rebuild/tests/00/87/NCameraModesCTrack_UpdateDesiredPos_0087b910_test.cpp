// Behaviour test for NCameraModes::CTrack::UpdateDesiredPos (0087b910): fraction blend and output position.
#include <cstdio>
struct V { float X, Y, Z; };
struct ShapeTrack {
    float nearestFraction;
    void GetTrackPosNearest(const V*, V* nearest, float* fraction) { V v = { 0, 0, 0 }; *nearest = v; *fraction = nearestFraction; }
    V GetTrackPosAtLengthFraction(float f) { V v = { f * 100.0f, 1, 2 }; return v; }
};
struct Point { long kind; V m_position; long GetKind() { return kind; } };
struct Mode {
    ShapeTrack* PTrack; float CurrentFractionAlongTrack; bool ZTargeting; Point* found; int refreshes;
    void Refresh() { ++refreshes; }
    void UpdateDesiredPos(V& desired)
    {
        Point* comp = found;
        V nearest; float nearestFraction;
        PTrack->GetTrackPosNearest(&comp->m_position, &nearest, &nearestFraction);
        float fraction = (CurrentFractionAlongTrack * 3.0f + nearestFraction) * 0.25f;
        Refresh();
        CurrentFractionAlongTrack = fraction;
        desired = PTrack->GetTrackPosAtLengthFraction(fraction);
        ZTargeting = false;
    }
};
int main()
{
    ShapeTrack st; st.nearestFraction = 1.0f;
    Point p; p.kind = 3;
    Mode m; m.PTrack = &st; m.CurrentFractionAlongTrack = 0.2f; m.ZTargeting = true; m.found = &p; m.refreshes = 0;
    V d = { 0, 0, 0 };
    m.UpdateDesiredPos(d);
    bool ok = m.CurrentFractionAlongTrack > 0.3999f && m.CurrentFractionAlongTrack < 0.4001f && d.X > 39.99f && d.X < 40.01f && d.Z == 2 && !m.ZTargeting && m.refreshes == 1;
    if (!ok) { std::printf("0087b910_TEST FAIL\n"); return 1; }
    std::printf("0087b910_TEST PASS\n");
    return 0;
}