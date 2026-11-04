// Behaviour test for NCameraModes::CTrack::OnInitLeaf() (0x0087B840): a model of the row -- the shape manager's best track is
// stored in PTrack; only when it has one is the fraction along the track set from the point nearest the track camera point.
#include <stdio.h>
struct Vec { float x; };
struct Track { float GetFraction(const Vec& p) const { return p.x * 0.5f; } };
struct ShapeManager { const Track* best; bool GetBestTrack(const Track** t) const { if (!best) return false; *t = best; return true; } };
struct Mode {
    ShapeManager* shapes; Vec dragCamPos; const Track* PTrack; float Fraction;
    void OnInitLeaf()
    {
        if (shapes->GetBestTrack(&PTrack))
            Fraction = PTrack->GetFraction(dragCamPos);
    }
};
int main()
{
    Track t; ShapeManager sm = { &t };
    Mode m = { &sm, { 3.0f }, 0, -1.0f };
    m.OnInitLeaf();
    int ok = m.PTrack == &t && m.Fraction == 1.5f;
    sm.best = 0; m.Fraction = -1.0f; m.OnInitLeaf();
    ok = ok && m.PTrack == &t && m.Fraction == -1.0f;
    printf(ok ? "0087b840_TEST PASS\n" : "0087b840_TEST FAIL\n");
    return ok ? 0 : 1;
}