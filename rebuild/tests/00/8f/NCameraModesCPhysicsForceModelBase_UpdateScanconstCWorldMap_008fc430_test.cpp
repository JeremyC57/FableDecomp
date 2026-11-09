// Behaviour test for NCameraModes::CPhysicsForceModelBase::UpdateScan<const CWorldMap> 0x008FC430: per frame, for each
// latitude theta from StartTheta to 0.5 whose ring is due this frame (frame % (density / count) == 0, count from the
// 1 - 14.3 (theta - 0.25)^2 profile), find the ring whose key matches theta (else the last one), drop its oldest point
// when it already holds count points, and append the sphere point at longitude phase = (frame % density) / density.
#include <cstdio>
#include <cmath>
#include <list>
#include <utility>

struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V operator+(const V& o) const { return V(X + o.X, Y + o.Y, Z + o.Z); }
    V operator*(float s) const { return V(X * s, Y * s, Z * s); } };
static const V XAxis(1, 0, 0), YAxis(0, 1, 0), ZAxis(0, 0, 1);
static const float EPS = 1e-4f;
static long gFrame = 0;
struct Points {
    std::list<std::pair<float, std::list<V> > > SmartPointList;
    long ScanDensity; float StartTheta, DeltaTheta;
};
struct Model {
    long TriggerFrame;
    void UpdateScan(Points& points, const V& centre, float radius)
    {
        unsigned long frame = gFrame;
        long density = points.ScanDensity;
        long step = frame % density;
        TriggerFrame = frame;
        float phase = (float)step / density;
        if (TriggerFrame < (long)(frame + points.ScanDensity)) {
            for (float theta = points.StartTheta; theta <= EPS + 0.5f; theta += points.DeltaTheta) {
                long count = (long)((1.0f - (theta - 0.25f) * (theta - 0.25f) * 14.3f) * points.ScanDensity);
                if (frame % (points.ScanDensity / count) != 0) continue;
                std::list<std::pair<float, std::list<V> > >::iterator it = points.SmartPointList.begin();
                bool found = false;
                while (it != points.SmartPointList.end() && !found) {
                    float ringTheta = (*it++).first;
                    if (ringTheta > theta - EPS && ringTheta < EPS + theta) found = true;
                }
                --it;
                std::list<V>& ring = (*it).second;
                if (ring.size() == (unsigned long)count) ring.pop_front();
                V end = centre + YAxis * radius * ((cos(phase * 6.2831854820251465))) * (sin(theta * 6.2831854820251465))
                               + XAxis * radius * (sin(phase * 6.2831854820251465)) * (sin(theta * 6.2831854820251465))
                               + ZAxis * radius * ((cos(theta * 6.2831854820251465)));
                ring.push_back(end);
            }
        }
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    bool ok = true;
    Points p; p.ScanDensity = 4; p.StartTheta = 0.25f; p.DeltaTheta = 0.25f;   // rings at 0.25 and 0.5
    p.SmartPointList.push_back(std::make_pair(0.25f, std::list<V>()));
    p.SmartPointList.push_back(std::make_pair(0.5f, std::list<V>()));
    Model m; V c(1, 2, 3);
    // theta 0.25: count = 4 -> every frame; theta 0.5: count = (1 - 0.0625*14.3)*4 = 0.425 -> (long)0 ... guard density/count
    // keep theta 0.5 out of the loop for count 0: start/delta cover 0.25 only when DeltaTheta pushes past 0.5
    p.DeltaTheta = 0.3f;
    for (gFrame = 0; gFrame < 6; ++gFrame) m.UpdateScan(p, c, 2.0f);
    std::list<V>& ring = p.SmartPointList.front().second;
    if (ring.size() != 4 || m.TriggerFrame != 5) ok = false;
    // the newest point: frame 5 -> phase 0.25: cos = 0, sin = 1; theta 0.25: sin = 1, cos = 0 -> centre + X * 2
    const V& last = ring.back();
    if (!Near(last.X, 3.0f) || !Near(last.Y, 2.0f) || !Near(last.Z, 3.0f)) ok = false;
    // the oldest kept is frame 2 (frames 0 and 1 dropped): phase 0.5 -> cos = -1 -> centre - Y * 2
    const V& first = ring.front();
    if (!Near(first.X, 1.0f) || !Near(first.Y, 0.0f) || !Near(first.Z, 3.0f)) ok = false;
    if (!p.SmartPointList.back().second.empty()) ok = false;
    if (!ok) { std::printf("008fc430_TEST FAIL\n"); return 1; }
    std::printf("008fc430_TEST PASS\n");
    return 0;
}