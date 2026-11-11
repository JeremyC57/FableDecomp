// Behaviour test for CNavigatorAStar::CalculateNavigableWidthOfLine @ 0x00A1DE30: same logic over a model route / steering agent.
#include <cstdio>

class C2DVector { public: float X, Y; };
class CNavWayPoint { public: C2DVector Position; unsigned char NavLayer; bool Removable; };

class CSteeringAgent { public:
    float radius;
    float CurrentMaxSpeed;
    float BestDistLeft, BestDistRight, BestNextDistLeft, BestNextDistRight;
    float ProximityForArrival;
    float GetRadius(bool) const { return radius; }
    float& GetBestDistLeft() { return BestDistLeft; }
    float& GetBestDistRight() { return BestDistRight; }
    float& GetBestNextDistLeft() { return BestNextDistLeft; }
    float& GetBestNextDistRight() { return BestNextDistRight; }
    float GetCurrentMaxSpeed() const { return CurrentMaxSpeed; }
    void SetProximityForArrival(float p) { ProximityForArrival = p; }
};

class CNavigatorAStar { public:
    CSteeringAgent* SteeringAgent;
    CNavWayPoint route[4];
    int count;
    float legWidths[3][2];      // widths of leg i (0 = start -> first)
    int calls;
    unsigned char startLayer;
    void CalculateNavigableWidthOfLine(const CNavWayPoint& from, const CNavWayPoint&, float& l, float& r, bool)
    {
        int leg = (&from == &route[0]) ? 1 : 0;
        startLayer = from.NavLayer;
        l = legWidths[leg][0]; r = legWidths[leg][1]; ++calls;
    }
    void CalculateNavigableWidthOfLine(const C2DVector& position)
    {
        CNavWayPoint start = { position, route[0].NavLayer, true };
        CNavWayPoint* first = &route[0];
        CNavWayPoint* second = count > 1 ? &route[1] : 0;
        float& bestDistLeft = SteeringAgent->GetBestDistLeft();
        float& bestDistRight = SteeringAgent->GetBestDistRight();
        float& bestNextDistLeft = SteeringAgent->GetBestNextDistLeft();
        float& bestNextDistRight = SteeringAgent->GetBestNextDistRight();
        CalculateNavigableWidthOfLine(start, *first, bestDistLeft, bestDistRight, false);
        if (second) {
            CalculateNavigableWidthOfLine(*first, *second, bestNextDistLeft, bestNextDistRight, false);
            float narrowest = bestDistLeft;
            if (bestDistRight < narrowest) narrowest = bestDistRight;
            if (bestNextDistLeft < narrowest) narrowest = bestNextDistLeft;
            if (bestNextDistRight < narrowest) narrowest = bestNextDistRight;
            float width = narrowest - SteeringAgent->GetRadius(false);
            SteeringAgent->SetProximityForArrival(width < SteeringAgent->GetCurrentMaxSpeed() ? SteeringAgent->GetCurrentMaxSpeed() : width);
        } else
            SteeringAgent->SetProximityForArrival(SteeringAgent->GetCurrentMaxSpeed());
    }
};

int main()
{
    CSteeringAgent agent = { 0.5f, 1.0f, 0, 0, 0, 0, 0 };
    CNavigatorAStar nav;
    nav.SteeringAgent = &agent;
    nav.route[0].NavLayer = 2;
    nav.legWidths[0][0] = 5.0f; nav.legWidths[0][1] = 4.0f;    // start leg
    nav.legWidths[1][0] = 3.0f; nav.legWidths[1][1] = 6.0f;    // next leg
    C2DVector p = { 1.0f, 2.0f };
    bool ok = true;
    nav.count = 2; nav.calls = 0;
    nav.CalculateNavigableWidthOfLine(p);
    ok = ok && nav.calls == 2 && agent.BestDistLeft == 5.0f && agent.BestNextDistLeft == 3.0f && agent.ProximityForArrival == 2.5f;
    agent.radius = 2.5f;                          // 3 - 2.5 = 0.5 < 1 -> clamped
    nav.CalculateNavigableWidthOfLine(p);
    ok = ok && agent.ProximityForArrival == 1.0f;
    nav.count = 1; nav.calls = 0; agent.ProximityForArrival = 0;
    nav.CalculateNavigableWidthOfLine(p);
    ok = ok && nav.calls == 1 && agent.ProximityForArrival == 1.0f;
    if (!ok) { std::printf("00a1de30_TEST FAIL\n"); return 1; }
    std::printf("00a1de30_TEST PASS\n");
    return 0;
}